# Checkpoint 62 graph ownership boundary

## Evidence inspected

The immutable original PE `evidence/original-binaries/DEF_GUI_MO_64.exe` was rehashed before the follow-up: SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (67,611,136 bytes). This pass used retained DUMPBIN disassembly only. It did not query or alter REA, open a GUI, edit source, or build.

I followed the alternative factory path already identified in the retained constructor listings. `c36bd0` calls the concrete `SfGraphHistogramItem` constructor `c2b6b0` on two conditional branches (`c36d9d`, `c36e55`). The constructor's RAX is staged in stack locals and copied into the factory's common return local; `c36bd0` then returns it at `c3711f..c3712d`. The paired factory method `c36b80` loads the factory vtable, dispatches slot `+0x58`, and returns that call's RAX at `c36bb1..c36bc2`. Bounded pointer-flow excerpts are in `analysis/resume62-histogram-pointer-flow.asm`, filtered from retained captures `resume50-histogram-construction-function.asm` and `resume51-factory-virtual-call.asm`.

As an alternative scene edge, retained PE-byte search had previously enumerated 132 calls through the `QGraphicsScene::addItem` import. Calls in the graph-adjacent `0x140c2...` region include `c2c560` and `c30fd7`; the snippets show scene insertion of an item subobject, but the available call snippets do not bind those receivers to `SfGraphHistogramItem` RTTI or to the return value from `c15910`/`c36b80`. They are therefore not ownership evidence for this graph instance. No caller to `c15910` was found in the complete `.text` searches documented in `resume60-graph-owner-xref.md`.

## What the pointer flow establishes

The histogram constructor's pointer is propagated as a factory result. This is consistent with a creator/factory API but says nothing about who adopts the pointer. The excerpt ends at the `c36b80` virtual return; there is no resolved caller of `c15910` to observe a later store, scene insertion, guarded-list insertion, or destruction transfer. Constructor-side `QGraphicsScene::addItem` is absent from the bounded histogram constructor evidence (`resume49-graph-ownership.md`).

The broader addItem import hits cannot be assigned to this histogram object from adjacency alone. The known cleanup path (`c0c650`/`c15db0`) and manager guard evidence also do not establish native ownership by the scene. Thus the current evidence still cannot choose between native scene ownership and an external owner with scene membership, and does not validate `unique_ptr` for `PostGraphSceneState::items_`.

## Qualified stopping point and smallest next query

No new owner edge is recoverable from the retained static callsites. The unresolved link is specifically the consumer of `c15910`'s returned RAX (or another concrete graph factory result) after virtual dispatch. The next bounded action must be a root-owned static-analysis indirect/data xref for `c15910` and the relevant factory slot, then capture a caller through its first RAX use. If no such reference exists, retain the uncertainty; do not infer scene ownership from unrelated `addItem` calls or expand the source model.
