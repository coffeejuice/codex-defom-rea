# CP61: public PPV Open→Save lifecycle test

## Result

Added `tests/mdt/test_ppv_public_lifecycle.cpp`. It copies the retained
`93_B_MB3ACT_before` project into the configured disposable test scratch area,
selects its second diagram item in that copy, adds one source-qualified local
PPV row, then resolves and calls
`m2m_convert_w` from the sibling `moproj2mst.dll`. This exercises the exported
C ABI while keeping the assigned test independent of CMake and production
source edits.

The current source path now admits the row: `loader.cpp::loadSimCtrl` calls
`readPpvOpenSequenceFromDisk` and rejects only unsupported input or an
unavailable owner. `save_current.cpp::saveSimCtrl` writes PPV after SimCtrl.KEY
when local recovery-B points exist, using the operation task path and the
known ASCII-compatible codec. The fixture supplies one local-only row, so the
test expects `1\r\n0\tfalse\t7\t8\t9.5\t10.5\t0\r\n`, the writer's current
source-qualified form. It also requires `M2M_OK`, an MST output, and the
rewritten PPV file. This is a focused one-call lifecycle assertion; it does
not claim parity for other PPV profiles, shared-point rows, encodings, or
post-open write failures.

The retained XML has `<Sub_Project Active="2">`, while
`src/model/subproject.cpp::parseSubProject` compares `Active` to a zero-based
`DiagramItem` index. The retained selection is therefore its third item
(`Operation_Id=3`), whose `Task00003` has no `SimCtrl.KEY`. Since the PPV reader
is reached only after the selected operation's KEY exists, injection under
that path would not exercise PPV. The disposable copy changes this attribute
to `Active="1"`, selecting `DiagramItem` 1 and `Operation_Id=2`. Retained
`Task00002/SimCtrl.KEY` exists, and its header has `CURSIM 2 2 0`. The test
compares a recursive manifest immediately after copying against the retained
source, then applies both the selection and PPV edits only inside scratch. It
verifies the output MST and rewritten PPV in the final scratch manifest and
rechecks the unchanged source manifest.

## Evidence and boundaries

- Reader ownership and local/shared branch: `analysis/resume50-ppv-reader-model.md`.
- Native PPV save path, local-before-shared serialization and delimiters:
  `tests/mdt/test_simctrl_ppv_save_file.cpp` and
  `src/engine/simctrl_ppv_save_file.cpp`.
- Public entry: `include/moproj2mst.h` defines `M2M_FLAG_WRITE_SAVE`; the test
  resolves `m2m_convert_w` by name from `moproj2mst.dll` and calls it once.
- Active-index and task-path qualification: `src/model/subproject.cpp::parseSubProject`,
  `src/model/opinfo.cpp::taskDirAbs`, retained `Gear_blank.moproj`, and
  `Operations/Task00002/SimCtrl.KEY`.
- A public conversion owns its Open/Save/Close scope internally; the API
  exposes no separate Close call. This test checks the public return and
  generated artifacts, not private close state.
- The test was not built or run in this lane. Root owns the next build and
  runtime result; successful behavior remains unverified until that receipt.
