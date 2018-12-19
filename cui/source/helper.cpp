#include "helper.h"
#include "xml_template.h"
#include "texturepack.h"
#include <math.h>
using namespace tinyxml2;

//全角が使われてるかのチェック
bool isZenkaku(const std::string str)
{
	bool rc = false;
	int i = 0;
	int size = str.length();
	const char *c = str.c_str();

	while (true)
	{
		if ((c[i] == '\0') || (size == i))
		{
			//終了
			break;
		}
		if ((c[i] < 0x20) || (c[i] > 0x7e))
		{
			//半角以外
			rc = true;
			//終了
			break;
		}
		i++;
	}
	return(rc);
}

//  文字列を置換する
std::string Replace(std::string String1, std::string String2, std::string String3)
{
	std::string::size_type  Pos(String1.find(String2));

	while (Pos != std::string::npos)
	{
		String1.replace(Pos, String2.length(), String3);
		Pos = String1.find(String2, Pos + String3.length());
	}

	return String1;
}

tinyxml2::XMLElement* CreateNewElementWithText( tinyxml2::XMLDocument& xml , std::string elementname , std::string text )
{
	tinyxml2::XMLElement* ele = xml.NewElement(elementname.c_str());

	tinyxml2::XMLText* et = xml.NewText(text.c_str());

	ele->InsertEndChild( et );


	return ele;
}

tinyxml2::XMLElement* CreateNewElementWithTextPoint2( tinyxml2::XMLDocument& xml , std::string elementname , int x , int y )
{
	tinyxml2::XMLElement* ele = xml.NewElement(elementname.c_str());

	char buffer[512];
	sprintf( buffer, "%d %d" , x , y );

	tinyxml2::XMLText* et = xml.NewText(buffer);

	ele->InsertEndChild( et );


	return ele;
}

tinyxml2::XMLElement* CreateNewElementWithTextPoint2( tinyxml2::XMLDocument& xml , std::string elementname , float x , float y )
{
	tinyxml2::XMLElement* ele = xml.NewElement(elementname.c_str());

	char buffer[512];
	sprintf( buffer, "%f %f" , x , y );

	tinyxml2::XMLText* et = xml.NewText(buffer);

	ele->InsertEndChild( et );


	return ele;
}

tinyxml2::XMLElement* CreateNewElementWithTextPoint2( tinyxml2::XMLDocument& xml , std::string elementname , double x , double y )
{
	tinyxml2::XMLElement* ele = xml.NewElement(elementname.c_str());

	char buffer[512];

	double dx = floor(x);
	double dy = floor(y);
	int ix = static_cast<int>(dx);
	int iy = static_cast<int>(dy);
	//小数でなければ整数で出力
	if (dx == x && dy == y)
	{
		sprintf( buffer, "%d %d" , ix , iy );
	}
	else if (dx == x && dy != y)
	{
		sprintf( buffer, "%d %.15f" , ix , y );
	}
	else if (dx != x && dy == y)
	{
		sprintf( buffer, "%.15f %d" , x , iy );
	}
	else
	{
		sprintf( buffer, "%.15f %.15f" , x , y );
	}

	tinyxml2::XMLText* et = xml.NewText(buffer);

	ele->InsertEndChild( et );


	return ele;
}

tinyxml2::XMLElement* CreateNewElementWithTextPoint3( tinyxml2::XMLDocument& xml , std::string elementname , int x , int y , int z )
{
	tinyxml2::XMLElement* ele = xml.NewElement(elementname.c_str());

	char buffer[512];
	sprintf( buffer, "%d %d %d" , x , y , z );

	tinyxml2::XMLText* et = xml.NewText(buffer);

	ele->InsertEndChild( et );


	return ele;
}

tinyxml2::XMLElement* CreateNewElementWithTextNumber( tinyxml2::XMLDocument& xml , std::string elementname , int x )
{
	tinyxml2::XMLElement* ele = xml.NewElement(elementname.c_str());

	char buffer[512];
	sprintf( buffer, "%d" , x );

	tinyxml2::XMLText* et = xml.NewText(buffer);

	ele->InsertEndChild( et );


	return ele;
}

//XMLNodeを再帰的にコピー
void deep_copy(tinyxml2::XMLNode* p_dst, tinyxml2::XMLNode* p_src)
{
	if (p_dst == NULL || p_src == NULL)
	{
	}
	else
	{
		for (tinyxml2::XMLNode* p_src_child = p_src->FirstChild(); p_src_child != NULL; p_src_child = p_src_child->NextSibling())
		{
			tinyxml2::XMLNode* p_dst_child = p_src_child->ShallowClone(p_dst->GetDocument());
			deep_copy(p_dst_child, p_src_child);
			p_dst->InsertEndChild(p_dst_child);
		}
	}
};

void textout(std::string str, FILE* fp)
{
#ifndef _WIN32
    str = str + "\n";
    fputs(str.c_str(), fp);
#else
	str = str + "\n";
	setlocale(LC_ALL, "japanese");
	wchar_t	wstr[1024];
	size_t wLen = 0;
	mbstowcs_s(&wLen, wstr, 1024, str.c_str(), _TRUNCATE);
	fputws(wstr, fp);
#endif
}

//sspjを作成
void make_sspj(std::string sspjname, std::string outputname, tinyxml2::XMLDocument* loadssop_xml)
{
	std::string sscename = outputname + ".ssce";
	std::string ssaename = outputname + ".ssae";

	//sspjのテンプレートを作成
	Sspj_template sspj_xml;
	sspj_xml.set_filename(sspjname);
	sspj_xml.make_template(loadssop_xml); //この時点でsspjは上書きされる

	tinyxml2::XMLDocument loadsspj_xml;

	if (tinyxml2::XML_NO_ERROR != loadsspj_xml.LoadFile(sspjname.c_str()))
	{
	}
	else
	{
		//アニメ情報等を出力
		tinyxml2::XMLElement* SpriteStudioProject = loadsspj_xml.FirstChildElement("SpriteStudioProject");
		SpriteStudioProject->FirstChildElement("name")->SetText(outputname.c_str());
		SpriteStudioProject->FirstChildElement("cellmapNames")->FirstChildElement("value")->SetText(sscename.c_str());
		SpriteStudioProject->FirstChildElement("animepackNames")->FirstChildElement("value")->SetText(ssaename.c_str());
		SpriteStudioProject->FirstChildElement("lastAnimeFile")->SetText(ssaename.c_str());
		SpriteStudioProject->FirstChildElement("lastCellMapFile")->SetText(sscename.c_str());

		//sspjを保存
		loadsspj_xml.SaveFile(sspjname.c_str());
	}
}

void addAnimeAttribute(tinyxml2::XMLElement* parent, std::string tag, std::string outvalue, bool iptype)
{
	tinyxml2::XMLDocument& xml = *parent->GetDocument();

	tinyxml2::XMLElement*  attribute = xml.NewElement("attribute");
	parent->InsertEndChild( attribute );

	//タグ
	attribute->SetAttribute("tag" , tag.c_str() );

	tinyxml2::XMLElement*  key = xml.NewElement("key");
	attribute->InsertEndChild( key );
	key->SetAttribute( "time" , "0");
	if (iptype == true)
	{
		key->SetAttribute("ipType", "linear");
	}

	//値
	key->InsertEndChild( CreateNewElementWithText( xml , "value" , outvalue  ) );

}

void addAnimeCellAttribute( tinyxml2::XMLElement* parent ,  std::string outvalue )
{
	tinyxml2::XMLDocument& xml = *parent->GetDocument();

	tinyxml2::XMLElement*  attribute = xml.NewElement("attribute");
	parent->InsertEndChild( attribute );

	//タグ
	attribute->SetAttribute("tag" , "CELL" );

	tinyxml2::XMLElement*  key = xml.NewElement("key");
	attribute->InsertEndChild( key );
	key->SetAttribute( "time" , "0");


	tinyxml2::XMLElement*  value = xml.NewElement("value");
	key->InsertEndChild( value );


	value->InsertEndChild( CreateNewElementWithText( xml , "mapId" , "0" ) );
	value->InsertEndChild( CreateNewElementWithText( xml , "name" , outvalue) );

}

void AddPartAnime(tinyxml2::XMLElement* parent, SSSpriteSheetPrim* Sheetcell, int type, bool addnull, tinyxml2::XMLDocument* loadssop_xml, tPoint2i gLayoutCenter)
{
	tinyxml2::XMLDocument& xml = *parent->GetDocument();

	tinyxml2::XMLElement*  partAnime = xml.NewElement("partAnime");
	parent->InsertEndChild(partAnime);
	switch (type)
	{
	case 0:
		// root
		partAnime->InsertEndChild(CreateNewElementWithText(xml, "partName", "root"));
		break;
	case 1:
		// null
	{
		std::string nullname = "null_" + Sheetcell->name;
		partAnime->InsertEndChild(CreateNewElementWithText(xml, "partName", nullname));
	}
	break;
	case 2:
		// nomral
		partAnime->InsertEndChild(CreateNewElementWithText(xml, "partName", Sheetcell->name));
		break;
	}


	tinyxml2::XMLElement*  attributes = xml.NewElement("attributes");
	partAnime->InsertEndChild(attributes);
	//アトリビュート
	//X Y HIDEだけでいいはず
	{
		//pivotを加味して与えてやる
		int x = 0;
		int y = 0;
		int pri = 0;

		if (
			((type == 1) && (addnull == true))
			|| ((type == 2) && (addnull == false))
			)
		{
			x = (Sheetcell->trim_left + Sheetcell->offset_x) - gLayoutCenter.x + ((float)Sheetcell->trim_w * 0.5f);
			y = gLayoutCenter.y - (Sheetcell->trim_top + Sheetcell->offset_y) - ((float)Sheetcell->trim_h * 0.5f);
		}
		if (type == 2)
		{
			pri = Sheetcell->pri;
			addAnimeCellAttribute(attributes, Sheetcell->name);
		}
#ifndef _WIN32
		addAnimeAttribute(attributes, "POSX", std::to_string((long long)x));
		addAnimeAttribute(attributes, "POSY", std::to_string((long long)y));
		if (pri != 0)
		{
			addAnimeAttribute(attributes, "PRIO", std::to_string((long long)pri));
		}
#else
		addAnimeAttribute(attributes, "POSX", std::to_string((long long)x));
		addAnimeAttribute(attributes, "POSY", std::to_string((long long)y));
		if (pri != 0)
		{
			addAnimeAttribute(attributes, "PRIO", std::to_string((long long)pri));
		}
#endif
		addAnimeAttribute(attributes, "ROTZ", "0");
		addAnimeAttribute(attributes, "HIDE", "0", false);
	}
	if (tinyxml2::XML_NO_ERROR != loadssop_xml->Error())
	{
		//ssopが存在しない場合
		//POSX,POSY,PRIO,ROTZ,HIDEのみ出力
	}
	else
	{
		//ssopが存在する場合
		//ssopの<defaultSetAttributes>に存在するアトリビュートにキーを作成
		tinyxml2::XMLElement* defaultSetAttributes = loadssop_xml->FirstChildElement("SpriteStudioOption")->FirstChildElement("projectSettings")->FirstChildElement("defaultSetAttributes");
		for (tinyxml2::XMLElement* item = defaultSetAttributes->FirstChildElement("item"); item != NULL; item = item->NextSiblingElement("item"))
		{
			std::string attribute_name = item->GetText();
			if (
				attribute_name == "CELL" || // 参照セル
				attribute_name == "POSZ" || //
				attribute_name == "ROTX" || // 回転
				attribute_name == "ROTY" || //
				attribute_name == "IFLH" || // イメージ反転
				attribute_name == "IFLV" || //
				attribute_name == "FLPH" || // 反転
				attribute_name == "FLPV" || //
				attribute_name == "PCOL" || // シンプルカラーブレンド
				attribute_name == "VCOL" || // カラーブレンド
				attribute_name == "VERT" || // 頂点変形
				attribute_name == "PVTX" || // 原点オフセットX
				attribute_name == "PVTY" || // 原点オフセットY
				attribute_name == "ANCX" || // アンカーポイント
				attribute_name == "ANCY" || //
				attribute_name == "UVTX" || // UVアニメ.移動.X
				attribute_name == "UVTY" || // UVアニメ.移動.Y
				attribute_name == "UVRZ" || // UVアニメ.回転
				attribute_name == "BNDR" || // 当たり判定用の半径
				attribute_name == "MASK" || // マスク閾値
				attribute_name == "USER" || // ユーザーデータ
				attribute_name == "IPRM" || // インスタンス用パラメータ
				attribute_name == "EFCT"    // エフェクト用パラメータ
				)
			{
				addAnimeAttribute(attributes, attribute_name, "0");
			}
			else if (
				attribute_name == "SCLX" || // スケール
				attribute_name == "SCLY" || //
				attribute_name == "LSCX" || // ローカルスケール
				attribute_name == "LSCY" || //
				attribute_name == "ALPH" || // 不透明度
				attribute_name == "LALP" || // ローカル不透明度
				attribute_name == "SIZX" || // サイズ
				attribute_name == "SIZY" || //
				attribute_name == "UVSX" || // UVアニメ.スケール.X
				attribute_name == "UVSY"    // UVアニメ.スケール.Y
				)
			{
				addAnimeAttribute(attributes, attribute_name, "1");
			}
		}
	}


}

void AddPartValue(tinyxml2::XMLElement* parent, const char* name, int index, int parentIndex, int type)
{

	tinyxml2::XMLDocument& xml = *parent->GetDocument();

	tinyxml2::XMLElement*  partValue = xml.NewElement("value");
	parent->InsertEndChild( partValue );

	partValue->InsertEndChild( CreateNewElementWithText( xml , "name" , name ) );
#ifndef _WIN32
	partValue->InsertEndChild( CreateNewElementWithText( xml , "arrayIndex" , std::to_string(( unsigned long long)index) ) );
	switch (type)
	{
	case 0:
		// root
		partValue->InsertEndChild(CreateNewElementWithText(xml, "parentIndex", "-1"));
		partValue->InsertEndChild(CreateNewElementWithText(xml, "type", "null"));
		break;
	case 1:
		// null
		partValue->InsertEndChild(CreateNewElementWithText(xml, "parentIndex", std::to_string((unsigned long long)parentIndex)));
		partValue->InsertEndChild(CreateNewElementWithText(xml, "type", "null"));
		break;
	case 2:
		// normal
		partValue->InsertEndChild(CreateNewElementWithText(xml, "parentIndex", std::to_string((unsigned long long)parentIndex)));
		partValue->InsertEndChild(CreateNewElementWithText(xml, "type", "normal"));
		break;
	}
#else
	partValue->InsertEndChild( CreateNewElementWithText( xml , "arrayIndex" , std::to_string((unsigned long long)index) ) );
	switch (type)
	{
	case 0:
		// root
		partValue->InsertEndChild(CreateNewElementWithText(xml, "parentIndex", "-1"));
		partValue->InsertEndChild(CreateNewElementWithText(xml, "type", "null"));
		break;
	case 1:
		// null
		partValue->InsertEndChild(CreateNewElementWithText(xml, "parentIndex", std::to_string((unsigned long long)parentIndex)));
		partValue->InsertEndChild(CreateNewElementWithText(xml, "type", "null"));
		break;
	case 2:
		// normal
		partValue->InsertEndChild(CreateNewElementWithText(xml, "parentIndex", std::to_string((unsigned long long)parentIndex)));
		partValue->InsertEndChild(CreateNewElementWithText(xml, "type", "normal"));
		break;
	}
#endif


	partValue->InsertEndChild( CreateNewElementWithText( xml , "boundsType" , "none" ) );
	if (type == 0)
	{
		partValue->InsertEndChild( CreateNewElementWithText( xml , "inheritType" , "self" ) );
	}
	else
	{
		partValue->InsertEndChild( CreateNewElementWithText( xml , "inheritType" , "parent" ) );
	}

	tinyxml2::XMLElement*  ineheritRates = xml.NewElement("ineheritRates");
	partValue->InsertEndChild( ineheritRates );
	ineheritRates->InsertEndChild( CreateNewElementWithText( xml , "ALPH" , "1" ) );
	ineheritRates->InsertEndChild( CreateNewElementWithText( xml , "FLPH" , "0" ) );
	ineheritRates->InsertEndChild( CreateNewElementWithText( xml , "FLPV" , "0" ) );
	ineheritRates->InsertEndChild( CreateNewElementWithText( xml , "HIDE" , "0" ) );
	ineheritRates->InsertEndChild( CreateNewElementWithText( xml , "IFLH" , "0" ) );
	ineheritRates->InsertEndChild( CreateNewElementWithText( xml , "IFLV" , "0" ) );

	partValue->InsertEndChild( CreateNewElementWithText( xml , "alphaBlendType" , "mix" ) );
	partValue->InsertEndChild( CreateNewElementWithText( xml , "show" , "1" ) );
	partValue->InsertEndChild( CreateNewElementWithText( xml , "locked" , "0" ) );
	partValue->InsertEndChild( CreateNewElementWithText( xml , "expandAttribute" , "0" ) );
	partValue->InsertEndChild( CreateNewElementWithText( xml , "expandChildren" , "1" ) );
}

//対応するセルの名前を取得
void MESH::set_name(XMLElement* xmlelement)
{
	if (xmlelement->GetText() == 0)
	{
		name = "";
	}
	else
	{
		name = xmlelement->GetText();
	}
}

//メッシュの有無を取得
void MESH::set_ismesh(XMLElement* xmlelement)
{
	if (xmlelement->GetText() == 0)
	{
		ismesh = "";
	}
	else
	{
		ismesh = xmlelement->GetText();
	}
}

//分割の種類を取得
void MESH::set_divtype(XMLElement* xmlelement)
{
	if (xmlelement->GetText() == 0)
	{
		divtype = "";
	}
	else
	{
		divtype = xmlelement->GetText();
	}
}

//格子の分割数(水平)を取得
void MESH::set_divw(XMLElement* xmlelement)
{
	if (xmlelement->GetText() == 0)
	{
		divw = "";
	}
	else
	{
		divw = xmlelement->GetText();
	}
}

//格子の分割数(垂直)を取得
void MESH::set_divh(XMLElement* xmlelement)
{
	if (xmlelement->GetText() == 0)
	{
		divh = "";
	}
	else
	{
		divh = xmlelement->GetText();
	}
}

//分割点の座標を取得
void MESH::set_pvInnerPoint(XMLElement* xmlelement)
{
	add_Point2d(xmlelement, pvInnerPoint);
}

//頂点の座標を取得
void MESH::set_pvOuterPoint(XMLElement* xmlelement)
{
	add_Point2d(xmlelement, pvOuterPoint);
}

//最終的にメッシュとして使用される点の座標を取得
void MESH::set_pvMeshPointList(XMLElement* xmlelement)
{
	add_Point2d(xmlelement, pvMeshPointList);
}

//メッシュとして使用される点の処理順番を取得
void MESH::set_pvMeshTriList(XMLElement* xmlelement)
{
	add_int(xmlelement, pvMeshTriList);
}

//stringからdoubleを2つ取りだし、tPoint2dにセット
void MESH::set_Point2d(const std::string& text, tPoint2d& point2d)
{
	int st = text.find(" ");	//区切り文字の位置を取得
	std::string x_text = text.substr(0, st);
	std::string y_text = text.substr(st + 1);

	sscanf(x_text.c_str(), "%lf", &point2d.x);
	sscanf(y_text.c_str(), "%lf", &point2d.y);
}

//stringからintを3つ取りだし、int[3]にセット
void MESH::set_int3(const std::string& text, int* int3)
{
	int st1 = text.find(" ");	//区切り文字の位置を取得
	std::string text1 = text.substr(0, st1);
	std::string temp  = text.substr(st1 + 1);
	int st2 = temp.find(" ");	//区切り文字の位置を取得
	std::string text2 = temp.substr(0, st2);
	std::string text3 = temp.substr(st2+1);

	sscanf(text1.c_str(), "%d", &int3[0]);
	sscanf(text2.c_str(), "%d", &int3[1]);
	sscanf(text3.c_str(), "%d", &int3[2]);
}

//xmlから座標値(double)を読み込む
void MESH::add_Point2d(XMLElement* xmlelement, std::vector<tPoint2d*>& pvPoint2d )
{
	for (XMLElement* value = xmlelement->FirstChildElement("value"); value != NULL; value = value->NextSiblingElement("value"))
	{
		tPoint2d *pPoint2d = new tPoint2d;
		std::string text = value->GetText();
		set_Point2d(text, *pPoint2d);
		pvPoint2d.push_back(pPoint2d);
	}
}

//xmlから頂点番号(int)を読み込む
void MESH::add_int(XMLElement* xmlelement, std::vector<int*>& pvint3)
{
	for (XMLElement* value = xmlelement->FirstChildElement("value"); value != NULL; value = value->NextSiblingElement("value"))
	{
		int *int3 = new int[3];
		std::string text = value->GetText();
		set_int3(text, int3);
		pvint3.push_back(int3);
	}
}

//xmlの子要素としてメッシュ情報を書き出し
void MESH::insert_into(XMLElement* cell)
{
	XMLDocument& xml = *cell->GetDocument();
	cell->InsertEndChild(CreateNewElementWithText(xml, "ismesh", ismesh));
	cell->InsertEndChild(CreateNewElementWithText(xml, "divtype", divtype.c_str()));
	//格子状分割の場合、<divw>,<divh>を出力
	if (divtype == "boxdiv")
	{
		cell->InsertEndChild(CreateNewElementWithText(xml, "divw", divw.c_str()));
		cell->InsertEndChild(CreateNewElementWithText(xml, "divh", divh.c_str()));
	}
	//<value>がない場合でも、<innerPoint />は出力される
	XMLElement* innerPoint = xml.NewElement("innerPoint");
	cell->InsertEndChild(innerPoint);
	for (std::vector<tPoint2d*>::iterator it = pvInnerPoint.begin(); it != pvInnerPoint.end(); it++)
	{
		innerPoint->InsertEndChild(CreateNewElementWithTextPoint2(xml, "value", (*it)->x, (*it)->y));
	}
	//<value>がない場合でも、<outerPoint />は出力される
	XMLElement* outerPoint = xml.NewElement("outerPoint");
	cell->InsertEndChild(outerPoint);
	for (std::vector<tPoint2d*>::iterator it = pvOuterPoint.begin(); it != pvOuterPoint.end(); it++)
	{
		outerPoint->InsertEndChild(CreateNewElementWithTextPoint2(xml, "value", (*it)->x, (*it)->y));
	}
	//<value>がない場合でも、<meshPointList />は出力される
	XMLElement* meshPointList = xml.NewElement("meshPointList");
	cell->InsertEndChild(meshPointList);
	for (std::vector<tPoint2d*>::iterator it = pvMeshPointList.begin(); it != pvMeshPointList.end(); it++)
	{
		meshPointList->InsertEndChild(CreateNewElementWithTextPoint2(xml, "value", (*it)->x, (*it)->y));
	}
	//<value>がない場合でも、<meshTriList />は出力される
	XMLElement* meshTriList = xml.NewElement("meshTriList");
	cell->InsertEndChild(meshTriList);
	for (std::vector<int*> ::iterator it = pvMeshTriList.begin(); it != pvMeshTriList.end(); it++)
	{
		meshTriList->InsertEndChild(CreateNewElementWithTextPoint3(xml, "value", (*it)[0], (*it)[1], (*it)[2]));
	}
}

void MESH::trans_coord(tPoint2i & origin, int mode)
{
	if (mode == 0)
	{
		//左上->中心
		for (std::vector<tPoint2d*>::iterator it = pvInnerPoint.begin(); it != pvInnerPoint.end(); it++)
		{
			(*it)->x = (*it)->x * 2.0 - origin.x;
			(*it)->y = (*it)->y * 2.0 - origin.y;
		}

		for (std::vector<tPoint2d*>::iterator it = pvOuterPoint.begin(); it != pvOuterPoint.end(); it++)
		{
			(*it)->x = (*it)->x * 2.0 - origin.x;
			(*it)->y = (*it)->y * 2.0 - origin.y;
		}

		for (std::vector<tPoint2d*>::iterator it = pvMeshPointList.begin(); it != pvMeshPointList.end(); it++)
		{
			(*it)->x = (*it)->x * 2.0 - origin.x;
			(*it)->y = (*it)->y * 2.0 - origin.y;
		}
	}
	else if (mode == 1)
	{
		//中心->左上
		for (std::vector<tPoint2d*>::iterator it = pvInnerPoint.begin(); it != pvInnerPoint.end(); it++)
		{
			(*it)->x = ((*it)->x + origin.x) / 2.0;
			(*it)->y = ((*it)->y + origin.y) / 2.0;
		}

		for (std::vector<tPoint2d*>::iterator it = pvOuterPoint.begin(); it != pvOuterPoint.end(); it++)
		{
			(*it)->x = ((*it)->x + origin.x) / 2.0;
			(*it)->y = ((*it)->y + origin.y) / 2.0;
		}

		for (std::vector<tPoint2d*>::iterator it = pvMeshPointList.begin(); it != pvMeshPointList.end(); it++)
		{
			(*it)->x = ((*it)->x + origin.x) / 2.0;
			(*it)->y = ((*it)->y + origin.y) / 2.0;
		}
	}
}

//newで確保したメモリを開放
void MESH::delete_Point2d(std::vector<tPoint2d*>& pvPoint2d)
{
	for (std::vector<tPoint2d*>::iterator it = pvPoint2d.begin(); it != pvPoint2d.end(); it++)
	{
		delete(*it);
	}
}

//newで確保したメモリを開放
void MESH::delete_int(std::vector<int*>& pvint3)
{
	for (std::vector<int*>::iterator it = pvint3.begin(); it != pvint3.end(); it++)
	{
		delete[](*it);
	}
}

//内部でnewしたメモリを開放する
MESH::~MESH()
{
	delete_Point2d(pvInnerPoint);
	delete_Point2d(pvOuterPoint);
	delete_Point2d(pvMeshPointList);
	delete_int(pvMeshTriList);
}


//色漏れ防止処理
void PostProcessing::expandColor(UserBitMapData srcBitMapData)
{
	//テクスチャ幅、高さを取得
	height = srcBitMapData.Height;
	width = srcBitMapData.Width;

	//テスクチャの作成と初期化
	tempTexture = new int *[height + ex * 2];
	srcTexture = new int *[height];
	for (int y = 0; y < height + ex * 2; y++)
	{
		tempTexture[y] = new int[width + ex * 2];
		for (int x = 0; x < width + ex * 2; x++)
		{
			tempTexture[y][x] = 0;
		}
	}
	for (int y = 0; y < height; y++)
	{
		srcTexture[y] = srcBitMapData.Data + width * y;
	}

	//srcから一時テクスチャにピクセル情報をコピー
	for (int y = 0; y < height; y++)
	{
		int y_ = y + ex;
		for (int x = 0; x < width; x++)
		{
			int x_ = x + ex;
			tempTexture[y_][x_] = srcTexture[y][x];
		}
	}

	//右に色を拡大
	for (int y = 0; y < height; y++)
	{
		int y_ = y + ex;
		for (int x = 0; x < width; x++)
		{
			int x_ = x + ex;
			//アルファ値の境界で色の拡大を行う
			if (tempTexture[y_][x_] >> 24 != 0 && tempTexture[y_][x_ + 1] >> 24 == 0)
			{
				for (int i = 1; i <= ex; i++)
				{
					//srcをはみ出さない
					if (x + i < width)
					{
						//アルファ値がゼロのピクセルにのみ処理を行う
						if (tempTexture[y_][x_ + i] >> 24 == 0)
						{
							srcTexture[y][x + i] = tempTexture[y_][x_];
							char* alpha = (char*)&srcTexture[y][x + i];
							alpha[3] = 0;
						}
					}
				}
			}
		}
	}
	//下に色を拡大
	for (int y = 0; y < height; y++)
	{
		int y_ = y + ex;
		for (int x = 0; x < width; x++)
		{
			int x_ = x + ex;
			//アルファ値の境界で色の拡大を行う
			if (tempTexture[y_][x_] >> 24 != 0 && tempTexture[y_ + 1][x_] >> 24 == 0)
			{
				for (int i = 1; i <= ex; i++)
				{
					//srcをはみ出さない
					if (y + i < height)
					{
						//アルファ値がゼロのピクセルにのみ処理を行う
						if (tempTexture[y_ + i][x_] >> 24 == 0)
						{
							srcTexture[y + i][x] = tempTexture[y_][x_];
							char* alpha = (char*)&srcTexture[y + i][x];
							alpha[3] = 0;
						}
					}
				}
			}
		}
	}
	//左に色を拡大
	for (int y = 0; y < height; y++)
	{
		int y_ = y + ex;
		for (int x = 0; x < width; x++)
		{
			int x_ = x + ex;
			//アルファ値の境界で色の拡大を行う
			if (tempTexture[y_][x_] >> 24 != 0 && tempTexture[y_][x_ - 1] >> 24 == 0)
			{
				for (int i = 1; i <= ex; i++)
				{
					//srcをはみ出さない
					if (x - i >= 0)
					{
						//アルファ値がゼロのピクセルにのみ処理を行う
						if (tempTexture[y_][x_ - i] >> 24 == 0)
						{
							srcTexture[y][x - i] = tempTexture[y_][x_];
							char* alpha = (char*)&srcTexture[y][x - i];
							alpha[3] = 0;
						}
					}
				}
			}
		}
	}
	//上に色を拡大
	for (int y = 0; y < height; y++)
	{
		int y_ = y + ex;
		for (int x = 0; x < width; x++)
		{
			int x_ = x + ex;
			//アルファ値の境界で色の拡大を行う
			if (tempTexture[y_][x_] >> 24 != 0 && tempTexture[y_ - 1][x_] >> 24 == 0)
			{
				for (int i = 1; i <= ex; i++)
				{
					//srcをはみ出さない
					if (y - i >= 0)
					{
						//アルファ値がゼロのピクセルにのみ処理を行う
						if (tempTexture[y_ - i][x_] >> 24 == 0)
						{
							srcTexture[y - i][x] = tempTexture[y_][x_];
							char* alpha = (char*)&srcTexture[y - i][x];
							alpha[3] = 0;
						}
					}
				}
			}
		}
	}

	//一時テスクチャを削除
	for (int y = 0; y < height + ex * 2; y++)
	{
		delete[] tempTexture[y];
	}
	delete[] tempTexture;
	delete[] srcTexture;
}
