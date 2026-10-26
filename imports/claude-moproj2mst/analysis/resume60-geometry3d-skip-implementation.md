# Known-false ownerless geometry mesh readiness

Source is frozen for root validation. The 3D viewer now preserves its edge
rectangle when the qualified coordinate getter succeeds and the ownerless
connectivity getter returns null. Missing coordinates, nonnull/unqualified
owners and noncanonical node-variable registries retain the existing unknown
scene-box boundary. The populated rectangle builder remains unchanged.

Owner: `gradient_state`. Exclusive writes: `src/engine/viewer.cpp`,
`tests/mdt/test_viewer_box.cpp` and this report. No shared interface, CMake,
build, GUI, original execution or mutable REA changes. Token measurement
unavailable. Root owns compilation, focused/full tests and candidate freeze.

## Controlling evidence

Read `analysis/resume60-geometry3d-next.md` with its checkpoint53/56 source
qualifications. The original DEF_GUI_MO_64.exe SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Its predicate1408ee940 returns false after positive counts if connectivity is
null; caller14126d0c0 then skips builder14124bec0. The rectangle reset and edge
sampling already occurred. A false predicate preserves the edge contribution;
it is not an original unsupported result.

The preceding coordinate getter is safe without a model owner only in the
qualified preallocated-coordinate, key0-skipping registry profile. It still
traverses the state-variable collection and can detach its shared COW handle.
Missing coordinate storage with null dimensions can dereference dims[0]; it is
not generalized into this benign skip. A nonnull element owner introduces a
separate synchronization path and is not admitted by this increment.

## Implementation

`canonicalGeometryNodeVariables` performs a nonmutating preflight using the
existing port collection count/const element access. It requires the canonical
28 registrations in source order, each exact ID and corresponding exact
reconstructed `StateVarNode` or `StateVarNodePost` type. Those classes represent
the two retained native slot2 implementations. Exact RTTI comparison rejects
an overriding derived type even if a broad dynamic_cast to StateVarNode would
succeed. Null entries, id0, wrong IDs, count changes and unknown types remain
unrecovered before any unsafe getter call.

`ownerlessGeometryConnectivitySkip` admits only absent connectivity, existing
coordinates, null nested node owner, null nested element owner and that canonical
registry. It invokes the existing `fun_140220440` coordinate getter first, then
`fun_140220460` connectivity getter. This preserves the key0 collection traversal
and COW detach instead of substituting a raw-pointer success test. No source
array, count, dimension or owner field is replaced.

The helper is called only after both positive count checks. A proven skip
returns from `refreshGeometryItem` after edge processing, leaving
`rectKnown=true`. Width, coordinate values and builder layout/indices are not
consumed on that predicate-false branch. Other cases continue through the
existing populated builder and its existing refusal conditions. In particular,
a populated connectivity array bypasses this new narrow helper profile and
retains the prior builder behavior.

The viewer comments now describe this current boundary and the already recovered
populated 2D-layout branch rather than claiming every populated mesh is refused.

## Release-active focused tests

Six cases were added; `test_viewer_box.cpp` now contains12 unique test cases.
Every new verification uses `testkit::CHECK`/`REQUIRE`; the file contains no
C `assert` calls, so these checks remain active in Release builds.

1. Positive-count, preallocated canonical coordinates and null connectivity give
   known edge-only bounds x[-2,2], y0,z0. Array pointers, counts and null owners
   remain unchanged.
2. The same predicate-false case with NaN coordinates, width0 and dimension flags3
   still yields known edge bounds, demonstrating builder checks are not reached.
3. Missing coordinates, nonnull node owner and nonnull element owner each retain
   the unsupported scene-box boundary; no opaque owner is dereferenced.
4. id0, null variable, an unknown overriding StateVarNode subtype and a changed
   registry count remain guarded. The unknown subtype's callback counter stays0.
5. A COW lease forces the reached coordinate getter to detach the registered
   variable pointer list. Old/new list bodies each retain one reference; actual
   StateVar pointers, IDs, current/previous fields and value-vector bodies remain
   unchanged. The lease owns list storage, not the variable pointees.
6. Populated->skip->populated refresh changes y extent1->0->1 while edge x bounds
   remain[-2,2]. This detects reuse of a stale populated mesh rectangle.

Scoped fixture restorers restore detached coordinate/connectivity pointers,
owners, dimensions/width and altered registry entries before the mesh's existing
destructors run. Connectivity is temporarily unlinked, not freed/reallocated or
deleted twice. The original six corpus/bounds/empty/populated/malformed cases
remain present.

## Checks, frozen hashes and limits

Direct trailing-whitespace, unique test-name and Release-assert checks passed;
`git diff --check` passed. No executable was built or run by this worker.
Root should build/run `m2m_mdt_viewer_box` (CTest `m2m.mdt.viewer_box`) followed by
the required candidate regression gate.

| File under `source/apps/moproj2mst/` | SHA-256 |
|---|---|
| `src/engine/viewer.cpp` | `8fc663a1a0c2a33fc70552dbebad916f1d6d7954430ca702d90395418decbdf3` |
| `tests/mdt/test_viewer_box.cpp` | `e4e2a61689f157a8b905217753745716117066c67eede1d3486e9ebc6722c941` |

This closes a bounded static predicate outcome. It supplies no new original
runtime/populated-project observation, no arbitrary owner-backed getter recovery,
no allocation-failure parity and no public lifecycle completion claim. The
source goal and independent acceptance gates remain unchanged.
