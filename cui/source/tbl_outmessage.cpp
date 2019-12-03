#include "tbl_outmessage.h"

#include <string>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>

struct MESSAGE_TABLE {
	int message_no;
	char* local[2];
};


static MESSAGE_TABLE tbl_message[] =
{
	{
		INFO_0001, 
		"テクスチャサイズを %d × %d にしてパッキングします。",
		"Pack with a texture size of %d x %d."
	},
	{
		INFO_0002, // 
		"パッキングが成功しました。",
		"Packing was successful."
	},
	{
		INFO_0003, // 
		"ssceファイルのコンバートが成功しました。",
		"ssce convert success!!: %s"
	},
	{
		INFO_0004, // 
		"ssaeファイルのコンバートが成功しました。",
		"ssae convert success!!: %s"
	},
	{
		INFO_0005, // 
		"sspjファイルのコンバートが成功しました。",
		"sspj convert success!!: %s"
	},

	{
		WARNING_0001, 
		"警告：セル間余白+セル内余白の値が1以下に指定されています。色もれが発生する可能性があります。",
		"Warning: The value of margin between cells + margin within the cell is specified as 1 or less. Color leakage may occur."
	},

	{
		ERROR_0001,
		"エラー：情報ファイルのオープンに失敗しました。：%s",
		"Error: Failed to open the information file."
	},
	{
		ERROR_0002, 
		"エラー：情報ファイルの内容が正しくありません。：%s",
		"Error: The contents of the information file are incorrect. : %s"
	},
	{
		ERROR_0003,
		"エラー：PSDファイルのオープンに失敗しました。：%s",
		"Error: Failed to open PSD file. : %s"
	},
	{
		ERROR_0004,
		"エラー：セル名に全角文字が使用されています。: %s  ",
		"Error: Double - byte characters are used in the cell name. : %s"
	},
	{
		ERROR_0005,
		"エラー：同名のセル名が使用されています。 : %s",
		"Error: The same cell name is used."
	},
	{
		ERROR_0006, // 
		"エラー：セルマップに収まりません",
		"Error: does not fit in cell map"
	},
	{
		ERROR_0007, // 
		"エラー：テクスチャの幅より大きいセルがあります。",
		"Error: There are cells larger than the width of the texture."
	},
	{
		ERROR_0008, 
		"エラー：テクスチャの高さより大きいセルがあります。",
		"Error: There are cells larger than the height of the texture."
	},
	{
		ERROT_0009,
		"パッキングに失敗しました。",
		"Packing failed."
	},
	{
		ERROT_0010 ,
		"設定情報(jsonファイル)の読み込みに失敗しました。"
		"failed to read test.json"
	},
	{
		ERROT_0011, // 
		"コンバート情報のオープンに失敗しました",
		"Failed to open convert information file"
	}

};


static int getMessageIndexFromID(int id)
{
	for (int i = 0; i < sizeof(tbl_message) / sizeof(MESSAGE_TABLE); i++)
	{
		if (tbl_message[i].message_no == id) return i;
	}
	return 0;
}


void ConsoleOutMessage(int MessageNumber, ...)
//std::string ConsoleOutMessage(int MessageNumber, ...)
{
	va_list args;

	int index = getMessageIndexFromID(MessageNumber);
	const char* format = tbl_message[index].local[LOCAL_JP];
	char buf[1024];

	va_start(args, MessageNumber);
//	vsprintf_s(buf , 1024 , format, args);
	vsnprintf(buf, 1023, format, args);
	va_end(args);

	//return std::string(buf);
	std::ios_base::sync_with_stdio(false);
	std::locale default_loc("");
	std::locale::global(default_loc);
	std::locale ctype_default(std::locale::classic(), default_loc, std::locale::ctype); //※
	std::wcout.imbue(ctype_default);

	std::cout << buf << std::endl;
}




