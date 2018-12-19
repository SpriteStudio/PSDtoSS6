#include "texturepack.h"

#include <float.h>
#include <algorithm>
#include <iostream>

//#define NUM_ERROR 0.001
//#define MAX_N 1000  /* 長方形の最大数 */

#define DEBUGLOGOUT (0)

class   FillAlgorithm
{
private:
public:
//    double	border_padding;
//    double	shape_padding;
    int	border_padding;
    int	shape_padding;


	FillAlgorithm() : border_padding(0) ,shape_padding(0) {}
	virtual ~ FillAlgorithm(){}

    void	setBorderPadding( int p )
    {
		border_padding = p;
    }

    void	setShapePadding( int p )
    {
    	shape_padding = p;
    }

 	virtual void	Init( int width , int height , int max ){}

 	virtual void	SetRect( int n , int width , int height ){}
 	virtual void	Calc(){}

    virtual void    GetRectPosisiton( int index , int& _x , int& _y ){}

};



struct rect_array_data
{
    int index;
    int	w;
    int	h;
    int	x;
    int y;
};

class MinMaxFillSort
{
public:
	bool operator()(const rect_array_data* lhs, const rect_array_data* rhs) const
	{
		if (lhs->h == rhs->h)
			return lhs->index > rhs->index;
		return lhs->h > rhs->h;
	}
};
static MinMaxFillSort _MinMaxFillSort;

class MinMaxFill  : public FillAlgorithm
{
private:
	static	const int	 MAX_N =  1000;
    int 	rectangle_max;

    std::vector<rect_array_data*>    rect_array;
	double strip_width, strip_height;  		/* 母材の幅，高さ */
    rect_array_data*                rect_array_etity;

public:

	MinMaxFill(){}
	virtual ~MinMaxFill()
    {
    	delete[]  rect_array_etity;
    }

	virtual void	SetRect( int n , int width , int height )
	{

        rect_array_etity[n].w =  (int)(width + shape_padding);
        rect_array_etity[n].h =  (int)(height + shape_padding);

        rect_array.push_back( &rect_array_etity[n] );

	}

 	virtual void	Calc()
    {
        //std::sort( rect_array.begin() , rect_array.end() , _MinMaxFillSort );

    	int line_y = 0;
        int add_w = 0;
        int line_y_max = 0;


        for ( std::vector<rect_array_data*>::iterator itr = rect_array.begin() ;
              itr != rect_array.end() ; itr++ )
        {

            //母材の大きさを超えた
            if ( add_w+(*itr)->w > strip_width )
            {
           		add_w = 0;
                //line_y+= (*itr)->h;
                line_y+= line_y_max;
                line_y_max = 0;
            }

            if ( line_y_max < (*itr)->h )
            {
            	line_y_max = (*itr)->h;
            }

        	(*itr)->x = add_w;
        	(*itr)->y = line_y;
            add_w+= (*itr)->w;
        }
    }


	virtual void	Init( int width , int height , int max )
	{

        strip_width = width - (border_padding*2);;
		strip_height = height - (border_padding*2);;
        rectangle_max = max;

        rect_array_etity = new rect_array_data[max];
        rect_array.clear();

    }

    virtual void    GetRectPosisiton( int index , int& _x , int& _y )
    {
        _x = rect_array[index]->x + border_padding;
        _y = rect_array[index]->y + border_padding;
    }


};



class BLF : public  FillAlgorithm
{
public:
    int 	rectangle_max;
	static const int	 MAX_N =  1000;

	double strip_width, strip_height;  		/* 母材の幅，高さ */
	int bl_num;  /* BL安定点の候補数 */

	double* w;
	double* h;
	double* x;
	double* y;

	double* blx;
	double* bly;
	double* blw;
	double* blh;


//    double	border_padding;
//    double	shape_padding;


public:
	BLF() : rectangle_max(0) , w(0) , h(0), x(0), y(0),blx(0),bly(0),blw(0),blh(0)
	{
	}

	virtual ~BLF()
	{

		if ( w ) delete[] w;
		if ( h ) delete[] h;
		if ( x ) delete[] x;
		if ( y ) delete[] y;

		if ( blx ) delete[] blx;
		if ( bly ) delete[] bly;
		if ( blw ) delete[] blw;
		if ( blh ) delete[] blh;

	}


    virtual void    GetRectPosisiton( int index , int& _x , int& _y )
    {
        _x = (int)(x[index] + (double)border_padding);
        _y = (int)(y[index] + (double)border_padding);
    }


	// BLF法で詰める
	void bottom_left_fill(int c)
	{
		static const double  NUM_ERROR = 0.001;

		double min_x, min_y;  /* 長方形cの配置可能な座標 */
		bool collision_flag;  /* 長方形cの重なり判定に用いるフラグ */
		int i,j;
#if DEBUGLOGOUT
		std::cerr << "num=" << c << " start\n";
#endif
		/* 長方形cを配置するBL安定点の候補を選ぶ */
		min_x = min_y = FLT_MAX;
		for(i = 0; i < bl_num; i++){

			/* BL安定条件のチェック */
			if(w[c] > blw[i]+NUM_ERROR && h[c] > blh[i]+NUM_ERROR){
#if DEBUGLOGOUT
				std::cerr << "1\n";
#endif
				/* 制約条件のチェック */
				if(blx[i] >= 0 && blx[i]+w[c] <= strip_width && bly[i] >= 0){
#if DEBUGLOGOUT
					std::cerr << "2\n";
#endif
					collision_flag = false;
					for(j = 0; j < c; j++){
						if(blx[i] < x[j]+w[j]-NUM_ERROR && blx[i]+w[c] > x[j]+NUM_ERROR && bly[i]+h[c] > y[j]+NUM_ERROR && bly[i] < y[j]+h[j]-NUM_ERROR){
							collision_flag = true;
#if DEBUGLOGOUT
							std::cerr << "2\n";
#endif
						}
					}
					if(collision_flag == false && (bly[i] < min_y || (bly[i] < min_y + NUM_ERROR && blx[i] < min_x))){
						min_x = blx[i];
						min_y = bly[i];
#if DEBUGLOGOUT
						std::cerr << "2\n";
#endif
					}
				}
			}
		}

		/* 長方形cの配置座標を決定 */
		if(min_x < FLT_MAX && min_y < FLT_MAX){
			x[c] = min_x;
			y[c] = min_y;
		}
		else
		{
			//入りきってない
			x[c] = -1 - (border_padding);
			y[c] = -1 - (border_padding);
			return;
		}
#if DEBUGLOGOUT
		std::cerr << "num=" << c << " min_x=" << min_x << " min_y=" << min_y << "\n";
#endif
		/* 長方形cと母材によって新たに生じるBL安定点の候補を追加 */
		blx[bl_num] = x[c]+w[c];
		bly[bl_num] = 0;
		blw[bl_num] = 0;
		blh[bl_num] = y[c];
		bl_num++;
		blx[bl_num] = 0;
		bly[bl_num] = y[c]+h[c];
		blw[bl_num] = x[c];
		blh[bl_num] = 0;
		bl_num++;

		/* 長方形cと他の長方形jによって生じるBL安定点の候補を追加 */
		for(j = 0; j < c; j++){
			/* 長方形cが長方形jの左側にある場合 */
			if(x[c]+w[c] < x[j]+NUM_ERROR && y[c]+h[c] > y[j]+h[j]+NUM_ERROR){
				blx[bl_num] = x[c]+w[c];
				bly[bl_num] = y[j]+h[j];
				blw[bl_num] = x[j] - (x[c]+w[c]);

				if(y[c] > y[j]+h[j]){
					blh[bl_num] = y[c] - (y[j]+h[j]);
				}else{
					blh[bl_num] = 0;
				}
				bl_num++;
			}

			/* 長方形cが長方形jの右側にある場合 */
			if(x[j]+w[j] < x[c]+NUM_ERROR && y[j]+h[j] > y[c]+h[c]+NUM_ERROR){
				blx[bl_num] = x[j]+w[j];
				bly[bl_num] = y[c]+h[c];
				blw[bl_num] = x[c] - (x[j]+w[j]);

				if(y[j] > y[c]+h[c]){
					blh[bl_num] = y[j] - (y[c]+h[c]);
				}else{
					blh[bl_num] = 0;
				}
				bl_num++;
			}

			/* 長方形cが長方形jの下側にある場合 */
			if(y[c]+h[c] < y[j]+NUM_ERROR && x[c]+w[c] > x[j]+w[j]+NUM_ERROR){
				blx[bl_num] = x[j]+w[j];
				bly[bl_num] = y[c]+h[c];
				if(x[c] > x[j]+w[j]){
					blw[bl_num] = x[c] - (x[j]+w[j]);
				}else{
					blw[bl_num] = 0;
				}

				blh[bl_num] = y[j] - (y[c]+h[c]);
				bl_num++;
			}

			/* 長方形cが長方形jの上側にある場合 */
			if(y[j]+h[j] < y[c]+NUM_ERROR && x[j]+w[j] > x[c]+w[c]+NUM_ERROR){
				blx[bl_num] = x[c]+w[c];
				bly[bl_num] = y[j]+h[j];
				if(x[j] > x[c]+w[c]){
					blw[bl_num] = x[j] - (x[c]+w[c]);
				}else{
					blw[bl_num] = 0;
				}

				blh[bl_num] = y[c] - (y[j]+h[j]);
				bl_num++;
			}
		}
	}

	virtual void	SetRect( int n , int width , int height )
	{


		w[n] = width + shape_padding;
		h[n] = height + shape_padding;
		bottom_left_fill( n );

	}

	virtual void	Init( int width , int height ,int max )
	{
		strip_width = width - (border_padding*2);
		strip_height = height - (border_padding*2);
		rectangle_max = max;

		w = new double[max];
		h = new double[max];
		x = new double[max];
		y = new double[max];
		int i;
		for (i = 0; i < max; i++)
		{
			w[i] = 0;
			h[i] = 0;
			x[i] = 0;
			y[i] = 0;
		}

		int size = max * max / 2 + 1;
		blx = new double[size];
		bly = new double[size];
		blw = new double[size];
		blh = new double[size];

		for (i = 0; i < size; i++)
		{
			blx[i] = 0;
			bly[i] = 0;
			blw[i] = 0;
			blh[i] = 0;
		}

		/* BL安定点の候補を格納する配列を初期化 */
		blx[0] = bly[0] = 0;
        blw[0] = blh[0] = 0;
		//blx[1] = bly[1] = blw[1] = blh[1] = border_padding;
		bl_num = 1;

	}




};

typedef unsigned char	uint8;

#if 1
static int TrimBitmapSub1(UserBitMapData* bitmap )
{
	for (int y = 0; y < bitmap->Height; y++)
	{
		//列スキャン
		for (int x = 0 ; x < bitmap->Width ; x++)
		{
			uint8 *ptr = (uint8 *)bitmap->Data + bitmap->Pitch * y + ( x * 4 );

			uint8 r = *ptr;ptr++;
			uint8 g = *ptr; ptr++;
			uint8 b = *ptr; ptr++;
			uint8 a = *ptr; ptr++;

			if ( a != 0 )
			{
				return y;
			}
		}
	}

	return bitmap->Height;
}
static int TrimBitmapSub2(UserBitMapData* bitmap )
//static int TrimBitmapSub2(TBitmap*   bitmap , TBitmapData& data )
{
	for (int y = bitmap->Height-1 ; y >= 0  ; y--)
	{
		//列スキャン
		for (int x = 0 ; x < bitmap->Width ; x++)
		{
			uint8 *ptr = (uint8 *)bitmap->Data + bitmap->Pitch * y + ( x * 4 );

			uint8 r = *ptr;ptr++;
			uint8 g = *ptr;ptr++;
			uint8 b = *ptr;ptr++;
			uint8 a = *ptr;ptr++;

			if ( a != 0 )
			{
                return y+1;
 			}
		}
	}

    return 0;
}

static int TrimBitmapSub3(UserBitMapData* bitmap )
//static int TrimBitmapSub3(TBitmap*   bitmap , TBitmapData& data )
{
	for (int x = 0 ; x < bitmap->Width ; x++)
	{
		//列スキャン
		for (int y = 0 ; y < bitmap->Height  ; y++)
		{
			uint8 *ptr = (uint8 *)bitmap->Data + bitmap->Pitch * y + ( x * 4 );

			uint8 r = *ptr;ptr++;
			uint8 g = *ptr;ptr++;
			uint8 b = *ptr;ptr++;
			uint8 a = *ptr;ptr++;

			if ( a != 0 )
			{
                return x;
 			}
		}
	}

	return bitmap->Width;
}

static int TrimBitmapSub4(UserBitMapData* bitmap )
//static int TrimBitmapSub4(TBitmap*   bitmap , TBitmapData& data )
{
	for (int x = bitmap->Width-1 ; x >= 0  ; x--)
	{
		//列スキャン
		for (int y = 0 ; y < bitmap->Height  ; y++)
		{
			uint8 *ptr = (uint8 *)bitmap->Data + bitmap->Pitch * y + ( x * 4 );

			uint8 r = *ptr;ptr++;
			uint8 g = *ptr;ptr++;
			uint8 b = *ptr;ptr++;
			uint8 a = *ptr;ptr++;

			if ( a != 0 )
			{
                return x+1;
 			}
		}
	}

    return 0;
}



void	SSTexturePack::PushBitmap(int index, UserBitMapData* src, std::string  name, int layertype)
{
	//TBitmap* temp = new TBitmap (src->Width , src->Height);
	//temp->CopyFromBitmap( src , TRect(0 , 0 ,src->Width,src->Height), 0, 0 );

	SSSpriteSheetPrim	prim;
	prim.name = name;
	TrimBitmap(src, prim, layertype);
	srcbitmap.push_back( prim );
}



static int arign2( int a )
{
    return (a/2)*2;
    //return a;

}

//void	SSTexturePack::TrimBitmap( TBitmap*   bitmap , SSSpriteSheetPrim& data )
void	SSTexturePack::TrimBitmap(UserBitMapData*   bitmap, SSSpriteSheetPrim& data, int layertype)
{
	data.bitmap = bitmap;

	data.org_w = bitmap->Width;
	data.org_h = bitmap->Height;

    //配置位置
	data.img_left = 0 ;
	data.img_right = 0;
	data.img_top = 0;
	data.img_bottom = 0;

	data.trim_left = 0 ;
	data.trim_right = data.org_w ;
	data.trim_top = 0;
	data.trim_bottom = data.org_h;

	data.offset_x = bitmap->offset_x;
	data.offset_y = bitmap->offset_y;

	//TBitmapData src_data;
	//bitmap->Map(TMapAccess::ReadWrite, src_data);
/*
	int trim_top = arign2( TrimBitmapSub1( bitmap  ) );
	int trim_bottom = arign2(TrimBitmapSub2(bitmap));
	int trim_left = arign2(TrimBitmapSub3(bitmap));
	int trim_right = arign2(TrimBitmapSub4(bitmap));
*/
	int trim_top = TrimBitmapSub1(bitmap);
	int trim_bottom = TrimBitmapSub2(bitmap);
	int trim_left = TrimBitmapSub3(bitmap);
	int trim_right = TrimBitmapSub4(bitmap);

	data.nodata = false;
	if (trim_left == data.org_w)
	{
		//全部透明の画像
		//2*2の矩形として登録する
		trim_top = 0;
		trim_bottom = 16;
		trim_left = 0;
		trim_right = 16;
		data.nodata = true;
	}

	data.trim_left = trim_left;
	data.trim_right = trim_right;
	data.trim_top = trim_top;
	data.trim_bottom = trim_bottom;

	data.trim_w = trim_right - trim_left;
	data.trim_h = trim_bottom - trim_top;

	data.trim_w_org = data.trim_w;//元のサイズを保存しておく
	data.trim_h_org = data.trim_h;//元のサイズを保存しておく

	//幅、高さが奇数の場合は偶数に補正する ssaeに配置される際の座標を整数化するため
	if (((data.trim_w + padding_shape) % 2) == 1)
	{
		data.trim_w += 1;
		data.trim_right += 1;
	}
	if (((data.trim_h + padding_shape) % 2) == 1)
	{
		data.trim_h += 1;
		data.trim_bottom += 1;
	}

	if (pack_padding > 1)	//範囲を最小単位に調整
	{
		int amari = ( data.trim_w + padding_shape + padding_inner * 2 ) % pack_padding;
		int add = 0;
		if (amari > 0)
		{
			add = pack_padding - amari;
			data.trim_w += add;
			data.trim_right += add;
		}

		amari = ( data.trim_h + padding_shape + padding_inner * 2 ) % pack_padding;
		if (amari > 0)
		{
			add = pack_padding - amari;
			data.trim_h += add;
			data.trim_bottom += add;
		}
	}
#if DEBUGLOGOUT
	//	std::cout << "TrimBitmapSub4\n";
	std::cerr << data.name << " top=" << data.trim_top << " bottom=" << data.trim_bottom << " left=" << data.trim_left << " right=" << data.trim_right << "\n";
#endif
	data.pri = bitmap->pri;

	data.layertype = layertype;
	if ( (layertype == 0) || (layertype == 1) )
	{
		data.isPacking = true;
	}
	else
	{
		//データレイヤーはパッキングしない
		data.isPacking = false;
	}
	//bitmap->Unmap(src_data);

}


static void    Bitblt( UserBitMapData* dst , UserBitMapData* src , TRect& rect , int _x , int _y , bool useMarker = false)
{

	unsigned long dammy = 0;

	for (int y = rect.top ; y <= rect.bottom ; y++)
	{
		if ( (_y+y-rect.top) >= dst->Height)
			continue;

		uint8 *ptr_dst = (uint8 *)dst->Data + dst->Pitch * (_y+y-rect.top) +(_x*4);
#ifndef _WIN32
		unsigned int *ptr_dst2 = ( unsigned int* )ptr_dst;
#else
		unsigned long *ptr_dst2 = ( unsigned long* )ptr_dst;
#endif

		for (int x = rect.left ; x <= rect.right ; ++x)
		{
			uint8 *ptr_src = (uint8 *)src->Data + src->Pitch * y + (x*4);
			unsigned long *ptr_src2 = ( unsigned long* )ptr_src;

			if ( ( (x-rect.left) + _x) >= dst->Width)
			{
				continue;
			}
			if ( y < 0 || y >= src->Height ||
				 x < 0 || x >= src->Width )
			{
				if ( useMarker )
				{
					*ptr_dst2 = 0xFFFF0000;//デバッグ用パディング
				}else{
					*ptr_dst2 = 0;
				}

			}else{
				*ptr_dst2 = *ptr_src2;
				//TEST
				if ( *ptr_src2 == 0 || (*ptr_src2 >> 24 == 0 ))
				{
					if ( useMarker )
					{
						*ptr_dst2 = 0x80FF0000;//デバッグ用パディング
					}else{
						*ptr_dst2 = 0;
					}
				}
			}

			if ( useMarker )
			{
				if ( x == rect.left || x == rect.right )
				{
					*ptr_dst2 = 0x80FF00FF;//デバッグ用パディング
				}
				if ( y == rect.top || y == rect.bottom )
				{
					*ptr_dst2 = 0x80FF00FF;//デバッグ用パディング
				}
			}
		

//			*ptr_dst2 = 0xFFFF00FF;//デバッグ用パディング
//*ptr_dst2 = 0xffff0000;
			//ptr_src2++;
			ptr_dst2++;

			}
		}


//	src->Unmap(src_data);
//	dst->Unmap(dst_data);

}


//ソートの評価関数
bool compare_name(const SSSpriteSheetPrim& left, const SSSpriteSheetPrim& right)
{
	bool rc = false;
	if (left.name < right.name)
	{
		rc = true;
	}
	return rc;
}
bool compare_rect(const SSSpriteSheetPrim& left, const SSSpriteSheetPrim& right)
{
	bool rc = false;
	if ((left.trim_w * left.trim_h) > (right.trim_w * right.trim_h))
	{
		rc = true;
	}
	return rc;
}
bool compare_w(const SSSpriteSheetPrim& left, const SSSpriteSheetPrim& right)
{
	bool rc = false;
	if ((left.trim_w) > (right.trim_w))
	{
		rc = true;
	}
	return rc;
}
bool compare_h(const SSSpriteSheetPrim& left, const SSSpriteSheetPrim& right)
{
	bool rc = false;
	if ((left.trim_h) > (right.trim_h))
	{
		rc = true;
	}
	return rc;
}

bool	SSTexturePack::MakeUp(bool DrawMask , bool isTrim ,int algorithm, int sorttype )
{
	bool rc = true;

	//int border_pading = 2 ; //テクスチャ境界のパディング
//	FillAlgorithm*  fill_arg = new BLF();
	FillAlgorithm*  fill_arg = 0;

    if ( algorithm == 0 )
    {
    	fill_arg = new MinMaxFill();
    }else{
    	fill_arg = new BLF();
    }

	if (fill_arg == 0)
	{
		return (false);  //本当はアサート
	}

	//画像のソート
	switch (sorttype)
	{
	case EN_PUSH_SORT_NONE:
		std::sort(srcbitmap.begin(), srcbitmap.end(), compare_name);//比較関数を指定してソート
		break;
	case EN_PUSH_SORT_RECTMAX:
		std::sort(srcbitmap.begin(), srcbitmap.end(), compare_rect);//比較関数を指定してソート
		break;
	case EN_PUSH_SORT_WMAX:
		std::sort(srcbitmap.begin(), srcbitmap.end(), compare_w);//比較関数を指定してソート
		break;
	case EN_PUSH_SORT_HMAX:
		std::sort(srcbitmap.begin(), srcbitmap.end(), compare_h);//比較関数を指定してソート
		break;
	}

	for ( size_t i = 0 ; i < srcbitmap.size() ; i ++)
	{
		srcbitmap[i].located = false;
	}

	//パッキングテクスチャのサイズ

    fill_arg->setBorderPadding( padding_border );
    fill_arg->setShapePadding( padding_shape );

//	blf.border_padding = padding_border;
//	blf.shape_padding = padding_shape;
    int	inner_padding = padding_inner;

	fill_arg->Init( width , height , srcbitmap.size()*2 );

	for ( size_t i = 0 ; i < srcbitmap.size() ; i++ )
	{
		if (srcbitmap[i].isPacking == false)
		{
			//パッキングを行わないデータ取得用の画像
			continue;
		}
		srcbitmap[i].padding = inner_padding; //パディングを入れる

		if (isTrim)
		{
			fill_arg->SetRect(i, srcbitmap[i].GetWidth(), srcbitmap[i].GetHeight());
		}
		else{
			fill_arg->SetRect(i, srcbitmap[i].GetOrgWidth(), srcbitmap[i].GetOrgHeight());
		}
	}

    fill_arg->Calc();

	//計算結果をコピー
	for ( size_t i = 0 ; i < srcbitmap.size() ; i++ )
	{
		if (srcbitmap[i].isPacking == false)
		{
			//パッキングを行わないデータ取得用の画像
			continue;
		}

		int x;// = blf.x[i];
		int y;// = blf.y[i];
        fill_arg->GetRectPosisiton( i , x , y );

		if (x < 0)
		{
			//パック画像より、詰める画像がでかい場合はエラー
			return false;	//テクスチャが入りきらない
		}
		if (x < 0)
		{
			//パック画像より、詰める画像がでかい場合はエラー
			return false;	//テクスチャが入りきらない
		}

		srcbitmap[i].img_left = x;
		srcbitmap[i].img_top = y;

#if DEBUGLOGOUT
		//for debug code start
		std::cerr << srcbitmap[i].name << " x=" << x << " y=" << y << "\n";
		//for debug code end
#endif

		if (srcbitmap[i].trim_w > width)
		{
			//パック画像より、詰める画像がでかい場合はエラー
			return false;	//テクスチャが入りきらない
		}
		if (srcbitmap[i].trim_h > height)
		{
			//パック画像より、詰める画像がでかい場合はエラー
			return false;	//テクスチャが入りきらない
		}

		if (isTrim)
		{
			srcbitmap[i].img_right = x + ( srcbitmap[i].GetWidth() );
			srcbitmap[i].img_bottom = y + ( srcbitmap[i].GetHeight() );
			TRect	rect = srcbitmap[i].GetTrimRect();

            srcbitmap[i].calcrect = rect;
            srcbitmap[i].calcrect.Offset( x, y );

			Bitblt( spriteSheetBitmap , srcbitmap[i].bitmap , rect , x  , y , DrawMask );
		}else{
			srcbitmap[i].img_right = x + ( srcbitmap[i].GetOrgWidth() );
			srcbitmap[i].img_bottom = y + ( srcbitmap[i].GetOrgHeight() );
			TRect	rect = srcbitmap[i].GetOrgRect();
            srcbitmap[i].trim_left = 0;
            srcbitmap[i].trim_top = 0;

            srcbitmap[i].calcrect = rect;
            srcbitmap[i].calcrect.Offset( x, y );

			Bitblt( spriteSheetBitmap , srcbitmap[i].bitmap , rect , x  , y , DrawMask );
		}

		//範囲外チェック
		if ((srcbitmap[i].img_right > width) || (srcbitmap[i].img_bottom > height))
		{
			rc = false;	//テクスチャが入りきらない
		}

	}

    delete fill_arg;

	return(rc);
}


#endif