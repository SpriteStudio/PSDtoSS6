// main.cpp
//
// $ g++ main.cpp -lpng -lpsd -ljpeg

#include "convertToSS.h"

using namespace SsConverter;


int main(int argc, char **argv)
{

	if (argc <= 1) {
		return -1;
	}
#ifdef _WIN32
	std::locale::global(std::locale("japanese"));
	setlocale(LC_CTYPE, "jpn");
#endif

	babel::init_babel();

	ConvertToSS converter;
	
	if (!converter.convert(argv[1]))
	{
		return 1;
	}
	return 0;

}
