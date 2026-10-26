# CP160: eager locks and the default PTD release cut

The synchronous source-free owner remains the publication prerequisite. This pass closes two smaller static gaps in CP158: **errno-provider locks12/13 belong to eager startup storage**, and **the retained-default PTD initializer/callback reference changes are balanced without a dynamic-locale destructor**. It also identifies an exact failure distinction: initial PTD registration failure does not explicitly free its fresh PTD; lazy registration failure does. These contracts are source-ready components of a future module owner. There is no source-ready body/slot-publication increment through the current owners.

Only this report was written. No source/build/test, GUI/app control, target execution, live REA retry/import or mutable analysis database was used. The reverse-engineer-anything skill was applied using bounded offline DUMPBIN14.44.35228.0 and the existing section-aware `tools/doe_static_pe.py`. Original DEF_GUI_MO_64 **v11.0.2**, `evidence/original-binaries/DEF_GUI_MO_64.exe`, independently rehashed SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Preferred-image VAs are not live pointer authority. New findings below are static observations; runtime branch frequency, current defaults and ownership binding remain unobserved.

The RGEOM queue in `analysis/takeover-state.json` still selects CP159's test-only actual CowVector body32-allocation throw regression and marks validation incomplete. CP157's pending B is insertion-local and unpublished. CP158's clone FH3 result was read, not repeated; this pass addresses the free/errno owner downstream of that cleanup.

## Eager lock startup and its exact failure cut

`14147f9cc` iterates36 records of stride16 in `[143b5e750,143b5e990)`. A record is `{pointer:qword, kind:dword, padding:dword}`. Initial kind1 indices are0,1,3,4,6,7,8,10,12,13,14,16,17,18. Other records are initially kind0; all pointers startnull. Each kind1 receives the next40-byte storage address from1446fff20, **publishes its pointer before initialization**, and calls `141481cb0(pointer,4000)`.

Thus locks12/13 are initial kind1, assigned storage144700060/144700088 respectively under this initial record order. Lock10, used to serialize dynamic lock creation, is eager too. These arithmetic addresses are static storage roles, not recovered initialized critical sections. After successful startup and absent later table mutation/teardown, PTD initialization/callback entry to locks12/13 uses the existing pointer and EnterCriticalSection directly. It does not reach the dynamic40-byte allocation path. This removes lazy lock acquisition from that bounded initialized-owner route; it does not establish an owned lock table in the current converter.

`141481cb0` invokes imported InitializeCriticalSectionAndSpinCount. Its `.xdata` scope is `[141481cb4,141481cc0)`, filter141bf24a1, handler141481cc0. The filter returns1 **only for exception C0000017**; handler sets LastError8 and returns0. Other exception codes return0 from the filter and continue searching. An ordinary API false return also reaches wrapper return0. Do not translate every exception into a recoverable false result.

On wrapper0, startup `14147fa47..54` nulls **only the failed record's published pointer** and returns0. Outer `141481708` then calls module cleanup14148140c and returns0 before FlsAlloc/publication. Cleanup first conditionally FlsFree's a published index, writesFFFFFFFF, then tail-calls lock cleanup14147fa58. Its first pass deletes/frees nonnull kind!=1 dynamic locks and nulls their pointers only after source-free returns; its second pass deletes nonnull kind1 locks without freeing static storage or clearing their pointers. Therefore earlier successful eager locks are deleted after a later eager-init failure. There is no invented source-free of static40-byte storage and no blanket table reset. The dynamic pass's free can itself require the synchronous errno owner; this pass has not erased that dependency.

## Initial registration failure differs from lazy registration failure

At startup, after successful locks/FlsAlloc, request1*712 obtains raw PTD inRBX. `141481754` calls FlsSetValue. Raw0 branches at175c to177c, calls14148140c, then returns0. The decoded caller tail and module cleanup contain **no explicit source-free of that fresh RBX allocation** and no callback call receiving RBX. Do not add RAII reclamation to this native failure cut merely because it seems desirable. This is a bounded static absence claim, not a captured resource leak or a statement about unobserved OS internals.

By contrast, lazy provider `1414814e8` explicitly calls source-free at154f on its fresh PTD when FlsSetValue fails, sets its local PTDnull, restores its saved LastError and returnsnull. Its cleanup can reenter provider on HeapFree0. Startup and lazy failure must be separate transitions; one generalized failed-registration cleanup would change at least one of them.

## Default-only initializer/callback reference graph

The leaf retain helper `[14147c760,14147c7f5)` atomically increments locale+0, then each nonnull pointed counter at+110/+120/+118/+130. For six records at locale+48+32*i, it conditionally increments the counter at+58+32*i when its associated pointer differs from sentinel143b5f540, and the counter at+60+32*i when the associated+50+32*i pointer is nonnull. It finally increments `[locale->qword(+158)+160]`. The decrement leaf `[14147c7f8,14147c89d)` performs corresponding decrements and returns the locale pointer; it does not free these objects itself.

Retained initial default slot143b5f6b0 points at143b5f550. That locale's refcount starts1; all four optional counters and all six pairs of record-counter pointers are null; its+158 points at143b5e340, whose+160 counter starts0. For a newly zeroed PTD initialized with RDX0 and these default identities, initialization contributes+1 to default+B8 counter143b5f000, default locale refcount and nested counter. An untouched PTD's callback contributes the matching-1 to each. With retained-initial counts and no other contributors the values0,1,0 become1,2,1 and return0,1,0; a live module with other PTDs need not have those absolute counts. B8's static-default identity excludes storage free even when its count reaches0. C0's identity equal to the current default slot excludes dynamic destructor14147c5e4; the second immortal identity143b5f550 excludes it too.

This closes the **retained-default, no-intervening-PTD-mutation** reference route, not all locale ownership. Current defaults are writable, thread activity may populate optional PTD buffers, and a nondefault C0 can invoke14147c5e4 after decrement to0. Its dynamic destruction remains a distinct dependency. Even this bounded untouched PTD callback ends by source-freeing PTD at1414816b5: raw0 still requires the complete provider/mapper/store/reentry owner. Reference balancing alone supplies no synchronous release backend.

## Decision and smallest missing witness

Keep `BeforeBodyPublication` at140832adc, before first-store140832ae9 and slot-store1409e486f. No T nonnull free, leaf completion, later receiver, Stop or retirement is admitted. ModuleHeapRole still explicitly has no free/handler surface; it retains T/B separately. Current host CowVector destruction cannot own this source payload. No source body32 backend or publication change is proposed by this report.

The next useful static owner work is now precisely divided: preserve the lock-init failure/cleanup and initial-versus-lazy registration rules above; recover dynamic locale destructor14147c5e4 only if admitting a nondefault-locale PTD profile. Do not repeat clone FH3 or another successful B8 allocation as a substitute.

For the smallest **original runtime ownership witness**, use one naturally reached errno-provider call on the conversion thread, without inducing invalid frees/OOM. Bind module hash/base/generation, successful source lock initialization (especially actual12/13 pointers), FlsAlloc callback141481590/index publication, and the same thread/fiber's actual FlsGetValue PTD. At provider entry/exit retain saved/restored LastError and exact returned errno address. If claiming the bounded default route, also bind current default slot, actualPTD+B8/C0 and the nested+158 identity/counters, then the natural callback and finalPTD free result before lock/index teardown. A nonnull fast lookup alone needs no lock/default mutation, but does not prove their startup/callback ownership. This is a future evidence request, not permission to acquire GUI control or a live session here.

A successful witness still cannot validate a natural HeapFree0 branch. That separate same-run witness must retain failed rawBOOL, failed LastError continuity, actual errno destination, live mapper input/rows/output/store and caller return; historical parked failures cannot be promoted. Full implementation must own the synchronous branch graph, including lazy-registration cleanup reentry, before first publication. Original parity, complete lifecycle and efficiency improvement remain unclaimed.

## Bounded immutable receipts

Ranges are half-open; hashes cover original PE bytes, not DUMPBIN output. Runtime-unwind bounds were used for nonleaf functions; locale leaves were decoded through their actual RET and exclude alignment. The truncated preliminary unlock/locale windows were discarded and complete aligned selections re-read. DUMPBIN executable SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`. Import-descriptor decoding binds141e03158/3160/3168 to Enter/Leave/DeleteCriticalSection and141e03458 to InitializeCriticalSectionAndSpinCount.

| Range | SHA-256 |
| --- | --- |
| `14147f9cc..14147fa56` | `9210a3010271446363dbf469d17bb259fc1b380b2e5519d669530876045afbcb` |
| `14147fa58..14147fadb` | `9439edd7d8092070105bc9654b321620ec953e57f909cc8154142767fe0f3219` |
| `14147fadc..14147faf4` | `43b7759b2dc9729fb5374e7b6278a01b6685ce081880065c92f5a1670a11e391` |
| `14147faf4..14147fbdc` | `cb11d33f933ecc7fd1ee25f674fd9ab7fe47acc2fb68fcb3fb1f244b1725481d` |
| `14147fbdc..14147fc20` | `be3c3068324a3c4139fee257769a18818376e2c18bf55a8dea2585e6f4d02172` |
| `141481cb0..141481cdd` | `2425196805d2a9500aca7a870ddb61e3a9334061793baa8d12045726e0fdb40c` |
| `141bf24a1..141bf24c2` | `9e1ce77c6f72e34d4c18472772206107f6bf8382d7a047b8b88de24f44377fbb` |
| `14347cdf8..14347ce18` | `43f6c6231c5879c9222e1de703675cb6116359180b43d08b27573b99233243c9` |
| `14148140c..141481431` | `4140787b4d9aad759a7972a53a265fad031beb40e6013d47675c9784649e1f80` |
| `141481708..141481789` | `2e60e5d74214a1cea2203d3f419be2bcc1a2434e8c0e69a15750a091631dda49` |
| `14147c760..14147c7f5` | `512ebc94476915160cce80745288ea650a5c85f32a879f9b970e81ee692ac140` |
| `14147c7f8..14147c89d` | `2852674895e5f55ed77a55228eefab17d0e1bee1edf1efc3fdfa649197855711` |
| `143b5e750..143b5e990` | `88cdd74da9e0b3b5b48d1d5a98b71a803b7c5f89d85ef1d6ed4bb7a44db2b27b` |
| `143b5f550..143b5f6b8` | `3ce0bf19baacac21359400be1fa60d717cbc84a2b0c323f21c856075845e14cd` |
| `143b5e4a0..143b5e4a4` | `df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119` |
| `143b5f000..143b5f004` | `df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119` |

Read source/qualification hashes:

- `analysis/resume158-cowvector-publication-owner.md`: `786e28be7843049fa573c6efd5d9a8b4353a03e289c6d4975c02b4ebead15dab`.
- `analysis/resume158-heapfree-fls-next.md`: `294b0eb0965bf5aa26aed19b8f65670de315e3948b543b6aec8ed73a65396d3d`.
- `analysis/resume157-geometry-unpublished-value-implementation.md`: `08bc5e30e529f6b6e014706bc5b20fe0ff4cbe1c73c1cb1e15ffcaf827d39bf8`.
- `analysis/resume159-geometry-next.md`: `d6fd24815edbe079d6c559b0cc9a08ba55ec58f50c7d0a445e71fb3e2d25c6d0`.
- `source/port/defrt/include/defrt/module_heap_role.hpp`: `a6fa20a95d835519a9447ee4ecb8792acc4df944e91f85501025d934080a7ac5`.
- `source/port/defrt/src/module_heap_role.cpp`: `de8d6748df324473e3276e3dc62fcee29f19fe4a98910ebdb566e9c0ff2a3538`.
