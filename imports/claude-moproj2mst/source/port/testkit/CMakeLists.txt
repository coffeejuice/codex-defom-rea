# testkit: header-only tier-1 test support.
add_library(testkit INTERFACE)
target_include_directories(testkit INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
