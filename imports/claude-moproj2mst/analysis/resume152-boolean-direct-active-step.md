# CP152: the pre-publication direct receiver updates MainWindow state, not E's polish cache

The ChangeActiveStepData leg now has a concrete receiver/slot chain. Its selected MainWindow callback conditionally binds **W+48 to the incoming SfStepData S**, before later scalar reads/UI callbacks. Null S preserves the prior guard. The notifier independently resets its own raw+90 byte when unsuppressed. These are new source-qualified pre-publication mutations; neither field is Boolean E's widget-private+d0 cache, and no same-E/H dequeue or Apply occurrence is established.

Only this report was written. Bounded immutable original PE/DUMPBIN views and metadata/import reads were used. No source/build/tests, GUI/app control, original process or mutable REA/import occurred. CP149's post/compression proof was not repeated.

## Direct named receiver, not a Qt signal-local-index guess

The fixed-literal/code-carrier scan found 27 direct ChangeActiveStepData carriers: 26 calls to custom emitter 2cf3c0 and one call to registrar 2cfbe0. This bounds that inventory only; it is not a runtime population or exclusion of dynamically constructed registrations.

The registration is in **SfMainWindowBase constructor 140c98b00**: at c98e62 it registers notifier 143d76700, signal literal 1429073c0 (`2signalChangeActiveStepData()`), **actual this receiver W**, slot literal 1429073a0 (`1slotChangeActiveStepData()`) and flag 0. The base constructor installs table 14290ac88; its qt_metacast cc7540 identifies SfMainWindowBase. MainWindow constructor 010490 calls that base at 0104d2 and later installs derived table 1420fb108. This is a different owner from the under-construction Boolean E.

The parenthesized no-argument registry literal does not define the dispatched argument list. The named registry retains a bare method name. Actual SfMainWindowBase metadata header 1429159f0 (180 local methods), strings 142916840 and method entry 155 identify **slotChangeActiveStepData(SfStepData*)**, signature offset 0x1150/string 142917990. Its qt_metacall cc75a0 delegates to dispatcher cc5970; local 155 selects cc6d24 and invokes **W virtual +718 at cc6d40**, passing the supplied SfStepData pointer.

Both inspected base and MainWindow tables have +718=c9c5a0. That wrapper calls **W virtual +728 at c9c5c0**. On the derived MainWindow table 1420fb108 the target is **03cd30**, which first calls base implementation **c9c880 at 03cd51**. Other/unbound current tables cannot be silently replaced by this one.

## Notifier gate and immediate state order

Boolean base setup 130830 binds E+38 before calling 2cf3c0 at 130ad1, while E still has its base table; Boolean table142cd9b18 is installed only after base return at 11e24b7. The payload is the SfStepData pointer S supplied by the emitter, not E or Apply's OPI O.

Emitter 2cf3c0 reads notifier N+141 at 2cf3f1. Nonzero exits at 2cf3fa -> 2cf72e; **it does not reset N+90 on that branch**. When unsuppressed, it writes **N+90=0 at 2cf408 before registry lookup/invocation**. No semantic “success” meaning for that byte is assumed here. The selected invokeMethod call supplies **raw type 1 at 2cf6bd**, then calls imported QMetaObject::invokeMethod at 2cf6c6. This is the direct invocation route, not Auto/queued delivery inferred from a Qt connection. A live registration, compatible metadata and actual successful invocation still need admission; the registration instruction alone does not prove this callback ran.

On a reached normal MainWindow path:

- **Null S:** c9c88e/894 -> c9c896 -> c9c934 returns without calling guard binding or later callbacks. 03cd30 then tests the same S at 03cd56 and returns on null. The previous W+48 guard is preserved; null does not mean clear it.
- **Non-null S:** base c9c880 calls cbc920 with **RCX=W+48, RDX=S at c9c8a9**. This guard-binding call precedes S's accessor c7b310 at c9c8b3, scalar setter 917d60 at c9c8ba, W callbacks+730/+738 at c9c8d1/c9c8e9 and later notification 2ce6a0 at c9c8fb. Do not move or roll back that binding when a later read/callback is unresolved.

Accessor c7b310 itself first resolves another owner through c7b2d0; its null branch obtains the main window and invokes its+260, while its non-null branch calls 8e01e0. Thus the raw scalar returned at c9c8b8 is not available merely because S is known. If an actual normal getter result n is admitted, 917d60 maps **n==2 to global 143dd9b98=1; all other n to 0**, then calls 9dd680. That helper makes the same mapping to **143dd9ea0** before its continuation 9dea30. These raw globals are not assigned an unverified semantic unit/cache name here. Unknown n and the 9dea30 continuation must remain guarded.

The derived 03cd30 performs further project/property/UI work after its base call. This lane did not exhaust those callbacks or W+730/+738. A cache/guard mutation is not evidence that no nested event loop ran elsewhere in the full notification.

## Bounded source-ready slice

Two small private prefixes can be modeled without GUI input, given separately admitted owner/source stages:

1. **Notifier gate/reset:**actual N lifetime, known suppression and canonical raw+90 cell. Suppressed completes this branch preserving 90. Unsuppressed resets 90 once, then stops before receiver-list lookup/invocation. Unknown suppression is not false, and reset does not admit delivery.
2. **MainWindow null/guard prefix:**a separately admitted slot entry on source-qualified W/table with known nullable S. Null returns from the selected slot/base/derived chain with no W+48 clear. Non-null requests the actual cbc920 guard bind; only its explicitly qualified normal completion admits W+48=S, then the model stops before c7b310. A later scalar-result bridge could preserve the two raw-global stores before 9dea30, but must not invent n or native callback completion.

Use one canonical owner/cell per modeled N and W; a Session/shared-owner integration must reconcile existing fields rather than add competing caches. Tests should distinguish unknown/null/non-null payload, suppressed versus reset order, preserved previous guard on null, guard-return-before-store, wrong/expired receiver or invocation and guarded accessor/callback continuation. Such tests are declared source projections, not a native timing witness. No helper files were created.

## Precise same-E/H dependency remaining

On an actually admitted synchronous invocation, W's above mutations can occur inside E's base constructor before 11e24b7. That gives a conditional source ordering, not a captured H interleaving. The needed live frame is outer Boolean E construction/raw O argument -> 130ad1 emission and actual W invocation/return -> any nested same-E/H posted notify with caller-return QtCore RVA 11b32f -> handler 6506cc2d, recording E's **actual table at that stage** and source-stage current-meta/cache values. If H runs before Boolean publication, its base profile cannot be relabeled CP143's completed-Boolean profile.

For first-Open Apply/add claims, retain actual current-widget UpdateData ->Boolean E Apply 11e2870 ->resolved OPI O ->normal add return 295a/2bcb inside the complete Open span. W+48=S, notifier90 reset or a scalar-global write supplies none of those relations. No captured occurrence or public guard change is asserted.

## Reproduction identities

Use the immutable original module hashes and verified DUMPBIN 14.44.35228.0 path from CP149/145. Selected code-byte SHA256, end exclusive:

| Range | SHA256 |
| --- | --- |
|140c98b00..140c991d9|894f1a757f5d892f0d81c2a57b9b23da48cd6f7205304dd8b565077d39e19f18|
|1402cf3c0..1402cf738|d1ed25625be971f56a377e5620ad47bf020b8499c30500e61e15977379e33b5d|
|140cc5970..140cc74d4, includes dispatch data|0af5963927902481048f11fd245d995e66f9d3569f110d19dc99576559e7542f|
|140c9c5a0..140c9c5cb|f6199f648af1307e7f95e9bbd4c79b1692c27f06783b6c77a40f4d3240b9af2b|
|14003cd30..14003ce46|fcbc4dce517ce3483741edd62e0d9356cafad29575ddd1697f19aef801ecd7a5|
|140c9c880..140c9c939|bad98f239fe3284b900f140c7662082995057846924d29abee9de83b9767ed1c|
|140c7b310..140c7b365|0faba6198749a2d4f20315266806f2283881972021b006b241982a52902109fe|
|140917d60..140917da6|4bb1caa1d655ef142b3bae21de1d9477dc72deebd5b00c10aea9b764b0d61ed7|
|1409dd680..1409dd6c2|51dc5fcee3da62bcbf96f24fc728624cf2347b7365e5382fa244636ea205b4f0|
