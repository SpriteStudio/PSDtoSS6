#pragma once
#include <string>

#ifndef _WIN32
#include "tinyxml2.h"
#include "babel.h"
#else
#include "tinyxml2.h"
#include "babel.h"
#include <locale.h>
#endif

#include <math.h>

#include "psdBitmapLayer.h"


/*	v 2.0.1
#if _WIN32
#define CONVERT_INFO_PATH "\\SpriteStudio\\PSDtoSS6\\convert_info"
#else
#define CONVERT_INFO_PATH "/Documents/SpriteStudio/PSDtoSS6/convert_info";
#endif
*/
#if _WIN32 // after v2.0.2
#define TOOL_DOCUMENT_FOLDER "\\PSDtoSS6\\"
#else
#define TOOL_DOCUMENT_FOLDER "/Documents/PSDtoSS6/";
#endif

