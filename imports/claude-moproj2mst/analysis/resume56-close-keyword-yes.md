# Close Keyword-question Yes target and accepted-Close order

## Static target trace

The Close guard contract binds the main-window vtable used by `0x140044110` to table `0x1420fb108`. For the alternate Keyword-question Yes call at `0x14004479a`, virtual offset `+0x3a0` resolves to `0x140029300` (slot evidence and target bytes are listed in `analysis/wave3-close-save-return-contract.json`; bounded assembly is `analysis/wave3-closure-next-140029300.asm`). This is static evidence for the target address, not a semantic name recovered from a complete owner/caller audit.

The bounded target body shows:

1. It reads current application process type using `0x1402cb7c0`.
2. On the true branch, it calls `0x14001d590` with its receiver/context, then if the returned pointer is nonnull calls `0x140052ec0` on that pointer.
3. It then calls `0x1400cc50a0` with the receiver/context and returns.

The retained Save contract identifies `0x140052ec0` as current-operation Save, but the current Close audit does not qualify the `0x14001d590` object selection or `0x1400cc50a0` helper's complete effects/result. Therefore the safe description is “selected current-operation Save followed by unresolved helper on the process-type branch,” not “Keyword file successfully saved.” The call path needs its own source closure before any stronger claim.

## Answer and accepted-Close ordering

The guard's Keyword question uses Yes `0x4000`, No `0x10000`, Cancel `0x400000`, with `NoButton` requested as explicit default. The retained static shipped-style/initial-focus contract derives Yes as default. The guard contract records that Keyword Yes calls virtual `+0x3a0` at `0x14004479a`; it does not replace the answer with that method's return. Immediately after the call, assembly `0x1400447ac` tests whether the answer is Cancel. Thus the answer remains Yes and the guard proceeds into its accepted-Close tail. The selected fresh `.moproj` current-operation question is a different branch; it is not this Keyword path.

For any final answer other than Cancel, including Keyword-question Yes and No, static order is:

1. invoke main-window virtual `+0x288` at `0x1400447e1`;
2. invoke the recovered main-window `+0x778` route and, if its pointer is nonnull, call `0x140d0e540` with `(0,0,true)`;
3. emit `signalRemoveGraphItems` through the notifier;
4. return true from the model guard, after which outer Close can continue toward teardown/detach.

The `d0e540` state/display contract traces selected displayed-model StateVar updates, notifications and eligible named-model work, but it explicitly leaves complete render/legend/notification closure pending. Do not describe this accepted tail as a no-op. Outer teardown ordering is a separate Close contract.

## Modal and path qualifications

Both Close question variants use a modal event loop. The event loop may dispatch eligible events before the answer, but retained static timer evidence supplies no due-set/order for this Keyword question. No original GUI capture verifies that the default Yes was actually selected, that a timer callback occurred, or the `0x140cc50a0` effects. The full `0x3a0` target trace is bounded to the retained function region, not a complete transitive callgraph.

This Keyword branch is not part of the successful native `.moproj` current-operation Save default path unless reachability evidence changes. Original-only default scope excludes manually selecting Keyword Yes for routine acceptance; the static branch is retained because it is reachable under the alternate process/project condition in the guard.

## Smallest safe test seam

Keep the isolated answer helper test limited to the already resolved branch: injected normal-question result followed by accepted-Close decision order, with a sentinel callback for virtual `+0x288`/removal ordering only if callback semantics are explicitly abstracted. Do not use the helper to assert effects for virtual `+0x3a0`, and do not wire a “Keyword Yes” test to `0x140052ec0` plus an invented successful save result. A future isolated target test needs a qualified unit for `0x14001d590` selection and `0x1400cc50a0` status/effects before it can assert more than call order.

No original runtime, GUI, build, source edit, or mutable REA operation was performed.
