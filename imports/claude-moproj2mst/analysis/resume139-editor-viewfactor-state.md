# CP139: bound private view-factor stateChanged

The bound private editor now consumes ordinary delivered view-factor checkbox state changes. The event changes only the projected definition-button enabled value. Actual VFACTR mode, curve and definition effects remain in the existing later Environment Apply path, including its partial stores before a missing-context refusal.

Assigned source/test files are frozen and released to root. Only owner.cpp, the new test, evidence proposal and this receipt were written; root supplied the shared declaration. No worker compiler/build/CTest, GUI, original execution, REA database, executable or deployment change occurred.

## Exact source qualification

Frozen proposal `analysis/resume139-editor-next.md` SHA-256 `085e8e677aefd64b3bbe33f69cd21b92c1c3848c7e793a32fef3741e30d31d41`. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Slot-map index66 reaches140ed0820 from dispatch1410dbd2d/call1410dbd40. Complete native `[140ed0820,140ed0871)` SHA-256 is `e054a1e246c227966b09bc8bccdee08c5679b41b7b79f55f6b55e057ad82f91a`. It tests state0, then calls QWidget::setEnabled on UI+da0 with false at140ed0849 or true at140ed0866, returning atd0870. There is no SC/model/dimension/curve access, checkbox write, allocation/dialog or file effect. Retained full assembly SHA `2be9286de7b5c1bdf883cd6d377bb9bfb84ae3925d2db75994c37e7902aa0f10`; decompile SHA `cc427084d30e7699c4674936797de168e32196300bc1798282a9ae4d1e896247`.

Existing Fill sets checkbox and definition-button projection from model mode!=0. Existing Apply first processes Environment curves/scalars, writes mode0/1 from checkbox, then on checked3D copies the retained curve/definition. Missing context stops after mode1 and earlier stores; unchecked skips curve/dimension/definition work. This increment changes none of that logic.

## Implementation and scope

`StagedEditor300::viewFactorStateChanged(binding,state)` admits ordinary states0/2, same live Project/OPI generation/model/remesh binding, completed private page metadata/source-model/remesh tokens and cached viewFactorChecked matching state!=0. The caller represents the changed checkbox before declaring event delivery. State1 and other ints remain a candidate exclusion, although the native body enables for any nonzero argument; no native rejection is claimed.

The method assigns only viewFactorDefinitionEnabled and returns Completed. It never calls pageState/simCtl, checks dimension, consumes sourceControl or any view-factor curve payload, invokes Apply, changes cached checkbox or current-curve index, mutates OPI/model/remesh, or performs I/O. Retained completed controls therefore still consume the callback when their same live model's actual step or curve payload is absent/replaced. Wrong identity/state/cache/partial owner refuses before projection.

This is an explicit private value event, not actual QWidget disabled/hidden bits, native signal scheduling/reentrancy, definition-dialog delivery or constructor completion. Repeated explicit delivery reasserts the requested argument. The public palette/6504abb0 and completeForPublicSave=false remain unchanged. No new Apply, dimension, curve or partial-state admission is enabled.

## Focused target and static preflight

Target `m2m_mdt_simcontrol300_viewfactor_state`; CTest `m2m.mdt.simcontrol300_viewfactor_state`. Eight Release-active Testkit cases cover:

1. Ordinary0/2 delivery projects only the button, retaining already changed checkbox, live mode/value/definition, actual retained curve identity/definition/current index, unsaved invalid text, other page/OPI and I/O counters.
2. Cache assignment alone does not deliver; repeated same-state occurrence reasserts button enablement.
3. Missing/replaced actual step or absent retained curve does not add a nonexistent SC/dimension/payload prerequisite or mutate old/replacement model data.
4. Uncheck leaves live mode/data unchanged until existing Apply; Apply writes mode0 while preserving then-current live curve/definition, rather than copying the old snapshot.
5. Check leaves live mode/data unchanged until Apply; passing the actual retained session curve/definition and current actual model dimension then copies them.
6. Check followed by missing Apply context returns UnsupportedSnapshot after scalar EHRATE and mode1 stores, with old live VFACTR curve/definition untouched. No fake dialog value is supplied and no rollback occurs.
7. Excluded state-1/1/3, mismatched checked-cache delivery and wrong Project binding refuse before projection.
8. Unconstructed/partial owner and explicitly declared OPI identity boundary cannot advance the public factory. The identity fixture does not claim native destruction.

Static preflight verified eight cases, one Testkit main, no C assert/REQUIRE_EQ, quoted/testkit/defmodel includes at actual include roots, exact shared signature and DB_STEP_DATA(3). CP138 frozen-source comparison shows only the new14-line method in owner.cpp. Existing CONFIGURE_DEPENDS test glob supplies the target; no CMake/lifecycle/shared-page or Apply implementation edit was made. No runtime test result is claimed. Root owns focused/full required validation and source/binary archive binding.

## Frozen release hashes

| File | SHA-256 |
| --- | --- |
| source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp | `dbb1962a5524c2e958789c374af1cb8baf9f132dd1a44af2ee19d0c59187d918` |
| source/apps/moproj2mst/tests/mdt/test_simcontrol300_viewfactor_state.cpp | `1268166b646caa5cee3a02a2bdc57cb4a3ad8e2b815399acaee6c3601ce5de2b` |

Files are held through root validation/archive freeze. Partial-state delivery, actual definition-dialog/function return, TIMEID/DB-step tail, operation-name/tree refresh and selected public palette history remain independent unrecovered boundaries.
