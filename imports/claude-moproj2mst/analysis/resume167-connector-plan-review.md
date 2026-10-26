# CP167 connector independent plan review

## Decision

**Conditionally source-ready after the CP166 predecessor is accepted.** The selected QtCore cut `[0x1311b5, 0x1311bb)` is exact: `xor edi,edi` clears RDI, and the following store writes eight zero bytes to the retained QObjectPrivate field at `+0x40`. Stopping before `0x1311bb` correctly excludes the independent `+0x48` write. Model this as a newly known optional 64-bit field observation, not as a default zero in host state.

The inspected checkout has since advanced beyond the plan's stated CP166 snapshot: `initializeNativePrefix` now handles `BeforeQStringSharedNull`, validates the same-root QList and QString records, commits one symbolic QString increment, and stops at `BeforePrivateZeroFields`. This is source evidence only; this review did not run the required focused tests or checkpoint build/regression gates. CP167 should proceed only after those predecessor gates pass.

## Instruction and boundary

The plan's byte interpretation and retained-RBX relationship are internally consistent with the cited static disassembly. The six-byte slice contains no field write before the zeroing store and excludes the next store. This supports a single normal-path field observation. It does not establish other fields, full constructor completion, exceptional paths, native unwind/release, or a live occurrence. The plan appropriately avoids inferring any of those.

## Owner, guard, and representation

Keep the observation on `PendingTaskPrototype::SparseQObjectPrivate`, which is retained by the pending Report. A suitable representation is `std::optional<std::uint64_t>` for private `+0x40`, initially disengaged. Disengaged means unknown allocated bytes; explicitly setting it to `uint64_t{0}` records this instruction's effect. Do not initialize it to zero, use an unqualified scalar default, or add a factory-local shadow field.

The existing CP166 seam already has the important ownership pieces: the pending Report and private allocation are retained by the `TaskAllocationStore`, the store is paired with its `WidgetRuntimeState`, and admission compares the retained QList binding with that exact root while checking the QString record on the same root. Preserve those checks. For CP167, additionally require the exact `BeforePrivateZeroFields` phase, unchanged pending/private identity, Report kind and allocation sizes, final private address point `0x1a9d10`, task address point, false QObject completion and unavailable backlinks, valid committed QList and QString bindings/markers, and already committed symbolic deltas. Require `+0x40` to be absent before entry. An expired, unassociated, foreign, mismatched, malformed, or aliasing-foreign root must refuse without changing the field, phase, bindings, or deltas.

On success, publish only `+0x40 = 0` and advance once to an explicit phase at `0x1311bb` (for example `BeforePrivateZero48`). Preserve the same owner and all other unknown observations. A replay from that phase must not recommit the field or any prior symbol delta. No host register model is needed: the instruction establishes zero directly.

## Acceptance coverage and conclusion

The plan's positive and refusal cases are appropriate. Verify one advance preserves pending/private/root identities and both prior deltas, makes only `+0x40` known zero, leaves `+0x48/+0x50/+0x58` unknown, and reaches the next boundary. Check same-key and different-key reentry after each refusal; inject missing/expired/foreign and aliasing roots, bad or uncommitted bindings, wrong address point/phase, and a prematurely observed `+0x40`. Include the actual `ConversionSession` pairing. Refusals must preserve the retained state and avoid prototype/task/cache growth.

Keep focused factory and root-owner tests plus the source-stable checkpoint build/regression and original comparison gates as acceptance requirements. No tests or builds were run for this review. The plan correctly does not invent an initial value for `+0x40` and does not claim absolute Qt refcounts, container identities/counts, backlinks, task state, or native release from this store.
