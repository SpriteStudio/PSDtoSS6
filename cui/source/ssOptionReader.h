#pragma once

#include "common.h"
using namespace tinyxml2;

class SSOptionReader
{
private:
	std::string ssopname;
	std::string ssopname_beta;

	XMLDocument loadssop_xml;

public:
	SSOptionReader();
	virtual ~SSOptionReader() {}

	bool	load(std::string ss_option_file_oath);
	XMLDocument&	getSSOption() {
		return loadssop_xml;
	}

};


