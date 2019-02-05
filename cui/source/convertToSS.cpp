#define USE_PNG 1

#include "convertToSS.h"
#include "persePsd.h"
#include "xml_template.h"
#include "../babel/babel.h"



using namespace SsConverter;


//paramにもっていくのが適正か調べる
bool	ConvertToSS::parseInputOutputFiles(std::string arg)
{

	inputname = arg;

	//入力psdファイル名
	int start = inputname.rfind(".psd");
	if (std::string::npos == start)
	{
		//拡張子なし
		is_psdload = false;
	}
	else
	{
		is_psdload = true;
	}
	inputname = inputname;

	//出力ファイル名
	start = inputname.find_last_of("\\");
	int end = inputname.find_last_of(".");
	if (std::string::npos == start)
	{
		start = inputname.find_last_of("/");
	}

	params.outputname = inputname.substr(start + 1, end - start - 1);

	//インプット、アウトプットパス
	end = inputname.find_last_of("\\");
	if (std::string::npos == end)
	{
		end = inputname.find_last_of("/");
	}

	inputpath = inputname.substr(0, end + 1);
	params.outputpath = inputpath;

	return true;
}


bool	ConvertToSS::getInfomationFilePath()
{
	//std::string convert_info;

	convert_info_path = get_documents_path();
	if (convert_info_path == "") return false;
	convert_info_path += TOOL_DOCUMENT_FOLDER;
	convert_info_path += "convert_info";

	return true;

}


//あらかじめ作られているファイルリストからpsdファイルを読み込む
PsdLoadErr		ConvertToSS::LoadPsdImageList()
{
	char readline[256 * PARTS_MAX]; //パーツ分の領域を確保
	std::string readstr;
	bool err = false;

	//テキストファイルからレイヤー順とファイル名を取得
	FILE* fp = fopen(inputname.c_str(), "r");
	if (fp == NULL) {
		std::cerr << "エラー：情報ファイルのオープンに失敗しました。：" << inputname << "\n";
		return SsConverter::OPENFAILED;
	}
	// ファイルの終端まで文字を読み取る
	while (fgets(readline, 256 * PARTS_MAX, fp) != NULL)
	{
		readstr = readline;
		//ファイル名は#で区切られている
		{
			int start = 0;
			int count = 0;
			while (true)
			{
				int next = readstr.find("#", start);
				if (std::string::npos == next)
				{
					//検索終わり
					if (count == 0)
					{
						std::cerr << "エラー：情報ファイルの内容が正しくありません。：" << inputname << "\n";
						err = true;
						return SsConverter::TEXTINVALID;
					}
					break;
				}
				else
				{
					count++;
					std::string work = readstr.substr(start, next - start);

					pb.inputlayerfile[readpngfile_max] = work;
					pb.inputlayername[readpngfile_max] = work.substr(0, work.length() - 4);
					start = next + 1;


					std::string pngpath = inputpath + pb.inputlayerfile[readpngfile_max];
					RGBAImage* p_readimage;
					//png読み込み
					p_readimage = new RGBAImage(0, 0);
					std::cerr << "read image : " << pngpath << std::endl;
					if (p_readimage->read_png(pngpath.c_str()) == true)
					{
						//			std::cout << "read png : " << pngpath << std::endl;
						std::cerr << "OK!" << std::endl;
					}
					else
					{
						std::cerr << "ERROR" << std::endl;
						err = true;
						return SsConverter::PNGLOADERR;
					}
					//パッカーへプッシュする
					UserBitMapData*	bitmap = new UserBitMapData();
					bitmap->Data = (int*)p_readimage->get();
					bitmap->Width = p_readimage->getWidth();
					bitmap->Height = p_readimage->getHeight();
					bitmap->Pitch = p_readimage->getWidth() * 4;
					bitmap->offset_x = 0;
					bitmap->offset_y = 0;

					bitmap->orgX = 0;
					bitmap->orgY = 0;
					bitmap->orgWidth = p_readimage->getWidth();
					bitmap->orgHeight = p_readimage->getHeight();
					bitmap->pri = 0;
					//ssae用のセンター位置を取得（PSDから作られている場合はサイズはどの画像も同じはず）
					gLayoutCenter.x = p_readimage->getWidth() / 2;		//ビルドするためにとりあえず数値を入れておく本来はpngファイルから取得する
					gLayoutCenter.y = p_readimage->getHeight() / 2;

					pb.readbitmap[readpngfile_max] = bitmap;

					readpngfile_max++;
				}

			}
		}
	}


	return SsConverter::ErrorNone;

}


bool	ConvertToSS::loadImageFile()
{
	if (this->isPsdMode()){
		if (!persePsdMain(inputname, params.addpri, readpngfile_max, gLayoutCenter, &pb))
		{
			std::cerr << "エラー：PSDファイルのオープンに失敗しました。：" << inputname << "\n";
			return false;
		}
	}else{
		if (!LoadPsdImageList() == SsConverter::ErrorNone)
			return false;
	}

	return true;
}

//無効なレイヤー名がないかチェックする
bool	ConvertToSS::checkInvalidLayerName()
{
	for (int i = 0; i < readpngfile_max; i++)
	{
		//全角チェック
		babel::bbl_string  name = pb.inputlayername[i].c_str();
		name = babel::utf8_to_sjis(name);
		if (isZenkaku(name) == true)
		{
			std::cerr << "エラー：セル名に全角文字が使用されています。 ";
			std::cerr << name << "\n";
			return false;
		}
		for (int j = i + 1; j < readpngfile_max; j++)
		{
			if (i != j)
			{
				if (pb.inputlayername[i] == pb.inputlayername[j])
				{
					std::cerr << "エラー：同名のセル名が使用されています。 ";
					std::cerr << pb.inputlayername[i] << "\n";
					return false;
				}
			}
		}
	}
	return true;
}


void	ConvertToSS::addPriority()
{

	//プライオリティ設定
	if (params.addpri != 0)	//プライオリティを設定する
	{
		int cnt = 0;
		for (int i = readpngfile_max - 1; i >= 0; i--)
		{
			int layertype = LAYERTYPE_NORMAL;
			//原点用レイヤーはパッキングに含まない
			if (pb.inputlayername[i].find("+") == 0)
			{
				//原点指定レイヤー
				//先頭文字が"+"の場合はパッキングしない
				layertype = LAYERTYPE_PIVOT;
			}
			if (pb.inputlayername[i] == "root")
			{
				//rootパーツ指定レイヤー
				layertype = LAYERTYPE_ROOT;
			}
			if (layertype == LAYERTYPE_NORMAL)
			{
				cnt += params.addpri;
			}
			pb.readbitmap[i]->pri = cnt;
		}
	}
}

void ConvertToSS::loadssae()
{
	if (tinyxml2::XML_NO_ERROR != loadssae_xml.LoadFile(ssaename.c_str()))
	{
		//読み込み失敗
		//新規作成
	}
	else
	{
		if (params.is_overwrite == false)
		{
			is_ssaeload = true;
		}
	}

	// output
	FILE* fp = fopen(sspjname.c_str(), "r");
	if (fp == NULL)
	{
	}
	else
	{
		if (params.is_overwrite == false)
		{
			is_sspjload = true;	//ファイルは存在する
		}
		fclose(fp);
	}

}


bool    ConvertToSS::texturePacking_PivotUse()
{
	is_ssceload = false;
	//ssceを読み込みpivotのリストを作製する。
	//同じセル名が存在する場合は読み込んだpivotを反映させる
	XMLDocument loadssce_xml;
	if (tinyxml2::XML_NO_ERROR != loadssce_xml.LoadFile(sscename.c_str()))
	{
		//ファイルが無いため新規作成とする
		return true;
	}
	else
	{
		is_ssceload = true;

		//ssecからpivotのリストを作製する
		XMLElement* ssce_root = loadssce_xml.FirstChildElement("SpriteStudioCellMap");

		//cellsを取得
		XMLElement* ssce_cells = ssce_root->FirstChildElement("cells");
		//cellを取得
		int idx = 0;
		for (XMLElement* ssce_cell = ssce_cells->FirstChildElement("cell"); ssce_cell != NULL; ssce_cell = ssce_cell->NextSiblingElement("cell"))
		{
			//pivotを取得
			XMLElement* cell_name = ssce_cell->FirstChildElement("name");
			XMLElement* cell_pivot = ssce_cell->FirstChildElement("pivot");

			pivot[idx].name = cell_name->GetText();
			std::string pivot_text = cell_pivot->GetText();
			int st = pivot_text.find(" ");	//区切り文字の位置を取得
			std::string pivot_x_text = pivot_text.substr(0, st);
			std::string pivot_y_text = pivot_text.substr(st + 1);

			sscanf(pivot_x_text.c_str(), "%f", &pivot[idx].x);
			sscanf(pivot_y_text.c_str(), "%f", &pivot[idx].y);

			//meshを取得
			mesh[idx].set_name(cell_name);
			XMLElement* cell_ismesh = ssce_cell->FirstChildElement("ismesh");
			if (cell_ismesh != NULL)
				mesh[idx].set_ismesh(cell_ismesh);
			XMLElement* cell_divtype = ssce_cell->FirstChildElement("divtype");
			if (cell_divtype != NULL)
				mesh[idx].set_divtype(cell_divtype);
			XMLElement* cell_divw = ssce_cell->FirstChildElement("divw");
			if (cell_divw != NULL)
				mesh[idx].set_divw(cell_divw);
			XMLElement* cell_divh = ssce_cell->FirstChildElement("divh");
			if (cell_divh != NULL)
				mesh[idx].set_divh(cell_divh);
			XMLElement* cell_innerPoint = ssce_cell->FirstChildElement("innerPoint");
			if (cell_innerPoint != NULL)
				mesh[idx].set_pvInnerPoint(cell_innerPoint);
			XMLElement* cell_outerPoint = ssce_cell->FirstChildElement("outerPoint");
			if (cell_outerPoint != NULL)
				mesh[idx].set_pvOuterPoint(cell_outerPoint);
			XMLElement* cell_meshPointList = ssce_cell->FirstChildElement("meshPointList");
			if (cell_meshPointList != NULL)
				mesh[idx].set_pvMeshPointList(cell_meshPointList);
			XMLElement* cell_meshTriList = ssce_cell->FirstChildElement("meshTriList");
			if (cell_meshTriList != NULL)
				mesh[idx].set_pvMeshTriList(cell_meshTriList);

			SSSpriteSheetPrim p;
			//読み込んだセルのnameを保存
			p.name = cell_name->GetText();
			//読み込んだセルのposを保存
			XMLElement* cell_pos = ssce_cell->FirstChildElement("pos");
			std::string pos_text = cell_pos->GetText();
			st = pos_text.find(" ");	//区切り文字の位置を取得
			std::string pos_x_text = pos_text.substr(0, st);
			std::string pos_y_text = pos_text.substr(st + 1);
			sscanf(pos_x_text.c_str(), "%d", &p.img_left);
			sscanf(pos_y_text.c_str(), "%d", &p.img_top);

			//メッシュの座標を、原点がセルの中心になるように、変換する
			XMLElement* cell_size = ssce_cell->FirstChildElement("size");
			std::string size_text = cell_size->GetText();
			st = size_text.find(" ");	//区切り文字の位置を取得
			std::string size_x_text = size_text.substr(0, st);
			std::string size_y_text = size_text.substr(st + 1);
			tPoint2i center;
			center.x = std::stoi(size_x_text);
			center.y = std::stoi(size_y_text);
			//座標変換 原点:左上->中心
			mesh[idx].trans_coord(center, 0);
			mesh[idx].coord_origin = MESH::COORD_ORIGIN_CENTER;

			//LAYERTYPE_DELETEで初期化
			p.layertype = LAYERTYPE_DELETE;

			ssce_cell_list.push_back(p);

			idx++;
			celldata_num++;
		}

		//読み込んだセルを以下3種類に分類
		//1. LAYERTYPE_NORMAL
		//2. LAYERTYPE_COPY
		//3. LAYERTYPE_DELETE
		for (auto origin = ssce_cell_list.begin(); origin != ssce_cell_list.end(); origin++)
		{
			//複製セルを検索
			for (auto copy = origin; copy != ssce_cell_list.end(); copy++)//開始位置に注意
			{
				if (origin == copy)
				{
					//複製セルのうち一つだけはLAYERTYPE_DELETEのままにしておく
					//後でこのセルに対して透明領域を作成する
				}
				//originと同じposを参照するセルは複製セル
				else if ((origin->img_left == copy->img_left) && (origin->img_top == copy->img_top))
				{
					copy->layertype = LAYERTYPE_COPY;
				}
			}
		}
		//PSDに存在するセルはLAYERTYPE_NORMAL
		int num = packer.GetPrimitiveNum();
		//PSDから読み込んだセルを走査
		for (int i = 0; i < num; i++)
		{
			SSSpriteSheetPrim layer = packer.GetPrimitiveInfo(i);
			//ssceからPSDと同名のセルを検索
			for (auto origin = ssce_cell_list.begin(); origin != ssce_cell_list.end(); origin++)
			{
				if (layer.name == origin->name)
				{
					//PSDのレイヤーと同名のセルはLAYERTYPE_NORMALであることを担保する
					origin->layertype = LAYERTYPE_NORMAL;
					break;
				}
			}
		}

		//同じUV座標を参照するセルをグルーピングする
		for (auto origin = ssce_cell_list.begin(); origin != ssce_cell_list.end(); origin++)
		{
			//複製セルを検索
			for (auto copy = ssce_cell_list.begin(); copy != ssce_cell_list.end(); copy++)
			{
				if (origin == copy)
				{
					//自分自身は複製ではない
				}
				//originと同じposを参照するセルは複製セル
				else if ((origin->img_left == copy->img_left) && (origin->img_top == copy->img_top)
					&& (copy->layertype != LAYERTYPE_NORMAL) && (copy->layertype != LAYERTYPE_DELETE))
				{
					//複製セル名を登録
					//全てのセルが自分の複製を知っている状態にする
					origin->copy_cell_name.push_back(copy->name);
				}
			}
		}
	}


	return true;
}

//はみ出すか調べる
bool	ConvertToSS::checkProtrudingTexture()
{
	//面積の調査
	unsigned long sizetble[] = { 4096,2048,1024,512,256 };
	int makesize_x = 0;
	int makesize_y = 0;

	long w_max = 0;
	long h_max = 0;
	unsigned long totalsize = 0;
	bool err = false;


	size_t num = packer.GetPrimitiveNum();
	//高さ、幅の最大を求める
	for (int i = 0; i < num; i++)
	{
		SSSpriteSheetPrim p = packer.GetPrimitiveInfo(i);
		if (p.isPacking == true)
		{
			if (w_max < p.trim_w)
			{
				w_max = p.trim_w;
			}
			if (h_max < p.trim_h)
			{
				h_max = p.trim_h;
			}
			totalsize += p.trim_w * p.trim_h;
		}
	}
	if (( params.tex_w == 0) && (params.tex_h == 0))
	{
		for (int i = 0; i < 5; i++)
		{
			if (w_max <= sizetble[i])
			{
				makesize_x = i;
			}
			if (h_max <= sizetble[i])
			{
				makesize_y = i;
			}
		}
		//面積から最小サイズを求める
		while (true)
		{
			if (totalsize > sizetble[makesize_x] * sizetble[makesize_y])
			{
				if (makesize_x >= makesize_y)
				{
					makesize_x--;
				}
				else
				{
					makesize_y--;
				}
			}
			else
			{
				std::cerr << "テクスチャサイズを" << sizetble[makesize_x] << "×" << sizetble[makesize_y] << "にしてパッキングします。\n";
				packer.SetSize(sizetble[makesize_x], sizetble[makesize_y]);
				break;
			}
			if ((makesize_x < 0) || (makesize_y < 0))
			{
				//作成できるサイズを超えた
				std::cerr << "エラー：セルマップに収まりません\n";
				err = true;
				break;
			}
		}
	}
	else
	{
		if (params.tex_w > 0)
		{
			if (w_max > params.tex_w)
			{
				//エラー
				std::cerr << "エラー：テクスチャの幅より大きいセルがあります。\n";
				//return true;
				err = true;
			}
		}
		if ( params.tex_h > 0)
		{
			if (h_max > params.tex_h)
			{
				//エラー
				std::cerr << "エラー：テクスチャの高さより大きいセルがあります。\n";
				//return true;
				err = true;
			}
		}

		if (err == false)
		{
			if ((params.tex_w == 0) && (params.tex_h > 0))
			{
				//幅自動
				makesize_x = 4;
				while (true)
				{
					if (totalsize > sizetble[makesize_x] * params.tex_h)
					{
						makesize_x--;
					}
					else
					{
						std::cerr << "テクスチャサイズを" << sizetble[makesize_x] << "×" << params.tex_h << "にしてパッキングします。\n";
						packer.SetSize(sizetble[makesize_x], params.tex_h);
						break;
					}
					if (makesize_x < 0)
					{
						//作成できるサイズを超えた
						std::cerr << "エラー：セルマップに収まりません。\n";
						err = true;
						break;
					}
				}
			}
			if ((params.tex_w > 0) && (params.tex_h == 0))
			{
				//高さ自動
				makesize_y = 4;
				while (true)
				{
					if (totalsize > params.tex_w * sizetble[makesize_y])
					{
						makesize_y--;
					}
					else
					{
						std::cerr << "テクスチャサイズを" << params.tex_w << "×" << sizetble[makesize_y] << "にしてパッキングします。\n";
						packer.SetSize(params.tex_w, sizetble[makesize_y]);
						break;
					}
					if (makesize_y < 0)
					{
						//作成できるサイズを超えた
						std::cerr << "エラー：セルマップに収まりません。\n";
						err = true;
						break;
					}
				}
			}
		}
	}

	if (err == false)
	{
		while (true)
		{
			if (packer.MakeUp(false, true, 1, params.sortmode) == false)
			{
				//テクスチャが入りきっていない
				std::cerr << "エラー：セルマップに収まりません。\n";
				if ((params.tex_w > 0) && (params.tex_h > 0))
				{
					std::cerr << "パッキングに失敗しました。\n";
					err = true;
					break;
				}
				if ((params.tex_w == 0) && (params.tex_h > 0))
				{
					if (makesize_x == 0)
					{
						std::cerr << "パッキングに失敗しました。\n";
						err = true;
						break;
					}
					else
					{
						//サイズを変えて挑戦してみる。
						makesize_x--;
						packer.SetSize(sizetble[makesize_x], params.tex_h);
						std::cerr << "テクスチャサイズを" << sizetble[makesize_x] << "×" << params.tex_h << "にしてパッキングします。\n";
					}
				}
				if ((params.tex_w > 0) && (params.tex_h == 0))
				{
					if (makesize_y == 0)
					{
						std::cerr << "パッキングに失敗しました。\n";
						err = true;
						break;
					}
					else
					{
						//サイズを変えて挑戦してみる。
						makesize_y--;
						packer.SetSize(params.tex_w, sizetble[makesize_y]);
						std::cerr << "テクスチャサイズを" << params.tex_w << "×" << sizetble[makesize_y] << "にしてパッキングします。\n";
					}
				}
				if ((params.tex_w == 0) && (params.tex_h == 0))
				{
					if ((makesize_x == 0) && (makesize_y == 0))
					{
						std::cerr << "パッキングに失敗しました。\n";
						err = true;
						break;
					}
					else
					{
						//サイズを変えて挑戦してみる。
						if (makesize_x >= makesize_y)
						{
							makesize_x--;
						}
						else
						{
							makesize_y--;
						}
						packer.SetSize(sizetble[makesize_x], sizetble[makesize_y]);
						std::cerr << "テクスチャサイズを" << sizetble[makesize_x] << "×" << sizetble[makesize_y] << "にしてパッキングします。\n";
					}
				}
			}
			else
			{
				std::cerr << "パッキング成功！\n";
				break;
			}
		}

		//色漏れ防止処理
		postProcessing.expandColor(*packer.spriteSheetBitmap);

		if (err == false)
		{
			RGBAImage image(packer.width, packer.height, (unsigned char*)packer.spriteSheetBitmap->Data);
			//png保存
			image.write_png(pngname.c_str());
		}
	}

	return !err;
}

bool    ConvertToSS::texturePacking()
{
	//テクスチャのパッキングクラス初期化
	//SSTexturePack	packer;
	//セル内余白を指定する
	packer.padding_inner = params.inner_padding;
	//サイズ
	packer.SetSize(params.tex_w, params.tex_h);
	packer.padding_shape = params.tex_padding_shape;
	packer.pack_padding = params.pack_padding;	//パッキンの単位を設定
	packer.padding_border = params.padding_border;	//縁サイズを設定

	//PSDのレイヤーをパッカーにプッシュ
	for (int read_count = 0; read_count < readpngfile_max; read_count++)
	{
		std::string name = pb.inputlayername[read_count];
		int layertype = LAYERTYPE_NORMAL;
		//原点用レイヤーはパッキングに含まない
		if (name.find("+") == 0)
		{
			//原点指定レイヤー
			//先頭文字が"+"の場合はパッキングしない
			layertype = LAYERTYPE_PIVOT;
		}
		if (name == "root")
		{
			//rootパーツ指定レイヤー
			layertype = LAYERTYPE_ROOT;
		}

		packer.PushBitmap(0, pb.readbitmap[read_count], name, layertype);
	}


	if (params.is_oldPivotUse)
	{
		texturePacking_PivotUse();
	}

	//削除されたセルが参照する透明セルを追加する
	for (auto it = ssce_cell_list.begin(); it != ssce_cell_list.end(); it++)
	{
		if (it->layertype == LAYERTYPE_DELETE)
		{
			//消去されたレイヤー用の空間を作成
			int deletecell_size = 2;
			UserBitMapData*	bitmap = new UserBitMapData();
			png_byte *image_data = new png_byte[deletecell_size * deletecell_size * sizeof(png_byte) + 15];
			memset(image_data, 0, sizeof(png_byte) * (deletecell_size * deletecell_size * sizeof(png_byte) + 15));
			bitmap->Data = (int*)image_data;
			bitmap->Width = deletecell_size;
			bitmap->Height = deletecell_size;
			bitmap->Pitch = bitmap->Width * 4;
			bitmap->offset_x = 0;
			bitmap->offset_y = 0;

			bitmap->orgX = 0;
			bitmap->orgY = 0;
			bitmap->orgWidth = deletecell_size;
			bitmap->orgHeight = deletecell_size;
			bitmap->pri = 0;

			packer.PushBitmap(0, bitmap, it->name, LAYERTYPE_DELETE);
		}
	}

	//↓調べる関数風だがパックと画像を保存している要修正
	if ( !checkProtrudingTexture()) return false;

	//セル配置間隔調整のために拡大したセルサイズを元に戻す
	int num = packer.GetPrimitiveNum();
	for (int i = 0; i < num; i++)
	{
		SSSpriteSheetPrim *p = packer.GetPrimitiveBuff(i);
		p->trim_right = p->trim_left + p->trim_w_org;
		p->trim_bottom = p->trim_top + p->trim_h_org;
		p->img_right = p->img_left + p->trim_w_org + p->padding * 2;
		p->img_bottom = p->img_top + p->trim_h_org + p->padding * 2;
		p->trim_w = p->trim_w_org;
		p->trim_h = p->trim_h_org;
	}

	return true;
}

void	ConvertToSS::makeSsceFile(SSOptionReader& option)
{
	//テンプレートの作成
	Ssce_template ssce_xml;
	ssce_xml.set_filename(sscename);
	ssce_xml.make_template(&option.getSSOption()); //この時点でssceは上書きされることに注意

	XMLDocument xml;
	xml.LoadFile(sscename.c_str());
	XMLElement* root = xml.FirstChildElement("SpriteStudioCellMap");

	//パッキングで決まるパラメータを出力
	root->FirstChildElement("name")->SetText( params.outputname.c_str());//セル名
	root->FirstChildElement("imagePath")->SetText((params.outputname + ".png").c_str());//テクスチャファイル名
	char size[256];
	sprintf(size, "%d %d", packer.width, packer.height);
	root->FirstChildElement("pixelSize")->SetText(size);//テクスチャサイズ

	XMLElement*  cells = root->FirstChildElement("cells");

//	bool err = false;

	int num = packer.GetPrimitiveNum();
	for (int read_count = 0; read_count < readpngfile_max; read_count++)
	{
		for (int i = 0; i < num; i++)
		{
			SSSpriteSheetPrim p = packer.GetPrimitiveInfo(i);
			if ((p.name == pb.inputlayername[read_count]) && (p.isPacking == true) && (p.layertype != LAYERTYPE_DELETE))
			{

				XMLElement*  cell = xml.NewElement("cell");
				cells->InsertEndChild(cell);

				cell->InsertEndChild(CreateNewElementWithText(xml, "name", p.name.c_str()));
				cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "pos", p.img_left, p.img_top));
				cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "size", p.img_right - p.img_left, p.img_bottom - p.img_top));

				//同じセル名があるかを検索しある場合はピボットを再設定する
				float piov_x = 0;
				float piov_y = 0;
				if (is_ssceload == true)
				{
					int j;
					for (j = 0; j < celldata_num; j++)
					{
						if (pivot[j].name == p.name)
						{
							pivot[j].use = true;
							piov_x = pivot[j].x;
							piov_y = pivot[j].y;
							break;
						}
					}
				}
				if ( params.is_layerPivotUse == true)
				{
					//原点レイヤーを適用する場合は検索して適用する
					for (size_t j = 0; j < num; j++)
					{
						SSSpriteSheetPrim pw = packer.GetPrimitiveInfo(j);
						if (pw.layertype == LAYERTYPE_PIVOT)
						{
							if (p.name == pw.name.substr(1, pw.name.length() - 1))
							{
								//同じ名前のレイヤーが存在した
								if (pw.nodata == true)
								{
									//透明レイヤーの場合は中心にする
									piov_x = 0;
									piov_y = 0;
								}
								else
								{
									//対象セルの中心位置x2 原点:PSD中心
									int org_image_x = (p.trim_left + p.offset_x) * 2 + p.trim_w;
									int org_image_y = -(p.trim_top + p.offset_y) * 2 - p.trim_h;
									//ピボットレイヤーセルの中心位置x2 原点:PSD中心
									int pivot_image_x = (pw.trim_left + pw.offset_x) * 2 + pw.trim_w;
									int pivot_image_y = -(pw.trim_top + pw.offset_y) * 2 - pw.trim_h;
									//差x2を計算
									int sa_x = pivot_image_x - org_image_x;
									int sa_y = pivot_image_y - org_image_y;
									//UV座標に変換
									float layer_pivotx = (float)sa_x / (float)(p.trim_w + p.padding * 2) / 2.f;
									float layer_pivoty = (float)sa_y / (float)(p.trim_h + p.padding * 2) / 2.f;
									piov_x = layer_pivotx;
									piov_y = layer_pivoty;
								}
								break;
							}
						}
					}
				}
				//原点分ずらしたオフセットを書き込む
				SSSpriteSheetPrim* pbuff = packer.GetPrimitiveBuff(i);
				pbuff->offset_x += piov_x * (float)(p.trim_w + p.padding * 2);	//原点分ずらす
				pbuff->offset_y += -piov_y * (float)(p.trim_h + p.padding * 2);

				cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "pivot", piov_x, piov_y));
				cell->InsertEndChild(CreateNewElementWithTextNumber(xml, "rotated", 0));
				cell->InsertEndChild(CreateNewElementWithText(xml, "orgImageName", ""));
				cell->InsertEndChild(CreateNewElementWithTextNumber(xml, "posStable", 0));

				//メッシュ情報を書き込む
				if (is_ssceload == true)
				{
					int j;
					for (j = 0; j < celldata_num; j++)
					{
						if (mesh[j].name == p.name)
						{
							//メッシュの座標を変換
							if (mesh[j].coord_origin == MESH::COORD_ORIGIN_CENTER)
							{
								tPoint2i center;
								center.x = p.img_right - p.img_left;
								center.y = p.img_bottom - p.img_top;
								//座標変換 原点:中心->右上
								mesh[j].trans_coord(center, 1);
								mesh[j].coord_origin = MESH::COORD_ORIGIN_TOPLEFT;
							}
							//メッシュ情報を書き出し
							mesh[j].insert_into(cell);
							break;
						}
					}
				}

				//上で出力したセルの複製セルを出力
				for (auto origin = ssce_cell_list.begin(); origin != ssce_cell_list.end(); origin++)
				{
					if (p.name == origin->name)
					{
						for (auto copy = origin->copy_cell_name.begin(); copy != origin->copy_cell_name.end(); copy++)
						{
							XMLElement*  cell = xml.NewElement("cell");
							cells->InsertEndChild(cell);

							cell->InsertEndChild(CreateNewElementWithText(xml, "name", copy->c_str()));
							cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "pos", p.img_left, p.img_top));
							cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "size", p.img_right - p.img_left, p.img_bottom - p.img_top));

							cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "pivot", piov_x, piov_y));
							cell->InsertEndChild(CreateNewElementWithTextNumber(xml, "rotated", 0));
							cell->InsertEndChild(CreateNewElementWithText(xml, "orgImageName", ""));
							cell->InsertEndChild(CreateNewElementWithTextNumber(xml, "posStable", 0));
							//メッシュ情報を書き込む
							if (is_ssceload == true)
							{
								int j;
								for (j = 0; j < celldata_num; j++)
								{
									if (mesh[j].name == *copy)
									{
										//メッシュの座標を変換
										if (mesh[j].coord_origin == MESH::COORD_ORIGIN_CENTER)
										{
											tPoint2i center;
											center.x = p.img_right - p.img_left;
											center.y = p.img_bottom - p.img_top;
											//座標変換 原点:中心->右上
											mesh[j].trans_coord(center, 1);
											mesh[j].coord_origin = MESH::COORD_ORIGIN_TOPLEFT;
										}
										//メッシュ情報を書き出し
										mesh[j].insert_into(cell);
										break;
									}
								}
							}
						}
						break;
					}
				}
			}
		}
	}


	//無くなったセルを出力する
	if (is_ssceload)
	{
		num = packer.GetPrimitiveNum();
		for (int i = 0; i < num; i++)
		{
			SSSpriteSheetPrim p = packer.GetPrimitiveInfo(i);
			if (p.layertype == LAYERTYPE_DELETE)
			{
				XMLElement*  cell = xml.NewElement("cell");
				cells->InsertEndChild(cell);
				cell->InsertEndChild(CreateNewElementWithText(xml, "name", p.name.c_str()));
				cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "pos", p.img_left, p.img_top));
				cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "size", p.img_right - p.img_left, p.img_bottom - p.img_top));
				cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "pivot", 0, 0));
				cell->InsertEndChild(CreateNewElementWithTextNumber(xml, "rotated", 0));
				cell->InsertEndChild(CreateNewElementWithText(xml, "orgImageName", ""));
				cell->InsertEndChild(CreateNewElementWithTextNumber(xml, "posStable", 0));

				//メッシュ情報を書き込む
				if (is_ssceload)
				{
					int j;
					for (j = 0; j < celldata_num; j++)
					{
						if (mesh[j].name == p.name)
						{
							//メッシュの座標を変換
							if (mesh[j].coord_origin == MESH::COORD_ORIGIN_CENTER)
							{
								tPoint2i center;
								center.x = p.img_right - p.img_left;
								center.y = p.img_bottom - p.img_top;
								//座標変換 原点:中心->右上
								mesh[j].trans_coord(center, 1);
								mesh[j].coord_origin = MESH::COORD_ORIGIN_TOPLEFT;
							}
							//メッシュ情報を書き出し
							mesh[j].insert_into(cell);
							break;
						}
					}
				}

				//上で出力したセルの複製セルを出力
				for (auto origin = ssce_cell_list.begin(); origin != ssce_cell_list.end(); origin++)
				{
					if (p.name == origin->name)
					{
						for (auto copy = origin->copy_cell_name.begin(); copy != origin->copy_cell_name.end(); copy++)
						{
							XMLElement*  cell = xml.NewElement("cell");
							cells->InsertEndChild(cell);

							cell->InsertEndChild(CreateNewElementWithText(xml, "name", copy->c_str()));
							cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "pos", p.img_left, p.img_top));
							cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "size", p.img_right - p.img_left, p.img_bottom - p.img_top));

							cell->InsertEndChild(CreateNewElementWithTextPoint2(xml, "pivot", 0, 0));
							cell->InsertEndChild(CreateNewElementWithTextNumber(xml, "rotated", 0));
							cell->InsertEndChild(CreateNewElementWithText(xml, "orgImageName", ""));
							cell->InsertEndChild(CreateNewElementWithTextNumber(xml, "posStable", 0));
							//メッシュ情報を書き込む
							if (is_ssceload)
							{
								int j;
								for (j = 0; j < celldata_num; j++)
								{
									if (mesh[j].name == *copy)
									{
										//メッシュの座標を変換
										if (mesh[j].coord_origin == MESH::COORD_ORIGIN_CENTER)
										{
											tPoint2i center;
											center.x = p.img_right - p.img_left;
											center.y = p.img_bottom - p.img_top;
											//座標変換 原点:中心->右上
											mesh[j].trans_coord(center, 1);
											mesh[j].coord_origin = MESH::COORD_ORIGIN_TOPLEFT;
										}
										//メッシュ情報を書き出し
										mesh[j].insert_into(cell);
										break;
									}
								}
							}
						}
						break;
					}
				}
			}
		}
	}


	xml.SaveFile(sscename.c_str());
	std::cerr << "ssce convert success!!: " << sscename << std::endl;


}


//
//ssaeの保存
//
//XMLDocument anime_xml;
//XMLDeclaration* anime_decl = anime_xml.NewDeclaration();
//anime_xml.InsertEndChild(anime_decl);
// standalone等が必要になってPrologueを変更したい場合は自前で書くことも可能
void	ConvertToSS::makeSsaeFile(SSOptionReader& option)
{

	XMLDocument& loadssop_xml = option.getSSOption();

	if (( !is_ssaeload ) && ( params.is_ssaeoutput))
	{
		//テンプレートの作成
		Ssae_template ssae_xml;
		ssae_xml.set_filename(ssaename);
		ssae_xml.make_template(&loadssop_xml); //この時点でssaeは上書きされることに注意

		XMLDocument anime_xml;
		anime_xml.LoadFile(ssaename.c_str());
		XMLElement* animeroot = anime_xml.FirstChildElement("SpriteStudioAnimePack");

		//パッキングで決まるパラメータを出力
		animeroot->FirstChildElement("name")->SetText(params.outputname.c_str()); //アニメ名

		XMLElement*  partList = animeroot->FirstChildElement("Model")->FirstChildElement("partList");

		//親子関係データの作成


		//通常パーツを登録
		int num = packer.GetPrimitiveNum();
		AddPartValue(partList, "root", 0, -1, 0);

		int parts_index = 1;
		for (int read_count = readpngfile_max - 1; read_count >= 0; read_count--)
		{
			for (size_t i = 0; i < num; i++)
			{
				SSSpriteSheetPrim p = packer.GetPrimitiveInfo(i);
				if ((p.name == pb.inputlayername[read_count]) && (p.isPacking) && (p.layertype != LAYERTYPE_DELETE))
				{
					if (params.is_addnull)
					{
						std::string nullname = "null_" + p.name;
						AddPartValue(partList, nullname.c_str(), parts_index, 0, 1);	//root パーツを0として通常パーツは1から始まるので。
						parts_index++;
						AddPartValue(partList, p.name.c_str(), parts_index, parts_index - 1, 2);	//root パーツを0として通常パーツは1から始まるので。
					}
					else
					{
						AddPartValue(partList, p.name.c_str(), parts_index, 0, 2);	//root パーツを0として通常パーツは1から始まるので。
					}
					parts_index++;
				}
			}
		}

		//セルマップ名
		XMLElement*  cellnames = animeroot->FirstChildElement("cellmapNames");
		std::string str = params.outputname + ".ssce";
		cellnames->FirstChildElement("value")->SetText(str.c_str());

		//Setup

		//アニメ情報の出力
		XMLElement*  anime = animeroot->FirstChildElement("animeList")->FirstChildElement("anime");
		XMLElement*  settings = anime->FirstChildElement("settings");
		char canvasSizestr[128];
		//基準枠サイズ
		if (params.canvasSize == 1)
		{
			sprintf(canvasSizestr, "%d %d", (gLayoutCenter.x * 2), (gLayoutCenter.y * 2));
			settings->FirstChildElement("canvasSize")->SetText(canvasSizestr);
		}

		//パーツアニメ情報
		XMLElement*  partanimes = anime->FirstChildElement("partAnimes");

		//元画像のサイズからssaeレイアウトを設定する
		//	gLayoutCenter.x = context->width / 2;
		//	gLayoutCenter.y = context->height / 2;
		SSSpriteSheetPrim* rootSpriteSheet = 0;
		int root_offset_x = 0;
		int root_offset_y = 0;
		if (params.is_rootLayerUse)
		{
			for (size_t i = 0; i < num; i++)
			{
				SSSpriteSheetPrim p = packer.GetPrimitiveInfo(i);
				if (p.layertype == LAYERTYPE_ROOT)
				{
					rootSpriteSheet = &p;
					root_offset_x = -((rootSpriteSheet->trim_left + rootSpriteSheet->offset_x + ((float)rootSpriteSheet->trim_w * 0.5f)) - gLayoutCenter.x);
					root_offset_y = gLayoutCenter.y - (rootSpriteSheet->trim_top + rootSpriteSheet->offset_y) - ((float)rootSpriteSheet->trim_h * 0.5f);
					break;
				}
			}
		}
		AddPartAnime(partanimes, 0, 0, false, &option.getSSOption(), gLayoutCenter);



		//パーツごとのアニメ情報を出力
		for (size_t i = 0; i < num; i++)
		{
			SSSpriteSheetPrim p = packer.GetPrimitiveInfo(i);
			//rootパーツ分オフセットを加える
			p.offset_x += root_offset_x;
			p.offset_y += root_offset_y;
			if ((p.isPacking == true) && (p.layertype != LAYERTYPE_DELETE))
			{
				if ( params.is_addnull)
				{
					AddPartAnime(partanimes, &p, 1, params.is_addnull, &loadssop_xml, gLayoutCenter);
				}
				AddPartAnime(partanimes, &p, 2, params.is_addnull, &loadssop_xml, gLayoutCenter);
			}
		}

		//anime_1

		//アニメ情報の出力
		anime = animeroot->FirstChildElement("animeList")->FirstChildElement("anime")->NextSiblingElement("anime");
		settings = anime->FirstChildElement("settings");

		//基準枠サイズ
		if (params.canvasSize == 1)
		{
			settings->FirstChildElement("canvasSize")->SetText(canvasSizestr);
		}

		//パーツアニメ情報
		partanimes = anime->FirstChildElement("partAnimes");

		XMLElement*  partAnime = anime_xml.NewElement("partAnime");
		partanimes->InsertEndChild(partAnime);

		partAnime->InsertEndChild(CreateNewElementWithText(anime_xml, "partName", "root"));

		XMLElement*  attributes = anime_xml.NewElement("attributes");
		partAnime->InsertEndChild(attributes);
		addAnimeAttribute(attributes, "HIDE", "0", false);

		//新規作成
		anime_xml.SaveFile(ssaename.c_str());
		std::cerr << "ssae convert success!!: " << ssaename << std::endl;
	}

}

void	ConvertToSS::makeSspjFile(SSOptionReader& option)
{
	XMLDocument& loadssop_xml = option.getSSOption();

	if ((is_sspjload == false) && (params.is_sspjoutput == true))
	{
		//新規作成
		make_sspj(sspjname, params.outputname, &loadssop_xml);
		std::cerr << "sspj convert success!!: " << sspjname << std::endl;
	}
}


bool	ConvertToSS::convert(std::string arg)
{
	gLayoutCenter.x;
	gLayoutCenter.y;
	ssce_cell_list.clear();
	celldata_num = 0;
	readpngfile_max = 0;
	memset(pivot, 0, sizeof(pivot));
	
	if (!parseInputOutputFiles(arg))return false;
	if (!getInfomationFilePath()) return false;

	if (!params.parseConfigJson(convert_info_path + ".json"))
	{
		if (!params.parseConfig(convert_info_path)) return false;
	}
	else {
		//jsonはutf8で保存されているためsjisへ変換
#ifdef _WIN32
		params.outputpath = babel::utf8_to_sjis(params.outputpath);
		params.outputname = babel::utf8_to_sjis(params.outputname);
#endif
	}

	//イメージファイルのロード
	if (!loadImageFile())return false;

	//無効なレイヤー名のチェック
	if (!checkInvalidLayerName()) return false;
	
	//プライオリティの加算
	addPriority();

	loadssae();

	//出力ファイル名を作製
	pngname = params.outputpath + params.outputname + ".png";
	sspjname = params.outputpath + params.outputname + ".sspj";
	sscename = params.outputpath + params.outputname + ".ssce";
	ssaename = params.outputpath + params.outputname + ".ssae";


	//パッキングの実行
	if (!texturePacking()) return false;


	SSOptionReader ssoption;

//	if ( !ssoption.load() ) return false;
	ssoption.load();
	makeSsceFile(ssoption);
	makeSsaeFile(ssoption);
	makeSspjFile(ssoption);


	return true;
}


