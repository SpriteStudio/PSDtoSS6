#include "stringconv.h"
#include <unicode/unistr.h>
#include <vector>

namespace stringconv
{

	std::string sjis_to_utf8(std::string str)
	{
		icu::UnicodeString src( str.c_str() , "shift_jis");
		int length = src.extract(0, src.length(), NULL, "utf8");

		std::vector<char> result(length + 1);
		src.extract(0, src.length(), &result[0], "utf8");

		return std::string(result.begin(), result.end() - 1);
	}


	std::string utf8_to_sjis(std::string str)
	{
		icu::UnicodeString src(str.c_str(), "utf8");
		int length = src.extract(0, src.length(), NULL, "shift_jis");

		std::vector<char> result(length + 1);
		src.extract(0, src.length(), &result[0], "shift_jis");

		return std::string(result.begin(), result.end() - 1);
	}

};

