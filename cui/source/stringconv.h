#ifndef __STRINGCONV__
#define __STRINGCONV__

#include <string>

namespace stringconv
{

	std::string sjis_to_utf8(std::string str);
	std::string utf8_to_sjis(std::string str);

};

#endif

