# CP166 connector independent plan review

## Decision

**Conditionally source-ready, after the CP165 predecessor passes its focused and checkpoint gates.** The selected cut `[QtCore RVA 0x1311a2, 0x1311b5)` is exact and models one coherent normal-path event: bind the retained private object's `+0x30` field to the exported QString shared-null identity at RVA `0x2f0b30`, then account for one symbolic `uint32_t` increment. The plan correctly stops before `0x1311b5` and does not promote that increment into an absolute count or QString value.

This is a static review. I did not edit source/tests/shared plan/state, build, control GUI, or use live REA.

## Native slice

The instruction bytes listed in the plan support the stated effects: `lea` at `0x1311a2` resolves the module-relative shared-null address; `mov [RBX+0x30],RAX` stores it into the retained private object; and the locked add at `0x1311ad` adds one to its leading DWORD. The proposed range ends exactly at `xor edi,edi` (`0x1311b5`), so it excludes the following zero-field and container initialization. Keeping QtCore's SHA-256 and the export RVA as identity evidence, rather than persisting a preferred or loaded pointer, is appropriate.

The effect is only established for the already selected non-null private-allocation path. The exception-directory extent and static disassembly do not independently establish all constructor paths, runtime occurrence, release behavior, or unwind behavior; the plan states those limits accurately.

## Owner and admission guard

The same-root requirement is essential and correctly stronger than matching the QtCore digest/RVA. CP165 binds the QList field through the allocation store's associated `WidgetRuntimeState`, and its source guard now checks `ownsWidgetRuntime(widgets)` before the first QList event and later address-point transition. CP166 should retain that ownership check and require the live private `+0x18` binding to match that exact expected root's qualified QList record, then require a distinct, qualified QString record owned by that same root. A factory-local QString record or the isolated `QStringPathOwner` is not sufficient evidence of the constructor's shared owner.

Current source has the QList owner and allocation-store association, but the inspected source does not yet provide the proposed root-owned QString shared-null record or its increment API. That is an implementation prerequisite, not a flaw in the plan. The API should make the identity stable for the lifetime of the root, qualify module digest plus RVA `0x2f0b30`, and apply only a modulo-`2^32` symbolic `+1`. The private field should hold a nonowning binding, and its committed marker/phase must prevent replay. Prepare host-side state before publishing the field; a refusal must leave the private field, both symbol deltas, and cursor unchanged.

Require the pending Report/private identity, exact `BeforeQStringSharedNull` phase, false QObject completion/no backlinks, address point `0x1a9d10`, committed valid QList binding, and allocation-store association to the supplied root. Verify the QList and QString identities against that one root, not merely by equal symbol metadata. Reentry, missing/expired/unassociated/foreign roots, and tampered CP165 state must refuse without a QString write or increment. Once advanced, the next explicit boundary should be before RVA `0x1311b5`.

## Acceptance coverage and count claims

The proposed focused checks cover the right state: one advance preserves pending/private/root identities, adds one QString symbolic delta, leaves the QList delta unchanged, and stops before `0x1311b5`; same-key and different-key reentry inspect the retained state after each refusal; damaged prerequisites and foreign/expired roots refuse without mutation. The two-frame case may demonstrate additive symbolic deltas only when both frames share the same actual root. It must not imply that the current native factory reaches a second prototype. Keep focused factory and root-owner tests plus the normal source-stable checkpoint build/regression and original comparison gates as acceptance prerequisites.

Do not report an absolute count, fresh-process baseline, string contents, or native release from this source event. No same-call live pre/post read of the actual QtCore DWORD is retained, and PE bytes or a test-supplied seed cannot establish it. The plan's explicit no-absolute-count statement is sound.

## Conclusion

The slice and requested symbolic model update are well bounded. Proceed only after CP165 is accepted and the root gains one canonical qualified QString shared-null identity with a guarded once-only increment. No test or build completion is asserted by this review.
