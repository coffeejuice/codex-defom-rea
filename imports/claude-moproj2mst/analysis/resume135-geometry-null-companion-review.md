# CP135 independent review: null-companion geometry edge path

## Result

No blocking defect found in the frozen null-companion slice. The new path is constrained to the current geometry owner's live same step, null companion, dimension 3, zero +0x1b58, high signed version and cache-zero profile. It shares the admitted no-copy node loop/Stop with the low-record path. Connectivity-null continues to edge-only scene contribution; nonnull connectivity is refused before the repeated getter/builder. CP134 low-record exact-frame retirement remains separate and unchanged.

This is a static code/test review. No build or test was run here, and no original runtime parity is claimed.

## Frozen hashes

Hashes match `analysis/resume135-geometry-null-companion.md`:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `1d483eaf5b674a28639f03cf5f803c793dcc6ba84c4c7ab4663ace0d77482532` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `9cbf647a080936b5cff9c502beabc719119c02c540c2fbc03c0907e488c5cda6` |
| `analysis/resume135-geometry-null-companion.md` | `8e07cf5387c600d0559e63b6fac4957ce699fd0d1814313df9da6a9983976555` |

## Source findings

- The common `completeGeometryNodeSkip` is called from the low-record initial query callback and the null-companion callback. It retains ordered A-index state checks, loads MAR index/stride before caller skips, skips null/type-7/byte-3610 objects, and requires same-step owner plus zero +0x87c for the only admitted node read. It does not form the unused MAR record pointer or allocate/read A/MAR data. The timer must be at count 2 before it calls Stop and returns success.
- The null path runs only after the first coordinate getter and table selection. Inside one synchronous step callback it checks nonnull current step, exact equality with `nodes->u130`, null DB, dimension 3, +0x1b58 zero, signed version at least 6000, and cache[0x0b] zero. Negative arrays-timer count and any helper refusal leave `coordinateReturned` false and return a pending readiness result. This branch does not install, clear, or reuse a low-record barrier and does not query the directory or mutate version/cache.
- On successful Stop the first coordinate prefix is marked complete. Connectivity is queried once. Null connectivity returns the ordinary Skip before the populated builder; nonnull connectivity becomes `HighVersionRepeatedGetterPending`, which `geometryMeshRect` refuses before its explicit second coordinate getter. This preserves the currently unported GUI repeat instead of treating the primary getter's return as sufficient for populated construction.
- CP134 `completeInitialCoordinateRead` remains in the low-record callback only. Its exact installed-frame/current-borrow checks and reset are not routed through the null companion branch. Static call-site search shows the helper remains called once from the initial low-record borrow.

## Test review

The new null-companion matrix covers four current versions, six arrays timer counts and both connectivity states. It checks one first-getter dispatch per refresh, Stop/count3 for nonnegative timer counts, no arrays timer mutation for negative count, accepted edge scene only for null connectivity with completed Stop, unsupported populated/negative-timer cases, and unchanged version/cache/main-array/optional-mesh identities. Its six-profile refusal test checks foreign owner, nonzero node gate, low/high out-of-range MAR selector and true assignment state over two fresh invocations; those paths keep their pending scene result and do not Stop. The shared geometry directory tests continue to cover low-record exact-frame retirement and its incomplete parked profiles.

The test assumptions are narrowly fixture-based: `GuiModel(nullptr, 3)` provides a null companion; the geometry node owner is explicitly set to that model's actual step; and the main object is checked against that same step. Counts/versions are directly configured and the DB companion, version, cache and storage pointers are asserted unchanged. No threaded/raw-concurrent replacement safety is established.

## Validation boundary

No blocker found in the requested checks. Root owns focused/full validation. No source/test changes, build, GUI/original app interaction, or mutable REA operation occurred in this review.
