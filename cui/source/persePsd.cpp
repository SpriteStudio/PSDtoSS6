#define USE_PNG 1

#include "common.h"
#include "persePsd.h"
#include <memory>
#include <vector>
#include <string>
#include "helper.h"

#include "stringconv.h"


#define DEBUG_VERBOSE 0
#define DEBUG_PREVIEW 0


#include <libpsd.h>

bool  persePsdMain(std::string inputname,
	int addpri,
	int &readpngfile_max,
	tPoint2i&	layoutCenter,
	psdBitMapLayer* pbl)
{
	psd_context *context;
	psd_status status;
	psd_context *dstcontext;

	//PSDからパーツリストを作成
	char psdname[255];
	strcpy(psdname, inputname.c_str());
	status = psd_image_load(&context, psdname);

	if (status != psd_status_done)
		return false;

	dstcontext = (psd_context*)malloc(sizeof(psd_context));
	memcpy(dstcontext, context, sizeof(psd_context));


	//PSDから画像イメージを取得
	char path[1024];
	int write_count = 1;
	int datacount = 0;

	bool	is_ingroup = false;
	int		in_group_nest = 0;

	std::vector<psd_layer_record*>	layer_group_list;
	std::string inputlayerfile_temp[PARTS_MAX];
	UserBitMapData*	readbitmap_temp[PARTS_MAX];


	layoutCenter.x = context->width / 2;		//ビルドするためにとりあえず数値を入れておく本来はpngファイルから取得する
	layoutCenter.y = context->height / 2;

	int readcount = 0;

	psd_layer_record *end = context->layer_records + context->layer_count;
	for (psd_layer_record* layer = context->layer_records; layer != end; ++layer)
	{

		//sjisで格納されている
		std::string name = (char*)layer->layer_name;
		name = stringconv::sjis_to_utf8(name);
		int layertype = LAYERTYPE_NORMAL;

		// each layer
		if (layer->layer_type != psd_layer_type_normal || in_group_nest > 0)
		{
			if (layer->layer_type == psd_layer_type_hidden)
			{
				//グループの開始
				if (
					(strcmp((char*)layer->layer_name, "</Layer group>") == 0)
					|| (strcmp((char*)layer->layer_name, "</Layer set>") == 0)
					)
				{
					//						std::cerr << " group start(" << layer->layer_name << ")" << std::endl;
					is_ingroup = true;
					in_group_nest++;
					continue;
				}
			}


			if (layer->layer_type == psd_layer_type_folder)
			{
				//					std::cerr << " group end(" << layer->layer_name << ")" << std::endl;
				is_ingroup = false;
				in_group_nest--;

				if (in_group_nest == 0)
				{
					//作成したレイヤーグループをコピー
					dstcontext->layer_count = layer_group_list.size();
					dstcontext->layer_records = (psd_layer_record*)malloc(sizeof(psd_layer_record) * dstcontext->layer_count);
					for (size_t i = 0; i < dstcontext->layer_count; i++)
					{
						memcpy(&dstcontext->layer_records[i], layer_group_list[i], sizeof(psd_layer_record));
					}

					//試しにブレンドしてみる
					dstcontext->blending_image_data = NULL;
					psd_image_blend(dstcontext, 0, 0, context->width, context->height);

					{
						RGBAImage textimage(dstcontext->width, dstcontext->height,
							(unsigned char*)dstcontext->blending_image_data);

						UserBitMapData*	bitmap = new UserBitMapData();
						bitmap->Data = (int*)dstcontext->blending_image_data;


						bitmap->Width = dstcontext->width;
						bitmap->Height = dstcontext->height;
						bitmap->Pitch = dstcontext->width * 4;

						psd_layer_record* current_layer = &dstcontext->layer_records[0];

						bitmap->orgX = current_layer->left;
						bitmap->orgY = current_layer->top;
						bitmap->orgWidth = current_layer->right - current_layer->left;
						bitmap->orgHeight = current_layer->bottom - current_layer->top;
						bitmap->offset_x = 0;
						bitmap->offset_y = 0;
						if (addpri != 0)	//プライオリティを設定する
						{
							bitmap->pri = (readpngfile_max + 1) * addpri;
						}

						if (
							(layer->visible == psd_false) //レイヤー非表示の時は登録しない
							|| (name.find("@") != std::string::npos)
							)
						{
							//名前に@が含まれている場合はセルに含めない
							std::cerr << " skip (" << layer->layer_name << ")" << std::endl;
						}
						else
						{
							std::cerr << " read (" << layer->layer_name << ")" << std::endl;
							inputlayerfile_temp[readpngfile_max] = name;
							readbitmap_temp[readpngfile_max] = bitmap;
							readpngfile_max++;
						}
						layer_group_list.clear();
					}
				}
				continue;
			}


			layer_group_list.push_back(layer);
			continue;
		}
		if (layer->number_of_channels == 3) continue; //背景とみなす


		UserBitMapData*	bitmap = new UserBitMapData();
		bitmap->Data = (int*)layer->image_data;
		bitmap->Width = layer->width;
		bitmap->Height = layer->height;
		bitmap->Pitch = layer->width * 4;// psdのレイヤー情報から判別しないといけないかも
		bitmap->offset_x = layer->left;
		bitmap->offset_y = layer->top;

		bitmap->orgX = 0;
		bitmap->orgY = 0;
		bitmap->orgWidth = layer->width;
		bitmap->orgHeight = layer->height;
		bitmap->pri = 0;

		if (
			(layer->visible == psd_false) //レイヤー非表示の時は登録しない
			|| (name.find("@") != std::string::npos)
			)
		{
			//名前に@が含まれている場合はセルに含めない
			std::cerr << " skip (" << layer->layer_name << ")" << std::endl;
		}
		else
		{
			std::cerr << " read (" << layer->layer_name << ")" << std::endl;
			inputlayerfile_temp[readpngfile_max] = name;
			readbitmap_temp[readpngfile_max] = bitmap;
			readpngfile_max++;
		}
	}
	//順番を逆にする
	int i;
	for (i = 0; i < readpngfile_max; i++)
	{
		std::string str = Replace(inputlayerfile_temp[readpngfile_max - i - 1], ".", "_");
		pbl->inputlayerfile[i] = str + ".png";
		pbl->inputlayername[i] = str;
		pbl->readbitmap[i] = readbitmap_temp[readpngfile_max - i - 1];
	}


	return true;
}