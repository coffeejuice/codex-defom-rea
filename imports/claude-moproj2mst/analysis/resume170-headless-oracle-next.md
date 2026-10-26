# CP170 bounded non-GUI original Save/Close route search

## Result

The retained DEF_GUI_MO_64 v11.0.2 binary exposes an **auto-save** wrapper, but this bounded source-backed pass found no fresh startup CLI Open → Save → Close route. Original binary SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The new discriminating evidence is the wrapper's direct-caller inventory and the fresh postOpen guard, not a new acceptance path.

No original process, installed application, GUI control, or live REA/Frida session was touched. Only retained PE bytes, disassembly, contracts and scripts were read.

## Exact bounded inspection

`tools/save_diff_pe_xrefs.py` scanned the original PE's file-backed `.text` span **`0x140001000..0x141e02400` end-exclusive** for `E8 rel32` direct-call patterns, classified candidate sites by `.pdata` unwind bounds, and returned these hints. The scan is pattern based; its matches were checked against DUMPBIN instruction listings at the selected sites. It does not enumerate indirect calls, Qt metadata invocation, tail jumps or callbacks, and raw bytes alone are not a complete call graph.

| Target | Direct `E8` callers found | Source-backed interpretation |
| --- | --- | --- |
| Auto-save `0x140010b80..0x140010c6b` | `0x14002a9f7` in `02a9b0..02aad5`; `0x14003aaa2` in `03a990..03ae33`; `0x140116cd6` in `116bd0..116e91` | The wrapper calls top Save `0x1400980d0` at `0x140010c17`. DUMPBIN shows it first obtains the current Project through `01d510`, rejects null, and checks singleton state before calling Save. It logs “Auto saving...” and a saved message; it returns 1 even though the top Save result is ignored (`analysis/wave3-boolean-save-io-contract.json`). |
| Top Save `0x1400980d0..0x1400982ac` | `0x140010c17`, `0x14001c942`, `0x1400285ce`, `0x140044507`, `0x140eafc10` | These include auto-save, legacy import, explicit Save, nested Close Save and another bounded caller. A call-site list does not show that fresh CLI Open reaches any of them. |
| Close wrapper `0x140031f10..0x140031f28` | `0x1400db5ff` in main-window Qt static metacall | This is `slotActionFileCloseAll()` dispatch on the actual main, not a CLI dispatch (`analysis/resume107-app-main-lifetime-call-boundary.md`). |
| Close root `0x1400299a0..0x140029d04` | `0x140010b63` and `0x140031f1e` | The former is a separate main-wrapper call; the latter is FileCloseAll's wrapper. Neither occurs directly in the reviewed startup CLI branch. |

The only direct auto-save caller on the normal fresh startup Open path is **`0x14002a9b0`**, reached from postOpen `0x14002a0e0` at `0x14002a19c`. DUMPBIN range `0x14002a9b0..0x14002aad5` shows `01d510 → 0133a0 → 045780` and a null test at `0x14002a9da`; null jumps over the auto-save call at `0x14002a9f7`. A nonnull current Project still needs singleton check `0x1402cb870` to pass. The retained original Open contract (`analysis/wave2-doe-model-manager-actual-open-contract.json`, `ordered_native_branch` and `post_open_selection`) establishes that the fresh manager's **current model is null until this postOpen selection**; its old-current bodies skip on that profile. Thus the reachable first-Open `02a9b0` call does not itself produce auto-save. A later selection with an existing current model is a different session state, outside the selected fresh one-project acceptance sequence.

The other two auto-save call sites have their own guards in DUMPBIN ranges `0x14003a990..0x14003ae33` and `0x140116bd0..0x140116e91`: `03aaa2` follows process-type-5 and busy checks; `116cd6` follows a current field-change and singleton check. This audit did not source-close their full external triggers, so it does **not** label either unreachable globally. It establishes no command-line path from `-m open -f` to either site. The existing main argv inspection `0x140001b56..0x140001fb6` (`analysis/wave3-native-cli-option-literals.json`, `analysis/wave3-native-cli-save-feasibility.json`) names `open`, `new`, `prob_dir`, `q` options and delayed FileNew/RelaxDockSizeLimit slots, with no Save or Close dispatch in that bounded branch.

The additional top-Save caller `0x14001c942` is in `01c2f0..01ca1c`. The retained original extension contract identifies `01c2f0` as the legacy import/dialog branch for `dprj/d2prj/d3prj/d2mo/d3mo/d2md/dmd/mdt`, while native `.moproj` takes the other branch (`analysis/wave2-doe-model-manager-actual-open-contract.json`). It is not evidence that a fresh `.moproj` CLI Open auto-saves without GUI, and it offers no Close trigger. Post-event-loop main teardown at `0x140001ffc` is likewise conditional on `QApplication::exec` returning; retained `analysis/resume149-close-next.md` distinguishes that destructor cleanup from accepted FileCloseAll and gives no non-GUI exit trigger.

## Blocker and next safe action

Fresh original Save still needs a naturally delivered, source-qualified action after CLI Open. Fresh normal Close additionally needs FileCloseAll delivery and its busy/current-main, manager, modal and nested-Save state; process termination or Frida detach is not Close. Directly calling `0x140010b80`, `0x140031f10` or `0x1400299a0` from instrumentation would manufacture a stimulus and would not by itself establish equivalence to the accepted original lifecycle. Windows messages or Qt action activation would be GUI input under the standing release gate.

Keep the original comparison blocked. The next original-free static search, if needed, is narrowly the **external trigger/caller chain for `0x14003a990` and `0x140116bd0`**, including their Qt metadata/timer registration and whether either can be reached from a fresh startup CLI Open without UI input. No need to repeat the established main argv or auto-save direct-call scans. Any claimed route must also supply a normal non-GUI Close trigger, equivalent initial state, short disposable paths under `C:\PROBLEM`, side-effect capture and owned-process cleanup. Current `python tools/gui_validation_status.py` returned `release_ready=false`, so a GUI route remains gated.

This report is a bounded negative with one newly checked conditional auto-save path. It does not assert global absence of hidden IPC, indirect dispatch or undocumented automation.
