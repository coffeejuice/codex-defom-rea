# CP170 connector independent plan review

## Decision

**Conditionally source-ready after CP169 is source-stable and its focused, checkpoint, and original-comparison gates pass.** The selected QtCore instruction cut `[0x1311c3, 0x1311c7)` is the single `48 89 7b 58` store: it writes the already-zero RDI to private `+0x58`, with RBX retaining the private pointer from the qualified initializer path. Ending at `0x1311c7` excludes the subsequent QVector shared-null address load. This establishes only one sparse field observation and cursor advance on the selected normal path.

The plan accurately identifies the outstanding prerequisite. At review time the header/source contain CP169's `private50`, `BeforePrivateZero58` entry and refusal boundary, but no `private58` or `BeforeQVectorSharedNull` implementation. The factory test file still checks CP169's stop. Thus the current checkout is not accepted CP169 evidence; static qualification cannot replace that gate.

## Contract assessment

`std::optional<std::uint64_t> private58`, disengaged initially, is the appropriate representation: allocated bytes remain unknown until the instruction cut is admitted. Keeping it in the same retained Report's private state preserves owner continuity. The required `private40`, `private48`, and `private50` zero observations plus the distinct `BeforePrivateZero58` cursor form a precise predecessor contract.

The repeated root guard is appropriate: require the factory's live runtime root to be owned by the allocation store by both control block and pointee identity, then require pointer-identical qualified QList and QString bindings and committed markers. Those markers are prior committed observations; CP170 must not replay their reference deltas or demand absolute counts. All validation should precede either field or phase mutation. On success only `private58` and phase change. On refusal or reentry, preserve observations, identities, deltas, counts, maps, and completion/backlink flags.

The plan's scope limits are sound. It leaves `+0x68`, `+0x70`, QVector identity/reference effects, later containers, backlinks, QObject completion, and cache effects unknown. The included positive `ConversionSession` owner-pairing case and negative aliasing-foreign case are useful safeguards. Focused refusal cases should also verify the CP169 predecessor's `private50 == 0` rather than merely phase.

## Qualification and gates

The cited assembly/static qualification records the local QtCore hash and marks the disassembly as an unwind region. The bytes and cut are consistent with the preceding CP169 store and later-vector-load boundary. This supports the selected instruction interpretation but does not prove every logical or exceptional path, a live occurrence, or native cleanup semantics.

Acceptance should follow the plan's order: obtain a source-stable accepted CP169 frame; demonstrate one CP170 continuation with unchanged captured identities, root deltas, counts, maps, and flags; prove same-key/different-key reentry and malformed-root/binding/predecessor refusal are non-mutating; then run the established focused and checkpoint/original-comparison gates. No additional live witness is required to interpret this store, but those acceptance receipts are required before implementation is considered unconditionally ready.

**Conclusion:** no material contract gap found for this one store. The sole readiness blocker is the missing source-stable accepted CP169 prerequisite. This review ran no tests, build, comparison, GUI, or live REA operation.
