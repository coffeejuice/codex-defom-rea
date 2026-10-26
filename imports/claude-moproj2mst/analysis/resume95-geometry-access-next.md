# CP95 ownership boundary and nonparked directory-query borrow

The retained frame pointer cannot be converted into a normal withStep borrow.
The next useful source increment is instead the ordinary high-record-version
directory-query invocation, which executes before any barrier is installed.
It can use a synchronous void callback with existing scalar outputs and actual
leased reader. This report freezes that bounded plan before the source edit
root has now authorized.

## Retained frame identity: current behavior and boundary

GeometrySyncBarrier::step is const DB_STEP_DATA*. Full source search finds one
assignment, `frame->step = model->step.get()`, and no subsequent read, compare
or dereference. The other raw geometry identities are likewise observational
keys for a suspended call. GeometrySyncAccess::pending/requireAvailable and
GuiModel::requireGeometrySyncAvailable check only private barrier presence;
install refuses an already present barrier. They do not dereference recorded
step or validate it to authorize continuation. Destructor teardown releases
adapter metadata. There is no frame retirement/resume API.

Thus a legacy external native-step replacement can leave stale recorded
identity, but it cannot make the model unparked or authorize a later native
dereference through that frame. Existing tests alter version/cache/global
tables, Viewer and owner provenance and still require persistent refusal.
This does not provide interception for public raw fields/preheld pointers.

Do not assign a withStep nativeStep into frame->step: that escapes its
synchronous contract. Replacing the pointer type or adding an ad hoc owner
would not establish native allocation lifetime or ABA/generation safety. Any
future continuation needs an explicit model/step observer/identity contract,
actual allocation provenance and invalidation/retirement rules. Current
GuiModelGuard proves model lifetime, not independent lifetime of its replaceable
unique_ptr step. Preserve current historical metadata without widening it.

Scalar-only fields remain: oldVersion and entryCache are copied before install;
postVersion is copied afterward. The pre-install copies could use void
callbacks, but they are write-only frame metadata with no direct observable
assertion, so changing them alone adds little value. More critically, invoking
ordinary withStep for the postVersion read or query after install self-refuses:
its required model preflight sees the newly installed barrier. Supporting
that path requires a distinct prepared-call/continuation capability, not
moving installation after the native version write or bypassing the guard.

## Exact useful source cut and timing

Write only viewer.cpp's ordinary nonparked `step_dir_sizes` call and this
report. Existing viewer_box tests remain unchanged. No frame, helper/header,
native query/layout, CMake, ABI or observer API changes.

Current owner/profile/COW/table/cache/timer/companion/strong DB lease/reader/
receipt/index guards execute first. The code reads the actual directory record
version. If it is<6000, it builds/installs a model barrier, invokes the query,
records post-version/stage and returns. Leave that entire branch untouched.
Only the subsequent query statement is selected; reaching it proves the low
record branch did not install a frame and return. Its locals hsize, marSize,
arraySize and version are already initialized immediately before the call.

Replace that one statement at its existing position with:

```cpp
GuiModelAccess::withStep(*model,
    [reader, index, &hsize, &marSize, &arraySize, &version](DB_STEP_DATA* nativeStep) {
        (void)def::dbio::dbread::step_dir_sizes(nativeStep, reader, index,
            &hsize, &marSize, &arraySize, &version);
    });
```

The lambda returns void and calls the same native helper exactly once. Only
the existing reader/index and output scalar references are captured; no step
pointer is returned/saved. The actual Project.dataDb shared lease remains in
scope through this synchronous call; reader capture neither invents ownership
nor widens its lifetime. Preserve the ignored uint64 query return, all output
addresses and the following HighVersionDbLoaderPending return. Do not move
receipt validation, record gate, query, native version write or public guards.

## Native/source qualification and tests

CP71 qualifies GUI query0x140825440..0x140825532, native-byte SHA-256
`4525b4f95fe70043fd1c70d20acced398afb237fa0b8331a12d0da131cd00ac3`,
for original DEF_GUI_MO_64 SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The existing source counterpart w2_objarrays.cpp:506, @src1401cc1a0, reads the
actual S.u740 record and fields+8/+c/+10/+20 into the four outputs, then
conditionally sets actual S.step_version. It ignores explicit index argument,
allocates nothing, changes no MAR/A body, and invokes no event callback. CP77
qualifies the live DB lease/receipt/physical bounds and keeps high-record query
pending behavior retry-stable. CP78/79 explain why low records install a
barrier before their version mutation.

Use unchanged m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box, currently45 cases.
`geometry_nonnull_directory_query_keeps_high_versions_pending_with_actual_owner`
crosses actual record versions6000/11000/INT32_MAX, indices0/31 and populated/
null connectivity (12 rows), deliberately differing entry model versions and
requiring the actual conditional version write plus continued pending result.
Bounds, logical-versus-physical count, failed reload, multipart/current receipt
and forged/expired owner cases cover the same preceding guards. Low-record
and actual parked retry fixtures remain unchanged and prove the excluded
installed-frame path did not gain a public continuation.

These are meaningful native mutation/owner-boundary tests already present;
duplicating them or adding an accessor for private write-only metadata is
unnecessary. Root owns builds/runtime and complete acceptance.

## Static, layout and ABI gates

Reverse only the ordinary query replacement to require the full input
viewer.cpp hash. Confirm exactly one query call in the new void callback,
identical seven argument values/identities and ignored result, explicit captures,
no nativeStep escape, and same local initialization/call/pending-return order.
Require the low-record frame assignment/install/query/postVersion/stage block
to remain byte-for-byte unchanged. Do not claim the whole function raw-free.

Helper/model/public headers and C ABI implementation stay unchanged. Retain
void/bool and rejected pointer/reference compile gates, native layout offsets/
size, GuiModel step type/nonpolymorphism, APIversion1 and six C signatures.
Root retains source-stable build/tests/export/ABI/archive gates. No successful
cache-miss loader/object-copy continuation, frame retirement or new owner API.

## Starting SHA-256 bindings

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | aee911933a4d05fb4dbf7ddb98f7c5dde6147fdd500e1434a9ad6e5baa286b49 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| analysis/resume77-geometry-directory-query-binding.md | 5b0664d813ab0ddc97b17b942074fd2cf4e22264b7431917965bf247c13f2e27 |
| analysis/resume78-geometry-query-cursor.md | 586378b2b903ef6de4dfe99285598c7fb9824f31b75b42ffcc1d2ed8e53ad4b1 |
| analysis/resume79-geometry-pending-barrier-plan.md | b34dae42d845a033be9ba8ce3ec1329cf520980ef149d82c5be4fbb91b464f82 |
| source/port/defdbio/src/dbread/w2_objarrays.cpp | c567ec0c3391507f438bfe66f5121aa25dab577f40592d5a84164d4133152359 |

Worker performed no source/build/CTest/GUI/mutable REA action before plan freeze.

## Authorized implementation and static release

Plan freeze SHA-256:
`e6ed5de72e1914d7f27e4d0860660565d9487e47aa5188f224e41069e9d6d1b0`.
Root authorized the ordinary high-record nonparked query cut. That single
invocation now executes through the existing void withStep callback after all
actual owner/receipt/bounds/record guards and output-local initialization. Its
explicit captures are reader/index values and references to the four original
scalar outputs; all seven native argument values/identities and ignored query
result are preserved. The borrow ends before the same pending return.

Static preflight passed: exactly one native query call in the selected callback;
no returned/saved nativeStep; same call/local/pending timing; reverse-only-call
replacement reconstructs the complete CP94 viewer.cpp SHA-256. Balanced block
comparison separately verifies the whole low-record frame preparation/install/
query/postVersion/stage/return branch is unchanged and contains no ordinary
withStep after installation. All frame fields/observations remain untouched.

Existing45-case test and helper/model/public C ABI sources are byte-for-byte
unchanged. Root owns build/runtime acceptance with existing12-row high-record
version-mutation matrix and owner/receipt/bounds/reload/parked regressions.

Final SHA-256 receipt:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 7274e7772e6dc0c6dbf02a9a0811ee77e65bf9b683e474831cab54f3f9797f75 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp (unchanged) | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source is frozen/released for root's m2m_mdt_viewer_box / CTest
m2m.mdt.viewer_box and grouped source-stable validation. Worker performed no
build, CTest, GUI or mutable REA action.
