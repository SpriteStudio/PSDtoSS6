// main.cpp
//
// $ g++ main.cpp -lpng -lpsd -ljpeg

#include "convertToSS.h"
#include <stdlib.h>

using namespace SsConverter;


int main(int argc, char **argv)
{

#if 1
	//なぜか未参照となるので書いておく
	rand();
	void * ptr = malloc(4);
	ptr = realloc(ptr, 8);
	free(ptr);
#endif

	if (argc <= 1) {
		return -1;
	}
#ifdef _WIN32
	std::locale::global(std::locale("japanese"));
	setlocale(LC_CTYPE, "jpn");
#endif


	ConvertToSS converter;
	
	if (!converter.convert(argv[1]))
	{
		return 1;
	}
	return 0;

}
