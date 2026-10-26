# CP156: Concrete CWD writer during the Cogging Save lookup window

## Finding

The uninterrupted synchronous File > Save prefix remains bounded: after `0x1400980d0` calls `QCoreApplication::processEvents()`, it performs the subproject check, resolves the active operation and calls `0x140052ec0`; there is no direct `QDir::setCurrent` call in that Save body or the straight Cogging callback path before `0x140762f30`'s relative lookup. On that uninterrupted path, the previously established project folder is the expected lookup directory.

There is, however, a concrete CWD-writing GUI route that the event pump can dispatch: main-window slot `0x140025b60` calls the File > Open dispatcher `0x14001ca30` (retained decompile `evidence/gui_mo/s1/p17/a8.c`, around its call near the function tail). The selected `.moproj` route calls `0x14008a9b0`, whose retained body calls `QDir::setCurrent(projectDir(P))` (`evidence/gui_mo/s1/s1-01/dc/open0.c`, `source/docs/re/moproj2mst/s1-01/spec.md`). The original QDir import caller inventory independently lists `0x14008a9b0` as a setter caller. Thus if a pending Open action is dispatched inside Save's explicit `processEvents` window, CWD can be set to the newly opened project's folder before the active-operation resolution and Cogging `QFile::exists("MAN_COUPLE.DAT")` call.

The static route proves **capability**, not that an Open action was pending or dispatched in any retained Save run. It also does not establish whether the reentrant Open changes which Project pointer/operation the still-running Save subsequently uses. No retained oracle evidence captures such a run or the lookup's actual CWD/result.

## Smallest remaining witness

One fresh original Save capture should instrument the interval around `QCoreApplication::processEvents()` in `0x1400980d0`: record entry/exit CWD and every `QDir::setCurrent` invocation with caller/target, then record CWD and relative `QFile::exists` result at `0x140762f30`. To discriminate the concrete route, arrange a queued File > Open to a second disposable project with `MAN_COUPLE.DAT` present only in the second directory and record whether the Open handler is dispatched during the Save pump. Also record the original Save's project/active-operation identity at pump exit and at `0x140052ec0`, since reentrancy can affect owner selection as well as CWD.

Until that witness, keep the CP143 helpers as explicit-bool projections and retain the public file-present refusal; do not substitute `projectDir(p)` for the native process-relative lookup. Candidate-private tests can continue to inject the existence bit directly.

## Source chain and evidence bounds

- Save pump and callback order: `evidence/gui_mo/s1/mdt/save_d0.c` / `evidence/gui_mo/s1/s1-02/a4s.c`; PE receipt `analysis/wave3-boolean-save-io-contract.json` binds the native image (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`).
- Main-window Open handler to dispatcher: `evidence/gui_mo/s1/p17/a8.c` (`0x140025b60` calls `0x14001ca30`).
- Open dispatcher to project open and directory change: `evidence/gui_mo/s1/s1-01/dc/sub0.c`, `open0.c`; `source/docs/re/moproj2mst/s1-01/spec.md` §1.
- Cogging relative lookup: `source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` §5 and `source/apps/moproj2mst/src/engine/cogging_save.cpp` spring branch.

Read-only immutable PE/corpus/source inspection only. No source, build, GUI, or mutable REA state was changed.
