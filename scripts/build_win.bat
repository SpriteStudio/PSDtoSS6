@echo off
set BASEDIR=%~dp0
set DSTDIR=%BASEDIR%..\out\

pushd %BASEDIR%

call build_win_cui  || exit /b 1
call build_win_gui  || exit /b 1

xcopy /y ..\LICENSE %DSTDIR%

rem pause
exit /b
