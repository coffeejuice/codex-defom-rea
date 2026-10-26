# CP77 nonnull geometry directory-query binding

CP75's real allocation receipt now supports a bounded directory query after the
callback retains the actual DbHandle and checks the actual step index against
that allocation. The integration still returns the existing nonnull DB-loader
pending result immediately after the query. To keep that public guard stable
across retries without adding a persistent cursor, it admits only records whose
signed version is >=6000. Older directory versions need a separate continuation
design because the query itself changes the next refresh's branch conditions.

This began as a read-only investigation during CP76 validation. After CP76's
archive, root assigned viewer.cpp and test_viewer_box.cpp exclusively. The
implemented bounded prefix and static release are recorded below. No compiler,
build, test execution, GUI or mutable REA action was performed by this worker.

## Actual callback and ownership boundary

`viewer.cpp:494 qualifiedGeometryReadiness` already requires canonical node
variables and actual N/E owner identities. With actual N owner, flag u1b58=0,
actual dimension3 and signed step_version>=6000, it executes real key0 COW and
3D table selection. Valid MAR/id bounds and cache marker0 then execute the two
actual node-reader timer checkpoints. The current nonnull S.db guard at534 is
the proposed query boundary. All earlier effects must remain in this order.

View.ownerModel is a GuiModelGuard, whose weak lifetime resolves the actual
model (`gui_model.cpp:36..46`); it is not a strong DbHandle owner. GuiModel.db
is raw. Do not dereference that raw DB pointer to seek an owner or reader.

The minimal lifetime-safe owner binding for the already live managed-Open scope
is:

1. Resolve the existing View model guard and retain its already checked actual
   step identity. No substitute S or copied model.
2. Read currentProject() while its actual ProjectScope is live; if absent,
   retain the existing nonnull pending result. Copy `project->dataDb` to a local
   shared_ptr **before dereferencing GuiModel.db**. Require it nonnull and
   `dbLease.get()==model->db` by identity comparison.
3. Obtain `dbLease->reader.get()` through that strong owner. Require it nonnull
   and exactly equal to actual `model->step->db`. Only then dereference it.
4. Hold dbLease through every receipt check, record read and helper call. Do not
   create shared_ptr from a raw pointer, copy a directory, call projectDb to
   manufacture a missing owner, or use loaded as a lifetime/extent predicate.

This derives from real newModel(Project&,dim) in loader.cpp:183, which obtains
the same Project dataDb through projectDb; Project owns that shared pointer and
DbHandle owns its unique reader. ConversionSession creates ProjectScope while
Project is live, and releaseProjectStorage removes the scope before deleting
Project (`conversion_session.cpp:419..432`). The scope is a thread-local borrow,
not an independent Project lifetime token. Calls must remain on the existing
serialized live model thread, with no concurrent owner/reader replacement.

This slice supports the actual Project dataDb owner route only. A real but
different SubProject.ownDb, missing scope, foreign Project dataDb, opaque model.db,
foreign S.db or a directly replaced/unregistered reader remains guarded. Mo slot
project/model identities can further restrict a native-Open admission profile;
they do not replace the actual strong DB lease or allocation receipt. No new
Mo/Project ownership claim follows merely from passing the minimal memory-owner
checks above.

## Physical allocation bounds and read order

Request `directory_receipts::current(dbLease.get(), *reader)`. Require a present
ready receipt with the actual owner/reader/pointer identities. Its owner and
allocation generations distinguish address reuse; it is observational and does
not own native memory. `isCurrent(receipt)` rejects a retired binding before
dereferencing a reader and rechecks the live native pointer.

Use signed actual S.u740, not a caller-supplied step, operation ID, directory
step number or uac8. Reject index<0; only then convert to size_t and require
index<receipt.elementCount. A ready receipt already identifies the actual typed
new[] allocation. An int32 index times the 0x28-byte record size fits the x64
pointer-offset range. No extra bound derived from logical uac8 is justified.
In particular, CP76's logical33/physical32 state must reject index32, while a
logical1/physical32 fixture can safely query actual index31 if other conditions
match; the original query does not consult logical count.

On this serialized, callback-free prefix, retain/recheck the receipt immediately
before the record-version preflight and query. No DB mutation/reload, event
processing or unknown callback belongs between those checks and the read.

Reuse `def::dbio::dbread::step_dir_sizes` (w2_objarrays.cpp:506; declaration
defdbio/dbread/w2.hpp). The previously qualified GUI query140825440 reads
record+8/+c/+10/+20 into local hsize, mar_size, a_size and version. It conditionally
writes actual S.step_version through setter140825400 **after those local reads**.
It neither writes S.u1b68/u1b6c nor allocates header/MAR/A bodies. Its explicit
step argument is ignored; the body indexes actual S.u740. Do not invoke the
whole source read_single_a_array_from_file: its DEF_PRE names_up_to_date fold
does not implement the GUI gate.

## Retry-safe smallest integrated subset

The existing entry branch tests `S.step_version>=6000`. A query that changes
11000 to5999 and then returns pending would leave a later viewerRefresh able to
take the already implemented low-version fast path. Keeping only the current
pending return on the first invocation therefore does **not** preserve the
public guard across retries.

For a query-only slice with no persistent frame, preflight the safely bounded
record's signed version and keep the existing pre-query guard when it is<6000.
This ordinary-memory preflight is an adapter admission check, not an extra
native state effect or a claim that the low-version native route is recovered.
For record version>=6000, call the existing query with four initialized locals,
retain its actual conditional version mutation, and immediately return the same
HighVersionDbLoaderPending result. Do not execute the loader's re-gate, header
diagnostic, A allocation, cache publication, arrays timer or object assignment.
The repeated invocation still enters the guarded high-version path and repeats
the appropriate fresh native-call prefix; no getter, COW or timer is replayed
within a single invocation. This is partial source fidelity, not full loading.

Alternative: a persistent pending call frame can retain the lower-version query
effect and prevent a new invocation from bypassing its unresolved outer reader.
That requires new lifecycle/cursor ownership and is larger than the proposed
viewer-only slice. Never roll the version back to avoid this dependency.

## Exact focused regressions required before integration

- Use a real Project/ProjectScope and actual projectDb owner, load a disposable
  COG copy through load_db_info to obtain an actual 32-entry receipt, construct
  GuiModel with that same handle and bind actual S.db. Use the existing qualified
  geometry node/E fixture; controlled flag0/cache0 state is a regression fixture,
  not proof that public Open reaches that state.
- Query a valid record at index0 and31; choose original S version6000 against
  actual record version11000 to prove the setter effect. Same-version retry
  stays unsupported, with cache0, A/MAR pointers and header lengths unchanged;
  arrays timer unchanged; first COW dispatch precedes table/node-timer/query
  effects. Only the single existing coordinate-prefix dispatch occurs.
- Missing Project scope, null/foreign dataDb, opaque model.db and foreign S.db
  remain pending before any raw DB dereference/query. Expired actual owner and
  unique-reader replacement must not resurrect an old receipt.
- Negative S.u740, index==extent and INT32_MAX retain the guard with version
  unchanged. Logical count33 does not admit index32 into physical32; logical
  count1 does not falsely reject valid physical index31. No count fabrication.
- Header-only/failed-reload retention uses the same old receipt; actual reload
  and multipart replacement obtain fresh current receipts. Do not cache an old
  receipt across separate native callback invocations.
- Record versions5999,0 and INT32_MIN must retain the pre-query guard and old
  version across repeated refreshes. Versions6000,11000 and INT32_MAX may execute
  the bounded query but remain publicly unsupported on each refresh. Assert no
  A allocation, cache1, arrays-timer checkpoint or geometry rectangle acceptance.
- Preserve existing null-companion timer/object-assignment guard, cached-marker
  normal return, known ownerless/low-version fast paths and opaque-owner tests.

The assigned source set is viewer.cpp plus its focused viewer-box test; new
includes are defdbio/dbread/w2.hpp and defmodel/step/db_directory_receipt.hpp.
Neither native layout nor receipt implementation nor CMake needs modification.
Any stronger Mo slot admission would also need its existing header and explicit
source-profile tests; none is claimed by this slice.

## Evidence binding before implementation

Original GUI SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Native loader/query/setter intervals and byte hashes are retained in CP71;
CP67 qualifies14082bf10's signed6000 gate; CP75/76 qualify actual receipt
publication and the logical-count mismatch. No new native capture was required.

| Current source | SHA-256 |
|---|---|
| viewer.cpp | d7d6d07aa5df2f82d4e2c719005f7c63d87a392c1bf0f088e8f6fe5894defa87 |
| conversion_session.cpp | 99731a89662bac4b9162c22f0bbb649da3e65c64a5431c96048060774b4ab8a3 |
| gui_model.cpp | 2c740841cadb794fd3bd652dffdc2430269847ad7de46d82137738ba1197f196 |
| w2_objarrays.cpp | c567ec0c3391507f438bfe66f5121aa25dab577f40592d5a84164d4133152359 |
| db_directory_receipt.hpp | 24bf727760f8d21240c7da181662331fe867dbca2c4805eab9dc1b0b1f863c15 |

## Implemented slice and static release

viewer.cpp's existing nonnull branch now obtains currentProject()->dataDb by
strong copy, compares raw handle identity before dereferencing it, checks owned
reader identity against actual S.db, requests the ready receipt, checks signed
S.u740 against physical elementCount and rechecks the receipt. A safely bounded
record version<6000 returns before the query. Otherwise step_dir_sizes executes
its local outputs and conditional version setter; the branch immediately returns
HighVersionDbLoaderPending. No loader re-gate, header diagnostic, A allocation,
cache publication, arrays timer or object assignment was added. No null/cached/
ownerless/low-step-version branch or native layout was changed.

Six new Testkit cases bring viewer-box to38 cases:

- Actual owner/receipt at first/last valid indexes, record versions6000/11000/
  INT32_MAX, and both populated/null connectivity. Two refreshes each retain
  public refusal; actual version changes are asserted.
- Signed record versions5999/0/-1/INT32_MIN retain the pre-query guard and old
  step version over repeated refreshes.
- Negative, extent-equal and INT32_MAX indexes refuse; logical33 does not expand
  actual32, and logical1 still permits physical index31.
- Eight owner/receipt refusal profiles: missing scope, missing Project dataDb
  with a separate live lease, expired owner, foreign Project dataDb, opaque
  model.db, opaque S.db, replaced unregistered reader and native-pointer mismatch.
  Raw identity mismatches must never dereference the opaque/expired values.
- Real full reload and real multipart replacement use fresh current receipts;
  querying new physical index63 proves the old32 receipt is not cached.
- Actual disposable-file failed reload publishes logical33, returns3 and keeps
  physical32. Query index32 refuses, while index31 still executes the bounded
  version mutation under the retained allocation generation.

The common regression checker forwards the real COW dispatch and verifies that
step version, index-table dimension and node timer remain unchanged *during*
that dispatch, before the later query effect. It verifies first-call detach and
second-call reuse, exactly one coordinate-prefix dispatch per invocation, public
refusal on both invocations, and no header/body/cache/connectivity/arrays-timer
writes. Existing standalone step_dir_sizes_and_version coverage already verifies
its ignored explicit step argument and four local outputs; it was not duplicated.

Only disposable COG copies beneath M2M_TEST_SCRATCH are written. Directory
receipts arise from actual loader allocations, without test-injected capacity or
copied reader ownership. Controlled flag0/cache0 and directory record contents
are regression inputs, not claims that managed Open reaches those states.

Static preflight passed:38 unique cases, Testkit main, Release-active supported
macros, no C assert/REQUIRE_EQ, resolvable quoted/new library includes, and diff
whitespace. No build or CTest was run. The unchanged registered target is
`m2m_mdt_viewer_box`, CTest `m2m.mdt.viewer_box`; new fixture-based cases require
the existing configured COG DB. Root owns focused/full validation and archive.

Frozen SHA-256:

| Released file | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | a9a58f67ec863595e55739ddb466f74702d6308de4dce9fe04888d95253a3c14 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | cbf4e4c15f913eabaf3c0800e2f17df11bf19992924125bae387e290f6e1207a |

The two source/test files and this report are released frozen. Receipt helpers,
shared headers, CMake and every other production file remain unchanged. Public
nonnull cache-miss completion remains guarded. Token measurement is unavailable.
