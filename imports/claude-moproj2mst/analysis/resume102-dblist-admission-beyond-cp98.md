# CP102 — DBLIST admission state after CP98

## Result

No new retained evidence closes the `0x143d766b8+0x1c` writer occurrence, caller receiver, or reset lifecycle. The correct public integration remains blocked: the DoeOutput parser keeps its DBLIST/ReportList refusal, and `ApplicationStateProjection` leaves DBLIST admission unavailable.

The only closed independent seams are downstream/value-only contracts: the mode-0 process-type projection for the separate `+0x0c` warning state and `makeDbListWarningReceipt` for a caller-supplied warning state, missing-name list, and optional parent token. Neither reads nor derives the `+0x1c` admission value. They are not public DBLIST integrations and cannot be used to bypass the gate.

## Retained-PE reconciliation

- Active Type2 DoeOutput path: `FUN_140650e60` supplies literal-zero argument 6, then passes application-global `0x143d766b8` to `FUN_1402cbec0`; the DBLIST handler is reached only when the receiver's DWORD `+0x1c` is zero.
- Writer: `FUN_1402cbf30` stores constant `1` to its argument receiver `+0x1c`. Its body gives no semantic owner or event.
- Initialization: PE `.data` virtual zero-fill makes the field zero on a newly loaded image only. It does not bind a new candidate Session to that value after startup/Open/Close.
- Lifecycle: successful Open and normal Close evidence recalculates the separate `+0x0c` process-type field. It does not establish `+0x1c=0` or `+0x1c=1`.
- Search limit: retained DUMPBIN found no direct E8/E9 call to `0x1402cbf30`, no exact code-pointer literal, and no nearby store through a register freshly loaded with the gate base. Those negatives do not cover indirect/virtual dispatch, receiver aliases, broader dataflow, or whole-object writes. The direct writer body alone cannot be promoted to the first runtime write.

The evidence does not yet tell us whether the `+0x1c=1` setter receives the active gate object or another same-layout object. No reset-to-zero path for that same runtime receiver is qualified. Consequently, defaulting admission to either state on candidate Open would invent a process-history rule.

## Candidate binding check

`ApplicationStateProjection` is a detached object with an API to observe only the native mode-0 process-type refresh. `dbListAdmission()` returns its default `Unavailable` observation; there is no setter/clear API. Search of current production source finds no call to either `dbListAdmission()` or `observeMode0ProcessTypeRefresh`; those names occur only in the isolated projection and its tests. `makeDbListWarningReceipt` likewise has only its declaration/implementation and focused tests as callers; it receives facts and does not consult the application projection. The Session record parser still stops at direct DBLIST/ReportList children.

The warning helper is independently valid as a supplied-facts contract: process type 5 suppresses only the warning, while manager effects/result remain separate. It is not enough to resume the active parser, because native gate admission occurs earlier and uses the different `+0x1c` field.

## Exact next executable action

After the separate REA owner releases its analysis database, perform one bounded read-only original-PE reference/dataflow pass for `0x1402cbf30` and global base `0x143d766b8`. It must enumerate code and data xrefs, including vtable/function-pointer slots and indirect callsites, and trace the receiver value into `rcx` at each candidate setter call. In parallel within that same bounded pass, enumerate writes to `0x143d766b8+0x1c` through both direct addresses and aliases, then bind each write to its enclosing lifecycle. The pass succeeds only if it finds the setter occurrence with receiver identity and a complete enough write/reset chain to determine the value at a later public DoeOutput import. A no-xref result must remain qualified as tool/scope-limited and leave the guard in place.

If static xrefs/dataflow cannot recover the occurrence, the missing witness is an original application-scope trace recording caller, `rcx`, and `+0x1c` before/after startup, successful Open, DBLIST dispatch, Close, and a later Open. No original execution or GUI/REA action was performed in this checkpoint; it is deferred because the REA database is owned by another worker.

## Receipts

Controlling original PE: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

| Receipt | SHA-256 |
|---|---|
| `analysis/resume65-dblist-global-owner.asm` | `5ce9990c8bc0e7cdcef3333c5a98c973b44e2ac83290d347764e39ebb455e62e` |
| `analysis/resume62-dblist-register-and-caller.asm` | `438bbe2b9ff00ab35f7ad8554e762044fdfa589182d61143157fb02f8ab39af1` |
| `analysis/resume66-dblist-global-lifecycle.md` | `e57b855a498e2cdc8d760663a829e7fbe849f73b925a2c4e64440d122bbbf90e` |
| `analysis/resume81-dblist-setter-pe.md` | `b7df69ac307e07edace57eb11240557076d146ae2383a831981d151a9a7267d5` |
| `analysis/resume75-doe-application-state-projection.md` | `80b46a39c465a4c946271ddbe296a309844cbd0f61bf4665488aaeb344ff0908` |
| `analysis/resume63-dblist-warning-implementation.md` | `df98294db16148820a63e39e44db8350fa00896fd6ea6dee55300d7857da3b19` |

Current-source hashes: `application_state_projection.hpp` `25d236d51f278ab7e57e3773d8f6e12df68c57900879d150cf3deba7d9cc597c`; `.cpp` `ba861fa708ff766c3ffaf5f8ccaabf5ee328ca5150e5e4fe6ae29289530f8803`; `session_records.cpp` `8e8410f7fb4596bc62e752b02969208e51c9cfcfab402ba72727885bb3f50794`.

This was a retained-evidence and source-symbol audit only. No REA database, GUI, original application, source, build, or test was touched.
