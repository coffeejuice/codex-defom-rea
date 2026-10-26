@echo off
rem Configure, build and run the tier-1 tests with MSVC + Ninja.
rem Usage: build.cmd [Release|Debug] [extra cmake -D options]
set CFG=%1
if "%CFG%"=="" set CFG=Release
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
cmake -S "%~dp0." -B "%~dp0build" -G Ninja -DCMAKE_BUILD_TYPE=%CFG% %2 %3 %4 || exit /b 1
cmake --build "%~dp0build" || exit /b 1
ctest --test-dir "%~dp0build" --output-on-failure -L tier1
