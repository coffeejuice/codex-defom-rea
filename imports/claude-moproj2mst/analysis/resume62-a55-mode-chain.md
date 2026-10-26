# CP62 A55 MoInputMode chain and discriminator

## Finding

The A55 `SOLMTD` delta cannot be assigned to the CG callback without binding the original application's global MoInputMode and the active tree item. The retained evidence now supplies a strong setting hypothesis: `evidence/gui_mo/oracle_b/_tools/settings_backup_l5s/DEFORM-MO.ini` contains `[Setting] MoInputMode=1` (SHA-256 `909d618a352e7f1c972a53b887aa5978e933d875ff4b260a8dd107fa4797b4af`, file timestamp 2026-10-22 09:12). But the snapshot has no receipt that binds it to the particular A55 historical Save event. Treat mode 1 as plausible, not as the established A55 launch value.

There is also an established candidate-side mode mismatch in the diagnostic sweep. `source/apps/moproj2mst/tests/s/test_save_folder.cpp:305` calls the C++ `m2m::convert(project, ..., M2M_FLAG_WRITE_SAVE, 1252)` overload without an `OriginalSettings` argument. `source/apps/moproj2mst/src/core/convert.cpp` delegates that overload with `OriginalSettings{}`, whose `moInputMode` default is 0. Thus the five-diff folder sweep ran its candidate using the mode-0 editor selection. It did not reproduce the mode-1 setting recorded by the retained INI snapshot.

The A55 raw mismatch remains exact: Task00002 `SimCtrl.KEY` changes from `[0,0,0,0]` to `[0,1,0,1]` after `SOLMTD 2 4`; offsets 1439 and 1455 are the only byte differences. These are the second/fourth CG fields at `SC+0x2f8` and `SC+0x308`. No evidence says the `.moproj` stores MoInputMode.

## Original 0/1 decision chain

Retained fixed-PE evidence attributes the mode selector to original `0x1405ff0b0`: mode 0 chooses `SfPreSimControl100`; nonzero chooses `SfPreSimControl300`. The current candidate mirrors this branch at `source/apps/moproj2mst/src/ops/family.cpp:181-184` using `p.originalSettings.moInputMode != 0`.

The retained original Boolean callback audit distinguishes two global mode actions:

- `0x14002fc20` calls the global setter `0x140040c00(..., 0)` and emits the zero-argument `signalMoInputModeChanged()` notification.
- `0x14002fe50` calls `0x140040c00(..., 1)` and emits the same notification.
- Metadata local method 1 identifies `slotMoInputModeChanged()`, which dispatches to `0x140d3c070`. That slot reads the current tree index and invokes `0x140d3b7d0(index, true)`, the editor create/apply path.
- The direct callers identified for those producers are action thunks `0x14000ace0` / `0x14000ad10`, rather than the fresh Open wrapper. The wrapper's direct call sequence does not by itself establish a mode change; the separately captured settings value and any prior action state are the relevant inputs.

The target fixed binary is SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Source qualification for the producer/slot chain is `analysis/wave3-boolean-external-callback-contract.json` and its disassembly/decompile children. This is static call evidence, not an execution trace.

The CG decision is downstream of mode selection:

| Fresh original input/state | Expected CG path from retained source | Result relevant to A55 |
|---|---|---|
| `MoInputMode == 0` | Factory selects editor100; the editor300 iteration fill and method 64 CG slot are not selected by this mode branch. | This callback does not explain a fresh `[0,1,0,1]` result. Keep the cause open; the historical output may include other session/action state. |
| `MoInputMode != 0` (captured producer values include 1), active Forming item is Task00002, `SOLMTD == 2`, editor300 setup has completed, and the CG radio changes unchecked→checked during initial Iteration fill | `0x140ec19f0` calls `setChecked(true)` at `0x140ec21b6`; auto-connect maps `toggled(true)` to method 64 / `0x140ed01b0`. With parameter count 4, second value `<=0` becomes 1 and fourth value outside `{1,5}` becomes 1. | `[0,0,0,0]` becomes exactly `[0,1,0,1]`; first/third remain 0. |

For the second row, all listed preconditions matter. The `.moproj` only identifies the selected subproject; the retained A55 project does not prove which operation is the active tree item at the moment of the first editor fill. Nor does static `setChecked(true)` alone prove a `toggled(true)` emission if the button were already checked. `setupUi` does not retain an explicit `Checked` property for the CG control; its initial state at the real fill boundary should be captured or sourced before treating the callback as certain.

## What candidate-only evidence establishes

`OriginalSettings::moInputMode` is an explicit headless input, and the C ABI adapter maps `M2M_MO_INPUT_MODE` to it (unset/empty = 0). This makes a mode-matched, non-GUI implementation feasible in principle: the source-qualified choice, solver test, parameter conditions, and model stores are deterministic. The existing `simcontrol300::normalizeCgAtOpen` is a bounded candidate projection, and `tests/mdt/test_simcontrol300.cpp` checks mode 0 versus mode 1, active versus inactive model, and parameter boundaries. Those fixture tests do not demonstrate the public editor-construction path.

The public mode-1 route currently cannot finish: `formingSimControl300Open` begins the session-owned `StagedEditor300` and then stops at the exact `0x6504abb0` private-palette guard before setup, auto-connect, or Iteration fill. The existing public regression `full_lifecycle_editor300_reaches_session_factory_and_exact_setup_guard` expects that refusal. Therefore:

- **Mode 0 can be exercised headlessly now**, but it bypasses the CG callback path.
- **Mode 1's narrow state transformation is unit-testable headlessly**, but the current public full-Save route refuses before the original callback boundary.
- **A faithful public mode-1 path is feasible without controlling a real GUI**, provided the bound editor owner/setup/connection/page-fill prefix is recovered and the original mode/active-operation inputs are preserved. A live original comparison is still the smallest independent way to qualify the setting and selection assumptions.

Do not replace this chain with a writer normalization or set mode 1 globally. If the retained `MoInputMode=1` snapshot matches the A55 original profile, the historical candidate-vs-original difference may primarily be an input-mode comparator mismatch; it becomes a candidate implementation defect only after equivalent mode and active-operation context are established.

## Smallest post-gate discriminator

Once the app-control release gate has a verifiable receipt, first record the original process's actual `DEFORM-MO.ini` path and `[Setting] MoInputMode` value/hash before opening the pristine `RS_55_Multiple_Operations_Example1_before` copy. Then capture a fresh Open→Save→Close under the approved default-dialog policy with a passive trace of these checkpoints:

1. mode value at process startup and any `0x140040c00` mode setter / `signalMoInputModeChanged` before or during Open;
2. first post-Open current tree index and the Forming operation it resolves to;
3. Task00002 initial `SOLMTD` count/values and CG button checked state immediately before Iteration fill;
4. whether `0x140ec19f0` executes, whether `setChecked(true)` changes false→true, and whether method 64 / `0x140ed01b0` runs;
5. model fields after Open and raw saved Task00002/Task00001 KEYs with hashes.

The decisive output for A55 is the pair of values at `SC+0x2f8` and `SC+0x308` immediately after initial fill and in raw Save. If actual mode is 0, do not expect this editor300 callback. If actual mode is 1 but Task00002 is not the current item, or the fill/checked transition does not occur, do not attribute the final bytes to the callback. If mode 1, Task00002, solver 2, false→true and slot delivery all align, `[0,1,0,1]` is the source-predicted result.

For candidate reproduction, use the exact captured mode. `M2M_MO_INPUT_MODE=1` can express that input through the C ABI, but the current public candidate will stop at `0x6504abb0`; capture that result as a known implementation guard, not as a parity comparison. A mode-0 sweep result cannot be compared directly with a mode-1 original result. Keep both raw output trees and do not alter the historical Oracle-B files.

## Evidence reviewed and limits

- A55 raw fields and callback: `analysis/wave2-save-diffs-diagnosis.json`, `analysis/wave2-save-diffs-original-slot-identity.json`, `analysis/wave2-save-diffs-original-iteration-fill.asm`, and `analysis/wave2-save-diffs-original-meta-dispatch.asm`.
- Mode producer and callback identity: `analysis/wave3-boolean-external-callback-contract.json`, `analysis/wave3-boolean-open-inputmode-14002fc20.c`, `analysis/wave3-boolean-open-inputmode-14002fe50.c`, and `analysis/wave3-boolean-open-inputmode-1400271b0.c`.
- Captured setting snapshot: `evidence/gui_mo/oracle_b/_tools/settings_backup_l5s/DEFORM-MO.ini` and sibling `sha256.txt`. It states `MoInputMode=1`, but does not bind to A55's original process/session.
- Candidate mode path: `source/apps/moproj2mst/tests/s/test_save_folder.cpp:305`, `source/apps/moproj2mst/src/core/convert.cpp`, `source/apps/moproj2mst/include/m2m/common.hpp`, `source/apps/moproj2mst/src/ops/family.cpp`, `source/apps/moproj2mst/src/engine/loader_l3.cpp`, and `source/apps/moproj2mst/tests/mdt/test_simcontrol300.cpp`.

All original executable evidence used here is retained static/decompile or filesystem evidence. No original runtime, source build/test, GUI control, or mutable REA action was performed. The capture recommendation remains conditional on verified app-control release.
