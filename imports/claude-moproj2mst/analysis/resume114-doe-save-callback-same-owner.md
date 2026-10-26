# CP114 Type2 Save: same-owner refresh boundary

## Finding

There is no complete source-closed callback branch or receiver owner that can be integrated in the current non-GUI candidate. The exact boundary is after `writeType2InputTableXmlPrefix` has returned and its `NormalWriterReturn` has been recorded, but before `DoeTableOwnerState::requireRefreshDelivery` at `moproj/write.cpp:159`. Native emits a no-argument global key there; it does not pass the cached Save Study/manager or a target receiver. Static registration identifies a possible receiver class and slot, not the receiver chosen or its return in this occurrence.

## Static path and first effect

Original GUI PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The Type2 Save caller caches the selected SUB's `+0x20` manager before XML. `0x1400d3200` emits `2signalDoeTableRefresh()` at `0x1400d369d` after the InputTable writer normal-return path, including an ordinary false writer result. The global dispatcher checks suppression, looks up and snapshots its ordered key registrations, skips dead guards, and invokes live receivers through DirectConnection. A suppressed/empty/dead-target path can return without a slot; direct delivery enters `SfMoDoeTable::slotUpdateUI()`. The invoke Boolean itself is ignored.

For direct table delivery, slot `0x1400f4e10` calls `QTimer::stop()` on `receiver+0xb0` at `0x1400f4e25`, then calls the same receiver's virtual `+0x1b8` at `0x1400f4e38` (identified primary target `0x1400f47e0`). Its normal return is required for synchronous dispatch to return. The receiver refresh resolves the then-current global Project/active SUB; it is not given the Save frame's cached manager. Save's later sample calls use the original cached manager. Record both values; do not impose equality as a native branch predicate or replace the cached manager with the table's refreshed value.

The receiver is statically bound to a global registration only at constructor `0x1400ebc5c` (`this`, key `2signalDoeTableRefresh()`, slot `1slotUpdateUI()`). The main-window getter publishes that completed table at `+0x458`; the dock uses `+0x460`. These edges do not encode a per-Save occurrence token linking selected SUB/manager to the table, timer, dispatch snapshot, current-study lookup, or normal slot return.

## Candidate boundary

Candidate `moproj/write.cpp:143-159` validates the Type2 Project/SUB/owned Study/I/O/ModelManager, performs immediate InputTable XML I/O, validates the normal-writer journal record and records the refresh boundary. Its `DoeTableOwnerState` checks Session admission scope but owns no QObject, QTimer, actual typed registration population, or Qt dispatch. Timer state and native receiver liveness remain unavailable. There is no code after the guard to reach PAR outputs.

The exact same-owner observation needed to cross this boundary is an occurrence-correlated trace or a real adapter receipt that records: (a) the Save's Project/SUB/Study/I/O/manager identity and XML Boolean/normal return; (b) notifier suppression and the ordered live/dead target snapshot for the key; (c) the resolved receiver equals the live published `+0x458` table generation; (d) its actual child timer is the object passed to `QTimer::stop()` and that call returns; (e) the same receiver enters its `+0x1b8` refresh and both refresh and slot return normally; and (f) Save resumes with its original cached manager. Capture the refresh-time Project/SUB/manager separately. If verifying that the same Study was active during UI refresh, compare those captured values; that is an observational equivalence question, not a native equality check that authorizes Save continuation.

No retained static artifact supplies these per-occurrence values or return. Reconstructing an empty receiver list, setting suppression, treating a metadata identity token as QObject liveness, or passing a synthetic “callback returned” flag would choose an unobserved route. A detached test of PAR bytes cannot close the dispatch edge. Keep the public Save/output guard unchanged; no isolated helper/test is warranted in this lane.

## Evidence and scope

Reviewed CP112 callback-owner report and CP65 slot/registration/refresh receipts, plus CP90/CP101/CP108 manager and boundary analyses. The bounded slot body is `analysis/resume65-doe-refresh-slot.asm`; complete body and registration metadata are inventoried by `analysis/resume65-doe-refresh-pe-receipt.json` (SHA-256 `BB41B752BF8F46F8B6449F596F118B639B4B501913D984B3362B59939F03520B`). Candidate source was read-only. No code/test edit, build, GUI/original launch, or mutable REA operation was performed.
