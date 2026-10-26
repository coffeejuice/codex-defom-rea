# CP91 cached-reader marker access source review

## Result

The `u26c4[0x0b] == 0` decision now uses a bool `GuiModelAccess::withStep` callback at its original position (`src/engine/viewer.cpp:581-583`). The field remains a signed `std::int32_t` element. The callback reads only that element and returns the equality result; it does not normalize negative values, add a null fallback, return a pointer, or retain a borrow.

The actual model/step provenance is still established earlier: the `View`'s owner model is selected only when owner pointers exist; E owner, if present, must equal the same live step; N owner, for this branch, must be nonnull and equal the same live step. The marker callback is reached only after the prior zero-byte, dimension-3, and signed-version-at-least-6000 checks. Thus its direct element read has the same nonnull actual owner qualification as before.

## Prefix and miss ordering

The cache callback remains **after** coordinate allocation/key-0 COW, 3D index-table selection, MAR-index bounds, and the signed `g_a_index[128] > 0x0b` check. It remains **before** the node-reader timer checkpoints and every subsequent null/non-null DB branch, directory receipt/bounds check, possible frame install, and later array/object-assignment handling. A nonzero marker (including `-1`) bypasses the miss work and returns through the same cached-reader path. A zero marker continues into the existing miss path. No prefix effect, guard, timer, DB query, or barrier operation moved across the callback.

The model barrier's normal viewer-entry preflight prevents this code from being reached on a parked retry. The helper adds its own availability check directly before borrowing; it does not make a pending-frame continuation accessible. Its return type is bool, so no native pointer escapes.

## Existing focused coverage

The 45-case viewer-box target already exercises the relevant branches without a duplicate test:

- `geometry_high_version_cached_reader_returns_normally_with_null_mar_and_array_bodies` covers three high versions, marker values `1` and `-1`, actual/null E owner, and populated/null connectivity (24 rows), checking getter/COW effects, global table dimension, unchanged marker/version/pointers, and normal result.
- `geometry_high_version_cached_reader_bounds_and_miss_keep_real_prefix_guarded` preserves all MAR/index limit checks, including a zero marker that reaches the miss path after the real prefix.
- `geometry_high_version_cached_reader_uses_valid_altered_actual_header_indices` covers valid boundary indices and a negative marker.
- `geometry_high_version_cached_miss_cached_transition_preserves_each_actual_result` changes one model's marker `1 → 0 → -1` and checks cached success, guarded miss, and cached success with exact dispatch/body/state expectations.
- Existing timer-prefix, DB receipt/bounds, and parked retry cases cover later effects and no-retry mutation.

## ABI and scope

No helper/header, model field, native layout, public C signature, or export changed. Current hashes match the CP91 receipt: `viewer.cpp` `12f0e6ae1fa493d466bee87f9d83f0daaa63debf2fd51b53dd73972f7f7c4459`; unchanged `test_viewer_box.cpp` `58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98`. The focused and broad runtime results belong to root; no build or tests were run for this review.

No blocker found.
