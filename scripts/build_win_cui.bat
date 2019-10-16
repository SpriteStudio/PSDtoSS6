@echo off
setlocal
set BASEDIR=%~dp0
if not defined SRCDIR set SRCDIR=%BASEDIR%..\cui\
if not defined DSTDIR set DSTDIR=%BASEDIR%..\out\
echo SRCDIR : %SRCDIR%
echo DSTDIR : %DSTDIR%

set CMAKE_TOOLCHAIN = "../../vcpkg/scripts/buildsystems/vcpkg.cmake"
if not %1=="" (
set CMAKE_TOOLCHAIN=%1/vcpkg/scripts/buildsystems/vcpkg.cmake
)


pushd %SRCDIR%

if exist build rd /s /q build
mkdir build
pushd build 

cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_TOOLCHAIN_FILE=%CMAKE_TOOLCHAIN% -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_PROJECT_NAME="PSDtoSS6" .. || exit /b 1
cmake --build . -- /t:clean;rebuild /p:configuration=release /clp:ErrorsOnly /m:4 || exit /b 1

xcopy /y Release\*.exe %DSTDIR%

rem pause
exit /b
