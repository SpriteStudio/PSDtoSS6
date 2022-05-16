CMAKE_MINIMUM_REQUIRED(VERSION 3.0)


set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "" FORCE)

if((NOT CMAKE_PROJECT_NAME))
    set(CMAKE_PROJECT_NAME "PSDtoSS6")
    #message(FATAL_ERROR "CMAKE_PROJECT_NAME must be specified!")
endif()


project(${CMAKE_PROJECT_NAME} CXX)

IF(WIN32)
ELSEIF(APPLE)
	set(CMAKE_CXX_STANDARD 11)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_DEFINITIONS(-std=gnu++14)
ENDIF()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


find_package(ZLIB REQUIRED)
find_package(PNG  REQUIRED)
find_package(JPEG  REQUIRED)
find_package(tinyxml2  )
find_path(SYSTEM_INCLUDE_DIR  picojson.h)

find_package(ICU COMPONENTS UC IO REQUIRED)
#find_package(OpenImageIO REQUIRED)



add_subdirectory(libpsd-0.9)

set(PNG_TEST 0)
set(PNG_SHARED 0)


ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}
    ./source/common.h
    ./source/helper.cpp
    ./source/helper.h
    ./source/main.cpp
    ./source/persePsd.cpp
    ./source/persePsd.h
    ./source/rgbaimage.h
    ./source/system_functions.cpp
    ./source/system_functions.h
    ./source/texturepack.cpp
    ./source/texturepack.h
    ./source/xml_template.cpp
    ./source/xml_template.h
    ./source/parameters.cpp
    ./source/parameters.h
    ./source/convertToSS.cpp
    ./source/convertToSS.h
    ./source/ssOptionReader.h
    ./source/ssOptionReader.cpp
    ./source/psdBitmapLayer.h
    ./source/typedef.h
    ./source/stringconv.h
    ./source/stringconv.cpp
	./source/tbl_outmessage.h
	./source/tbl_outmessage.cpp

) 



IF(WIN32)
    ADD_DEFINITIONS(/D_WIN64)
#	set(CMAKE_CXX_FLAGS_RELEASE "/MT ")
#	set(CMAKE_CXX_FLAGS_DEBUG "/MTd  /Zi /Od")
	set(CMAKE_EXE_LINKER_FLAGS "/OPT:REF /OPT:ICF ")	
	ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

    ADD_DEFINITIONS(-D_WIN32 )
ELSEIF(APPLE)
#	SET(SEARCH_DIR "/usr/local/include" )
#	SET(SEARCH_LIB "/usr/local/lib" )
#	SET(LINK_LIB1 "tinyxml2" "ItsyPSD" "zlibstatic" "png16_static" "babel")
#	SET(LINK_LIB4 "-framework AppKit -framework IOKit" )
	SET(LINK_LIB4 "" )

    ADD_DEFINITIONS(-APPLE)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
ENDIF()

#include_directories( ${openimageio_include} ${PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${tinyxml2_include_dir} ${jpeg_include_dir}  libpsd-0.9/include  ${ICU_UC_INCLUDE_DIR})
include_directories( ${PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${tinyxml2_include_dir} ${jpeg_include_dir}  libpsd-0.9/include  ${ICU_UC_INCLUDE_DIR})


#target_link_libraries( ${CMAKE_PROJECT_NAME}  PRIVATE ${PNG_LIBRARY} ${libpsd-0.9} ${OPENIMAGEIO_LIBRARIES}  libpsd  tinyxml2::tinyxml2 ZLIB::ZLIB ${ICU_LIBRARIES} ${ICU_UC_LIBRARY} ${ICU_IO_LIBRARY} ${ICU_DATA_LIBRARY})
target_link_libraries( ${CMAKE_PROJECT_NAME} libpsd ${CONAN_LIBS} )


#target_link_libraries( ${CMAKE_PROJECT_NAME}  PRIVATE libpng_static ${libpsd-0.9} ${OPENIMAGEIO_LIBRARIES}  libpsd  tinyxml2::tinyxml2 ZLIB::ZLIB ${ICU_UC_LIBRARY} ${ICU_IO_LIBRARY} ${ICU_DATA_LIBRARY})




