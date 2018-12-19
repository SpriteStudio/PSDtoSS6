
#ifndef __texturepack__
#define __texturepack__


//typedef TBitmap	void* 

#include <string>
#include <vector>

enum
{
	EN_PUSH_SORT_NONE,
	EN_PUSH_SORT_RECTMAX,
	EN_PUSH_SORT_WMAX,
	EN_PUSH_SORT_HMAX,
};

struct TRect
{
	int top;
	int bottom;
	int left;
	int right;

	TRect(){}
	TRect( int l , int t , int r , int b )
	{
		left = l; top = t; right = r ; bottom = b;
	}

	void	Offset( int x , int y )
	{
		top+=y;
		bottom+=y;
		left+=x;
		right+=x;
	}
};


struct UserBitMapData
{
	int*	Data;
	int		Pitch;
	int		Width;
	int		Height;

	int		orgX;
	int		orgY;
	int		orgWidth;
	int		orgHeight;
	int		offset_x;
	int		offset_y;
	int		pri;

};


struct SSSpriteSheetPrim
{
	//配置されたかどうかのフラグ
	bool	located;

	//配置を行うかのフラグ(画像情報のみ取得したい場合に使用する)
	bool	isPacking;
	int		layertype;	//レイヤー種別
	bool	nodata;		//透明レイヤー

	//配置順番号
	int	index;

	//オリジナルの画像サイズ
	int	org_w;
	int	org_h;

	//出力座標オフセット
	int	offset_x;
	int	offset_y;

    //トリミングされたデータ(元)
	int trim_left;
	int	trim_right;
	int trim_top;
	int	trim_bottom;

	int	trim_w;
	int	trim_h;

	int trim_w_org;
	int trim_h_org;

	//まとめられたイメージ上の座標
	int img_left;
	int	img_right;
	int img_top;
	int	img_bottom;

	int	pri;

	//作成された個別のイメージ
	//TBitmap*	bitmap;

	//SsString    name;
//	std::wstring	name;
	std::string	name;
	std::vector<std::string> copy_cell_name;
	//void*		bitmap;
	UserBitMapData*	bitmap;


	int padding;

    TRect   calcrect;	//計算後のレクト

	//SSSpriteSheetPrim(){}

	TRect GetOrgRect()   const
	{
		return TRect( 	0 - padding ,  0 - padding ,
                        org_w + padding , org_h + padding );
	}


	TRect GetTrimRect()   const
	{
		return TRect( 	trim_left - padding ,  trim_top - padding ,
                        trim_right + padding , trim_bottom + padding );
	}

	int	   GetWidth(){
		TRect t = GetTrimRect();
		return t.right - t.left;
	}

	int	   GetHeight(){
		TRect t = GetTrimRect();
		return t.bottom - t.top;
	}

	const int	   GetOrgWidth() const {
		TRect t = GetOrgRect();
		return t.right - t.left;
	}

	const int	   GetOrgHeight() const {
		TRect t = GetOrgRect();
		return t.bottom - t.top;
	}
};


class   SSTexturePack
{
private:
	std::vector<SSSpriteSheetPrim>	srcbitmap;

public:
	int						width;
	int						height;

							//セルマップ縁幅
	int						padding_border;
							//セル間余白
	int						padding_shape;
							//セル内余白
	int						padding_inner;
							//セル配置間隔
	int						pack_padding;

	UserBitMapData*			spriteSheetBitmap;

public:
	SSTexturePack() : spriteSheetBitmap(0) , padding_border(4) , padding_shape(0) , padding_inner(0), pack_padding(0){}
	virtual ~SSTexturePack()
	{


		for ( size_t i = 0 ; i < srcbitmap.size() ; i++ )
		{
//			delete srcbitmap[i].bitmap;
		}
        srcbitmap.clear();

	}

    int	getWidth(){ return width;}
    int	getHeight(){ return height;}


//	TBitmap*	GetBitmap(){ return spriteSheetBitmap; }

    size_t	GetPrimitiveNum()
    {
    	return   srcbitmap.size();
    }

//    const SSSpriteSheetPrim&    GetPrimitiveInfo( size_t index )
    const SSSpriteSheetPrim&    GetPrimitiveInfo( size_t index )
    {
    	return srcbitmap[index];
    }

	SSSpriteSheetPrim*    GetPrimitiveBuff(size_t index)
	{
		return &srcbitmap[index];
	}


	void	rebuild()
	{
		ReleaseBitmap();
		//spriteSheetBitmap = new TBitmap( width , height );

		spriteSheetBitmap = new UserBitMapData();

		spriteSheetBitmap->Width = width;
		spriteSheetBitmap->Height = height;
		spriteSheetBitmap->Pitch = width * 4;

		spriteSheetBitmap->Data = new int[width * height];

		for ( int i = 0 ;i < width * height ; i++ )
		{
			spriteSheetBitmap->Data[i] = 0x00;;
		}

	}

	void    SetSize( int w , int h )
	{
		width = w ;
		height = h;
		rebuild();
    }


	void	ReleaseBitmap()
	{
		if ( spriteSheetBitmap ) delete spriteSheetBitmap;
		spriteSheetBitmap = 0;
	}

//	void	TrimBitmap( TBitmap*   bitmap , SSSpriteSheetPrim& data );
//	void	PushBitmap( int index , TBitmap* src );

	//void	TrimBitmap( void*   bitmap , SSSpriteSheetPrim& data );
	void	TrimBitmap(UserBitMapData*   bitmap, SSSpriteSheetPrim& data, int layertype);


	//void	PushBitmap( int index , UserBitMapData* src );
	void	PushBitmap(int index, UserBitMapData* src, std::string  name, int layertype);


	bool	MakeUp(bool DrawMask, bool isTrim, int algorithm, int sorttype);

};


#endif
