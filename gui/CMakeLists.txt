cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "" FORCE)


if((NOT CMAKE_PROJECT_NAME))
    set(CMAKE_PROJECT_NAME PSDtoSS6GUI)
endif()

project(${CMAKE_PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(DEFINED ENV{QT_PATH})
    list(APPEND CMAKE_PREFIX_PATH $ENV{QT_PATH})
elseif((APPLE AND (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")))
    if(EXISTS /usr/local/opt/qt)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
    endif()
endif()
find_package(Qt5Widgets CONFIG REQUIRED)

set(src 
    mainwindow.ui
    mainwindow.h
    mainwindow.cpp
    main.cpp
    resource.qrc
    resource.rc
    images/Application-000.icns #入れないとアイコンが消える
    ../cui/source/parameters.h
    ../cui/source/parameters.cpp
    subwindow.ui
    subwindow.h
    subwindow.cpp
    translations/translate_en.qm
    translations/translate_ja.qm
)

IF(WIN32)
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${src})
ELSEIF(APPLE)
	set(MACOSX_BUNDLE_ICON_FILE Application-000.icns)
    set_source_files_properties(images/Application-000.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(translations/translate_en.qm PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_source_files_properties(translations/translate_ja.qm PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE ${src})
ENDIF()

target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Widgets)
