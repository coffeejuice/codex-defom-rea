# CP155: owner-bound recovery-tail design review

No blocker was found to CP154's bounded **private lifecycle** recovery-table tail. Native list self-reinstallation is source-closed, and the PPV payload fields already exist in the typed model. The implementation must resolve actual owners at the reached tail, preserve source partial effects and keep actual public mode300 Save guarded. The proposed lifecycle overload is a useful connected private composition; it is not production Session Save routing.

Only this report was written. No source/test/CMake edit, build/test, GUI/original execution or REA operation occurred. Review used current source, retained recovery decompile and bounded DUMPBIN/immutable-PE views. Original DEF_GUI_MO_64 v11.0.2 SHA-256 was independently verified as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected.

## Native records and setters agree

`RecoveryEntry` is a semantic model, not a native byte overlay. Read fields by name: type at native+0, signed `ppvSecondInt` at+4, `ppvFirstDouble` at+8 and `ppvSecondDouble` at+10. `value` represents KEY+18 and is unused by this tail. `simcontrol300_io.cpp::fromPpvPoint` actually supplies all four PPV fields; `readPpvForOperation` appends them to real root A/local B holders. The old nonempty-refusal comment claiming those fields are absent is stale. Correct that comment without weakening the legacy arbitrary-vector adapter.

Independent disassembly confirms the owner setters:

- `14005a3a0` resolves active subproject/data root, then calls065590 if present. `065590` is a plain pointer store to root+19a8; it does not delete/ref/copy list entries.
- `140f34b60` checks existing OPI+20. Its pointer-equal branch at4b84/88 skips the deletion call and joins the store at4bac. Reinstalling the actual current B is safe within this represented path; replacement-list deletion is outside it.

Retain the actual shared holders for the synchronous operation and reinstall those same holders if represented. Do not rebuild lists, clone/reparse entries, clear flags, deduplicate rows or route through a replacement setter. A and B aliasing is not inherently permission to deduplicate: native A-before-B loops would visit an aliased list twice. A candidate may explicitly exclude aliasing, but must not silently change the loop semantics.

## Accepted order and candidate admission

The complete retained140ed5dd0 body establishes:

1. Resolve current model/SC and actual A/B. Free positive-count NRECID storage, then set NRECID.n0=0/ncomp=0. ERECID is untouched here.
2. Count A then B, reinstall A/B, allocate positive node count with ncomp2, then copy type0 rows A-before-B.
3. Allocate positive element count with ncomp2, then copy exactly type1 rows A-before-B. Native counts every nonzero type as element but copies only type1: excluding other types is a sound candidate admission, not a repair.
4. Invoke Advanced Apply **again**, then resize outputs22/3f. A local Advanced ValidationStopped still reaches both resizes.

Axis conversion is signed int32→double, exact for the full int32 range. Copy both payload doubles directly; no g6 text formatting, KEY fallback, sorting or arithmetic on their values. At minimum test signed zero and distinguishable fractional payloads with exact stored bits/values. If a bounded profile excludes nonfinite payloads, state that explicitly; the native copy itself does not validate them.

CP154's upfront candidate guards are appropriate: 3D/current SC/step, advancedApplyProfile, actual ordinary root, nonnull holders, only types0/1, signed32 count and2*count bounds plus a stated resource cap, ordinary B2/B1 owned1D shapes with inactive axes/counts empty and no borrowed-axis marker/u68. Preflight must occur before this tail's frees/stores. It need not claim the excluded native profiles' partial effects. Ordinary owned means valid independent allocator-backed storage; shape alone cannot prove an arbitrary pointer's allocation length. Reject detectable axis/value or cross-table aliasing if it could make the free/reallocation path unsafe, or explicitly limit admission to valid production-owned tables.

Exclude type4 **before** calling recoveryListA: that accessor can lazily create its separate root. For the admitted ordinary active subproject, require its project backlink to equal Operation.project, its actual Project recoveryA holder and operation.recoveryB, and Operation.engineModel/current model/SC/step identity. Do not treat any two borrowed vectors as proof of these owners. Actual PPV producer tests should also retain unchanged list/entry contents and KEY values.

`dynary_alloc` already frees old arrays before clamp/calloc; do not add a convenience ERECID clear. Node-only rows must retain old ERECID pointer/count/contents; element-only rows must leave NRECID0/0 after its initial clear. Allocation-null admission must retain reached metadata and any completed preceding table, refuse before writes through null, and make no native OOM/unwind equivalence claim. Do not inject a fake failure facility or rollback source-prefix effects.

## Lifecycle and failure contracts to pin

The root overload should validate actual Operation/model admission before any earlier page/model/file effect. However, acquire/retain actual A/B at the **reached recovery-tail point**, as native does after Extra3D. Do not retain stale entry-time snapshots across arbitrary Extra3DIo callbacks. Recheck current SC/step/model and current list-holder identity there; if a callback changed admission, refuse at that tail while preserving earlier pages/I/O. If the implementation instead freezes owner inputs at entry, it needs an explicit no-mutation/no-reentry contract and must not claim the broader native late-resolution behavior.

Preserve the existing lifecycle aggregation: ValidationStopped records page-local outcome and later pages/tail run; UnsupportedSnapshot stops at its reached candidate boundary. Earlier ValidationStopped remains report outcome even if the repeated Advanced occurrence completes. Conversely, a later unsupported tail takes precedence without erasing prior mutations. Keep both Advanced invocations: the earlier one occurs before Extra3D; the second after recovery tables are rebuilt. Do not cache its result or suppress the second remap on equality.

The existing `resizeRecoveryOutputs` deliberately uses TIMEID.n0 for node width, not NRECID. ERECID.n0 supplies element width; all nonnull objects, including kind7, are visited only for changed widths. Tests should use actual object-owned state arrays to observe this, and verify resizes after Invalid time/geometry fields leave recovery tables committed. No global transaction/rollback is justified.

Current StagedEditor300 has no call to `applyInitialEditorPageState`; FormingClass retains140eb5180 Save refusal and actual constructor remains palette-blocked. InitialApplyReport explicitly has `completeForPublicSave=false`. Root's new owner-bound overload connects this tail to the explicit private page/Extra3D composition only. Do not label its positive regression public Session/C ABI Save, or bypass6504abb0/140eb5180 to make it reachable. Outer rename/tree/view/cleanup completion remains separate.

## File split and decisive tests

The proposed split is coherent and disjoint:

- Worker: existing `src/engine/simcontrol300.cpp` and NEW `tests/mdt/test_simcontrol300_recovery_tail.cpp`. New bound function; legacy nonempty-vector refusal retained.
- Root: tail declaration in `include/m2m/engine/simcontrol300_io.hpp`, explicit lifecycle overload in lifecycle.hpp/.cpp, and its existing lifecycle test. Root coordinates CMake/build and any routing changes; worker does not edit these shared files.

Before integration, agree the exact signature and whether the root overload passes only Operation& or an explicit owner binding; avoid duplicated ownership preflight with different criteria. Existing unbound interfaces must retain behavior. The focused worker target is `m2m_mdt_simcontrol300_recovery_tail`; the lifecycle target remains the existing one.

Required acceptance cases are CP154's mixed actual PPV A/B rows, node-only ERECID retention, element-only node clear, endpoint IDs/raw payload copies, empty bound lists, negative owner/type/count/shape profiles before tail effects, and repeated-Advanced ValidationStopped/resizes. Add the lifecycle callback mutation case if arbitrary Extra3DIo is admitted: its tail must resolve current holders or reject changed identity, rather than silently apply stale vectors. Retain the old populated-arbitrary-vector lifecycle refusal regression. No tests were run by this review.

## Evidence binding

CP154 proposal SHA-256: `f07e679da36dc19b8c03f949ba907f9ea9780aa07d5592335830c50ca00680fe`. Recovery decompile SHA: `52cc5cfeed98ac339e3cfeb91b2741df1b492862ee13376408ce883328dbbb9c`.

Independently recomputed half-open EXE byte hashes all match CP154:

| Range | SHA-256 |
| --- | --- |
|140ed5dd0..140ed6450|`29df5765664c85e3e45526299b44f919552546574393a4654040c2ff36244d69`|
|140f34b60..140f34bb5|`4bf249c4355de9eb2bd8d504941b1659be29be692d6805abd755a84cddec7db0`|
|14005a3a0..14005a3f2|`9e440357a44c3a66c1a9024c58bf772d6fbe51321366d2028a92774df5e2c7ad`|
|140065590..1400655ac|`6962566d0baa2e2161e9b7278edbda32957c513c50ea48899a8c5c0554370954`|
|14089b840..14089b95f|`10cb06f0ffd758fb0609b57bdc07447dd7ab5a6e9e5530b72fa24bb3e76c4fd8`|

Read-time source hashes under source/apps/moproj2mst/: model.hpp `baf657f01683196d78a45cbb180273cfa8d10b57c0523110171fe4fe235e078d`; accessors_p01.cpp `36a9c55a3127170d1027de6c57052434e71685121dae784c1f63bf74b8533213`; simcontrol300.cpp `5029294067d02890a01726d518664dbb9cb65504ee54cc3c4ead46fdeade30d0`; lifecycle.cpp `245fe59467e0852f6a34c540a975dcf5fe7e564922f80cad3a00e6cfdb3d6854`; io.cpp `7cccbd614ebda370eae85f313a4cb1f321be080dba827ce8976bb318d68dec69`. These bind a design review, not a future patch review or passing test receipt.
