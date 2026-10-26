# CP246 success-order repair independent review

## Verdict: PASS, within the stated success-only boundary

The proposal isolates the known source-order defect without claiming to solve the native add/copy failure contract. Its split places both ID assignments after the existing first-KEY/add/name/path prefix and before the final KEY path lookup and serialization. That matches the retained successful original sequence while preserving current `equipmentSync` behavior.

## Source and ordering check

Current `formingPostLoad` calls `createEquipment`, then assigns `slot->equipmentId`, then `op.equipmentId`. Current `createEquipment` performs first serialization, `addEquipment`, movement-name copy, first manager-path lookup, and conditional final serialization/write before returning. `equipmentSync` is a second live caller of `createEquipment`; keeping it on the wrapper that executes prefix then final append preserves its current ordering and behavior.

The retained Forming decompile (`d0.c`, `0x140c06130`; same sequence in the Forming2D twin) places the slot setter then operation setter before reacquiring the step and invoking the second writer. The cited hashes and v11.0.2/original-binary qualification are consistent with the prior CP246 review. They establish static decompiled order, not a fresh original runtime trace.

The proposed Forming call sequence—begin prefix; store slot ID; store operation ID; reacquire the step; query the manager path again; conditionally append the final KEY—puts the second serialization after both stores and does not add a new `simCtl`/object guard. It retains the converter's nonempty-path condition and explicitly labels it as converter behavior, not proof of original `-1` behavior. The alias/pointer scope is unchanged; this is not presented as the broader owner-command closure from the prior contract.

## Failure and compatibility limits

The document appropriately limits its exception-prefix statements to actual C++ exceptions and does not invent a throw seam between the two scalar stores. It correctly says no existing `FileEffects` seam can currently interrupt final serialization after the ID stores, so source-order inspection is the available evidence unless a real seam is found. Its proposed test checks the reachable successful recorded-effects route and does not claim that a final-state assertion proves ID-before-write order.

The explicit exclusions are material and correctly retained: `addEquipment` still reports success for any non-null byte string, cannot represent native copy failure/row suppression/`-1`, and current `equipmentBytes` differs from the native temporary-source ownership path. The success-order repair must not be used to claim failure-prefix fidelity. No transactional rollback or catch-and-repair is introduced.

## Test review

Extending the actual `loadOp`-reached `forming_without_equipment_creates_one` fixture to assert the selected PDIE slot ID, the single new row and its name/path, and the two ordered write entries with renamed movement bytes is a meaningful positive-path regression test. Existing/missing-object/missing-slot cases preserve the `objEntryN` lazy-append distinction. The report correctly qualifies Oracle-B 96 as historical final-output comparison only and prohibits treating it as intermediate-order or native-failure evidence.

One practical implementation check remains for the owning source change: ensure the two recorded writes are selected by their equipment destination rather than assuming `fx.writes` has no unrelated earlier entries in the fixture. This is a test precision detail, not a contract blocker.

No source, tests, build, shared state, original application, GUI-control session, or mutable REA state was changed or run.
