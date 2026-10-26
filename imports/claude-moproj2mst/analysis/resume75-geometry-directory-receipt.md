# CP75 actual companion directory allocation receipt

The companion DB now records the extent of its actual directory allocation at
the two real allocation sites. It distinguishes owner lifetime, allocation
generation, published storage and completed initialization. Geometry retains
its existing nonnull DB-loader guard: this checkpoint supplies the prerequisite
observation, not a directory query or cache-miss success path.

Root assigned the exact seven-file write set listed below after the read-only
CP74 audit. No native model layout, viewer, shared CMake or DB load decision was
changed. This worker ran static preflight only; root owns compilation/testing.

## Source-qualified allocation and retirement order

The native bounds and hashes for the GUI loader/query remain those retained in
resume71-geometry-cache-miss-loader.md, against original GUI SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
This checkpoint adds source bookkeeping only, with no new binary/runtime claim.

| Actual source route | Native state and receipt effect |
|---|---|
| DbHandle constructor in db_load.cpp | After constructing its actual unique reader, bind those two identities and allocate a distinct owner generation. No extent is known. Observer allocation failure is caught and leaves observation unavailable. |
| load_db_info in db_info_read.cpp | Header updates logical uac8 before directory read. Header-only and errors before actual replacement retain the old allocation receipt. On directory-read success, delete old array first; its real delete helper retires the receipt. Convert local nrsets to size_t once, use that same operand for new[] and receipt publication, then mark ready only after record initialization and the existing cleanup tail. |
| prepend_previous_part in db_chain.cpp | Existing part-table/uac8 updates occur first. Capture the actual updated-count conversion used by new[], publish its real result as pending, fill new entries, copy old entries, free words and delete old array, then mark the new allocation ready. Old-array retirement matches pointer identity, so it cannot erase the replacement receipt. |
| delete_dir_records in db_info.cpp | Retire receipts whose current allocation equals the real pointer passed to delete[], immediately before deletion. No count inference. |
| db_info_reset in db_info.cpp | Preserve native signed/nonzero uac8 gate and all native writes. Within the taken branch, actual deletion retires storage and the later pointer-null write clears the binding. With uac8==0, the native function retains uad8; the actual allocation remains observable, without freeing or repairing it. |
| db_info_init in db_info.cpp | Revoke prior allocation observation before the native pointer initialization. Do not free a lost/leaked array as an adapter repair. |
| DbHandle destructor / reader destructor | Revoke the actual owner binding at DbHandle destruction; reader destruction independently retires its identity, covering direct unique-reader replacement. A leaked native array does not preserve a dead owner receipt. |

The full-load count is local nrsets; the multipart count is the updated uac8
*evaluated at allocation*. Later uac8 is never consulted to reconstruct capacity.
uacc (stored records) and uad0 (next DB record) are not capacities. As corrected
in CP74, op_directory's count==0 writeback is not a demonstrated successful
full-load mismatch: full load rejects nrsets<1 before calling it.

Native partial effects remain intact. For example, failed/header-only reload may
leave a new logical count with the old allocation, and failed multipart new[]
may leave updated part metadata/count with the previous pointer. The receipt
does not roll these writes back. It records only storage that actually exists.
It does not validate the DB loader's own raw copies or repair malformed input.

## API, ownership and validity

New header `defmodel/step/db_directory_receipt.hpp` provides an observational
sidecar in `def::model::directory_receipts`. Header-only storage is shared by
the participating translation units of the linked image; defmodel/defdbio and
m2m_core are static libraries in the current converter build. No CMake change
or public C ABI is required.

`Receipt` contains only actual owner/reader/allocation identities, size_t
elementCount, uint64 ownerGeneration and allocationGeneration. These numbers
are receipt identities, not invented native counters. Default construction is
not a qualified observation. A registered owner has no allocation until a real
instrumented new[] publication occurs. A publication is unavailable to queries
until its initialization/copy tail marks it ready. Pointer reuse always obtains
a new allocation generation. A new DbHandle at a reused address obtains a new
owner generation. Generation exhaustion makes observation unavailable rather
than wrapping. Allocation/publish/ready/retire/query functions are noexcept;
only initial registration needs bookkeeping allocation, whose failure is caught.

- `current(actualOwner, actualReader)` returns optional<Receipt> only for a ready
  matching binding and the same live uad8 pointer. It never uses logical uac8.
- `isCurrent(receipt)` first locates a still-registered reader and compares both
  generations/identities/extent, then verifies its current native pointer. An
  expired/unregistered reader is rejected before any dereference.
- `allocationPublished/Ready/Retired`, `clearReaderAllocation`, `bindOwner`,
  `retireOwner` and `retireReader` are instrumentation hooks, not a supported
  external observation-injection channel.

Snapshots do not own the native DB or its allocation. Calls belong to the
existing serialized DB/model thread; this adds no concurrent DB mutation
protocol. Before a future geometry query, acquire the actual strong DbHandle
lease from the live managed Open owner, match GuiModel.db before dereferencing
it, and match its actual owned reader to S.db. Only then request/recheck the
receipt and require S.u740>=0 and S.u740<elementCount. Missing/foreign/replaced
ownership remains guarded. A copied directory, raw shared_ptr constructor,
loaded flag or guessed extent cannot satisfy that boundary.

## Focused tests and next boundary

New `test_db_directory_receipt.cpp` has ten Release-active Testkit cases:

1. Actual DbHandle registration supplies no extent; raw synthetic pointer/count
   and a foreign owner remain unavailable.
2. Real test allocation operand 2 remains extent 2 across different logical
   counts; pending publication is unavailable before ready.
3. Replacing a live 2-entry allocation with 3 entries invalidates the old
   generation; later deletion of the old pointer preserves the new receipt.
4. Actual delete helper and reset retire the corresponding storage.
5. Native uac8==0 reset retains its actual allocation and receipt, without repair.
6. DbHandle destruction and placement reuse through optional::emplace produce
   distinct owner generations and reject the old snapshot.
7. Direct unique-reader replacement retires the original binding and does not
   manufacture an owner/extent for the replacement.
8. Pointer mismatch and native init revoke the current view; fixture-only cleanup
   frees the array that native init deliberately drops without deletion.
9. Real copied COG load creates 32-entry observation; header-only preserves its
   generation, full reload advances allocation generation, and missing-file reset
   retires it.
10. Real copied multipart route prepends a COG copy, publishes 64 entries under
    the same owner generation, rejects the old allocation generation, preserves
    the new one across a no-part return, and retires it on reset.

The real-load tests require configured M2M_DB_FIXTURE/COG_P1_B1.DB and write only
under M2M_TEST_SCRATCH. They use the existing directory/chain algorithms, not a
copied implementation. All cases have a Testkit entry point, checked quoted
includes and supported CHECK/CHECK_EQ/REQUIRE macros; no C assert. Static
preflight also checked diff whitespace, unique case names and the two production
new[] hook pairs. No compiler, build, CTest or fixture mutation was run here.

Root's globbed target is `m2m_mdt_db_directory_receipt`; CTest name
`m2m.mdt.db_directory_receipt`. No hand registration is needed. Recommended focused
group also includes existing DB-load, DB-inheritance and viewer-box targets.

The next geometry slice remains CP74's bounded step_dir_sizes query with actual
S.u740 and version mutation, after actual strong-owner and receipt checks. This
checkpoint neither enables that slice nor claims public flag0/high-version
cache-miss reachability. The MAR/A global lifecycle and every later A/cache/
object-copy guard remain unchanged.

## Exclusive write set

- New source/port/defmodel/include/defmodel/step/db_directory_receipt.hpp
- source/port/defmodel/src/step/db_info.cpp
- source/port/defdbio/src/step/db_info_read.cpp
- source/port/defdbio/src/db_query/db_chain.cpp
- source/apps/moproj2mst/src/engine/db_load.cpp
- New source/apps/moproj2mst/tests/mdt/test_db_directory_receipt.cpp
- This report

Static preflight passed. Frozen SHA-256:

| File | SHA-256 |
|---|---|
| db_directory_receipt.hpp | 24bf727760f8d21240c7da181662331fe867dbca2c4805eab9dc1b0b1f863c15 |
| defmodel/src/step/db_info.cpp | b0ecd4dd467432a4f7b35c75f9b6e6ff39feb200713621f62e84e07977202119 |
| defdbio/src/step/db_info_read.cpp | 512550efd22c460fb08730c069ba53b78aac473ea8d594cd9a5f1425fc03fd13 |
| defdbio/src/db_query/db_chain.cpp | 3523b65484b3a38cb5183826b33a5cfcdb9c80437a6f69e21fb1d0da2cbfbcbf |
| m2m/src/engine/db_load.cpp | 112643cd9277c64d18180b17b5f3c7893ab5234b35d0bdb363d1030aaa0fe705 |
| test_db_directory_receipt.cpp | 4d584ae95664b0989bbd1ea7005d6532041160b4713e748ef609d96e55ee515a |

Viewer and viewer-box test remain unchanged at CP73's frozen hashes
`d7d6d07aa5df2f82d4e2c719005f7c63d87a392c1bf0f088e8f6fe5894defa87`
and `b55b39d02c4be895fca99b733044c2428390ee1b454879ff852d31738b5ccab7`.
Token measurement is unavailable. CP74's report remains a separate read-only
audit snapshot. All seven owned files are released frozen to root.
