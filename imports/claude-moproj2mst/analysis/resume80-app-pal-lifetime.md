# CP80 — `app_pal` owner lifetime across SplitView and Close

## Conclusion

The original getter is a QtGui module-global lazy cache, not a per-SplitView or per-Project cache. Its null branch allocates and caches one `QPalette` wrapper at `QtGui+0x9c9690`; subsequent reads copy from that same wrapper. The bounded getter disassembly shows no reset path. A native Project Close therefore should not be modeled as resetting this owner solely because a Project or SplitView-related receiver is released. That is a source-based lifetime inference, not a directly captured Close result: none of the retained original probe receipts records Open→Close→Open or reads the app_pal slot.

The current candidate placement is lifetime-compatible for one application/session: `WidgetRuntimeState::Values` owns one canonical `QApplicationPaletteOwnerState`; `ConversionSession::releaseProjectAfterCloseEffects()` releases only Project storage; the `WidgetRuntimeState`, MainView and app owner remain members until `ConversionSession` destruction. Calls for multiple placeholder values in that same runtime bind the same canonical owner but keep per-call getter output/cursor state separate. If a future adapter creates a new `ConversionSession` for each project while representing one live native QApplication, it would incorrectly reset this module-global state. The stated current lifecycle scope is a fresh application with one project, so repeated-project parity is not established by current validation.

## Original source and capture boundary

Static QtGui identity is pinned by `QtGui4_64.dll` SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. In `analysis/wave3-closure-next-65016f70.asm`, `QApplication::palette()` (`0x65016f70`) loads the static `app_pal` slot at absolute `0x659c9690` (`QtGui+0x9c9690`). If null, it allocates `0x10` bytes, calls `QPalette(Qt::GlobalColor)` at `0x6504bf80` with argument `2`, stores the returned wrapper pointer in the slot, and then copies from that wrapper. When nonnull, it copies directly from the same cached wrapper. QWidgetPrivate's default `QPalette()` path reaches this getter before storing its palette data in private `+0x170` (`0x6505e8d4` then `0x6504abb0`). The getter path itself contains no Project/Close condition.

Original splitter probe002 is a passive startup capture through `d35a80 return`: receipt `analysis/wave3-original-splitter-probe-002-receipt.json`, raw capture SHA-256 `bdb95dbe5840180a1b1bb178faafe997accbff835f66242e1a3c4618d8d47fca`. It reports six palette states and six equalities; MainView, SplitView and placeholders share the observed palette data pointer and local word zero. The probe does not read `app_pal` or hook the QApplication getter, so it cannot say whether the cache was initialized by MainView, another earlier QWidget, or a prior startup call.

Probe003 adds palette lookup/cache and widget palette observations but its retained receipt `analysis/wave3-original-splitter-probe-003-receipt.json` expressly limits coverage to startup through the same `d35a80 return`, says there were no Qt getter calls, no event pump, and no Open/Save/Close acceptance. Probe004 likewise ends at first actual SplitView constructor return and states no project Open/Save/Close comparison. These captures show startup sharing and selected lookup inputs, not persistence across close/reopen. The original editor probe `close-evidence.json` files are capture-session cleanup records; they are not app_pal observations through native Project Close.

## Candidate ownership and getter assumptions

Current implementation (`source/apps/moproj2mst/src/engine/view_manager.cpp`) stores `applicationPaletteOwner` in the one `WidgetRuntimeState::Values` object. `observeApplicationPaletteSlot` requires the pinned QtGui hash and exact RVA, a qualified runtime and palette alias already interned by that runtime. A present wrapper is checked against the runtime's registered palette data owner. Startup `defaultPalette` and the separate class-palette cache do not infer the global slot state. The owner accessor is read-only diagnostics.

`readApplicationPaletteFor` revalidates the live receiver, runtime and exact registered owner, then calls `owner->read()` without an initializer. A known-present slot returns a copy from the canonical wrapper; a known-null slot remains an initializer dependency. This matches the observed static getter's already-present copy branch, but it does not emulate the native first getter when the slot is null. For SplitView specifically, static constructor order places MainView QWidget construction before SplitView construction, and MainView has a default QPalette input. That makes “app_pal is already present when SplitView asks” plausible, but the original probes did not inspect the slot during MainView construction and do not prove when or from which call the first cache fill occurred. This is the key getter-integration assumption.

`SplitViewPlaceholderCall` holds the canonical owner weakly and the runtime revalidates it on each continuation. The runtime owns the wrapper, while a completed getter result lives in that call's separate state. Repeating the getter for a second placeholder under the same runtime therefore reads the same global wrapper and produces a distinct output receipt; destroying one placeholder call does not reset the runtime owner. `ConversionSession::releaseProjectAfterCloseEffects()` releases Project/model storage and keeps `widgetRuntime_` and `mainView_`; only `ConversionSession::~ConversionSession()` releases the MainView/app adapter scope. This candidate lifetime split is application/session versus project, consistent with the static owner boundary. It is not evidence that the full native Close path preserves the value; it is the current candidate ownership model.

The helper's original CP77 comment says it is “not wired into WidgetRuntimeState's class palette cache or the SplitView callsite.” The class is now wired through both a runtime owner and the placeholder getter. That comment is stale; no source edit was made in this read-only task.

## Next bounded original state contract

The next evidence needed is one same-process sequence with fixed QtGui module base and QApplication identity: observe `app_pal` and its palette data owner immediately before/after the earliest MainView QWidget palette getter, then at the first SplitView placeholder getter, after accepted Project Close, and at the corresponding getter on a second Open. Record whether the wrapper pointer and palette data owner remain the same; if a setter or Close replaces either, capture that exact transition and its caller. No equality-only receiver capture can substitute for the slot owner observation.

Until that sequence exists, retain one app-level owner through Project Close in a single runtime, keep the actual slot state Unknown unless explicitly qualified, and refuse to infer KnownPresent from the startup default or class cache. KnownNull should remain pending in the current candidate getter path until an exact native-equivalent constructor/cache provider is separately qualified. Do not reset the owner on placeholder destruction or `releaseProjectAfterCloseEffects`; do not claim repeated native Open/Close parity.

## Evidence hashes and limits

- Probe002 reconciliation: `analysis/wave3-close-splitter-probe-002-reconciliation.json`, SHA-256 `3be51baafe0f1a89d5611943656c89ef42ccdd08a084c3af4a3739d7bbd2c07f`.
- Probe003 receipt: `analysis/wave3-original-splitter-probe-003-receipt.json`, SHA-256 `657ca5da216ac9337979d28f9101b0715151a71ec585b228464d44b90ea37791`.
- Current candidate `view_manager.cpp` at audit read: `958324aa0d33696057bec472b72bb149be20c28dd155f1eead54d8539e1f8718`.
- Current candidate `conversion_session.cpp` at audit read: `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604`.
- `application_palette_owner.cpp`: `58d6355b88150698b0af87128e8994f3595fa2c4d0a8db995aec1875d7851348`.

No source, build, test, GUI or mutable REA operation was performed for this report. Current candidate source was inspected at its then-current hash; another source owner may advance it after this read.
