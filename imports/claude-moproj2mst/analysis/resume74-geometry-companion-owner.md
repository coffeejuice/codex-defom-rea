# CP74 geometry companion owner and directory extent

Current Open supplies a real companion DB lifetime chain. It does not expose a
reliable directory allocation extent. The existing nonnull companion guard in
viewer.cpp must therefore remain until an allocation receipt is bound to that
actual owner. No viewer-only query admission is justified by `loaded`, `uac8`, or
a nonnull `uad8` alone.

This is a read-only source audit. No source/test/build/GUI/mutable REA action was
performed. The existing CP71 native qualification is retained: original GUI
SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`,
loader `[140828060,1408283d5)`, query `[140825440,140825532)`. Native byte
hashes and capture receipts remain in resume71-geometry-cache-miss-loader.md.

## Actual Open lifetime

- `conversion_session.cpp:199..203` constructs the Project and its ProjectScope.
  `prepareProjectModel:264` creates a shared DbHandle, registers that same model,
  and moves it into `p.dataDb`. `commitProjectModelConstruction` binds its actual
  Project identity. `releaseProjectStorage:419` removes the scope before deleting
  Project storage, preserving a local shared model while clearing its binding.
- `model.hpp:394` owns `dataDb` through shared_ptr. `db_load.cpp:95` creates the
  actual unique reader; `engine.hpp:115` owns it at native DB-handle offset +40.
- `loader.cpp:183` constructs each GuiModel from `projectDb(p)`. GuiModel retains
  only the raw DbHandle pointer (`gui_model.cpp:157`), not a strong owner.
- `multiblow_props.cpp:458..465` implements currentProject/ProjectScope as a
  thread-local borrowed Project pointer. This is safe only within the actual
  live managed scope; it is not an independent Project lease or native QObject
  lifetime proof.

For the bounded managed-Open profile, obtain `currentProject()->dataDb` by strong
copy while that scope is live, compare its identity to GuiModel.db *before*
dereferencing GuiModel.db, and compare its owned reader identity to actual
S.db (S+3390). Keep that lease for the query. A missing scope, another DB handle,
another reader, expired Project storage or an unqualified thread remains guarded.
Do not make a new shared_ptr from the raw model.db/S.db pointer. Project/model
slot lifetime identities are useful checks but do not establish directory extent.

## Why current directory fields do not establish extent

`load_db_info` in `defdbio/src/step/db_info_read.cpp` publishes logical `uac8`
at111 before the family check, directory read and replacement allocation at155.
Header-only return at113 retains the existing allocation. Unsupported family
or directory-read failure also can retain old uad8 after the new logical count
has been written. `dbRefreshIfChanged` in db_load.cpp:248 does not clear loaded
when dbReopen fails; loader.cpp:445 and the operation load path ignore its false
return. Thus loaded=true/new-count/old-allocation is possible in current source.

`db_chain.cpp:198..244` is another allocation writer: multipart prepend changes
part tables and uac8, publishes a new array sized from that updated value, fills
it and copies old entries, then deletes the previous array. A receipt based only
on the initial dbOpen allocation would become stale.

Important qualification: `dbcore::op_directory` at dbops.cpp:347 writes its count
argument only when the input count is zero. Although read_result_sets passes a
count pointer, full load_db_info rejects nrsets<1 at93 before calling it. The
zero-count helper branch is therefore **not** evidence that a successful full
load changes its local positive count. The failed/header-only and multipart
cases above independently establish the extent problem. Capture the actual
new[] operand rather than inferring provenance from later field equality.

`uacc` is the stored-entry count, and `uad0` is the next DB record; neither is
capacity. The layout comment calling uac8 an allocation indicator is not a live
extent guarantee. No allocation count/epoch receipt exists in the audited paths.

## Smallest next prerequisite and bounded mutation

Record an observational sidecar at the real allocation/publication/deletion
sites. It must identify the actual reader, actual pointer, actual allocated
element count and allocation generation, with actual DbHandle owner qualification
at the callback boundary. Cover full load, multipart replacement and reset/
destruction. Preserve old receipts when a failed/header-only read retains the
same allocation. Do not roll back partial native fields or substitute a copied
directory. CP75 traces these sites more precisely.

Once this prerequisite exists, require actual S.u740>=0 and below the live
allocation count, then reuse `dbread::step_dir_sizes` in w2_objarrays.cpp:506.
It reads hsize/+8, mar_size/+c, a_size/+10 and version/+20 from a 0x28-byte
record indexed by **actual S.u740**, ignoring its explicit step argument. Outputs
are locals. Only the conditional S.step_version write is model mutation; it
precedes the caller's version re-gate. It does not initialize S.u1b68/u1b6c.
Retain that partial version write if a later branch remains guarded. A allocation,
zero-length cache publication, disk loading and object assignment remain later.

Managed Open's examined constructor/readDbEntry paths set S.u1b58=1. This audit
does not establish a public transition to flag0/high-version cache miss. The
owner and extent prerequisite is useful independently, but public reachability
of this branch remains conditional.

## Focused regression prerequisites

Test actual-owner admission plus missing scope, foreign DB, mismatched reader,
replaced/expired owner and stale allocation generation. Test actual count versus
larger/smaller logical uac8, header-only and failed reload retaining the old
allocation, multipart replacement and reset. Reject negative/out-of-extent
S.u740 without a directory read. Verify the explicit step argument is ignored,
four local outputs and conditional version update occur in native order, and no
header-length/A/cache/object writes occur at the next guard. Keep current nonnull
viewer pending tests unchanged until the receipt is integrated. Release-active
Testkit assertions are required; root owns builds and CTest.

## Source binding

Paths below are relative to this converter checkout. SHA-256:

| Source | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/db_load.cpp | f87d28001efdbA3fc3fd286077a4ef83c7cb106d8c8495daa6eacbc9d5b35b75 |
| source/apps/moproj2mst/src/engine/conversion_session.cpp | 99731a89662bac4b9162c22f0bbb649da3e65c64a5431c96048060774b4ab8a3 |
| source/apps/moproj2mst/src/engine/loader.cpp | 746c6855de4831c15d8ad04f01df859d1e48a6ee5a1b16fd327c994d0fad9e7c |
| source/port/defdbio/src/step/db_info_read.cpp | a40750756c612f6ae71febf6f25a63e8b40e8b26e88e5d196cb728851e99e3b2 |
| source/port/defdbio/src/db_query/db_chain.cpp | d58f174d3f6fec6782d1368207f9c3fd9ca1b8c7535c907be91aed395609bc51 |
| source/port/defmodel/src/step/db_info.cpp | c46dc4152a786b10343b31ae371759a2eee4723e5ffd52a0f00b8adf8bde1500 |
| source/port/defdbio/src/dbcore/dbops.cpp | 425f2ef23644058962627bcb1f135a30a4a91023a430faa9102f1742bb97fedc |
| source/apps/moproj2mst/src/engine/viewer.cpp | d7d6d07aa5df2f82d4e2c719005f7c63d87a392c1bf0f088e8f6fe5894defa87 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | b55b39d02c4be895fca99b733044c2428390ee1b454879ff852d31738b5ccab7 |
