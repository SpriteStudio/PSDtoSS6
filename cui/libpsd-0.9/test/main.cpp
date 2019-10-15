// main.cpp
//
// $ g++ main.cpp -lpng -lpsd -ljpeg

#include <libpsd.h>

#define USE_PNG 1
#include "rgbaimage.h"

#include <iostream>

int main(int argc, char **argv)
{
  if(argc <= 1){
    std::cout << "usage: " << argv[0] << " {psd file}" << std::endl;
    return -1;
  }

  psd_context *context;
  psd_status status = psd_image_load(&context, argv[1]);
  if(status!=psd_status_done){
    return 1;
  }

  char path[1024];
  int write_count=1;
  psd_layer_record *end=context->layer_records+context->layer_count;
  for(psd_layer_record* layer=context->layer_records;
      layer!=end;
      ++layer){
    // each layer
    if(layer->layer_type!=psd_layer_type_normal){
      // group‚Æ‚©ŽÌ‚Ä‚é
      continue;
    }
	
    RGBAImage image(layer->width, layer->height, 
        (unsigned char*)layer->image_data);
//    sprintf(path, "%03d.png", write_count++);
    sprintf(path, "%s.png" , layer->layer_name );


    image.write_png(path);

	std::cout << "write: " << path << std::endl;
	std::cout << "name(" <<layer->layer_name << std::endl;
	std::cout << "tblr(" <<layer->top << "," << layer->bottom << "," << layer->left << "," << layer->right << ")" << std::endl;
	std::cout << "wh(" <<layer->width << "," << layer->height << ")" <<std::endl;

  }

  psd_image_free(context);

  return 0;
};