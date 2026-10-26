# Checkpoint 63 graph adoption xref follow-up

## Immutable target and bounded xref checks

The retained original `evidence/original-binaries/DEF_GUI_MO_64.exe` has SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and size 67,611,136 bytes. I used read-only PE-byte searches and existing bounded DUMPBIN captures; there was no REA session, GUI, source edit, or build.

The `SfPstGraphFactory` primary vftable is at `0x14284d0c8`; slot `+0x60` is at `0x14284d128` and points to `c36b80`, while slot `+0x58` points to graph-constructing `c36bd0` (bindings already captured in `resume53-graph-consumer.md` and `resume56-graph-consumer-static.md`). A complete-image search for the literal 64-bit slot address `0x14284d128` returned no occurrences. A `.text` search for common RIP-relative LEA/MOV/indirect-memory forms resolving exactly to the vtable start found two references, at VAs `0x140c36b36` and `0x140c36b5e`. Their bounded disassembly (`resume63-graph-adoption-xref.asm`) shows two setup references: `c36b00` installs the derived vftable, and `c36b50` restores the base vftable on the destructor path. Neither is a consumer callsite.

The previous complete `.text` scan for direct `E8`/`E9` references to manager wrapper `c15910` found none; the complete-image absolute-VA search also found none. The caller-side indirect/data edge to `c15910` is still absent. The wrapper itself is only manager+`0x18` → factory virtual `+0x60` → return-RAX unchanged, as bounded in the new excerpt.

## Scene-insertion cross-check

The available `QGraphicsScene::addItem` import-call sites are not enough to identify adoption of this histogram result. The previously enumerated graph-adjacent candidates (`c2c560`, `c30fd7`) do not have a retained dataflow binding to the result of `c15910` or `c36b80`, nor to `SfGraphHistogramItem` RTTI. Their proximity to other graph code is not a pointer identity proof. The histogram constructor body and factory body remain without a direct `addItem` call in the retained ranges (`resume49-graph-ownership.md`, `resume50-graph-scene-caller.md`).

## Result and remaining edge

This follow-up confirms the only recovered RIP-relative references to the factory vftable start are constructor/destructor setup, and it finds no literal slot-address xref from code/data. It does not identify a caller dispatching the manager wrapper or a scene owner. Consequently, native scene ownership versus external ownership with scene membership is still unresolved; the source `PostGraphSceneState::items_` representation remains unvalidated by this evidence.

The remaining one-hop query is an indirect caller/data xref for `c15910` (or an alternate exact xref from the concrete `c36b80` slot dispatch) in root-owned static analysis. If that query yields a caller, capture that function only through its first post-return pointer use. If it yields none, keep the ownership edge unresolved; do not promote unrelated addItem sites into proof.

## Search limits

The vtable-start scan covered common x64 RIP-relative LEA/MOV/indirect memory operand encodings in the full `.text` interval documented in `resume56-graph-consumer-static.md`; the slot check was a complete-image exact 8-byte search. These checks do not rule out computed address arithmetic, alternate instruction encodings, unresolved indirect control flow, dynamic dispatch, or references from another module. The manager-wrapper negative search has the direct/absolute/common-form limits listed in `resume60-graph-owner-xref.md`.



