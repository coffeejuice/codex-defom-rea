# Resume50 next native Close destructor phase

The next safe increment is a typed guarded OperationInfo identity for `OpClass::current`, with explicit invalidation at the recovered QObject boundary. Native current invalidation occurs after operation payload/member cleanup and before QObject destroyed delivery. Clearing every registry current pointer before deleting rows would change that order. The existing capsule's `operations_.reset()` remains storage-only and does not establish this destructor protocol.

Method: read-only retained PE/QtCore plus bounded DUMPBIN disassembly, fixed vtable qwords, exports/IAT bindings and current source. No source/build/test/GUI/mutable REA changes. This report is the sole written artifact. Original DEF_GUI_MO_64 v11.0.2 SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; shipped QtCore4_64.dll: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. DUMPBIN reports 14.44.35228.0 from the installed 14.44.35207 tool directory. This is static control/data-flow evidence, not runtime callback-population evidence.

## One Operation row is two original identities

The original operation row contains a non-QObject `SfMoOprItem` and an owned QObject `SfOperationInfo` (OPI). Modern Operation folds them together. Their separate destructor boundaries matter.

- `045c30` constructs the item and installs vtable `142119a58`. Its slot 0 is deleting wrapper `045fa0 -> 045cb0`. QStandardItem destructor dispatch uses slot 0 here; do not use the QObject-style +18 slot on this item.
- Item destructor `045cb0` calls `045dc0` at `045cd6`, then destroys item+50 QString at `045ce5`, then calls SfMoItem destructor `06b750` at `045cf1`.
- `045dc0` tests item+48 OPI. If nonnull, calls its deleting virtual+18(flag1) at `045e07`, then writes item+48=null at `045e1f`. Null skips deletion and the null store.
- OPI vtable `1421247d8` +18 is `0652a0 -> 050440`. The scalar wrapper frees allocation only if raw flag&1 after the body returns.
- OPI destructor `050440` invokes cleanup `050690` at `050466`, then destroys OPI members, then calls imported QObject::~QObject at `050531` (IAT141e042b0).

Thus registry current guards to OPI expire before the outer item's diagName QString/base QStandardItem destruction. A lifetime token that only expires at the end of the folded Operation object's entire member destruction is a useful adapter fallback but does not by itself establish this earlier native OPI boundary.

## Ordered OPI cleanup and conditions

`050690` directly executes this order:

1. `0512c0` at `0506a1`: if OPI+10 Props is nonnull, invoke its deleting virtual+18(flag1) at `051307`, then store OPI+10=null at `05131f`. It does not clear an OpClass current guard first. Actual derived Props destructors remain separate payloads.
2. `05f980` at `0506ae`: read guarded OPI+38 through c1aef0. If nonnull, invoke that engine model's deleting virtual+18(flag1) at `05f9d3`. This routine contains no explicit cbc920(null) afterward; QObject destruction can invalidate the guard. Do not infer that dropping one shared_ptr always executes native model deletion when external adapter leases exist.
3. `061180` at `0506bb`, then `061f70` at `0506c8`: the object and position/list cleanup paths. This audit confirms calls/order; element deletion, list sharing, row iteration and reentrancy inside their callees remain unexpanded. `061f70` calls `062070`, then `061ec0`.
4. If OPI+20 nonnull, invoke `065440(...,1)` at `050709`, then zero +20 at `050726`.
5. If OPI+28 nonnull, invoke `065400(...,1)` at `05076a`, then zero +28 at `050787`.
6. Resolve guarded OPI+160. If nonnull, invoke its deleting virtual+18(flag1) at `0507e5`, then assign its guard null via cbc920 at `050809`.

After `050690`, `050440` destroys guard+160 (cbc900 removes the guard registration, not the guarded object), strings at+158,+148,+130,+128,+120,+118, the+60 list (`066a10`),+50 list (`066890`),+40 list (`2081a0`), guard+38 (cbc900), then QObject base. Do not conflate list storage destructor calls with the earlier payload deletion calls.

Current source Operation implicitly destroys its last-declared engineModel first and its earlier Props shared_ptr late. This differs from native Props-before-engine-model. The capsule solved outer ownership lifetime, not row/OPI destructor order. Do not mark Operation deletion faithful merely because capsule slot order now matches.

## Registry current guard invalidation is now source-bound

- Original `5ef280` assigns class+20 by calling `cbc920`; `5ef2b0` reads class+20 via pure c1aef0. The setter's cbc920 compares old pointer with new and skips work when identical. Otherwise it calls IAT141e058b0, verified `QMetaObject::changeGuard(QObject**,QObject*)`.
- Shipped QtCore export `changeGuard` is6712db20, `addGuard`6712d7a0, `clearGuards`6712de20, QObject::~QObject6712f870 and destroyed6712f700. Add/change paths set private+20 bit0x200 at6712d83d/6712dc65.
- QObject destructor first updates private+20 at6712f8a8..ae (clear0x4,set0x8), then tests bit9 and bit0. With bit9 set and bit0 clear, call clearGuards6712de20 at6712f8bd. OPI is constructed as QObject, and registry binding supplies the guard. Do not generalize this exact early path to QWidget destruction.
- clearGuards matches guard records for the dying QObject; at6712df75 it loads the registered guard-address from node+18 and at6712df79 writes zero to that address. It does not delete the registry/OpClass owning the guard.
- QObject destructor's destroyed notification path follows this guard clearing. At6712f939 it tests private+60 bit0 and also checks global callback hooks; when admitted it calls6712ec70 at6712f97b with the QObject and null second argument. Receiver population and exact transitive effects remain qualified. Do not invoke a synthetic event callback merely because this branch exists.

The resulting contract is precise: an OpClass bound to this OPI stays bound during Props/model/list/member cleanup, becomes null when the OPI QObject guard boundary is reached, and is null to admitted destroyed observers. Other OpClasses bound to other live OPI identities remain bound. Equal-pointer reassignments preserve binding. Rebinding a guard before old-object destruction must prevent old-object invalidation from nulling the new identity.

Modern `OpClass::current` is still a raw Operation* (`include/m2m/mst/writer.hpp`), and current fallback access is used by `src/ops/base.cpp::OpClass::props`. Registry retention now makes stale-current exposure explicit. No runtime dangling dereference was reproduced here.

## Operation manager and the four item managers

Actual SfMoOprManager vtable14211de88+18 resolves04c8c0 ->0460f0. Its app destructor:

1. If manager+30 is nonnull, invoke its deleting virtual+18(flag1) at04614f, then manager+30=null at046167. Constructor045fe0 builds this selection-model state and wiring; its destruction can matter before row teardown.
2. Call shared item-manager base06c1e0 at046174.
3. That calls d48d00; d48d00 calls imported QStandardItemModel::~QStandardItemModel at d48d1d (IAT141e05a50).

The other four manager app wrappers are bound by constructor vtables and exact +18 slots:

| Slot | Constructor-installed table | Deleting wrapper -> body | Body delegates |
| --- | --- | --- | --- |
| objects |14212cf68|06b610 ->068c60|068c7d ->06c1e0|
| materials |14210e8c8|043e00 ->042680|04269d ->06c1e0|
| equipment |142153418|0c0200 ->0bed60|0bed7d ->06c1e0|
| lubricants |14214f458|0beb10 ->0bd740|0bd75d ->06c1e0|

These four inspected app bodies only reinstall their table and call the same base. They do not establish that QStandardItemModel destruction is inert, nor the order in which its root/header/row items and connected selection receivers are destroyed. Modern ItemManager has only kind/rows and does not represent QObject model signals/selection state. Native QStandardItem callbacks and individual object/material/equipment/lubricant item payload destructors remain prerequisites.

SfMoItem base destructor06b750 destroys +38 helper, +28/+20 strings, then imported QStandardItem::~QStandardItem at06b7a5 (IAT141e0b660). This is a non-QObject base; do not attach OPI QObject current-guard invalidation to every Item uniformly.

## ModelManager is another later boundary

Keep three things separate: operation manager destruction, individual OPI QObject destruction, and outer SfModelManager clear/destruction. Prior qualified evidence establishes c78300 child deleting virtual calls, then list clear atc7852b and explicit current guard reset atc7853e. A selected model's QPointer can already become null at its QObject boundary before that final explicit reset.

Existing ModelManager::clear() resets shared owners, clears list/current/connections; connections_.clear() is an adapter disconnection projection. It does not execute destroyed notifications, row callbacks, queued type3 ModelChanged/ModelUpdated delivery or posted-event deletion. Those cannot be silently claimed from container clearing. Existing `wave3-close-qobject-posted-event-removal-contract.json` expressly lacks actual destructor queue populations/dynamic event owners; do not reuse its synchronous widget event state or another startup/editor capture as proof.

## Smallest safe implementation increment

Implement a typed `OperationInfoGuard` and an OperationInfo lifetime identity distinct from the outer merged row allocation; wire the actual OpClass current storage through it. Preserve normal pointer-read/assignment behavior at existing call sites after a bounded usage audit. The guarded object must NOT retain Operation ownership. Provide an explicit guarded lifecycle method to commit the recovered OPI QObject-boundary invalidation; adapter composite-object destruction can expire it as fallback but must remain labelled later/coarser cleanup.

Do not scan the registry and null every current before deleting an operations vector. Per-identity guarded handles naturally preserve rebinding, multiple aliases and unrelated current selections. Do not expire a first-declared token in the Operation destructor body before deleting Props/model/lists. Do not claim a token expiring after the entire folded row represents the earlier OPI boundary; expose and test that distinction.

Keep accepted Close guarded. The first integrated slice can fix actual registry current storage and expose the correct explicit boundary while the native destructive coordinator still refuses unresolved Props/model/list/QStandardItemModel effects. Follow-on work recovers/stages those payloads before invoking that boundary from accepted Close.

Focused tests: registry bound before destruction; Props cleanup observes the still-bound guard; explicit OPI boundary expires it before destroyed-hook observation while outer row identity is retained; two OpClasses sharing one OPI both null; another live OPI stays bound; old-object destruction does not clear a rebind; equal-pointer assignment preserves identity; capsule/registry outlive Project without dangling current at qualified boundary. Use actual object lifetime/identity and effect hooks, not only a phase counter. Add refusal tests proving no native guard-expiry claim when earlier payload work is unresolved. No tests were run here.

## Reproducible bounded code receipts

New reads used DUMPBIN `/disasm:bytes /range:<start>,<end>` against the hash-verified local originals, and direct PE fixed-qword/IAT/export resolution. The table hashes exact unwind-record byte ranges (end exclusive); an unwind span is not proof of a whole logical function or transitive closure. Newly read disassembly was not saved as separate files because this lane owns only this report; commands, key instruction sites and exact byte hashes support repeatable recapture.

| Binary | Start..end exclusive | SHA-256 |
| --- | --- | --- |
|MO|140045cb0..140045cfb|0dc71aee079aaa5390eeb1b9eba572c41d21c323871db1e24b6f1d488e185fbc|
|MO|140045dc0..140045e2c|6c1cf7a95ce8ee5396d3cb89784ec4e3e16706be760da116433896693ad115e7|
|MO|1400460f0..14004617e|bfc66d6c4fd240f2e96f9f1bea6a20606a505fcefaaf90486b4f39f773438df7|
|MO|140050440..14005053c|2a30891bad2d50d84eb779f8d7b7ad81c43b9654a37725622d3fe42d89c78247|
|MO|140050690..140050813|24a977641207431f5e855a68c134285b5842803b649d08fd0726ace8f8163b8a|
|MO|1400512c0..14005132c|57471f6e75d1c985a5f199541e467501ca51da8583ab7a9ee01fe64f31ab38d6|
|MO|14005f980..14005f9eb|702a4bc5073e6cc0fe27a325d5c9136f65f4d977c392f02d5ba1823e06c1f7b7|
|MO|14006c1e0..14006c207|ace2945357a2d429b77118b0407a7ad19388b1eb6c78b31a11094507cf02d023|
|MO|140d48d00..140d48d28|dd5c50d379a574ae75cb7daae0df23d9c7a90eb2c00003b60794b413772657d5|
|MO|140cbc920..140cbc957|fadb5223030c46fcc06d80a5eacec37408b8a697f2249bf949755d9ea84af6b8|
|QtCore|6712de20..6712e02a|314a81ae67a7f93581c5e8ec9104820ee9763371ce0ac2545142e194d8abee25|
|QtCore|6712f870..6712fdbf|e629e2450ab541e6cfc9370ae975aa17e3911537c42d54fd923f35d3bcc29693|

No reconstruction build/deployment occurred in this lane.
