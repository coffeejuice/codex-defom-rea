# Checkpoint64: shared Type2 Save node prefix

The shared `.moproj` writer now receives the conversion's existing borrowed
`ConversionSession` through `moproj_write.hpp`, `save_project.cpp` and
`convert.cpp`. Type0 callers retain the previous default argument. The native
writer still opens/truncates the project file before entering the DOM body.

At the Type2 arm, `write.cpp` calls the source-qualified
`appendType2SaveNodePrefix` helper with its cached active SubProject and the
pending new `Sub_Project` node. The helper checks the actual live
Session/Project/SubProject/Study/StudyIo/ModelManager identities, appends the
owned `SfDoeStudy` name to that pending node, and reports whether variables
lead next to dimension reconciliation or empty variables lead to XML device
open. The shared caller immediately returns `M2M_E_UNSUPPORTED` at that exact
unrecovered boundary. It does not install the pending node, write study files,
dispatch DoeTableRefresh, or claim public Type2 Save success. Type3 retains a
separate refusal.

The helper's focused test calls the real shared recording writer with an
actual Session-owned Type2 Study and verifies earlier root DOM mutations,
unchanged old SubProject identity, no project/study writes, and the precise
InputTable guard. Root also ran the Release `m2m.doe.type2_save_node_prefix`
and `m2m.moproj.moproj_write` targets; both passed. The complete checkpoint64
build and 208-test run passed with stable source/executable hashes and no
fixture changes. The original Type2 partial I/O and final result remain to be
reconstructed from the staged InputTable/refresh/PAR sequence in
`analysis/resume64-doe-type2-integration.md` and
`analysis/resume64-doe-save-effects.md`.

The retained B120/B127 Type2 SubProject is index 1; the current public C ABI
selects index 0. The internal shared-writer test does not establish a public
selection route or fresh original output parity.
