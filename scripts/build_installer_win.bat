@echo off
set BASEDIR=%~dp0

pushd %BASEDIR%

cd ..
mkdir installer
cd installer
cmake ..
cmake --build .
cpack -C Release

popd

exit /b
