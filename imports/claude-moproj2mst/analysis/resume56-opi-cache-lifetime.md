# Resume56 OPI+160 null-cache lifetime through native Close

A qualified null+160 cache stays null through a noncreating lookup or null class-clear branch. At native OPI teardown, a null CURRENT+160 skips target deletion; later destruction of its null QPointer registration is a Qt fast return. Neither event expires the OPI identity bound by OpClass.current. That identity expires later at QObject clearGuards. A defaults-era KnownNull snapshot alone is insufficient to select the teardown null branch after intervening creation/callback paths.

This lane wrote only this report and uniquely named bounded ASM receipts. No source/build/test/GUI/mutable REA action occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; shipped QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. All behavior below is static, with branch/population qualifications retained.

## Noncreating lookup and class-clear lifetime

Native054f40 receives a create byte. False branches directly to the current+160 guarded-pointer getter at054fc6..054fd2. It neither allocates nor assigns the guard. True also preserves a currently nonnull identity; only true+null enters allocation0x50/0c4890 and writes the resulting target through cbc920 at054fc1.

Native050c80 class-key update has two separate cases:

- Equal class key: return without lookup, model clear or guard mutation.
- Changed class key: assign the new QString, call054f40(false) at050cd4, then if returned model is nonnull call QStandardItemModel::clear at050ceb. IAT141e0b600 binds that exact clear method. Null skips clear.

Thus a KnownNull cache remains null on this branch. For a nonnull cache, this direct caller preserves the SAME model/guard binding and requests content clearing; arbitrary transitive clear recipients destroying the model are not excluded by this body. Class clear is not target deletion, guard-null assignment, guard deregistration or OPI expiry. Native constructor/RTTI binding of+160 to SfMoItemModel remains in resume54-opi-expiry-owner.md.

## Creation paths prevent global null-lifetime assumptions

A direct E8 byte scan of the retained .text located candidate054f40 callers; selected sites were validated by decoding their containing unwind procedures. It is a location aid and not an indirect-call/inlining proof.

Validated noncreating examples set edx=0 before054f40 at03496e,0386f1,047e95,050cd4,058794,060ae6,0621dd,13126e and1312e6. This supports those selected direct call branches only, not their whole surrounding callback closures or every Open/Save path.

There are explicit creation routes:

- `054c80`: resolve class via054e90, then engine model via05f770. Null class or model returns. Otherwise create=true at054cbb/c0 ->054f40 at054cca; then class virtual+b0 receives that item model and a notifier follows. A loaded engine model is therefore a concrete creation dependency, not evidence that+160 remains absent.
- `054950`: resolve class; its virtual+a0 gate at05499d admits the branch. After another callback, current+160 is read at0549de. Null allocates0x50 and calls0c4890 at054a08, then assigns+160 at054a3b. This allocation is inline relative to054f40, so tracing only getter calls misses it.

The byte scan found direct calls to0c4890 at054a08 and054f91. That does not exclude computed/indirect constructors or other writes. In particular, Props/model/Qt callbacks can occur before the destructor's late+160 test. No exhaustive argument is supplied that fresh original Open/Save/accepted Close always takes the null branch.

The isolated cache helper's Unknown/KnownNull state is currently safe as a declared observation fixture because it is not wired across unrepresented production creation effects. Future integration must stop before an unsupported creating path or invalidate its observation when an unknown mutation/callback is actually admitted. Do not carry an old KnownNull certificate across such a boundary and then use it to skip native deletion.

## Accepted-Close ownership route and its limits

The already qualified accepted path is0299a0 after busy/can-close/nested-Save and post-answer receivers succeed. At029a76 it enters actual ModelManager c78300. That deletes model children via virtual+18. The Mo child045350 ->044040 ->0440c0 first deletes its Project (044bf0), clears model+19c0 after return, then045000 deletes the five Mo manager slots in order.

For operations+19c8, actual manager wrapper04c8c0 ->0460f0 first deletes its nonnull selection-model+30, then shared base06c1e0 ->d48d00 ->QStandardItemModel destructor. Operation rows own SfMoOprItem; its045fa0 ->045cb0 cleanup deletes OPI via045dc0 before item diagName/base destruction. Exact Qt root/header/row deletion iteration, dynamic populations and transitive callbacks remain outside the recovered app-wrapper order; this report does not close them by assuming vector equivalence.

On an OPI row actually entering050440:

1.050466 calls050690 payload cleanup. Its Props, engine-model, object, position and recovery-wrapper stages run BEFORE the current+160 read.
2.05079e obtains the then-current+160 guard.0507a3 tests it; zero jumps to05080e. The target deleting virtual call0507e5 and cbc920(null)050809 are BOTH skipped on this null branch.
3. Return to050440. It always calls cbc900 on the+160 guard at050478, before string/list/member destruction and QObject base.
4.cbc900 calls IAT141e042a8 = QMetaObject::removeGuard. Shipped QtCore6712d950 compares `*guard` to0 at6712d96d; null jumps at6712d971 to the epilogue6712db0c. No registry lookup, detach, target deletion or QObject signal occurs on that fast path. The wrapper call exists, but its target-dependent cleanup is empty for qualified null.
5.050440 continues OPI strings, list storage and+38 guard destruction, then QObject::~QObject at050531.
6.The OPI's OWN registered guards, including actual OpClass.current, are cleared later by QObject clearGuards at6712f8bd before admitted destroyed delivery. A null+160 guard says nothing about those separate incoming guards.

This distinction is central: **a member guard can be null/deregistered while the containing OPI identity is still live**. Model absence is not OPI deletion. OpClass and its registry can survive both because they have session lifetime.

The null fast path does not execute a cached model's destructor, selection-model destructor, QStandardItemModel row cleanup or cached-model QObject callbacks. It also does not justify skipping ANY of the earlier OPI Props/model/list stages, later OPI member/QObject stages, outer item cleanup or subsequent ModelManager current reset.

## Consequences for current adapter

Current actual OpClass.current uses stable metadata; three temporary writer/loader bindings restore snapshots. The Props-first helper still stops on nonnull Props and at the following engine-model stage. OperationInfoModelCache supports only Unknown/native-default KnownNull and noncreating null lookup. It exposes no live target or deletion API, and lookup refuses once the owning OPI metadata expires. These are appropriate limits.

The cache's KnownNull diagnostic field may remain readable after OPI expiry, but it is a retained historical observation, not an addressable native+160 field. A getter on a destroyed native OPI is not legitimized by returning null; the helper's expired-owner refusal is intentional.

Automatic merged Operation destruction still produces AdapterIdentityLifetime. Do not relabel it QObjectBoundary merely because the+160 null member fast path is now source-bound. The real native expiry remains after the distinct payload/member prefix and before outer item destruction.

## Smallest safe follow-up validation

An isolated staged+160 null-teardown test can now distinguish three successive states without enabling Close:

1. Actual OPI/current live; the current cache result at the declared teardown encounter is qualified null. Select only the null-target skip; record no target destructor and no explicit guard-null assignment. Do not infer this encounter from a stale defaults snapshot.
2. Reach the later member-guard removal encounter. For qualified null, record the source fast-path return without a cache/model destructor or event callback. OpClass.current still identifies the OPI.
3. Only a separately declared completed OPI prefix can enter the existing explicit QObject-boundary commit; aliases then null before a fixture observer while outer row remains allocated.

Such a test validates a declared continuation schedule, not fresh original acceptance. The existing public path must continue refusing before unresolved Props/model/Qt receiver effects. A real integration needs a current guard observation/validity chain, not a public `cleanupComplete` bool or nonblank string treated as proof.

Also retain a class-clear regression: changed class/noncreating null lookup has no allocation/delete, while an eventually supported live cache clears contents without rebinding identity. Never combine these with a broad registry current reset.

## Retained bounded receipts

Each new ASM uses installed DUMPBIN14.44.35228.0 `/disasm:bytes /range:<start>,<end-inclusive>` against the hash-verified binary. Bounds below are exact PE unwind regions, end exclusive; they do not establish transitive closure. Source text files retain tool banner/range bytes. Prefix `analysis/resume56-opi-cache-` is unique to this lane.

| ASM suffix | Code SHA-256 | ASM SHA-256 |
| --- | --- | --- |
|140054f40.asm|2d5dae26240126418419a455d9462f2f66444ac69afed9dfe7e1c9f9dddba297|15c617ce9f10ea6dcc9b65fdec22781718468d09e29ee79c5acf961ee694f931|
|140050c80.asm|ac30afb3e5ce5bb2ce71f377007a97a79bb3135aeb1e7c8699b46143a24fe71d|20cef8ca1095db368dda9476d4cbbb07ed35e4ea433d95ffb75f976e9257f6a5|
|140050690.asm|24a977641207431f5e855a68c134285b5842803b649d08fd0726ace8f8163b8a|7bacc92563c959bafabed471fdc83c0efb102cc39a030060429d0dbab12a5a14|
|140050440.asm|2a30891bad2d50d84eb779f8d7b7ad81c43b9654a37725622d3fe42d89c78247|c96e510773c410c90b490c9e7ffd069bffc37c8eba91a495f35ae4c8f21838d9|
|140cbc900.asm|13e827f0805e0c97ff985371040318551bf7f8a5ce980fb5a83de5ceca316e4f|18b63d39f6af3ce82a7327f16c17b262f3ed1508326414edf8a754a1ec7b2993|
|140054c80.asm|3400d7b67c4c8394f9b2f971cf51a52febb896085c959c669688a5b5940d9498|f8bbe67a841a0999fe09438ad04852da5cc6253e8f1f74daf6f9739a9a9202b7|
|140054950.asm|4e73e67993a8faa93eeee5a997004a1f8b42e94ad1f62b9bf4755bdb5fedfdff|7a5e356914f4561c813ee8d001dc7c1559ecdeb5207c82c672ec06c43b839d95|
|6712d950.asm|50a5b39194fb3fcec07a7962469eaae4d4fd63e4b774c57578f44f83bcf2fdd0|94e4e5f23aa994b155bcdcecc9efb799008c2aa91b11434493ff442f11db0111|

Regions:054f40..054fdc,050c80..050cf6,050690..050813,050440..05053c,cbc900..cbc919,054c80..054d07,054950..054c17, QtCore6712d950..6712db1d (add MO image base140000000 where omitted). Existing normal-Close/manager/OPI clearGuards receipts remain referenced by resume50-close-destructor-next.md.

Current-source inspection hashes: operation_info_model_cache.cpp `6d4793964bb3dd297fd9e62e2c01eeae28a229c3b414669b58c8a2b95708e0ad`; operation_info_cleanup.cpp `a56f34eb1aee5dfc48ca0fe68c68ce0a55bedb69c8f492dfcc0092d8e2ae87dc`.

No reconstruction executable build/deployment occurred in this lane.
