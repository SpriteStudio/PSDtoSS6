#include "system_functions.h"





#ifndef _WIN32

#if 0
void get_documents_path(char* p_path)
{
	SHGetSpecialFolderPath(NULL, p_path, CSIDL_MYDOCUMENTS, 0);
};
#endif

std::string	get_documents_path()
{
	char* p_c = getenv("HOME");
	if (p_c == nullptr)
	{
		return "";
	}

	return p_c;
}

#else
#include <Shlobj.h>

#if 0
void get_documents_path(char* p_path)
{
    SHGetSpecialFolderPath(NULL, p_path, CSIDL_MYDOCUMENTS, 0);
};
#endif


std::string	get_documents_path()
{
	char	buffer[_MAX_PATH];
	if (SHGetSpecialFolderPath(NULL, buffer, CSIDL_MYDOCUMENTS, 0) == false) return "";
	
	return buffer;
}

#endif
