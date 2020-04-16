#pragma once

#include "texturepack.h"
#include <string>



class  convert_parameters
{
public:
	int tex_w;
	int tex_h;
	int tex_padding_shape;
	int pack_padding;
	int addpri;

	bool is_ssaeoutput;
	bool is_sspjoutput;
	bool is_addnull;
	bool is_overwrite;
	bool is_layerPivotUse;
	bool is_rootLayerUse;
	bool is_oldPivotUse;
	int sortmode;
	int padding_border;
	int canvasSize;
	int inner_padding;

	std::string outputpath;
	std::string outputname;
	std::string inputpsdfile;


public:	
	convert_parameters() : 
		tex_w(0),
		tex_h(0),
		tex_padding_shape(2),
		pack_padding(1),
		addpri(1),
		is_ssaeoutput(false),
		is_sspjoutput(false),
		is_overwrite(false),
		is_addnull(false),
		is_layerPivotUse(false),
		is_rootLayerUse(false),
		is_oldPivotUse(false),
		sortmode(EN_PUSH_SORT_RECTMAX),
		padding_border(4),
		canvasSize(0),
		inner_padding(0),
		outputpath(""),
		outputname(""),
		inputpsdfile("")
	{}

	~convert_parameters() {}


	bool parseConfig(std::string fname);
	bool parseConfigJson(std::string fname);
	bool saveConfigJson(std::string fname);
	int SortModeStringToInt(std::string str);
    std::string SortModeIntToString(int i);

	bool parseConfigArg(int num, std::vector<std::string> arglist);



};

