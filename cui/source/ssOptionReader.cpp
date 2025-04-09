#include "common.h"
#include "ssOptionReader.h"
#include "helper.h"
#include "system_functions.h"


using namespace tinyxml2;

SSOptionReader::SSOptionReader()
{
}

bool	SSOptionReader::load( std::string ss_option_file_oath)
{

	std::string home = get_documents_path();
	if (home == "") return false;

#ifndef _WIN32
    ssopname = home + "/Documents/SpriteStudio/SsOption_v6.ssop";
    ssopname_beta = home + "/Documents/SpriteStudio/SsOption_v6_beta.ssop";
#else
    ssopname = home + "\\SpriteStudio\\SsOption_v6.ssop";
    ssopname_beta = home + "\\SpriteStudio\\SsOption_v6_beta.ssop";
#endif

    if (!ss_option_file_oath.empty())
    {
        ssopname = ss_option_file_oath;
    }

    std::cout << "Loading ssop : " << ssopname << std::endl;   
	//ssopをxmlドキュメントとして取得
	if (tinyxml2::XML_SUCCESS != loadssop_xml.LoadFile(ssopname.c_str()))
	{
        std::cout << "Failed. Try Loading ssop_beta : " << ssopname_beta << std::endl;   
		//SsOption_v6がない場合は、SsOption_v6_betaを読む
		if (tinyxml2::XML_SUCCESS != loadssop_xml.LoadFile(ssopname_beta.c_str()))
		{
            std::cout << "Failed. Use tool default settings." << std::endl;
			return true;
		}else{
            std::cout << "Success." << std::endl;
            return true;
        }
	}else{
        std::cout << "Success." << std::endl;
    	return true;
    }

    //std::cout << "Project setting Load Tool Default " << std::endl;
	return true;
}
