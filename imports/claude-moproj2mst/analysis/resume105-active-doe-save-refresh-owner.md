# CP105 active Type2 Save refresh owner boundary

## Finding

The native Type2 Save arm statically selects and caches one manager before XML, while `DoeTableRefresh` targets a separate main-window `SfMoDoeTable` whose synchronous refresh resolves the then-current Project/SUB independently. Native Save resumes with its original cached manager only if the signal-dispatch call returns normally. Candidate code has a source-bound Study/I/O/manager prefix and records XML normal return (including ordinary false), but the next public step is still an unconditional guard. There is no source-closed candidate leaf after that boundary: the first native observable receiver effect is a real `QTimer::stop()` on the live registered table.

## Native selection and callback order

All native offsets below refer to original `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

1. The Type2 arm in `0x140099040` selects the active SUB, checks its Type2 kind, calls `0x1400b5a90` to read its `+0x20` DOE manager, and caches that returned pointer at `[rsp+0x90]` (`analysis/resume90-doe-save-refresh-binding.md`). The same cached identity supplies the study node, InputTable writer, current PAR and fixed mode 0/1/2 writers; no later active-SUB lookup replaces it.
2. `0x1400d3200` invokes Save reconciliation `0x1400d2b60`, then prune/variable processing and InputTable writer `0x14090a550`. Once the writer reaches its normal return path, including ordinary false, it emits the no-argument `signalDoeTableRefresh()` at `0x1400d369d` (`analysis/resume103-doe-save-reconcile-1400d3200.asm`). The signal result is not used to branch around the later outputs.
3. The global dispatcher `0x1402cf3c0` can suppress delivery or snapshot ordered registrations and invoke live direct receivers. The table is main-window cached at `+0x458`; construction registers `SfMoDoeTable::slotUpdateUI()` under this key. If that receiver is invoked, the slot first stops its child timer `table+0xb0` (`0x1400f4e25`), then calls refresh virtual `table+0x1b8` (`0x1400f4e38`). The refresh consults the global current main window/Project and active SUB and updates its own cached manager; it is not passed the Save arm's cached manager. The exact construction/dispatch chain and static limits are in `resume65-doe-refresh-owner.md` and its receipt.
4. On normal dispatch return, Save continues with the original stack-cached manager. Thus a candidate must not use `SfMoDoeTable`'s refreshed current-manager projection as a substitute for the manager selected before XML, nor reselect the active SUB after the callback.

Static control flow establishes the possible route and identities, not a particular run's table liveness, registry population, suppression state, actual invocation, timer effect, normal slot/refresh completion, or equivalence between refreshed current manager and the original cached manager. A suppressed/no-live-target return is also a distinct route from direct slot invocation; neither can be selected from the static registration alone.

## Candidate seam and earliest unresolved owner

Candidate `moproj/write.cpp:143-159` calls `appendType2SaveNodePrefix`, then `writeType2InputTableXmlPrefix` for the empty-variable case, verifies the latest XML writer normal-return journal entry, records the refresh boundary, obtains the actual Session's table-owner projection, and calls `requireRefreshDelivery`. That method currently refuses at `signalDoeTableRefresh() timer/UI receiver delivery remains unsupported`.

Before this refusal, `appendType2SaveNodePrefix` validates the live Project/SUB, owned `Study`, `Study::sub`, `Study::doe` alias, configured StudyIo and Session ModelManager identity. The XML phase locks and rechecks those same identities while it performs/open-journals the InputTable write. These are the earliest source-closed candidate owner checks and XML effects already integrated. The Save prefix result carries the Study as a `weak_ptr`; XML holds a strong local only for its own call. The candidate does not have a source-owned frame that spans a real receiver callback and resumes with the original selected Study/I/O/manager identities.

The first missing effect is not another sample serializer. It is the delivery outcome at the existing boundary: identify the actual `+0x458` table generation and its live same-key registration/suppression state; distinguish no delivery from direct invocation; bind the timer to that receiver; then observe timer stop and normal return from the direct slot's `+0x1b8` refresh. Only after that return may the call chain advance. `DoeTableOwnerState` stores diagnostic owner tokens and constructor-stage receipts; its timer, receiver-live and active-state fields intentionally remain unavailable, and it does not own or invoke a QObject/QTimer or the complete global registration set.

There is no safe source-only bypass or detached immediate helper that closes this missing transition. Treating the absent candidate GUI receiver as native no-target behavior would invent the original runtime registry state. Adding a strong Study lease alone is also insufficient: it cannot establish native table dispatch or the raw manager's liveness across current-Project changes. Keep sample outputs guarded pending a real source-owned synchronous delivery/result frame that preserves the pre-XML manager identity and records actual receiver/return state.

## Narrow next action

The first candidate integration requires an actual `RefreshDeliveryReceipt` emitted by a source-owned table/dispatcher adapter at this exact post-XML boundary. Its minimum useful fields are the original selected SUB/Study/I/O/manager identity, table generation and live registration key, suppression/no-target/direct-invocation outcome, timer identity and stop result, and normal return from the slot refresh. It must preserve ordinary XML false-result continuation, preserve already-recorded XML effects on a later callback stop, and resume against the manager cached before XML. If the converter has no actual Qt table/dispatcher owner to supply this receipt, public current-PAR and mode 0/1/2 continuation remains a runtime-owner blocker rather than an isolated output-helper task.

## Evidence reviewed

- Original PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- Save arm and post-XML caller: `analysis/resume90-doe-save-refresh-binding.md`; `analysis/resume103-doe-save-reconcile-static.json` (capture for `0x1400d3200`, SHA-256 `73f96c7d1b7861059fef9cd1a71dd7fc0283221c272a145b4b7ee027db318e08`).
- Table/slot/refresh owner: `analysis/resume65-doe-refresh-owner.md`; receipt `analysis/resume65-doe-refresh-pe-receipt.json`, SHA-256 `BB41B752BF8F46F8B6449F596F118B639B4B501913D984B3362B59939F03520B`.
- Candidate source SHA-256: `moproj/write.cpp` `8AF4BE80324C39E1F8F452D7723BEBD200E4B7372180D8E32C1E6EE32C2399A2`; `doe/type2_save_node_prefix.cpp` `A3B64864214915E0C775019E8B0BD4A10F6B35E62392B5908930183FE1F5DEC3`; `doe/type2_xml_prefix.cpp` `10BD0E9690F62387E394EB603D7F90619AA81304F9FD8127F7FA58C5F14D4D57`; `doe/table_owner_state.cpp` `D57B8A6BA6F6DAC0D7D9EFB515B73A6DAFBC10BA4213E5681C37744FAF5FCFCF`.

Read-only retained-capture/source review only. No code, build, GUI, original runtime, or mutable REA database was used.
