# CP232 frequency-text implementation review

**PASS — no unresolved implementation or test defect found after the requested coverage repairs.** The extraction preserves the reviewed reconstructed-source contract. This is a static source/test review; root owns source-stable integration, the full suite and freeze.

Reviewed the CP231 next-leaf proposal, CP232 independent contract review and implementation report, CP137 qualifications, the bounded working diff/current bodies in `engine.hpp`, `gui_model.cpp`, `simcontrol300_owner.cpp`, the complete frequency-edit test and the supplemental parked-viewer assertion. Accumulated changes from prior checkpoints in shared files were distinguished from this leaf.

## Source behavior

The caller retains the original short-circuit binding/OPI lifetime, row, KnownC locale, constructor, complete-page, source model/remesh and populated-cache content checks. Row admission precedes cache indexing. It introduces no `pageState()` query, enabled/combo predicate, same-text shortcut or cache write. Boolean command success maps to Completed and false to UnsupportedSnapshot.

The command preserves the exact reviewed sequence:

1. Validate row, then the unchanged bounded text profile: admit zero length; otherwise require at most ten UTF-16 units, first ASCII digit 1–9, remaining units digits, and magnitude at most INT32_MAX. The ten-unit bound makes uint64 accumulation safe. Excluded spelling refuses before geometry/model access.
2. Call `requireGeometrySyncAvailable()` exactly once, without catching exceptions or adding `noexcept`.
3. Resolve the current nullable step/control; reject null or different expected address. Expected is only compared, never dereferenced. Both-null refuses, and guard precedence is preserved even when expected is null.
4. Read columns into stride, then retain short-circuit rows/stride/component shape checks in that order.
5. Call the existing `qt::toInt(deliveredText)` with its default null validity pointer.
6. Read the live values pointer only after conversion and refuse null without repair/allocation.
7. Store exactly one `static_cast<double>(converted)` at row times stride and return true.

Null and nonnull-empty strings retain content equality through `qt::String::operator==`; unengaged optional cache remains unknown and refuses. Both zero-length strings pass lexical admission and convert to integer zero, yielding positive double zero. Neither input nor cache is normalized. Positive admitted int32 values are exactly representable as double. The direct qt4 include resolves to the existing header; required limits/integer declarations are present.

## Tests and repair

Added direct tests meaningfully check INT32_MAX with unchanged other row bits, empty-to-positive-zero, mismatch and live-current/null-expected refusal, invalid rows/text, each shape failure, missing values storage, absent step with old expected token, and both-null refusal. Existing caller tests retain all six rows, null/empty cache nullness and positive-zero bits, preservation of unrelated raw bits, repeated explicit delivery, excluded spellings, locale/cache/binding refusal, missing/replaced control, table refusal, partial/expired OPI gates and no unrelated I/O effects. Owned table metadata is restored before fixture teardown. Testkit entry point/assertions and existing target discovery remain valid; no C `assert` or unsupported macro was added.

Review initially found two coverage omissions: direct live-current/null-expected and the proposed parked frequency-text assertion. Both are now present. At `test_viewer_box.cpp:2844`, the mutable parked model is called with valid row/text and null expected. The existing `refused` helper requires Error(M2M_E_UNSUPPORTED), so an ordinary false result cannot pass. This tests guard precedence over null-expected rejection when executed. Source order establishes invalid-input-before-barrier and other identity/shape ordering; the test does not dynamically cover every parked argument or the owner callback under a barrier. No provider/reentrant effects or instruction-level equivalence are claimed.

The implementation owner's reported focused build/1-of-1 test pass predates these test repairs. This reviewer ran no build or tests and does not certify that receipt for the revised source. Root must run the source-stable frequency_edit/viewer_box/stopping_cached group and complete its usual full-suite/freeze gates.

## Scope and identity

No C ABI, Stopping/cache1, coordinate query/publication or public Save behavior is changed by this extraction. `simcontrol300.cpp` and stopping-cached test have no working diff. The expected raw address token is not ownership closure, lifetime protection or ABA prevention. Native comparison retains CP137/CP232 contract qualifications; no fresh original execution or real widget/validator acceptance follows from this review.

Reviewed full-file SHA-256 values:

- `engine.hpp`: `58BA04CF72FF6DA7D59DEED6D058F1A1E38F09E7C7A4EF20FB27D510F36F489F`
- `gui_model.cpp`: `2E68432B8147D60C96BBBE745946D302912C0762B475B8EAD83C2CBFA32B298F`
- `simcontrol300_owner.cpp`: `935D1C933BE37FDDB18D4D5D1AABA0E29D0A8CEA985902220CC9F0E0B0C54977`
- `test_simcontrol300_frequency_edit.cpp`: `3115E05D9D28C400C6C9B11E99308312FA8B682D3A362E43F0F8F458627FCBB0`
- `test_viewer_box.cpp`: `1625FD68654D93946A5A9AF5CE3D0F3B5588F1D0FDD71008CA6F8A870D39FD37`

Only this report was written. No source/test/build/shared state, original, GUI or REA state was changed by this reviewer.
