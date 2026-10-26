# Shared build rules for the DEF_PRE port libraries (docs/port-conventions.md).
#
#   def_port_fidelity(<target>)        floating-point fidelity flags (plan §4)
#   def_port_module(<name> DEPS ...)   one port library: include/<name>, src/**.cpp, tests/test_*.cpp
#
# Sources and tests are globbed so that parallel porting packets add files without editing a
# shared CMakeLists.txt. Re-run cmake after adding a file (CONFIGURE_DEPENDS does it for Ninja).

# ---- floating point ---------------------------------------------------------------------------
# The original was built with MSVC (C/C++) and PathScale -O2 -msse2 -mno-sse3 (Fortran): scalar
# SSE2 doubles, no x87 excess precision, no fused multiply-add. We keep each operation rounded to
# double exactly where the original rounds it:
#   MSVC   /fp:precise      no reassociation; since VS 2022 17.0 it also does not contract a*b+c
#                           into FMA (only /fp:contract would). /arch is left at the x64 default,
#                           which is SSE2 (no /arch:AVX*, so no VEX FMA instructions exist to use).
#   clang  -ffp-contract=off -fno-fast-math, and -msse2 -mno-sse3 -mfpmath=sse on x86.
# tests/fp_fidelity.cpp checks the result at run time.
function(def_port_fidelity target)
    if(MSVC)
        target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/fp:precise>)
        if(CMAKE_CXX_FLAGS MATCHES "/arch:AVX|/fp:fast|/fp:contract")
            message(FATAL_ERROR "def_port: CMAKE_CXX_FLAGS has a flag that breaks FP fidelity: ${CMAKE_CXX_FLAGS}")
        endif()
    else()
        target_compile_options(${target} PRIVATE -ffp-contract=off -fno-fast-math)
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86|AMD64|amd64|i.86")
            target_compile_options(${target} PRIVATE -msse2 -mno-sse3 -mfpmath=sse)
        endif()
    endif()
endfunction()

function(def_port_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:/W4;/permissive-;/utf-8;/Zc:__cplusplus>")
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra)
    endif()
endfunction()

# ---- one port library -------------------------------------------------------------------------
# def_port_module(defmodel DEPS defrt deformdb_core)
#
#   include/<name>/...   public headers (#include <defmodel/...>)
#   src/**.cpp           library sources
#   tests/test_*.cpp     one tier-1 test executable each: ctest name <name>.<topic>, labels
#                        "<name>;tier1" plus any labels listed on a line
#                        "// ctest-labels: p1-03 registry" in the first lines of the file.
#                        Test executables link the module and testkit.
function(def_port_module name)
    cmake_parse_arguments(M "" "" "DEPS" ${ARGN})
    file(GLOB_RECURSE srcs CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
    # src/**.asm (MASM, MSVC only): x87 sequences the original runs that C++ cannot express
    # bit for bit (e.g. the CRT pow 140483f60); see defrt/src/stdlib.
    if(MSVC)
        file(GLOB_RECURSE asm_srcs CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.asm")
        list(APPEND srcs ${asm_srcs})
    endif()
    add_library(${name} STATIC ${srcs})
    target_include_directories(${name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
                                       PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
    if(M_DEPS)
        target_link_libraries(${name} PUBLIC ${M_DEPS})
    endif()
    def_port_warnings(${name})
    def_port_fidelity(${name})

    file(GLOB tests CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp")
    foreach(t IN LISTS tests)
        get_filename_component(stem "${t}" NAME_WE)
        string(REGEX REPLACE "^test_" "" topic "${stem}")
        set(exe "${name}_${stem}")
        add_executable(${exe} "${t}")
        target_link_libraries(${exe} PRIVATE ${name} testkit)
        def_port_warnings(${exe})
        def_port_fidelity(${exe})
        set(labels "${name};tier1")
        file(STRINGS "${t}" lab_lines LIMIT_COUNT 20 REGEX "^// ctest-labels:")
        foreach(l IN LISTS lab_lines)
            string(REGEX REPLACE "^// ctest-labels:[ \t]*" "" l "${l}")
            string(REGEX REPLACE "[ \t]+" ";" l "${l}")
            list(APPEND labels ${l})
        endforeach()
        add_test(NAME "${name}.${topic}" COMMAND ${exe})
        set_tests_properties("${name}.${topic}" PROPERTIES LABELS "${labels}"
            ENVIRONMENT "DEF_PORT_REPO=${REPO_ROOT}")
    endforeach()
endfunction()
