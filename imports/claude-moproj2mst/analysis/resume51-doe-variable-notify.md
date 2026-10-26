# DOE variable notification: receiver and continuation boundary

`2signalSetOprHasDoe()` has live flow-chart-model registrations. It cannot be replaced by an assumed empty notifier or by setting an operation Boolean. Its identified receiver updates the diagram item's primary icon through the actual diagram backend. A bounded continuation past the notification is possible when the **actual shared notifier state and complete receiver population are qualified**, and either dispatch is suppressed or every reached receiver takes a verified return branch before icon allocation. The current `FlowChart` projection has no backend/lifetime witness, so the public DOE guard cannot yet be lifted from this static finding alone.

## Evidence and correction

The original PE SHA-256 was rechecked as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All new captures use local Microsoft `dumpbin` 14.44.35228.0 with explicit ranges. `resume51-doe-variable-notify-receipt.json` contains 13 selected function ranges/code hashes, Qt metadata values, registration sites and 14 capture hashes. `resume51-doe-variable-notify-candidates.json` retains the initial literal/reference candidates; behavioral claims below use confirmed disassembly. No target execution, GUI, build, source edit, Ghidra or mutable REA session occurred.

**Argument-order correction to the resume50 body report:** `0x1400d25a0` constructs the bool argument before the int argument, but copies them into the final call frame with **R8 pointing to int operation ID and R9 pointing to bool true**. Thus the actual slot call is `slotSetOprHasDoe(operationId, true)`, not `(true, operationId)`. `resume51-doe-variable-notify-argument-order.asm` shows the final copies/setup at `0x1400d28e1`–`0x1400d2995`. The prefix source currently retains named `value` and `operation` fields and performs no dispatch, so no delivered call needs correction there; a future typed dispatcher must use int first.

The inherited registration contract is qualified by `wave2-doe-task-factory-fixed-registration-contract.json`, `wave2-doe-session-notifier-call-instructions.json` and `wave2-doe-session-notifier-register-binding-decompile.json`. Their private static analysis and selected disassembly establish a fixed **direct-call** inventory, not whole-program runtime/dynamic registration closure. Current captures independently verify this key's constructor registration and dispatcher path. Retained prose about an empty map does not establish this signal's population.

## Actual registrations and method binding

`SfMoDiagEditorModel` constructor `0x140070f10` calls registration API `0x1402cfbe0` at **`0x140071166`**:

- global notifier owner `0x143d76700`;
- key `2signalSetOprHasDoe()` at `0x142139230`;
- receiver = this actual flow-chart QObject;
- slot `1slotSetOprHasDoe()` at `0x142139218`;
- fifth flag = 0.

The complete retained fixed direct-call inventory has a second matching registration at `0x140071a7f` in copy constructor `0x1400711a0`. The controlling fresh Open path uses `0x140070f10` per sub-project; the copy constructor is a separate GUI-creation path. The normal constructor is captured completely in `resume51-doe-variable-notify-model-ctor.asm`; the copy-ctor match is supported by the retained inventory and prior decompile, not a new complete copy-ctor capture.

Inherited/current registration logic removes the initial Qt marker and text beginning with `(` to retain bare method name `slotSetOprHasDoe`. Flag 0 appends when the same receiver/method pair is not already registered; flag 1 is the prepend variant. Consequently, different flow-chart models can all be registered for this global signal in constructor order. **Use all registered models, not only the active sub-project's model.** A registration is a weak QObject guard plus method, not ownership of the model. No current receiver count/order at B121 execution was observed.

The original constructor installs primary vtable `0x14213bc08`; its qt_metacall slot points to `0x1400d90b0`. The PE-resident Qt metadata table at `0x142168f00` has revision 6, 29 methods and table offset 14. With string table `0x142169180`, method row **23** decodes to `slotSetOprHasDoe(int,bool)` (row `[900,883,20,20,10]`). `0x1400d90b0` delegates through static metacall `0x1400d8a70` for IDs below 29. Jump-table entry 23 at `0x1400d8f3c+23*4` targets **`0x1400d8eb2`**, which loads int argument 1 and bool argument 2, then calls **`0x140086e20`** at `0x1400d8ed5`.

The static metacall returns at `0x1400d8f3b`; its following bytes are a data jump table. The capture's automatic disassembly of those data bytes is not used as code evidence. The runtime staticMetaObject storage at `0x143d7dfa0` is not file-backed in the PE; no startup initialization or Qt dynamic-metaobject override was executed/observed in this pass.

## Dispatcher ordering and failure behavior

Complete body `0x1402cf3c0`–`0x1402cf737` is in `resume51-doe-variable-notify-dispatch.asm`:

1. Read notifier byte `+0x141` at `0x1402cf3f1`. If nonzero, return directly. This skips flag reset, map access and receivers.
2. Otherwise set notifier `+0x90=0` at `0x1402cf408`.
3. Convert the key to QString, look up/default-insert its list through map helper `0x1402d03f0` at notifier `+0x88`, then copy the selected list into local `rsp+0x70` through `0x1402ef070` (`0x1402cf454`, `0x1402cf461`). Default insertion semantics and the Qt list-copy interpretation are inherited qualified helper contracts; their complete Qt internals were not re-recovered here.
4. Traverse the local snapshot in order. Resolve each QObject guard. Expired entries are removed from the **local traversal list** at `0x1402cf6ec`, with local count/index adjusted. Do not turn this into unconditional pruning of the original registration population; no write-back of the local list appears in this dispatcher.
5. For a live receiver, obtain its metadata, copy the ten QGenericArguments, and invoke its bare method with connection type **1 (DirectConnection)** at `0x1402cf6c6`. The call is synchronous; a later queued event pump is not this delivery mechanism.
6. Store the invokeMethod Boolean result at local `+0x8c` (`0x1402cf6cc`) but do not branch on it. An ordinary false invocation result does not abort subsequent receivers or become a parser false return. An exception/fault can still interrupt dispatch; no rollback is present.
7. Destroy the local list and return. The emitter then continues its operation loop; after all its operation messages return, the parser performs its later selector/type/text writes and eventually reindexing.

The scalar prefix's committed variable/keyword state precedes all these dispatcher effects. If an adapter stops at an unresolved receiver, preserve the earlier dirty/list mutations, any flag reset/map lookup and effects of earlier receivers. Do not move the refusal before committed keyword state or run the later scalar setters early.

## Receiver effects

The selected slot wrapper `0x140086e20` simply forwards `(this, operationId, hasDoe)` to **`0x140086ca0`**. Its complete 370-byte body is `resume51-doe-variable-notify-hasdoe.asm`.

1. If **model `+0x18` is null**, return at `0x140086e09`, before item lookup or any QIcon allocation.
2. Otherwise call `0x140086820` to find the first logical item whose virtual `+0x28` value equals the operation ID. That helper walks model `+0x20` in order and returns the first match or null. If null, return, also before icon allocation.
3. For `hasDoe=true`, allocate 8 bytes (`0x140086d0d`), construct a Qt **QIcon** through import `0x141e0b688`, construct the default QSize, and call **QIcon::addFile** (`0x141e0b690`) with resource **`:/SfTool/icon/tool/iconDoe_24_24.png`**, mode 0 and state 1. There is no resource-load-success check or explicit repaint call in this body.
4. Invoke actual backend `model+0x18` virtual **`+0x238(logicalItem, iconPointer, 1)`** at `0x140086dd2`. For `hasDoe=false`, invoke the same slot with a null icon pointer at `0x140086e03`.

Constructor-only null backend is real evidence: `0x140070f10` calls base constructor `0x14014c220` on model+0x10, and that base writes its own `+8=0` at `0x14014c298`, hence whole-model `+0x18=0`. This proves initial state only. It does not prove the backend is still null after widget/diagram publication or during lazy DOE parsing. The current source `FlowChart` stores logical items and number-cache state but has no qualified native `+0x18` witness.

For the **retained vtable candidates** `SfDiagEditor` and `SfDiagCellEditor`, backend `+0x238` maps to `0x140181c80` and `+0x1d8` maps to `0x140171190`. Actual backend type/identity was not bound by runtime evidence here; guard other targets. The candidate setter's complete body:

- null logical item returns;
- otherwise backend `vt+0x1d8(logicalItem)` obtains its graphic item;
- null graphic item returns;
- selector 1 calls `0x14016dbe0`; other selectors call a separate secondary-icon setter.

`0x140171190` checks the backend's implementation map at backend `+0x28`, map `+0x20`, for the logical-item key and returns its mapped graphic item or null. `0x14016dbe0` checks graphic-item implementation `graphic+0x68`, primary-icon pointer `impl+0x118`; if non-null, it deletes the old icon **before** assigning the new pointer. Delete helper `0x14016e8d0` invokes QIcon destructor then heap deletion for flag 1. There is no direct `Operation.hasDoe` field store in this identified chain.

**Missing graphic item is not an early no-op:** for a true signal, QIcon allocation/addFile happened before this later map lookup. If the candidate setter returns because no graphic item exists, this caller does not delete the new icon afterward. Treat its allocation/lifetime outcome as retained/possibly leaked native state, not automatic cleanup. Null allocation, old-icon destruction and Qt resource/cache behavior were not runtime-tested.

## Exact next bounded prefix

The safe continuation is conditional, not a blanket assertion that this signal is empty or UI-only:

- With an **actual qualified shared notifier owner** and observed/constructed suppression byte nonzero, model the suppressed return (leave `+0x90` and map unchanged), then resume the native scalar sequence after the message.
- With suppression zero, require the actual qualified complete ordered receiver list. Reset `+0x90`, preserve map/default-insertion and snapshot semantics, and process expired/live guards. A known live flow receiver with a still-qualified null backend or a qualified missing logical item returns without icon allocation. If **every** reached receiver completes in those recovered branches, the scalar parser can continue past the notification.
- A live backend plus logical item reaches QIcon allocation/resource setup and the actual backend setter. This needs qualified backend/map/icon ownership state; stop before the first unrecovered call while retaining earlier effects. It cannot be replaced by an active-operation Boolean, a copied active-only model, or an invented null backend.

After all messages for the keyword return, the already recovered next parser prefix is `0x1409109c8/0x140910a1b` keyword retrieval, selector write `0x140910a38`, function/enum flags, nominal-text setter `0x140910aba`, scalar empty-bound branches and final `vt+0x1e0` reindex. Retain the existing function/enum/geometry/design-mode guards. No `0x140916530` or `temp.KWD` is introduced for scalar double.

Preserve that **post-delivery keyword lookup** rather than retaining an assumed unchanged pre-delivery target across callbacks. The recovered callback candidates update icon ownership, but arbitrary/dynamic registrations and reentrant effects were not closed; an unavailable or null post-delivery keyword remains an explicit unresolved state.

The smallest implementation follow-up is a typed int/bool notification continuation bound to the **same shared notifier and actual flow models**, with explicit receiver lifetime/order and backend witnesses. Tests should cover suppression, two live null-backend models, expired snapshot entries, missing logical item, stop at an unqualified live icon branch after earlier receiver effects, and post-message scalar setter ordering. Public active DOE success remains guarded until that actual ownership/population/binding is available. This report establishes no fresh-original delivery, full graphics closure, Save parity or objective completion.
