@echo off
set BASEDIR=%~dp0
set DSTDIR=%BASEDIR%..\out\

pushd %BASEDIR%

call build_win_cui %1 || exit /b 1
call build_win_gui %1 || exit /b 1

xcopy /y ..\LICENSE %DSTDIR%

rem pause
exit /b
