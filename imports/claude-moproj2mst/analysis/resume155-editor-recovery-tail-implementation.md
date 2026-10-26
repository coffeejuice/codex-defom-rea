# CP155: owner-bound nonempty recovery-table Save tail

Implemented `ApplyResult applyOwnedRecoveryTail3D(SimControl&, GuiModel&, const AdvancedPage&, Operation&)` in the existing editor page source. It resolves actual operation/root recovery holders at the reached tail, rebuilds bounded node/element tables in native order, then repeats Advanced Apply and resizes output arrays even after its ValidationStopped return. The existing explicit empty-vector adapter remains unchanged in behavior. Source/test are frozen and released to root.

Only assigned simcontrol300.cpp, NEW test_simcontrol300_recovery_tail.cpp and this receipt were written. Root owns shared declarations, lifecycle overload/routing and integration tests. No compiler/build/CTest, GUI/original execution, mutable REA, public mode300 or CMake change occurred in this lane.

## Evidence and actual ownership

Controlling report `analysis/resume154-editor300-next-cut.md` SHA-256 `f07e679da36dc19b8c03f949ba907f9ea9780aa07d5592335830c50ca00680fe` qualifies complete native140ed5dd0..140ed6450 SHA `29df5765664c85e3e45526299b44f919552546574393a4654040c2ff36244d69` against original DEF_GUI_MO_64 v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Actual RecoveryEntry already represents native+0/+4/+8/+10 using type/ppvSecondInt/ppvFirstDouble/ppvSecondDouble. KEY value at+18 is not consumed by this tail.

The API requires a live OPI generation, the actual operation.engineModel/control/step and Project, and an existing active ordinary subproject/data root. It excludes type4 lazy root creation. It obtains and retains the actual Project.recoveryA and operation.recoveryB shared holders, checks recoveryListA resolves that same root, then rechecks generation/model/current holder pointers and control blocks before effects. The caller must invoke it at the reached tail after Extra3D; it does not snapshot A/B during entry validation or substitute copied lists. Same actual A/B list aliases are supported and counted twice, matching two native occurrences.

Native root/local setters reinstall the same pointers. The OPI setter's same-pointer branch skips free atf34b88 before storing atf34bac; no candidate list replacement, deletion, fake native owner or extra persistent list was introduced. Captured shared owners retain candidate typed storage only and do not claim Qt list/BC allocator lifetime parity.

## Bounded profile and ordered effects

Candidate admission requires existing advancedApplyProfile, only type0/1 records, at most4096 combined records, and ordinary owned1D B2/B1 NRECID/ERECID metadata. Positive old tables require component2 and distinct live axis/value pointers with signed32-safe counts; zero-count tables require null arrays and component0/1/2. Inactive dimensions/axes, variable0 and u68 are bounded; borrowed markers, malformed shapes and opaque nested owners refuse before any clear. The record cap is a candidate resource limit, not an original format maximum.

On admission, positive old NRECID arrays are freed, then its n0 and ncomp become0 in source order5e87/5e91/5ea0. ERECID is not cleared here. Positive node count sets n0/ncomp2, allocates, and writes type0 A rows then B rows. Positive element count follows independently for type1 rows. Axis uses signed integer+4 converted exactly to double; each value pair copies+8/+10 without numeric formatting or KEY-value substitution. Zero element count leaves old ERECID storage/count/data unchanged.

Actual allocation availability is checked before array dereference. An unavailable allocation retains reached metadata/partial allocation state and returns UnsupportedSnapshot; no fake success, original OOM/error completion or failure-hook behavior is asserted. Normal model/profile conditions remain the authority for this private value projection, not arbitrary caller pointer extents.

After completed tables, existing applyAdvanced3D executes again at the native63ea position. Existing resizeRecoveryOutputs follows unconditionally, including after ValidationStopped. It retains the qualified TIMEID-derived node width and ERECID-derived element width. No speculative correction to NRECID width or global Save-abort interpretation was added. Native Qt/validator/allocation/EH/FP status and the full outer editor lifecycle remain outside this increment; public palette/completeness guards are untouched.

## Focused tests and static preflight

NEW target `m2m_mdt_simcontrol300_recovery_tail`, CTest `m2m.mdt.simcontrol300_recovery_tail`, has ten Release-active Testkit cases:

1. Actual Project/SubProject/Operation/GuiModel plus production readPpvForOperation populate shared A and local B; mixed rows rebuild both tables A-before-B, preserve current list/control-block identities and contents, and ignore distinguishable KEY values.
2. Node-only rows include signed32 ID endpoints and retain actual existing ERECID allocation/count/payload.
3. Real A/B holder aliasing counts both occurrences without deduplication or persistent row duplication.
4. Quiet-NaN payload and negative-zero bits supplied in actual typed record fields are copied without formatting/normalization.
5. Element-only and empty owner-bound branches clear only NRECID and retain ERECID when element count is0.
6. An earlier separate Advanced page call is followed by a distinct tail reapply value, preventing accidental deduplication.
7. Completed recovery tables survive a local Advanced validation stop; actual object-owned node/element arrays grow from an explicit current width0 profile. TIMEID width1 differs from NRECID count2, while ERECID width2 is retained.
8. Missing Project/holders, foreign model, type4 lazy-root path and explicitly declared expired OPI refuse before table clearing.
9. Unknown record type, resource cap, wrong table type, actual borrowed-marker shape and actual nested owner refuse without pointer/data mutation; owned memory is restored appropriately for test cleanup.
10. Legacy vector-only nonempty refusal retains old table state; the actual owner-bound entry then succeeds.

Static preflight passed source whitespace/diff checks, test newline/no-NUL checks, actual quoted/defmodel includes, ten unique Testkit cases/one main and no C assert/REQUIRE_EQ. Source checks confirm owner/profile checks before free, node clear before rebuild, node rebuild before element rebuild, Advanced before resize and no entry.value read. Exact signature matches root's shared declaration. No C++ test execution or build occurred; root owns focused/full validation and archive binding.

## Frozen release hashes

| Owned file | SHA-256 |
| --- | --- |
|source/apps/moproj2mst/src/engine/simcontrol300.cpp|`8a62d68c7c209d0713e11c509b2968c317b7a52339e272837fd7619f2de340f9`|
|source/apps/moproj2mst/tests/mdt/test_simcontrol300_recovery_tail.cpp|`5642d540fdac01a5b1707437d7227d9e43e531d4d5b9ab430259c6d484643dde`|

All owned source/test files are frozen and released. No additional source ownership is held.
