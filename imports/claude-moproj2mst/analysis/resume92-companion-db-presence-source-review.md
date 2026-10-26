# CP92 companion DB presence access source review

## Result

The selected `nativeStep->db != nullptr` test is now an inline bool `withStep` callback at the same point in the cache-miss path (`src/engine/viewer.cpp:594-596`). It checks only pointer presence. It does not dereference or return the companion pointer, capture it, or hold the borrowed step beyond the callback. The prior actual N-owner identity checks establish a nonnull actual step before this expression; there is no invented null-step branch.

## Timer and owner-qualification order

The order is preserved exactly:

1. Actual model/step ownership, zero synchronization byte, 3D dimension and signed-version checks.
2. Coordinate getter/COW prefix, 3D global table selection, MAR and ID bounds.
3. Cache marker zero decision.
4. Node-reader timer `Start` and `ReadSingleAArrayFromDB` checkpoints.
5. Guarded companion-DB presence check.
6. On nonnull only: resolve `currentProject`, take the actual `Project::dataDb` strong lease, require it matches `model->db`, obtain its reader and require identity with the companion pointer, then validate the live directory receipt and physical index before query/frame effects.
7. On null: retain the existing arrays-timer prefix and later object-assignment refusal.

Thus the new helper's availability guard remains after the two timer checkpoints, matching the selected expression's original location. The presence test does not bypass forged-pointer defenses: a nonnull but foreign/expired companion proceeds only to the existing owner identity checks, before any dereference through the candidate pointer. The `reader != model->step->db` comparison remains after a valid `DbHandle` lease resolves an actual reader.

## Null and parked paths

The null-companion tests verify the node-reader timer prefix, arrays-timer behavior over varied initial counts, unchanged DB/cache/version/object state, and refusal before unqualified object assignment (`test_viewer_box.cpp:1691-1756`). The opaque nonnull DB cases and eight owner/reader provenance modes cover foreign, missing, expired, or forged identities after the same timer prefix (`1918-1951`). Actual directory-query tests cover the valid Project-owned reader/receipt path and its bounds/reload behavior (`1870-1974`).

For parked models, the existing viewer barrier preflight stops before `qualifiedGeometryReadiness`; the `withStep` helper adds an immediate pre-borrow availability check if reached. Existing parked retry tests verify the timer and model remain unchanged. No frame installation or retirement path is added by this presence test.

## ABI and scope

This adds no helper/header, field, native layout, public signature or export change. Current hashes match the CP92 receipt: `viewer.cpp` `b94efd57a7beb810b7d73498eb43f0f5c52aba320150ca9dd07aa5d11f4cbf38`; unchanged `test_viewer_box.cpp` `58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98`.

No blocker found. Read-only review; no source/test edits, builds, or tests were performed.
