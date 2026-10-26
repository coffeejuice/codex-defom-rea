# CP154: next substantial private editor300 Save cut

The largest source-ready increment found is the **nonempty recovery-table Save tail140ed5dd0**, under an actual operation/list-owner binding and bounded ordinary table profile. Current applyEmptyRecoveryTail still refuses every nonempty list on the stated basis that RecoveryEntry lacks native+4/+8/+10. That basis is stale: those semantics are already represented by ppvSecondInt/ppvFirstDouble/ppvSecondDouble, and the actual PPV reader populates them. The current private composed Save reaches this exact refusal after all page and Extra3D effects; its lifecycle regression explicitly proves that route. This is a connected Save-state improvement, not another control-only event or an orphan helper.

Only this report was written. No source/test edits, build/test run, GUI/original execution or mutable REA occurred. Scope is the existing private typed-model/value projection; it does not claim completion of the native outer editor lifecycle.

## Reconciliation of earlier closures

- CP130/131 already implement stateful meshing activation/Save, same-row reload, previous-temporary partial commit, disabled remesh and masked nonfinite node conversion. Reimplementing those is not a new increment.
- Environment table ownership, constant/table radio projection, ACVCOF callbacks, TIMEID display, view-factor state/deferred Apply and constructor backup/restore already exist. The old blanket environment/enable audit is no longer the current boundary.
- CP137/138 frequency edit/index callbacks and CP140 explicit visibility are implemented. CP141/142 added bounded solver6 Fill/Apply, including four known texts and partial convergence writes. Their earlier blanket solver6 refusal is closed within its explicit profile.
- Advanced zero-constant/zero-table movement paths and signed integer frequency profiles are already admitted. Nonzero movement evaluation remains a real unresolved model effect, not a missing numeric widget adapter.
- PPV Open/Save is connected through loader.cpp and the actual operation/root list holders. RecoveryEntry's semantic four-field projection and reader binding predate the current stale recovery-tail comment; no new model.hpp/layout migration is needed.

## Complete native recovery-tail order

Retained `wave2-save-diffs-300-support-140ed5dd0.c` is complete. Bounded immutable-PE inspection corroborates the following order:

1. Resolve native current model/SC and root A/local B recovery pointers. If NRECID.n0>0, free its arrays at5e87. Store NRECID.n0=0 at5e91 and ncomp=0 at5ea0. Do not clear ERECID at this initial point.
2. Count A then B: type0 counts as a node row, every nonzero type counts toward the native element allocation count. Reinstall the same A and B pointers through05a3a0 at5f8b andf34b60 at5fa5.
3. For positive node count, set NRECID.n0/count and ncomp2 at5fc6/5fd5, allocate at5fe7, then append type0 rows from A followed by B. Axis value is signed BC+4 converted to double; the two values are the raw BC+8/+10 doubles. BC+18/RecoveryEntry.value is not used.
4. For positive element count, set ERECID.n0/count and ncomp2 at61da/61e9, allocate at61fb, then append exactly type1 rows A-before-B with the same field mapping. If there are no element rows, retain old ERECID arrays/count/values. Do not clear it as a convenience.
5. Call Advanced Apply again at63ea, then resize state outputs for IDs0x22 and0x3f at6418/6446. The tail calls resize even if Advanced returns early on its own validation; do not deduplicate the earlier page's Advanced call or treat its ValidationStopped as global Save abort.

The node/element asymmetry and ordering are material. In particular, unknown type2 is counted as an element but not copied by the exact-type1 loop. The first bounded cut should exclude types other than0/1 rather than silently repair this behavior.

### Actual owner edge is source-closed for self-reinstallation

The owner setters do not require inventing a replacement list.05a3a0 resolves the active data root then065590 stores the supplied pointer at root+19a8. OPI setterf34b60 checks current+20 against the supplied B pointer atf34b84/88; the equal-pointer branch skips its free and stores the same pointer atf34bac. For the actual A/B just obtained from those owners, this is self-reinstallation, not deletion/refcount reconstruction. Keep both candidate shared owners alive for the synchronous operation and leave their identity/contents unchanged.

## Recommended bounded integration, not arbitrary list admission

Add an owner-bound private entry, for example:

```cpp
ApplyResult applyOwnedRecoveryTail3D(SimControl&, GuiModel&,
    const AdvancedPage&, Operation&);
```

Require the actual live Operation.project, its same engineModel/control/step, and an existing ordinary active data root. Resolve actual A via recoveryListA(operation) and B via operation.recoveryB; retain their real shared owners. The first profile can exclude type4's lazy separate-root creation and refuse null/mismatched holders before effects. Do not accept unrelated vectors as proof of native owner self-reinstallation. Existing explicit empty-list adapter can remain for its legacy fixture contract.

Admit only type0/1 records with all represented fields supplied by the typed state; exercise positive tests using actual readPpvForOperation production, not fabricated hidden bytes. Bound combined count and2*count to signed32 and a stated candidate resource cap. Require standard owned1D B2/B1 NRECID/ERECID shapes (normal var0, inactive dimensions/axes empty, u68 null, consistent existing positive-count allocations and no zero-count borrowed-axis marker). Keep existing advancedApplyProfile/actual model admission. These are candidate guards before effects, not native validation.

Use existing dynary_free_arrays/dynary_alloc and source-ordered writes, preserving initial node-clear and zero-element retention. Allocation success is a prerequisite for filling actual arrays; if an actual allocation is unavailable, retain its reached metadata/partial state and refuse before dereference without claiming original OOM behavior. Do not add a fake allocation success or broad failure hook. No new global, native field, recovery list owner, or full Qt list model is needed.

Integrate this entry through a separate owner-bound private lifecycle overload (or an equivalently explicit root-approved binding), retaining the current empty-vector interface unchanged. The owner-bound overload must validate Operation/model identity before earlier page effects and then execute the same page/IO order, selecting this tail only at the current tail point. This avoids silently widening existing arbitrary-vector callers and preserves the old refusal regression as a legacy admission test. An actual borrowed Operation reference plus retained actual A/B holders is sufficient; no persistent frame or public Session admission is proposed.

## Exact source reservation and tests

Worker reservation after root release:

- Existing `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: new bounded owner-bound tail, retaining the empty adapter and existing Advanced/resize functions.
- NEW `source/apps/moproj2mst/tests/mdt/test_simcontrol300_recovery_tail.cpp`: focused actual PPV/list-owner and typed-model cases. Existing test glob supplies target `m2m_mdt_simcontrol300_recovery_tail`, CTest `m2m.mdt.simcontrol300_recovery_tail`.

Root-coordinated shared reservation:

- `include/m2m/engine/simcontrol300_io.hpp` (the current tail declaration is here, not simcontrol300.hpp): declare the bound entry.
- `include/m2m/engine/simcontrol300_lifecycle.hpp` and `src/engine/simcontrol300_lifecycle.cpp`: explicit operation-owner overload/binding and tail call; unchanged old interface.
- Existing `tests/mdt/test_simcontrol300_lifecycle.cpp`: add one bound nonempty positive route after earlier page/Extra3D effects; retain its current arbitrary-vector populated refusal test unless its contract is explicitly replaced.

Required focused assertions:

1. Real Project/SubProject/Operation/GuiModel owners; populate A and B through readPpvForOperation (shared mode then local mode). Mixed0/1 rows produce exact NRECID/ERECID axes and paired values, A-before-B within each table, unchanged list identities/contents/KEY values and no doubled PPV mutation.
2. Node-only rows clear/rebuild NRECID while retaining a distinguishable preexisting ERECID allocation/value/count. Element-only rows clear NRECID to0/0 and rebuild ERECID. Empty bound inputs retain the existing empty-tail outcome.
3. Existing positive NRECID storage is replaced in the original order; both new tables have independent owned allocations. Signed integer endpoints convert exactly to double. The two payload doubles are copied, not formatted/rounded or sourced from KEY value.
4. Later Invalid Advanced time/geometry field retains completed recovery tables and still executes existing resizeRecoveryOutputs; use actual object-owned state arrays to observe element width changes. Preserve the existing node-width source TIMEID.n0, not an invented NRECID width correction.
5. Wrong Operation/model/root ownership, null holder, unsupported record type, count/shape/borrowed-axis guards refuse before this tail's frees/stores. Legacy vector-only path remains guarded for nonempty rows.
6. Full private page composition reaches this new tail after all earlier pages/Extra3D, including the repeated Advanced occurrence and its existing ValidationStopped semantics. Keep all completeness qualifications.

## Other candidates and why they rank lower

Normal type80 StepSize tables are also a source-ready smaller alternative: admit owned1D/ncomp1 tables with var0=1 or2, positive bounded n0, only axis0/values, no u68/borrowed marker. Native89b4d0 maps those definitions to1/2;89b450 writes ndim1 and the same var, so the Fill callback is idempotent. Current StepSizePage already owns a deep curve and Apply copies it only after its scalar-validation tail, restoring the reached mode byte. This can remove the constant-only guard without a definition dialog. It needs focused table Fill/backed-refresh/Apply/partial-validation tests, but closes less behavior than the recovery Save tail.

Nonzero movement-kind6 Advanced time evaluation still calls unported874630 after892370; no shared model implementation of that effect was found. Nonfinite meshing abs/CRT branches still need their FP/error contract. Definition dialogs, operation-name/tree refresh and arbitrary numeric-history expansion lack a complete new owner/effect closure. They should not be disguised as large ready changes merely by weakening a guard.

## Evidence and current-source binding

Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Bounded DUMPBIN14.44.35228.0 views and direct section-mapped PE hashes were used; no original code ran.

| End-exclusive native range | SHA-256 |
| --- | --- |
|140ed5dd0..140ed6450 full recovery tail|`29df5765664c85e3e45526299b44f919552546574393a4654040c2ff36244d69`|
|140f34b60..140f34bb5 OPI self-set/free guard|`4bf249c4355de9eb2bd8d504941b1659be29be692d6805abd755a84cddec7db0`|
|14005a3a0..14005a3f2 root setter wrapper|`9e440357a44c3a66c1a9024c58bf772d6fbe51321366d2028a92774df5e2c7ad`|
|140065590..1400655ac root slot store|`6962566d0baa2e2161e9b7278edbda32957c513c50ea48899a8c5c0554370954`|
|14089b840..14089b95f existing GUI allocator|`10cb06f0ffd758fb0609b57bdc07447dd7ab5a6e9e5530b72fa24bb3e76c4fd8`|
|14089b450..14089b4c1 StepSize definition setter|`d0d6036532cb76c8539f643c6548382347ebf7324718ccc5218ab2231e199811`|
|14089b4d0..14089b528 definition getter|`36724836d304046460c3a0cc41057524f6f3802a65a3cedcc4dc05681c240e70`|

Current relative source hashes: simcontrol300.cpp `5029294067d02890a01726d518664dbb9cb65504ee54cc3c4ead46fdeade30d0`; lifecycle.cpp `245fe59467e0852f6a34c540a975dcf5fe7e564922f80cad3a00e6cfdb3d6854`; model.hpp `baf657f01683196d78a45cbb180273cfa8d10b57c0523110171fe4fe235e078d`; simcontrol300_io.cpp `7cccbd614ebda370eae85f313a4cb1f321be080dba827ce8976bb318d68dec69`; lifecycle test `c74a62d1c1ee6b1d3a0b840b0993ee6882addb70b8e230960bd5657ccccb0661`. Retained recovery decompile SHA `52cc5cfeed98ac339e3cfeb91b2741df1b492862ee13376408ce883328dbbb9c`.

No implementation reservation is held by this report. Root can assign the bounded owner-bound recovery bundle for the next grouped build.
