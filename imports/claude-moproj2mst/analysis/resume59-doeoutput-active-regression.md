# Checkpoint 59 DoeOutput active-dispatch regression

Added `source/apps/moproj2mst/tests/mdt/test_doeoutput_active_guard.cpp` as a
candidate-only regression continuing the explicit Sub_Project 1 path from
`analysis/resume54-connector-project-check.md`. It copies the complete configured
`M2M_ORACLE_B/120_B_DOE` fixture under the target's `M2M_TEST_SCRATCH`, checks
the source project/DOE-document identities, and compares relative-path,
entry-type, size, and SHA-256 manifests before and after dispatch. It opens the
copy through `openProject`, activates sub-project index 1, reasserts the
operation 3 -> 6 connector, its active item 5, and registry flags `0x1a` /
`0x20097`, then calls `engine::openActiveOperation` directly.

The first runtime result supersedes the earlier Image/QImage expectation. The
active dispatch reads the retained `.DS` (2,013 bytes, SHA-256
`97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d`), then
refuses at the first encountered `DBLIST` or `ReportList` child with
`M2M_E_UNSUPPORTED` and the exact message
`DOE session 0x140650e60 DBLIST/ReportList import remains unsupported`.
Before that refusal, Type 2 study setup performs three **recorded/simulated**
text-write attempts in order: `Samples_LHS.par` with `0\r\n`,
`Samples_FF.par` with `1\r\n`, and `Samples_USR.par` with `2\r\n`, all under
the copied `DoeStudies/DoeStudy1/Control` directory. The revised test asserts
each exact path, byte string, order, and successful recording status. These
records are observations by `RecordingStudyIo`; they do not mean bytes were
written to disk. The before/after scratch-tree manifest must still match, and
the retained source-tree manifest must also remain unchanged. The test asserts
no `engine::FileEffects` writes/deletes/creates/removals/renames, while it
explicitly preserves and reports the three DOE text-write observations. DOE Qt
warnings remain captured separately.

Source order explains why the earlier Image expectation was wrong for this
loader path. `session_records.cpp` calls `readSessionRecords` with the record
import mode that skips Image/GraphicsItems/SfDispView imports; `importPart`
visits root element children in document order, imports each `Session`, then
throws when it encounters `DBLIST` or `ReportList`. The retained file's first
Session therefore does not stop at QImage in this call configuration. The
observed DBLIST refusal follows study initialization and its three recorded
sample-file attempts.

The CMake target is auto-discovered by its existing
`tests/mdt/test_*.cpp` glob as
`m2m_mdt_doeoutput_active_guard` / `m2m.mdt.doeoutput_active_guard`; no CMake or
production file changed. The test checks no FileEffects writes, deletes,
directory creations, removals, or renames and expects the three recorded DOE
sample text writes described above. It checks input immutability separately
from those simulated write observations. Root reported the focused run's
observed DBLIST/ReportList refusal and three study text writes; this task updates
the expectation to match that result. Root owns the rebuild and acceptance run.
Revised test source SHA-256:
`0ea39c3910535e4f8a72d5ab260a23d6ddb5a12ed4e6dd2ae8079be06f6501c3`.
Input identities are project
`60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e` and DOE
document `97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d`.

## Qualification

Passing establishes a candidate path from full-project XML, real registry
and parser admission, through explicit second-subproject selection, into
DoeOutput active-operation dispatch and up to the first qualified `.DS`
DBLIST/ReportList guard, with the preceding three simulated study text-write
attempts recorded in order. It does not establish public C ABI selection of Sub_Project
1: the current converter explicitly activates index 0. It does not establish
original GUI dispatch, DoeOutput class-data completion, task/global-session
application, Save, Close/reset, or project parity. The mocked original
`0x140074f50` probe and retained Oracle-B files do not supply those results.

Only this test and this report were edited for the correction. No build, GUI,
project fixture, or REA database was run or modified by this task.
