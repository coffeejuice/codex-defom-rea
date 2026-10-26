# CP76 — QApplication default palette owner and cache path

## Conclusion

The retained QtGui PE evidence identifies a separate lazy application-default wrapper at `QtGui+0x9c9690` (`QApplicationPrivate::app_pal`). `QApplication::palette()` (`0x65016f70`) reads this slot; when null, it allocates a 16-byte `QPalette`, constructs it using `QPalette(Qt::GlobalColor)` with argument `2`, stores the wrapper in the slot, then copies from that owner into the caller's output. On later calls it skips allocation/construction and copies from the existing slot owner.

The candidate has an application default palette **value/data owner** (`Values::defaultPalette`) plus a bool and map for the separate class-palette cache. It does not model `app_pal` as a distinct lazy wrapper/cache owner or preserve its first-use transition. Neither the original placeholder capture nor candidate evidence qualifies the actual `app_pal` slot state at the relevant constructor input. Therefore there is no established matching candidate owner for the native singleton wrapper lifecycle. A distinct, optional `app_pal` owner/lifecycle state is needed before claiming that transition; do not use the existing class-cache bool as a substitute. The already-existing palette shared-data owner can remain the value backing store after provenance binds it.

## Retained PE path

Source: retained `evidence/original-binaries/QtGui4_64.dll`, SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

`analysis/wave3-closure-next-65016f70.asm` gives the complete bounded getter path:

- `0x65016f8a` loads the pointer at absolute `0x659c9690` (`QtGui+0x9c9690`).
- If null, it calls allocator `0x655e1172` for `0x10` bytes (`0x65016f96..9e`), then calls `0x6504bf80` with second argument `2` (`0x65016fad..b0`). The symbol map binds `0x6504bf80` to `QPalette::QPalette(Qt::GlobalColor)`.
- It stores the resulting owner back to `0x659c9690` (`0x65016fb9`), then calls `0x6504a8b0` with destination caller output and the cache pointer as source (`0x65016fc0..c6`). The symbol map binds this helper to `QPalette` copy construction.
- On nonnull entry, the same copy call at `0x65016fc0..c6` runs directly, with no wrapper allocation.

The preceding `QPalette::QPalette()` at `0x6504abb0` calls this QApplication getter into a temporary, copies its shared data pointer to its own destination, and then performs the Qt shared-data reference operation (`analysis/wave3-closure-next-6504abb0.asm`). In QWidgetPrivate, that no-argument palette constructor is called into `private+0x170` at `0x6505e8d4` (`analysis/wave3-closure-next-6505e780.asm`). Thus the chain that feeds SplitView is:

`QWidgetPrivate+0x170 ← QPalette() ← QApplication::palette() ← app_pal slot`

The getter analysis `analysis/wave3-closure-next-close-splitter-palette-global-getter-static.json` covers `0x65016f70`; its symbol map places `QApplicationPrivate::app_pal` at `0x659c9690`. This is static PE evidence, not a record that the original probe read the slot or observed the lazy allocation.

## Runtime evidence boundary

Original probe002 (`build/original-splitter-probe-002/capture-evidence.json`, SHA-256 `bdb95dbe5840180a1b1bb178faafe997accbff835f66242e1a3c4618d8d47fca`; reconciliation `analysis/wave3-close-splitter-probe-002-reconciliation.json`, SHA-256 `3be51baafe0f1a89d5611943656c89ef42ccdd08a084c3af4a3739d7bbd2c07f`) records that MainView, SplitView, and both placeholders ended with the same palette data pointer and local group/resolve word zero. The reconciliation explicitly says these equality outcomes do not close earlier QApplication getter/cache effects. The capture records QApplication and widget-list/count globals, but not `app_pal`, its initial null/present state, the allocation, or the returned getter copy. It consequently cannot qualify which getter branch ran or serve as runtime proof of the singleton wrapper's owner identity.

Candidate `source/apps/moproj2mst/src/engine/conversion_session.cpp::freshConverterWidgetValues` seeds an alias-local `applicationPalette.defaultValue` and marks `classCachePresent=true`, with QMenuBar/QMenu class entries. `WidgetRuntimeState::Values` in `view_manager.cpp` stores `defaultPalette`, `paletteCachePresent`, and optional `paletteClasses`. The cache bool/map represent class-specific palette lookup state. The code exposes `applicationPalette()` as the default value, but has no independent `app_pal` wrapper-presence/initialization owner. Aliases are runtime-local identifiers, not the original address `0x659c9690` or a proof of the same owner.

## Smallest required state and admission

Represent the singleton wrapper separately from both the default `WidgetPaletteDataState` and the class-key cache. Its state must distinguish at minimum **unqualified/unknown**, **null and not yet initialized**, and **present with a qualified `WidgetPaletteValue` owner**. Only a source-qualified first getter may transition null to a constructed/cached owner; only then should it copy that owner's value to the actual placeholder input. If evidence arrives only for an already-present slot, bind that exact owner without replaying allocation/construction. Preserve the getter's no-cache-mutation copy path for already-present state. Do not synthesize `app_pal` as present from `paletteCachePresent=true`.

The existing `WidgetPaletteValue` data object can back the wrapper's palette value after that actual owner is qualified; a second duplicate palette brush/data graph is not justified by the PE path. The missing bit is identity and lifecycle of the global wrapper plus its initialization branch, not another uncorrelated palette value.

An acceptance test should use a nondefault seeded palette owner and check both branches: (1) qualified null slot performs one wrapper initialization and produces a copy sharing that data owner with local group/resolve word zero; re-entry does not allocate again; (2) qualified present slot copies without initialization and preserves owner identity. Unqualified slot state must remain pending and must not fall through to the class-cache map or runtime startup default. The test should keep the QWidget counter unchanged at this phase.

## Evidence hashes and limits

Relevant static file hashes: getter disassembly `analysis/wave3-closure-next-65016f70.asm` `632a5d4727f9c292c646769f91acee102a871b4dddbb1f202c94da6670117c39`; no-argument palette constructor `analysis/wave3-closure-next-6504abb0.asm` `398b9a1b6a81bc2b6388502df686a93cd12e8c44e45096a50fd553902a4b3a4f`; QtGui PE hash above. Candidate files were read only. No new original runtime capture, build, GUI control, or mutable REA operation was performed.
