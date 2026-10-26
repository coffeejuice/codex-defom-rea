# Checkpoint 54 candidate-only connector regression

Added `source/apps/moproj2mst/tests/mdt/test_connector_active_project.cpp` as a
single candidate-only integration regression. It resolves
`${M2M_ORACLE_B}/120_B_DOE/COG_P1_B1.moproj`, checks the retained project SHA-256,
copies the full project folder under `M2M_TEST_SCRATCH`, and takes sorted
relative-path/type/size/SHA-256 manifests of both the source and disposable copy.
It calls the real candidate sequence `initProject` -> `makeRegistry` ->
`openProject` -> explicit `activateSubProject(p, 1, ctx)` with recording DOE I/O
and no conversion session. It asserts accepted suffix, successful project walk,
sub-project 1 activation, the XML request at DiagramItem 2 to item index 5,
operation ids 3 and 6, flags `0x1a` / `0x20097`, and reciprocal
Cogging `next` / DoeOutput `prev` links. It compares both complete input manifests
afterward to detect writes or deletions.

The test is deliberately not a Save test. It neither invokes the standard
conversion driver (which activates sub-project 0) nor operation dispatch, Save,
Close, or the original application. The manifest comparison concerns contents,
file sizes, entry types, and relative paths; read-only access-time changes are
not tracked. The test prints Open/activation/edge status and captures thrown
error code/message; a parse or DOE recording failure remains a failed check.

Test integration is automatic: the converter `CMakeLists.txt` globs
`tests/mdt/test_*.cpp` with `CONFIGURE_DEPENDS`, links non-standalone tests to
`m2m_core` and `testkit`, and names this target/test
`m2m_mdt_connector_active_project` / `m2m.mdt.connector_active_project`.
The file includes the standard support path header and `testkit/check.hpp`,
defines the `testkit::run_all()` main, and uses `CHECK`/`CHECK_EQ`, matching the
existing test conventions. No CMake or shared source was edited. The test was
not compiled or run in this worker task.

## Acceptance qualification

A pass would show the candidate parses the retained full project, and that
explicit activation of its second sub-project accepts the actual requested
Cogging (operation 3) -> DoeOutput (operation 6) edge with unchanged copied
inputs. It would improve on the two-item synthetic P03 case by using the actual
MOPROJ and complete dependency folder.

It would not establish that the original GUI activates the second sub-project
or accepts the edge, nor would it establish DoeOutput operation loading, Save
outputs, or Close/reset parity. The original `0x140074f50` probe still uses
controlled callbacks; the captured historical `B:120_B_DOE` comparator result
is not fresh paired acceptance. Fresh original and candidate lifecycle captures
remain separate work.

Only the new P03-independent MDT test and this report were written. No build,
GUI, retained project, or REA database was run or modified.
