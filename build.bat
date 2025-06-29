@echo off

set BUILD_DIR="build"
set COMPILER="D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.43.34808\bin\Hostx64\x64\cl.exe"

set SDL_HEADERS_INCLUDE_DIR="..\src\external\SDL3-3.2.16\include"
set SDL_LIB_DIR="..\src\external\SDL3-3.2.16\lib\x64"
set SDL_DLL_PATH="..\src\external\SDL3-3.2.16\lib\x64\SDL3.dll"

echo Checking for build directory...

IF NOT EXIST %BUILD_DIR% (
  echo Creating build directory...
  mkdir "%BUILD_DIR%"
)

pushd %BUILD_DIR%

echo Entering build directory..

echo Copying dll file...

xcopy "%SDL_DLL_PATH%" "." /y

echo Compiling...

%COMPILER% /EHsc /Zi^
  /DEBUG:FULL^
  /INCREMENT:NO^
  /std:c++20^
  ../src/main.cpp^
  /I%SDL_HEADERS_INCLUDE_DIR%^
  %SDL_LIB_DIR%\SDL3.lib^
  /link /out:fylgja.exe

popd
