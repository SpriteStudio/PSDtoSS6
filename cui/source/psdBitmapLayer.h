#pragma

#include "typedef.h"
#include "rgbaimage.h"
#include "texturepack.h"


struct psdBitMapLayer
{
	std::string inputlayerfile[PARTS_MAX];
	std::string inputlayername[PARTS_MAX];
	UserBitMapData*	readbitmap[PARTS_MAX];
};


enum
{
	LAYERTYPE_NORMAL = 0,	//通常のパッキングレイヤー
	LAYERTYPE_DELETE,		//今回消去されたレイヤー
	LAYERTYPE_PIVOT,		//原点指定レイヤー
	LAYERTYPE_ROOT,			//ルートパーツ指定レイヤー
	LAYERTYPE_COPY,			//複製されたセル
};



