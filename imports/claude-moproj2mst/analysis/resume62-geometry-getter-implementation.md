# CP62 populated geometry getter fidelity

The qualified populated ownerless geometry mesh now executes the original
readiness getters and the second getter round before rectangle construction.
This preserves native collection COW effects while retaining the existing
rectangle result and the missing-coordinate/unknown-owner getter guards.

Only `source/apps/moproj2mst/src/engine/viewer.cpp`,
`source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` and this report changed in
this implementation lane. No shared model interface, CMake, build, CTest, GUI or
mutable REA action was performed. Source is frozen for root integration.

## Evidence and implemented order

The source qualifications and immutable PE/code/capture hashes are retained in
`analysis/resume62-geometry-owner.md`, including the three bounded CP62 asm
captures. The original target is `DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Let M be the actual optional geometry mesh, N=M+8 and E=M+148. Positive counts
precede readiness. The native predicate `1408ee940` calls coordinate getter
`140514f00` then connectivity getter `140894330`. On true, caller
`14126d296..14126d305` reads node count, element count and width, calls the same
coordinate then connectivity getters again, and passes their returned arrays
to `14124bec0`. The recovered display-child construction remains outside this
rectangle slice.

`ownerlessGeometryReadiness` now distinguishes unqualified, known skip and
populated states. Its preflight still requires existing coordinates N+8, null
N+130 and E+f0 owners, and the exact canonical28 registry IDs and Node/Post
method types. The actual ported getters perform the first round. Null returned
connectivity retains the CP60 edge-only early return. A populated result reaches
`geometryMeshRect`, which reads count/count/width, performs the second real
getter round and uses those returned arrays in all builder checks and reads.

The coordinate getter's key0 dispatcher obtains mutable collection slots.
Native `14086e340` calls detach `1408f2550`, whose unsigned refs>1 branch calls
`1408f27f0`. Thus the first getter detaches a shared list even though canonical
nonzero-ID variable methods skip key0 dimension/value resizing; the second
getter retains the now-unique body. Both rounds use the same actual nested N/E,
without synthesizing model dimensions or attaching an owner.

Missing coordinates remain preflight-rejected because native allocation would
read null dims[0] before any later null predicate. Unknown owners and custom,
id0, absent or noncanonical variables receive no newly admitted getter calls.
The existing populated raw-builder fallback on an unqualified profile is
unchanged and is not claimed as qualified original getter readiness. Existing
builder layout, width, product/index and finite-value guards remain.

## Focused tests and preflight

The viewer test now has14 unique Testkit cases (two new). A scoped existing
`llobj::ext::g_hook` observer forwards every observed call to its real default
dispatcher with the hook disabled and restored by RAII. It records this actual
node set's key0 collection dispatch; it supplies no fake success result. Hook
scope ends before fixture cleanup, and COW leases own only list storage.

- New populated/shared case requires exactly two n4/null-dims/key0 dispatches:
  first old-to-new collection body, then new-to-same body. It checks refs,
  unchanged variable identities/IDs/current/previous/value bodies, unchanged
  arrays/owners and the populated scene bounds.
- New populated/unique case requires two dispatches on each of two refreshes,
  preserving the unique body, array identities, owner fields and bounds.
- Strengthened CP60 shared-list skip requires exactly one dispatch and the
  existing COW detach/edge-only result.
- Strengthened absent-coordinate/unknown-owner and malformed-registry skip
  matrices require no collection dispatch. Existing malformed-builder and
  populated-to-skip-to-populated coverage remains.

Static preflight verified the new quoted/angle include paths, supported
CHECK/REQUIRE macros, the `testkit::run_all()` entry point,14 unique case names,
no C assert/unsupported REQUIRE_EQ and no trailing whitespace. `git diff
--check` returned0. No compilation or test execution is claimed here.

Root validation target: **`m2m_mdt_viewer_box`**;
CTest name: **`m2m.mdt.viewer_box`**. Root owns focused and full source-stable
validation. The real-project optional-mesh population/original comparison gate
remains unchanged; these checks establish internal getter/COW fidelity only.

## Frozen source SHA-256

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `ec88fb2967d3fcc05998df2eaaf11b8b58a2a4701771250138f1e88699efa1ea` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `766a12aad9982e55c782ec6e7f4b484c0542c08ae076d1f5140fcd980b0a042b` |

Token measurement unavailable. No speed or token-saving claim is made.
