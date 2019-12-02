#include "system_functions.h"
#include <iostream>
#include <locale> 
#include <codecvt> 
#include <cstdio>



#ifdef _WIN32

#include <Shlobj.h>

std::string	get_documents_path()
{
//	char	buffer[_MAX_PATH];
	WCHAR buffer[_MAX_PATH];

	if (SHGetSpecialFolderPath(NULL, buffer, CSIDL_MYDOCUMENTS, 0) == false) return "";
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::string reconvert = cv.to_bytes(buffer);

	return reconvert;
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
