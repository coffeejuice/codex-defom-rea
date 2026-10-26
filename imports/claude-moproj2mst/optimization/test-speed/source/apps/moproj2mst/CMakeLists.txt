# moproj2mst: .moproj -> .MST converter DLL (docs/plan-gui-mo-stage1-moproj2mst.md).
#
#   build.cmd [Release|Debug]          configure (MSVC 2022 x64, Ninja), build, run tier 1
#   ctest -L m2m                       tier 1, all packets;  ctest -L m2m-p04  one packet
#   cmake --build build -t m2m_tier2   12 fixed corpus cases (one per operation type)
#   cmake --build build -t m2m_tier3   every oracle-A corpus case (G1)
#
# Products: m2m_core (static), moproj2mst.dll, moproj2mst.exe. Static CRT (/MT) when this is the
# top-level project; when added from another project the parent's runtime setting applies.
# Sources and tests are globbed (CONFIGURE_DEPENDS), so packets add files without editing this file.
cmake_minimum_required(VERSION 3.24)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    project(moproj2mst VERSION 0.1.0 LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    enable_testing()
endif()

set(M2M_REPO "${CMAKE_CURRENT_SOURCE_DIR}/../..")
get_filename_component(M2M_TAKEOVER "${M2M_REPO}/.." ABSOLUTE)
if(DEFINED M2M_EXPERIMENT_TAKEOVER_ROOT)
    # Isolated test-speed source uses the original imported evidence and keeps
    # every scratch path beneath that takeover's build directory.
    get_filename_component(M2M_TAKEOVER "${M2M_EXPERIMENT_TAKEOVER_ROOT}" ABSOLUTE)
endif()
set(M2M_CORPUS "${M2M_TAKEOVER}/evidence/gui_mo/s1/corpus" CACHE PATH
    "Corpus copies (docs/re/moproj2mst/runner/cases.tsv), read-only")
set(M2M_ORACLE_B "${M2M_TAKEOVER}/evidence/gui_mo/oracle_b" CACHE PATH "Imported historical Save oracles, read-only")
set(M2M_DB_FIXTURE "${M2M_TAKEOVER}/evidence/test-rea/COG_P1_B1" CACHE PATH "Imported project DB fixture, read-only")
set(M2M_REFERENCE_EXE "${M2M_TAKEOVER}/evidence/gui_mo/DEF_GUI_MO_64.exe" CACHE FILEPATH "Read-only keyword-table reference, never executed")
set(M2M_TEST_SCRATCH "${CMAKE_BINARY_DIR}/t" CACHE PATH "Disposable test writes, beneath takeover/build")
file(REAL_PATH "${M2M_TAKEOVER}/build" m2m_build_root)
file(REAL_PATH "${M2M_TEST_SCRATCH}" m2m_scratch_root)
cmake_path(IS_PREFIX m2m_build_root "${m2m_scratch_root}" NORMALIZE m2m_local_scratch)
if(NOT m2m_local_scratch OR m2m_scratch_root STREQUAL m2m_build_root)
    message(FATAL_ERROR "M2M_TEST_SCRATCH must be a child of ${m2m_build_root}")
endif()

# Set TEMP/TMP for Win32 GetTempPath and Python tempfile as well as C++ helpers.
# Every executable gets a separate short directory to avoid parallel-test collisions.
function(m2m_test_environment name)
    string(SHA256 key "${name}")
    string(SUBSTRING "${key}" 0 8 key)
    set(scratch "${M2M_TEST_SCRATCH}/${key}")
    file(MAKE_DIRECTORY "${scratch}")
    set_tests_properties("${name}" PROPERTIES WORKING_DIRECTORY "${scratch}"
        ENVIRONMENT "DEF_PORT_REPO=${M2M_REPO};M2M_CORPUS=${M2M_CORPUS};M2M_ORACLE_B=${M2M_ORACLE_B};M2M_DB_FIXTURE=${M2M_DB_FIXTURE};M2M_REFERENCE_EXE=${M2M_REFERENCE_EXE};M2M_CLI=$<TARGET_FILE:moproj2mst_cli>;M2M_TEST_SCRATCH=${scratch};TEMP=${scratch};TMP=${scratch};TMPDIR=${scratch};M2M_S_DUMP=${scratch}/dump-s;M2M_DB_DUMP=${scratch}/dump-db;M2M_MOPROJ_DUMP=${scratch}/dump-moproj;M2M_FOLDER_SWEEP=1")
endfunction()

# Fidelity and warning flags shared with the DEF_PRE port.
if(NOT COMMAND def_port_fidelity)
    include("${M2M_REPO}/port/cmake/DefPort.cmake")
endif()
if(NOT TARGET testkit)
    add_subdirectory("${M2M_REPO}/port/testkit" testkit)
endif()
# defrt: CRT/libm behaviour of the original (def::rt::libm::crt_*). FIDELITY rule: ported code uses
# these, never <cmath>, for CRT math the binary calls (e.g. atan2 0x141470fec).
if(NOT TARGET defrt)
    if(MSVC)
        enable_language(ASM_MASM)   # defrt/src/**.asm
    endif()
    if(NOT DEFINED REPO_ROOT)
        set(REPO_ROOT "${M2M_REPO}")
    endif()
    add_subdirectory("${M2M_REPO}/port/defrt" defrt)
endif()

# The DEF_PRE engine model (port/defmodel, defkey, defdbio, defcmd): the GUI embeds the same engine,
# and the operation loader (src/engine, packet MDT) builds its model with it. Tests of these
# libraries run in the port superbuild, not here.
if(NOT TARGET def_pre_core)
    set(DEF_PORT_SKIP_TESTS ON)
    if(NOT TARGET deformdb_core)
        add_subdirectory("${M2M_REPO}/deformdb" deformdb)
    endif()
    foreach(m defmodel defkey defdbio defcmd def_pre)
        add_subdirectory("${M2M_REPO}/port/${m}" ${m})
    endforeach()
    set(DEF_PORT_SKIP_TESTS OFF)
endif()

function(m2m_flags target)
    def_port_warnings(${target})
    def_port_fidelity(${target})
    target_compile_definitions(${target} PRIVATE UNICODE _UNICODE NOMINMAX _CRT_SECURE_NO_WARNINGS)
endfunction()

# ---- core: everything except the ABI and the CLI --------------------------------------------------
file(GLOB_RECURSE m2m_core_srcs CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER m2m_core_srcs EXCLUDE REGEX "/src/(dll|cli)/")
add_library(m2m_core STATIC ${m2m_core_srcs})
target_include_directories(m2m_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
                                    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_definitions(m2m_core PUBLIC M2M_STATIC)
target_link_libraries(m2m_core PUBLIC defrt defcmd def_pre_core)
m2m_flags(m2m_core)

# ---- the DLL (C ABI) --------------------------------------------------------------------------------
add_library(moproj2mst SHARED "${CMAKE_CURRENT_SOURCE_DIR}/src/dll/abi.cpp")
target_include_directories(moproj2mst PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_link_libraries(moproj2mst PRIVATE m2m_core)
# m2m_core's M2M_STATIC must not leak into the DLL's own export macro.
target_compile_options(moproj2mst PRIVATE /UM2M_STATIC)
target_compile_definitions(moproj2mst PRIVATE M2M_BUILD_DLL)
m2m_flags(moproj2mst)

# ---- the CLI: same ABI code, linked statically ---------------------------------------------------
add_executable(moproj2mst_cli "${CMAKE_CURRENT_SOURCE_DIR}/src/cli/main.cpp"
                              "${CMAKE_CURRENT_SOURCE_DIR}/src/dll/abi.cpp")
set_target_properties(moproj2mst_cli PROPERTIES OUTPUT_NAME moproj2mst)
target_link_libraries(moproj2mst_cli PRIVATE m2m_core)
m2m_flags(moproj2mst_cli)

# ---- tier 1: tests/<packet>/test_*.cpp --------------------------------------------------------------
# Labels: m2m, m2m-<packet>, plus "// ctest-labels: a b" in the first lines. Tests link m2m_core
# and testkit; tests/g0/test_abi_*.cpp link the DLL instead.
file(GLOB m2m_test_dirs LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/tests/*")
foreach(dir IN LISTS m2m_test_dirs)
    if(NOT IS_DIRECTORY "${dir}")
        continue()
    endif()
    get_filename_component(pkt "${dir}" NAME)
    file(GLOB tests CONFIGURE_DEPENDS "${dir}/test_*.cpp")
    foreach(t IN LISTS tests)
        get_filename_component(stem "${t}" NAME_WE)
        string(REGEX REPLACE "^test_" "" topic "${stem}")
        set(exe "m2m_${pkt}_${topic}")
        # "// ctest-standalone: src/ops/a.cpp tests/p12/doubles.cpp" builds the test from the listed
        # sources (relative to this folder) with testkit, without m2m_core (test doubles).
        file(STRINGS "${t}" solo_lines LIMIT_COUNT 20 REGEX "^// ctest-standalone:")
        set(solo_srcs "")
        foreach(l IN LISTS solo_lines)
            string(REGEX REPLACE "^// ctest-standalone:[ \t]*" "" l "${l}")
            string(REGEX REPLACE "[ \t]+" ";" l "${l}")
            foreach(s IN LISTS l)
                if(NOT s STREQUAL "")
                    list(APPEND solo_srcs "${CMAKE_CURRENT_SOURCE_DIR}/${s}")
                endif()
            endforeach()
        endforeach()
        add_executable(${exe} "${t}" ${solo_srcs})
        if(solo_lines)
            target_link_libraries(${exe} PRIVATE testkit)
            target_include_directories(${exe} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
                                                      "${CMAKE_CURRENT_SOURCE_DIR}/src")
            target_compile_definitions(${exe} PRIVATE M2M_STATIC)
        elseif(stem MATCHES "^test_abi")
            target_link_libraries(${exe} PRIVATE moproj2mst testkit)
            target_include_directories(${exe} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
        else()
            target_link_libraries(${exe} PRIVATE m2m_core testkit)
        endif()
        m2m_flags(${exe})
        set(labels "m2m;m2m-${pkt}")
        file(STRINGS "${t}" lab_lines LIMIT_COUNT 20 REGEX "^// ctest-labels:")
        foreach(l IN LISTS lab_lines)
            string(REGEX REPLACE "^// ctest-labels:[ \t]*" "" l "${l}")
            string(REGEX REPLACE "[ \t]+" ";" l "${l}")
            list(APPEND labels ${l})
        endforeach()
        add_test(NAME "m2m.${pkt}.${topic}" COMMAND ${exe})
        set_tests_properties("m2m.${pkt}.${topic}" PROPERTIES LABELS "${labels}")
        m2m_test_environment("m2m.${pkt}.${topic}")
    endforeach()
endforeach()

# Experiment scheduling: focused runs understate the historical folder sweep's
# weighted CTest cost. Start its full-suite work early without changing coverage.
if(TEST m2m.s.save_folder)
    set_tests_properties(m2m.s.save_folder PROPERTIES COST 25.0)
endif()

# ---- corpus runner (tiers 2 and 3) ------------------------------------------------------------------
find_package(Python3 3.10 COMPONENTS Interpreter)
if(Python3_FOUND)
    set(m2m_runner "${M2M_REPO}/docs/re/moproj2mst/runner/run_corpus.py")
    add_test(NAME m2m.g0.runner_self_test
             COMMAND "${Python3_EXECUTABLE}" -B -I "${m2m_runner}" --self-test --corpus "${M2M_CORPUS}")
    set_tests_properties(m2m.g0.runner_self_test PROPERTIES LABELS "m2m;m2m-g0;runner" TIMEOUT 600)
    m2m_test_environment(m2m.g0.runner_self_test)

    # Python checks: tests/<packet>/test_*.py, run with python -I; exit 77 = skipped (e.g. no corpus).
    # Shared corpus line checker: tools/moproj2mst/corpus_lines.py with forms in tools/moproj2mst/forms.
    foreach(dir IN LISTS m2m_test_dirs)
        if(NOT IS_DIRECTORY "${dir}")
            continue()
        endif()
        get_filename_component(pkt "${dir}" NAME)
        file(GLOB pytests CONFIGURE_DEPENDS "${dir}/test_*.py")
        foreach(t IN LISTS pytests)
            get_filename_component(stem "${t}" NAME_WE)
            string(REGEX REPLACE "^test_" "" topic "${stem}")
            add_test(NAME "m2m.${pkt}.${topic}" COMMAND "${Python3_EXECUTABLE}" -B -I "${t}")
            set_tests_properties("m2m.${pkt}.${topic}" PROPERTIES LABELS "m2m;m2m-${pkt};python"
                SKIP_RETURN_CODE 77 TIMEOUT 600)
            m2m_test_environment("m2m.${pkt}.${topic}")
        endforeach()
    endforeach()
    # The CLI reports a missing project with a non-zero exit code (M2M_E_OPEN).
    add_test(NAME m2m.g0.cli_missing_project
             COMMAND "$<TARGET_FILE:moproj2mst_cli>" "${CMAKE_BINARY_DIR}/no_such_dir/x.moproj"
                     "${CMAKE_BINARY_DIR}/g0_cli_out.MST")
    set_tests_properties(m2m.g0.cli_missing_project PROPERTIES LABELS "m2m;m2m-g0" WILL_FAIL TRUE)
    m2m_test_environment(m2m.g0.cli_missing_project)

    file(STRINGS "${M2M_REPO}/docs/re/moproj2mst/runner/tier2.txt" m2m_tier2_lines REGEX "^[0-9]")
    set(m2m_tier2 "")
    foreach(l IN LISTS m2m_tier2_lines)
        string(REGEX REPLACE "[ \t].*$" "" l "${l}")
        list(APPEND m2m_tier2 "${l}")
    endforeach()
    add_custom_target(m2m_tier2
        COMMAND "${CMAKE_COMMAND}" -E env "TEMP=${M2M_TEST_SCRATCH}" "TMP=${M2M_TEST_SCRATCH}"
                "${Python3_EXECUTABLE}" -B -I "${m2m_runner}" --cli "$<TARGET_FILE:moproj2mst_cli>"
                --corpus "${M2M_CORPUS}" --scratch "${M2M_TEST_SCRATCH}/tier2" --keep
                --tsv "${CMAKE_BINARY_DIR}/m2m_tier2.tsv" --cases ${m2m_tier2}
        DEPENDS moproj2mst_cli USES_TERMINAL)
    add_custom_target(m2m_tier3
        COMMAND "${CMAKE_COMMAND}" -E env "TEMP=${M2M_TEST_SCRATCH}" "TMP=${M2M_TEST_SCRATCH}"
                "${Python3_EXECUTABLE}" -B -I "${m2m_runner}" --cli "$<TARGET_FILE:moproj2mst_cli>"
                --corpus "${M2M_CORPUS}" --scratch "${M2M_TEST_SCRATCH}/tier3" --keep
                --oracle-a --tsv "${CMAKE_BINARY_DIR}/m2m_tier3.tsv"
        DEPENDS moproj2mst_cli USES_TERMINAL)
endif()

# Release packaging (owner: P04, claude/m2m-release): version resource, install rules.
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/release.cmake" OPTIONAL)

# The release fragment adds Python/C#/C++ smoke tests after the packet loops.
get_property(m2m_all_tests DIRECTORY PROPERTY TESTS)
foreach(t IN LISTS m2m_all_tests)
    if(t MATCHES "^m2m\\.")
        m2m_test_environment("${t}")
    endif()
endforeach()
