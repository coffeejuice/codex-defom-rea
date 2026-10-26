#!/bin/bash
W=C:/Users/alext/Projects/deform-rea-work/gui_mo
export JAVA_HOME='C:\tools\jdk-21'
cmd //c "C:\tools\ghidra_12.1.4_PUBLIC\support\analyzeHeadless.bat" "$W/gproj_s1c" guimo -process DEF_GUI_MO_64.exe -noanalysis -scriptPath "$W/s1/s1-03/tools" -postScript DisAt.java "$@" > $W/s1/s1-03/last_dis.log 2>&1
grep -E "ERROR|Exception" $W/s1/s1-03/last_dis.log | grep -v Demangl | head -5
