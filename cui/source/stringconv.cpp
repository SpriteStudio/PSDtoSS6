#include "stringconv.h"
#include <unicode/unistr.h>
#include <vector>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

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

	std::string local8bit_to_utf8(std::string str)
	{
#ifdef _WIN32
		return sjis_to_utf8(str);
#endif
#ifdef __APPLE__
		return str;
#endif
	}

	std::wstring sjis_to_wstring(const std::string& sjisString)
	{
#ifdef _WIN32
		if (sjisString.empty()) {
			return std::wstring();
		}

		// get size
		int size = MultiByteToWideChar(932, 0, sjisString.c_str(), -1, nullptr, 0);
		if (size == 0) {
			DWORD error = GetLastError();
			std::cerr << "MultiByteToWideChar (size) failed with error: " << error << std::endl;
			return std::wstring();
		}

		// alloc buffer
		std::vector<wchar_t> buffer(size);

		// convert
		int result = MultiByteToWideChar(932, 0, sjisString.c_str(), -1, buffer.data(), size);
		if (result == 0) {
			DWORD error = GetLastError();
			std::cerr << "MultiByteToWideChar (conversion) failed with error: " << error << std::endl;
			return std::wstring();
		}

		// to std::wstring excluding null termination.
		return std::wstring(buffer.begin(), buffer.end() - 1);
#else
		assert("No implementation.");
		return L"";
#endif
	}

};

