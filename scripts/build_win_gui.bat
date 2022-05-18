@echo off
setlocal
set CURDIR=%~dp0
set BASEDIR=%CURDIR%..
if not defined SRCDIR set SRCDIR=%BASEDIR%\gui\
if not defined DSTDIR set DSTDIR=%BASEDIR%\out\
echo SRCDIR : %SRCDIR%
echo DSTDIR : %DSTDIR%

rem set CMAKE_PREFIX_PATH=C:\Qt\5.15.2\msvc2019_64\lib\
rem set QT_PATH=C:\Qt\5.15.2\msvc2019_64\
if "%VCPKG_PREFIX%" == "" (
    if exist %BASEDIR%\vcpkg (
        set VCPKG_PREFIX=%BASEDIR%\vcpkg
    )
)

rem set DEFAULT_QT_PATH=C:\Qt\Qt5.9.7\5.9.7\msvc2017_64
set DEFAULT_QT_PATH=C:\Qt\5.15.2\msvc2019_64

if "%QT_PATH%" == "" (
    set QT_PATH=%DEFAULT_QT_PATH%
)
if not "%Qt5_Dir%" == "" (
    set QT_PATH=%Qt5_Dir%
)

set CMAKE_PREFIX_PATH=%QT_PATH%\lib
echo CMAKE_PREFIX_PATH : %CMAKE_PREFIX_PATH%
echo QT_PATH : %QT_PATH%

set PATH=%QT_PATH%\bin;%PATH%;

pushd %SRCDIR%

if exist build rd /s /q build
mkdir build
pushd build 

rem cmake -DCMAKE_TOOLCHAIN_FILE="%VCPKG_PREFIX%\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_PROJECT_NAME="PSDtoSS6GUI" .. || exit /b 1
cmake -DCMAKE_PROJECT_NAME="PSDtoSS6GUI" .. || exit /b 1
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
