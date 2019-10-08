CMAKE_MINIMUM_REQUIRED(VERSION 3.00)
project(libpsd)

if(APPLE AND (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
	set(BUILD_SHARED_LIBS OFF)
#	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
endif()

find_package(ZLIB REQUIRED)
include_directories( ${ZLIB_INCLUDE_DIR} )

set(libpsd_source
	src/adjustment.c
	src/bevel_emboss.c
	src/bitmap.c
	src/blend.c
	src/boundary.c
	src/brightness_contrast.c
	src/channel_image.c
	src/channel_mixer.c
	src/color.c
	src/color_balance.c
	src/color_mode.c
	src/color_overlay.c
	src/curves.c
	src/descriptor.c
	src/drop_shadow.c
	src/effects.c
	src/file_header.c
	src/fixed.c
	src/gaussian_blur.c
	src/gradient_blend.c
	src/gradient_fill.c
	src/gradient_map.c
	src/gradient_overlay.c
	src/hue_saturation.c
	src/image_data.c
	src/image_resource.c
	src/inner_glow.c
	src/inner_shadow.c
	src/invert.c
	src/layer_mask.c
	src/levels.c
	src/outer_glow.c
	src/path.c
	src/pattern.c
	src/pattern_fill.c
	src/pattern_overlay.c
	src/photo_filter.c
	src/posterize.c
	src/psd.c
	src/psd_system.c
	src/psd_zip.c
	src/rect.c
	src/satin.c
	src/selective_color.c
	src/solid_color.c
	src/stream.c
	src/stroke.c
	src/threshold.c
	src/thumbnail.c
	src/type_tool.c
)


set(libpsd_include
	include/libpsd.h
	include/psd_color.h
	include/psd_config.h
	include/psd_types.h
	src/psd_bitmap.h
	src/psd_blend.h
	src/psd_descriptor.h
	src/psd_fixed.h
	src/psd_gradient.h
	src/psd_math.h
	src/psd_rect.h
	src/psd_stream.h
	src/psd_system.h
	test/rgbaimage.h
)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/)


if(MSVC)
	ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
    ADD_DEFINITIONS(/D_WIN64)
#	SET_TARGET_PROPERTIES (libpsd PROPERTIES COMPILE_FLAGS "/Zc:wchar_t")
	set(CMAKE_CXX_FLAGS_RELEASE "/MT")
	set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Od")
	set(CMAKE_EXE_LINKER_FLAGS "/OPT:REF /OPT:ICF")	

elseif(APPLE AND (${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
    link_directories(/usr/local/lib )
endif()

add_library(libpsd ${libpsd_source} ${libpsd_include} )

