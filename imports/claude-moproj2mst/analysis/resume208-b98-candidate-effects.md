# CP208 B98 candidate effect audit

## Scope and evidence

This is a read-only audit of the frozen CP201 candidate run recorded by CP206. It compares the before/after manifests for `C:\PROBLEM\b98cp201\project`, the checked-in B98 before/after oracle snapshots, and current converter source. It does not establish original application parity.

The copied input project began with 27 files. Its `Gear_blank.moproj` was 10,932 bytes, SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`. The post-run manifest records 29 files: no removals; `Gear_blank.moproj` changed to 11,229 bytes, SHA-256 `dddc325a6d6eb34c1625a904eb1bc055a6403c450c312bbbe1703bb26af5f9da5`; and two additions. The first differing byte in the .moproj is offset 2,117 (zero-based); the files share a 2,075-byte suffix. This suffix/offset is an alignment summary, not a claim that only one contiguous range changed.

Added files:

| Path | Bytes | SHA-256 |
|---|---:|---|
| `Gear_blank.KEY` | 2,291 | `82565a02650afa84774ea45eccea1a32b64ed113b405720b43a7b34e967b5290` |
| `Operations\Task00001\DieStress.Wzd` | 64 | `eca2ebca2ef161fb4dd382cf7665b1a2c7e2fe68795e10a1303192f5bffe65d4` |

The explicit MST output was 2,718 bytes, SHA-256 `94ea8a00d2f860def960ca3f6c110a72d045d35229ac691cfa81d5cb50ef3b35`. The historical oracle comparison summary instead reports a 2,756-byte MST; that is a separate size/result and must not be conflated with this CP201 output.

## .moproj byte/content changes

The tracked human-readable oracle diff (`evidence/gui_mo/oracle_b/98_B_DS3F_diff/Gear_blank.moproj.diff`) gives these element-level changes. CP206's candidate post-run hash and byte count are for the disposable run; the recorded diff is the historical oracle before/after comparison, so the audit treats the diff as a description of the corresponding changes, not proof that every output byte matches the oracle-after file.

- In the first operation, `<Objects Number="3">` with three inline object records and compressed `ObjPropertiesKey` blobs became `<Objects Number="0"/>`.
- The same operation's empty `<Properties Number="0"/>` became two property groups: `SfOprDiestress` (equipment type/spec and `StepNumber=-1`) and `TestGroup` (`NumSteps=100`, `StepIncr=10`, temperature/convection defaults, step mode and distance).
- The `Sub_Project` view state changed: `CenterX 719.409 → 1650.92`, `CenterY 126.955 → 128.154`; quaternion `(-0.104126,-0.458291,-0.878926,0.0813343) → (-0.191342,-0.46194,-0.800103,0.331414)`; bounds Left/Right/Top/Bottom `(-445.927,445.927,445.927,-445.927) → (0,0,0,0)`; and Origin Z `-103.505 → 0` (X/Y stay zero).

These are the exact semantic XML changes represented by the available diff. The source fixture and candidate output lengths/hashes establish that the disposable file changed; they do not establish byte-for-byte equality with the historical after snapshot.

## Stage attribution from source

The public `--write-save` path in `src/core/convert.cpp` calls `openProject` first, then `activateSubProject`, then `openActiveOperation`. Open-created item blobs and open effects are applied before `saveCurrent`; the full Save then applies Save step 2, writes the .moproj (step 3), performs property writers (step 4), and emits the MST (step 5). The sequence and flags are also documented in `include/m2m/convert.hpp`.

- **`Gear_blank.KEY`: Save stage.** The B98 operation is DieStress. `src/engine/save_l6.cpp` implements `diestressSaveKey`, documenting it as `SfOprDiestress vt+0xe8` and deriving the project/sub-project DB-base `.KEY`; `src/engine/l6_slots.cpp` installs it as the DieStress Save-files callback. This is invoked from the operation's save flow, not by `openProject`.
- **`DieStress.Wzd`: Save stage.** `src/engine/save_l6.cpp` labels the property `vt+0xb8` writer as step 4, after the .moproj write. It writes `DieStress.Wzd` under the operation task directory. This is not the DieStress BM reader or `DieStressBmWizard.Wzd`; B98's file is the plain DieStress wizard. (There is a separate Open-time reader in `src/ops/diestress.cpp` for loading that wizard.)
- **MST: Save/conversion output stage.** `src/core/convert.cpp` reaches the step-5 MST writer after the open, step-2 operation save, step-3 project write, and step-4 property dispatch. It is not an Open-stage file effect.
- **.moproj: Save step 3.** `src/moproj/read.cpp::openProject` reads the input and populates the project model; it does not write the .moproj. With `M2M_FLAG_WRITE_SAVE`, `src/core/convert.cpp` invokes the native project writer after Save step 2. Thus the observed .moproj rewrite belongs to Save, though its serialized values reflect model/view state established during Open and subsequent processing.

The source ordering supports stage attribution for the converter implementation. The output manifests alone cannot distinguish which individual fields were first changed in memory during Open versus Save; the Save writer serializes the state at step 3. Nor does this imply that the original GUI completes Open or Save for this fixture: the separately captured original run crashed during `postOpen` before Open returned.

## Conclusion / limits

For this frozen converter invocation, the generated `Gear_blank.KEY`, `DieStress.Wzd`, and MST are Save-stage products (steps 2, 4, and 5 respectively); the .moproj rewrite is step 3. None should be labeled an Open-stage disk write. The B98 original capture is reported to crash before Open returns, so this successful converter run remains a lifecycle discrepancy candidate. No original-parity claim follows from this audit.
