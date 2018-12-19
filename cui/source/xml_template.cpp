#include "xml_template.h"
using namespace tinyxml2;

//XMLNodeを再帰的にコピー
static void deep_copy(XMLNode* p_dst, XMLNode* p_src)
{
	if (p_dst == NULL || p_src == NULL)
	{
	}
	else
	{
		for (XMLNode* p_src_child = p_src->FirstChild(); p_src_child != NULL; p_src_child = p_src_child->NextSibling())
		{
			XMLNode* p_dst_child = p_src_child->ShallowClone(p_dst->GetDocument());
			deep_copy(p_dst_child, p_src_child);
			p_dst->InsertEndChild(p_dst_child);
		}
	}
};

static void textout(std::string str, FILE* fp)
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

void Ssxx_template::set_filename(std::string filename_)
{
	filename = filename_;
}

//テンプレートを作成
//ssopが存在すれば「新規作成時のプロジェクトのデフォルト設定」を反映する
void Ssxx_template::make_template(XMLDocument* loadssop_xml)
{
	if (tinyxml2::XML_NO_ERROR != loadssop_xml->Error())
	{
		make_template_form_default();
	}
	else
	{
		make_template_from_ssop(loadssop_xml);
	}
}

//SSのデフォルト値でテンプレートを作成
void Sspj_template::make_template_form_default()
{
	//テンプレートを新規作成
	FILE* fp = fopen(filename.c_str(), "w");
	if (fp != NULL)
	{
		std::string str;
		textout("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>", fp);
		textout("<SpriteStudioProject version=\"2.00.00\">", fp);
		textout("	<name></name>", fp);
		textout("	<exportPath></exportPath>", fp);
		textout("	<settings>", fp);
		textout("		<animeBaseDirectory></animeBaseDirectory>", fp);
		textout("		<cellMapBaseDirectory></cellMapBaseDirectory>", fp);
		textout("		<imageBaseDirectory></imageBaseDirectory>", fp);
		textout("		<effectBaseDirectory></effectBaseDirectory>", fp);
		textout("		<exportBaseDirectory>Export</exportBaseDirectory>", fp);
		textout("		<queryExportBaseDirectory>1</queryExportBaseDirectory>", fp);
		textout("		<ssaxImport>", fp);
		textout("			<convertImageOffsetToCellAnime>1</convertImageOffsetToCellAnime>", fp);
		textout("			<ignorePivotAsCellRegister>1</ignorePivotAsCellRegister>", fp);
		textout("			<keepAngleAsIsIfNegativeScale>1</keepAngleAsIsIfNegativeScale>", fp);
		textout("		</ssaxImport>", fp);
		textout("		<copyWhenImportImageIsOutside>1</copyWhenImportImageIsOutside>", fp);
		textout("		<exportAnimeFileFormat>SSAX</exportAnimeFileFormat>", fp);
		textout("		<exportCellMapFileFormat>invalid</exportCellMapFileFormat>", fp);
		textout("		<exportCellMap>0</exportCellMap>", fp);
		textout("		<copyImageWhenExportCellmap>1</copyImageWhenExportCellmap>", fp);
		textout("		<ssConverterOptions></ssConverterOptions>", fp);
		textout("		<ssaxExport>", fp);
		textout("			<addAnimeNameEvenIfSingle>1</addAnimeNameEvenIfSingle>", fp);
		textout("			<listAllCellMaps>1</listAllCellMaps>", fp);
		textout("			<connectAnimations>0</connectAnimations>", fp);
		textout("			<zRotCurveParamAsRadian>1</zRotCurveParamAsRadian>", fp);
		textout("			<reverseZRotSignForNegativeScale>1</reverseZRotSignForNegativeScale>", fp);
		textout("			<numberOfPivotDecimals>0</numberOfPivotDecimals>", fp);
		textout("		</ssaxExport>", fp);
		textout("		<player>any</player>", fp);
		textout("		<strictVer4>0</strictVer4>", fp);
		textout("		<dontUseMatrixForTransform>0</dontUseMatrixForTransform>", fp);
		textout("		<rootPartFunctionAsVer4>0</rootPartFunctionAsVer4>", fp);
		textout("		<interpolateColorBlendAsVer4>1</interpolateColorBlendAsVer4>", fp);
		textout("		<interpolateVertexOffsetAsVer4>1</interpolateVertexOffsetAsVer4>", fp);
		textout("		<restrictXYAsInteger>0</restrictXYAsInteger>", fp);
		textout("		<inheritRatesNoKeySave>1</inheritRatesNoKeySave>", fp);
		textout("		<availableInterpolationTypes>", fp);
		textout("			<item>none</item>", fp);
		textout("			<item>linear</item>", fp);
		textout("			<item>hermite</item>", fp);
		textout("			<item>bezier</item>", fp);
		textout("			<item>acceleration</item>", fp);
		textout("			<item>deceleration</item>", fp);
		textout("		</availableInterpolationTypes>", fp);
		textout("		<availableAttributes>", fp);
		textout("			<item>CELL</item>", fp);
		textout("			<item>POSX</item>", fp);
		textout("			<item>POSY</item>", fp);
		textout("			<item>POSZ</item>", fp);
		textout("			<item>ROTX</item>", fp);
		textout("			<item>ROTY</item>", fp);
		textout("			<item>ROTZ</item>", fp);
		textout("			<item>SCLX</item>", fp);
		textout("			<item>SCLY</item>", fp);
		textout("			<item>LSCX</item>", fp);
		textout("			<item>LSCY</item>", fp);
		textout("			<item>ALPH</item>", fp);
		textout("			<item>LALP</item>", fp);
		textout("			<item>PRIO</item>", fp);
		textout("			<item>IFLH</item>", fp);
		textout("			<item>IFLV</item>", fp);
		textout("			<item>FLPH</item>", fp);
		textout("			<item>FLPV</item>", fp);
		textout("			<item>HIDE</item>", fp);
		textout("			<item>PCOL</item>", fp);
		textout("			<item>VCOL</item>", fp);
		textout("			<item>VERT</item>", fp);
		textout("			<item>PVTX</item>", fp);
		textout("			<item>PVTY</item>", fp);
		textout("			<item>ANCX</item>", fp);
		textout("			<item>ANCY</item>", fp);
		textout("			<item>SIZX</item>", fp);
		textout("			<item>SIZY</item>", fp);
		textout("			<item>UVTX</item>", fp);
		textout("			<item>UVTY</item>", fp);
		textout("			<item>UVRZ</item>", fp);
		textout("			<item>UVSX</item>", fp);
		textout("			<item>UVSY</item>", fp);
		textout("			<item>BNDR</item>", fp);
		textout("			<item>MASK</item>", fp);
		textout("			<item>USER</item>", fp);
		textout("			<item>IPRM</item>", fp);
		textout("			<item>EFCT</item>", fp);
		textout("		</availableAttributes>", fp);
		textout("		<availableFeatures>", fp);
		textout("			<value>bone</value>", fp);
		textout("			<value>effect</value>", fp);
		textout("			<value>mask</value>", fp);
		textout("			<value>mesh</value>", fp);
		textout("		</availableFeatures>", fp);
		textout("		<defaultSetAttributes>", fp);
		textout("			<item>POSX</item>", fp);
		textout("			<item>POSY</item>", fp);
		textout("			<item>ROTZ</item>", fp);
		textout("			<item>PRIO</item>", fp);
		textout("			<item>HIDE</item>", fp);
		textout("		</defaultSetAttributes>", fp);
		textout("		<wrapMode>clamp</wrapMode>", fp);
		textout("		<filterMode>linear</filterMode>", fp);
		textout("		<interpolationType>linear</interpolationType>", fp);
		textout("		<renderingSettings>", fp);
		textout("			<outputFolder></outputFolder>", fp);
		textout("			<outputType>AVI</outputType>", fp);
		textout("			<bgColor>FF606060</bgColor>", fp);
		textout("			<addAnimeName>0</addAnimeName>", fp);
		textout("			<addTimeStamp>0</addTimeStamp>", fp);
		textout("			<addAlphaChanel>1</addAlphaChanel>", fp);
		textout("			<imageSizeRatioW>100</imageSizeRatioW>", fp);
		textout("			<imageSizeRatioH>100</imageSizeRatioH>", fp);
		textout("			<imageSizeRatioFix>1</imageSizeRatioFix>", fp);
		textout("			<imageSizeIsPixcel>1</imageSizeIsPixcel>", fp);
		textout("		</renderingSettings>", fp);
		textout("		<effectSettings>", fp);
		textout("			<gridSize>50</gridSize>", fp);
		textout("		</effectSettings>", fp);
		textout("		<cellTags />", fp);
		textout("		<useDecimalDigit>2</useDecimalDigit>", fp);
		textout("		<canvasFrameColor>1</canvasFrameColor>", fp);
		textout("		<convertImageToPMA>0</convertImageToPMA>", fp);
		textout("		<blendImageAsPMA>0</blendImageAsPMA>", fp);
		textout("		<unpremultiplyAlpha>0</unpremultiplyAlpha>", fp);
		textout("	</settings>", fp);
		textout("	<animeSettings>", fp);
		textout("		<fps>30</fps>", fp);
		textout("		<frameCount>11</frameCount>", fp);
		textout("		<sortMode>prio</sortMode>", fp);
		textout("		<canvasSize>320 320</canvasSize>", fp);
		textout("		<pivot>0 0</pivot>", fp);
		textout("		<bgColor>FF323232</bgColor>", fp);
		textout("		<gridSize>32</gridSize>", fp);
		textout("		<gridColor>FF808080</gridColor>", fp);
		textout("		<ik_depth>3</ik_depth>", fp);
		textout("		<startFrame>0</startFrame>", fp);
		textout("		<endFrame>10</endFrame>", fp);
		textout("		<outStartNum>0</outStartNum>", fp);
		textout("	</animeSettings>", fp);
		textout("	<texPackSettings>", fp);
		textout("		<maxSize>4096 4096</maxSize>", fp);
		textout("		<forcePo2>1</forcePo2>", fp);
		textout("		<forceSquare>0</forceSquare>", fp);
		textout("		<margin>0</margin>", fp);
		textout("		<padding>1</padding>", fp);
		textout("	</texPackSettings>", fp);
		textout("	<cellmapNames>", fp);
		textout("		<value></value>", fp);
		textout("	</cellmapNames>", fp);
		textout("	<animepackNames>", fp);
		textout("		<value></value>", fp);
		textout("	</animepackNames>", fp);
		textout("	<lastAnimeFile></lastAnimeFile>", fp);
		textout("	<lastAnimeName>anime_1</lastAnimeName>", fp);
		textout("	<lastCellMapFile></lastCellMapFile>", fp);
		textout("</SpriteStudioProject>", fp);

		fclose(fp);
	}
}

	//「新規作成時のプロジェクトのデフォルト設定」からテンプレートを作成
void Sspj_template::make_template_from_ssop(XMLDocument* loadssop_xml)
{
	//テンプレートを新規作成
	FILE* fp = fopen(filename.c_str(), "w");
	if (fp != NULL)
	{
		textout("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>", fp);
		textout("<SpriteStudioProject version=\"2.00.00\">", fp);
		textout("	<name></name>", fp);
		textout("	<exportPath></exportPath>", fp);
		textout("	<settings>", fp);
		textout("	</settings>", fp);
		textout("	<animeSettings>", fp);
		textout("	</animeSettings>", fp);
		textout("	<texPackSettings>", fp);
		textout("	</texPackSettings>", fp);
		textout("	<cellmapNames>", fp);
		textout("		<value></value>", fp);
		textout("	</cellmapNames>", fp);
		textout("	<animepackNames>", fp);
		textout("		<value></value>", fp);
		textout("	</animepackNames>", fp);
		textout("	<lastAnimeFile></lastAnimeFile>", fp);
		textout("	<lastAnimeName>anime_1</lastAnimeName>", fp);
		textout("	<lastCellMapFile></lastCellMapFile>", fp);
		textout("</SpriteStudioProject>", fp);

		fclose(fp);

		XMLDocument xml;
		if (tinyxml2::XML_NO_ERROR != xml.LoadFile(filename.c_str()))
		{
		}
		else
		{
			//「新規作成時のプロジェクトのデフォルト設定」をコピー
			//exportPath
			deep_copy(xml.FirstChildElement("SpriteStudioProject")->FirstChildElement("exportPath"),
				loadssop_xml->FirstChildElement("SpriteStudioOption")->FirstChildElement("exportPath"));
			//settings
			deep_copy(xml.FirstChildElement("SpriteStudioProject")->FirstChildElement("settings"),
				loadssop_xml->FirstChildElement("SpriteStudioOption")->FirstChildElement("projectSettings"));
			//animeSettings
			deep_copy(xml.FirstChildElement("SpriteStudioProject")->FirstChildElement("animeSettings"),
				loadssop_xml->FirstChildElement("SpriteStudioOption")->FirstChildElement("animeSettings"));
			////animeSettings 不要なタグを削除
			//XMLElement* animeSettings = xml.FirstChildElement("SpriteStudioProject")->FirstChildElement("animeSettings");
			//animeSettings->DeleteChild(animeSettings->FirstChildElement("ik_depth"));
			//animeSettings->DeleteChild(animeSettings->FirstChildElement("startFrame"));
			//animeSettings->DeleteChild(animeSettings->FirstChildElement("endFrame"));

			//texPackSettings
			deep_copy(xml.FirstChildElement("SpriteStudioProject")->FirstChildElement("texPackSettings"),
				loadssop_xml->FirstChildElement("SpriteStudioOption")->FirstChildElement("texPackSettings"));

			//ファイルを保存
			xml.SaveFile(filename.c_str());
		}
	}
}

void Ssce_template::make_template_form_default()
{
	//テンプレートを新規作成
	FILE* fp = fopen(filename.c_str(), "w");
	if (fp != NULL)
	{
		textout("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>", fp);
		textout("<SpriteStudioCellMap version=\"2.00.00\">", fp);
		textout("	<name></name>", fp);
		textout("	<exportPath></exportPath>", fp);
		textout("	<imagePath></imagePath>", fp);
		textout("	<pixelSize></pixelSize>", fp);
		textout("	<overrideTexSettings>0</overrideTexSettings>", fp);
		textout("	<wrapMode>clamp</wrapMode>", fp);
		textout("	<filterMode>linear</filterMode>", fp);
		textout("	<imagePathAtImport></imagePathAtImport>", fp);
		textout("	<packInfoFilePath></packInfoFilePath>", fp);
		textout("	<texPackSettings>", fp);
		textout("		<maxSize>4096 4096</maxSize>", fp);
		textout("		<forcePo2>1</forcePo2>", fp);
		textout("		<forceSquare>0</forceSquare>", fp);
		textout("		<margin>0</margin>", fp);
		textout("		<padding>1</padding>", fp);
		textout("	</texPackSettings>", fp);
		textout("	<cells>", fp);
		textout("	</cells>", fp);
		textout("</SpriteStudioCellMap>", fp);

		fclose(fp);
	}
}

//「新規作成時のプロジェクトのデフォルト設定」からテンプレートを作成
void Ssce_template::make_template_from_ssop(XMLDocument* loadssop_xml)
{
	//テンプレートを新規作成
	FILE* fp = fopen(filename.c_str(), "w");
	if (fp != NULL)
	{
		textout("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>", fp);
		textout("<SpriteStudioCellMap version=\"2.00.00\">", fp);
		textout("	<name></name>", fp);
		textout("	<exportPath></exportPath>", fp);
		textout("	<imagePath></imagePath>", fp);
		textout("	<pixelSize></pixelSize>", fp);
		textout("	<overrideTexSettings>0</overrideTexSettings>", fp);
		textout("	<wrapMode>clamp</wrapMode>", fp);
		textout("	<filterMode>linear</filterMode>", fp);
		textout("	<imagePathAtImport></imagePathAtImport>", fp);
		textout("	<packInfoFilePath></packInfoFilePath>", fp);
		textout("	<texPackSettings>", fp);
		textout("	</texPackSettings>", fp);
		textout("	<cells>", fp);
		textout("	</cells>", fp);
		textout("</SpriteStudioCellMap>", fp);

		fclose(fp);

		XMLDocument xml;
		if (tinyxml2::XML_NO_ERROR != xml.LoadFile(filename.c_str()))
		{
		}
		else
		{
			//「新規作成時のプロジェクトのデフォルト設定」をコピー
			//texPackSettings
			deep_copy(xml.FirstChildElement("SpriteStudioCellMap")->FirstChildElement("texPackSettings"),
				loadssop_xml->FirstChildElement("SpriteStudioOption")->FirstChildElement("texPackSettings"));

			//ファイルを保存
			xml.SaveFile(filename.c_str());
		}
	}
}

void Ssae_template::make_template_form_default()
{
	//テンプレートを新規作成
	FILE* fp = fopen(filename.c_str(), "w");
	if (fp != NULL)
	{
		textout("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>", fp);
		textout("<SpriteStudioAnimePack version=\"2.00.01\">", fp);
		textout("	<settings>", fp);
		textout("		<fps>30</fps>", fp);
		textout("		<frameCount>11</frameCount>", fp);
		textout("		<sortMode>prio</sortMode>", fp);
		textout("		<canvasSize>320 320</canvasSize>", fp);
		textout("		<pivot>0 0</pivot>", fp);
		textout("		<bgColor>FF323232</bgColor>", fp);
		textout("		<gridSize>32</gridSize>", fp);
		textout("		<gridColor>FF808080</gridColor>", fp);
		textout("		<ik_depth>3</ik_depth>", fp);
		textout("		<startFrame>0</startFrame>", fp);
		textout("		<endFrame>10</endFrame>", fp);
		textout("		<outStartNum>0</outStartNum>", fp);
		textout("	</settings>", fp);
		textout("	<name></name>", fp);
		textout("	<exportPath></exportPath>", fp);
		textout("	<Model>", fp);
		textout("		<partList>", fp);
		textout("		</partList>", fp);
		textout("	</Model>", fp);
		textout("	<cellmapNames>", fp);
		textout("		<value></value>", fp);
		textout("	</cellmapNames>", fp);
		textout("	<animeList>", fp);
		textout("		<anime>", fp);
		textout("			<name>Setup</name>", fp);
		textout("			<overrideSettings>1</overrideSettings>", fp);
		textout("			<settings>", fp);
		textout("				<fps>30</fps>", fp);
		textout("				<frameCount>1</frameCount>", fp);
		textout("				<sortMode>prio</sortMode>", fp);
		textout("				<canvasSize>320 320</canvasSize>", fp);
		textout("				<pivot>0 0</pivot>", fp);
		textout("				<bgColor>FF323232</bgColor>", fp);
		textout("				<gridSize>32</gridSize>", fp);
		textout("				<gridColor>FF808080</gridColor>", fp);
		textout("				<ik_depth>3</ik_depth>", fp);
		textout("				<startFrame>0</startFrame>", fp);
		textout("				<endFrame>10</endFrame>", fp);
		textout("				<outStartNum>0</outStartNum>", fp);
		textout("			</settings>", fp);
		textout("			<labels/>", fp);
		textout("			<isSetup>1</isSetup>", fp);
		textout("			<partAnimes>", fp);
		textout("			</partAnimes>", fp);
		textout("		</anime>", fp);
		textout("		<anime>", fp);
		textout("			<name>anime_1</name>", fp);
		textout("			<overrideSettings>1</overrideSettings>", fp);
		textout("			<settings>", fp);
		textout("				<fps>30</fps>", fp);
		textout("				<frameCount>11</frameCount>", fp);
		textout("				<sortMode>prio</sortMode>", fp);
		textout("				<canvasSize>320 320</canvasSize>", fp);
		textout("				<pivot>0 0</pivot>", fp);
		textout("				<bgColor>FF323232</bgColor>", fp);
		textout("				<gridSize>32</gridSize>", fp);
		textout("				<gridColor>FF808080</gridColor>", fp);
		textout("				<ik_depth>3</ik_depth>", fp);
		textout("				<startFrame>0</startFrame>", fp);
		textout("				<endFrame>10</endFrame>", fp);
		textout("				<outStartNum>0</outStartNum>", fp);
		textout("			</settings>", fp);
		textout("			<labels/>", fp);
		textout("			<isSetup>0</isSetup>", fp);
		textout("			<partAnimes>", fp);
		textout("			</partAnimes>", fp);
		textout("		</anime>", fp);
		textout("	</animeList>", fp);
		textout("</SpriteStudioAnimePack>", fp);

		fclose(fp);
	}
}

//「新規作成時のプロジェクトのデフォルト設定」からテンプレートを作成
void Ssae_template::make_template_from_ssop(XMLDocument* loadssop_xml)
{
	//テンプレートを新規作成
	FILE* fp = fopen(filename.c_str(), "w");
	if (fp != NULL)
	{
		textout("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>", fp);
		textout("<SpriteStudioAnimePack version=\"2.00.01\">", fp);
		textout("	<settings>", fp);
		textout("	</settings>", fp);
		textout("	<name></name>", fp);
		textout("	<exportPath></exportPath>", fp);
		textout("	<Model>", fp);
		textout("		<partList>", fp);
		textout("		</partList>", fp);
		textout("	</Model>", fp);
		textout("	<cellmapNames>", fp);
		textout("		<value></value>", fp);
		textout("	</cellmapNames>", fp);
		textout("	<animeList>", fp);
		textout("		<anime>", fp);
		textout("			<name>Setup</name>", fp);
		textout("			<overrideSettings>1</overrideSettings>", fp);
		textout("			<settings>", fp);
		textout("			</settings>", fp);
		textout("			<labels/>", fp);
		textout("			<isSetup>1</isSetup>", fp);
		textout("			<partAnimes>", fp);
		textout("			</partAnimes>", fp);
		textout("		</anime>", fp);
		textout("		<anime>", fp);
		textout("			<name>anime_1</name>", fp);
		textout("			<overrideSettings>1</overrideSettings>", fp);
		textout("			<settings>", fp);
		textout("			</settings>", fp);
		textout("			<labels/>", fp);
		textout("			<isSetup>0</isSetup>", fp);
		textout("			<partAnimes>", fp);
		textout("			</partAnimes>", fp);
		textout("		</anime>", fp);
		textout("	</animeList>", fp);
		textout("</SpriteStudioAnimePack>", fp);

		fclose(fp);

		XMLDocument xml;
		if (tinyxml2::XML_NO_ERROR != xml.LoadFile(filename.c_str()))
		{
		}
		else
		{
			//「新規作成時のプロジェクトのデフォルト設定」をコピー
			//SpriteStudioAnimePack>Settings
			deep_copy(xml.FirstChildElement("SpriteStudioAnimePack")->FirstChildElement("settings"),
				loadssop_xml->FirstChildElement("SpriteStudioOption")->FirstChildElement("animeSettings"));

			XMLElement* anime = xml.FirstChildElement("SpriteStudioAnimePack")->FirstChildElement("animeList")->FirstChildElement("anime");
			//anime>Settings
			deep_copy(anime->FirstChildElement("settings"),
				loadssop_xml->FirstChildElement("SpriteStudioOption")->FirstChildElement("animeSettings"));
			anime->FirstChildElement("settings")->FirstChildElement("frameCount")->SetText("1");

			anime = anime->NextSiblingElement("anime");
			//anime>Settings
			deep_copy(anime->FirstChildElement("settings"),
				loadssop_xml->FirstChildElement("SpriteStudioOption")->FirstChildElement("animeSettings"));

			//ファイルを保存
			xml.SaveFile(filename.c_str());
		}
	}
}
