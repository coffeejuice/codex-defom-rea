# RGRAPH CP156: populated-cell writer next witness

## Result

Static evidence still does not identify a proven populated-cell writer or the derived `slotItemChanged(QStandardItem*)` body. Keep the populated-cell refusal and preserve existing guards. The smallest useful next witness is a completed, bounded machine-code decode/decompile of `0x140d78490` with its caller context from `0x140d78220`, sufficient to establish (or rule out) `QStandardItem` construction/setters and insertion into the exact `SfObjectManager` returned by `0x140c90210`. If that body delegates, follow only the concrete insertion callee; separately resolve the derived Qt meta-call target only if item changes need their native side effects/order.

## Evidence boundary

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (identity recorded by CP143/149; CP149 reports unchanged after its read-only pass).

- `analysis/resume143-property-cell-writer.md`: vtable slot +0x140 at `0x140d78220`; bounded calls include `0x140d78490` and `0x140d7a9f0`; actual same-owner callers `0x140c80a20` and `0x140c80b00` dispatch via manager accessor `0x140c90210`. CP143 explicitly leaves candidate `d78490` unproven.
- `analysis/resume145-property-cell-rea.md`: bounded REA request for updater/callees/base slot returned no function results; close status unconfirmed. Do not treat this as negative code evidence or repeat the same batch absent provider status change.
- `analysis/resume146-property-cell-rizin.md`: exact `s 0x140d78220; pd 80` attempt unavailable (`unsupported_provider`), no disassembly/evidence ID.
- `analysis/resume149-property-cell-static.md`: offline scan and retained captures did not close writer or derived callback. Constructor `0x140d78100` proves signal/slot strings connected on the same manager receiver, not callback effects.
- `analysis/resume60-graph-owner-xref.md`: prior graph-owner pointer xref boundary at `0x140c15910` remains unresolved; no owner inference from factory wrapper.

Known bounded addresses: `0x140d78220` updater; candidate callee `0x140d78490` (`.pdata` through `0x140d7974a`); other direct callee `0x140d7a9f0`; lazy manager accessor `0x140c90210`; same-owner dispatch callers `0x140c80a20`, `0x140c80b00`; constructor/connection `0x140d78100`; base item-change slot `0x140d3d8c0`. The smallest missing proof is the receiver-preserving edge from the candidate updater into an actual item insertion/setter. Addresses alone do not establish that behavior.

## Next witness specification

Obtain one completed retained body for `[0x140d78490,0x140d7974a)` (or a narrower function body if exact `.pdata` metadata revises the bound), plus enough of `0x140d78220` to identify the `this`/manager argument. Record target hash, tool completion receipt, instruction/decompile range, and call-site argument flow. A positive result must show the inserted item collection is the same `SfObjectManager` object from `0x140c90210`, and identify row/column/value source before any source model change is considered. A negative body result should state which operations are ruled out and then follow the single concrete delegate; do not widen to another broad batch.

No source, build, tests, GUI, mutable REA state, or executable bytes were changed in this task.
