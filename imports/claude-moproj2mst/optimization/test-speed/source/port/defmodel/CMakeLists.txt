# defmodel: in-memory keyword model (keyword classes, registry, units)
# Layout and rules: docs/port-conventions.md.
def_port_module(defmodel DEPS defrt deformdb_core)

# Layout stubs (src/layout/) abort in DEF_NOT_PORTED, also in destructors: C4722 is expected there.
if(MSVC)
    target_compile_options(defmodel PRIVATE /wd4722)
endif()

# Keyword registry (docs/re/status/keyword-registry.md): the constexpr table is checked at
# compile time, which needs a larger constexpr step budget on MSVC.
if(MSVC)
    target_compile_definitions(defmodel PRIVATE _CRT_SECURE_NO_WARNINGS)
    if(TARGET defmodel_test_keyword_registry)
        target_compile_options(defmodel_test_keyword_registry PRIVATE /constexpr:steps10000000)
        target_compile_definitions(defmodel_test_keyword_registry PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
endif()

# The committed table must be what the generator reads from DEF_PRE_64.exe. Skipped (77) on
# machines without DEFORM installed.
find_package(Python3 3.8 COMPONENTS Interpreter)
if(Python3_FOUND AND NOT DEF_PORT_SKIP_TESTS)
    add_test(NAME defmodel.keyword_registry_matches_binary
             COMMAND "${Python3_EXECUTABLE}" -I "${CMAKE_CURRENT_SOURCE_DIR}/scripts/gen_keyword_registry.py" --check)
    set_tests_properties(defmodel.keyword_registry_matches_binary PROPERTIES
        SKIP_RETURN_CODE 77 LABELS "defmodel;tier1;registry")
endif()
