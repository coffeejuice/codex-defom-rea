# CP171: remaining original auto-save callers and a non-GUI lifecycle

## Result

The retained original `DEF_GUI_MO_64.exe` **v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, has a newly identified **conditional, input-free postOpen route to auto-save** through `0x14003a990`. This corrects CP170's narrower suggestion that only `0x14002a9b0` lies on fresh startup Open. It does **not** establish an original Open → Save → Close oracle: the refresh-derived Save has unproved data and busy guards, and neither remaining caller supplies a source-bound non-GUI FileCloseAll/accepted Close trigger. The second caller `0x140116bd0` is a changed mode-button handler, not a startup command.

This pass read retained PE bytes, DUMPBIN listings, Ghidra text and existing contracts only. It did not launch the original application, acquire GUI control, use live/mutable REA, touch installed files or change source/tests/shared state. Only this report was written.

## First caller: postOpen refresh may call Save

The source-backed conditional chain is:

```text
main -m open -f ...: 0x140001c8f Open(01ca30)
  -> 0x140001ca9 postOpen(02a0e0)
  -> 0x14002a19c select(02a9b0)
     -> 0x14002aa8a busy=true
     -> 0x14002aabc virtual main+7f0 (014260), while busy
     -> 0x14002aacb busy=false
  -> 0x14002a98f virtual main+7f0 (014260), if main+3c0 remains zero
  -> 0x1400144bf update(0151a0)
  -> 0x1400155ec slotStepBrowserVisibilityChanged(true) (03a990), if derived visibility=true
  -> 0x14003aaa2 auto-save(010b80), if current Project, process type 5, true argument and unbusy
  -> 0x140010c17 top Save(0980d0), if wrapper's own current Project/type guards pass
```

The main CLI Open call sites are in `analysis/wave3-editor-owned-open-entry-140001410.asm` at `1c8f/1ca9`; this main branch has no explicit Save/Close argv command (`analysis/wave3-native-cli-save-feasibility.json`). `analysis/wave3-boolean-external-callback-14002a0e0.asm` shows `02a107` writes main+3c0=0 and `02a978` conditionally admits the later virtual+7f0 at `02a98f`. `analysis/wave2-doe-model-manager-postopen-selection.asm` shows the earlier virtual+7f0 at `02aabc` **before** the `02aacb` false-busy setter. The second call is the meaningful Save candidate; preceding callbacks could change main+3c0 or busy again, so source alone does not certify its occurrence or admission.

`analysis/wave3-boolean-external-callback-140014260.asm` shows convergence to the consecutive update helpers and the direct `0144bf -> 0151a0` call. DUMPBIN `0151a0..015615` shows `01d510` obtains the current Project at `0151d6`; the value at stack+74 is computed through project-dependent helpers `014120/014210/014170/0141c0`, `2cb7f0/2cb830`, and a later `12e85a0` exclusion. At `01557e..01559d`, process type 1 or `2cb830` and the derived **true** flag admit the call; `0155e2` sets `dl=1`, and `0155ec` calls `03a990` on the same main. This is an internal direct call, not proof that Qt emitted a dock signal. The same slot is also Qt metadata method 12 (`analysis/resume117-doe-mainwindow-methods.json`; retained `evidence/gui_mo/s1/mdt/L0/C/c4s.c`, case `0xc`). Its constructor connection from dock `main+420` to `slotStepBrowserVisibilityChanged(bool)` establishes another possible producer, but registration alone does not establish emission.

DUMPBIN `03a990..03aadd` shows `01d510` current-Project nonnull at `03a9af/b7`, process type `2cb7c0` (1 or 5) at `03a9c4/ce`, true argument at `03a9d4/de`, and a process-type-5 branch at `03aa65/6d`. That branch calls `2ca920(show=true)` at `03aa81`; a busy result jumps past Save at `03aa91..03aa98`, while unbusy reaches `03aaa2 -> 010b80`. The process type 5 qualification for successful native `.moproj` Open follows `analysis/wave3-normal-close-guard-contract.json`: `02a0e0 -> 013d40` derives type from the freshly loaded Project, and native reader success sets Project+0x55. `2ca920` also reads singleton+9 and actual main virtual+300 busy state (`analysis/resume101-close-live-owner-chain.md`), so the false setter alone does not establish unbusy. The auto-save wrapper can still reject absent current Project or non-5 type, and it ignores the top Save result while returning success (`analysis/wave3-boolean-save-io-contract.json`). No Save occurrence or successful file write was observed in this read-only pass.

## Second caller: mode-button transition

The retained Qt metadata at `0x142171580` (strings `0x142171800`) belongs to `SfMoTreeViewDock`. Its local method 14 is `on_btnGrpMode_buttonClicked(QAbstractButton*)`; the local jump entry `0x1400de766` loads the button argument and calls `0x140116bd0` at **`0x1400de777`** (`analysis/wave3-boolean-reselect-dispatch-1400de5f0.asm`). This is distinct from local method 12 `slotUpdateCheckButtons` documented in `analysis/wave3-boolean-reselection-contract.json`.

DUMPBIN `116bd0..116d25` shows the handler reads global current main and casts it to the primary main at `116bf1..116c1b`; null returns. It maps the supplied button against dock UI pointers at `[dock+0x28]+0x20/+0x28/+0x30` (`116c30..116c92`), exits if selected mode equals dock+0x58 (`116ca6..116cab`), and only on an **old mode zero** with singleton process type 5 (`116cb8..116ccf`) calls auto-save at **`116cd6`**. No retained CLI startup call to this handler or source-backed programmatic button-click occurrence was identified in the inspected `main` startup branch. The metadata permits invocation; it does not make a button event an input-free Open continuation. It has no Close call.

## Close and next executable action

The accepted Close route remains actual main Qt method 20 `slotActionFileCloseAll()` → `0x1400db5ff` → `0x140031f10` → `0x1400299a0` (`analysis/resume107-app-main-lifetime-call-boundary.md`). `analysis/resume153-accepted-close-trigger.md` source-binds its `QAction::triggered()` connection, but no CLI or other input-free signal delivery after Open is established. The alternate `0x140010b10` virtual+2d8 wrapper has preceding effects and likewise no known non-GUI startup trigger. Event-loop return followed by main destructor (`analysis/resume149-close-next.md`) is a different cleanup path, not accepted FileCloseAll, and lacks an established non-GUI exit trigger. Therefore neither remaining auto-save caller presently permits a **source-bound full Open → Save → Close comparison on one Project lifetime**.

The next **safe executable step is offline only**: resolve the finite predicate chain `014120/014170/0141c0/014210`, `2cb7f0/2cb830` and `12e85a0` for the selected fresh CLI `.moproj` Project, then audit direct/vtable/metadata producers of `010b10` and method 20 for a non-GUI postOpen Close trigger. Use the retained PE and bounded disassembly; preserve the same source Project and actual main/manager identity in any future proposed harness. If those predicates close favorably, an owned, disposable **passive** original process observation could test the Save hypothesis in a separate authorized execution lane, but it would still not be a full Close oracle. Do not claim acceptance from direct Frida calls to Save/Close, process termination, or Qt action stimulation under the present GUI-control release gate (`python tools/gui_validation_status.py` reports `release_ready=false`).

The direct-call inventory remains the bounded `.text` pattern scan in CP170. This report qualifies selected callers and branches; it does not prove absence of all indirect callbacks or external automation.
