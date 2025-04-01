CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

message(STATUS "CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "" FORCE)

if((NOT CMAKE_PROJECT_NAME))
    message(FATAL_ERROR "CMAKE_PROJECT_NAME must be specified!")
endif()
project(${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

IF(WIN32)
ELSEIF(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_DEFINITIONS(-std=gnu++17)
ENDIF()

message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")

find_package(ZLIB REQUIRED)
find_package(PNG  REQUIRED)
find_package(JPEG  REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_path(SYSTEM_INCLUDE_DIR  picojson.h)

IF(WIN32)
    find_package(ICU COMPONENTS UC IO DT REQUIRED)
ELSEIF(APPLE)
    find_package(ICU COMPONENTS UC IO DATA REQUIRED)
ENDIF()
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
	set(CMAKE_CXX_FLAGS_RELEASE "/MT ")
	set(CMAKE_CXX_FLAGS_DEBUG "/MTd  /Zi /Od")
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


message(STATUS "PNG_INCLUDE_DIR=${PNG_INCLUDE_DIR}")
message(STATUS "PNG_PNG_INCLUDE_DIR=${PNG_PNG_INCLUDE_DIR}")
message(STATUS "PNG_LIBRARY=${PNG_LIBRARY}")
message("ICU")
message(STATUS "ICU_VERSION=${ICU_VERSION}")
message(STATUS "ICU_INCLUDE_DIRS=${ICU_INCLUDE_DIRS}")
message(STATUS "ICU_UC_INCLUDE_DIR=${ICU_UC_INCLUDE_DIR}")
message(STATUS "ICU_IO_INCLUDE_DIR=${ICU_IO_INCLUDE_DIR}")
IF(WIN32)
message(STATUS "ICU_DT_INCLUDE_DIR=${ICU_DT_INCLUDE_DIR}")
ELSEIF(APPLE)
message(STATUS "ICU_DATA_INCLUDE_DIR=${ICU_DATA_INCLUDE_DIR}")
ENDIF()
message(STATUS "ICU_LIBRARIES=${ICU_LIBRARIES}")

#include_directories( ${openimageio_include} ${PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${tinyxml2_include_dir} ${jpeg_include_dir}  libpsd-0.9/include  ${ICU_UC_INCLUDE_DIR})
include_directories( ${PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} ${tinyxml2_include_dir} ${jpeg_include_dir}  libpsd-0.9/include  ${ICU_UC_INCLUDE_DIR})

IF(WIN32)
    target_link_libraries( ${CMAKE_PROJECT_NAME}  PRIVATE ${PNG_LIBRARY} ${libpsd-0.9} ${OPENIMAGEIO_LIBRARIES}  libpsd  tinyxml2::tinyxml2 ZLIB::ZLIB ${ICU_UC_LIBRARY} ${ICU_IO_LIBRARY} ${ICU_DT_LIBRARY})
ELSEIF(APPLE)
    target_link_libraries( ${CMAKE_PROJECT_NAME}  PRIVATE ${PNG_LIBRARY} ${libpsd-0.9} ${OPENIMAGEIO_LIBRARIES}  libpsd  tinyxml2::tinyxml2 ZLIB::ZLIB ${ICU_UC_LIBRARY} ${ICU_IO_LIBRARY} ${ICU_DATA_LIBRARY})
ENDIF()

#target_link_libraries( ${CMAKE_PROJECT_NAME}  PRIVATE ${ICU_UC_LIBRARY} ${ICU_IO_LIBRARY} ${ICU_DT_LIBRARY} ${PNG_LIBRARY} ${libpsd-0.9} ${OPENIMAGEIO_LIBRARIES}  libpsd  tinyxml2::tinyxml2 ZLIB::ZLIB )


#target_link_libraries( ${CMAKE_PROJECT_NAME}  PRIVATE libpng_static ${libpsd-0.9} ${OPENIMAGEIO_LIBRARIES}  libpsd  tinyxml2::tinyxml2 ZLIB::ZLIB ${ICU_UC_LIBRARY} ${ICU_IO_LIBRARY} ${ICU_DATA_LIBRARY})




