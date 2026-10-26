# Checkpoint 77: original SetOprHasDoe receiver route

The original fixed route is an actual flow-model QObject slot that updates an item's primary QIcon through the diagram backend. It does not store an Operation.hasDoe Boolean. Its first lasting receiver effect on the matched `(operationId=1, hasDoe=true)` route is QIcon allocation/resource setup, before the later graphic-item lookup. There is no basis to skip this route merely because the candidate currently refuses Population.

This report follows retained original PE/Qt metadata evidence, not candidate refusal behavior. No new capture, source/build/test/GUI/application action or mutable REA occurred. Only this report was written.

## Argument order and registration

The descriptive earlier phrase `SetOprHasDoe(true,1)` refers to the logical bool/operation values, not the delivered ABI order. CP51's final frame setup proves int first, bool second. Use `slotSetOprHasDoe(1,true)` for the actual slot. CP50 prose that listed bool first is historical and superseded by the argument-order disassembly.

Retained complete normal `SfMoDiagEditorModel` constructor `0x140070f10` registers at `0x140071166`:

| Input | Original value |
| --- | --- |
| Notifier RCX | actual global `0x143d76700` |
| Key RDX | `0x142139230`, `2signalSetOprHasDoe()` |
| Receiver R8 | this actual flow-model QObject |
| Slot R9 | `0x142139218`, `1slotSetOprHasDoe()` |
| Flag stack+0x20 | 0, append variant |
| API | `0x1402cfbe0` |

Registration strips the Qt marker/signature to store the bare method `slotSetOprHasDoe`. Distinct receivers are retained in registration order; duplicate same receiver/method handling is distinct from model construction order. Copy constructor `0x1400711a0` has its own registration site `0x140071a7f` in the retained fixed-call inventory. Do not reinterpret copied construction as a normal SUB constructor event.

All registered flow models can receive this global signal. Selecting only the active SUB would change behavior. Static registration sites do not establish the exact runtime first live receiver instance for the historical B121 action; that requires actual constructor/registration/liveness history. If a source-closed fresh constructor history admits SUB0 then SUB1, that ordering is a profile fact to bind, not a conclusion from the method's metadata alone.

## Dispatch to the first slot body

The retained complete dispatcher `0x1402cf3c0` first checks notifier+0x141 suppression, then on unsuppressed entry clears flag+0x90, obtains/default-inserts the exact key's map list, snapshots it and walks that ordered snapshot. Expired guards are removed from the local snapshot only. For a live receiver it invokes the bare method with ConnectionType1, DirectConnection, and ignores the ordinary invoke Boolean. Earlier receiver effects remain if a later receiver refuses/faults; ordinary invoke false is not an abort.

Constructor primary vtable is `0x14213bc08`. Its `qt_metacall` goes to `0x1400d90b0`, which delegates local method IDs below29 to static metacall `0x1400d8a70`. PE-resident Qt metadata header `0x142168f00`/string table `0x142169180` identifies method row23 as `slotSetOprHasDoe(int,bool)`; row words are `[900,883,20,20,10]`.

Static metacall case23 is `0x1400d8eb2`. It loads `argv[2]`'s byte to R8D and `argv[1]`'s int to EDX, then calls `0x140086e20` at `0x1400d8ed5`. The wrapper forwards those arguments to `0x140086ca0` at `0x140086e41`.

This closes the **static selected route**, not an observed runtime Qt invocation. The runtime staticMetaObject storage at `0x143d7dfa0` is not file-backed; dynamic overrides, exact live registration population and QObject lifetime still need their actual source/runtime owner facts. The bytes after the static metacall's return are its jump-table data, not additional executed instructions.

## Exact first receiver effects

Complete `0x140086ca0` makes the following decisions:

1. At `0x140086cc7`, test this model's whole-object `+0x18` backend pointer. Null returns immediately at `0x140086e09`, before item lookup/QIcon allocation.
2. Nonnull calls `0x140086820` at `0x140086ce2`. This walks the actual model+0x20 logical item list in order, invokes each item's virtual+0x28 ID getter and returns its first operation-ID match. Missing operation1 returns before allocation. It does not search only the active operation or select the last duplicate.
3. For true, allocate8 bytes at `0x140086d0d`; nonnull calls imported QIcon constructor at `0x140086d24`. The native null-allocation continuation still reaches subsequent icon use, so a reconstruction must guard undefined/fault behavior rather than fabricate a successful null icon.
4. Create default QSize and resource QString, then call QIcon::addFile at `0x140086d95` with `:/SfTool/icon/tool/iconDoe_24_24.png`, mode0, state1. No resource-success check becomes a branch here.
5. Re-read actual backend+0x18 and invoke virtual+0x238 at `0x140086dd2`, passing the matched logical item, allocated icon and selector1. For false, the corresponding call at `0x140086e03` passes null instead; that is a different icon-clear ownership path.

Constructor base evidence (`0x14014c220`, store `0x14014c298`) proves only that initial whole-model+0x18 is null. Actual widget/backend setter history can change it before lazy variable parsing. A headless candidate, absent field, or unknown publication interval cannot be interpreted as this native null return.

For retained backend vtable candidates SfDiagEditor/SfDiagCellEditor, virtual+0x238 maps to `0x140181c80`. That body calls backend virtual+0x1d8 to find the graphic item. Candidate map accessor `0x140171190` checks implementation `backend+0x28`, map+0x20 for the logical-item key and returns the mapped item or null. Other actual backend targets require their own binding.

When selector1 finds a graphic item, `0x140181cd6` calls `0x14016dbe0`. It inspects the item's implementation at graphic+0x68, deletes an existing primary icon at impl+0x118 through `0x14016e8d0(flag1)` **before** storing the new pointer at `0x14016dc55`. The delete helper calls QIcon destructor, then heap deallocation when flag1 is set. These ownership effects cannot be replaced with an operation Boolean or an unqualified integer token.

Crucially, a missing **graphic** item returns after the true branch allocated/initialized the new icon. The caller does not delete that icon after the backend returns. Preserve its allocation/possibly leaked lifetime outcome; do not model this as the earlier missing-logical-item no-allocation return or add automatic cleanup to improve it.

## Smallest qualified effect before DoeTableRefresh

There are three different granularities:

- At the global dispatcher, the first persistent source mutation on unsuppressed entry is flag90 reset at `0x1402cf408`, before list lookup. That is already source-qualified independently of any receiver result. Exact list lookup/snapshot/liveness remains required before invoking a first live slot.
- Inside the delivered slot, null-backend and missing-logical-item routes return before icon allocation. A source-closed actual backend/list witness can admit those returns. They are existing qualified branches, not evidence that the fresh B121 profile takes them.
- For actual nonnull backend plus logical operation1, the smallest next lasting receiver effect is allocation of the new QIcon at `0x140086d0d`, then its constructor/resource calls. A primary-icon pointer store at `0x14016dc55` is later and additionally depends on actual graphic map/old-icon ownership. It cannot be advanced first.

Thus the smallest safe new mutating implementation on the matched true route requires a correctly typed actual icon allocation/resource owner, with partial allocation retained if a later map/ownership boundary refuses. Current opaque backend prerequisite/commit metadata alone does not supply that QIcon or graphic-item object. A descriptor saying which resource would be requested is diagnostic evidence, not execution of that effect. This report recommends no duplicate nominal notification helper to sidestep the missing owner.

An isolated ownership helper for the later old-icon-delete/new-icon-store sequence could be considered only after its inputs are actual qualified graphic implementation and concrete old/new QIcon owners. It must preserve deletion-before-assignment, aliasing/repeated-pointer hazards and earlier leaked allocations. Generic delete callbacks/tokens by themselves are a test profile, not proof of native object/resource construction.

## Position before refresh and scalar continuation

SetOprHasDoe belongs to variable/keyword reading: variable append/sample invalidation and keyword/operation commitment occur before its emission. After **all** keyword operation messages and receivers return, native code re-fetches the keyword at `0x1409109c8/0x140910a1b`, then performs selector/type flags and nominal-text setter `0x140910aba`, followed by remaining child processing/reindex/deferred links. Do not reuse an assumed unchanged keyword pointer across dispatch or copy pending text into committed fields before that return.

DoeTableRefresh is a different, later no-argument signal from the InputTable Save wrapper `0x1400d3200` after XML writer return. It targets SfMoDoeTable/QTimer/UI, not these flow-model icon slots. Closing this SetOprHasDoe route does not close DoeTableRefresh, and a Save-time XML refresh must not be used to replace earlier read-time SetOprHasDoe effects. A post-action B121 snapshot alone cannot prove either runtime dispatch was empty or harmless.

Focused future checks must separate suppression, expired guards, two live receivers, actual null backend, first matching logical op1, missing logical item, icon allocation/resource before missing graphic item, old-icon deletion before primary-store, and partial earlier-receiver effects on later refusal. They must keep the retained scalar prefix's collected nominal text pending until a real dispatch return and preserve both public guards. No tests were authored/run in this lane.

## Evidence receipts

Retained original image SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All claims above use original registration/dispatch/metadata/slot/icon captures and CP50/51 qualification; candidate source was inspected only to distinguish implemented projections from native facts.

| Retained artifact | SHA-256 |
| --- | --- |
| resume51-doe-variable-notify-receipt.json | 159D1E29113B720B20BA949B721BD4A9D7BB73AAE31FFF28C11C736A28ECF8AB |
| resume51-doe-variable-notify-model-ctor.asm | ECE4C8D343DBF75C024C56FAAEEAFE921A75B31F197996C53455C429BB6E144D |
| resume51-doe-variable-notify-static-metacall.asm | CF77D7CE29874F2D8DF458441C8D87D4434C791BA730ABB193FED3C545BA1A80 |
| resume51-doe-variable-notify-hasdoe.asm | 2439025A3730101ACE5EDB17BCD1FAD8CAE86C466092564E046223E96C910E19 |
| resume51-doe-variable-notify-icon-setter.asm | 4F1930A55BC3448D7FBB995A4AAB2865DC55E88C0D7EC70786B38ADFCF6D4555 |

Files are under analysis/. No observed B121 runtime receiver count/order, delivered native slot, Qt startup metaobject initialization, resource-cache result, native destructor parity or public DOE completion is claimed.
