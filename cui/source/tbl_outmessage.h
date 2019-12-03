#ifndef  __TBL_OUTMESSAGE__
#define  __TBL_OUTMESSAGE__


enum {
	INFO_0000 = 0 ,
	INFO_0001, // "テクスチャサイズを %d × %d にしてパッキングします。\n";
	INFO_0002, // "パッキング成功！";

	INFO_0003, // ssce convert success!!: " << sscename
	INFO_0004, // ssae convert success!!: " << sscename
	INFO_0005, // sspj convert success!!: " << sscename

	WARNING_0000 = 0x1000,

	WARNING_0001, //警告：セル間余白+セル内余白の値が1以下に指定されています。色もれが発生する可能性があります。


	ERROR_0000 = 0x2000,

	ERROR_0001,	// "エラー：情報ファイルのオープンに失敗しました。：" << inputname << "\n";
	ERROR_0002, // "エラー：情報ファイルの内容が正しくありません。：" << inputname << "\n";
	ERROR_0003, // "エラー：PSDファイルのオープンに失敗しました。：" << inputname
	ERROR_0004, // "エラー：セル名に全角文字が使用されています。 ",
	ERROR_0005, // "エラー：同名のセル名が使用されています。 " 
	ERROR_0006, // "エラー：セルマップに収まりません\n";
	ERROR_0007, // "エラー：テクスチャの幅より大きいセルがあります。\n";
	ERROR_0008, // エラー：テクスチャの高さより大きいセルがあります。
	ERROT_0009, // std::cerr << "パッキングに失敗しました。\n";
	ERROT_0010 ,// std::cerr << "failed to read test.json" << std::endl;
	ERROT_0011, // コンバート情報のオープンに失敗しました

	MESSAGE_TERM
};


enum {
	LOCAL_JP = 0,
	LOCAL_EN
};


void ConsoleOutMessage(int MessageNumber, ...);


#endif