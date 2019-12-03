#include "parameters.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>

#include "../picojson/picojson.h"
//#include "picojson/picojson.h""



int convert_parameters::SortModeStringToInt(std::string str)
{

	if (str == "RECTMAX" || str == "rectmax")
		return EN_PUSH_SORT_RECTMAX;

	if (str == "WMAX" || str == "wmax")
		return EN_PUSH_SORT_WMAX;

	if (str == "HMAX" || str == "hmax")
		return EN_PUSH_SORT_HMAX;


    return EN_PUSH_SORT_NONE;
}


std::string convert_parameters::SortModeIntToString(int i)
{
    if (i == EN_PUSH_SORT_RECTMAX)
    {
        return "rectmax";
    }
    else
    if (i == EN_PUSH_SORT_WMAX)
    {
        return "wmax";
    }
    else
    if (i == EN_PUSH_SORT_HMAX)
    {
        return "hmax";
    }
    else
    {
        return "none";
    }
}


bool convert_parameters::parseConfigJson(std::string fname)
{
#if _WIN32
	std::locale::global(std::locale("japanese"));
#endif

	std::ifstream ifs(fname, std::ios::in);
	if (ifs.fail()) {
//		std::cerr << "failed to read test.json" << std::endl;		
		return false;
	}
	const std::string json((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();

	picojson::value v;
	const std::string err = picojson::parse(v, json);
	if (err.empty() == false) {
		std::cerr << err << std::endl;
		return false;
	}

	picojson::object& obj = v.get<picojson::object>();


	//jsonデータから値へ
	tex_w				= obj["texture_width"].get<double>();
	tex_h				= obj["texture_height"].get<double>();
	tex_padding_shape	= obj["texture_padding_shape"].get<double>();
	pack_padding		= obj["pack_padding"].get<double>();
	addpri				= obj["add_priority"].get<double>();

	is_ssaeoutput		= obj["is_ssaeoutput"].get<bool>();
	is_sspjoutput		= obj["is_sspjoutput"].get<bool>();

	is_overwrite		= obj["is_overwrite"].get<bool>();
	is_addnull			= obj["is_addnull"].get<bool>();
	is_layerPivotUse	= obj["is_layerPivotUse"].get<bool>();
	is_rootLayerUse		= obj["is_rootLayerUse"].get<bool>();
	is_oldPivotUse		= obj["is_oldPivotUse"].get<bool>();

	sortmode			= SortModeStringToInt( obj["sortmode"].get<std::string>() );
	padding_border		= obj["padding_border"].get<double>();
	canvasSize			= obj["canvasSize"].get<double>();
	inner_padding		= obj["inner_padding"].get<double>();

	outputpath = obj["outputpath"].get<std::string>();
	std::string temp = obj["outputname"].get<std::string>();


	if (temp != "")
	{
		outputname = temp;
	}

	if ((tex_padding_shape + inner_padding) <= 1)
	{
//		std::cerr << "警告：セル間余白+セル内余白の値が1以下に指定されています。" << std::endl;
//		std::cerr << "　　　色もれが発生する可能性があります。" << std::endl;
//		ConsoleOutMessage(WARNING_0001);
	}
	return true;
}


bool convert_parameters::saveConfigJson(std::string fname)
{
#if _WIN32
	std::locale::global(std::locale("japanese"));
#endif
	std::ofstream ofs(fname);
	if (ofs.fail()) {
		//std::cerr << "failed to read test.json" << std::endl;
		//ConsoleOutMessage(ERROT_0010);
		return false;
	}

	picojson::object obj;
	//値からjsonデータへ
	obj["texture_width"]			= picojson::value(static_cast<double>(tex_w));
	obj["texture_height"]			= picojson::value(static_cast<double>(tex_h));
	obj["texture_padding_shape"]	= picojson::value(static_cast<double>(tex_padding_shape));
	obj["pack_padding"]				= picojson::value(static_cast<double>(pack_padding));
	obj["add_priority"]				= picojson::value(static_cast<double>(addpri));

	obj["is_ssaeoutput"]			= picojson::value(is_ssaeoutput);
	obj["is_sspjoutput"]			= picojson::value(is_sspjoutput);

	obj["is_overwrite"]				= picojson::value(is_overwrite);
	obj["is_addnull"]				= picojson::value(is_addnull);
	obj["is_layerPivotUse"]			= picojson::value(is_layerPivotUse);
	obj["is_rootLayerUse"]			= picojson::value(is_rootLayerUse);
	obj["is_oldPivotUse"]			= picojson::value(is_oldPivotUse);
    obj["sortmode"]					= picojson::value(SortModeIntToString(sortmode));
	obj["padding_border"]			= picojson::value(static_cast<double>(padding_border));
	obj["canvasSize"]				= picojson::value(static_cast<double>(canvasSize));
	obj["inner_padding"]			= picojson::value(static_cast<double>(inner_padding));
	obj["outputpath"]				= picojson::value(outputpath);
	obj["outputname"]				= picojson::value(outputname);

	picojson::value v(obj);
	v.serialize(std::ostream_iterator<char>(ofs), true);
	ofs.close();
	return true;
}


//powerd by koizumi
//old version config file
bool convert_parameters::parseConfig(std::string fname)
{
#if _WIN32
	std::locale::global(std::locale("japanese"));
#endif
	std::ifstream ifs;

	ifs.open(fname);
	if (ifs)
	{
		//出力フォルダ
		{
			char c[256];
			ifs.getline(c, 256);
			outputpath = c;	//あとで精査
		}

		//テクスチャサイズ幅
		{
			std::string str;
			ifs >> str;
			tex_w = std::atoi(str.c_str());
		}
		//テクスチャサイズ高さ
		{
			std::string str;
			ifs >> str;
			tex_h = std::atoi(str.c_str());
		}
		//パーツ間の余白
		{
			std::string str;
			ifs >> str;
			tex_padding_shape = std::atoi(str.c_str());
		}
		//ソート指定
		{
			std::string str;
			ifs >> str;
			sortmode = std::atoi(str.c_str());
		}
		//優先度設定
		{
			std::string str;
			ifs >> str;
			addpri = std::atoi(str.c_str());
		}
		//ssae出力
		{
			std::string str;
			ifs >> str;
			if (str == "ssae")
			{
				is_ssaeoutput = true;
			}
		}
		//sspj出力
		{
			std::string str;
			ifs >> str;
			if (str == "sspj")
			{
				is_sspjoutput = true;
			}
		}
		//addnull出力
		{
			std::string str;
			ifs >> str;
			if (str == "null")
			{
				is_addnull = true;
			}
		}
		//オーバーライト
		{
			std::string str;
			ifs >> str;
			if (str == "ow")
			{
				is_overwrite = true;
			}
		}
		//レイヤーの原点を適用する
		{
			std::string str;
			ifs >> str;
			if (str == "yes")
			{
				is_layerPivotUse = true;
			}
		}
		//レイヤーの原点を適用する
		{
			std::string str;
			ifs >> str;
			if (str == "root")
			{
				is_rootLayerUse = true;
			}
		}
		//出力フォルダの原点設定を引き継ぐ
		{
			std::string str;
			ifs >> str;
			if (str == "yes")
			{
				is_oldPivotUse = true;
			}
		}
		//配置間隔
		{
			std::string str;
			ifs >> str;
			pack_padding = std::atoi(str.c_str());
		}
		//縁幅
		{
			std::string str;
			ifs >> str;
			padding_border = std::atoi(str.c_str());

		}
		//基準枠サイズ
		{
			std::string str;
			ifs >> str;
			canvasSize = std::atoi(str.c_str());
		}
		//セル内余白
		{
			std::string str;
			ifs >> str;
			inner_padding = std::atoi(str.c_str());
		}
		//出力ファイル名設定
		ifs >> outputname;
	}
	else
	{
		//std::cerr << "コンバート情報のオープンに失敗しました" << std::endl;
//		ConsoleOutMessage(ERROT_0011);
		return false;
	}

	if ((tex_padding_shape + inner_padding) <= 1)
	{
//		std::cerr << "警告：セル間余白+セル内余白の値が1以下に指定されています。" << std::endl;
//		std::cerr << "　　　色もれが発生する可能性があります。" << std::endl;
//		ConsoleOutMessage(WARNING_0001);
	}

	return true;
}
