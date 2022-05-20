set BASEDIR=%~dp0
set DSTDIR=%BASEDIR%..

if not ""%1""=="""" (
    set DSTDIR=%1
)

pushd %DSTDIR%

popd
