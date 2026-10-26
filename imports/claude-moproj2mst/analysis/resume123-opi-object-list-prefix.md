# CP123: empty OPI object-list cleanup prefix

Extended the existing private OperationInfoCleanupProgress through061180's **zero object-count branch**, after its explicit Props/cache-null steps. It stops before position cleanup0506c8 ->061f70. Nonempty lists, including null rows, remain blocked before the first native element/container effect; no object is removed or deleted. No public Close, destructor hook or QObject expiry was added.

Only exclusive operation_info_cleanup.hpp/.cpp, its focused test and this receipt were written. No Project/storage, shared model.hpp, Editor300, Session/main, CMake or plan edit. No compile/build/test execution, GUI, original launch, REA import/retry or mutable REA database. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. No reconstructed executable/deployment was inspected or changed.

## Closed branch and nonempty partial-effect order

Native050690 calls061180 at0506bb, after Props0512c0 and engine-cache05f980; its next call is061f70 at0506c8.061180 calls061220 at06118e.061220 selects OPI+50 and calls004ae0, which forwards IAT141e04210. The PE import/export pair identifies QtCore QListData::size at **670032c0** (`?size@QListData@@QEBAHXZ`). Its complete body loads the list-data pointer, reads end+0xc, subtracts begin+8, and returns: no allocation, detach, refcount or list mutation.

Zero count at061193 takes061195 ->061207 and returns. Current Operation.objs is the actual represented owned object-row list corresponding to OPI+50; an empty vector selects this logical branch without touching its storage or a pointee. This does not claim full physical Qt QList representation from an STL container.

The nonempty native order matters:

```text
0611a2  first-element accessor14b5b0
0611a7  fetch element pointer
0611bd  removeOne13dffe0 from the owning list
0611c2  test fetched pointer for null
0611f0  nonnull typed deleting wrapper0658a0(flag1)
061205  repeat from count query
```

Thus even a null row is removed before its delete is skipped; a nonnull target is unlinked before deletion. The element accessor/removal's Qt effects and typed payload deletion are not closed here. The new helper refuses **before0611a2**, rather than clearing an all-null vector or moving deletion before removal. No nonempty partial effects are claimed executed.

## Actual owner and progress contract

New opt-in `attemptOperationInfoObjectListCleanup(Operation&, progress)` first validates the same live OPI generation, then requires the progress already completed both earlier null steps. It rechecks Props/cache absence before reading the actual object count.

- ObjectCleanupBoundary plus actual empty objs advances to PositionCleanupBoundary and returns EmptyObjectsSkipReached. No position/recovery/current/OPI owner changes.
- Nonempty count records only that count and enters ObjectListEffectsBlocked, returning NonemptyObjectsEffectsUnavailable. It does not inspect or retain entry pointers. Stable retries remain blocked; a changed count, including external clearing, refuses without turning the list into a successful source empty branch. Same-count membership is not certified, and there is no resume API past this guard.
- After the empty skip, a newly nonempty list or reappearing Props/cache refuses. Stable retries report PositionCleanupUnavailable;061f70 and later recovery/+160/member/QObject work remain pending.

Existing Props/cache APIs keep their earlier opt-in stops. In particular replaying the cache helper at ObjectCleanupBoundary still returns ObjectCleanupUnavailable, even when objs is empty. Only the explicit new method advances this step. Once it has advanced, prior helper entries revalidate/report the reached object/position guard without replaying effects. New enum entries are appended. The only new metadata is a count; no allocation owner, native pointer or completed-Close capability was introduced.

## Focused validation prepared

`m2m_mdt_operation_info_cleanup` now has19 Release-active Testkit cases (14 retained, five added): prerequisite order/opt-in behavior; empty list preserving reserved vector storage, nonempty position list, recovery/current/live OPI; nonempty and null rows unchanged and external-clear refusal; late object/Props/cache appearance; and foreign/explicitly expired OPI rejection. Explicit expiry/external mutations are declared fixtures, not actual native cleanup.

Static preflight checked quoted include resolution, Testkit main and supported CHECK/CHECK_EQ/REQUIRE/TEST_CASE macros, no C assert/REQUIRE_EQ, source signatures/stage dispatch and whitespace. No compile or test ran here. Root owns the focused build and grouped full validation:

```text
cmake --build <build> --config Release --target m2m_mdt_operation_info_cleanup
ctest --test-dir <build> -R '^m2m_mdt_operation_info_cleanup$' --output-on-failure
```

Actual normal Close entry/owner, modal/nested Save and Project/slot/destructor prerequisites remain unresolved as qualified by CP110/120/122. This private branch does not move cleanup into adapter exception handling or infer that a successfully opened operation has null Props/cache or empty objects. Nonnull prior payloads still refuse first.

## Frozen receipt

Source files below are under source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|include/m2m/engine/operation_info_cleanup.hpp|`d7b003c9e49fd6f6199b87c4363f1b457b355942053d35d6cae0dd70eed4bbc1`|
|src/model/operation_info_cleanup.cpp|`2051ea0168fc435c2292c2fd26f7959ef7df30b99eec642fa79d229b45dbebb2`|
|tests/mdt/test_operation_info_cleanup.cpp|`91da6b1b7dc2445a3c7021b3577fa437fd3a16ce19f8f09a501e7f5b52d99d0c`|

Bounded offline PE code spans, end-exclusive, DUMPBIN14.44.35228.0/section-aware mapping:140061180..14006120c SHA-256 `d28c811365d7b65aca1b22abe223fe472c02e7108fb83d9f77555fe4569f7651`;140061220..14006123c `8af0f8290c979b74cda5d00e88d5da509118a8b839de41f441f3cabf0ea272b4`;140004ae0..140004af9 `638c97280f018b2497d6a1c0d2973edbd25ef35038945a2258b0cd3de6f9e9e7`. Qt size export bytes670032c0..670032cc are `48 8b 01 8b 48 0c 2b 48 08 8b c1 c3`. Prior resume122-close-next-prefix.md retains Props/cache qualifications.

The three source/test files are frozen and ownership released for root integration. This is static preflight, not a tested candidate archive or original accepted-Close witness.
