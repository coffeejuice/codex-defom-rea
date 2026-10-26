# CP141 DBLIST generated-item receiver audit

## Conclusion

The retained static evidence does not bind the `signalGetTaskBase(QString)` result used by DBLIST/ReportList to a concrete `SfTaskReportGenerate` instance. It establishes a plausible conditional vtable target, but not the active receiver or an implementation cut that can safely consume the element. Keep the candidate public guard.

## Evidence chain

- `analysis/resume65-gettaskbase-dispatch.md` maps `SfMoMainWindow` meta-call record 17 (`slotGetTaskBase(QString)`) to `0x1400db504`. That body passes the QString key to `FUN_14003c630`, then stores its returned pointer in Qt's return slot. The getter reads a value from the main-window container at `this+0x4a0`; it constructs no object and does not validate its vptr.
- `analysis/resume66-report-taskbase-bridge.md` and `analysis/resume60-dblist-signal-next.md` record the DBLIST/ReportList wrapper call using dispatcher global `0x143d76700`, signal spelling `2signalGetTaskBase()`, and a constructor connection to a no-argument slot spelling `1slotGetTaskBase()`. The wrapper resolves its QObject receiver indirectly through registry state at `global+0x88`. The evidence does not show that the invocation reaches the QString-returning MainWindow meta-call, or that the resolved receiver is that MainWindow instance.
- `analysis/resume131-dblist-metaobject-next.md` binds the wrapper's dynamic invoke receiver to an object pointer derived from the current registry item (`item+8`), but does not identify the item's dynamic class or its relationship to the MainWindow getter.
- `analysis/resume65-gettaskbase-dispatch.md` shows the getter result is whatever pointer is already stored under the supplied key. Its lookup does not establish who inserted that value or what its vptr is.
- Conditional behavior is known: for an object whose dynamic type is `SfTaskReportGenerate`, `+0x218` resolves to `0x1407a0150` (slot 67), as captured in `analysis/resume63-reportgenerate-slot67.asm` and the retained vtable inventory. This is not evidence that the generic getter returned such an object for the active DBLIST occurrence. Those captures are qualified against original `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.
- The retained prototype-constructor inventory does not close the insertion edge. In particular, `analysis/wave2-doe-task-prototype-ctor-1407a0750.asm` shows a constructor writing addresspoint `0x1426a2508`; the retained `SfTaskReportGenerate` primary table is `0x14269e558`. No evidence links that constructor, key `SfTaskReportGenerate`, and the MainWindow map entry. Proximity or the selector spelling is insufficient to infer the relationship.

## Exact missing occurrence

To bind the active object, evidence must connect, for the same DBLIST call:

1. The wrapper registry item and resolved QObject/method pair.
2. The no-argument signal/slot registration to the method that consumes the QString selector and returns through the generic `SfTaskBase*` slot (or show a different route).
3. The key's map insertion/factory result to the exact pointer returned on lookup, including its runtime vptr (or null).

Without these links, dispatching `+0x218` as `SfTaskReportGenerate` would assume the unresolved type. The candidate source's DBLIST/ReportList refusal should remain. Even after this type binding, the separate AppState `+0x1c` admission value/lifecycle remains unqualified for public Open.

## Acceptance boundary

This audit used retained static PE captures and candidate-source evidence only. No source, build, GUI, runtime, or mutable REA state was changed. No candidate-only or original parity claim is added.
