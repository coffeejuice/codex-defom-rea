# Palette ref and partial-slot integration review

No blocking mismatch was found in the released CP81 cut. The getter associates one reference state with the same runtime-interned palette data, retains one source token per actual modeled read and carries that token into the same placeholder. The placeholder stores only its private data pointer, keeps the word unknown and always refuses before temporary deref. No count is seeded and no private decrement, destination ref or public activation is admitted.

Only this report was written. No source, build, test executable, GUI, original execution or mutable REA changed. All seven inspected source hashes match the frozen implementation receipt. This is static source review, not executed acceptance.

## Same data and canonical ref origin

`WidgetPaletteDataState` owns the new private `nativeRefs_`, with const diagnostic `nativeReferenceState()` (`view_manager.hpp:249-261`). No public setter accepts an external ref state. The actual data remains the existing registry object, not a new palette graph or an alias-only attachment.

`readApplicationPaletteFor` (`view_manager.cpp:344-385`) retains the CP79 receiver/runtime/registered app_pal owner preflight. It resolves the wrapper data through this runtime's alias map and requires pointer/control-block equality. The mutable registered data object, not a copied wrapper or foreign runtime's equal alias, receives the reference state.

For KnownPresent, `:370-379` creates a state only if this data lacks one. Its stable qualification includes the runtime and palette alias; an existing state's canonical control block, exact qualification, native availability and absence of destruction-pending status are checked before reuse. Invalid state is refused rather than replaced. Association is guaranteed by the private data member and runtime lookup, not by qualification text alone.

Unknown/KnownNull have no wrapper/data and take the unchanged no-provider helper read before state/token creation. They cannot retain a startup/default palette or initialize GlobalColor2. Present output prepares one GetterTemporary token and performs one retain before returning the logical copy, matching native getter-copy ref `6504a8c6`. This records sequence/ref/balance while numeric bits stay unknown. No production `observeCurrent` call or initial numeric count was added.

The selected normal allocation/getter profile is preserved: constructor/unwind/allocation-failure closure is not newly claimed. This review does not promote host allocation exceptions into native returned failure or propose rollback of a committed source retain.

## Getter token retention

`QApplicationPaletteRead` privately stores its mutable token, exposing a shared const diagnostic and friendship only to runtime/placeholder (`application_palette_owner.hpp:27-38`). Host report copies share the same token; they do not issue another retain. Standalone helper reads remain tokenless because `application_palette_owner.cpp` is unchanged and cannot manufacture actual runtime origin.

The placeholder retains the CP79 one-read cursor. A successful runtime read adds one ApplicationPaletteGetterReturned receipt and leaves ApplicationPaletteGetter permanently (`splitview_placeholder.cpp:134-144`). Subsequent resumes do not call the getter or retain again. Independent runtime reads use distinct tokens on the same data state, so their sequences/balances compose without treating diagnostic copies as native values.

Before the pointer store, `:146-154` requires a completed output, actual app_pal wrapper data/control block, matching data reference state/control block, live non-pending origin and a Held GetterTemporary with a retain receipt. Null token/data/wrapper or expired state short-circuit safely before dereference. A tokenless standalone result, different data or withdrawn native origin cannot authorize the private store.

## Partial destination and exact stop

`WidgetPaletteConstructionSlot` holds the same shared data plus an optional word. The optional slot belongs to the canonical WidgetState and is exposed only through a const diagnostic. It is not a completed `WidgetPaletteValue`, a second data graph or another native ref.

At PrivatePaletteCopy, the placeholder refuses to replace an existing completed/partial palette, then records exactly:

```text
widget.partialPalette.data = SAME getter data
widget.partialPalette.word = unknown
effect PrivatePaletteDataStored
stage PaletteTemporaryDeref
```

This is `splitview_placeholder.cpp:155-163`, corresponding to native `6504abcb`. No destination retain occurs. Repeated resumes skip this store and revalidate the same partial data/unknown word plus absence of completed palette (`:164-166`). Optional engagement correctly distinguishes unknown word from an explicitly initialized word0.

The final branch (`:167-171`) returns PaletteRefUnavailable when bits are unknown, or PaletteTemporaryDerefUnrecovered when bits are known. **Neither branch calls token.release()**, performs a decrement, sets private word0, creates a destination token/ref, destroys/frees data or advances font. An isolated manual count anchor is deliberately insufficient to execute the integrated deref. Known numeric0 would likewise be known/unrecovered rather than mistaken for unknown; no raw branch is executed in this cut.

The native required continuation is still pointer store → temporary deref → zero destruction guard or private word0 → destination ref → font. This release stops at the second step, preserving Held temporary and committed pointer, so it does not cross the refcount/destructor ordering proved in CP78/80. QWidgetReturned/completeForPublicSave remain false and no production SplitView/Editor300 caller was added.

## Lifetime and observer limits

`WidgetRuntimeState::~WidgetRuntimeState` invalidates each attached state's native admission (`view_manager.cpp:831-838`), without deref, count reset or physical data free. Strong historical data/state/token diagnostics therefore cannot keep the expired runtime callable. The placeholder's existing runtime-first validation retains its getter/partial-store receipts while refusing later work.

`WidgetState::releaseNativeOwner` drops partial-slot metadata alongside existing completed palette metadata (`:814-826`). This is explicitly host invalidation, not a source pointer rollback or native destructor. Token destruction has no native decrement; balance/sequence retain the unclosed source reference. The tests distinguish those history effects from C++ storage loss.

The token's diagnostic state origin still exposes the isolated CP80 observation surface. That is not a runtime current-count producer or original count receipt. Current code correctly refuses integrated deref even after such observation. A later source task must supply exact current-sequence/data/runtime provenance before enabling release; it cannot use `currentBits().has_value()` alone as native admission. Per-widget observed input/value reconciliation and the zero branch's complete QBrush/free closure likewise remain future obligations.

The new store changes no retained application wrapper or completed private palette, font/locale/thread, QWidgetData/class/registry membership, QObject delta or QWidget current/maximum counter. Getter ref sequence/balance is separate from all those counters and never inferred from174/760 or C++ use_count.

## Focused tests read

The inspected test files contain25 placeholder and19 runtime-binding cases. Successful getter cases now expect the narrower temporary-deref guard. New placeholder cases verify same data/unknown partial word/Held token, copied diagnostics and retry without ref/store replay, known isolated count still refusing decrement/word initialization, withdrawn origin preserving prior pointer/receipts and abandonment without native rollback.

Runtime tests require one state per data with distinct tokens for distinct reads, equal aliases in separate runtimes remaining distinct, runtime loss invalidating callability without deref and invalidated origin refusing another getter without replacement. Unknown/null tests explicitly require no token/state creation. Existing counter/lifetime/qualification cases remain and inspect unchanged counters/fields.

The cases directly cover this source cut, using declared isolated state/owner fixtures. They do not establish a current original native count, execute a private-copy destructor branch or validate public startup/Open/Save. No tests were compiled/run in this review. Root retains grouped/full stable-source validation and archive responsibility.

## Frozen source identity

Implementation report SHA-256: `afe09507afdcc2710f4ae8aaf43ae3bd20b7d861c22fe9346fbc76bf8a86648a`.

| Source under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `include/m2m/engine/view_manager.hpp` | `0715b73412eaac42084d3830de6a667d4804777bc76c040137053ca9a4078eff` |
| `src/engine/view_manager.cpp` | `4f11d60d40ccef3730f4cd7575af53ebf60af1af8581b76fb7d732e64c69e805` |
| `include/m2m/engine/application_palette_owner.hpp` | `a28bfa6b3fa141d7e063350c2b5f9c8f71ca74cc68f04d2a62540391c2dfd78b` |
| `include/m2m/engine/splitview_placeholder.hpp` | `bed773b7c03166e16d9295857459b71ad7a6b86c71a6f3e9767360264c1741b8` |
| `src/engine/splitview_placeholder.cpp` | `560c4fbd58c6d5fe15447d18929b0e276121cf0e19bf56c3bb2dc9f627038128` |
| `tests/mdt/test_splitview_placeholder.cpp` | `d7b4e2f05bb9af07cd3cd9b61a7058039f7beb6464a219a74b38098241b6665d` |
| `tests/mdt/test_widget_application_palette_binding.cpp` | `0e560b1961d8a537ab7d7d152c771199082fbaa554cae30433fa8ad9455dd33e` |

Token measurements are unavailable.
