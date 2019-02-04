#define USE_PNG 1

#include "common.h"
#include "persePsd.h"
#include <memory>
#include <vector>
#include <string>
#include "helper.h"


#define DEBUG_VERBOSE 0
#define DEBUG_PREVIEW 0


#include "../ItsyPSD/itsypsd.h"


void	blendLayer(psd::layer* dst, psd::layer* src)
{

#if DEBUG_VERBOSE
	std::cerr << dst->name.back() << " width:" << dst->width << " height:" << dst->height << " left:" << dst->left << " right:" << dst->right << " top:" << dst->top << " bottom:" << dst->bottom << std::endl;
	std::cerr << src->name.back() << " width:" << src->width << " height:" << src->height << " left:" << src->left << " right:" << src->right << " top:" << src->top << " bottom:" << src->bottom << std::endl;
#endif

	size_t dstsize = dst->pixels.size();
	size_t srcsize = src->pixels.size();

	float op = (src->opacity / 255.0f);

	for (size_t i = 0; i < dstsize; i++)
	{
		uint32_t dst_pix = dst->pixels[i];
		uint32_t src_pix = src->pixels[i];

		if (dst_pix != 0 && src_pix != 0)
		{
			int a = 0;
		}

		int dst_pix_a = (dst_pix & 0xff000000) >> 24;
		int dst_pix_b = (dst_pix & 0x00ff0000) >> 16;
		int dst_pix_g = (dst_pix & 0x0000ff00) >> 8;
		int dst_pix_r = (dst_pix & 0x0ff);

		int src_pix_a = (src_pix & 0xff000000) >> 24;
		int src_pix_b = (src_pix & 0x00ff0000) >> 16;
		int src_pix_g = (src_pix & 0x0000ff00) >> 8;
		int src_pix_r = (src_pix & 0x0ff);
		if (src_pix_a == 0)continue;

		int ba = dst_pix_a;
		//int ba = (dst_pix_a + (src_pix_a - dst_pix_a) * op);
		int bb = (dst_pix_b + (src_pix_b - dst_pix_b) * op);
		int bg = (dst_pix_g + (src_pix_g - dst_pix_g) * op);
		int br = (dst_pix_r + (src_pix_r - dst_pix_r) * op);

		dst->pixels[i] = ba << 24 | bb << 16 | bg << 8 | br;
	}

}

bool  persePsdMain(std::string inputname,
	int addpri,
	int &readpngfile_max,
	tPoint2i&	layoutCenter,
	psdBitMapLayer* pbl) 
{

	psd _psd;

	if (!_psd.load(inputname)) return false;

	layoutCenter.x = _psd.width / 2;		//ビルドするためにとりあえず数値を入れておく本来はpngファイルから取得する
	layoutCenter.y = _psd.height / 2;

	std::string inputlayerfile_temp[PARTS_MAX];
	UserBitMapData*	readbitmap_temp[PARTS_MAX];

	//各レイヤー画像のプレビュー出力
#if DEBUG_PREVIEW

	for (auto& n : _psd.layers)
	{
		psd::layer* layer = n.get();

		std::string name = "";
		for (size_t i = 0; i <  layer->name.size(); i++ )
		{
			name = name + layer->name[i];
			name = name + "_";
		}
		name = name + ".png";

		RGBAImage image( layer->width , layer->height, (unsigned char*)layer->pixels.data());
		//png保存

		std::string path = "c:\\temp\\";
		image.write_png( (path + name).c_str() );
		//image.write_ppm((path + name).c_str());
	}
#endif


	
	//レイヤーグループ化
	struct groupLayer{
		std::string baseName;
		std::vector<psd::layer*> child_layer;
		psd::layer* blendLayer;
		groupLayer() : baseName(""){}
	};

	std::vector<groupLayer> _groupLayer;

	groupLayer currentGroup;

	for (auto& n : _psd.layers)
	{
		psd::layer* layer = n.get();

		if (currentGroup.baseName == "")
		{
			currentGroup.baseName = layer->name[0];
			currentGroup.child_layer.push_back(layer);
			currentGroup.blendLayer = layer;
			continue;
		}
		
		if (currentGroup.baseName != "" &&
			currentGroup.baseName != layer->name[0]
			)
		{			
			_groupLayer.push_back(currentGroup);
			currentGroup.child_layer.clear();
			currentGroup.baseName = layer->name[0];
			currentGroup.child_layer.push_back(layer);
		}
		else if (currentGroup.baseName == layer->name[0]){
			currentGroup.child_layer.push_back(layer);
		}
	}

	_groupLayer.push_back(currentGroup);
	psd::layer* l = currentGroup.child_layer.back();
	currentGroup.blendLayer = l;



	for (size_t i = 0 ; i < _groupLayer.size() ; i ++)
	{
		groupLayer& currentGroup = _groupLayer[i];

		//合成
		psd::layer* l = currentGroup.child_layer.back();
		currentGroup.child_layer.pop_back();
		currentGroup.blendLayer = l;

		while (currentGroup.child_layer.size() != 0)
		{
			psd::layer* l = currentGroup.child_layer.back();
			babel::bbl_string  name = l->name.back();
			name = babel::sjis_to_utf8(name);

			currentGroup.child_layer.pop_back();
			if (l->hide || name.find("@") != babel::bbl_string::npos)
			{
				//名前に@が含まれている場合はセルに含めない
				std::cerr << " skip (" << name << ")" << std::endl;
			}
			else {
				std::cerr << " read (" << name << ")" << std::endl;
				blendLayer(currentGroup.blendLayer, l);
			}
		}

		psd::layer* layer = _groupLayer[i].blendLayer;
		//psd::layer* layer = n.get();

		std::string layer_path = "";
		babel::bbl_string  name = _groupLayer[i].baseName;

		name = babel::sjis_to_utf8(name);


		UserBitMapData*	bitmap = new UserBitMapData();

		bitmap->Width = layer->right - layer->left;
		bitmap->Height = layer->bottom - layer->top;

		bitmap->Data = (int*)malloc( sizeof(int) * ( bitmap->Width) * (bitmap->Height) );

		for (int y = layer->top; y < layer->bottom; y++)
		{
			for (int x = layer->left; x < layer->right ; x++)
			{
				if (y >= layer->height) continue;

				uint32_t	pix = layer->pixels[y * layer->width + x];
				if (pix != 0)
				{
					uint32_t _a = (pix & 0xff000000) >> 24;
					uint32_t _r = (pix & 0x00ff0000) >> 16;
					uint32_t _g = (pix & 0x0000ff00) >> 8;
					uint32_t _b = (pix & 0x0ff);

					pix = _a << 24 | _b << 16 | _g << 8 | _r;
				}

//				int yy = (bitmap->Height-1) - (y - layer->top);
				int yy = (y - layer->top);
				int xx = x - layer->left;
				int n = bitmap->Width * yy + xx;
				bitmap->Data[n] = pix;				
			}
		}

		layer->width = bitmap->Width;
		layer->height = bitmap->Height;

		bitmap->Pitch = bitmap->Width * 4;// psdのレイヤー情報から判別しないといけないかも
		bitmap->offset_x = layer->left;
		bitmap->offset_y = layer->top;
		bitmap->orgX = 0;
		bitmap->orgY = 0;
		bitmap->orgWidth = layer->width;
		bitmap->orgHeight = layer->height;
		bitmap->pri = 0;

		if (
			(layer->hide == true) //レイヤー非表示の時は登録しない
			|| (name.find("@") != babel::bbl_string::npos)
			)
		{
			//名前に@が含まれている場合はセルに含めない
			std::cerr << " skip (" << name << ")" << std::endl;
		}
		else
		{
			std::cerr << " read (" << name << ")" << std::endl;

			inputlayerfile_temp[readpngfile_max] = name;
			readbitmap_temp[readpngfile_max] = bitmap;
			readpngfile_max++;
		}
	}

	int i;
	for (i = 0; i < readpngfile_max; i++)
	{
		std::string str = Replace(inputlayerfile_temp[i], ".", "_");
		pbl->inputlayerfile[i] = str + ".png";
		pbl->inputlayername[i] = str;
		pbl->readbitmap[i] = readbitmap_temp[i];
	}

	return true;
}

