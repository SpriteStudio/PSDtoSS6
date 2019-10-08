@echo off
setlocal
set BASEDIR=%~dp0
if not defined SRCDIR set SRCDIR=%BASEDIR%..\cui\
if not defined DSTDIR set DSTDIR=%BASEDIR%..\out\
echo SRCDIR : %SRCDIR%
echo DSTDIR : %DSTDIR%

pushd %SRCDIR%

if exist build rd /s /q build
mkdir build
pushd build 

cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_TOOLCHAIN_FILE="../../vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_PROJECT_NAME="PSDtoSS6" .. || exit /b 1
cmake --build . -- /t:clean;rebuild /p:configuration=release /clp:ErrorsOnly /m:4 || exit /b 1

xcopy /y Release\*.exe %DSTDIR%

rem pause
exit /b
