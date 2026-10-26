# defcmd: command keywords and the Fortran kernels they call
# Layout and rules: docs/port-conventions.md.
def_port_module(defcmd DEPS defkey defdbio)
# Phase 4 gate: the end-to-end GENDB oracle test compares DBs through defcmp.
if(TARGET defcmd_test_gendb_e2e_oracle)
    target_link_libraries(defcmd_test_gendb_e2e_oracle PRIVATE defcmp)
endif()
if(TARGET defcmd_test_dbread_roundtrip)
    target_link_libraries(defcmd_test_dbread_roundtrip PRIVATE defcmp)
endif()
