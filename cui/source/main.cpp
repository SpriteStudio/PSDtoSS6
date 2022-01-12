// main.cpp
//
// $ g++ main.cpp -lpng -lpsd -ljpeg

#include "convertToSS.h"
#include <stdlib.h>
#include "tbl_outmessage.h"

using namespace SsConverter;

#define VERSION "PSDtoSS6 CUI Ver2.4.4"


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

	MessageInit();
	ConvertToSS converter;
    std::cerr << VERSION << std::endl;
	
	std::vector<std::string> arglist;
	for (int i = 0; i < argc; i++)
	{
		arglist.push_back(argv[i]);
		std::cerr << argv[i] << std::endl;
	}

	if (!converter.convert(argc , arglist))
	{
		return 1;
	}
	return 0;

}
