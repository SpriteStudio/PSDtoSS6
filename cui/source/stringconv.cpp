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

		// 1. 必要なバッファサイズの取得
		int size = MultiByteToWideChar(
			932, // コードページ: 932 は Shift-JIS
			0,     // フラグ: 通常は 0
			sjisString.c_str(),
			-1,    // 入力文字列は null 終端
			nullptr,
			0       // 出力バッファのサイズは 0 を指定して必要なサイズを取得
		);

		if (size == 0) {
			DWORD error = GetLastError();
			std::cerr << "MultiByteToWideChar (size) failed with error: " << error << std::endl;
			return std::wstring();
		}

		// 2. バッファの確保
		std::vector<wchar_t> buffer(size);

		// 3. 実際の変換
		int result = MultiByteToWideChar(
			932, // コードページ: 932 は Shift-JIS
			0,     // フラグ: 通常は 0
			sjisString.c_str(),
			-1,    // 入力文字列は null 終端
			buffer.data(),
			size    // 出力バッファのサイズ
		);

		if (result == 0) {
			DWORD error = GetLastError();
			std::cerr << "MultiByteToWideChar (conversion) failed with error: " << error << std::endl;
			return std::wstring();
		}

		// 4. std::wstring の作成 (null 終端を含まない)
		return std::wstring(buffer.begin(), buffer.end() - 1);
#else
		assert("No implementation.");
		return L"";
#endif
	}

};

