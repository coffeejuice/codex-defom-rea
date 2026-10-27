@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b %errorlevel%
cl /nologo /std:c++20 /EHsc /W4 /O2 /DNDEBUG /D_CRT_SECURE_NO_WARNINGS /fp:strict /I"C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\source\port\testkit\include" /Fo"C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\analysis\cp292-render-core-staging\scratch\\" /Fe"C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\analysis\cp292-render-core-staging\scratch\test_curve_render_transform.exe" "C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\analysis\cp292-render-core-staging\curve_render_transform.cpp" "C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\analysis\cp292-render-core-staging\test_curve_render_transform.cpp"
exit /b %errorlevel%

