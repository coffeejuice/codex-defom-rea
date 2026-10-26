# CP108 DOE Save: earliest refresh-owner binding

## Result

Static evidence does not bind the Type2 Save occurrence's cached manager to a particular live `SfMoDoeTable`, timer, active-study context, or normally returning refresh slot. It does identify the first possible receiver binding: table construction registers `this` for the global `2signalDoeTableRefresh()` key, and the receiver is cached at main-window `+0x458` only after constructor return. Save emits the key without a receiver pointer. The global dispatcher resolves registrations at emission time. Therefore no static pointer-flow connects the saved SUB/manager to the actual receiver chosen for that occurrence.

The earliest source-closed candidate leaf remains the pre-XML Save owner check plus InputTable XML normal-return observation. The next candidate call is the unconditional `DoeTableOwnerState::requireRefreshDelivery()` guard. Advancing requires an occurrence-specific runtime delivery witness; no further source-only identity shortcut closes it.

## Where the static chain stops

Original binary SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

- Save selects a Type2 SUB and caches its `+0x20` manager in the Save frame. The same pointer is used for XML and later sample calls (`resume90-doe-save-refresh-binding.md`).
- `0x1400d3200` emits the no-argument refresh signal after InputTable writer normal return, including ordinary false (`resume103-doe-save-reconcile-1400d3200.asm`). The signal call carries no manager, Study, SUB, table, timer, or callback token.
- The lazy main-window getter uses `+0x458`; constructor `0x1400eb6f0` registers its `this` at `0x1400ebc5c` for the global key. The constructor then returns and the getter publishes the returned pointer. This is a potential receiver owner, not proof that a particular Save reached a published/live table.
- Dispatcher `0x1402cf3c0` checks suppression and looks up/snapshots the key's ordered registration list, skips dead guards and calls `QMetaObject::invokeMethod` with DirectConnection for live entries. It can return without invoking the table. The invoke result is ignored. No retained static data binds the per-run suppression value, actual list contents, liveness, or selected entry to one Save occurrence.
- Only on the direct table route does `slotUpdateUI()` stop `table+0xb0` at `0x1400f4e25`, then call refresh virtual `+0x1b8` at `0x1400f4e38`; refresh reads the then-current global Project/active SUB. Slot normal return depends on that synchronous path completing. Static call order does not prove the QTimer call or refresh returned normally in the requested occurrence.

The saved manager and the table's refresh-time manager are separate values. A runtime record should capture both and preserve their distinction. Equality is evidence that the same Study was current at refresh; inequality is possible under the retained call structure and must not be erased by re-querying the active SUB for the Save continuation.

## Candidate check

Candidate `source/apps/moproj2mst/src/moproj/write.cpp:143-159` validates/publishes the Type2 study node, writes/journals InputTable XML, observes its refresh boundary and calls `table->requireRefreshDelivery(*session)`. `type2_save_node_prefix.cpp` validates the live Project/SUB, owned Study alias/backlink, StudyIo and ModelManager identity. `type2_xml_prefix.cpp` rechecks those identities during XML output, but its strong `Study` local ends when XML returns; the prefix carries a weak Study owner. `table_owner_state.cpp:232-236` validates only Session admission scope and then refuses timer/UI delivery. Its fields deliberately leave receiver liveness, timer state and widget state unavailable. `SessionNotificationState` has no DoeTableRefresh typed registration list; the known fresh Session suppression value is false, which makes unavailable registration population the next missing discriminator.

None of these candidate owners is the `SfMoDoeTable` QObject, its QTimer child, or a live native registration. A diagnostic `TableOwnerIdentity`, a Session field marked KnownNull/Cached, the existing boolean receiver registry, or an unpopulated typed key cannot certify that `0x1400f4e25` executed and returned. Repeating table-constructor static inspection cannot supply the missing occurrence data.

## Exact runtime witness needed

For one active Type2 Save occurrence, a runtime trace/instrumentation receipt must correlate:

1. **Save selection before XML:** Project pointer, selected SUB pointer, its `+0x20` manager pointer, and Study identity as observed by the Save arm.
2. **Emission and dispatch:** writer result (including ordinary false), entry to `0x1400d369d`, notifier suppression byte, ordered key registration snapshot with receiver guard/liveness, and the concrete receiver selected or the concrete no-delivery return path.
3. **Receiver binding:** table object pointer/generation matches the live main-window `+0x458` cache; its `+0xb0` timer pointer is its child and the same timer is passed to `QTimer::stop()` at `0x1400f4e25`; capture normal return from that call.
4. **Refresh and return:** same table pointer enters virtual `+0x1b8 -> 0x1400f47e0`; capture current Project/active SUB/manager resolved by refresh, its normal epilogue/return (including `+0x98` clear), and normal return through `slotUpdateUI()` and the signal-dispatch call.
5. **Resume owner:** after the callback, show the Save arm continues with the original cached manager from item 1, not the refresh-time manager from item 4.

This makes absent/suppressed delivery distinguishable from a live callback, and distinguishes callback return from successful `QMetaObject::invokeMethod` status (which the native dispatcher ignores). A runtime witness may show that refresh used another active SUB; that is a fact to retain, not a basis for substituting that manager into Save's later outputs.

## Stop condition and next action

No such runtime witness exists in the retained static captures reviewed here, and this lane was explicitly read-only with no GUI/original execution. Stop at the guard. The next useful evidence action is a single correlated runtime trace of the items above for an actual Type2 Save; absent that, the refresh callback/return remains the exact missing owner, and current-PAR/mode sample output must stay unreachable from public Save.

Reviewed retained evidence: `resume65-doe-refresh-owner.md` and receipt `resume65-doe-refresh-pe-receipt.json` (`BB41B752BF8F46F8B6449F596F118B639B4B501913D984B3362B59939F03520B`); `resume90-doe-save-refresh-binding.md`; `resume101-doe-refresh-runtime-witness.md`; `resume103-doe-save-reconcile-static.json`; `resume72-doe-manager-after-refresh.md`. Candidate source review only; no code, build, GUI, original runtime, or mutable REA database used.
