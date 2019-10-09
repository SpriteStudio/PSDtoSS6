CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
if((NOT CMAKE_PROJECT_NAME))
    message(FATAL_ERROR "CMAKE_PROJECT_NAME must be specified!")
endif()
project(${CMAKE_PROJECT_NAME})

IF(WIN32)
ELSEIF(APPLE)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_DEFINITIONS(-std=gnu++14)
ENDIF()

find_package(ZLIB REQUIRED)
find_package(PNG  REQUIRED)
#find_package(tinyxml2  REQUIRED)
#find_package(picojson  REQUIRED)
find_package(JPEG  REQUIRED)

find_package(tinyxml2 CONFIG REQUIRED)
find_path(SYSTEM_INCLUDE_DIR  picojson.h)

find_package(ICU COMPONENTS UC IO REQUIRED)


add_subdirectory(libpsd-0.9)
#add_subdirectory(zlib-1.2.8)
#set(ZLIB_LIBRARY "zlib-1.2.8")
#set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/zlib-1.2.8/;${CMAKE_CURRENT_SOURCE_DIR}/zlib-1.2.8)

set(PNG_TEST 0)
set(PNG_SHARED 0)

#add_subdirectory(lpng1615)
add_library(ItsyPSD STATIC
    ./ItsyPSD/itsypsd.cpp
    ./ItsyPSD/itsypsd.h
)

#add_library(tinyxml2 STATIC
#    ./tinyxml2/tinyxml2.cpp 
#    ./tinyxml2/tinyxml2.h)

#add_library(babel STATIC
#    ./babel/babel.cpp
#    ./babel/babel.h)

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
) 



IF(WIN32)
#	SET(LINK_LIB1 "tinyxml2" "ItsyPSD" "zlibstatic" "png16_static" "babel")
#	SET(ZLIB::ZLIB ${PNG_LIBRARY} ${JPEG_LIBRARY} ${tinyxml2_LIBRARY} )


#	ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
    ADD_DEFINITIONS(/D_WIN64)
#	SET_TARGET_PROPERTIES (${CMAKE_PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Zc:wchar_t")
	set(CMAKE_CXX_FLAGS_RELEASE "/MT ")
	set(CMAKE_CXX_FLAGS_DEBUG "/MTd  /Zi /Od")
	set(CMAKE_EXE_LINKER_FLAGS "/OPT:REF /OPT:ICF ")	

    ADD_DEFINITIONS(-D_WIN32 )
ELSEIF(APPLE)
	SET(SEARCH_DIR "/usr/local/include" )
	SET(SEARCH_LIB "/usr/local/lib" )
#	SET(LINK_LIB1 "tinyxml2" "ItsyPSD" "zlibstatic" "png16_static" "babel")
#	SET(LINK_LIB4 "-framework AppKit -framework IOKit" )
	SET(LINK_LIB4 "" )

    ADD_DEFINITIONS(-APPLE)
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
ENDIF()

include_directories( ${PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${tinyxml2_include_dir} ${jpeg_include_dir} babel libpsd-0.9/include ItsyPSD ${ICU_UC_INCLUDE_DIR})
#include_directories( tinyxml2 ;lpng1615 ; ${ZLIB_INCLUDE_DIR} ; babel ;)

#target_link_libraries(${CMAKE_PROJECT_NAME}  ${PNG_LIBRARY} ${libpsd-0.9} libpsd ItsyPSD  babel tinyxml2::tinyxml2 ZLIB::ZLIB)
target_link_libraries(${CMAKE_PROJECT_NAME}  ${PNG_LIBRARY} ${libpsd-0.9} libpsd ItsyPSD tinyxml2::tinyxml2 ZLIB::ZLIB ${ICU_UC_LIBRARY} ${ICU_IO_LIBRARY} ${ICU_DATA_LIBRARY})




