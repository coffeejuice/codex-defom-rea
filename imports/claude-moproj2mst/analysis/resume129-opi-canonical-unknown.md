# CP129: canonical Operation-owned Unknown observation

The root-owned Operation member now has focused validation that it is a single const-accessed Unknown observation bound to the same OPI generation. A detached KnownNull diagnostic cannot substitute for it. The diagnostic commit API additionally refuses the canonical instance, even if a caller removes const, so a receipt string cannot become a production null-state shortcut. No defaults event, current native target, model allocation, cleanup advancement or public Close admission is claimed.

This lane edited only the reserved existing cache header/source/test and this receipt. Root independently owns the model.hpp include/member/accessor. No model.hpp, operation_rescue, loader, cleanup, Session/main, CMake or plan edits here; no compilation/tests, GUI/original execution or mutable REA database.

## Contract and source qualification

Root placed private `OperationInfoModelCache operationInfoModelCache_{*this}` immediately after operationInfo_, exposing only `const OperationInfoModelCache& operationInfoModelCache() const noexcept`. Static inspection confirms identity initializes first. Existing noncopy/nonmove Operation semantics are unchanged. This member contains observation metadata, not native SfMoItemModel storage or a Qt guard registration.

The cache constructor retains its existing same-live-OPI guard capture. No model factory, per-slot make_shared, default-null event or creating lookup was added. This receipt does not assert that all modern Operation construction is allocation-free: identity/recovery allocations already exist, and no allocator probe ran.

`commitNativeDefaultNull` preserves the existing detached diagnostic behavior. After same-live-OPI validation, it compares this instance with the canonical accessor's address and refuses that instance with M2M_E_UNSUPPORTED before any label copy or knowledge update. Unknown/expired/foreign identity timing remains guarded. Detached receipt validation, first-commit preservation and noncreating/creating lookup behavior remain unchanged. The new runtime check supplements the const-only production interface; it does not introduce a canonical promotion method.

The CP128 owner audit `analysis/resume128-opi-cache-owner-audit.md` SHA-256 `fa50e31f533059cd88cf0e7a24967eccb54979aa1430ddb765992bb65f1f7e1c` qualifies immutable original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Native ctor04fdd1 initializes+160 through15d170 before defaults05067b. Later054950 and054f40 can construct/assign models; parser Props+c0 and earlier class/Props loader callbacks preclude carrying an unaudited defaults-null receipt into cleanup. The canonical slot intentionally remains Unknown across all current candidate paths. No native KnownNull claim follows merely from its existence.

## Focused tests and static preflight

Target `m2m_mdt_operation_info_model_cache` now has15 Release-active Testkit cases (eight retained, seven added):

- Compile-time const-only accessor types for mutable/const Operation and inability to invoke the diagnostic mutator through that const result.
- Stable canonical member identity, live OPI binding, empty receipt and Unknown lookups refusing both create flags.
- Actual candidate operationDefaults/initOperation retaining the same Unknown member, without a source-owned+160 event.
- Detached same-OPI KnownNull leaving canonical state Unknown; the diagnostic API refusing canonical promotion even after deliberate const removal.
- Lookup leaving cleanup progress and OpClass.current live.
- Foreign/explicitly expired OPI rejection, with identity validation preceding the canonical-promotion refusal.
- Guard snapshot expiry after Operation destruction and optional-storage address reuse producing a new Unknown generation; no destroyed canonical reference is accessed and the old detached diagnostic rejects the reused address.

The new cases do not instantiate a native model, assert native allocation/deallocation, commit default events or expire an OPI through production cleanup. Explicit expiry/reset/reuse are declared fixtures. Existing detached diagnostic tests remain unchanged.

Static preflight passed real quoted-include resolution, root accessor/member declaration inspection, matching header/source API, Testkit main/macros, supported CHECK/CHECK_EQ/REQUIRE usage, no C assert/REQUIRE_EQ, and `git diff --check`. No build or tests ran in this lane. Root owns broad rebuild, focused tests and grouped source-stable freeze:

```text
cmake --build <build> --config Release --target m2m_mdt_operation_info_model_cache
ctest --test-dir <build> -R '^m2m_mdt_operation_info_model_cache$' --output-on-failure
```

## Frozen exclusive receipt

Paths below are relative to source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|include/m2m/engine/operation_info_model_cache.hpp|`bd87dc12903ff0a5ca1fd9fd8c80a62f7899ea65680309655574aeabe2199510`|
|src/model/operation_info_model_cache.cpp|`0f095c7dd252a0a896d9009a38777a5b349752bb94278e9d025f7f6ef18c4271`|
|tests/mdt/test_operation_info_model_cache.cpp|`8d718646279446a5e720cd35878091fe13f9afbceb336308838e9d9e985a5bd6`|

These three files are frozen and ownership released to root. Root records its shared model.hpp hash separately. Canonical defaults/current-state promotion needs a source-owned event and closed creation/callback invalidation coverage; the+160 null cleanup branch, model deletion/guard removal, QObject expiry and accepted Close root remain guarded. This is a static implementation receipt, not a tested candidate archive. No reconstructed executable/deployment was inspected or changed.
