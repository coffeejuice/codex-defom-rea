# CP141 inherited non-Report property/XML next-step audit

## Finding

No additional source-closed production helper is justified by the remaining non-Report RINHERIT candidates. Several formerly live omissions are already integrated at bounded callsites. The nearest unresolved candidate is CopyMirror's property-to-engine mutation: its original Open loader has no recovered `vt+0x60` call, while its Save-current property dispatch is reachable, but no retained evidence proves whether the fields written by that dispatch affect a supported downstream consumer. Do not insert the mutation into Open or change the writer based on the old “not ported” comment alone.

## Queue reconciliation

| Candidate | Current evidence/status | Decision |
|---|---|---|
| Convert2Dto3D `Properties vt+0x60` | `resume46-copy2d-property-crosswalk.md` binds the original `0x1401c3a60` load path to `0x1401ce590`; candidate `c23Load` applies properties in both admitted branches. The writer-local lack of a second application is not a lifecycle omission. | Settled for the bounded load profile; retain unrelated geometry/second-model guards. |
| CopyMirror `Properties vt+0x60` | Original `0x140e24b80` Open body has no identified call to `0x140e2d230`; `+0x58` property parsing is distinct from reverse application. Candidate `cmLoad`/`cmPostLoad` likewise do not call it. The generic Save-current dispatcher does invoke CopyMirror `propsToEngine`, which writes conversion kind/axis/dimensions to non-kind-7 objects. `resume46-copymirror-first-consumer.md` leaves field-sensitive downstream output unknown. | No Open-time fix supported. Save-time call exists; impact beyond model mutation remains unknown. |
| Boolean property Save side files | `resume88-inherited-public-effect.md` reconciles the earlier `resume71` ordering gap: `convert.cpp` invokes the source-qualified Boolean property slot during step 4; the separate MST-time call remains. Existing failure-order tests and CP84 receipt cover the earlier side effects. | Settled for ordinary bound Boolean properties; keep the separate duplicate SideBytes recording-mode question open. |
| ShapeOptim property copy | `resume51-shapeoptim-property-copy.md` maps `0x140142d00` to the two class fields and current Save-current handles the false-editor-update branch before later slots. | Settled for that branch; do not infer editor/GUI completion. |
| SimCtrl.PPV | The reader/writer is integrated behind the operation path with unsupported profile/owner guards retained. The public malformed-row regression requested by `resume79` now exists as `public_ppv_malformed_second_row_keeps_first_row_through_save` in `tests/mdt/test_ppv_public_lifecycle.cpp` (around line 447). | This old proposed test increment is no longer pending. Remaining codec/owner profiles stay explicitly guarded. |
| Type2 Constraint XML | CP124 implements the source-qualified ordered Constraint-row append for the admitted positive-zero records and tests partial writer ordering. It still stops at the actual DOE table-refresh owner. | Settled only for this serializer prefix; not broad XML or public active-DOE completion. |
| XML internal DTD entities | `src/xml/dom.cpp` explicitly refuses internal DTD entity declarations/general entity references; `resume50-xml-property-omission.md` found no checked-in project requiring them. | Known supported-grammar boundary, not a corpus-triggered omission established by current evidence. |
| `Group.Expanded` | The imported path writes `yes`, but the native value is reported uninitialized; no defined native value contract is available. | Preserve as a qualified nondeterministic deviation. Do not synthesize a deterministic helper. |

The separate populated DOE property-table case is not an XML parser gap: CP141's `resume141-property-table-populated-audit.md` shows that finalization consumes existing checkable cells, but does not establish the same manager's item creation or connected callback. Its exact missing owner is the source-qualified population/signal path for the same `SfObjectManager` and eligible View row. Keep its populated-cell refusal.

## Recommended next action

The smallest discriminating action is a bounded **static** CopyMirror consumer query, not a source edit:

1. Confirm the original caller set for properties virtual `0x140e2d230`, distinguishing Save-current from editor-only paths and the absent Open call.
2. Starting at writer `0x140e24e80`, inspect only immediate model/object serialization callees for reads of the fields written by `0x140e2d230` (`+0x2be8`, `+0x2bf0/+0x2bf8`, `+0x2c10/+0x2c18`, `+0x2c38`). Record branch conditions and first field-sensitive output/effect, or state that none is present in that bounded closure.

If the query finds a reachable field consumer, the next source task can be a focused nondefault-property regression for that exact effect; it still must preserve the original Save-current ordering. If it finds no consumer in the bounded writer closure, that closes only direct MST-byte sensitivity, not other Save effects. Until then, do not call this an Open omission, declare it harmless, or change the source.

**Status:** several narrow inherited handlers are settled; CopyMirror consumer impact and the definedness of `Group.Expanded` remain unknown; DOE populated-table creation/callback ownership remains unbound. No production helper/source fix is selected from the evidence. Read-only source/receipt review; no build, GUI, original run, or mutable REA action.
