# CP141 Report document owner audit

## Finding

No private Report document ownership cut is source-closed. Retained evidence establishes the access/read sequence and the Save call ordering, but does not bind the same Project, active SubProject, main-window parent, and Report document instance through accepted Close. The existing `reportDsPath(Project.path)` helper closes only string derivation. Keep public Report Open and Save guarded.

## Evidence crosswalk

| Lifecycle edge | What is established | What remains unbound |
|---|---|---|
| Open owner resolution | Report dispatch resolves its current Project and active SubProject (`L7/d1.c`, `FUN_1412add20`; `L7/d2.c`, `FUN_1412b0f70`). `FUN_1412ae070` derives sibling `.DS`, gets the document through that SubProject, then calls reset `0x140644fd0`, read/path `0x140645dc0`, and traversal `0x140644440` in order. | No candidate Project/SubProject is shown to own the original native identities. Current `ReportClass::loadClassData` stops before this document access. |
| Lazy construction/adoption | `L7/d3.c`, `FUN_1400b5ad0`, reads `SP+0x30` (`0x140c1aef0` is a direct dereference per `V/v3.c`). If null, it allocates `0x70`, reads mode, fetches the global main window (`0x1402c8090(0x143d766b8)`), calls `0x140643100(raw, mainWindow, mode, mode!=2, 0)`, then stores the returned pointer through `0x140cbc920`; otherwise it reuses the slot. Constructor evidence shows QObject parent construction and seven main-window connection registrations before this slot publication. | `0x140cbc920` ownership/refcount semantics and constructor unwind behavior are unresolved. The static QObject body allows a thread-compatibility branch that may clear the proposed parent; actual parent identity, branch outcome, child-list delta, and connection effects are not observed for a Report instance. The pointer slot is not proven to be a QPointer from these excerpts alone. |
| Save document and viewer | `L7/s0.c`, `FUN_1412adf90`, calls its `vt+0x80` (`0x1412aeb30`) first, then `0x1412af0c0`. The former derives `.DS`, resolves the active SubProject document, and calls `0x140645030(document,path,1)`. | `0x1412af0c0`'s body, returned owner, iteration, and failure/partial effects are absent from retained evidence. Save cannot be reduced to the document callback or property serialization. Candidate `save_current.cpp` still refuses before these calls. |
| Accepted Close | Retained Close evidence binds the predecessor gates and manager clear call (`0x140029a76 -> 0x140c78300`). The narrow empty-manager branch clears the manager list and then resets current; the nonempty branch invokes child deleting virtuals. Project lookup/notifications follow manager clear. | No retained Close chain binds the active Project/SubProject's `SP+0x30` slot to either Project/child destruction or main-window QObject child teardown. The inspected `FUN_1400b5bb0` slot cleanup has null and nonnull branches, but no occurrence/identity ties it to accepted Close. The slot's clear/last-reference behavior and its order against QObject parent teardown are unknown. |

## Decision and smallest next evidence

There is no evidence-backed source edit that can safely create/adopt a candidate Report document or connect `reportDsPath` to Open/Save. The smallest missing static witness is a bounded trace from the native SubProject destructor and accepted Project Close path that resolves every `SP+0x30` write/clear, the concrete `0x140cbc920` release semantics, and the resulting Report deleting-destructor order relative to main-window QObject teardown. Bind that to one same Project/SubProject/document identity. If static dataflow cannot establish identity/order, an original same-instance Open→Save→accepted-Close lifecycle receipt is required.

Public Save has an independent blocker: recover the body and returned-model ownership/effects of `0x1412af0c0`, preserving its order after `0x140645030` and before Report F80/F90/FB0 properties. The path helper or ReportDocumentSlot diagnostic model cannot substitute for either owner.

**Status:** lazy accessor/call order settled; same-instance lifetime and Close release unknown; viewer callback unknown; no source-closed private ownership cut. Read-only retained-source/current-source audit only. No source edits, build/tests, GUI, original execution, or mutable REA action.
