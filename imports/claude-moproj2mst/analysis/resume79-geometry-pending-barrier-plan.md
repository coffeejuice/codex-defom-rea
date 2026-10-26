# CP79 implementation plan: parked lower-version geometry query

The first architecture increment now admits a lower-version directory query
exactly once, then parks the actual GuiModel until its unresolved reader is
recovered. It neither resumes object assignment nor retires the cursor on a
Viewer change, cache/version change or incomplete reset. This closes the retry
bypass without guessing step/mesh lifetime generations.

This report began as a shared architecture proposal. Root approved the exact
source lane after CP78's freeze; the implementation/static release below records
the result. No compiler/build, test execution, GUI or mutable REA action was
performed by this worker.

## Exact exclusive write set

| File | Planned change |
|---|---|
| source/apps/moproj2mst/include/m2m/engine.hpp | Forward-declare GeometrySyncBarrier and GeometrySyncAccess. Add GeometrySyncAccess as GuiModel friend and a private shared_ptr<GeometrySyncBarrier> geometrySyncBarrier_. No native DEF model layout or C ABI change. |
| source/apps/moproj2mst/src/engine/gui_model.cpp | GuiModel::reset checks the private nonnull barrier before any native reset effect and refuses this newly pending profile. No retirement hook until a complete reset/abort is independently qualified. Shared_ptr destruction needs no complete barrier definition and performs metadata cleanup only. |
| source/apps/moproj2mst/src/engine/viewer.cpp | Define the private parked frame and access bridge; install it before the lower-version query write; gate subsequent viewer work and public scene acceptance; propagate a newly parked result out of bind/refresh loops. Keep CP77's ordinary high-version query path unchanged. |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | One composite real-owner/two-View regression, plus minimal test-helper parameterization for a second object. Existing38 cases remain intact. |
| analysis/resume79-geometry-pending-barrier-plan.md | Final implementation receipt, static preflight, source hashes and validation limits if root approves the source lane. |

No receipt helper, db_load, native step/mesh class, CMake, build runner or public
constructor activation is part of this increment. Coordinate the shared
engine.hpp/gui_model.cpp files with root; a viewer-only substitute is insufficient.

## Representation and access

Use the actual GuiModel's private nullable shared_ptr as the durable barrier.
Its empty state means no admitted lower-query frame has been installed. The
barrier object is not shared with another GuiModel. The friend bridge is defined
only in viewer.cpp and reads/installs that field; no new public pending-state API
is needed for this first increment. Tests can observe actual version changes,
native effect counts, cached refusal and retained view count through existing
interfaces.

The complete barrier owns a fixed-size frame with:

- Stage PreparedForQuery or QueryCommitted/Parked; the first increment has no
  successful/cleared/resumed stage.
- Actual GuiModelGuard, borrowed actual S identity, weak_ptr to the actual
  existing Viewer control block, actual View identity, item ordinal and bound
  geometry index. Require the View to be in that actual Viewer's current list
  before initial admission. Raw GeometryItem addresses are unstable when its
  vector grows and are not a cursor key.
- Diagnostic object/geometry/mesh/N/E identities and actual entry id0x0b,
  outer which0, flag1, dimension3, S.u740/cache0/old version. These are observed
  values, not replacement owners or authorization to dereference them later.
- Weak actual DbHandle owner, actual reader identity and the CP75 receipt's
  owner/allocation generations, pointer and elementCount used for the query.
- Four local query outputs and post-query version. No copied native bodies,
  CowList lease, added Qt references, default DB data or rollback snapshot.

No View/item/step/mesh generation is fabricated at the query callback. Because
the first increment never resumes native work from a parked frame, an unknown
or changed identity remains blocked without dereferencing captured raw keys.
Later resumability needs the real lifetime/mutation receipts listed in CP78.

## Admission and commit order

1. Execute the existing qualified geometry prefix in native order: sampled edge
   rectangle, actual coordinate COW,3D tables and two node-reader checkpoints.
2. Reuse every CP77 owner/reader/ready-receipt/physical-index check. Unknown
   provenance or invalid index retains the current pre-query refusal. Do not
   create a model/DB owner to satisfy the checks.
3. For record version>=6000, keep the current fresh-invocation CP77 query and
   nonnull pending return, without creating this new parked barrier.
4. For signed record version<6000, fully prepare a frame and install it on the
   actual model **before** calling step_dir_sizes. If adapter frame allocation
   fails, retain the current pre-query guard and leave S.version unchanged;
   earlier native prefix effects are retained. No rollback or native allocator
   behavior is claimed by that bookkeeping failure.
5. Execute step_dir_sizes exactly once into the frame's four locals, preserving
   its conditional actual S.step_version write. Mark QueryCommitted/Parked.
   Native14082bf10's true old-version return has no additional model mutation;
   do not execute arrays timer or model-object copy in this increment.
6. Retain sampled item prefix and unknown contribution. Stop caller iteration
   immediately, before later Views or further native bind/refresh effects.

Use a mutable pointer to the actual guard-resolved GuiModel when installing
the private barrier; do not const_cast a different ownership context. No helper
can complete or remove this frame just because the post-query version is low.

## Barrier invariants and all relevant entries

The durable barrier is tested **before** classifying current version/cache/mesh
state. It remains effective if model.viewer is null, replaced or aliases a
different model's Viewer. A weak Viewer mismatch does not erase model-owned
state. Same-address reuse or unknown provenance cannot turn pending into success.

- `viewerRefresh` is the supported poll/retry entry: when parked it returns
  without COW, query, table selection, timer/checkpoint, edge sampling, rectangle
  resets or another native invocation. Retain the incomplete qualification.
- Common `viewerOf` and the public scene/box acceptance route must reject a
  parked model before accepting a cached result or starting a different native
  operation. This also gates existing-barrier viewerSync/rebuild and the other
  viewer mutators that pass through viewerOf. Pure diagnostic item-count reads
  may remain available.
- `refreshGeometryItem` checks before source dereference/rectReset. On the first
  newly parked result, `refresh` exits its View loop before the next View and
  before the caller unions an unreturned item's result as complete.
- Bind can reach geometry before refresh: propagate new pending state through
  bind3D -> bind -> viewerSync/viewerRebuild. Stop geometry loops and later
  caller effects. A guard only at the final refresh is insufficient.
- `viewerClear` does not use viewerOf and needs its own pre-effect pending
  guard. GuiModel::reset needs the explicit guard above it as well. They must
  refuse rather than silently return as if native clear/reset completed.
- Model destruction releases the observational frame only. No borrowed object
  dereference, query replay, native cleanup or version restoration occurs in its
  destructor. Existing nonpending reset/clear behavior stays unchanged.

This intentionally leaves reset/clear of the newly admitted unfinished profile
unsupported. Clearing at reset entry, at viewerClear alone, or on a pointer
mismatch would lose the only durable continuation gate. A future reset/abort
slice may retire the barrier only after its complete source-qualified lifecycle
has returned. No such retirement is included in this patch.

CP77 cases remain fresh native invocations; do not globally suppress their COW/
timer behavior. Only the new lower-query branch creates the parked barrier.
Likewise, an externally set cache1 or restored high version does not complete an
already parked reader. Globals changed externally are neither restored nor
reselected by a retry. The barrier is a reconstruction guard, not a claim that
the native application throws a new error at the adapter's refusal sites.

## Minimal incremental regression

Add one Testkit case using a disposable COG copy, actual ProjectScope/dataDb and
the existing actual directory receipt. Build two geometry Views on the same
actual S while its ordinary fast path is active, then arm the controlled flag0,
cache0/high-version fixture. Both Views use canonical/preallocated geometry and
actual S ownership. Set the queried actual in-range record version to5999.

The case has five sequential assertions:

1. Hold CowList test leases for both Views so detach is observable. First refresh
   lowers actual S.version once, performs one first-View COW and two node-reader
   entry checkpoints, leaves A/header/cache/arrays timer unchanged, and remains
   publicly unsupported. The second View's body/refcount and cached contribution
   remain untouched: same-refresh iteration stopped.
2. Snapshot the post-first native fields, rectangle prefix where observable,
   table globals, both CowList bodies/refcounts and timers. Second/third refresh
   is byte/effect stable: no new dispatch, query, resampling or checkpoint.
3. Externally restore a high S.version and set cache1. Another refresh still
   parks, retaining those externally supplied values without starting a cached
   successful call. Public scene acceptance remains refused.
4. Retain the original Viewer externally, replace model.viewer with another
   model's existing Viewer (or null it), and repeat poll/scene access. The
   model-owned barrier persists, foreign model state is untouched, and no
   captured raw source is dereferenced.
5. viewerClear and GuiModel::reset refuse before their native effects; actual
   step/view storage remains present. Normal destruction of this fixture and
   its metadata must not replay effects or access stale diagnostic pointers.

This single composite case verifies the new architecture, rather than copying
the query implementation. Existing CP77 physical bounds, missing/foreign/expired
owner and low-version pre-query refusal tests must be updated narrowly: the
qualified lower-record cases now park with a first version write, whereas all
unqualified owner/index cases still stop before the query. Keep version0 and
INT32_MIN as additional rows of that existing qualified test if it remains
readable; their parked retries must be stable too. No public geometry completion
case is added.

Static preflight must check Testkit entry/macros/includes and exclusive-file
hashes. Root runs m2m_mdt_viewer_box plus affected GuiModel/reset/viewer/Slice
owner targets as one focused group before full validation/freeze. No worker
build or competing source mutation is authorized by this plan.

## Approval boundary

This is the concrete shared patch proposal for root review. The exact new
behavior is one lower-query version commit followed by a durable parked model;
reader object copying, lifecycle retirement and public success remain guarded.
The implementation should not start until root assigns the shared files after
the CP78 archive. Token measurement is unavailable.

## Approved implementation and static release

Root approved and assigned the four source/test files after CP78's archive.
engine.hpp now forward-declares the opaque frame/access bridge and stores one
private model-owned shared_ptr. gui_model.cpp refuses reset before any native
effect while it is nonnull. viewer.cpp defines a fixed metadata frame with weak
Viewer/DB owners, actual borrowed provenance, physical receipt, query outputs
and sampled-edge prefix. The complete frame is installed before the real lower
query; its version mutation is retained and its stage becomes QueryCommitted.
There is deliberately no retirement/resume path.

The normal high-record-version CP77 path retains its fresh-invocation behavior.
Unknown owner/reader/receipt/index, or missing actual View/item membership,
retains the pre-query refusal. Frame allocation failure also refuses before the
new version write. No native fields/default DB bodies or public success were
introduced.

New pending state propagates from refreshGeometryItem through bind3D/bind and
viewerSync/rebuild, and exits refresh before the next View or numeric union of
the suspended item's unreturned result. The helper now receives the actual
caller model and refuses mismatched View-owner context. Existing parked retries
return at refresh entry before every effect; common viewerOf/public scene and
box access refuse. viewerClear and reset refuse before their effects. Existing
StepData property-table/write/mutable-view accessors that bypass viewerOf also
check the barrier before their native or output effects. Read-only diagnostics
remain available. Model destruction only releases metadata; no captured borrow
is dereferenced from a frame destructor.

The focused viewer target now has39 Testkit cases. Its existing qualified low
record-version rows5999/0/-1/INT32_MIN now assert one committed version change
and no second COW dispatch on the parked retry. The38-case CP77 high-record,
unknown-owner, physical-bound and reload/multipart paths otherwise retain their
expectations. One additional composite two-View case checks:

- First lower query commits5999 after the actual first-View COW/table/node-timer
  prefix; second-View CowList body/refcounts remain untouched in the same refresh.
- Repeated refreshes keep the full actual DB_STEP_DATA bytes, all five actual
  timers, index-table globals and both CowList body/refcounts stable.
- Sync/rebuild/box acceptance cannot start a different call; externally set
  version11000/cache1 and dimension-table marker2 are not repaired/reselected.
- Replacing the public Viewer with another model's existing Viewer or null does
  not lose the model barrier. Foreign settings/view count remain unchanged.
- The actual Project dataDb owner expires after its last strong reference is
  released, proving the frame retains only a weak DB owner. Polls still refuse
  without touching the dangling diagnostic companion identity.
- viewerClear and GuiModel::reset refuse before removing views or clearing the
  actual step. Model/fixture destruction performs no cursor replay.

The test helper was minimally parameterized to create a second actual object.
All DB inputs are the existing configured fixture copied beneath
M2M_TEST_SCRATCH. This worker did not execute those tests or mutate a DB fixture.

Static preflight passed:39 unique Testkit cases, existing main, supported
Release-active macros, no C assert/REQUIRE_EQ, quoted/new library includes
resolved, all private helper call sites updated, no frame retirement path, and
diff whitespace clean. Root owns the grouped build, independent review,
focused/full tests and archive; this is not a passing-test claim.

Frozen SHA-256:

| Released file | SHA-256 |
|---|---|
| source/apps/moproj2mst/include/m2m/engine.hpp | a63af54f21807744f80a9660d6ba0169fb0d450522413c7a7d2be5fab0f797b1 |
| source/apps/moproj2mst/src/engine/gui_model.cpp | 26768d26df546b8335a48f2374b5743c4a4b60242ff61631326652e8fdba0d45 |
| source/apps/moproj2mst/src/engine/viewer.cpp | 2ed5180c116e66ca7c1d320d4a4c2262edca0eb17cac1c713b68739acc3fae0a |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | f91b986a0a0c1a6badd5ae866025fbb2e03a36bdb42a5feacf4e9e48cb65138d |

Target `m2m_mdt_viewer_box`, CTest `m2m.mdt.viewer_box`. Shared header/reset
changes also need root's existing affected GuiModel/viewer/Slice focused group.
The four source/test files and this report are released frozen. No other
production/header/CMake/receipt file changed. Public lower-version geometry
completion, actual reader copying and cursor retirement remain guarded.

Root's first grouped compile found a missing direct declaration include in the
new composite test: viewerDisplayOptionSnapshot belongs to m2m/engine_scene.hpp.
The worker added that direct include and verified the declaration at line48.
At that include-only repair, production hashes were unchanged. Repeated static
preflight passed for39 cases, macros, resolved quoted
includes/direct declaration and whitespace. No worker build/test was run; root's
rerun remains required. This corrects a missed declaration dependency in the
original static preflight rather than claiming that preflight proved compilation.

## Independent-review repair: built-in GuiModel entry bypasses

After root reported a successful grouped compile of the include-corrected
candidate, independent review found that several GuiModel mutators could modify
the step before a later viewer guard, or never enter the viewer. Root released
source for the expanded entry repair before the candidate freeze. The hash table
above now identifies that **repaired** release, which needs root's new validation.

A private const requireGeometrySyncAvailable helper now runs as the first
statement in all13 reviewed entries: reset, objectWrap, appendObject, readKey,
readMaterial, loadObjectKey, loadObjectKeyGeometry,
beginPickingConstructionPrefix, simCtl, object, objectAt, material and setPdie.
This blocks both native mutation and fresh mutable pointer access before any
effect. No public method signature changes. objectCount remains value-only;
pdie reads a const sim-control value directly rather than invoking the newly
gated mutable simCtl accessor. Destructor teardown remains unchanged, and its
clearAuxiliaryDrawings/displayForget paths were checked not to call these gates.

The39-case test file adds focused assertions within the existing composite
regression. A real disposable KEY copy first succeeds on a nonpending control
model, whose ordinary object/sim-control access, existing objectWrap and PDIE
setter remain usable. For the parked model, all12 expanded entries are required
to refuse. Full actual step bytes, model tables/state, primary-die value, object/
view counts, Viewer identity, picking-construction pointer, all five native timers
and index globals remain unchanged. The picking-pointer check detects a refusal
that occurs only after allocating a picking prefix. The original viewerSync
check now uses an object pointer captured before parking, so it still reaches
the viewer gate rather than stopping first in the new object accessor gate.

Public raw fields and pointers obtained before parking cannot be intercepted.
These entry guards do not claim to prevent every external C++ write. Such writes,
including the test's changed version/cache/Viewer and expired DB owner, never
retire the private barrier or authorize reader/public continuation. No guessed
native content generation or automatic reset was added.

Static preflight passed again: all13 methods' first statements were audited,
value-only pdie confirmed,39 unique Testkit cases/macros/includes/main checked,
the direct snapshot declaration and existing KEY fixture resolved, and whitespace
clean. viewer.cpp is unchanged by this repair; engine.hpp, gui_model.cpp and the
focused test have the new hashes above. Source is released frozen to root for
review/rebuild. No worker compiler/build/test run occurred.
