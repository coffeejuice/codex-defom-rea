# P09 tier 1, standalone (ctest -L m2m-p09): builds the emit_b sources against test doubles of
# the seams other packets own (doubles.cpp), so it runs before those packets merge.
#   cmake -S apps/moproj2mst/tests/p09 -B <fresh dir> -G Ninja && cmake --build <dir> && ctest --test-dir <dir>
# The main build globs the same tests and builds them from their "// ctest-standalone:" sources.
cmake_minimum_required(VERSION 3.20)
if(NOT DEFINED M2M_ROOT)
    project(m2m_p09 CXX)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    enable_testing()
    set(M2M_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
endif()

add_executable(m2m_p09_test
    test_emit_b.cpp doubles.cpp
    ${M2M_ROOT}/src/mst/emit_b.cpp ${M2M_ROOT}/src/mst/emit_b_lines.cpp
    ${M2M_ROOT}/src/mst/emit_b_doe.cpp ${M2M_ROOT}/src/doe/item_owner.cpp
    ${M2M_ROOT}/src/core/basic.cpp
    ${M2M_ROOT}/src/model/project_owned_storage.cpp
    ${M2M_ROOT}/src/model/subproject_lifetime.cpp
    ${M2M_ROOT}/src/doe/flow_receiver_witness.cpp
    ${M2M_ROOT}/src/model/operation_info_guard.cpp
    ${M2M_ROOT}/src/model/operation_info_model_cache.cpp
    ${M2M_ROOT}/src/model/current_operation_binding.cpp)
target_include_directories(m2m_p09_test PRIVATE ${M2M_ROOT}/include)
if(MSVC)
    target_compile_options(m2m_p09_test PRIVATE /W4 /permissive- /utf-8 /fp:precise)
endif()
add_test(NAME m2m_p09_emit_b COMMAND m2m_p09_test)
set_tests_properties(m2m_p09_emit_b PROPERTIES LABELS "m2m-p09")

add_executable(m2m_p09_kw
    test_keywords.cpp doubles.cpp kw_doubles.cpp
    ${M2M_ROOT}/src/model/keywords.cpp ${M2M_ROOT}/src/model/objdata.cpp
    ${M2M_ROOT}/src/mst/emit_b.cpp ${M2M_ROOT}/src/mst/emit_b_lines.cpp
    ${M2M_ROOT}/src/mst/emit_b_doe.cpp ${M2M_ROOT}/src/doe/item_owner.cpp
    ${M2M_ROOT}/src/core/basic.cpp
    ${M2M_ROOT}/src/xml/dom.cpp
    ${M2M_ROOT}/src/model/project_owned_storage.cpp
    ${M2M_ROOT}/src/model/subproject_lifetime.cpp
    ${M2M_ROOT}/src/doe/flow_receiver_witness.cpp
    ${M2M_ROOT}/src/model/operation_info_guard.cpp
    ${M2M_ROOT}/src/model/operation_info_model_cache.cpp
    ${M2M_ROOT}/src/model/current_operation_binding.cpp)
target_include_directories(m2m_p09_kw PRIVATE ${M2M_ROOT}/include)
if(MSVC)
    target_compile_options(m2m_p09_kw PRIVATE /W4 /permissive- /utf-8 /fp:precise)
    target_compile_definitions(m2m_p09_kw PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
add_test(NAME m2m_p09_keywords COMMAND m2m_p09_kw)
set_tests_properties(m2m_p09_keywords PROPERTIES LABELS "m2m-p09")

find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
    add_test(NAME m2m_p09_corpus_lines
             COMMAND ${Python3_EXECUTABLE} -I ${CMAKE_CURRENT_SOURCE_DIR}/corpus_lines.py)
    set_tests_properties(m2m_p09_corpus_lines PROPERTIES LABELS "m2m-p09" SKIP_RETURN_CODE 77)
endif()
