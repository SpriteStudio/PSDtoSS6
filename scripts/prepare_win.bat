set BASEDIR=%~dp0
set DSTDIR=%BASEDIR%..

if not ""%1""=="""" (
    set DSTDIR=%1
)

pushd %DSTDIR%
rd /s /q vcpkg
git clone https://github.com/microsoft/vcpkg.git
pushd vcpkg
call bootstrap-vcpkg.bat

set VCPKG_DEFAULT_TRIPLET=x64-windows-static
vcpkg install tinyxml2
vcpkg install zlib
vcpkg install libpng
vcpkg install libjpeg-turbo
vcpkg install icu

vcpkg integrate install
popd

popd
