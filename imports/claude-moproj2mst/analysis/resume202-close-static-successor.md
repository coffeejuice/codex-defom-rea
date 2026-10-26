# RCLOSE source-readiness successor after CP169/CP172

## Decision

No new production source increment is justified by CP169/CP172. The narrowest honest next action is a passive static-PE occurrence audit, then hold the current Close guards until a same-owner lifecycle occurrence is available. Do not add another Close state/helper or alter storage cleanup.

## Reconciliation

The frozen Close helpers still stop at their intended boundaries in current source:

- `close_entry_prefix.cpp::attemptCloseEntryPrefix` requires a Session-owned manager and qualified entry observations. With a real nonnull `moProject`, it returns `MoProjectCanCloseUnavailable` before modal/callback/busy effects.
- `close_empty_manager_tail.cpp::attemptCloseEmptyManagerTail` snapshots the actual Session manager population and refuses a nonempty child at `0x140c784f2:owned child deleting virtual+0x18`; it cannot be advanced by an external clear or changed population.
- `ConversionSession::releaseProjectAfterCloseEffects` remains a diagram-null-gated adapter storage endpoint. It does not coordinate native Close, invoke feature cleanup, or establish that native effects ran. The Close ownership and cleanup reports (CP117/120/144/149) already establish why the Session destructor and empty-tail helper cannot stand in for that protocol.

CP169 concerns a retained QObject-private `+0x50` zero-binding admission; CP172 adds a root-owned QVector shared-null binding at QtCore `0x1311c7..0x1311d2`. Their validation records show conditional connector/runtime cuts and incomplete objective acceptance. Those cuts do not bind an accepted Close callback, an actual main/manager/Project owner, or a property/QPointer invalidation to the same Close occurrence. The CP169 Report owner audit separately finds the Close document receiver is freshly reread from `S+0x30`, not tied to the Open/Save document; it cannot be repurposed as a Close cleanup witness.

## Exact missing occurrence

The unresolved evidence is one source-produced accepted-Close occurrence on the same concrete main `M`, manager, Mo child, and current Project `P`: action/slot delivery through the registered route; successful earlier acceptance/effects; child deleting virtual `+0x18`; Project feature-release effects and its diagram/SUB/member teardown; subsequent Mo backlink/slot/manager-current cleanup. CP153 closes the static registration edge (`M -> Close All action -> slotActionFileCloseAll -> 0x1400299a0`) but explicitly does not prove connection success, signal delivery, acceptance, or preservation of Project identity through callbacks. CP149's application-exit route is a different conditional occurrence and likewise does not supply those owners/effects.

Thus the missing item is not another static target address: it is the same-owner *occurrence and mutable-field history* that connects the known static chain to a native object lifetime. Without it, there is no exact source-ready behavioral cleanup increment. Existing guards should remain unchanged.

## Passive evidence route

Use only the retained original static PE and retained disassembly/decompile artifacts: cross-check the CP153 action/slot edge and Close entry, then trace static references to the relevant QObject/QPointer/property writers and the Project/SubProject destruction tail. Record only instruction-bounded ranges, section-mapped bytes, and identities for each writer/reader; distinguish static reachability from runtime occurrence. The expected output is a bounded cross-reference table that can either identify a previously unqualified writer/reader dependency or confirm that static evidence stops at the already-known conditional chain. No new GUI input, original launch, dynamic REA session, source change, build, or test is needed for this route. A passive static pass cannot prove signal delivery or accepted same-instance cleanup; that requires a future permitted runtime occurrence with retained owner identities and field history.

Only this analysis note is added. The converter source, build, tests, original executable, GUI control, REA state, and parent recovery state were not changed.
