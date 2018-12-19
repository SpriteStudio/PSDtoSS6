#pragma once
#include "common.h"

//ピボット構造体
struct PIVOT
{
	std::string name;
	float x;
	float y;
	bool use;
};

struct tPoint2i
{
	int	x;
	int	y;
};

struct tPoint2d
{
	double x;
	double y;
};

//全角が使われてるかのチェック
bool isZenkaku(const std::string str);
//  文字列を置換する
std::string Replace(std::string String1, std::string String2, std::string String3);
tinyxml2::XMLElement* CreateNewElementWithText(tinyxml2::XMLDocument& xml, std::string elementname, std::string text);
tinyxml2::XMLElement* CreateNewElementWithTextPoint2(tinyxml2::XMLDocument& xml, std::string elementname, int x, int y);
tinyxml2::XMLElement* CreateNewElementWithTextPoint2(tinyxml2::XMLDocument& xml, std::string elementname, float x, float y);
tinyxml2::XMLElement* CreateNewElementWithTextPoint2(tinyxml2::XMLDocument& xml, std::string elementname, double x, double y);
tinyxml2::XMLElement* CreateNewElementWithTextPoint3(tinyxml2::XMLDocument& xml, std::string elementname, int x, int y, int z);
tinyxml2::XMLElement* CreateNewElementWithTextNumber(tinyxml2::XMLDocument& xml, std::string elementname, int x);
//XMLNodeを再帰的にコピー
void deep_copy(tinyxml2::XMLNode* p_dst, tinyxml2::XMLNode* p_src);
void textout(std::string str, FILE* fp);
//sspjを作成
void make_sspj(std::string sspjname, std::string outputname, tinyxml2::XMLDocument* loadssop_xml);
void addAnimeAttribute(tinyxml2::XMLElement* parent, std::string tag, std::string outvalue, bool iptype = false);
void addAnimeCellAttribute(tinyxml2::XMLElement* parent, std::string outvalue);
void AddPartAnime(tinyxml2::XMLElement* parent, SSSpriteSheetPrim* Sheetcell, int type, bool addnull, tinyxml2::XMLDocument* loadssop_xml, tPoint2i gLayoutCenter);
void AddPartValue(tinyxml2::XMLElement* parent, const char* name, int index, int parentIndex, int type);

//メッシュ構造体
struct MESH
{
	//対応するセルの名前
	std::string name;
	//メッシュの有無
	std::string ismesh;
	//分割の種類
	std::string divtype;
	//格子の分割数(水平)
	std::string divw;
	//格子の分割数(垂直)
	std::string divh;
	//分割点の座標
	std::vector<tPoint2d*> pvInnerPoint;
	//頂点の座標
	std::vector<tPoint2d*> pvOuterPoint;
	//最終的にメッシュとして使用される点の座標
	std::vector<tPoint2d*> pvMeshPointList;
	//メッシュとして使用される点の処理順番
	std::vector<int*>     pvMeshTriList;

	enum
	{
		COORD_ORIGIN_CENTER = 0,	//セルの中心を原点とする座標
		COORD_ORIGIN_TOPLEFT		//セルの左上を原点とする座標
	};
	//座標の原点の位置を表す変数
	int coord_origin;

	//対応するセルの名前を取得
	void set_name(tinyxml2::XMLElement* xmlelement);
	//メッシュの有無を取得
	void set_ismesh(tinyxml2::XMLElement* xmlelement);
	//分割の種類を取得
	void set_divtype(tinyxml2::XMLElement* xmlelement);
	//格子の分割数(水平)を取得
	void set_divw(tinyxml2::XMLElement* xmlelement);
	//格子の分割数(垂直)を取得
	void set_divh(tinyxml2::XMLElement* xmlelement);
	//分割点の座標を取得
	void set_pvInnerPoint(tinyxml2::XMLElement* xmlelement);
	//頂点の座標を取得
	void set_pvOuterPoint(tinyxml2::XMLElement* xmlelement);
	//最終的にメッシュとして使用される点の座標を取得
	void set_pvMeshPointList(tinyxml2::XMLElement* xmlelement);
	//メッシュとして使用される点の処理順番を取得
	void set_pvMeshTriList(tinyxml2::XMLElement* xmlelement);
	//stringからdoubleを2つ取りだし、tPoint2dにセット
	static void set_Point2d(const std::string& text, tPoint2d& point2d);
	//stringからintを3つ取りだし、int[3]にセット
	static void set_int3(const std::string& text, int* int3);
	//xmlから座標値(double)を読み込む
	static void add_Point2d(tinyxml2::XMLElement* xmlelement, std::vector<tPoint2d*>& pvPoint2d);
	//xmlから頂点番号(int)を読み込む
	static void add_int(tinyxml2::XMLElement* xmlelement, std::vector<int*>& pvint3);
	//xmlの子要素としてメッシュ情報を書き出し
	void insert_into(tinyxml2::XMLElement* cell);
	//セルの中心を原点にする座標変換
	//mode 0 : 左上->中心
	//mode 1 : 中心->左上
	void trans_coord(tPoint2i& origin, int mode);
	//newで確保したメモリを開放
	static void delete_Point2d(std::vector<tPoint2d*>& pvPoint2d);
	//newで確保したメモリを開放
	static void delete_int(std::vector<int*>& pvint3);
	//内部でnewしたメモリを開放する
	~MESH();
};

//ポスト処理のためのテクスチャクラス
class PostProcessing
{
	//入力テクスチャ
	int **srcTexture;
	//一時テクスチャ
	int **tempTexture;
	//テクスチャ幅
	int width;
	//テクスチャ高さ
	int height;
	//拡大するピクセル数
	const int ex = 1;
public:
	//色漏れ防止処理
	void expandColor(UserBitMapData srcBitMapData);
};
