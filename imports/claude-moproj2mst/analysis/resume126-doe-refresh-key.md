# CP126: refresh key QString, map/list and first receiver dependency

No further production refresh cut is source-closed after CP125's scalar store. The next actual edge is construction of a 24-byte signal-key QString using the process's invocation-time C-string codec. That process/read/data owner is absent. Even a completed key would not establish the actual map/list population or a live receiver return. Keep the current public guard; another fixed-key value helper would repeat already-known data without admitting the native stage.

Only this report was written. No source/test/build/GUI/original process or mutable REA state changed. Existing retained immutable exports and bounded DUMPBIN reads were used; no new raw artifact was written. This is distinct from the closed QFileInfo custom-handler-state investigation.

## Exact source slot and conditional data owner

The original GUI PE is SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; shipped QtCore is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

Retained Save wrapper `resume103-doe-save-reconcile-1400d3200.asm` loads RDX=0x142166b90 at0x1400d368f, RCX=global0x143d76700 at0x1400d3696 and calls dispatcher0x1402cf3c0 at0x1400d369d. The source literal is exactly `2signalDoeTableRefresh()` (24 ASCII bytes before NUL). Registration uses the identical text at a different address0x14218af60. This is not a QString shared-null symbol, object pointer or pre-existing key data owner.

In the unsuppressed dispatcher,2cf408 clears flag90. At2cf40f RDX loads the saved C-string pointer,2cf417 sets RCX=rsp+0x98 and2cf41f calls IAT0x141e04480. CP81's exact import binding resolves it to QtCore QString(char const*), export RVA0x174bd0. Constructor0x67174bd0 calls fromAscii_helper(chars,-1)0x67074120 at174be2; successful return is stored into the source object's data slot at174be7. The returned object pointer spills in dispatcher stack locals are not additional QString owners. Failure before that store publishes no completed key object.

The helper first reads QtCore codecForCStrings at0x67074126 (global RVA0x2f3e10). Its branch is selected at this invocation, not inferred from PE zero-fill, fresh Session, language-cache value, ASCII spelling, a Qt handler flag or CP82's diagnostic Null profile.

- If that actual read is null, nonnull/nonempty input computes strlen24 and enters0x6706bfa0. Allocation request is2*24+0x20=80 bytes at6bfb2, distinct from the icon resource's104 bytes. Success initializes ref1, size/allocation24, payload data+0x1a, low five flag bits0 and a trailing UTF-16 NUL, then widens the key. Other flag bits/padding remain unknown. The key does not acquire QString shared-null on this branch. Allocation failure goes to6bfbf/throw before source-slot publication; the earlier notifier flag store remains.
- If nonnull,0x6707419f calls the actual codec conversion wrapper0x67072460 (virtual decoder). It retains the returned QString data and releases the conversion temporary. Concrete codec identity, returned text/data/ref owner and failure are not qualified by the fixed ASCII input. Do not force Latin-1 or a known ref1.

CP82's QStringResourceProcessScope has unknown production codec by default and only isolated profile setters. Its source constructor remains guarded on actual Session/receiver occurrence. Its fixed36-byte icon path/104-byte occurrence cannot be reused as this different24-byte key/80-byte source slot. There is no process context bound to this reached Save dispatcher occurrence; no current production interface closes the source read. Source-known conditional allocation arithmetic alone is not admission.

## Map lookup and local list are separate later owners

The retained complete dispatcher capture (`resume64-boolean-event-route-1402cf3c0.asm`, SHA `9AB15E4A88C891082CBFC05D07D01893D082AA204330970C893E783773DC3B64`) gives this later order:

1.2cf445 addresses notifier+0x88.2cf454 calls0x1402d03f0 with the completed key object.
2. The selected helper first calls2d0730 at2d0415. That reads QMap data ref at data+0x68 and tests !=1 via QBasicAtomicInt; when shared it calls2d0770 before the lookup. The actual QMap data/ref state therefore matters even when a key already exists.
3.2d042f calls the lookup helper0x140070af0; the result is compared against the actual map data sentinel. On miss,2d0459 constructs a temporary empty list via0x14018f9e0,2d04a3 calls insertion0x1402d08c0, then destroys the temporary. The helper returns the node's list value via13cc960/+8. On hit, it returns that value without the miss insertion. Do not equate missing key with present empty value, or claim native allocation/copy/failure parity from a host map.
4.2cf461 calls0x1402ef070 to create the dispatcher-local list at rsp+0x70 from that value. The helper copies the native data pointer, increments its atomic ref at2ef096, and checks low bit1 in data+0x10; an unshareable source takes2ef0b3 ->15d230 copy/detach. A deep host vector copy is not a literal replay of those native ref/allocator effects.
5. Only after the snapshot does2cf46f destroy the temporary key QString.2cf47a obtains count, then traversal begins. The source key's destruction is after snapshot construction, not immediately after map lookup. On normal end,2cf724 releases the local list. A later exception does not undo the earlier notifier store or a completed map insertion; exact native allocator/COW unwind parity is not newly recovered here.

Default empty list constructor18f9e0 loads the QListData shared-null import0x141e04218 and increments its current ref via0x141e042f8. Existing `wave2-doe-session-notifier-empty-map-data.json` proves QMapData shared-null initial size0 at QtCore0x672f0a70; that is an initial image datum, not a Save-time map/population or current native refcount witness. Table registration and other source activity intervene.

The candidate's canonical SessionNotificationState currently represents scalar suppression/flag90, selected qualified other-key registrations and some empty-key facts. It has no typed actual DoeTableRefresh ordered population/native key/map/list-data owner. An emptyKeys set entry, a table diagnostic identity or another signal's registration list cannot fill this gap.

## First identified receiver, without claiming it is the first actual entry

The fixed retained direct-key inventory identifies a normal matching registration call0x1400ebc5c in SfMoDoeTable constructor0x1400eb6f0: this receiver, slot `1slotUpdateUI()`, key `2signalDoeTableRefresh()`, global0x143d76700, flag0. The recovered registration routine appends when that receiver/method pair is not already present for flag0; its alternative flag path prepends. This establishes a producer and ordering rule, not the actual complete ordered Save-time list or absence of indirect/dynamic registrations.

The table constructor performs initial UI refresh before registration. Main-window getter0x1400196d0 publishes at+0x458 only after constructor return; dock+0x460 installation owns the widget. Interrupted construction is not automatically a published live registration. Current DoeTableOwnerState receipts do not execute those QObject/QTimer/UI operations.

Dispatcher traversal first gets a registration entry and evaluates its guard via0x140c1aef0 (2cf4c7). A dead guard is removed only from the local snapshot and skipped. For a live entry it obtains QObject metadata and invokes the stored bare method with DirectConnection1; invokeMethod's Boolean is ignored. Do not turn method-lookup false into callback success or an application Boolean.

For the identified table, metadata resolves slot0x1400f4e10. It loads timer at table+0xb0 and calls QTimer::stop at0x1400f4e25, without a null-timer branch, then calls same receiver virtual+0x1b8 atf4e38 ->0x1400f47e0. The refresh flag/widget/current-global Project/active SUB/manager operations follow. Only a normal slot/dispatcher return permits wrapper0x1400d36a2 and the cached Save manager's subsequent PAR sequence. Refresh's currently selected manager need not equal that cached one. Zero variables or known Constraints do not prove timer/UI state or make this callback no-op.

## Smallest exact next input, and stopping rule

No source change is proposed now. The smallest new production input is one source-qualified process/codec read bound to this actual post-XML dispatcher key constructor: process/module identity and generation, actual live Session/notifier occurrence, key pointer/content and destination slot, reached QtCore+0x74126 value and source interval. A null read permits the conditional80-byte fresh-data owner only after the modeled allocation and constructor publication; a nonnull read additionally needs its real decoder/data owner. A reconstructed process producer must cover initialization and intervening setters on the same canonical context; a free-form qualification string or isolated Null setting is insufficient.

After that, a separate same-occurrence map/list binding must establish actual key existence/data ownership, complete ordered guarded registrations and lifetime. Empty/dead-only or suppression can return without a timer, but cannot be chosen from constructor/static-count evidence. Direct table delivery needs the actual table/timer and normal refresh return. There is no authorization here to acquire runtime/GUI control. If the process/read owner cannot be supplied under current constraints, stop this branch and pursue a different active reader/output increment rather than adding another isolated fixed-key helper or repeating handler/codec import scans.

No exclusive implementation files are requested while these producers are absent. Eventual root coordination would own the real process/Session context and shared dispatch call; a DOE source lane could then reuse the canonical QString data/ref owner with this distinct key occurrence, not create a parallel counter. Focused tests must distinguish actual source admission from diagnostic Null, wrong/revoked process/Session/slot generations, failure before publication retaining flag reset, key-hit versus insertion, shared/unshareable snapshot, dead versus live registrations and interrupted versus normal callback return. Those tests cannot substitute labels for the missing producer.

## Inspection identities

The bounded original normal bodies were read without execution; raw hashes through RET are:2d03f0/230=`f9024e5d86fac2f76f35cf6979ed1c332c207cd6f78d9a311d4fbc4a57c2b3b8`;2d0730/54=`132286022fff56f6e55fbb1b9010f6e4d91db95d8024c65f0920f5f3238e177f`;2ef070/82=`4e47ec14a92c305810bd004f4bc350a8a218365869033954decbd39c45fde72a`;18f9e0/48=`a17fd6ed20323281552f62200d2152f06cc39e181381a9a84235da56541d621b`. These bound the described branches; nested allocation unwind behavior remains separately limited as above.

Retained constructor capture SHA `39562AA6405DE1900DD8E7D5263AA6828B3F457D98CADE572B6D5FCD070149E2`; fromAscii capture `0DCD3D48F97C3F157D4DF4BC9FB5D65BB99CF0690D8BF04D8E667A3296174F53`; table slot capture `A12B2571B28BA5CF842532FC23ACF407EBF2E4F9A64694EC4844B89B4BDA6762`; direct-key inventory `1A947BB2DAF487A9E9D8AEE1999D09675F2ABB6202B3E526EF1847FCBC6CF0D9`; empty-map data `E32D87B0CE4EAB268C343770A64A27FBBD8E6E0F5FC3C614F660878524D19497`. CP125 table-owner source remains SHA `AAD5EB5382EDF7DF98ADD74576FFAB8B89738BBFF0CD0E6CCC2427C113EE466E` and was not edited.
