CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

project(PSDtoSS)

IF(WIN32)

set(Programs 
./out/LICENSE
./out/PSDtoSS6.exe
./out/PSDtoSS6GUI.exe
./out/Qt5Core.dll
./out/Qt5Gui.dll
./out/Qt5Svg.dll
./out/Qt5Widgets.dll
./out/translate_ja.qm
)

set(imageformats
./out/imageformats/qgif.dll
./out/imageformats/qicns.dll
./out/imageformats/qico.dll
./out/imageformats/qjpeg.dll
./out/imageformats/qsvg.dll
./out/imageformats/qtga.dll
./out/imageformats/qtiff.dll
./out/imageformats/qwbmp.dll
./out/imageformats/qwebp.dll
)

set(iconengines
./out/iconengines/qsvgicon.dll
)

set(platforms
./out/platforms/qwindows.dll
)

set(scripts
	./tools/Ai_PSDtoSS6_Export.jsx
	./tools/Ai_PSDtoSS6_GeneratePivotLayer.jsx
	./tools/Ps_PSDtoSS6_Export.jsx
	./tools/Ps_PSDtoSS6_GeneratePivotLayer.jsx
)



ELSEIF(APPLE)
ENDIF()


#add_subdirectory(cui)

install(FILES ${Programs}
DESTINATION .
COMPONENT 
)

install(FILES ${imageformats}
DESTINATION imageformats
COMPONENT 
)

install(FILES ${iconengines}
DESTINATION iconengines
COMPONENT 
)

install(FILES ${platforms}
DESTINATION platforms
COMPONENT 
)

install(FILES ${scripts}
DESTINATION scripts
COMPONENT 
)

set(CPACK_GENERATOR WIX)
set(CPACK_PACKAGE_NAME "PsdToSS6")
set(CPACK_PACKAGE_VENDOR "Web Technology Corp.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "psd file to SpriteStudio File")
set(CPACK_PACKAGE_VERSION "2.0.4")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "4")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "PsdToSS6")

INCLUDE(CPack)



