// rgbaimage.h
//
#ifndef RGB_IMAGE_H
#define RGB_IMAGE_H


#ifdef USE_PNG
#include <png.h>
//#include <pngstruct.h>
#include <zlib.h>
#endif

#include <vector>
#include <fstream>
#include <iostream>

class RGBAImage
{
  int width_;
  int height_;
  std::vector<unsigned char> data_;

public:
  RGBAImage()
    : width_(0), height_(0)
    {}

  RGBAImage(int w, int h)
  {
    resize(w, h);
  }

  RGBAImage(int w, int h, unsigned char* data)
  {
    resize(w, h);
    data_.assign(data, data+w*h*4);
  }

  void resize(int w, int h)
  {
    width_=w;
    height_=h;
    data_.resize(w*h*4);
  }

  unsigned char* get()
  {
    return data_.empty() ? NULL : &data_[0];
  }

  int getWidth()
  {
	  return width_;
  }
  int getHeight()
  {
	  return height_;
  }

  bool write_ppm(const char *path)
  {
    std::ofstream io(path, std::ios::binary);
    if(!io){
      return false;
    }

    // header
    io
      << "P6\n"
      << width_ << ' ' << height_ << '\n'
      << 255 << '\n'
      ;

    // raw data
    char *current=(char*)&data_[0];
    for(int y=0; y<height_; ++y){
      for(int x=0; x<width_; ++x, current+=4){
        io.write(current, 3);
      }
    }
    
    return true;
  }

#ifdef USE_PNG
  bool read_png(const char *file_name)
  {
	  // output
	  FILE* fp = fopen(file_name, "rb");
	  if (fp == NULL){
		  std::cerr << "error: " << file_name << " is not a PNG.\n";
		  return false;
	  }

	  png_byte header[8];
	  // read the header
	  fread(header, 1, 8, fp);

	  if (png_sig_cmp(header, 0, 8))
	  {
		  std::cerr << "error: " << file_name << " is not a PNG.\n";
		  fclose(fp);
		  return false;
	  }

	  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	  if (!png_ptr)
	  {
		  fprintf(stderr, "error: png_create_read_struct returned 0.\n");
		  std::cerr << "error: png_create_read_struct returned 0.\n";
		  fclose(fp);
		  return false;
	  }

	  // create png info struct
	  png_infop info_ptr = png_create_info_struct(png_ptr);
      png_set_chunk_malloc_max(png_ptr,0); 

	  if (!info_ptr)
	  {
		  fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		  std::cerr << "error: png_create_info_struct returned 0.\n";
		  png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		  fclose(fp);
		  return false;
	  }

	  // create png info struct
	  png_infop end_info = png_create_info_struct(png_ptr);
      png_set_chunk_malloc_max(png_ptr,0); 

	  if (!end_info)
	  {
		  fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		  std::cerr << "error: png_create_info_struct returned 0.\n";
		  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		  fclose(fp);
		  return false;
	  }

	  // the code in this if statement gets called if libpng encounters an error
	  if (setjmp(png_jmpbuf(png_ptr))) {
		  fprintf(stderr, "error from libpng\n");
		  std::cerr << "error from libpng\n";
		  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		  fclose(fp);
		  return false;
	  }

	  // init png reading
	  png_init_io(png_ptr, fp);

	  // let libpng know you already read the first 8 bytes
	  png_set_sig_bytes(png_ptr, 8);

	  // read all the info up to the image data
	  png_read_info(png_ptr, info_ptr);

	  // variables to pass to get info
	  int bit_depth, color_type;
	  png_uint_32 temp_width, temp_height;

	  // get info about png
	  png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
		  NULL, NULL, NULL);

	  //カラーフォーマット判定
	  //パレット/グレースケール/16bit/tRNS も含め、libpngの変換で 8bit RGBA に正規化する。
	  //（アルファチャンネルを持たない画像は α=0xFF を補完する）
	  if (color_type == PNG_COLOR_TYPE_PALETTE)
	  {
		  //32bitカラーではない
		  std::cerr << "warning:palette image\n";
		  png_set_palette_to_rgb(png_ptr);
	  }
	  if ((color_type == PNG_COLOR_TYPE_GRAY) || (color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
	  {
		  //32bitカラーではない
		  std::cerr << "warning:grayscale image\n";
		  if (bit_depth < 8)
		  {
			  png_set_expand_gray_1_2_4_to_8(png_ptr);
		  }
		  png_set_gray_to_rgb(png_ptr);
	  }
	  if (color_type == PNG_COLOR_TYPE_RGB)
	  {
		  //32bitカラーではない
		  std::cerr << "warning:24bit image\n";
	  }
	  if (bit_depth == 16)
	  {
		  //16bit/channel は 8bit/channel へ落とす
		  std::cerr << "warning:16bit per channel image\n";
		  png_set_strip_16(png_ptr);
	  }
	  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	  {
		  //tRNSによる透過指定はアルファチャンネルへ展開する
		  png_set_tRNS_to_alpha(png_ptr);
	  }
	  //アルファチャンネルを持たない場合は 0xFF で補完して 32bit RGBA にする
	  png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);

	  resize(temp_width, temp_height);	//サイズを設定

	  // Update the png info struct.
	  png_read_update_info(png_ptr, info_ptr);

	  // Row size in bytes. 上記の変換により 32bit RGBA (width * 4) となる。
	  int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	  if (rowbytes != (int)(temp_width * 4))
	  {
		  //32bitカラーへ正規化できなかった
		  std::cerr << "error:Not a 32bit image\n";
		  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		  fclose(fp);
		  return false;
	  }

	  // Allocate the image_data as a big block, to be given to opengl
	  png_byte * image_data;
	  image_data = new png_byte[rowbytes * temp_height * sizeof(png_byte) + 15];
	  if (image_data == NULL)
	  {
		  fprintf(stderr, "error: could not allocate memory for PNG image data\n");
		  std::cerr << "error: could not allocate memory for PNG image data\n";
		  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		  fclose(fp);
		  return false;
	  }

	  // row_pointers is for pointing to image_data for reading the png with libpng
	  png_bytep * row_pointers = new png_bytep[temp_height];
	  if (row_pointers == NULL)
	  {
		  fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
		  std::cerr << "error: could not allocate memory for PNG row pointers\n";
		  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		  delete[] image_data;
		  image_data = NULL;
		  fclose(fp);
		  return false;
	  }

	  // set the individual row_pointers to point at the correct offsets of image_data
	  int i;
	  for (i = 0; i < temp_height; i++)
	  {
		  row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
	  }

	  // read the png into image_data through row_pointers
	  png_read_image(png_ptr, row_pointers);


	  //ここに来る時点で image_data は 32bit RGBA となっている
	  for (int y = 0; y<temp_height; y++) {
		  for (int x = 0; x<temp_width; x++) {
			  int xx = x * 4;
			  int ww = temp_width * 4;

			  unsigned long r = image_data[xx + 0 + (y * rowbytes)];
			  unsigned long g = image_data[xx + 1 + (y * rowbytes)];
			  unsigned long b = image_data[xx + 2 + (y * rowbytes)];
			  unsigned long a = image_data[xx + 3 + (y * rowbytes)];

			  data_[xx + 0 + ((temp_height - y - 1) * ww)] = b;
			  data_[xx + 1 + ((temp_height - y - 1) * ww)] = g;
			  data_[xx + 2 + ((temp_height - y - 1) * ww)] = r;
			  data_[xx + 3 + ((temp_height - y - 1) * ww)] = a;
		  }
	  }


	  if (image_data)
	  {
		  delete[] image_data;
	  }
	  if (row_pointers)
	  {
		  delete[] row_pointers;
	  }
	  fclose(fp);

	  return true;
  }
  bool write_png(const char *path, bool isInterlaced = false)
  {
    // output
    FILE* pFile=fopen(path, "wb");
    if(pFile==NULL){
		std::cerr << "error: write_png " << path << "\n";
		return false;
    }

    // png
    png_struct *pPngStruct=png_create_write_struct(PNG_LIBPNG_VER_STRING,
        NULL, NULL, NULL);
    png_info *pPngInfo=png_create_info_struct(pPngStruct);

    // setjmp
    png_byte** ppRowTable=0;
    if(setjmp(png_jmpbuf(pPngStruct))){
      if(ppRowTable){
        delete[] ppRowTable;
      }
      png_destroy_write_struct(&pPngStruct, &pPngInfo);
      fclose(pFile);
      return false;
    }

    png_init_io(pPngStruct, pFile);
    png_set_compression_level(pPngStruct, Z_BEST_COMPRESSION);
    png_set_IHDR(pPngStruct, pPngInfo,
        width_, height_,
        8, PNG_COLOR_TYPE_RGB_ALPHA,
        isInterlaced ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // gamma
    double CRT_exponent=2.2;
    double LUT_exponent=1.0;
    double default_display_exponent=LUT_exponent*CRT_exponent;
    double default_gamma=1.0/default_display_exponent;
    png_set_gAMA(pPngStruct, pPngInfo, default_gamma);

    png_write_info(pPngStruct, pPngInfo);
    png_set_bgr(pPngStruct);

    if(isInterlaced){
      ppRowTable=new png_byte*[height_];
      for(int y=0; y<height_; ++y){
        ppRowTable[y]=&data_[y*width_*4];
      }
      png_write_image(pPngStruct, ppRowTable);
    }
    else{
      for(int y=0; y<height_; ++y){
        png_write_row(pPngStruct, &data_[y*width_*4]);
      }
    }

    png_write_end(pPngStruct, NULL);

    if(ppRowTable){
      delete[] ppRowTable;
    }
    png_destroy_write_struct(&pPngStruct, &pPngInfo);
    fclose(pFile);

    return true;
  }
#endif
};

#endif