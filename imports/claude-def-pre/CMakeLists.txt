cmake_minimum_required(VERSION 3.25)
project(deform_key VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(DEFORM_KEY_BUILD_TESTS "Build unit tests" ON)
option(DEFORM_KEY_BUILD_CLI "Build the dkey command line tool" ON)

if(MSVC)
  add_compile_options(/W4 /permissive- /utf-8 /Zc:__cplusplus /EHsc)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX WIN32_LEAN_AND_MEAN UNICODE _UNICODE)
else()
  add_compile_options(-Wall -Wextra)
endif()

# Keyword registry (docs/re/keyword_registry.tsv) and layout table
# (data/keyword_layouts.txt) are compiled into the library.
set(DK_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
add_custom_command(
  OUTPUT ${DK_GENERATED_DIR}/keyword_table.inc ${DK_GENERATED_DIR}/layout_table.inc
  COMMAND ${CMAKE_COMMAND}
          -DREGISTRY=${CMAKE_CURRENT_SOURCE_DIR}/docs/re/keyword_registry.tsv
          -DLAYOUTS=${CMAKE_CURRENT_SOURCE_DIR}/data/keyword_layouts.txt
          -DOUT_DIR=${DK_GENERATED_DIR}
          -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateTables.cmake
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/docs/re/keyword_registry.tsv
          ${CMAKE_CURRENT_SOURCE_DIR}/data/keyword_layouts.txt
          ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateTables.cmake
  COMMENT "Generating keyword and layout tables")

set(DK_CORE_SOURCES
  src/numbers.cpp
  src/keywords.cpp
  src/key_document.cpp
  src/layout.cpp
  src/step.cpp
  src/def_pre.cpp
  src/database.cpp
  ${DK_GENERATED_DIR}/keyword_table.inc
  ${DK_GENERATED_DIR}/layout_table.inc)

# Static core library: the C++ API.
add_library(deform_key_core STATIC ${DK_CORE_SOURCES})
target_include_directories(deform_key_core
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${DK_GENERATED_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(deform_key_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Shared library with the stable C ABI (include/deform_key.h).
add_library(deform_key SHARED src/c_api.cpp)
target_link_libraries(deform_key PRIVATE deform_key_core)
target_include_directories(deform_key PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(deform_key PRIVATE DK_BUILDING_DLL)

if(DEFORM_KEY_BUILD_CLI)
  add_executable(dkey tools/dkey.cpp)
  target_link_libraries(dkey PRIVATE deform_key_core)
endif()

if(DEFORM_KEY_BUILD_TESTS)
  enable_testing()
  add_executable(dk_tests tests/test_main.cpp tests/test_parser.cpp tests/test_c_api.cpp)
  target_link_libraries(dk_tests PRIVATE deform_key_core deform_key)
  target_include_directories(dk_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
  add_test(NAME dk_tests COMMAND dk_tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  # Integration tests against real databases run only when DK_TEST_DB is set.
  add_test(NAME dk_integration COMMAND dk_tests --integration WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  set_tests_properties(dk_integration PROPERTIES SKIP_RETURN_CODE 77)
  set_tests_properties(dk_tests dk_integration PROPERTIES
    ENVIRONMENT "PATH=$<TARGET_FILE_DIR:deform_key>\;$ENV{PATH}")
endif()
