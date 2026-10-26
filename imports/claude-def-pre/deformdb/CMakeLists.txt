cmake_minimum_required(VERSION 3.24)
project(deformdb VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)

option(DEFORMDB_BUILD_TOOLS "Build dbinfo" ON)
option(DEFORMDB_BUILD_TESTS "Build tests" ON)

add_library(deformdb_core STATIC
    src/codec.cpp
    src/file.cpp
    src/database.cpp
    src/region_names.cpp
    src/model.cpp
    src/writer.cpp
    src/manager.cpp)
target_include_directories(deformdb_core PUBLIC include)
set_target_properties(deformdb_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
if(MSVC)
    target_compile_options(deformdb_core PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(deformdb_core PRIVATE -Wall -Wextra)
endif()

# C ABI shared library for applications such as DMO.
add_library(deformdb SHARED src/c_api.cpp)
target_link_libraries(deformdb PRIVATE deformdb_core)
target_include_directories(deformdb PUBLIC include)
target_compile_definitions(deformdb PRIVATE DEFORMDB_BUILDING)

if(DEFORMDB_BUILD_TOOLS)
    add_executable(dbinfo tools/dbinfo.cpp)
    target_link_libraries(dbinfo PRIVATE deformdb_core)
    add_executable(dbraw tools/dbraw.cpp)
    target_link_libraries(dbraw PRIVATE deformdb_core)
    add_executable(dbmanager tools/dbmanager.cpp)
    target_link_libraries(dbmanager PRIVATE deformdb_core)
endif()

if(DEFORMDB_BUILD_TESTS)
    enable_testing()
    add_executable(test_codec tests/test_codec.cpp)
    target_link_libraries(test_codec PRIVATE deformdb_core)
    add_test(NAME codec COMMAND test_codec)
    add_executable(test_writer tests/test_writer.cpp)
    target_link_libraries(test_writer PRIVATE deformdb_core)
    add_test(NAME writer COMMAND test_writer)
    set_tests_properties(codec writer PROPERTIES LABELS "deformdb;tier1")
endif()

if(DEFORMDB_BUILD_TESTS)
    # Needs a real database: capi_smoke <file.DB> [step]
    add_executable(capi_smoke tests/capi_smoke.cpp)
    target_link_libraries(capi_smoke PRIVATE deformdb)
endif()
