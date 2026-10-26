cmake_minimum_required(VERSION 3.24)
project(defcmp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(REPO_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")

# deformdb decodes the .DB files; its own tools and tests are not needed here. A parent project
# (port/CMakeLists.txt) may have added it already, with its tests.
if(NOT TARGET deformdb_core)
    set(DEFORMDB_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
    set(DEFORMDB_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    add_subdirectory("${REPO_ROOT}/deformdb" deformdb)
endif()

# Keyword names of DEF_PRE's registry, used to find keyword lines in KEY files.
set(REGISTRY_TSV "${REPO_ROOT}/docs/re/keyword_registry.tsv")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${REGISTRY_TSV}")
file(STRINGS "${REGISTRY_TSV}" registry_lines)
set(keyword_inc "")
foreach(line IN LISTS registry_lines)
    string(REPLACE "\t" ";" cols "${line}")
    list(GET cols 1 name)
    if(NOT name STREQUAL "name")
        string(APPEND keyword_inc "\"${name}\",\n")
    endif()
endforeach()
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generated/keyword_names.inc" "${keyword_inc}")

add_library(defcmp STATIC src/hash.cpp src/common.cpp src/key.cpp src/transcript.cpp src/db.cpp src/json.cpp)
target_include_directories(defcmp PUBLIC include PRIVATE src "${CMAKE_CURRENT_BINARY_DIR}/generated")
target_link_libraries(defcmp PUBLIC deformdb_core)
if(MSVC)
    target_compile_options(defcmp PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(defcmp PRIVATE -Wall -Wextra)
endif()

add_executable(defcmp_cli tools/defcmp.cpp)
set_target_properties(defcmp_cli PROPERTIES OUTPUT_NAME defcmp)
target_link_libraries(defcmp_cli PRIVATE defcmp)

enable_testing()
add_executable(test_defcmp tests/test_defcmp.cpp)
target_link_libraries(test_defcmp PRIVATE defcmp)
add_test(NAME defcmp_unit COMMAND test_defcmp)
set_tests_properties(defcmp_unit PROPERTIES LABELS "defcmp;tier1")

# Optional checks on copies of real outputs. Configure with
#   -DDEFCMP_TEST_DBS="C:/scratch/COG_P1_B1.DB;C:/scratch/Inc718.DB"
#   -DDEFCMP_TEST_KEYS="C:/scratch/OUT.KEY;..."
# The test copies each file into the build tree, plants differences in the copy
# and never writes to the listed files.
set(DEFCMP_TEST_DBS "" CACHE STRING "Real .DB files (copies) for the corpus test")
set(DEFCMP_TEST_KEYS "" CACHE STRING "Real KEY files (copies) for the corpus test")
if(DEFCMP_TEST_DBS OR DEFCMP_TEST_KEYS)
    add_executable(test_defcmp_corpus tests/test_corpus.cpp)
    target_link_libraries(test_defcmp_corpus PRIVATE defcmp)
    set(corpus_args "")
    foreach(f IN LISTS DEFCMP_TEST_DBS)
        list(APPEND corpus_args "--db" "${f}")
    endforeach()
    foreach(f IN LISTS DEFCMP_TEST_KEYS)
        list(APPEND corpus_args "--key" "${f}")
    endforeach()
    add_test(NAME defcmp_corpus COMMAND test_defcmp_corpus "${CMAKE_CURRENT_BINARY_DIR}/corpus_work" ${corpus_args})
    set_tests_properties(defcmp_corpus PROPERTIES LABELS "defcmp;tier3")
endif()
