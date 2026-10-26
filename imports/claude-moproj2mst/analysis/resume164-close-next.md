# CP164 Close: next document owner witness

The smallest candidate Close slice is the existing `SubProject+0x30` Report guard-read prefix, but it is **already implemented only as a diagnostic boundary**. No further original Close, destructor, or reset effect is source-ready on the present owner graph. The next useful cut is to produce the actual Report document during Report Open, then preserve its current identity and payload through Save and the accepted Close rereads. A new detached null certificate or destructor helper would not advance the public lifecycle.

This is a read-only converter investigation. Only this report was written. I did not edit source, tests, shared plans/state, or original evidence; run a build or original application; acquire GUI/app control; or open a live REA/native analysis session. The facts below come from the retained static byte qualifications cited below and inspection of current candidate source. They establish conditional paths, not an observed fresh Open→Save→Close occurrence. No new REA Evidence ID or candidate freeze is claimed.

## Native chain and first unbound state

The retained original is `evidence/original-binaries/DEF_GUI_MO_64.exe`, **v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The EXE image base is `0x140000000`; the table gives both full VA and RVA. These are previously qualified static edges, with exact range hashes in CP160/161, not a fresh disassembly in this pass.

| Edge | VA (RVA) | Established conditional behavior |
| --- | --- | --- |
| Report document accessor | `0x1400b5ad0` (`0x000b5ad0`) | Constructs `SfSessionManager` via `0x140643100` (`0x00643100`) and publishes its return to the selected `SUB+0x30` after construction. |
| SubProject document cleanup | `0x1400b5bb0` (`0x000b5bb0`) | Reads `SUB+0x30` twice; a first null read returns. On the nonnull route, the second current read controls deleting virtual `+0x18` with flag 1, then `0x140cbc920` (`0x00cbc920`) clears the guard. A callback or intervening destruction can change the second read. |
| Document deleting target | `0x14064f060` (`0x0064f060`) | For an object retaining its constructor-installed table `0x1424e0098`, invokes ordinary destructor `0x1406432f0` (`0x006432f0`) before scalar free. The table/RTTI identify `SfSessionManager`. |
| Document payload cleanup | `0x140643400` (`0x00643400`) | Nulls the `+0x18` QString, then handles current guarded `+0x20`, raw `+0x30`, `+0x40` part list, and `+0x48` DOM owner before QObject base destruction. Their producer and callback effects cannot be skipped by assuming an empty document. |
| Report model use | `0x1412b0d08` (`0x012b0d08`) | A qualified Report path calls `0x140643540(document,true)` (`0x00643540`) on the document returned through the current SUB accessor; this can create guarded `SfItemModel+0x20`. |
| Interactive payload | `0x140645320` (`0x00645320`) | On current raw `+0x30` null, constructs/publishes `SfInteractivePost`, runs `0x1406453c0`, then rereads. The alternate `0x140646370` (`0x00646370`) may replace the slot on its nonnull-node branch. |
| DOM owner | `0x14064b890` (`0x0064b890`) | The document constructor's initialization allocates/publishes a `QDomDocument` at document `+0x48` before `SUB+0x30` publication. |

CP160 binds the conditional accepted Close chain through manager child deletion, Mo Project, SubProject, and this document slot. CP159/156 separately qualify command delivery and fresh busy/current-main/can-close/modal checks. **Inference:** the next implementation dependency is producer and lifetime state, not another search for the deleting virtual. None of these conditional byte paths proves that the same document remained live at a particular accepted Close.

## Current candidate boundary

`source/apps/moproj2mst/src/engine/report_document_slot.cpp` reads only a diagnostic `KnownNull`/`KnownNonNull` observation on the adapter `SubProject`. The nonnull branch stops before the deleting virtual. Its immutable initial observation cannot track a real subsequent publish, rebind, callback destruction, or guard clear. `source/apps/moproj2mst/src/ops/report.cpp` still refuses at the Report `vt+0x78` document accessor instead of constructing/adopting `D`; `source/apps/moproj2mst/src/engine/conversion_session.cpp` labels Session destruction as adapter cleanup rather than native normal Close. The source `SubProject` member is a slot projection, not a Report QObject owner. These are current source observations, not original behavior.

The public command has an earlier independent dependency: an actual application/main `M` with the same source-produced Session manager `G` in `M+0x50`, separately mutable current-main `W`, and its busy/phase/modal state at command delivery. The existing entry-prefix and empty-manager helpers are bounded diagnostics. The normal populated child cannot take the empty-manager tail. The private OperationInfoModelCache `+0x160` remains `Unknown` for its canonical Operation at the late cleanup read; the constructor's default null or a separate diagnostic instance does not establish its current value.

## Exact next cut

The first **bounded evidence cut** is the Report Open path from `0x1412ae070` (RVA `0x012ae070`) through its current SUB lookup and `0x1400b5ad0` publication, then the Report document's first admitted model/interactive/part/DOM producers. Record the actual `P`, `SUB S`, `S+0x30 D`, constructor-accepted QObject parent `H`, and every subsequent rebind or death through Save. For `D`, establish current `+0x20/+0x30/+0x40/+0x48` before attempting `0x140643400`; `+0x48` is ordinarily produced during construction. At `0x1400b5bc2` and `0x1400b5bd5` (RVAs `0x000b5bc2` and `0x000b5bd5`), bind both actual guard reads to that live or previously destroyed `D`, preserving intervening callbacks. If an earlier QObject parent deletes `D`, identify that occurrence and resulting guard invalidation. An isolated constructor/default value is insufficient.

The first **source cut after that witness** belongs at the Report document producer and its owning SubProject, before extending `attemptReportDocumentReleasePrefix`. Tests should start with same-`SUB` publication and current-state rereads, then cover nonnull destructor ordering and the first-null return separately. Public Close integration must wait for the independent `M/G/W` producer and can-close/modal/effect ordering; private `+0x160` cleanup needs its own canonical producer. A static trace can narrow these dependencies without GUI control. A same-instance runtime trace remains gated by the parent repository's supported GUI-control release requirement.

## Read receipts and limits

- CP160 `analysis/resume160-inherit-close-next.md`: SHA-256 `f80e719ca754ad29d55e75c08077949eef33a556ed46ec77668f320d3de1c99f`; provides exact PE range hashes and source qualifications for the SUB setter, document table, ordinary/deleting destructors, payload cleanup, and Qt base-destructor selection.
- CP161 `analysis/resume161-inherit-payload-next.md`: SHA-256 `e1d1e95bc8ffb7e2805c4aac016dec09f8fdfe695abfbfec199c3ca509a09959`; provides exact PE range hashes for the model, interactive, part, DOM producer/cleanup paths and their limits.
- CP159 `analysis/resume159-close-next.md`: SHA-256 `1427b9ef3535541a65689c50896a1658cc535d7958cbe7fb245e9098292b7076`; CP156 `analysis/resume156-close-delivery-admission.md`: SHA-256 `701922c1f56b8d8a0c679e00d5c894816ac0be5cac3cb8373bb285d99d3e202b`. These qualify delivery versus admission and canonical `+0x160` uncertainty.
- Current source reads: `source/apps/moproj2mst/src/engine/report_document_slot.cpp` SHA-256 `1f004f5f797fc0abeed34b0a90a0a58059c7763e9ea61576eaa0a7c4fb7251b4`; `source/apps/moproj2mst/src/ops/report.cpp` `e378d472550bbe9f6a109baa330f6612277328ac425853cf5e17ba316252b43a`; `source/apps/moproj2mst/src/engine/conversion_session.cpp` `679f4c2195a4e664709ec946a67253c177e3b3957ef36bf253dce53178eef9a5`.

The retained native findings are static; range coverage and unresolved callbacks remain as qualified in their source reports. CP101's older REA attempt timed out with zero retained query records, so it supplies no independent Close witness.
