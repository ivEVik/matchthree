@echo off

if not exist "%~dp0..\build" mkdir "%~dp0..\build"
pushd "%~dp0..\build"
echo %cd%

qmake %~dp0matchthree.pro -o %cd%\Makefile -spec win32-g++ "CONFIG+=qtquickcompiler" "CONFIG+=nowindeploy"
mingw32-make -C %cd% -f Makefile.Release
windeployqt --no-patchqt --no-translations --no-system-d3d-compiler --no-virtualkeyboard --no-webkit2 --no-angle --no-opengl-sw %cd%\release

popd
pause
