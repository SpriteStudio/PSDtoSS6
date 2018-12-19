#include "common.h"
#include "ssOptionReader.h"
#include "helper.h"
#include "system_functions.h"


using namespace tinyxml2;

SSOptionReader::SSOptionReader()
{
}

bool	SSOptionReader::load()
{
#ifndef _WIN32
	char* p_c = getenv("HOME");
	if (p_c == nullptr)
	{
		return false;
	}
	else
	{
		std::string home = p_c;
		ssopname = home + "/Documents/SpriteStudio/SsOption_v6.ssop";
		ssopname_beta = home + "/Documents/SpriteStudio/SsOption_v6_beta.ssop";
	}
#else
	char p_c[256];
	get_documents_path(p_c);
	if (p_c == NULL)
	{
		return false;
	}
	else
	{
		std::string home = p_c;
		ssopname = home + "\\SpriteStudio\\SsOption_v6.ssop";
		ssopname_beta = home + "\\SpriteStudio\\SsOption_v6_beta.ssop";
	}
#endif

	//ssopをxmlドキュメントとして取得
	if (tinyxml2::XML_NO_ERROR != loadssop_xml.LoadFile(ssopname.c_str()))
	{
		//SsOption_v6がない場合は、SsOption_v6_betaを読む
		if (tinyxml2::XML_NO_ERROR != loadssop_xml.LoadFile(ssopname_beta.c_str()))
		{
			return false;
		}
	}

	return true;
}
