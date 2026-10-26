# CP161 Stopping owned directory-query prefix implementation

The Stopping cache0 branch now admits the actual companion directory query for a bounded physical record whose signed version is at least6000. It retains the helper's conditional step-version update and then returns the existing `CoordinateSync` refusal. This is a query-only prefix; loader continuation, arrays timer, assignment, positive getter completion and public Save admission remain unrecovered.

Exclusive source ownership was limited to `source/apps/moproj2mst/src/engine/simcontrol300.cpp` and `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp`. This implementation report is the only additional write. No build, test executable, GUI, live REA, mutable analysis DB, installed program or original project was run or modified by this lane. Source is released on hold for root's independent review, focused integration group, complete stable suite and freeze; no further source edits will occur without root reassignment. Token measurement is unavailable.

## Evidence and implementation order

Read the accepted `resume161-editor-db-next.md`, independent `resume161-editor-db-plan-review.md`, CP74 companion owner audit, CP75 real allocation receipt and CP77 query-only binding/retry qualifications. Read the retained CP159 loader assembly's zero-initialized output locals and the actual existing `step_dir_sizes` implementation/declaration. No new native behavioral claim is made. The retained native qualification is original DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; this lane did not inspect a reconstructed executable or deployment.

The actual canonical dispatch/COW, dimension-table selection, valid MAR/id bounds, cache0 check and two CP159 node-timer samples remain ahead of new admission. The added block then:

1. Reads the existing live serialized `currentProject()` scope and strongly copies its existing `dataDb`. It does not call `projectDb` or manufacture an owner.
2. Compares that lease's handle identity with raw `model.db` before accessing through the raw identity. Obtains the reader through the strong lease and compares it with actual `S.db`.
3. Requests the real ready `directory_receipts::current`, rejects signed negative `S.u740` before conversion, compares with physical `elementCount` and rechecks `isCurrent` immediately before record admission. No callback/reload/event processing occurs between those checks and the query.
4. Reads only the bounded record's signed version for the >=6000 preflight. Older versions retain the old high S version so fresh retries cannot escape through the low-version fast path.
5. Calls actual `def::dbio::dbread::step_dir_sizes(S,reader,S.u740,&hsize,&marSize,&aSize,&recordVersion)` with all four local outputs initialized to zero. Its version mutation survives; no header/MAR/A/cache/coordinate writes or rollback were added. The block immediately refuses `CoordinateSync`.

## Focused test source

Six added Testkit cases use an actual `Project`, `ProjectScope`, `projectDb` handle and real `load_db_info` producer against a disposable COG DB copy under `M2M_TEST_SCRATCH`. They never publish, inject or fabricate an allocation receipt. Directory edits are controlled regression inputs only.

- First/last physical entries0/31, old versions6000/11000 and record versions6000/11000/INT32_MAX retain the exact resulting S version. Fresh page invocations repeat the getter normally and acquire the production path's current receipt.
- Record versions5999/0/-1/INT32_MIN preserve the old high S version across two fresh page invocations.
- Signed indices-1/INT32_MIN/physical32/INT32_MAX refuse; logical33/physical32 rejects32 and logical1/physical32 permits31.
- Missing scope/dataDb, expired actual owner, live foreign handle, opaque model.db/S.db, replaced unregistered reader, null/mismatched directory pointer, actual reset/retirement, null companion, registered never-loaded owner, null reader and a real foreign companion reader all retain refusal. Borrowed pointer sentinels restore before actual owner teardown.
- Full reload and real multipart setup invalidate the old receipt and provide fresh generations, including physical record63 after the actual64-entry prepend. Header-only reload retains the old receipt; a real edited-copy directory-read failure commits logical33 while retaining physical32, and Stopping still rejects32 while admitting31.

The forwarding dispatch trace now records version/table/timer state before and after the real canonical dispatch, proving that its COW occurs before table selection, node timer and query effects. Each new outcome checks one getter/dispatch per fresh invocation; cache0; unchanged header/MAR/A pointers, lengths and body metadata; bit-identical stored coordinates; unchanged coordinate buffer contents; untouched arrays timer; unchanged later page text; and the still-false public completeness flag. Existing cached1/-1, invalid tables, noncanonical owner/type, private constructor and CP159 timer tests remain in the same file.

An in-flight pending allocation cannot be obtained through the actual synchronous producer without adding a callback/hook or fabricating publication. This lane deliberately does neither. The consumer's no-ready-allocation refusal is covered by a registered actual never-loaded owner; CP75's existing receipt target independently covers pending-publication internals. No new consumer-specific native runtime or public Open reachability claim follows from these controlled fixtures.

## Static preflight and root validation queue

Testkit `TEST_CASE`, `CHECK`, `CHECK_EQ`, `CHECK_BITS`, `REQUIRE` and `int main(){ return testkit::run_all(); }` are present and Release-active; there is no C assert or unsupported REQUIRE_EQ. The new quoted `../support/test_paths.hpp` resolves relative to the test source directory. Direct declaring headers resolve under existing public module include directories: `defdbio/dbread/w2.hpp`, `defdbio/db_query.hpp`, `defdbio/step/db_info_read.hpp` and `defmodel/step/db_directory_receipt.hpp`. `m2m_core` links the existing public port dependency chain and this test links `m2m_core`/`testkit`; no shared build/interface change is needed.

Root should run target `m2m_mdt_simcontrol300_stopping_cached` / CTest `m2m.mdt.simcontrol300_stopping_cached` alongside changed focused targets and supporting DB-directory/query/viewer regressions before the complete source-stable suite/freeze. This source-only lane has not compiled or executed those checks.

## Released SHA-256

| Path | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300.cpp` | `dc56989070aa3a934580eb4ab3c4e5230c956a6d3747fd102a23ccbb3fe4c829` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp` | `c3168dba07b1cff6ce6f87aa3d219f6d50b690acc6a77e95126e886ede3fa82d` |
| unchanged `source/port/defmodel/include/defmodel/step/db_directory_receipt.hpp` | `24bf727760f8d21240c7da181662331fe867dbca2c4805eab9dc1b0b1f863c15` |
| unchanged direct query declaration `source/port/defdbio/include/defdbio/dbread/w2.hpp` | `ac5ada504545294321a4a8d0446093d50d19778205f9c54ab72297fcc9f787a7` |

Public Open-to-300-InitialFill reachability, lower-record-version durable continuation, nonempty loader body/diagnostic/exception behavior and full reader assignment remain outside this implemented cut.
