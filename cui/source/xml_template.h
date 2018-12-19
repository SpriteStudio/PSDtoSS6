#pragma once
#include "common.h"

//ssxxファイルのテンプレートを作成する基底クラス
class Ssxx_template
{
protected:
	//新規作成するssxxファイル名
	std::string filename = "";
	//「新規作成時のプロジェクトのデフォルト設定」の値でテンプレートを作成
	virtual void make_template_from_ssop(tinyxml2::XMLDocument* loadssop_xml) = 0;
	// SSのデフォルト値でテンプレートを作成
	virtual void make_template_form_default() = 0;
public:
	//ファイル名(path)を設定
	void set_filename(std::string filename_);
	//テンプレートを作成
	//ssopが存在すれば「新規作成時のプロジェクトのデフォルト設定」を反映する
	void make_template(tinyxml2::XMLDocument* loadssop_xml);
};

//sspjのテンプレートを作成するクラス
class Sspj_template :
	public Ssxx_template
{
public:
	//SSのデフォルト値でテンプレートを作成
	virtual void make_template_form_default();

	//「新規作成時のプロジェクトのデフォルト設定」からテンプレートを作成
	virtual void make_template_from_ssop(tinyxml2::XMLDocument* loadssop_xml);
};

//ssceのテンプレートを作成するクラス
class Ssce_template :
	public Ssxx_template
{
public:
	//SSのデフォルト値でテンプレートを作成
	virtual void make_template_form_default();

	//「新規作成時のプロジェクトのデフォルト設定」からテンプレートを作成
	virtual void make_template_from_ssop(tinyxml2::XMLDocument* loadssop_xml);
};

//ssaeのテンプレートを作成するクラス
class Ssae_template :
	public Ssxx_template
{
public:
	//SSのデフォルト値でテンプレートを作成
	virtual void make_template_form_default();

	//「新規作成時のプロジェクトのデフォルト設定」からテンプレートを作成
	virtual void make_template_from_ssop(tinyxml2::XMLDocument* loadssop_xml);
};
