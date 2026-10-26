# cmake -DDLL=<moproj2mst.dll> -DDUMPBIN=<dumpbin.exe> -DVERSION=<a.b.c.d> -P check_exports.cmake
#
# Release checks of the DLL (owner: P18 front half):
#   1. exports are exactly the C ABI of include/moproj2mst.h, undecorated;
#   2. the image is x64;
#   3. no C/C++ runtime DLL is imported (static CRT, /MT), so DMO needs only this one file;
#   4. the version resource carries VERSION.

set(expected m2m_api_version m2m_convert_utf8 m2m_convert_w m2m_set_codepage m2m_set_deform_dir_w m2m_set_gui_unit)
set(ok TRUE)

execute_process(COMMAND "${DUMPBIN}" /nologo /exports "${DLL}" OUTPUT_VARIABLE out RESULT_VARIABLE rc)
if(NOT rc EQUAL 0)
    message(FATAL_ERROR "dumpbin /exports failed: ${rc}")
endif()
# lines: "    ordinal hint RVA      name"
string(REGEX MATCHALL "\n +[0-9]+ +[0-9A-F]+ +[0-9A-F]+ +[^\r\n]+" rows "${out}")
set(names "")
foreach(r IN LISTS rows)
    string(REGEX REPLACE "^\n +[0-9]+ +[0-9A-F]+ +[0-9A-F]+ +([^ \r\n]+).*$" "\\1" n "${r}")
    list(APPEND names "${n}")
endforeach()
list(SORT names)
if(NOT names STREQUAL expected)
    message(SEND_ERROR "exports: got [${names}], expected [${expected}]")
    set(ok FALSE)
else()
    message(STATUS "exports: ${names}")
endif()

execute_process(COMMAND "${DUMPBIN}" /nologo /headers "${DLL}" OUTPUT_VARIABLE out)
if(NOT out MATCHES "8664 machine \\(x64\\)")
    message(SEND_ERROR "not an x64 image")
    set(ok FALSE)
endif()

execute_process(COMMAND "${DUMPBIN}" /nologo /dependents "${DLL}" OUTPUT_VARIABLE out)
string(TOLOWER "${out}" out_l)
string(REGEX MATCHALL "[a-z0-9_.-]+\\.dll" deps "${out_l}")
list(REMOVE_ITEM deps "moproj2mst.dll")
message(STATUS "imports: ${deps}")
foreach(d IN LISTS deps)
    if(d MATCHES "^(msvcp|vcruntime|ucrtbase|msvcr|concrt|api-ms-win-crt|qt)")
        message(SEND_ERROR "runtime dependency ${d}: the DLL must link the CRT statically (/MT)")
        set(ok FALSE)
    endif()
endforeach()

file(TO_NATIVE_PATH "${DLL}" dll_native)
execute_process(COMMAND powershell -NoProfile -NonInteractive -Command
                        "(Get-Item -LiteralPath '${dll_native}').VersionInfo.FileVersion"
                OUTPUT_VARIABLE fv OUTPUT_STRIP_TRAILING_WHITESPACE)
if(NOT fv STREQUAL "${VERSION}")
    message(SEND_ERROR "file version '${fv}', expected '${VERSION}'")
    set(ok FALSE)
else()
    message(STATUS "file version: ${fv}")
endif()

if(NOT ok)
    message(FATAL_ERROR "moproj2mst.dll release checks failed")
endif()
