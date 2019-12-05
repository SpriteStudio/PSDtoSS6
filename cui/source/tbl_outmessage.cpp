#include "tbl_outmessage.h"

#include <string>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "../picojson/picojson.h"
#include "stringconv.h"



struct LOCAL_MESSAGE {
	std::string localmessage[LOCAL_TEAM];
};

std::map<std::string, LOCAL_MESSAGE> message_dic;


//message.jsonをbase64でエンコードした文字列
const char* base64_message = "Ww0KCXsNCgkJImxhYmVsIjoiSU5GT18wMDAxIiwgDQoJCSJqcCI6IuODhuOCr+OCueODgeODo+OCteOCpOOCuuOCkiAlZCDDlyAlZCDjgavjgZfjgabjg5Hjg4Pjgq3jg7PjgrDjgZfjgb7jgZnjgIIiLA0KCQkiZW4iOiJQYWNrIHdpdGggYSB0ZXh0dXJlIHNpemUgb2YgJWQgeCAlZC4iDQoJfSwNCgl7DQoJCSJsYWJlbCIgOiAiSU5GT18wMDAyIiwNCgkJImpwIjoi44OR44OD44Kt44Oz44Kw44GM5oiQ5Yqf44GX44G+44GX44Gf44CCIiwNCgkJImVuIjoiUGFja2luZyB3YXMgc3VjY2Vzc2Z1bC4iDQoJfSwNCgl7DQoJCSJsYWJlbCIgOiAiSU5GT18wMDAzIiAsDQoJCSJqcCIgOiAic3NjZeODleOCoeOCpOODq+OBruOCs+ODs+ODkOODvOODiOOBjOaIkOWKn+OBl+OBvuOBl+OBn+OAgiIsDQoJCSJlbiI6ICJzc2NlIGNvbnZlcnQgc3VjY2VzcyEhOiAlcyINCgl9LA0KCXsNCgkJImxhYmVsIjogIklORk9fMDAwNCIsDQoJCSJqcCIgOiAic3NhZeODleOCoeOCpOODq+OBruOCs+ODs+ODkOODvOODiOOBjOaIkOWKn+OBl+OBvuOBl+OBn+OAgiIsDQoJCSJlbiI6ICJzc2FlIGNvbnZlcnQgc3VjY2VzcyEhOiAlcyINCgl9LA0KCXsNCgkJImxhYmVsIjogIklORk9fMDAwNSIsIA0KCQkianAiIDogInNzcGrjg5XjgqHjgqTjg6vjga7jgrPjg7Pjg5Djg7zjg4jjgYzmiJDlip/jgZfjgb7jgZfjgZ/jgIIiLA0KCQkiZW4iOiAic3NwaiBjb252ZXJ0IHN1Y2Nlc3MhITogJXMiDQoJfSwNCg0KCXsNCgkJImxhYmVsIiA6ICJXQVJOSU5HXzAwMDEiLCANCgkJImpwIiA6ICLorablkYrvvJrjgrvjg6vplpPkvZnnmb0r44K744Or5YaF5L2Z55m944Gu5YCk44GMMeS7peS4i+OBq+aMh+WumuOBleOCjOOBpuOBhOOBvuOBmeOAguiJsuOCguOCjOOBjOeZuueUn+OBmeOCi+WPr+iDveaAp+OBjOOBguOCiuOBvuOBmeOAgiIsDQoJCSJlbiI6ICJXYXJuaW5nOiBUaGUgdmFsdWUgb2YgbWFyZ2luIGJldHdlZW4gY2VsbHMgKyBtYXJnaW4gd2l0aGluIHRoZSBjZWxsIGlzIHNwZWNpZmllZCBhcyAxIG9yIGxlc3MuIENvbG9yIGxlYWthZ2UgbWF5IG9jY3VyLiINCgl9LA0KDQoJew0KCQkibGFiZWwiIDogIkVSUk9SXzAwMDEiLA0KCQkianAiIDogIuOCqOODqeODvO+8muaDheWgseODleOCoeOCpOODq+OBruOCquODvOODl+ODs+OBq+WkseaVl+OBl+OBvuOBl+OBn+OAgu+8miVzIiwNCgkJImVuIjogIkVycm9yOiBGYWlsZWQgdG8gb3BlbiB0aGUgaW5mb3JtYXRpb24gZmlsZS4iDQoJfSwNCgl7DQoJCSJsYWJlbCIgOiAiRVJST1JfMDAwMiIsIA0KCQkianAiIDogIuOCqOODqeODvO+8muaDheWgseODleOCoeOCpOODq+OBruWGheWuueOBjOato+OBl+OBj+OBguOCiuOBvuOBm+OCk+OAgu+8miVzIiwNCgkJImVuIjogIkVycm9yOiBUaGUgY29udGVudHMgb2YgdGhlIGluZm9ybWF0aW9uIGZpbGUgYXJlIGluY29ycmVjdC4gOiAlcyINCgl9LA0KCXsNCgkJImxhYmVsIiA6ICJFUlJPUl8wMDAzIiwNCgkJImpwIiA6ICLjgqjjg6njg7zvvJpQU0Tjg5XjgqHjgqTjg6vjga7jgqrjg7zjg5fjg7PjgavlpLHmlZfjgZfjgb7jgZfjgZ/jgILvvJolcyIsDQoJCSJlbiI6ICJFcnJvcjogRmFpbGVkIHRvIG9wZW4gUFNEIGZpbGUuIDogJXMiDQoJfSwNCgl7DQoJCSJsYWJlbCIgOiAiRVJST1JfMDAwNCIsDQoJCSJqcCIgOiAi44Ko44Op44O877ya44K744Or5ZCN44Gr5YWo6KeS5paH5a2X44GM5L2/55So44GV44KM44Gm44GE44G+44GZ44CCOiAlcyAgIiwNCgkJImVuIjogIkVycm9yOiBEb3VibGUgLSBieXRlIGNoYXJhY3RlcnMgYXJlIHVzZWQgaW4gdGhlIGNlbGwgbmFtZS4gOiAlcyINCgl9LA0KCXsNCgkJImxhYmVsIiA6ICJFUlJPUl8wMDA1IiAsDQoJCSJqcCIgOiAi44Ko44Op44O877ya5ZCM5ZCN44Gu44K744Or5ZCN44GM5L2/55So44GV44KM44Gm44GE44G+44GZ44CCIDogJXMiLA0KCQkiZW4iOiAiRXJyb3I6IFRoZSBzYW1lIGNlbGwgbmFtZSBpcyB1c2VkLiINCgl9LA0KCXsNCgkJImxhYmVsIiA6ICJFUlJPUl8wMDA2IiwNCgkJImpwIiA6ICLjgqjjg6njg7zvvJrjgrvjg6vjg57jg4Pjg5fjgavlj47jgb7jgorjgb7jgZvjgpMiLA0KCQkiZW4iOiAiRXJyb3I6IGRvZXMgbm90IGZpdCBpbiBjZWxsIG1hcCINCgl9LA0KCXsNCgkJImxhYmVsIiA6ICJFUlJPUl8wMDA3IiwNCgkJImpwIiA6ICLjgqjjg6njg7zvvJrjg4bjgq/jgrnjg4Hjg6Pjga7luYXjgojjgorlpKfjgY3jgYTjgrvjg6vjgYzjgYLjgorjgb7jgZnjgIIiLA0KCQkiZW4iOiAiRXJyb3I6IFRoZXJlIGFyZSBjZWxscyBsYXJnZXIgdGhhbiB0aGUgd2lkdGggb2YgdGhlIHRleHR1cmUuIg0KCX0sDQoJew0KCQkibGFiZWwiIDogIkVSUk9SXzAwMDgiLCANCgkJImpwIiA6ICLjgqjjg6njg7zvvJrjg4bjgq/jgrnjg4Hjg6Pjga7pq5jjgZXjgojjgorlpKfjgY3jgYTjgrvjg6vjgYzjgYLjgorjgb7jgZnjgIIiLA0KCQkiZW4iOiAiRXJyb3I6IFRoZXJlIGFyZSBjZWxscyBsYXJnZXIgdGhhbiB0aGUgaGVpZ2h0IG9mIHRoZSB0ZXh0dXJlLiINCgl9LA0KCXsNCgkJImxhYmVsIiA6ICJFUlJPVF8wMDA5IiwNCgkJImpwIiA6ICLjg5Hjg4Pjgq3jg7PjgrDjgavlpLHmlZfjgZfjgb7jgZfjgZ/jgIIiLA0KCQkiZW4iOiAiUGFja2luZyBmYWlsZWQuIg0KCX0sDQoJew0KCQkibGFiZWwiIDogIkVSUk9UXzAwMTAiICwNCgkJImpwIiA6ICLoqK3lrprmg4XloLEoanNvbuODleOCoeOCpOODqynjga7oqq3jgb/ovrzjgb/jgavlpLHmlZfjgZfjgb7jgZfjgZ/jgIIiLA0KCQkiZW4iOiAiZmFpbGVkIHRvIHJlYWQgdGVzdC5qc29uIg0KCX0sDQoJew0KCQkibGFiZWwiIDogIkVSUk9UXzAwMTEiLA0KCQkianAiIDogIuOCs+ODs+ODkOODvOODiOaDheWgseOBruOCquODvOODl+ODs+OBq+WkseaVl+OBl+OBvuOBl+OBnyIsDQoJCSJlbiI6ICJGYWlsZWQgdG8gb3BlbiBjb252ZXJ0IGluZm9ybWF0aW9uIGZpbGUiDQoJfQ0KXQ0K";
static const std::string b = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";//=
typedef unsigned char uchar;

static std::string base64_decode(const std::string &in) {

	std::string out;

	std::vector<int> T(256, -1);
	for (int i = 0; i < 64; i++) T[b[i]] = i;

	int val = 0, valb = -8;
	for (uchar c : in) {
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0) {
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return out;
}



static std::string getMessageIndexFromID(std::string str , OUTMESSAGE_LOCAL loc)
{
	return message_dic[str].localmessage[(int)loc];

}


void MessageInit()
{
	std::string json = base64_decode(base64_message);
	
	picojson::value v;
	const std::string err = picojson::parse(v, json);

	if (err.empty() == false) {
		std::cerr << err << std::endl;
		return;
	}

	auto arr = v.get<picojson::array>();

	for (auto i = 0; i < arr.size(); i++)
	{
		auto obj = arr[i].get < picojson::object>();
		auto   l = obj["label"].get<std::string>();
		auto   jp = obj["jp"].get<std::string>();
		auto   en = obj["en"].get<std::string>();

#ifdef _WIN32 //sjisへ
		jp = stringconv::utf8_to_sjis(jp);
#endif

		LOCAL_MESSAGE message;
		message.localmessage[LOCAL_JP] = jp;
		message.localmessage[LOCAL_EN] = en;

		message_dic[l] = message;
	}

}



void ConsoleOutMessage(std::string MessageNumber, ...)
{
	va_list args;


	std::string format = getMessageIndexFromID(MessageNumber, LOCAL_JP);

	char buf[1024];

	va_start(args, MessageNumber);
	vsnprintf(buf, 1023, format.c_str(), args);
	va_end(args);

	//return std::string(buf);
	std::ios_base::sync_with_stdio(false);
	std::locale default_loc("");
	std::locale::global(default_loc);
	std::locale ctype_default(std::locale::classic(), default_loc, std::locale::ctype); //※
	std::wcout.imbue(ctype_default);

//	std::cout << buf << std::endl;
	std::cerr << buf << std::endl;
}




