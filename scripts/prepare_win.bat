set BASEDIR=%~dp0
set DSTDIR=%BASEDIR%..

pushd %BASEDIR%
cd %DSTDIR%
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
call bootstrap-vcpkg.bat

set VCPKG_DEFAULT_TRIPLET=x64-windows-static

vcpkg install tinyxml2
vcpkg install zlib
vcpkg install libpng
vcpkg install libjpeg-turbo

vcpkg integrate install


