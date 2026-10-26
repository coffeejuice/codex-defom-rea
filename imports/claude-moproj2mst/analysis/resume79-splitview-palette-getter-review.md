# Retained placeholder palette getter review

No blocking mismatch was found in the released CP79 **getter-only continuation**. It validates the same runtime/placeholder/registered owner, retains one successful result, preserves Unknown/KnownNull dependencies without a provider and stops before private data-pointer copying. The released source hashes match the implementation receipt. This is static review, not a compilation/test or native runtime acceptance result.

Only this report was written. No source, build, executable test, GUI, mutable REA or original execution changed.

## Runtime read admission

`view_manager.cpp:343-371` implements `readApplicationPaletteFor(receiver, expectedOwner)` over the existing CP78 runtime owner. Its preflight requires a live shared runtime, a nonnull native-live receiver whose component control block is canonical, that receiver's exact runtime/control-block binding and exact expected/registered owner pointer/control block. A copied or rewrapped helper cannot substitute for the registered owner merely by sharing data or qualification text.

For a present wrapper, it additionally resolves the wrapper data's positive observation alias through this runtime's palettes map and checks exact pointer/control-block equality. A present slot without its wrapper/data is refused before read. The function does not create module observations, a runtime owner, per-widget private inputs or a default fallback. Existing registered owner existence implies the Values owner is present before its map access.

Only after preflight does it call the canonical helper's `read()` with **no constructor provider**. Unknown returns SlotStateUnknown, no output/effects; KnownNull returns InitializerUnavailable, no allocation/construction/cache/output; KnownPresent returns the same data/full wrapper word with OutputCopied only. Its no-provider behavior cannot turn startup defaultPalette into QPalette(GlobalColor2) construction. A direct runtime read is one getter call; once-only retention correctly belongs to its caller frame rather than to this shared getter.

The header uses a forward declaration for the returned result and the implementation includes its complete definition. The new runtime API does not introduce a helper/header cycle or expose a mutable owner/provider through the existing diagnostic getter.

## Same placeholder and one successful read

`splitview_placeholder.cpp:44-57` validates its retained weak runtime and original canonical control block on every resume. `:102-110` then checks the retained value's actual widget, native identity equal to that value, canonical component block and exact runtime binding. Unexpected parent/thread ownership remains a PrefixStateChanged guard. No other widget, MainView or editor can replace the call's private value.

At PrivatePalette, `:111-126` preserves the old no-owner/private-input guards. Explicit per-widget observations alone do not produce a getter output. Existing private receipt qualification must match this call before owner binding. With an actual runtime-registered owner, the call binds it **weakly once** and changes to ApplicationPaletteGetter.

`splitview_placeholder.cpp:130-143` checks that same weak owner against the runtime's current owner before reading. Only ApplicationPaletteGetter calls the runtime read. A pending read report is retained without adding getter-return effects. Later source observation can refine the same owner's Unknown state, but does not itself resume the placeholder. On successful output the call retains the returned result, adds one ApplicationPaletteGetterReturned caller receipt and advances to PrivatePaletteCopy.

Every later resume revalidates ownership but skips the read block because its stage is PrivatePaletteCopy. Thus it cannot replace a completed output with a pending report, issue OutputCopied again or duplicate the caller receipt. The optional result lives in the same nonmovable call object; diagnostics expose only a const pointer and weak const owner. Two placeholder calls have distinct result storage/native values while sharing the runtime's module wrapper/data.

Runtime loss is checked before further native activity and leaves the previous stage, successful/pending result and effect history intact. The completed result's data lease or an escaped diagnostic owner does not hold the runtime alive. Value abandonment invalidates its widget's native-owner admission; it does not claim native palette temporary destruction, QWidget cleanup, posted work or counter decrement.

## Exact stop and remaining native work

The native boundary remains the CP78 sequence: QWidgetPrivate `6505e8d4` invokes default QPalette `6504abb0`, which first obtains a QApplication palette getter temporary. The next private destination data-pointer store is `6504abcb`. This release retains the getter result and stops **before that store** (`splitview_placeholder.cpp:145-148`).

It does not assign `WidgetState::palette`, reset its private word to0, execute temporary deref/possible60-brush destruction/free, ref the private destination, copy font/locale, retain the native QObject thread, assign QWidgetData/class/registry or increment current/maximum QWidget counters. The getter's full nonzero wrapper word remains in its result; it is not prematurely copied into private storage or zeroed at the global wrapper. The report declares QWidgetReturned and completeForPublicSave permanently false.

OutputCopied is the existing logical getter value-copy receipt. C++ shared data ownership is not a new explicit native atomic-ref ledger and this review makes no numeric Qt refcount claim. Full copy completion still needs the source-ordered private pointer store, temporary release/destruction branch, private word0 and destination ref. No production SplitView/Editor300 public call site is added; earlier constructor/PostPending gates remain separate.

At this getter-only boundary, checking existing private input **qualification** is sufficient for the declared scope: it produces a temporary without altering the private destination. Before the next private copy, a consumer must also reconcile any actual per-widget observed value/produced receipt and retain the source temporary-release ordering. Existing input observations must not silently authorize the later copy or replace its produced data.

## Focused test review

The released test files contain21 placeholder and15 runtime-binding cases, matching the implementation report. Existing prefix cases remain. Seven new placeholder cases inspect late Unknown-to-Present resume, KnownNull without initialization, no completed-private-input prerequisite, retained result/runtime loss, two independent frames, wrong receipt qualification and runtime loss while pending.

The successful retry case retains the exact diagnostic result address, one OutputCopied and one caller return receipt through repeated PrivatePaletteCopy resumes. Other cases explicitly require absent private palette/font/locale/thread/data/registration and preserve unknown or known historical counters174/174 and QObject98. The two-frame case verifies abandoning one native placeholder leaves the other's result and effects unchanged. Unknown/null cases do not obtain an initialized wrapper from defaults.

Four new runtime-read cases cover Unknown/Null semantics, full-word same-data success, missing/foreign/copied/rewrapped owner rejection and null/foreign/rewrapped/dead receiver/runtime rejection. These directly exercise control-block and current registry admission independent of the placeholder's outer checks.

The tests are declared isolated adapter observations, not acquired original state or first startup/Open/editor timing evidence. They do not test the still-unsupported private-copy/refcount effects, and no such completion assertion appears. No compiler/CTest/test executable ran in this review. Root must provide grouped/full validation and source/artifact freeze before treating this source contract as tested.

## Reviewed hashes

Implementation report SHA-256: `47b66e2d10005f7df2db62131e6272ff01816ce2625f76e138b5115c3ffca842`.

| Released source under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `include/m2m/engine/view_manager.hpp` | `b7ec2ca09e342a2fd5760b7f97c236fc23b159d54a1a9094dadb614d4793aba5` |
| `src/engine/view_manager.cpp` | `958324aa0d33696057bec472b72bb149be20c28dd155f1eead54d8539e1f8718` |
| `include/m2m/engine/splitview_placeholder.hpp` | `d9d6a335d5d8ac7be4ddff5ec86504ed72fc7c6cac72b9e9b66accf9ce90354b` |
| `src/engine/splitview_placeholder.cpp` | `9db88e6ca7144796e88b91192af3ec5221d3d16ac7a35314cf81401371e1668d` |
| `tests/mdt/test_splitview_placeholder.cpp` | `c441260a74879b6b02f53eed8d26747456415d917d81676ed769cdda981db4d2` |
| `tests/mdt/test_widget_application_palette_binding.cpp` | `fe14fb63d1ce700ba32cdcaea8f6b782a02448cc7bd05999a89669d355349bde` |

Token measurements are unavailable.
