@echo off
setlocal
set OWNER="SpriteStuido"
set REPO="PSDtoSS6"
set ARTIFACT="PSDtoSS6_win.zip"

if "%APPVEYOR_REPO_TAG%"=="true" (
    7z a %REPO%\%ARTIFACT% out
    pushd %REPO%
    git config --global user.email "example@example.com"
    git config --global user.name "appveyor"
    git clone git@github.com:%OWNER%/%REPO%.git
    git add %ARTIFACT%
    git commit -m "deploy %APPVEYOR_BUILD_NUMBER%"
    git push
) else (
    echo skip deployment
)