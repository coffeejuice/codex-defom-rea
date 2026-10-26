# Checkpoint 46 graph audit — ownership and mutation edges

## Finding

The unresolved contract is the populated post-graph manager collection: when the native `Close` removal slot selects a graph by its embedded name, the original removes that graph's guarded list entry before invoking its deleting destructor, and its numeric loop still advances after removal. A candidate that stores scene objects as raw pointers, deletes before unlinking, or uses an erase-all/corrected iterator loop changes observable lifetime and can skip the element that shifted into the removed index.

## Evidence boundary

The native identity for these statements is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Static contract `analysis/wave3-close-graph-manager-cleanup-contract.json` records the offline PE/DUMPBIN evidence and hashes for the bounded captures. It states that manager teardown deletes the owned factory before traversing its graph collection; the list nodes hold separately allocated QPointer wrappers; `take_at` unlinks a node but returns a still-guarded graph; and name-based removal releases that temporary guard before calling graph deleting virtual `+0x18`. The same contract records the post-removal index increment and warns against replacing it with erase-all or a no-skip loop. This is bounded static evidence, not populated-graph runtime validation; shared QList detach internals, populated graph destructor callbacks, and reentrant effects remain qualified.

Checkpoint45 (`analysis/wave3-checkpoint45-validation.json`) records 150 CTests passing, with six logged gaps and one logged difference, and explicitly says no original executable was launched and the result is not original parity. The graph manager cleanup contract is still only a supported projection for the empty private collection. In `source/apps/moproj2mst/src/engine/close_effects.cpp`, `PostGraphManagerState::~PostGraphManagerState` resets the factory then clears `graphItems_`, while the populated native list remains unavailable; `PostGraphSceneState::removeGraphItems` rejects non-text graph items because concrete classification/deletion is not recovered. The source therefore does not yet exercise the native `take_at`/guard/destructor sequence.

## Focused regression candidate

Add one focused `CloseRemoveGraphItems` characterization once an actual graph owner and guarded-list representation are available: populate a manager list in native order with three named graph items, make only the middle item's name match the scene-selected item, run the slot, and capture (1) list membership/order, (2) guarded-pointer validity at each step, (3) destructor invocation order/count, and (4) the post-removal numeric index behavior. Compare original and candidate in a fresh process with copied state. This distinguishes unlink-before-delete and the native index advance from plausible but incompatible ownership/erase-all implementations. Keep populated graph destructor/reentrancy effects as explicit separate qualifications until the original capture establishes them.

## Candidate46 consequence

Do not promote the current empty-list cleanup projection into support for populated graphs. First recover an actual graph creation/registration path and graph destructor effects, then bind those owners to the manager's guarded collection and add the focused original-first regression before widening `Close` acceptance.
