# DOE table owner: native lifecycle and Session binding seam

The native `SfMoDoeTable` has a complete **main-window field lifecycle**, distinct from Project lifetime: initialize `+0x458/+0x460` to null, construct/register a table, publish it to `+0x458`, install it in the dock, and later delete it before clearing `+0x458`. Candidate Session already supplies the correct ModelManager/current-Project identity seam, but it has no actual main-window table/timer/widget owner. A Session-owned canonical **admission holder** can represent those source stages without inventing UI state; it cannot make public refresh succeed merely by existing.

Only this report and bounded static evidence/receipt were written. No source, shared interface, build, executable test, GUI, original execution or mutable REA changed.

## Newly closed field initialization and deletion

Main-window constructor `0x140010490` calls field initialization **`0x1400106d0` at `0x14001058e`**, before startup **`0x1400111c0` at `0x140010598`**. Initialization calls **`0x140019760` at `0x14001085c`**. That 38-byte leaf performs these exact stores:

- `0x14001976a`: main-window **`+0x458 = null`**.
- `0x14001977a`: main-window **`+0x460 = null`**.

These are source initialization effects, not a table deletion protocol. A holder may record them only at its qualified initialization stage; an unknown field must not start as a fabricated null or be reset to null to simulate cleanup.

Window cleanup **`0x1400108e0` calls `0x140019790` at `0x140010966`**. The latter tests cached `+0x458`:

1. Null returns without table deletion.
2. Non-null invokes **table virtual `+0x18`, deleting flag1**, at `0x1400197dd`. The identified table vtable targets deleting destructor `0x1400ffb90 ->0x1400ebc80`.
3. **Only after that call returns**, `0x1400197f5` clears main-window **`+0x458`**.

It does not clear `+0x460` in this deletion helper. An interrupted destructor must not be treated as a successfully cleared cache or destroyed dock. This selected direct cleanup is main-window scoped; Project release is not evidence that it ran. Constructor/destructor calls are native code paths, not observed runtime completion.

## Construction, registration and QTimer ownership

The CP65 native getter `0x1400196d0` checks `+0x458`. On a miss it allocates `0xb8`, invokes `SfMoDoeTable` constructor `0x1400eb6f0` with **parent null**, and stores its returned pointer at `0x140019738` only after constructor return. Cached hits return the existing identity without another allocation/constructor. The source's allocator-null branch returns/publishes null; unavailable allocation evidence is a separate reconstruction guard, not that native outcome.

Within the table constructor:

- allocate/setup UI payload `+0x38`;
- construct **QTimer(parent=this table)** and store it at table `+0xb0`;
- connect timer timeout to the same table's slotUpdateUI with AutoConnection0;
- perform initial UI refresh `0x1400f47e0` at `0x1400ebaf8`, including the Delete widget effect recovered in CP66;
- perform remaining constructor UI connections;
- register **this actual table QObject** for `2signalDoeTableRefresh()/1slotUpdateUI()` at **`0x1400ebc5c`**;
- return to the getter, which publishes `+0x458`.

Thus registration precedes cache publication, but follows substantial widget/timer/UI work. Allocation or a diagnostic shared lease does not establish constructor return, native QObject liveness, successful registration or publication. Retain partial allocation/registration effects if a later boundary is uncompleted; do not move registration after publication for convenience.

Dock initialization `0x140019950` calls the getter at `0x14001a513`, then **QDockWidget::setWidget(table)** at `0x14001a52a` on the dock stored at main-window `+0x460`. Dock installation is another effect after cache publication; failure there does not undo the earlier cache or registration. The timer's QObject parent remains the **table**, not the dock, Project, Boolean diagram receiver or Session token.

The selected table destructor body frees its UI payload, destroys its list holder and calls base destructor `0x140377ac0`. Complete Qt child destruction, native timer unregister/thread effects and weak-guard invalidation still require actual lifecycle qualification. The native parent argument establishes the intended parent-child relation; it is not a receipt that every later Qt destruction effect occurred. Escaped diagnostic leases must not keep a deleted native receiver dispatchable.

## Current Project linkage that already has a candidate owner

Native refresh gets the global main window and calls **`0x14001d510`**. The newly bounded body calls **`0x1400133a0`** (main-window `+0x50`, cast to the actual Mo ModelManager), then **`0x140045780`**:

- obtain current model through `0x140c78ad0`;
- no current model returns null;
- a non-Mo current model cast returns null;
- a Mo model calls `0x140044a90` for its actual Project. Its unrecovered lazy-Project branch must not become a null result or a substitute Project.

Refresh then stores current Project in table `+0x40`, unit in `+0x58`, active SUB in `+0x48` and that SUB's Study manager in `+0x50`. Those reads occur **after timer stop, `+0x98=1`, and the first Delete.setEnabled call**, not at the XML writer's entry. Do not eagerly resolve/freeze them before those preceding effects or force them to equal the Study whose XML was just written if actual global selection changed.

Candidate `ConversionSession::modelManagerOwner()` and `ModelManager::ownerSession()` explicitly project native main-window `+0x50` and manager `+0x10`. `currentModel()` checks weak liveness plus current list membership, and each current Mo `DbHandle::moProject` is the actual Project backlink. These are the correct existing identities to bind and resolve at invocation.

Require the same actual Session manager, the same Project/dataDb backlink when non-null, and its actual SUB/Study aliases. A selected Mo model with a missing Project reaches the existing candidate lazy-reconstruction guard; never fall back to `session.project()` or the passed saved Study. `releaseProjectAfterCloseEffects()` is storage-only and can leave a selected model with `moProject=null`; that is distinct from native ModelManager clear/no-current state. The table owner can survive both.

## Interface that can bind without fabricated UI

Use **one canonical Session-owned main-window/table admission state**, consumed by the reached public XML-to-refresh continuation. This is a shared integration seam, not another independently assumed receiver profile. No new source was added here.

The binding can immediately capture weak identities of the actual Session's existing ModelManager, notifier and WidgetRuntime, validating `manager->ownerSession()==&session`. Every operation should receive the current actual Session and revalidate those identities, avoiding dereference of an escaped old Session pointer. A Session-lifetime anchor is an adapter scope identity, **not** an SfMoMainWindow QWidget or QObject.

The admission state's data should distinguish:

- main-window field **Unavailable / KnownNull / actual cached table identity**;
- actual source initialization commit (`0x140019760`), allocation and pending constructor generation;
- actual table QObject/timer/UI constructor receipts, global registration, constructor return and cache-publication commit separately;
- dock-install return separately from field publication;
- native deleting-destructor completion and post-return field clear separately;
- actual QObject/native-owner death from adapter-scope release;
- invocation-time current-model/Project resolution, not a copied current Project.

The holder must begin **Unavailable**. Source-bound field initialization can mark KnownNull without asserting Qt widget construction. A getter then stops before its first unavailable allocation/constructor boundary. Successful constructor publication must use the exact actual allocation/return owner and control block. No default QWidget attributes, QTimer inactive state, UI pointers, selected row or empty notifier population is admitted by the holder alone.

Current `MainViewState` is a separate `SfMainView`/manager widget, not the native main window or table. `WidgetRuntimeState::bindWidgetComponent` explicitly does not register a widget or certify constructor stages. Neither may be aliased/recast into the missing table. Existing `WidgetAreaLazyParent` demonstrates canonical parent-field admission and interrupted constructor retention, but its field `+0x3c8` and native `0x140018460` contract cannot be reused as the different `+0x458` getter.

The candidate already reaches immediate InputTable XML through the shared Type2 writer and then refuses with **`Type2 study Save: signalDoeTableRefresh() receiver delivery remains unsupported`**. Bind the canonical owner state at that exact message phase, preserve XML result/error/partial bytes, and keep the next actual timer/UI guard. A holder or static source fact alone does not justify bypassing that delivery guard. Nonempty B121 still has its independent earlier reconciliation/reader/serialization prerequisites.

## Acceptance boundary

An integrated follow-up should exercise the actual Session and public Type2 XML continuation, covering:

- unknown field versus source-initialized null; cache hit versus allocation/constructor; no constructor/registration inference from an owner token;
- constructor interruption before/after timer allocation and before global registration, retaining prior effects while leaving the cache unpublished;
- registration before publication, exact allocation/result identity, and dock failure after publication without rollback;
- explicit table deletion before field clear, parented timer lifetime, Project release preserving table admission, and adapter scope release invalidating escaped diagnostic access without claiming native cleanup ran;
- actual no-current-model, non-Mo, current Mo backlink, missing/lazy Project and selection-change branches at the reached refresh lookup;
- false and true XML results preserving their earlier effects and reaching the same no-argument receiver stage, while unresolved timer/UI work remains guarded.

These would be source-contract integration tests, not original runtime acceptance. No tests were authored or executed in this lane.

## Evidence/version receipt

Original PE SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. New `analysis/resume67-doe-table-lifecycle-pe.txt` is a **22,883-byte** bounded selection containing the constructor/init/cleanup/zero/delete and current-Project getter bodies. Capture SHA-256 `cf4ede249a70d2e8dc25abc929fa53a6f1ab3c23fa33f6f1ad72cc3360ff71e6`; range/hash receipt `analysis/resume67-doe-table-lifecycle-pe-receipt.json` SHA-256 **`078d68396c43a7d875c11cf96615eb6fd6305d5638d36c25f4d293b6f98e1141`**. Microsoft dumpbin14.44.35228.0 was used offline. The receipt distinguishes the RET-bounded zero leaf from selected unwind bodies; direct-call candidate sites were confirmed by disassembly.

CP65/66 receipts retain table/timer/UI construction, publication, dock and first-effect evidence. Current source was read at these hashes:

| Source | SHA-256 |
| --- | --- |
| `engine/conversion_session.hpp` | `09feedab6ca1ac321deee04eecf5ff51bd6c1b2302c396ee4de0a3af5acfcebe` |
| `engine/conversion_session.cpp` | `fb54fd5f79dbbe6660c0cc0284015bff8d33a8ae9495bf88de8b985b7f55a8fd` |
| `engine/model_manager.hpp` | `7c55be276bc833f1681d81ebccbc87612d9e3f61e7d486c9388e43dda7202e59` |
| `engine/model_manager.cpp` | `69f426a43ffead038991178fe7106c3fd3f4b46b93fc4968b48d0dbd2f94972d` |
| `doe/type2_xml_prefix.cpp` | `1db2d6bd3c3a78773a31e505775bb4c91cbee2e480b7567c6523d9a4337050aa` |
| `moproj/write.cpp` | `31fa67461d534b99ced922cf9361b1497054d291d064ccb55e9ed198488e4a0d` |

No runtime parent/child/QObject/timer state, full native constructor/destructor closure, actual refresh population or end-to-end active DOE success is established. Token measurements are unavailable.
