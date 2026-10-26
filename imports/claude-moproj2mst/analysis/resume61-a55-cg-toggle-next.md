# CP61 A55 CG-toggle localization

## Exact changed fields

The retained A55 raw comparison is tightly localized to `Operations/Task00002/SimCtrl.KEY`:

- Both files are 2,146 bytes.
- Zero-based offsets 1439 and 1455 change from candidate ASCII `0` to retained-original ASCII `1`.
- On line 52, after `SOLMTD 2 4`, the four values are candidate `[0,0,0,0]` and retained original `[0,1,0,1]`.
- Candidate hash: `2bee048e6645fd36416d1b7582883c65a9727e5756715302a928ca3f1656854e`.
- Retained original Save hash: `79f9ec098a22b46f60e23072751a23fa036459f4db02f719e56e6421f243f79b`.

In the native SimCtrl record, these fields are `SC+0x2f8` (second CG parameter) and `SC+0x308` (fourth CG parameter). The first (`SC+0x2f0`) and third (`SC+0x300`) remain zero. The original reader at `0x140b69090` copies the explicit KEY values; it does not explain the two changes.

The source-qualified mutation is the true CG radio callback `SfPreSimControl300::on_m_rbtnDefSolCG_toggled(bool)` at `0x140ed01b0`. When `SOLMTD==2`, iteration-page fill `0x140ec19f0` calls `setChecked(true)` on the CG radio at UI offset `+0xa90` (`0x140ec21b6`); `connectSlotsByName` plus the retained metadata table identify that callback as method 64. The callback sets the second parameter to 1 when its integer value is `<=0`, and sets the fourth to 1 unless its integer value is 1 or 5. For this input `[0,0,0,0]`, with the retained count of four, it yields exactly `[0,1,0,1]`; it leaves the first and third values alone.

This establishes a plausible exact cause for the historical delta. It does not show that the callback ran during a fresh Open on this project.

## Earliest current candidate decision and missing input

The earliest unresolved decision is the input-mode gate in `source/apps/moproj2mst/src/ops/family.cpp:181-184`:

```cpp
simControl300Shown_ = p.originalSettings.moInputMode != 0;
if (simControl300Shown_) l3Slot(g_loaderSlotsL3.formingSimControl300Open)(p, op);
```

`moInputMode` is not stored in the retained A55 `.moproj`. It represents an application setting (`Setting/MoInputMode`). The candidate C ABI samples the adapter-only `M2M_MO_INPUT_MODE` environment variable; unset/empty returns `OriginalSettings{}` whose `moInputMode` is 0. The ordinary C++ `convert` overload also defaults to mode 0. Thus the CP59 folder-sweep candidate enters the mode-0 editor branch unless its caller explicitly supplies a nonzero setting. The historical original's fresh-process setting value is not in the project fixture or the saved KEY pair. This global setting is the first missing fresh-session input to bind.

There is a second, later candidate gap. If mode is explicitly nonzero, `formingSimControl300Open` enters `ConversionSession::beginEditor300Factory` and currently stops at `not_ported(0x6504abb0, "QtGui QWidgetPrivate palette inputs for SfPreSimControl300")`. That precedes widget setup, auto-connect, initial page-0 iteration fill, and the CG callback. A mode override alone therefore cannot reproduce the original branch today. The original fresh capture must also record which operation is selected when the fill runs; the retained A55 pair identifies Task00002 as the changed output but does not establish that Task00002 was the active editor at that exact point in a fresh launch.

The model-level `simcontrol300::normalizeCgAtOpen` and `fillIteration3D` encode the already qualified narrow callback rule, but the legacy unbound-manager projection is not evidence that the session-owned public path reached it. Do not compensate by unconditional normalization in the reader or writer.

## Smallest post-gate original comparison

After a verifiable app-control release is available, use the immutable pre-interaction input `evidence/gui_mo/oracle_b/RS_55_Multiple_Operations_Example1_before/`. First hash its complete tree and Task00002 SimCtrl file. Under a short disposable root such as `C:\PROBLEM\a55r\run\` (project path below 100 characters), make a fresh owned original-process Open → default dialog responses → Save → Close capture. Record the original executable identity and read-only `Setting/MoInputMode` value used by that process. At the first Forming object-tree fill, record the selected operation, its `SC+0x2e4` solver value, `SC+0x2e8` parameter count, `SC+0x2f0..0x308` values before fill, whether the constructor/page-0 iteration fill runs, whether checked=true is delivered to method 64, and the model values immediately after Open. Preserve raw output tree, `SimCtrl.KEY`, stdout/diagnostics, and hashes; do not normalize the historical bytes.

The discriminating outcomes are:

1. **Mode 0 or editor100 path:** no `SfPreSimControl300` CG toggle is expected from this chain. If the fresh Save keeps `[0,0,0,0]`, the historical `[0,1,0,1]` depends on a different setting/session/action and is not fresh Open–Save expectation.
2. **Nonzero mode, active Task00002, successful page fill, true-toggle delivered:** expect `[0,1,0,1]` immediately after the fill and in the saved line, with Task00001 unchanged. This would confirm the candidate's missing public editor300 construction/fill route as the relevant defect.
3. **Nonzero mode but Task00002 not selected, or no initial page fill/toggle:** retain the observed selection/order. Do not attribute the two bytes to this path without finding the actual event.

The candidate should be tested only after the original setting and path are recorded. Use the identical pristine tree and path for a serial candidate run with explicit `M2M_MO_INPUT_MODE` matching the original setting, preserving the candidate result before restoring the pristine copy. With today's CP59 source, mode 0 is the only branch expected to complete this part of conversion; nonzero mode is expected to stop at the palette guard, which itself should be recorded rather than treated as a failed parity comparison. If original mode is nonzero, finish the source-qualified palette/setup/auto-connect prefix before claiming A55 parity.

## Evidence and limit

Reviewed `analysis/wave2-save-diffs-diagnosis.json`, `analysis/wave2-save-diffs-original-slot-identity.json`, `analysis/wave2-save-diffs-original-iteration-fill.asm`, `analysis/wave2-save-diffs-original-meta-dispatch.asm`, `analysis/resume48-a55-cg-timing.md`, `source/apps/moproj2mst/src/ops/family.cpp`, `source/apps/moproj2mst/src/engine/loader_l3.cpp`, `source/apps/moproj2mst/src/engine/conversion_session.cpp`, `source/apps/moproj2mst/src/dll/abi.cpp`, `source/apps/moproj2mst/include/moproj2mst.h`, and the retained A55 before-tree KEY. The code/decompile evidence is static; no fresh original call trace or original global mode value was inspected. Current GUI status remains `release_unverified`/`release_ready=false` per the status tool, so this comparison is conditional.

Read-only audit only. No source, build, fixture, original application, GUI control, or mutable REA state was changed or run.
