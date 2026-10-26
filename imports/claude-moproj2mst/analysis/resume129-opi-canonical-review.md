# CP129 independent review: canonical OPI model-cache observation

## Finding

The frozen canonical cache is correctly limited to an `Unknown` observation. `Operation` declares its `OperationInfoIdentity` before the cache and exposes the cache through a const-only accessor. The canonical cache cannot be promoted by the detached diagnostic receipt API, including after an explicit `const_cast`; a detached `KnownNull` instance does not alter or substitute for the canonical member. I found no premature native `+0x160` cleanup or null-state claim in this increment.

## Field order and lifetime

In `source/apps/moproj2mst/include/m2m/model.hpp`, `operationInfo_` is declared immediately before `operationInfoModelCache_`, both before the represented OPI payload fields. C++ reverse member destruction therefore destroys the later payload fields first, then the cache metadata, then `operationInfo_`. The cache's `OperationInfoGuard` is only a shared liveness-metadata snapshot: its destructor drops its shared pointer; it does not call Qt `removeGuard`, delete an `SfMoItemModel`, or invoke the native `+0x160` member destructor. The final `OperationInfoIdentity` expiry remains an adapter identity-lifetime event after those represented members, not the earlier native QObject boundary.

That distinction agrees with retained original evidence in `analysis/resume128-opi-cache-owner-audit.md`: native OPI payload cleanup runs before the `+0x160` member guard removal at `0x140050478`, and the outer QObject boundary occurs later. This patch does not implement those actions or claim accepted Close progress. `OperationInfoModelCache::lookup` refuses while state is Unknown, and its diagnostic KnownNull behavior remains confined to detached cache instances.

## Access and promotion checks

The accessor returns `const OperationInfoModelCache&` from both mutable and const `Operation`; the static assertions and tests verify those types and prohibit calling the mutator through the accessor result. `commitNativeDefaultNull` first validates the live OPI identity and then rejects `this == &operation.operationInfoModelCache()` before it copies a receipt or changes `knowledge_`. The regression deliberately removes const qualification and verifies that the canonical cache remains Unknown. A detached diagnostic attached to the same live Operation can reach KnownNull, but canonical lookup still refuses.

The new tests also verify stable member address, Unknown after candidate `operationDefaults`/`initOperation`, no cleanup-progress advancement on Unknown lookup, foreign/expired OPI refusal, and a new Unknown cache on explicit address reuse. The test explicitly avoids dereferencing a canonical member reference after Operation destruction. These are candidate metadata/guard fixtures; they do not instantiate the native model, commit a native defaults event, or claim a runtime null pointer.

The pre-existing public `commitOperationInfoQObjectBoundary` API remains a diagnostic identity-expiry operation and appears only in explicit test fixtures in the inspected call-site scan. The new cache neither calls it nor re-labels its result as native cleanup. Its existence does not turn the current Unknown-only cache into `KnownNull` or authorize cleanup.

## Qualification

This review is static and read-only. No build, tests, GUI, original runtime, or mutable REA operation was performed. The canonical member is not a completed native constructor/defaults event. Native `+0x160` state remains Unknown across current code paths, and the public Close/cleanup guards remain necessary.

## Source identities checked

- `source/apps/moproj2mst/include/m2m/model.hpp` (root-owned): `BAF657F01683196D78A45CBB180273CFA8D10B57C0523110171FE4FE235E078D`
- `source/apps/moproj2mst/include/m2m/engine/operation_info_model_cache.hpp`: `BD87DC12903FF0A5CA1FD9FD8C80A62F7899EA65680309655574AEABE2199510`
- `source/apps/moproj2mst/src/model/operation_info_model_cache.cpp`: `0F095C7DD252A0A896D9009A38777A5B349752BB94278E9D025F7F6EF18C4271`
- `source/apps/moproj2mst/tests/mdt/test_operation_info_model_cache.cpp`: `8D718646279446A5E720CD35878091FE13F9AFBCEB336308838E9D9E985A5BD6`
- source receipt: `analysis/resume129-opi-canonical-unknown.md`
