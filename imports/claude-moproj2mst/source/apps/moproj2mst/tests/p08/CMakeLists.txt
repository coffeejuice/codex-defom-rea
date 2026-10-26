# P08 tier 1 (ctest -L m2m-p08). Builds emit_a.cpp against test doubles of the seams other packets
# own (doubles.cpp), so it runs before those packets merge. Standalone:
#   cmake -S apps/moproj2mst/tests/p08 -B <fresh dir> -G Ninja && cmake --build <dir> && ctest --test-dir <dir>
cmake_minimum_required(VERSION 3.20)
if(NOT DEFINED M2M_ROOT)
    project(m2m_p08 CXX)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    enable_testing()
    set(M2M_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
endif()

add_executable(m2m_p08_test
    test_emit_a.cpp doubles.cpp
    ${M2M_ROOT}/src/mst/emit_a.cpp ${M2M_ROOT}/src/core/basic.cpp
    ${M2M_ROOT}/src/model/project_owned_storage.cpp
    ${M2M_ROOT}/src/model/subproject_lifetime.cpp
    ${M2M_ROOT}/src/doe/flow_receiver_witness.cpp
    ${M2M_ROOT}/src/model/operation_info_guard.cpp
    ${M2M_ROOT}/src/model/operation_info_model_cache.cpp
    ${M2M_ROOT}/src/model/current_operation_binding.cpp)
target_include_directories(m2m_p08_test PRIVATE ${M2M_ROOT}/include)
if(MSVC)
    target_compile_options(m2m_p08_test PRIVATE /W4 /permissive- /utf-8 /fp:precise)
endif()
add_test(NAME m2m_p08_emit_a COMMAND m2m_p08_test)
set_tests_properties(m2m_p08_emit_a PROPERTIES LABELS "m2m-p08")

find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
    # The corpus line check moved to tools/moproj2mst/corpus_lines.py (tests/p08/test_corpus_lines.py
    # on claude/m2m-tools); the main build runs tests/p08/test_*.py once that branch is merged.
    add_test(NAME m2m_p08_keyword_table
             COMMAND ${Python3_EXECUTABLE} -I ${CMAKE_CURRENT_SOURCE_DIR}/test_keyword_table.py)
    set_tests_properties(m2m_p08_keyword_table PROPERTIES LABELS "m2m-p08" SKIP_RETURN_CODE 77)
endif()
