# CP135: bound private reaction-rate click

The bound private editor now consumes an explicit ACVCOF reaction-rate radio click after Open. It projects the existing Environment control values first, resolves the actual current simulation control, then updates the current temporary index and live/temporary variable codes in source order. Constant selection clears only the live curve's dimension; temporary dimensions and both tables' storage remain intact.

Source/test files are frozen and released to root. Only the assigned owner source, new focused test, compact evidence proposal and this receipt were written. Root added the shared enum/report/method declarations. No worker build/compiler/CTest, GUI, original execution, mutable REA operation, executable or deployment change occurred.

## Source evidence and distinction from existing Fill

Frozen compact proposal: `analysis/resume135-editor-next.md`, SHA-256 `b77800660a0d878b114be93174f981c560a680f94149317c8a899e0c5a33a608`. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Constructor clicked() connections at UI+e70/e80/e78/e88 reach wrappers140ecf7c0/7e0/810/840. Bounded immutable-PE DUMPBIN verifies constants0/1/2/3 passed to140eccf10; this is delivered clicked scope, not an inferred event when cached fields or programmatic setChecked change. Complete callback `[140eccf10,140ecd2e5)` SHA-256 is `9b53369cd493e1d3cad521a65293ab114f46edaa8dabc0e09a8b5c0b6834da38`.

The callback first selects radios and sets control enabled arguments. Current SC lookup is only at140ecd1b5; null skips subsequent index/data stores. Nonnull writes currentCurveIndex3 at140ecd1d1. Selection0 then writes live ndim0 atd211, live var0 atd220, temporary.var0 atd243. Choices1/2/3 write live var and temporary var3/12/1; they never set live ndim. Temporary ndim, data pointers/counts, constant text and all other curve fields are untouched. Exact retained assembly/decompile/wrapper hashes are in the compact proposal.

Existing `fillEnvironment3D` and its snapshot test matrix already project this behavior during Fill. They were not changed or recoded. This increment supplies the missing explicit post-Open transition through the actual bound private owner, without a Fill or Apply replay.

## Implemented ordering and guard

`StagedEditor300::reactionRateClicked(binding,selection)` accepts0..3 and validates live Project/OPI generation/model/remesh binding, completed private page metadata and source-model/remesh identity. The existing Environment temporary3 must be nonnull, distinct from the other temporaries and aliased by the page through the same pointer/control-block owner. These are candidate owner-admission checks, not new native validation.

The method projects `EnvironmentAcvcofControls {selection, selection==0, selection==0, selection!=0}` and commits ControlsProjected before querying simCtl. It does not call pageState early. Missing/replaced SC or a temporary alias into any live SC environment curve returns UnsupportedSnapshot at ControlsProjected, preserving those control values without index/data writes or stale-source dereference. An exception during source lookup propagates unchanged with prior control projection retained. The guarded path is not labeled native exception/null behavior.

With qualified current SC, it stores index3, live ndim0 only for selection0, live var and temporary var using `[0,3,12,1]`, then returns Complete/Completed/modelUpdated. No native memory allocation/free, curve-copy operation, numeric conversion, constant-text refresh, OPI mutation or file operation is introduced. Repeated declared clicks are processed again.

Controls here are the existing private value projection. No physical QWidget disabled/hidden bits, signal emissions, automatic event scheduling or reentrancy is certified. Actual Session construction remains behind the selected palette/6504abb0 guard and completeForPublicSave remains false; the new method refuses absent/partial private page state.

## Focused target and static preflight

Target `m2m_mdt_simcontrol300_reaction_rate`; CTest `m2m.mdt.simcontrol300_reaction_rate`. Eight Release-active Testkit cases cover:

1. All four delivered choices on actual bound owners with live and temporary DYNARY tables allocated by dynary_alloc. Verify exact variables, selection0-only live ndim, unchanged temporary ndim/counts/pointers/payload/metadata, unsaved invalid numeric text retained, owner identity, other page/OPI and I/O counters unchanged.
2. Sequence0→1→2→3→0 preserves live ndim0 after constant selection, temporary ndim1 and both retained tables.
3. Repeated same choice rewrites externally changed live/temp variable values and current index.
4. Missing/replaced actual model step retains control projection but no index/old/replacement/temp writes.
5. Invalid selection and wrong Project binding refuse before control projection.
6. Missing/replaced/aliased temporary/page owner refuses before projection.
7. A diagnostic shared lease aliasing the actual live curve is guarded after controls and before index/data stores, without creating a second deleting owner.
8. Unconstructed/partial owner and explicitly declared OPI identity boundary refuse; the fixture does not claim native destruction or public completion.

Static preflight verified eight cases, one Testkit main, no C assert/REQUIRE_EQ, all quoted/defmodel/testkit includes against their actual include roots, supported shared declarations and explicit DB_STEP_DATA(3). The comparison against CP134's frozen source.zip shows only array/dynary includes and the new method in owner.cpp. CMake's existing CONFIGURE_DEPENDS test glob discovers the new target. No runtime test result is claimed; root owns configuration, focused execution, full required regression and source/binary archive binding.

## Frozen release

| File | SHA-256 |
| --- | --- |
| source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp | `c36e0fcd801cf5c7d7534f046b308f74bcf520772c653b83e29d6a617bb1c9de` |
| source/apps/moproj2mst/tests/mdt/test_simcontrol300_reaction_rate.cpp | `6c7107ad558895d30aaefaa72ce349fb29a40373aac2c061162d46cbabb3d82a` |

All assigned source is held for root integration and archive freeze. The next dependency is the focused/root validation group. Physical control ownership, selected constructor palette history and the operation-name/tree tail remain separate unrecovered boundaries.
