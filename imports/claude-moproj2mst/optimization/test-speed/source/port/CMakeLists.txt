cmake_minimum_required(VERSION 3.24)
project(def_pre_port VERSION 0.0.1 LANGUAGES CXX)
if(MSVC)
    enable_language(ASM_MASM) # src/**.asm in port modules (cmake/DefPort.cmake)
endif()

# The C++ port of DEF_PRE_64 batch mode and its test tiers (docs/port-plan-def-pre-batch.md §6):
#
#   ctest -L <module>        tier 1: unit tests of one module (deformdb, defcmp, def_pre, defrt,
#                            defmodel, defkey, defdbio, defcmd) or one packet (-L p1-03)
#   ctest -L tier1           all tier-1 tests
#   cmake --build . -t tier2 tier 2: smoke set (harness/corpus/tier2.tsv), before every commit
#   cmake --build . -t tier3 tier 3: every corpus case, one ctest process per case
#   cmake --build . -t corpus_table   same cases as tier 3 in one process, pass/fail table
#
# Tier 2/3 tests run the original through the oracle cache (tools/oracle) and the port in a
# scratch copy of the same inputs, then compare with defcmp. They need DEFORM's DEF_PRE only for
# cases the cache does not hold yet (DEF_PRE_PORT_OFFLINE=ON forbids that).

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")
enable_testing()

add_subdirectory("${REPO_ROOT}/deformdb" deformdb)
add_subdirectory("${REPO_ROOT}/harness/defcmp" defcmp)

# ---- port libraries (docs/port-conventions.md) ------------------------------------------------
# deformdb -> defrt -> defmodel -> defkey / defdbio -> defcmd -> def_pre
include(cmake/DefPort.cmake)
def_port_fidelity(deformdb_core)
add_subdirectory(testkit)
add_subdirectory(defrt)
add_subdirectory(defmodel)
add_subdirectory(defkey)
add_subdirectory(defdbio)
add_subdirectory(defcmd)
add_subdirectory(def_pre)   # def_pre_core: def_pre/src, def_pre/include, def_pre/tests

file(GLOB def_pre_srcs CONFIGURE_DEPENDS def_pre/*.cpp)
add_executable(def_pre ${def_pre_srcs})
target_link_libraries(def_pre PRIVATE def_pre_core defcmd)
def_port_warnings(def_pre)
def_port_fidelity(def_pre)

# Tier 1 for the entry point: it starts and identifies itself.
add_test(NAME def_pre_starts COMMAND def_pre --port-version)
set_tests_properties(def_pre_starts PROPERTIES LABELS "def_pre;tier1"
                     PASS_REGULAR_EXPRESSION "def_pre port")

# ---- tiers 2 and 3: corpus cases --------------------------------------------------------------
find_package(Python3 3.10 REQUIRED COMPONENTS Interpreter)
option(DEF_PRE_PORT_OFFLINE "Never run the original DEF_PRE; use the oracle cache only" OFF)
set(DEF_PRE_TIER3_JOBS 12 CACHE STRING "Parallel ctest processes for the tier3 target")

set(corpus_dir "${REPO_ROOT}/harness/corpus")
set(runner "${corpus_dir}/runner.py")
set(runner_args --port $<TARGET_FILE:def_pre> --defcmp $<TARGET_FILE:defcmp_cli>)
if(DEF_PRE_PORT_OFFLINE)
    list(APPEND runner_args --offline)
endif()
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${corpus_dir}/cases.py" "${corpus_dir}/tier2.tsv"
    "${REPO_ROOT}/docs/corpus/manifest.tsv" "${REPO_ROOT}/docs/corpus/db_coverage.tsv")

foreach(tier 2 3)
    execute_process(COMMAND "${Python3_EXECUTABLE}" -I "${corpus_dir}/cases.py" --tier ${tier} --long
                    OUTPUT_VARIABLE listing RESULT_VARIABLE rc)
    if(NOT rc EQUAL 0)
        message(FATAL_ERROR "cases.py --tier ${tier} failed")
    endif()
    string(REPLACE ";" "\\;" listing "${listing}")
    string(REPLACE "\n" ";" listing "${listing}")
    set(n 0)
    foreach(line IN LISTS listing)
        if(line STREQUAL "")
            continue()
        endif()
        string(REPLACE "\t" ";" cols "${line}")
        list(GET cols 0 id)
        list(GET cols 1 kind)
        list(GET cols 2 set)
        add_test(NAME "t${tier}:${id}"
                 COMMAND "${Python3_EXECUTABLE}" -I "${runner}" ${runner_args} "${id}")
        set(labels "tier${tier};corpus;${kind}")
        if(tier EQUAL 3)
            list(APPEND labels "tier3_${set}")
        endif()
        set_tests_properties("t${tier}:${id}" PROPERTIES LABELS "${labels}" TIMEOUT 1800)
        math(EXPR n "${n} + 1")
    endforeach()
    message(STATUS "tier ${tier}: ${n} corpus cases")
endforeach()

add_custom_target(tier2
    COMMAND ${CMAKE_CTEST_COMMAND} --test-dir "${CMAKE_BINARY_DIR}" -L tier2 -j 8 --output-on-failure
    DEPENDS def_pre defcmp_cli USES_TERMINAL)
add_custom_target(tier3
    COMMAND ${CMAKE_CTEST_COMMAND} --test-dir "${CMAKE_BINARY_DIR}" -L tier3 -j ${DEF_PRE_TIER3_JOBS}
    DEPENDS def_pre defcmp_cli USES_TERMINAL)
add_custom_target(corpus_table
    COMMAND "${Python3_EXECUTABLE}" -I "${runner}" ${runner_args} --tier 3 -q
            -j ${DEF_PRE_TIER3_JOBS} --report "${CMAKE_BINARY_DIR}/corpus_report.tsv"
    DEPENDS def_pre defcmp_cli USES_TERMINAL)
