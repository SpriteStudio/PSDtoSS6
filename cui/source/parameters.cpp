#include "parameters.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>

#include "../picojson/picojson.h"
//#include "picojson/picojson.h""


/*
	省略	フル
	-TW		--texture_width 256		defalut = 0
	-TH		--texture_height 256		default = 0
	-PS		--pading_shape 2			default = 2
	-PP		--pack_padding 1			defalut = 1
	-AP		--add_priority 0			default = 1
	-OA		--out_ssae true			default = false
	-OP		--out_sspj true			default = false
	-OW		--overwrite true			default = false
	-AN		--add_null true			default = false
	-UP		--use_layer_pivot true	default = false
	-UR		--use_layer_root true	default = false
	-UO		--use_oldpivot true		default = false
	-SO		--sortmode "mode"	mode = rectmax / wmax / hmax / none
	-PB		--padding_border 1		default = 4
	-CS		--canvas_size 1024		default = 0
	-PI		--padding_innner 1		default = 0
	-O		--outputpath "file"	default = ""
	-ON		--outputname "filename" default = ""
*/

struct option_struct{
	char* OP1;
	char* OP2;
};

option_struct option_char_tbl[] = {

	{ "-TW","--texture_width"},
	{ "-TH","--texture_height" },
	{ "-PS","--pading_shape" },
	{ "-PP","--pack_padding" },
	{ "-AP","--add_priority" },
	{ "-OA","--out_ssae" },
	{ "-OP","--out_sspj" },
	{ "-OW","--overwrite" },
	{ "-AN","--add_null" },
	{ "-UP","--use_layer_pivot" },
	{ "-UR","--use_layer_root" },
	{ "-UO","--use_oldpivot" },
	{ "-SO","--sortmode" },
	{ "-PB","--padding_border" },
	{ "-CS","--canvas_size" },
	{ "-PI","--padding_innner" },
	{ "-O","--outputpath" },
	{ "-ON","--outputname" },
	{ "-I","--inputfile" },
	{ "-J","--json"},
	{0,0}
};


std::string replaceString(std::string &inStr, std::string key , std::string replaceStr) {

    std::string ret = inStr;
	if (key.empty()) {
		return inStr;
	}

    size_t pos;

	while( std::string::npos != (pos = ret.find(key) ) )
    {
        ret = ret.replace(pos, key.length() , replaceStr);
    }

	return ret;
}

bool isFindArgtype(std::string arg)
{
	size_t tablenum = sizeof(option_char_tbl) / sizeof(option_struct)-1;

	for (size_t n = 0; n < tablenum; n++)
	{
		if (arg == option_char_tbl[n].OP1)
		{
			return true;
		}
		if (arg == option_char_tbl[n].OP2)
		{
			return true;
		}
	}
	return false;
}

std::string convertOptionCode(std::string arg)
{
	size_t tablenum = sizeof(option_char_tbl) / sizeof(option_struct) - 1;

	for (size_t n = 0; n < tablenum; n++)
	{
		if (arg == option_char_tbl[n].OP1)
		{
			return option_char_tbl[n].OP1;
		}
		if (arg == option_char_tbl[n].OP2)
		{
			return option_char_tbl[n].OP1;
		}
	}


	return "";
}

bool isAllFindArgType(std::vector<std::string> arglist)
{

	for (size_t i = 0; i < arglist.size(); i++)
	{
		if (isFindArgtype(arglist[i])) return true;
	}


	return false;
}

int tryInt(std::string str)
{
	try {
		return atoi(str.c_str());
	}
	catch (...)
	{

	}
	return 0;
}


bool stringToBool(std::string str)
{
	if (str == "TRUE" || str == "true")
	{
		return true;
	}
	if (str == "FALSE" || str == "false")
	{
		return false;
	}

	return false;
}

std::string boolToString(bool b)
{
	if (b == true) return "true";

	return "false";
}


// 最低限　-Iが無いと成立しない
bool convert_parameters::parseConfigArg(int num, std::vector<std::string> arglist)
{

	std::map < std::string, std::string> parammap;

	if (num > 2)
	{
		parammap["-TW"] = "0";
		parammap["-TH"] = "0";
		parammap["-PS"] = "2";
		parammap["-PP"] = "1";
		parammap["-AP"] = "1";
		parammap["-OA"] = "false";
		parammap["-OP"] = "false";
		parammap["-OW"] = "false";
		parammap["-AN"] = "false";
		parammap["-UP"] = "false";
		parammap["-UR"] = "false";
		parammap["-UO"] = "false";
		parammap["-SO"] = "RECTMAX";
		parammap["-PB"] = "4";
		parammap["-CS"] = "0";
		parammap["-PI"] = "0";
		parammap["-O"] = "";
		parammap["-ON"] = "";
		parammap["-I"] = "";
		parammap["-J"] = "false";

		//引数リストを含んでいる
		if (isAllFindArgType(arglist))
		{
			int cnt = 1;

			while (1)
			{
				if (cnt == arglist.size()) break;
				if (isFindArgtype(arglist[cnt]))
				{
					std::string code = convertOptionCode(arglist[cnt]);

					parammap[code] = arglist[cnt + 1];
				}
				else {
					//
					//知らないオプション

				}
				cnt+=2;
			}


			this->tex_w = tryInt(parammap["-TW"]);
			this->tex_h = tryInt(parammap["-TH"]);
			this->tex_padding_shape = tryInt(parammap["-PS"]);
			this->pack_padding = tryInt(parammap["-PP"]);
			this->addpri = tryInt(parammap["-AP"]);

			this->is_ssaeoutput = stringToBool(parammap["-OA"]);
			this->is_sspjoutput = stringToBool(parammap["-OP"]);
			this->is_overwrite = stringToBool(parammap["-OW"]);
			this->is_addnull = stringToBool(parammap["-AN"]);
			this->is_layerPivotUse = stringToBool(parammap["-UP"]);
			this->is_rootLayerUse = stringToBool(parammap["-UR"]);
			this->is_oldPivotUse = stringToBool(parammap["-UO"]);
			this->sortmode = SortModeStringToInt(parammap["-SO"]);
			this->padding_border = tryInt(parammap["-PB"]);
			this->canvasSize = tryInt(parammap["-CS"]);
			this->inner_padding = tryInt(parammap["-PI"]);
			this->inputjson = stringToBool(parammap["-J"]);


			this->outputpath = parammap["-O"];
			if (this->outputpath.back() != '\\')
			{
				this->outputpath += "\\";
			}


			this->inputpsdfile = parammap["-I"];

			if (parammap["-ON"] != "")
			{
				this->outputname = parammap["-ON"];
			}

#if _WIN32
			outputpath = replaceString(outputpath, "/", "\\");
			inputpsdfile = replaceString(inputpsdfile, "/", "\\");
#else
			outputpath = replaceString(outputpath, "\\", "/");
			inputpsdfile = replaceString(inputpsdfile, "\\", "/");
#endif


			return true;
		}
	}

	return false;
}

std::string convert_parameters::makeArgFromParam()
{
	std::string str;

	str  = " -TW " + std::to_string(tex_w);
	str += " -TH " + std::to_string(tex_h);
	str += " -PS " + std::to_string(tex_padding_shape);
	str += " -PP " + std::to_string(pack_padding);
	str += " -AP " + std::to_string(addpri);

	str += " -OA " + boolToString(is_ssaeoutput);
	str += " -OP " + boolToString(is_sspjoutput);
	str += " -OW " + boolToString(is_overwrite);

	str += " -AN " + boolToString(is_addnull);

	str += " -UP " + boolToString(is_layerPivotUse);
	str += " -UR " + boolToString(is_rootLayerUse);

	str += " -UO " + boolToString(is_oldPivotUse);
	str += " -SO " + SortModeIntToString(sortmode);

	str += " -PB " + std::to_string(padding_border);
	str += " -CS " + std::to_string(canvasSize);

	str += " -PI " + std::to_string(inner_padding);

#if _WIN32
	outputpath = replaceString(outputpath, "/", "\\");
#else
	outputpath = replaceString(outputpath, "\\", "/");
#endif

	str += " -O "  + outputpath;
	//str += " -ON " + '"' + outputname + '"';



	return str;
}

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
