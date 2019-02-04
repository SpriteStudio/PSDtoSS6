@echo off
setlocal

set BASEDIR=%~dp0
if not defined SRCDIR set SRCDIR=%BASEDIR%..\gui\
if not defined DSTDIR set DSTDIR=%BASEDIR%..\out\
echo SRCDIR : %SRCDIR%
echo DSTDIR : %DSTDIR%

set CMAKE_PREFIX_PATH=C:\Qt\Qt5.9.7\5.9.7\msvc2017_64\lib
set QT_PATH=C:\Qt\Qt5.9.7\5.9.7\msvc2017_64\bin
echo CMAKE_PREFIX_PATH : %CMAKE_PREFIX_PATH%
echo QT_PATH : %QT_PATH%

set PATH=%QT_PATH%;%PATH%;

pushd %SRCDIR%

if exist build rd /s /q build
mkdir build
pushd build 

cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_PROJECT_NAME="PSDtoSS6GUI" .. || exit /b 1
cmake --build . -- /t:clean;rebuild /p:configuration=release /clp:ErrorsOnly /m:4 || exit /b 1

xcopy /y Release\*.exe %DSTDIR%

call windeployqt ^
--release ^
--force ^
--no-quick-import ^
--no-translations ^
--no-system-d3d-compiler ^
--no-webkit2 ^
--no-angle ^
--no-opengl-sw ^
%DSTDIR%\PSDtoSS6GUI.exe

rem pause
exit /b
