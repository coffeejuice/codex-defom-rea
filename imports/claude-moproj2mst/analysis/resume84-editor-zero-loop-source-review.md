# CP84 independent source review — positive-length zero path loop

## Result

The frozen change follows the selected native loop order and retains the earlier guards. The predicate reads the current GuiModel object’s embedded DYNARY, admits only the bounded type7a/ndim1/ncomp4 shape, checks every axis sample before any dependent value, and refuses nonzero or unordered samples. Its `4*n0` value count cannot overflow signed 32-bit arithmetic for admitted inputs. Fill and Apply both evaluate the predicate against the current model; Apply rejects changed/unsupported data before its first store.

One boundary remains explicit: the DYNARY layout has shape metadata and raw pointers but no stored allocation capacity. The guard checks positive/bounded counts and required nonnull pointers; the well-formed model-owned DYNARY allocation invariant supplies the actual extent. It cannot certify arbitrary externally corrupted pointer/count pairs. The added tests use the actual embedded DYNARY and allocator, and no assertion should be read as coverage for arbitrary raw pointer metadata.

## Owner and shape trace

`GuiModel::objectAt()` in `source/apps/moproj2mst/src/engine/gui_model.cpp:217-221` indexes the live step object list. `advancedTimeProfile()` in `simcontrol300.cpp:438-469` keeps that object pointer and obtains `object+0x25a8` as a DYNARY reference via `objectField`; it does not construct a clone, consult a stale AdvancedPage snapshot, or allocate a curve. The native receipt in `analysis/resume84-editor300-next.md` identifies the same object+25a8 path owner.

For a table path, the production guards run before either loop:

- `type == 0x7a`, `ndim == 1`, positive `n[0]`;
- `n[0] <= INT32_MAX/4`, before any axis/value element read;
- `ncomp == 4`, `axis[0]` and `values` nonnull;
- no inactive-axis counts or pointers, and no additional `u68` owner.

The `ndim == 0` scalar-zero branch remains separate and unchanged. Other dimensions, types, component counts, empty/negative/overflow counts, null arrays, extra owners, nonzero samples and NaN remain refused. `!= 0.0` rejects unordered NaN and accepts either sign of zero.

`source/port/defmodel/src/table/dynary_template.cpp:62-113` confirms the represented allocation contract: `dynary_value_count()` multiplies `ncomp` by active dimensions with signed32 arithmetic; `dynary_alloc()` allocates each active axis by its count and allocates the contiguous value array by that value count. There is no separate capacity field in `DYNARY`; therefore nonnull alone is not a capacity proof. The candidate’s narrowing to `0 < n0 <= INT32_MAX/4` chooses the nonwrapping valid profile instead of admitting native overflow/wrap behavior.

## Read order and arithmetic

After all profile guards pass, `simcontrol300.cpp:454-459` loops `point` from zero to `n[0]`, comparing every `axis[0][point]` to zero. Only after that complete axis loop does it compute `valueCount = 4 * n[0]` and scan `values[0..valueCount)`. This matches the retained native branch: all axis samples first, then the signed32 count helper, then all dependent values. At the maximum admitted `n0=536870911`, `valueCount=2147483644`; both loop upper bounds and increments remain within signed32 range. Over-limit counts refuse before either array loop or multiplication.

The original count helper has a wrap-to-zero behavior for negative wrapped products; that shape is not emulated. This candidate supports the positive, nonwrapping profile only, and the source report explicitly keeps other profiles guarded.

## Live Apply and regression coverage

`fillAdvanced3D()` calls `advancedTimeProfile()` on the live model (`simcontrol300.cpp:1154-1161`). `advancedApplyProfile()` calls it again (`473-482`), and `applyAdvanced3D()` returns `UnsupportedSnapshot` before its first model store when that live predicate fails (`1207-1211`). Thus changed counts, reallocated arrays or newly changed samples are checked at Apply; AdvancedPage stores no previous DYNARY pointer or “was zero” result.

The four CP84 regression cases cover:

- actual two- and three-point allocations with mixed signed-zero axis/value bits, preservation of array identities and position/scalar sentinels through changed-time Apply;
- nonzero, subnormal, infinity and NaN in later axis entries refusing before time stores;
- nonzero/unordered samples across later dependent values, including the final point’s fourth component;
- resizing/reallocating the same embedded curve through `dynary_alloc`, applying against current arrays, then refusing a later live value change before the time store.

The amended shape guard regression tests zero, negative and overflow counts against a real one-point allocation, restoring metadata before object destruction. These cases establish that the overflow/empty guards reject before a possible out-of-bounds traversal; they do not test enormous valid allocations.

## Limits and validation status

This internal predicate increment does not recover nonzero curve evaluation, other type/dimension/component profiles, malformed pointer/capacity states, or public editor300 reachability through the earlier widget/private-palette gate. The CP84 implementation report records static preflight only. I did not build or run tests in this review.

Reviewed frozen hashes:

| File | SHA-256 |
|---|---|
| `include/m2m/engine/simcontrol300.hpp` | `d0702d081a2074bc0e6d1646f56846665c3caf6f97e79442b455031a2d0447a2` |
| `src/engine/simcontrol300.cpp` | `9c4cc19de5d61573365dd8a5d1d3bc089a85b2ece8515676e28a7ae3e93f78ee` |
| `tests/mdt/test_simcontrol300_snapshot.cpp` | `d7896740ea11d305e1edec438d684f0e13b9c6bb976bafea7846c7d133994f93` |

Native path/count evidence and its limits are recorded in `analysis/resume84-editor300-next.md`. The source-level review found no discrepancy in the requested contract.
