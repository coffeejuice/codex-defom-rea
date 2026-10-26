# CP124 — smallest retained witness for DBLIST admission

## Decision

The retained evidence does not yet bind the `+0x1c=1` setter to singleton `0x143d766b8` or prove the field's value at later DoeOutput dispatch. The smallest **already present** receiver-side witness is the constructor slice: CRT initializer `0x141ca0bb0` passes the singleton to `0x1402c7ee0` / `0x1402c7f80`, whose last state-store prefix explicitly writes `this+0x1c=0` at `0x1402c8030`. That closes startup value immediately after AppState construction, not later project history.

For a later writer, the narrowest retained route worth following is the DBLIST handler's post-gate Qt meta-object path. It is already captured, but its generated receiver/slot implementation is not. This is a plausible route for an effect on a subsequent gate read only: it executes after the current call has passed the `+0x1c==0` check. The current artifacts do not show that it invokes the setter or writes this global.

Keep the public DBLIST guard. Do not infer that this dynamic path is a setter or make startup zero a per-conversion value.

## Known gate and construction boundary

* Original PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
* Startup owner chain and exact raw range hashes: `analysis/resume116-dblist-setter-slice.md`, SHA-256 `38b5a2c21b327afcc6d7150ea27a6b7d1fbb1366b43730245c92c9b5cd345de1`.
* Active Type2 DoeOutput caller at `0x140651eba` passes global `0x143d766b8` to predicate `0x1402cbec0`; it admits when receiver `+0x1c` equals zero before DBLIST parsing. See `analysis/resume62-dblist-register-and-caller.asm`, SHA-256 `438bbe2b9ff00ab35f7ad8554e762044fdfa589182d61143157fb02f8ab39af1`.
* Setter body `0x1402cbf30` writes one to its receiver-relative `+0x1c`. Prior direct-call/data-pointer and broader receiver scans did not find a caller that proves the receiver is the gate singleton; see CP117 and CP120. Those negative searches remain coverage-limited.

## Smallest currently available static route

The retained complete handler capture `analysis/resume56-dblist-handler.asm` (SHA-256 `0c87637089cd61388f050296f700aed4df8e1416176737bf78d2eb0842af3b7a`) shows:

1. Handler `0x140655df0` begins with local `[rsp+0x78]=null` and later builds a generic argument typed `SfTaskBase *` for that slot.
2. It calls wrapper `0x1402cf740`, which reaches Qt `QMetaObject::invokeMethod` through IAT `0x141e05348`.
3. The member string is exactly `2signalGetTaskBase()` and the paired data string is `SfTaskReportGenerate`; CP58's bounded `.rdata` receipt and `analysis/resume58-dblist-literals.txt` qualify those strings.
4. If the local pointer becomes non-null, the handler calls its vtable slot `+0x218` at `0x14065618c..0x1406561a4`, then returns. The actual QObject receiver, concrete class/slot target, and any state effects are unresolved.

This handler path cannot alter the gate value for the occurrence that entered it because the gate branch at `0x140651eba` has already been evaluated. The smallest existing evidence that could advance the next-occurrence question is the native Qt meta-object data and concrete initialization/publication path for the `SfTaskReportGenerate`/`2signalGetTaskBase()` target, followed through the `+0x218` implementation. No retained receipt currently binds those data to `0x1402cbf30` or `0x143d766b8+0x1c`.

## Missing binding

The required edge is one of:

* a generated meta-object/slot or virtual target reached on this route that directly invokes `0x1402cbf30`, with call-time `rcx` proven to be `0x143d766b8`; or
* an alternate writer/reset reached from that route with same-object destination dataflow and ordering before a later gate check.

Current receipts establish neither. The mere coexistence of the class/method strings and a `SfTaskBase*` argument is not a QObject construction witness, and a static slot table would still need a receiver-generation/call-path link. If retained PE metadata cannot close those transitions, the smallest conclusive dynamic witness is one same-instance trace capturing constructor completion, the first DoeOutput gate, actual setter/alternate-writer caller and receiver, and a subsequent gate value.

## Scope

This is a read-only retained-evidence dependency audit. No GUI, mutable REA database/session, original runtime, source edit, or build was used. No new native behavior is claimed; the existing DBLIST guard remains the correct candidate boundary.
