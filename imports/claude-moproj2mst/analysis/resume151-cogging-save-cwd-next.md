# CP151: Cogging Save-time CWD mutation frontier

## Finding

The retained synchronous Save prefix contains no direct current-directory setter between its event pump and the relative Cogging lookup. This narrows the issue: the project-root directory established at Open/operation load is the expected directory on the uninterrupted path. It does **not** close a run that services a CWD-changing callback during the pump (or a synchronous signal/callback), so the relative `MAN_COUPLE.DAT` result cannot be derived from project ownership alone.

## Bounded call-path result

Original `evidence/gui_mo/s1/mdt/save_d0.c` orders `0x1400980d0` as: open-state check; `QCoreApplication::processEvents()`; subproject checks; resolve active operation; call `0x140052ec0`; then project write, property saves, and MST. Cogging's `0x14075c260` / `0x140762ec0` / `0x140762f30` occurs only under that active-operation callback. The loaded Cogging callback performs `QFile::exists("MAN_COUPLE.DAT")` relative to process CWD in the spring branch.

A read-only run of the retained `evidence/gui_mo/s1/mdt/L1/A/iatcall.py` against `evidence/original-binaries/DEF_GUI_MO_64.exe`, querying `setCurrent`, enumerated direct Qt `QDir::setCurrent` callsites. The source-qualified relevant ones are:

- `0x14008a9b0` OpenProject: sets project directory before returning.
- `0x140051340` / `0x140051c30` operation loaders: set project directory before load slots.
- `0x1400a2200`: single-operation run/DOE preparation path, explicitly excluded from File > Save in `source/docs/re/moproj2mst/s1-02/spec.md` and `s1-03/spec.md`.
- The other enumerated direct setters were not exhaustively traced through their callers here. The retained Save decompile and Cogging call tree contain no direct edge to them before the lookup; their possible reachability through dispatched callbacks remains open.

The setter inventory is not itself a dynamic call graph. `QCoreApplication::processEvents()` deliberately dispatches pending work before active-operation resolution. That work may reach OpenProject or an operation loader, both known setters, and could change which project/operation is active. Static direct-call absence cannot rule that out. Save operation messenger callbacks also are not covered by the CWD inventory. The retained oracle folder comparisons contain no CWD trace or observation of `QFile::exists` at `0x140762f30`.

## Smallest same-run witness

At one fresh original File > Save, capture CWD and project identity immediately before/after the `processEvents` call in `0x1400980d0`, then at entry to `0x140052ec0` and immediately before the `QFile::exists` call in `0x140762f30`. During the pump, log any entry to the imported `QDir::setCurrent` (and the sole `SetCurrentDirectoryA` call) with caller/target. Record the boolean result of the relative existence call. A control with `MAN_COUPLE.DAT` only in the launch directory distinguishes lookup resolution if the directory changes. This tests the exact original path; a candidate run or CWD sampled only after Open is not a substitute.

Until that witness exists, public project-directory lookup remains unqualified. CP143's reference helpers remain valid as explicit-bool field projections, but their normal Save callsites should not be enabled from `projectDir(p)` based only on static continuity.

## Evidence and scope

- Native Save order: `evidence/gui_mo/s1/mdt/save_d0.c`; corroborating bounded original PE receipt `analysis/wave3-boolean-save-io-contract.json` (original SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`).
- Open and operation-load directory setup: `evidence/gui_mo/s1/s1-01/spec.md` and `source/docs/re/moproj2mst/mdt/L1/spec_L1A.md`.
- Relative Cogging check: `source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` §5.
- Save/operation-load and run-path separation: `source/docs/re/moproj2mst/s1-02/spec.md`, `s1-03/spec.md`.

Read-only PE/corpus/source inspection only. No source, build, GUI, or mutable REA state was changed.
