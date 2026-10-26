# B127 active Type 2 Open-path probe

## Probe setup

I built and ran a standalone public-core probe in the unique directory `build/b127-active-probe-1792867845145/`. The probe source is `probe.cpp`; its private wrapper CMake file is `CMakeLists.txt`; the private generated build is `build/`. The wrapper adds the existing converter project as a subdirectory and links the probe to `m2m_core`; no shared `CMakeLists.txt`, shared build directory, or production source/test file was edited. The private build completed successfully after one scratch-probe include correction (`build-final.log`, exit 0). The final instrumented probe executable SHA-256 is `abd9b968821dce91c5b6e090f97131fabed2a755482ccd5fcc7ee180cc15b503`. The source tree digest corresponding to the current checkpoint50 candidate sources is recorded in `analysis/resume50-close-dialog-focused-build.json` as `448a734c8c68b3789fdb11038f8b3a9809c3b6500bbadfd186aba614069dbfe8`.

The probe uses the public core setup sequence (`ConversionSession`, `initProject`, `makeRegistry`, `openProject`) and explicitly calls `activateSubProject(project, 1, ctx)`, then `engine::openActiveOperation`. It binds the same native `StudyIo` and session-owned runtime holders used by the public Save path. The input was a fresh disposable copy of `evidence/gui_mo/oracle_b/127_B_DOE_OPEN_before`; its `.moproj` hash is `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e`. The retained fixture is unchanged. Two independent copies were run; the instrumented second run retains the exact final partial in-memory document state in `probe-instrumented.log`.

## Result and first guard

`openProject` succeeded with two subprojects and the model's initial selection at index 0. The explicit activation call returned successfully with subproject index 1, type 2, loaded state true, active diagram item index 5, and class `SfOprDoeOutput` (operation id 6). The subsequent active-operation load stopped with `M2M_E_UNSUPPORTED` (code 7):

```text
DOE session 0x140650e60 DBLIST/ReportList import remains unsupported
```

The first element causing this refusal is `DBLIST`. In the retained `.DS`, `SessionRoot` contains `DBLIST` first, then `ReportList`, then the `Session` containing `Image`, `StepData`, and `SfTask` (see its lines 3–10 and 85–93). Current `session_records.cpp:125-142` appends a `SessionPart` and assigns its root before iterating those children, then refuses when it reaches `DBLIST`/`ReportList`. The instrumented run confirms this partial state: one part, root present, no session records, `currentSession=-1`, operation range 1–5, and no selected current part. Because `DBLIST` is encountered first, this input does not reach the later Session's Image/QImage, StepData, Task-list, task-factory, or DOE Output Save branches. The `.DS` contains one Image tag, but that tag is not the first reachable guard on this fixture.

## Side effects before refusal

The probe used native StudyIo and retained raw pre/post manifests (`input-before-manifest.json`, `input-after-manifest.json`, `instrumented-before-manifest.json`, `instrumented-after-manifest.json`). The instrumented run began with 71 files and ended with 73. It added only `Samples_FF.par` and `Samples_USR.par` (each 3 bytes; SHA-256 `f1b2f662800122bed0ff255693df89c4487fbdcf453d3524a42d4ec20c3d9c04` and `df4e26a04a444901b95afef44e4a96cfae34690fff2ad2c66389c70079cdff2b`). It rewrote `Samples_LHS.par` from 495 bytes, SHA-256 `000edf802c560ab52196be82feea8ae0732cf263dcd1fe8b3e6c9f6b1ab0f42a`, to 3 bytes, SHA-256 `13bf7b3039c63bf5a50491fa3cfd8eb4e699d1ba1436315aef9cbe5711530354`. Each write opened and closed successfully; the requested content was the mode header `0\r\n`.

Before the active-operation refusal, constructor I/O reported the existing study and Control directories as present, then successfully created `DoeStudies/DoeStudy1/Solutions` and `DoeStudies/DoeStudy1/Output`. Lazy activation read `DoeInputTable.XML` (249 bytes) and `DEF_DOE.PAR` (495 bytes), and wrote the three sample files in mode order. The active-operation loader then opened and read all 2,013 bytes of `COG_P1_B1.DS`; it made no changes to that document or the project file. The exact `.DS` SHA-256 remained `97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d`.

The file deltas are input-side lazy-load effects, not DOE output generation. This probe did not call a Save writer after Open; the first guard prevented the load from completing, and the report does not claim an active DOE Save result.

## Source qualification and scope

- `core/convert.cpp:223` shows why the ordinary public driver does not take this path: it unconditionally activates subproject index 0. This probe explicitly selected index 1 through the core API to test the otherwise-unreachable active Type 2 branch.
- `model/subproject.cpp:310-356` performs lazy activation and calls `doe::readLazyStudy` for Type 2 `SfDoeStudy`.
- `doe/activation.cpp:31-43` reads InputTable, then processes sample files in LHS/FF/USR order before reading `DEF_DOE.PAR`; `doe/sample_io.cpp` preserves the immediate native write semantics.
- `ops/noop.cpp:138-185` loads the `SfOprDoeOutput` operation and calls the document reader; `doe/session.cpp:37-72` resets/reads the `.DS` document and enters record import.
- `doe/session_records.cpp:125-142` documents the partial Part append and the first `DBLIST`/`ReportList` refusal.

This is candidate-only source/build behavior using a historical retained input, not fresh-original parity. It shows the current active branch's first blocker and its preceding writes/deletions/directories. It does not show public default-subproject behavior changing, nor does it establish original behavior after those effects.

## Raw artifacts

All disposable inputs and receipts are under `build/b127-active-probe-1792867845145/`: `probe.cpp`, wrapper `CMakeLists.txt`, `build/`, `build-final.log`, `probe.log`, `probe-instrumented.log`, `result.json`, `instrumented-result.json`, and the before/after manifests. The first probe run and instrumented replay used separate copies of the retained fixture. No original application, GUI, shared build, shared CMake, production source/test file, or mutable REA database was used.
