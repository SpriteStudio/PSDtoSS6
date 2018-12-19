#include "system_functions.h"
#ifndef _WIN32
#else
#include <Shlobj.h>
void get_documents_path(char* p_path)
{
    SHGetSpecialFolderPath(NULL, p_path, CSIDL_MYDOCUMENTS, 0);
};
#endif
