# CP153: Close All action registration and accepted-owner boundary

The native Close All action is produced by the same concrete main that later receives its slot: main setup creates an SfMoAction owner, that owner retains main in +10, and its Close All QAction is connected to `slotActionFileCloseAll()` on that retained main. This closes the previously unexpanded action-registration edge. It does not prove signal delivery, acceptance, or that callbacks preserve the Project saved earlier. No new candidate coordinator prefix is source-ready without the actual application/main producer and fresh admission state.

Only this report was written. No source edits, build/test, GUI/app control, original execution or mutable REA access. The reverse-engineer-anything workflow used retained static evidence and bounded DUMPBIN 14.44.35228.0/section-aware PE reads; no REA startup/import retry. Original retained DEF_GUI_MO_64.exe v11.0.2: `analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable or deployment was inspected or changed. All addresses below are full VAs.

## Constructor-owned action and actual slot

Let M denote the concrete primary main receiver, B its SfMoAction bundle, A the Close All QAction, and W the separately mutable application current-main pointer. They are separate cells/objects.

1. During main setup `1400106d0`, site `14001087a` passes the same saved main receiver to `1400121c0`. That helper requests 0x1c8 bytes at `1400121d7`, conditionally constructs B with `(B, M)` through `1400052f0` at `1400121f3`, and publishes the returned pointer/null to **M+218 only after construction**, at `14001221c`. The null-allocation branch exists; successful construction is conditional, not an observed startup outcome.
2. `1400052f0` calls imported `QObject(QObject*)` with parent null at `140005314`, installs vtable `1420f4748` (RTTI type `.?AVSfMoAction@@`), and stores its actual second argument M into **B+10** at `14000533d`. It then calls setup `1400056d0` at `140005349`. B is not asserted to be a QObject child of M merely because it retains M.
3. `1400056d0` prepares strings “Close all currently opened projects”/“Close All” at `1420f2380`/`1420f23a8`. At `140005788`, it calls action factory `1402f0e90` with actual B+10 as first argument; at `140005798`, it writes the returned action into **B+18**. In the factory, the successful 0x10 allocation branch invokes imported `QAction(QObject*)` at `1402f0ed5` with that same first argument as parent and installs its derived action table. This parent relation belongs to A, not B. Full factory/icon/text effects are not promoted to a completed candidate producer here.
4. At **1400057f9**, imported `QObject::connect(const QObject*, const char*, const QObject*, const char*, Qt::ConnectionType)` receives:

   | Argument | Native production |
   | --- | --- |
   | sender | current B+18, loaded at 1400057f5 |
   | signal | `1420f23d8`: `2triggered()` |
   | receiver | current B+10, loaded at 1400057e2 |
   | member | `1420f23b8`: `1slotActionFileCloseAll()` |
   | connection type | integer 0 stored at 1400057cb (AutoConnection) |

   The returned Boolean is not tested before the next setup work. Registration intent/arguments therefore do not prove connect success, a triggered occurrence, delivery thread/timing, or a particular menu selection. Menu placement and the signal-emission body were not expanded in this bounded lane; no click/shortcut capture is claimed. This is a concrete QAction registration, not the unrelated ATL selector/event-proxy path.
5. CP107's independently retained Qt metadata has SfMoMainWindow local method20 `slotActionFileCloseAll()`. For call-kind0/local20, static metacall `1400db1a0` selects case `1400db5fa`, reloads the original receiver and calls **140031f10 at 1400db5ff**. That wrapper reloads the same receiver and calls **1400299a0 at 140031f1e**. The case does not map the returned Boolean into another result. This route is conditional on Qt selecting/resolving/delivering that connection; the static connection alone is not a live accepted occurrence.

CP107 already established main startup/constructor ownership and metadata/table identities. The new edge here is B/A production and the exact connect arguments. A headless reconstructed command may eventually select the recovered slot/Close entry directly on a genuinely produced M; fidelity does not require manufacturing a QAction just to choose that command. It still requires the entry's owners and effects.

## Accepted root: same M, fresh W/manager/Project reads

`1400299a0` takes only M. It takes no prior Save Boolean or saved Project pointer. CP110's corrected Save-answer/writer distinction remains controlling: a returned project/MST write failure does not by itself cancel the later Close, and Close may invoke Save again.

The source ordering is:

1. **Fresh admission:** `1400299cb` calls `1402ca920(singleton143d766b8, show=true)`. Global+9 and, conditionally, current **W**/its actual virtual+300 busy predicate are read. W is not silently identified with M; publication/readiness and later state writers remain separate. Busy can run a warning before false return.
2. **Actual M-owned can-close:** `1400299e6` calls `140c9b8c0(M)`. That wrapper reads M+50 through `140c9b7c0`, reads it again on the nonnull path, and invokes current manager vt+58 at `140c9b8f4`; null returns true. The qualified manager checks its actual children. Mo child `140044110` uses its then-current Mo+19c0 Project and process/question/nested-Save path; accepted callbacks, display reset and graph removal precede accepted root cleanup. These can dispatch effects/reenter. A saved P from explicit Save is not an authorized replacement for those reads.
3. **Accepted predecessor effects:** `140029a54` invokes true-busy on the application singleton; `140029a61` calls `14002aae0(M)` for null Tree/Property/Scene refreshes. Their notifier/Qt/provider effects must return before child teardown.
4. **Fresh concrete manager resolution:** only afterward `140029a6e` calls **1400133a0(M)**. This reads current **M+50** at `1400133c6`, performs `__RTDynamicCast` at `1400133ca` with the retained manager type descriptors, and returns the cast result. `140029a76` calls **140c78300 on that result**. The code does not cache the manager from the earlier can-close wrapper. Same M does not prove unchanged M+50 across intervening callbacks.
5. **Child-to-Project disposal:** `140c78300` walks its current/copy-owned list state; the child deleting virtual+18(flag1) at `140c784f2` precedes list clear `140c7852b` and explicit current-null `140c7853e`. For actual SfMoModel table `142112878`, +18 resolves `140045350 ->140044040 ->1400440c0 ->140044bf0`. Current nonnull Mo+19c0 P receives `1400982c0 ->1400aab30 ->1412f1190(P+60)` before the later reread and Project deleting wrapper `1400453e0 ->14008a3c0`. Project does diagram null-rebind, then ascending SUB deletion `1400a80e0 ->1400ac330 ->1400b58d0`, followed by its inline context/members. Mo clears its Project backlink after Project deletion, then tears down five model slots. CP142/143/149 qualify these targets and the absence of direct published DOE+20/+28 deletion in the SUB body.
6. **Later root effects remain:** post-clear Project lookup/refresh, widget-area cleanup, false-busy and status precede true return. Normal FileCloseAll retains main/application; CP149's later post-exec main destructor/global unpublication/manager destruction is a distinct route.

This establishes a conditional source path from a constructed action receiver to child/Project/SUB destruction. It does not certify that the reached child/P is the exact instance saved earlier without qualifying the intervening owner/callback history. Nor does successful registration discharge modal/due-event effects, feature providers or incomplete Qt destruction.

## Candidate comparison and smallest next task

Current `core/convert.cpp` still performs per-invocation Open/Save and adapter returns; it has no slotActionFileCloseAll command, actual native application/main producer or accepted Close coordinator. Its ProcessCodecOwner is inert Unknown metadata, not M/W. `ConversionSession::~ConversionSession` explicitly revokes receipts, releases Project storage and expires adapter receiver/scopes. `ModelManager::clear()` releases shared leases/current/connections; an external lease can postpone destruction. These operations cannot stand in for the native explicit deleting virtual/effect sequence.

`attemptCloseEntryPrefix` requires actual Session/manager ownership but its busy inputs are supplied qualified observations, not source-produced singleton/W/widget state. With an actual post-Open nonnull moProject, it stops at `MoProjectCanCloseUnavailable` before modal/accepted mutation. `attemptCloseEmptyManagerTail` likewise refuses nonempty ownership. FeatureContextAnchor still lacks the actual P+60 native constructor/Open/provider history; isolated feature runtime test frames are not that P. Existing guards remain appropriate.

**No additional helper-only or public coordinator implementation is justified by the recovered connect call.** The exact first missing integration witness is a source-produced application/main lifetime M with the existing Session manager G bound to its real M+50, separate mutable W/dynamic phase and fresh busy inputs at this selected Close occurrence. Selecting command20 can then be direct in the headless runner. For an observed native event route specifically, successful connection plus actual A signal resolution/delivery to M and delivered state-changing events would additionally be needed. Static registration does not establish any of those outcomes. The first normal Qt startup producer remains CP106/107's QApplication construction, not a new invocation-local pointer/ready flag.

After that owner exists, the smallest useful coordinator cut is **command selection on exact M -> fresh busy query -> can-close invocation on actual M+50**, stopping before the first unavailable modal/callback effect. It must not accept prior Save completion, detached helper observations, or M==W by default. Advancing to deletion additionally requires actual accepted preceding effects and the later fresh manager cast; actual feature cleanup remains a further guard before Project destruction.

Focused validation should use the real producer/Session bridge, not a label-vector fixture: keep M, W, G, actual Mo/P and allocation epochs across Open/Save; select the command once; mutate W independently and ensure busy targets W while manager lookup still targets M; permit earlier callback changes to M+50 without silently using the can-close manager; test exact Cancel/nested Save answer independently of writer false; refuse absent owner/Qt/modal state before effects, preserving completed writes; and retain current until actual child teardown finishes. No tests were written/executed in this read-only lane.

## Static byte and source receipts

These are end-exclusive native byte ranges, SHA-256, not new mutable REA Evidence IDs:

| Range | SHA-256 |
| --- | --- |
|1400106d0..1400108d8 main setup|`ca96cd55d05c58bdbbbe46cfd80a658885b1a590f8a99b99cae362f6a1fd17fe`|
|1400121c0..140012228 action-owner allocation/publication|`a82c3b4aadadc97d04293363f1ac2b8aaa8a859c29cd2faa84cb34921ca59969`|
|1400052f0..14000534e ctor through retained-main/setup call|`88c077f1cc1aaeb240d410c7be609d219ac187e84c8f72f6c9c968b10e9ecd16`|
|1400056d0..1400059b7 action setup|`e7d3f27321d0abcbb16fa6d6f5b91d762fb587c85f693a21db05678eec2174e4`|
|1400057cb..1400057ff exact Close connect|`e805150ebbecf98ba6cf449e61da062942e89f314d9141c52bbdff2e01a66e56`|
|1420f2380..1420f23e4 action text/signatures|`1fb487363bf4e13bf6b4b4305069148b112d65f7277fd3de8ceb11e0f23c3086`|
|1400db5fa..1400db609 metadata20 body|`c293b36f9578bf82064afbdb0c56400ecf93fa63431cc50237d013e38119976a`|
|140031f10..140031f28 slot wrapper|`04dd3035662edf8caff271e6a476b625294ce8e76f63c09b4eb43edabbaee3d7`|
|1400299a0..140029d04 Close root|`8a3490e8c9007cd78fa2c3d876f2e071ee2e8bb113292ae70f944cb4a3a30d9a`|
|140c9b8c0..140c9b900 can-close wrapper|`59157c3fe5b827e9b1722b8b0cc4df6663801ecf6fde1dc7d37df466e52f8e45`|
|1400133a0..1400133de fresh concrete manager cast|`6b6904774a8f9b2108dc414f05ea8357bc09d7ac47f04d43727fcc027898ac69`|
|140c78300..140c7854b child clear/current reset|`b2c665e2182ec5050431091d4859aafd7d86e1ca1ca2e4e7d14a36c5bf7b1487`|

Imports independently decoded: `141e041c0` QObject constructor; `141e041d0` static QObject::connect; `141e0b580` QAction(QObject*) constructor; `141e0bf20` QWidget::addAction (used later for the other action in setup, not evidence that Close All was added there). The raw E8 scan found one direct caller each of 1400056d0 (140005349), 1400052f0 (1400121f3), and 1400121c0 (14001087a), all validated at function/instruction boundaries; this does not exclude indirect callers.

Current source inspection identities under `source/apps/moproj2mst`: `src/core/convert.cpp` `f3fbc077fc700df96782eef7e621eaf1cba5010712a9b245279b9642c1ec14ce`; `src/engine/conversion_session.cpp` `679f4c2195a4e664709ec946a67253c177e3b3957ef36bf253dce53178eef9a5`; `src/engine/close_entry_prefix.cpp` `3af630e8a3ac1729d939342de5344efdbbe781cf4a3ba3358f43d222124ebc3e`; `src/engine/close_empty_manager_tail.cpp` `3b6dddd574342c44342e7ab8c321747c35807244205ab97904e706b16fc029d0`; `src/engine/model_manager.cpp` `76d7ec9f605fff55d8c7acb694ce1d66337aa43571277fd971883fb8649b3cd4`; `src/model/project_feature_context_anchor.hpp` `44883c50ec41c56d7a0f596f46647d6a931de5c7fd7862faba2a1641a30fe48a`. These are read identities, not a candidate/deployment freeze.
