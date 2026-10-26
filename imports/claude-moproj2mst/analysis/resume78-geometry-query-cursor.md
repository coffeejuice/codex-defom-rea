# CP78 lower-version directory query: persistent cursor and model barrier

Keep CP77's version<6000 pre-query guard until a persistent **model-owned**
barrier is installed. A cursor stored only in a GeometryItem is insufficient:
the query changes shared S.step_version, and refresh then visits other Views.
They could take the low-version fast path in the same refresh, or the original
item could do so on the next refresh. A model barrier must stop both cases and
must survive Viewer replacement or lost provenance without falling through.

The smallest safe successor is a parked cursor after the lower-version query,
with no object-copy continuation. It can preserve the version write and refuse
later work without replaying COW, table selection, timers or the query. Resuming
the remaining native reader requires additional step/mesh lifetime qualification.

This is a read-only design; no source/build/test/GUI/mutable REA action was
performed while CP77 verification runs.

## Native boundary and retained state

CP71 retains original GUI loader `[140828060,1408283d5)`, query
`[140825440,140825532)` and setter `[140825400,140825419)` bounds/hashes against
GUI SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The query reads the actual companion record indexed by S.u740, writes four
locals, then conditionally updates S.step_version. With actual dimension3,
u1b58=0 and the resulting signed version<6000, the recovered14082bf10 gate
returns true. That completes only the DB loader's early return. The caller still
has its arrays-timer checkpoint and model-object copy loop. CP71 explicitly
establishes that a low version does not complete the reader.

The existing viewer prefix before the query has already committed edge sampling,
coordinate key0 COW, actual3D index-table selection and the node reader's Start /
ReadSingleAArrayFromDB checkpoints. A suspended native call has advanced past
these effects. A retry of this **same** frame must not start a new invocation.
This differs from CP77's supported fresh high-version refreshes, which deliberately
repeat a fresh native prefix and remain guarded.

Minimal cursor phase sequence:

| Phase | Meaning and allowed next effect |
|---|---|
| PreparedForQuery | Frame/barrier is fully stored before the newly admitted version write. No lower-version query has run yet. |
| QueryCommitted | Preserve hsize, marSize, arraySize, recordVersion, queried index and post-query actual version. Never requery or restore the old version. |
| LoaderReturnedOldVersion | The recovered gate's true early return has no additional model writes. Next native effect is the actual arrays timer checkpoint, not another COW/query. |
| ArraysTimerPending / ObjectAssignmentPending | A later assigned slice may reuse CP73's exact actual-global timer guard and checkpoint once. No object copy is admitted by this design. |
| ProvenanceChanged | Keep the barrier and captured effects; refuse. Never erase the cursor and reclassify by current version/cache flags. |

The smallest implementation can park at QueryCommitted/LoaderReturnedOldVersion.
It need not add the arrays checkpoint now. The stage is an adapter continuation
marker, not an invented native field or proof that the whole reader returned.

## Exact owner and storage

The pending barrier must belong to the actual GuiModel, whose GuiModelLifetime
already supplies a weak identity guard. It must not be owned solely by Viewer:
GuiModel.viewer is a replaceable public shared_ptr, and a new Viewer could lose
a viewer-local flag while the same actual step retains the lowered version.

Proposed minimal representation is a private forward-declared
`GeometrySyncBarrier` shared_ptr in GuiModel, initially absent, with a complete
definition in viewer.cpp. It owns the single pending frame and has no native
reference-count effects. This requires coordinated engine.hpp/gui_model.cpp
ownership; it is **not** a viewer-only change. Its contents must never be shared
as a different model's pending call. A Viewer may refer to that same model-owned
barrier but cannot create another independent frame for the same suspended call.
An observational registry keyed by raw model pointers is not needed.

Retain only these provenance keys and primitive state:

- GuiModelGuard for the actual owner, plus actual S pointer. The guard detects
  model expiration; S remains a borrow owned by that model's unique_ptr.
- Weak_ptr to the **actual existing** model.viewer control block and the exact
  View provenance. On access, lock the weak Viewer and compare control-block
  identity to model.viewer; pointer equality alone cannot detect address reuse.
- View construction generation, geometry-item ordinal, item binding generation
  and bound geometry index. GeometryItem lives in a growing vector: its raw
  element address is not a stable key. View's unique_ptr address is stable only
  until removal. Generations here are adapter identity receipts, not native
  counters, and must never wrap into an accepted identity.
- Actual object, geometry, mesh, N and E pointer identities; actual node/E step
  owner identities; actual S.u740, id0x0b, flag1, outer which0, dimension3, and
  cache0 at entry. These raw pointers are diagnostic keys, not retained owners.
- Weak_ptr to the real DbHandle obtained from the live currentProject dataDb;
  actual reader identity and CP75 owner/allocation generations, pointer/count
  used for the query. Capture the four returned locals and original/post-query
  S.step_version. Retain a primitive copy of the sampled-edge prefix if needed
  for diagnostics; the live item rectangle already contains it.

Do not store a strong native DB lease across pending calls: relock and revalidate
actual currentProject ownership when a future continuation needs that owner.
Do not retain a CowList copy or add Qt/native ref effects merely to keep N
variables alive; the native frame does not add that ownership. No cloned step,
reader, directory or private default body can replace the actual owners.

Prepare/store the complete barrier before invoking the newly admitted query.
An inline fixed frame or a successfully allocated frame avoids losing the guard
after a version write. If adapter bookkeeping allocation fails, keep the current
pre-query refusal and do not run the lower-version query. Do not roll native
effects back to hide an incomplete frame.

## Barrier placement and same-refresh propagation

The barrier is checked before any new branch decision or native effect at a
geometry/viewer entry. A pending frame wins over current low version, changed
cache marker, missing mesh, empty counts or settings changes. In particular:

1. `refreshGeometryItem` must consult the owning model barrier before
   rectReset/edge resampling and before dereferencing a previously captured
   geometry source. A retry returns the pending qualification with rectKnown
   false, without replaying the prefix.
2. When the first lower-version query publishes its pending stage, propagate a
   suspended result out of bind/refresh. `refresh` currently continues its
   per-View loop after the unknown item result; it must stop before processing
   the next View, retaining the already committed prefix and unknown rectangle.
3. `viewerSync`, `viewerRebuild` and repeated `viewerRefresh` must not start a
   new bind/COW/timer frame while this model is pending. Binding can itself reach
   geometry work before the final refresh, so gating refresh alone is too late.
4. `viewerSceneBox` and any public scene result that could bypass the incomplete
   contribution must retain the existing unsupported continuation gate. A
   replacement Viewer or alternate cached-box/settings route is not permission
   to forget the model's unfinished call.

This interlock affects only the newly admitted lower-query pending profile;
ordinary existing cached/ownerless/low-version paths never create the barrier.
It is a reconstruction guard, not a claim that the native application throws a
new error at those locations. Integration must state this acceptance boundary.

## Invalidation and lifetime limits

- Model destruction releases adapter metadata only. Do not dereference the
  borrowed S/mesh/reader or replay native cleanup from the cursor destructor.
- GuiModel::reset calls viewerClear before step_clear. Do not clear the barrier
  at reset entry or at viewerClear alone. A failed view cleanup retains the
  barrier. Retiring it after a **fully returned, separately qualified** complete
  model reset can establish a new model-content generation; that hook needs
  explicit source qualification before implementation.
- View removal/rebind, Viewer replacement, mesh replacement or owner mismatch
  makes the current cursor nonresumable. Preserve a model-level ProvenanceChanged
  tombstone and public refusal; never drop it into the low-version branch.
- DB receipt retirement/reload after QueryCommitted does not authorize replay.
  The old record has already been consumed into locals. Keep that original
  provenance and stage. Requiring or adopting a fresh directory generation as
  a substitute would silently execute a different call. A later continuation
  must separately identify each remaining native DB borrow.
- Current GuiModel weak lifetime does not detect direct replacement of its
  public step unique_ptr, nor step_clear/mesh deletion and recreation at reused
  addresses. GEO2D_AND_MESH2D owns and deletes its raw mesh; no current mesh/step
  allocation generation receipt closes that ABA. Unknown provenance therefore
  remains parked, even if raw pointers happen to compare equal.
- Index-table globals can be changed by another model. A parked retry must not
  silently reselect3D tables or restore captured globals. Before any future
  model-object copy, qualify actual remaining MAR/A/table owners and appropriate
  noninterleaving/native-call context. Captured addresses do not prove that body.

For a parked-only cursor, no continuation dereferences raw geometry keys, so
missing generation evidence is handled by permanent refusal rather than unsafe
resumption. A resumable cursor needs actual step/content and mesh lifetime
receipts at their mutation sites or an equally complete existing owner protocol.
Those shared native/source owners are a separate dependency, not permission to
add guessed epochs only at the query callback.

## Smallest meaningful regression matrix

| Test group | Required behavior |
|---|---|
| Lower query once | Real owner/receipt, actual versions5999/0/INT32_MIN; first call commits the exact version/query locals and one COW/table/timer prefix. Cache/A/header fields remain untouched; public scene remains refused. |
| Repeated retry | Second/third refresh leaves all native fields, CowList body/refcounts, edge rectangle prefix, table globals and timers byte-identical to post-first state. No second dispatch/query/checkpoint. The model barrier wins over current lowered version. |
| Same-refresh second View | Two actual geometry Views share S. First query lowers its version; later View performs no COW/getter/query/rectangle effects. Model iteration is parked at the first frame. |
| Alternate entry/settings | viewerSync/rebuild/scene queries and changed display flags cannot bypass the same model barrier or start a different frame. Existing no-barrier cached/fast paths retain their results. |
| Provenance changes | Remove/rebind the pending View/item, replace Viewer, change actual S/mesh/N/E identity, expire/replace DB owner or reload its directory. Keep original captured locals and a nonresumable barrier; no raw stale dereference, requery or fast-path success. Include same-address generation reuse where actual receipts exist. |
| External flags/globals | Set cache1, set step version high again, change logical directory count or module table selection after the first park. Retry remains effect-free and does not repair/reselect fields or claim a cached reader return. |
| Lifecycle / preflight | Unknown owner/receipt, invalid index or unrecordable cursor keeps CP77 pre-query refusal with version unchanged. Failed model reset retains barrier. Only an explicitly qualified complete reset/new content generation retires it; model destruction releases metadata without native effects. |

Existing CP77 high-version query tests remain fresh-invocation tests and should
not be silently converted into resumed-frame tests. Keep their public guard and
existing actual-owner/physical-bounds regressions.

## Source dependencies and release recommendation

Relevant source: viewer.cpp GeometryItem/View/Viewer and
refreshGeometryItem641, refresh880, viewerSync935, viewerRebuild952,
viewerClear979 and viewerSceneBox1174; GuiModel::reset at gui_model.cpp:187;
GuiModelGuard at36..46; unique step and replaceable viewer fields in engine.hpp;
GEO2D_AND_MESH2D::delete_mesh at geo2d_and_mesh2d.cpp:93. Line numbers are CP77
source, not immutable native addresses.

Current viewer.cpp SHA-256
`a9a58f67ec863595e55739ddb466f74702d6308de4dce9fe04888d95253a3c14`;
gui_model.cpp SHA-256
`2c740841cadb794fd3bd652dffdc2430269847ad7de46d82137738ba1197f196`.
CP77/75 reports retain query/helper/receipt hashes. No new source edits were made.

Root should keep the lower-version pre-query guard until coordinated ownership
of engine.hpp, gui_model.cpp, viewer.cpp and its focused test is available and
the model barrier/reset policy is reviewed. Viewer-only item state cannot meet
the stated replacement and shared-step guarantees. A parked-only model barrier
is the smallest safe source successor; actual reader continuation remains a
separate evidence lane. Token measurement is unavailable.
