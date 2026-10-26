# CP159: queued Close envelope and the private cleanup frontier

The next static edge after CP156 is now bound: Qt's queued branch constructs a type-43 MetaCall event from the actual connection fields, posts it to that connection's current receiver, and, conditional on actual later delivery through QObject::event, invokes the event's stored metacall on the delivered receiver. Posting is a separate occurrence from delivery and Close admission. This closes a conditional continuation without repeating CP156's signal-clone/thread comparison.

No new source-ready Close integration cut was found in the inspected owners. The independent private OPI cleanup frontier remains its canonical current +160 guard: production owns one slot, but its knowledge remains Unknown. A defaults-era null receipt cannot advance that cleanup. Neither gap is repaired by an additional identity token or detached helper.

Only this report was written. No source/build/test, GUI/app-control, original execution or mutable REA operation occurred. The reverse-engineer-anything workflow used retained qualifications and bounded offline DUMPBIN 14.44.35228.0 plus section-aware PE/table/hash reads. No new analysis session, engine import, or connection repair was attempted. No reconstructed executable or deployment was inspected or changed.

## New conditional queued continuation

Controlling identities: original DEF_GUI_MO_64 v11.0.2 retained executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; `evidence/original-binaries/QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Addresses below are full VAs in the latter DLL. This is static recovered behavior, not observed original event delivery.

1. CP156's activation branch calls queued helper **6712c310** with the actual connection record. At entry it retains that record in R12. Record+30 holds the queued argument-type information. The unresolved-type path performs metadata/type work; a sentinel at 672f4340 makes the helper return at 6712c568 without posting. Successful type preparation is therefore a predecessor, not a supplied default. This bounded review does not claim complete registration/type/allocation failure behavior.
2. The prepared path allocates arrays, writes type/argument entry0 as zero/null at 6712c47e/485, and, for additional entries, calls 67126590 per actual type/source argument at 6712c4c3. The event carries copies produced there; it is not just a saved pointer to the emitter's argument array. The number/types for the particular live Close connection were not observed. The registered no-argument spelling alone does not certify that profile after CP156's signal normalization.
3. At 6712c4de the helper requests **0x50 bytes**. On the successful constructor branch, 6712c511 constructs the QEvent base with **type0x2b (43)**; 6712c516 installs table **671a93d0**. It stores sender/signal information at event+18/+20, argument count/types/arguments at +24/+28/+30, null semaphore+38, and the connection-derived static-call/method-offset/local-method fields at **+40/+48/+4a**. These field names follow the exported QMetaCallEvent interface and the consumers below; they do not assert original C++ source spelling.
4. At 6712c55e it **freshly reads record+8** for the post receiver, then 6712c563 calls exported two-argument **QCoreApplication::postEvent6711bfd0** with that receiver and the constructed event/null result. The wrapper clears R8D (priority0) and jumps to the three-argument implementation **6711ae90**. The constructor call and a call into postEvent do not prove queue insertion, wakeup, receiver survival, event-filter passage or eventual delivery. This lane stops at that implementation boundary instead of inferring those effects.
5. Independently, exported **QObject::event6712caf0** reads event type at +10, applies its byte-index/dword jump tables, and type43 selects **6712cb8c**. I decoded the actual table entry: `(43-1)` indexes byte6712cf32, yielding2; dword slot6712cef4 resolves6712cb8c. That case installs an on-stack sender context in the actual delivered receiver's private+58, then calls **event vt+8 at6712cbc7**, passing that actual receiver. It restores the prior context on the ordinary returned path when its live-state flag remains1. Full unwind/reentrant-deletion semantics are not closed by this selected case.
6. The event table's first two qwords are **67002720, 67129a20**. Exported **QMetaCallEvent::placeMetaCall67129a20** reads stored static call+40, local ID+4a and arguments+30. With a nonnull static call, it tail-jumps to that call with actual receiver, call-kind0, stored local ID and copied argument vector. Otherwise it adds stored offset+48 to local ID and jumps to **QMetaObject::metacall6711f9b0** on the actual receiver. CP156 already qualifies that fallback's dynamic-metaobject branch and main's static metacall/local20 route.

Thus a prepared event with the established main local20 fields can conditionally reach the existing Close slot once delivered. The actual record fields, successful production, actual receiver and delivery route remain witnesses. The queued route cannot be compressed into CP156's immediate direct call, a forced same-thread call, or a converter-return destructor. In particular, busy/current-W/modal state must be read when Close is actually invoked, after any intervening queued work, rather than captured at emission/posting.

## Independent private cleanup check

CP129's actual Operation-owned OperationInfoModelCache is present and exposed through a const canonical accessor. `commitNativeDefaultNull` explicitly refuses that canonical instance; separately constructed instances are diagnostic observations. Its private production knowledge remains Unknown. `attemptOperationInfoRecoveryFlagsCleanup` reaches ItemModelCacheBoundary only after the same live Operation's explicit Props-null, engine-cache-null, empty-object, empty-position, null-wrapper and null-flags steps; at that boundary it returns ItemModelCacheCleanupUnavailable. No cleanup continuation reads a qualified current +160 value.

CP130 remains controlling: constructor/defaults locally establishes zero, but positive-ID setup can enter task-directory failure modal execution before later Props/class/postLoad callbacks. Those later paths can also construct/rebind the actual item model. A canonical KnownNull commit added only to operationDefaults, or invalidation added only to postLoadViews, would therefore be insufficient. A bare/nonpositive-ID fixture has a narrower local interval and is not normal project Open/Close admission.

This is distinct from QAction: even a headless command selecting the recovered Close entry directly on a real main would still encounter these later cleanup dependencies. Conversely, a private same-Operation payload cleanup helper can be developed without proving a QAction occurrence, but only after its actual current owners/effects are supplied. Nothing in the queued continuation supplies a model owner or authorizes ordinary shared_ptr destruction as the native +160 deleting call.

## Exact next owner and ordering

For **public Close**, root's next architecture task remains the real application/main producer and Session bridge, independently of menu emulation: canonical main M owns its actual manager cell M+50; the existing Session manager G must be that manager, while G's constructor-main backlink and adapter Session backlink remain separate. The application current-main W and dynamic construction phase are separately mutable. A direct headless command may choose 140031f10/1400299a0 only on that produced main. It must run fresh busy query, then the actual M+50 can-close lookup, then accepted effects; the later typed-manager read occurs again after callbacks. If the event route is selected, queue/type/receiver/filter/scheduler delivery are additional predecessors, not replacements for that owner task.

For **private OPI cleanup**, the exact missing witness is a source-owned producer for the canonical then-current Operation+160 target at the late 14005079e read, tracking every admitted create/rebind/target-death event or a narrowly qualified current-null observation. The first practical source task needs exclusive ownership of cache header/source/test plus the actual producer/invalidation callers after their callback audit. Adding a cleanup-only overload that accepts a detached KnownNull certificate is not a viable cut. If source-owned knowledge is produced, invalidate it before unrepresented effects; Unknown does not mutate the native pointer. Keep payload-return, guard deregistration, containing OPI/QObject destruction and public accepted-Close admission as separate stages.

No source edit is recommended merely to expose another stored pointer or event envelope. There is no ready code task from these new static bytes alone. The report narrows the next evidence action: bind actual producer/lifetime state or audit postEvent6711ae90 through event removal/delivery/filter routing for the actual receiver profile. Do not repeat signal normalization or the AutoConnection equality investigation.

Future focused tests, once the owners exist, should establish:

- A failed/unprepared queued type profile does not call Close; a posted-but-undelivered event does not admit it either.
- A prepared type43 event preserves its recorded local/offset/static-call fields and copied arguments, and dispatches on the actual delivered receiver. Test static and combined-index fallback paths separately.
- Intervening queued work may change W/busy/M+50; Close uses the fresh correct cells, retaining constructor G->M rather than retargeting it to W.
- OPI cleanup refuses canonical Unknown without advancing or deleting; old defaults receipts do not survive task-directory/modal/class/model callback boundaries as current-null authority.
- Any future current-null continuation advances only through payload-cleanup return. It neither calls null guard assignment nor expires OPI/member/QObject ownership.

These are proposed checks, not executed tests or acceptance evidence. Existing target `m2m_mdt_operation_info_cleanup` can own the last two groups only after the canonical producer is qualified; public lifecycle tests require the real application/main bridge.

## Compact immutable receipts

Half-open native byte spans, SHA-256; reproducible with section mapping in `tools/doe_static_pe.py`. The helper's complete unwind region is 6712c310..6712c579; selected QObject case spans are not a claim of whole-function or exception closure.

| Native range | SHA-256 |
| --- | --- |
|6712c310..6712c579 queued helper|`df210cfdf121a963e2bd6b8e8ccc55295aa99470564e4a9bda01240ff02300c3`|
|67129a20..67129a4c placeMetaCall leaf|`d4a91d0dc8f8d9221d626cdc6f3aba2ecebace245ca0c8a4b5f9198198890a30`|
|6712caf0..6712cb3d event selection prefix|`60a104ba46db8545f8d324310c7443006b2e8a13d02b074350a3c370de595754`|
|6712cb8c..6712cbf1 MetaCall case|`86ce84483f981b79eb92d643fa618fbd26adfe6a9425d5265f2d2dc4fdeace7b`|
|6711bfd0..6711bfd8 priority0 postEvent wrapper|`5ff5c34d6cb2c15a01b83babd507992c6034f2556fd37de55691d68231f9dbaf`|
|6712ceec..6712cf4f event jump tables|`8622c02f6bba0724bcf28ce39dd59db419a4aa220484fe8ded1b5d4f2a91d96e`|

Read receipts: CP156 report `701922c1f56b8d8a0c679e00d5c894816ac0be5cac3cb8373bb285d99d3e202b`; CP130 report `354636c57c6ea1a3141ec73d0ff76188e94cada2725af4ae1da353a15e898048`; CP129 report `12cb0d8c04ab173227f773ba45095d32a633cbfbb8e7a96b55b305346a272acb`.

Current source inspection identities under `source/apps/moproj2mst`: `src/model/operation_info_cleanup.cpp` `d684d985865a795bc3000a7eff78d36e58db3cb457d0831a63c160e3521a2909`; `src/model/operation_info_model_cache.cpp` `d316e3dec25a4ed97a389654d2a8d9de6d69a36858b2a512483cf48ab3305a06`; its header `bd87dc12903ff0a5ca1fd9fd8c80a62f7899ea65680309655574aeabe2199510`; `src/engine/close_entry_prefix.cpp` `3af630e8a3ac1729d939342de5344efdbbe781cf4a3ba3358f43d222124ebc3e`; `src/engine/conversion_session.cpp` `679f4c2195a4e664709ec946a67253c177e3b3957ef36bf253dce53178eef9a5`; `src/engine/model_manager.cpp` `76d7ec9f605fff55d8c7acb694ce1d66337aa43571277fd971883fb8649b3cd4`. These are read identities, not a new source freeze or compiled candidate.
