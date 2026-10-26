# CP64: DOE Type2 public acceptance boundary

## Disposition

No new public C ABI success test is justified by the current path. The public
converter unconditionally activates subproject index 0 in
`src/core/convert.cpp` after `openProject`. The retained DoeOutput fixture
`evidence/gui_mo/oracle_b/120_B_DOE/COG_P1_B1.moproj` has a first subproject
with `Type="0"` and a second subproject with `Type="2"` (`Id="1"`). Its
Type2 active diagram item is index 5 and points to `Operation_Id="6"`; the
retained direct-dispatch regression explicitly selects index 1 to reach it.
`m2m_convert_w` has no public subproject-index argument or flag. Altering
subproject order in a copy would invent an unqualified project structure, so
it is not a valid public acceptance fixture.

Even after a future, source-qualified public selection route reaches this
Type2 operation, current source does not support successful dispatch. The
`DoeOutputClass::loadOperation` path calls `loadClassData` and then explicitly
refuses at `SfOprDoeOutput selected session/global task application remains
unsupported`. With this retained fixture, `loadClassData` reaches the existing
`.DS` reader first; that reader currently refuses the `DBLIST` or `ReportList`
payload with `M2M_E_UNSUPPORTED`. An unsupported result is not a conversion
success and cannot satisfy a success-path acceptance gate.

## Reusable disposable fixture

Use a full copy of `evidence/gui_mo/oracle_b/120_B_DOE` under the test's
configured `M2M_TEST_SCRATCH`, never the oracle directory itself. Pin these
inputs before copying:

- `COG_P1_B1.moproj`, 34,774 bytes, SHA-256
  `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e`.
- `DoeStudies/DoeStudy1/Control/COG_P1_B1.DS`, 2,013 bytes, SHA-256
  `97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d`.

The existing `tests/mdt/test_doeoutput_active_guard.cpp` already qualifies the
copied XML/registry route: subproject index 1 is Type2, its active item is 5,
and the operation-3 to operation-6 connector remains linked. It also verifies
the exact `.DS` read and first unsupported DBLIST/ReportList guard. That test
uses `RecordingStudyIo` and proves only simulated sample writes; it is not a
public C ABI test and must not be mistaken for native filesystem effects.

A future public test may reuse the copied fixture only after the public
converter has a qualified way to select subproject index 1 and the Type2
DoeOutput load/session path has been implemented. Do not edit the copied XML
to move/reindex subprojects as a substitute for that evidence. For an interim
negative integration probe, a new test may be considered only once that
selection route is public: call `m2m_convert_w` with `M2M_FLAG_WRITE_SAVE` on
the disposable copy and require the exact supported boundary/status rather
than treating it as success.

## Staged current-path effects if the Type2 operation is reached

This is the expected failure progression for a future selected-subproject
probe against the current sources, not a test result:

1. `M2M_FLAG_WRITE_SAVE` configures `NativeStudyIo` in `core/convert.cpp`.
   Type2 `makeStudy` attempts the `DoeStudies/DoeStudy1` root plus `Control`,
   `Solutions`, and `Output` directories. The retained copy already contains
   these directories; absent directories are created in scratch, with no
   rollback promise.
2. `readLazyStudy` calls `loadSampleTable` for modes 0, 1, and 2. Native
   `StudyIo::writeTextFile` truncates/writes
   `Samples_LHS.par` as `0\r\n`, `Samples_FF.par` as `1\r\n`, and
   `Samples_USR.par` as `2\r\n` under the copied `Control` directory. The
   existing LHS file is overwritten; FF and USR files are created. These are
   real scratch side effects, unlike the recorded writes in the isolated
   regression.
3. DoeOutput active dispatch reads the retained 2,013-byte `.DS` and reaches
   the current `DBLIST/ReportList import remains unsupported` guard before
   public Save or MST writer dispatch. Expected adapter status is
   `M2M_E_UNSUPPORTED` (7), because `src/dll/abi.cpp` returns `Error::code`.
   No successful output MST or project Save should be claimed. The three
   sample-file effects and any earlier directory creations remain in scratch.
4. If that `.DS` guard is later resolved, the next current source guard remains
   `SfOprDoeOutput selected session/global task application remains unsupported`.
   Passing the document reader alone still does not yield public success.

These effects are bounded to the retained fixture and current code order. They
do not predict original GUI output, final `.DS` bytes, Save/Close effects, or
success parity. No test file was added because there is no qualified successful
public Type2 path. No build, GUI, or mutable REA session was used.
