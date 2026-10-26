# Checkpoint 70: Smallest isolated Type2 PAR helper

## Native owner and order after refresh

After the Type2 public Save arm sends `signalDoeTableRefresh`, it continues with four independent calls on the **same manager pointer cached from active SUB `+0x20`**. CP64 retains the native order and exact calls:

| Phase | Native call site | Target | Native output |
|---|---:|---:|---|
| Current selected mode | `0x1400997b2` | `0x1400d75a0` | `DoeStudies/<Study.name>/Control/DEF_DOE.PAR` |
| Fixed mode 0 | `0x140099a77`, `EDX=0` | `0x1400d7790` | `Control/Samples_LHS.par` |
| Fixed mode 1 | `0x140099d7c`, `EDX=1` | `0x1400d7790` | `Control/Samples_FF.par` |
| Fixed mode 2 | `0x14009a081`, `EDX=2` | `0x1400d7790` | `Control/Samples_USR.par` |

The actual manager is the active SUB's `+0x20` object. Current source represents this as `SubProject::ownedDoeStudy` plus the `SubProject::doe` alias; `Study::sub` links the Study back to that SUB. Sample state is on that Study: selected mode at native manager `+0x40`, three validity bytes beginning at `+0x70`, and table storage beginning at `+0x58`. No new manager, sample generator, or alternate active Study is involved. After the refresh callback, later writes must remain bound to the original cached manager; re-reading `activeSubProject` could silently route later phases to another Study.

## Exact predicates and ordinary failure behavior

The CP64 full-body evidence distinguishes the two helper entry points:

- **Current mode (`0x1400d75a0`)** starts with false, reads `samples.mode`, then its validity byte. Invalid state returns false without opening `DEF_DOE.PAR`. The original does not range-check the selected mode before indexing; the current reconstruction's unsupported check outside 0..2 is an explicit adapter guard, not a native false branch. A valid table with zero rows is still passed to the writer and emits its mode header.
- **Fixed mode (`0x1400d7790`)** returns false for mode outside 0..2 or for that mode's invalid flag. Otherwise it passes the selected table to the same writer. The public caller supplies only modes 0, 1, and 2.
- **Shared PAR writer (`0x140915360`)** opens QFile WriteOnly|Text. Open failure returns false. After a successful open it writes the mode header, one-based row numbers, each numeric value plus a trailing tab, and Text-mode line endings, closes, and returns true without checking QFile.error or QTextStream status. Its Boolean therefore means open success in this recovered path. A successful open truncates in place; there is no mkdir, temporary replacement, rollback, or post-close retry decision at the study-helper level.
- The four caller Booleans are not tested as gates. Invalid/current mode or failed open is an ordinary false result: the next fixed-mode helper is still called. Keep that continuation separate from a reached unsupported owner/callback guard, which would stop later work while retaining earlier effects.

InputTable's QFile post-close error Boolean is a different contract and runs earlier. The refresh wrapper dispatches regardless of its ordinary XML result. This report does not qualify or implement refresh delivery; it identifies the PAR sequence that follows that source phase.

## Candidate helper that can be isolated

The smallest reusable addition is a **mode-specific immediate writer** with a narrow contract such as `writeSampleTableNow(const Study&, int mode, const String& path, StudyIo&)`:

1. Apply the same fixed-mode range and `samples.invalid[mode]` gates before opening a file.
2. Serialize the selected retained rows using the already sourced PAR format. A valid empty table emits the mode header alone.
3. Call the existing `StudyIo::writeTextFile(path, bytes)` at this reached phase and return its open-success Boolean unchanged.

This is distinct from current `saveSampleTable(..., FileEffects&)` and `saveCurrentSamples(..., FileEffects&)`, which only append deferred payloads. Calling those planners from native Save without a same-phase executor loses their output; `saveProjectFileNative` still has a local `unused` FileEffects bucket. `StudyIo::writeTextFile` already has the right PAR Boolean contract and immediate semantics: native uses in-place WriteOnly/truncate then records open/write/close observations; recording updates the shared simulated file store immediately. It does not apply a second Text translation because the sample serializer already emits `\r\n` bytes.

Keep current-path selection as a thin separate wrapper: read `Study::samples.mode`, retain the explicit out-of-range adapter guard, build the nested `DoeStudies/<name>/Control/DEF_DOE.PAR` path, and call the same mode-specific writer. This keeps the two native predicates visible without duplicating the byte serializer. Do not have the helper create directories; native sample writers rely on earlier study construction and do not mkdir here.

The helper can be tested in isolation against a declared `Study` plus a `StudyIo` recording/failure fixture without changing `subProjectElement` or public Save admission. The current `writeSampleTable` is already a pure FileEffects serializer; the new seam should reuse its formatting logic (factor out a pure byte builder if needed), not clone a competing format implementation.

## Minimum focused tests

- Valid mode with an empty, valid table: immediate write of only `<mode>\r\n`; prove no empty-row skip.
- Valid rows: exact mode header, one-based IDs, trailing value tabs, CRLF; use integer-valued doubles to avoid introducing a locale dispute.
- Invalid table flag: false and no open/write attempt. Fixed invalid mode: same. Current selected mode outside 0..2: bounded unsupported guard before indexing/effect, recorded as adapter guard rather than an original branch.
- Open failure: false, no new simulated content, and no exception. Successful-open write/close failure observations must not change the helper's Boolean from true because the native PAR caller reports successful open only.
- Ordered caller-level helper test (still not public Save): force current PAR open false, then verify modes 0, 1, and 2 are still invoked in order. Also verify invalid LHS does not prevent FF/USR. Assert call path strings and per-mode tables.
- Study ownership fixture: use the same actual Study selected from the SUB's owned pointer and verify the helper does not resolve another active SUB, mutate samples, or regenerate rows.

The first two helper tests are standalone and cannot establish complete Type2 Save behavior. The caller-order test may exercise a small source-level orchestration helper with an explicit same-Study reference, but it must not deliver `DoeTableRefresh`, append/replace Project DOM, write final `.moproj`, or remove the public guard.

## Integration boundary

The public source sequence remains **XML -> DoeTableRefresh -> current PAR -> LHS -> FF -> USR -> DbName -> old Sub_Project replacement -> final `.moproj` serialization**. Today the Type2 writer stops at the unsupported refresh receiver boundary after XML, so the new immediate PAR helper must remain detached until the actual refresh lifecycle owner is qualified. Source support for sample formatting and immediate StudyIo writes does not authorize calling it before refresh or publishing Type2 Save success.

The eventual orchestration must retain the original manager across refresh, preserve all earlier effects if a later phase refuses, continue after each ordinary false result, and keep Type3's distinct `.OPT` arm guarded. Complete Type2 still depends on actual refresh receiver/timer/UI ownership, variable/InputTable behavior, backend/population state, and later DOE outputs.

## Evidence boundary

Native evidence: `analysis/resume64-doe-save-effects.md`, `analysis/resume64-doe-public-call-receipts.asm`, and `analysis/resume64-doe-save-effects-pe-receipt.json`, all tied to original GUI PE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Candidate qualifications read: `analysis/resume64-doe-sample-save-binding.md`, `source/apps/moproj2mst/src/doe/doe_study.cpp`, `include/m2m/doe_study.hpp`, `include/m2m/doe_io.hpp`, `src/doe/io.cpp`, and `include/m2m/model.hpp`. No source/build/test/GUI/runtime/REA changes were made; no new original capture was performed.
