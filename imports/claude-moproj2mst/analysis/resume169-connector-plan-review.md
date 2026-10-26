# CP169 connector independent plan review

## Decision

**Conditionally source-ready after CP168 is implemented from a stable source snapshot and its acceptance gates pass.** The selected QtCore cut `[0x1311bf, 0x1311c3)` is exactly the four-byte `mov [rbx+0x50], rdi` instruction described in the plan. Given the qualified predecessor's `RDI = 0` and unchanged `RBX`, it records one known 64-bit zero in the same retained QObjectPrivate object. Stopping at `0x1311c3` excludes the independent `+0x58` store. This is a narrow normal-path field observation and does not establish reference, container, backlink, QObject completion, exceptional behavior, or release effects.

The plan correctly identifies the missing prerequisite: CP168 has a source seam in the observed checkout, but no source-stable accepted build, focused tests, checkpoint regression, or original comparison is evidenced. Therefore CP169 must remain conditional until those CP168 gates pass; the available static qualification is not implementation acceptance.

## Owner, phase, and unknown representation

The proposed `std::optional<std::uint64_t> private50`, initially disengaged, matches the sparse observation model: allocated native bytes stay unknown until this store is admitted. Keeping it inside the retained pending Report's `SparseQObjectPrivate` preserves ownership continuity. The required entry state is coherent with the predecessor: exact `BeforePrivateZero50` phase / RVA `0x1311bf`, same pending Report and private identity, native sizes and address points, prior `private40 == 0` and `private48 == 0`, and `private50` still disengaged. The completion and backlink guards prevent this partial constructor cut from being applied to an already-completed object.

The root guard is appropriately repeated at the mutation boundary. It checks the factory's current `WidgetRuntimeState` against the allocation store's associated pointee/control block, then verifies pointer-identical qualified QList/QString records and committed markers. Requiring both committed deltas as existing evidence while refusing to replay increments preserves the ownership invariant without changing counts. The specific plan asks that `ownsWidgetRuntime` be used; implementation should ensure this checks both control-block ownership and pointee identity, as the prior review says.

Admission should finish all validation before mutation. Success should set only `private50` to zero and advance exactly once to `BeforePrivateZero58` / RVA `0x1311c3`. Reentry or malformed owner, phase, identity, binding, marker, predecessor, or completion state should leave observations, phase, deltas, mappings, counts, and flags unchanged. The plan's focused checks cover these invariants, including aliasing-foreign ownership and the actual `ConversionSession` pairing. No change to the plan's requested unknown status for `+0x58` and later fields is warranted.

## Instruction qualification and limits

The byte sequence and boundary stated in the plan are internally consistent with its predecessor note: CP168's store at `0x1311bb` ends exactly at `0x1311bf`, followed by `48 89 7b 50`; the next store begins at `0x1311c3`. The predecessor evidence also supplies the register and private-object continuity. These claims remain qualified to the cited local QtCore image hash and displayed normal-path instruction sequence. The plan appropriately avoids promoting the unwind-region display into proof of every logical or exceptional path, or inferring a live occurrence or native destructor semantics.

## Acceptance guidance

The acceptance sequence is appropriately ordered: first establish a source-stable accepted CP168 frame; then prove a single CP169 transition and unchanged captured owner/root identities, deltas, counts, and completion flags; then exercise reentry and refusal cases; finally run the focused factory/root-owner tests and the established checkpoint build/regression and original-comparison gates. Include the exact `private50` disengaged precondition in positive and refusal assertions so unknown bytes cannot be mistaken for zero.

**Conclusion:** no material contract gap found for this one selected store. The only blocker to unconditional readiness is the explicitly missing CP168 source-stable gate result. This review performed no tests, builds, comparison, GUI control, or live REA work.
