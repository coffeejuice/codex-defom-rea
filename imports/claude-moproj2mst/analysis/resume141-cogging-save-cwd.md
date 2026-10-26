# CP141: Native Save-time directory for MAN_COUPLE.DAT

## Finding

The native Open/operation-load chain establishes the project folder as current, and the recovered synchronous Save body contains no explicit current-directory change before the Cogging `vt+0xd8` callback. This is strong continuity evidence, but **not a complete Save-time guarantee**: `0x1400980d0` calls `QCoreApplication::processEvents()` before it traverses subprojects and invokes `0x140052ec0`; the retained static evidence does not close the effects of events dispatched there (or indirect callback effects). Retained oracle folder diffs record saved artifacts and times, not the process CWD at Cogging entry. Therefore do not yet replace the public project-relative file probe/refusal on this evidence alone.

## Qualified order

1. OpenProject `0x14008a9b0` stores the cleaned `.moproj` path and calls `QDir::setCurrent(projectDir(P))` before returning (`evidence/gui_mo/s1/s1-01/spec.md` §1; `analysis/resume76-open-save-dialog-filter-path.md`).
2. The lazy operation loader `0x140051340` / `0x140051c30` calls `QDir::setCurrent(projectFolder)` immediately before its slots (`source/docs/re/moproj2mst/mdt/L1/spec_L1A.md`, opening call-order note). The retained native Cogging spec locates `QFile::exists("MAN_COUPLE.DAT")` in manipulator placement (`source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` §5).
3. The immutable Save decompile `evidence/gui_mo/s1/mdt/save_d0.c` shows `0x1400980d0` first calls `0x1400a4090`, then `QCoreApplication::processEvents()`, then checks subprojects, resolves the active operation and calls `0x140052ec0`. That callback runs Cogging's `vt+0xd8` (`0x14075c260`) before the `.moproj`, properties/MDT, and MST steps. The saved bounded PE receipt `analysis/wave3-boolean-save-io-contract.json` binds this order to original hash `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
4. The Save body has no direct `QDir::setCurrent` call in this prefix. That absence does not constrain the event work serviced by step 3. No retained run log samples CWD at `0x14075c260` or `0x140762f30`.

## Smallest next witness / bounded port contract

To qualify the public positive branch, use a fresh disposable original run: open a project whose directory has an empty `MAN_COUPLE.DAT`, activate its spring-enabled Cogging operation, then at entry to `0x140762f30` (or immediately before the `QFile::exists` call) capture `QDir::currentPath()` and the result of `QFile::exists("MAN_COUPLE.DAT")`. Record the process-event interval and any directory change; repeat with the file only in the launch directory as a path-disambiguation control. This is the missing witness; existing `42_vdie` oracle captures have no file and no CWD observation.

Without that native witness, the safe isolated helper contract is `coupledManipulatorsExists(explicitLookupDirectory)`, with tests for present/absent and no implicit dependence on process CWD. The source-qualified consequences, conditional on a true existence result, are exact: object 5 receives movement-reference id 4/value 1.0, object 7 receives id 6/value 1.0. Do not wire the explicit project directory to the public native path yet, and do not infer any content parsing from `MAN_COUPLE.DAT`.

Read-only retained-source inspection only. No source, build, GUI, or mutable REA state was changed.
