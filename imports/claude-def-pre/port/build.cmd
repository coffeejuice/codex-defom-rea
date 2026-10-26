@echo off
rem Configure and build the port with MSVC + Ninja, then run tier 1.
rem Usage: build.cmd [Release|Debug] [extra cmake -D options]
rem Then: cmake --build port\build -t tier2   (smoke)   or  -t tier3 / corpus_table (full corpus)
set CFG=%1
if "%CFG%"=="" set CFG=Release
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
cmake -S "%~dp0." -B "%~dp0build" -G Ninja -DCMAKE_BUILD_TYPE=%CFG% %2 %3 %4 || exit /b 1
cmake --build "%~dp0build" || exit /b 1
ctest --test-dir "%~dp0build" --output-on-failure -L tier1
