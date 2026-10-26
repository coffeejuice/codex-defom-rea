# CP124: empty OPI position-list cleanup prefix

Implemented061f70's source-closed zero-count branch on the actual Operation.objpos owner, after the progress's explicit Props/cache-null and empty-object steps. It advances only to RecoveryCleanupBoundary, before0506d5's OPI+20 condition. Nonempty position lists, including null rows, remain intact behind their first removal/deletion guard. No public Close, destructor hook or OPI expiry was added.

Only the exclusive existing cleanup header/source/test and this receipt were written. No shared model, Project/storage, Editor300, Session/main, lifecycle/io, CMake or plan edits. No compile/build/test execution, GUI, original launch, REA import/retry or mutable REA database. Immutable original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Native branch and exact next guard

050690 calls061f70 at0506c8.061f70 calls062070 at061f7e; that count accessor selects OPI+60 and forwards through004ae0 to the previously qualified pure QtCore QListData::size670032c0. Count zero takes061f85 ->061f98 and returns without element access, removal or deletion. Current Operation.objpos is the represented owned+60 row list; an actual empty vector selects this logical branch without releasing its allocated storage. No physical Qt QList equivalence is inferred from the vector representation.

For nonempty positions,061f91 calls061ec0(this,1).061ec0 converts that one-based1 to index0, checks bounds using the count, fetches through0620a0, removes from OPI+60 through066f60 at061f14, then tests the fetched target and invokes typed deleting wrapper065a90(flag1) at061f3f when nonnull. It returns to the outer count loop. Therefore a null row still has a removal effect, and a nonnull row is unlinked before deletion. This patch stops before061f91 and implements neither those container effects nor payload cleanup.

After061f70 returns, the caller reloads OPI at0506cd, then0506d5 tests the **wrapper pointer+20**, followed later by+28 and+160 cleanup. The new stage stops before that condition. An empty recovery vector is not a null recovery wrapper; this prefix inspects neither wrapper and does not delete/null either one.

## Private continuation and preserved ordering

New opt-in `attemptOperationInfoPositionListCleanup(Operation&, progress)`:

1.Validates the same live OPI generation before Operation fields.
2.Requires the actual progress already reached PositionCleanupBoundary; earlier null/empty steps cannot be bypassed just because today's fields happen to be empty.
3.Rechecks Props/cache absence and object-list emptiness before reading the position count.
4.Empty positions advance the diagnostic cursor to RecoveryCleanupBoundary and return EmptyPositionsSkipReached. Position storage, recovery/flags/current/OPI are untouched.
5.Nonempty count enters PositionListEffectsBlocked and returns NonemptyPositionsEffectsUnavailable before removal. Only the count is recorded; no entry pointer/ownership is inspected or retained. A stable retry stays blocked, and a changed count—including external clearing—refuses without manufacturing a native empty skip. Same-count row identities are not certified; there is no bypass continuation past this stage.
6.After the empty skip, reappearing positions or any earlier payload refuse. Stable retries report RecoveryCleanupUnavailable with no recovery condition/effect executed.

The existing object API still reports PositionCleanupUnavailable at PositionCleanupBoundary and never opts into this step automatically. Earlier Props/cache behavior likewise stays unchanged. Once the explicit new step has advanced, prior entries revalidate/report the reached position/recovery guard without replay. Enum entries are appended. A count is the only new metadata; there is no new strong allocation lease, native pointer API or main/application owner.

## Focused tests and preflight

The existing `m2m_mdt_operation_info_cleanup` target now has24 Release-active Testkit cases (19 retained, five added): explicit prerequisite/opt-in order; empty positions retaining reserved storage, recovery wrappers/flags and live current; nonempty/null rows unchanged and external-clear refusal; late positions/objects/cache/Props appearance; and foreign/explicitly expired OPI rejection. The expiry/external changes are declared candidate fixtures, not evidence that original cleanup ran.

Static preflight checked real include resolution, Testkit main/macros, no C assert/REQUIRE_EQ, stage-switch/signature coverage, appended enum ordering and whitespace. No compilation or tests ran here. Root owns the focused and source-stable grouped build/run:

```text
cmake --build <build> --config Release --target m2m_mdt_operation_info_cleanup
ctest --test-dir <build> -R '^m2m_mdt_operation_info_cleanup$' --output-on-failure
```

The native accepted root still requires the application/main/manager producer, fresh busy/modal/nested Save and Project/slot cleanup predecessors. Ordinary nonnull Props or cache stop earlier. This conditional private prefix does not move cleanup into adapter exception handling, commit QObject expiry or claim accepted Close. Recovery wrappers, item-model+160, members, guard destruction and recipients remain guarded.

## Frozen receipt

Edited paths under source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|include/m2m/engine/operation_info_cleanup.hpp|`3d2347a7e263b6df5505173de12e21f0d3a52bb1f55bed074a962fccda3d6374`|
|src/model/operation_info_cleanup.cpp|`f99531f029eeb3f76c1cda40ca3df7c35b3120bed162fd007febcef9da138797`|
|tests/mdt/test_operation_info_cleanup.cpp|`824b68f7e352c5875113b81ae90ddfa8580566d2f0873e47353a9e47889e4aab`|

Bounded offline PE/.pdata/DUMPBIN14.44.35228.0 reads, end-exclusive:140061f70..140061f9d SHA-256 `4dfb9956f4169d27ef24ce98e4bd50233b6b32406c6f5b1f798db597e9cb9499`;140062070..14006208c `8857946cdc59e993a7f54a9bfef85b45b91c322a9797e1bf36133238e5806260`;140061ec0..140061f5b `0992c8669024fe1ca059a1e9eb89050cca31d902456e7f321e53237ce3a35467`.

Retained source qualifications: resume56-opi-cache-140050690.asm; resume122-close-next-prefix.md; resume123-opi-object-list-prefix.md (`0921045988b19760ecfbdf88a993d82ea95b9d7b6069d3e71ab198c32fee13b6`). Current objdata.cpp/newObjpos and model.hpp owner correspondence were also read. The three files are frozen and ownership released for root. This receipt is static preflight, not a tested candidate archive or original accepted-Close witness.
