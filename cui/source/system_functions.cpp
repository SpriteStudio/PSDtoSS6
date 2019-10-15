#include "system_functions.h"




#ifdef _WIN32

#include <Shlobj.h>

std::string	get_documents_path()
{
	char	buffer[_MAX_PATH];
	if (SHGetSpecialFolderPath(NULL, buffer, CSIDL_MYDOCUMENTS, 0) == false) return "";

	return buffer;
}

#else

#include <stdlib.h>

std::string	get_documents_path()
{
	char* p_c = getenv("HOME");
	if (p_c == nullptr)
	{
		return "";
	}

	return p_c;
}

#endif
