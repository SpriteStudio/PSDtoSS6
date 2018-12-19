#pragma once

#include<string>
#include "common.h"
#include "helper.h"
#include "parameters.h"
#include "system_functions.h"
#include "ssOptionReader.h"


using namespace tinyxml2;

namespace SsConverter
{
	enum  PsdLoadErr {
		ErrorNone = 0,
		OPENFAILED,
		TEXTINVALID,
		PNGLOADERR,
	};

	class ConvertToSS
	{
	public:

	private:
		convert_parameters	params;

		bool is_psdload;	//画像種別のフラグ　ロードされたかどうかではない点に注意


		std::string inputname;
		std::string inputpath;

		//std::string outputname;

		//std::string outputpath;

		std::string convert_info_path;
		//std::string ssopname;

		//出力ファイル名
		std::string pngname;
		std::string sspjname;
		std::string sscename;
		std::string ssaename;


		//ピボット構造体
		PIVOT pivot[PARTS_MAX];
		//ssceから読み込んだメッシュ情報
		MESH mesh[PARTS_MAX];
		//色漏れ防止機能
		PostProcessing postProcessing;
		//ssceから読み込んだセルリスト
		std::vector<SSSpriteSheetPrim> ssce_cell_list;

		tPoint2i	gLayoutCenter;
		int celldata_num;
		bool is_ssceload = false;
		bool is_ssaeload = false;
		bool is_sspjload = false;

		int readpngfile_max;
		psdBitMapLayer	pb;
		SSTexturePack	packer;
		XMLDocument loadssae_xml;
	private:

		bool	isPsdMode() { return is_psdload; }
		bool	parseParameter();
		bool	parseInputOutputFiles(std::string arg);
		bool	getInfomationFilePath();


		PsdLoadErr		LoadPsdImageList();

		bool	loadImageFile();
		bool	checkInvalidLayerName();
		void	addPriority();
		void	loadssae();

		bool    texturePacking();
		bool    texturePacking_PivotUse();
		bool	checkProtrudingTexture();
		void	makeSsceFile(SSOptionReader& option);
		void	makeSsaeFile(SSOptionReader& option);
		void	makeSspjFile(SSOptionReader& option);


	public:
		ConvertToSS() {}
		virtual ~ConvertToSS() {}

		bool	convert(std::string arg);


	};

};
