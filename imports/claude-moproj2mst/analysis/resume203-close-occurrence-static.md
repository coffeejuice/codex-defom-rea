# CP203: Close occurrence and cleanup-prefix static feasibility

## Result

This converter-only pass finds no passive-static route that can establish a same-owner accepted `Close` occurrence, and no new source-ready cleanup prefix. The remaining gap is occurrence-correlated mutable state across callbacks and destructive calls. Static disassembly can show which receiver/fields are loaded at each site and that one conditional call path exists; it cannot show that the callbacks ran, returned, preserved a concrete Project generation, or accepted Close for that generation.

The only viable passive evidence route is a bounded re-audit of the already retained PE and its section-mapped instruction records, joined to existing Qt/Project/SUB owner receipts. That route can confirm path edges and conditional branch order, but CP202/153/156 already cover those edges. It cannot produce the requested occurrence witness. A new static pass would therefore be duplicate evidence without a new owner fact.

## Read-only scope and evidence identity

Only this report was written. No source, build, test, original process, GUI/app-control, mutable REA session, deployment, or shared recovery state was touched. The converter project is `source/apps/moproj2mst`; the native original used by the retained static records is `analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe`, v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Read the CP202 writer/xref audit and successor, then reconciled the controlling retained CP153 accepted-trigger, CP156 delivery-admission, CP149 post-event-loop route, CP120 ownership-order, and CP142/143 SUB/Project destruction qualifications. Also inspected the current converter `src/core/convert.cpp`, `src/engine/close_entry_prefix.cpp`, and `src/engine/close_empty_manager_tail.cpp` to check whether a narrower implementation cut had become possible.

## Why static evidence cannot close the occurrence

- CP153 qualifies the construction and registration intent from main `M` through the Close All action to `slotActionFileCloseAll()` / `0x1400299a0`. The native `QObject::connect` result is ignored; there is no observation that a signal was emitted or delivered.
- CP156 refines signal normalization and AutoConnection's thread/receiver-thread condition. These are dispatch rules, not a delivered event record. No source-produced receipt supplies the actual signal occurrence or receiver/thread state.
- After entry, `0x1400299a0` re-reads global/current main `W`, then can-close from `M+0x50`; accepted effects may invoke callbacks before the fresh manager read/cast at `0x1400133a0`. Static control flow does not identify the allocation generation at those reads or establish that callback writes left those cells unchanged.
- The disposal path (`0x140c78300` child deleting virtual `+0x18`; Mo `+0x19c0` Project release; `0x14008a3c0` Project destructor; `0x1400a80e0` SUB iteration) is conditional on those gates and current pointers. It cannot establish that the Project being destroyed is the same Project previously saved, nor that feature release and later callbacks completed for that instance.
- CP149 is a separate conditional QApplication event-loop-return/main-destructor route. It establishes another possible teardown order, not an accepted FileCloseAll occurrence.

Thus an instruction-bounded xref or even a statically complete direct-call chain cannot witness `accepted = true`, signal delivery, callback completion, or a same-owner lifetime interval. A runtime occurrence with concrete `M/W/G/Mo/P/S` identities and allocation epochs plus field/callback history is required for that claim.

## Source-ready cut check

`attemptCloseEntryPrefix` still requires a Session-owned manager and qualified busy observations. When the actual post-Open Mo child has a nonnull Project, it stops at `0x140044143` (`MoProjectCanCloseUnavailable`) before modal/callback/accepted effects. `attemptCloseEmptyManagerTail` snapshots the real manager population and refuses a nonempty child at `0x140c784f2`; it only calls `manager.clear()` after proving the population and connection list empty. Those stopping points are intentional and neither can be advanced by assuming accepted Close or projecting a native deleting virtual into ordinary shared-pointer cleanup.

`core/convert.cpp` remains a per-invocation Open/Save adapter and does not dispatch the native FileCloseAll slot or own a QApplication/main lifetime. `ConversionSession` cleanup remains an adapter storage-release path. A static cleanup helper would have no source-produced owner/effect predecessor to justify its call. The narrowest already supported future cut remains the one stated by CP153: produce the actual main and bind its `+0x50` manager, keep mutable application `W` separate, then source the fresh busy and can-close inputs. Even that cut must stop before unavailable modal/callback effects. It is not newly enabled by this pass.

## Next evidence

No additional passive-static query is recommended unless a new retained image/receipt or changed source gives a specific unqualified writer/reader edge to resolve. To proceed beyond current guards, collect one permitted source-produced Close occurrence that retains the same concrete `M`, `G`, `Mo`, and `P` generations through action/slot delivery, fresh admission, modal/nested-Save results, accepted callbacks, Project feature release, and deletion. Record any owner change instead of assuming equality. Until then, keep both helper boundaries and storage cleanup unchanged; do not claim destructor entry as accepted Close.

## Qualification

This is a feasibility/duplication assessment of retained static records and converter source. It is not a fresh original runtime observation, a new REA Evidence record, proof that a connection fails, or proof that accepted Close is globally unreachable.
