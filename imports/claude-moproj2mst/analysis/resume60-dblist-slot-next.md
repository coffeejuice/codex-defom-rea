# Checkpoint 60: corrected DBLIST dispatch and bounded handler trace

## Correction to prior labels

The earlier CP60 note `analysis/resume60-dblist-signal-next.md` followed a mistaken assignment of the two part-state handlers. Preserve it as historical evidence, but do not rely on its claim that `FUN_140655df0` is the DBLIST target. The retained caller `FUN_140650e60` and root's correction identify `0x1424e3cb0` as `DBLIST`, `0x1424e3cb8` as `ReportList`; the DBLIST branch calls `FUN_1406554a0(param_1, node, param_3)`, while `FUN_140655df0` is the ReportList continuation. The caller's decompile is in `analysis/wave2-doe-session-valid-existing-input-decompile.json`; literal bindings are in `analysis/wave2-doe-part-state-leaves-receipt.json`. Both refer to original `DEF_GUI_MO_64.exe` (SHA-256 recorded below).

Verified original binary SHA-256 from direct local hash: `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

## Bounded `FUN_1406554a0` trace

New DUMPBIN capture: [resume60-dblist-reportlist-6554a0.asm](resume60-dblist-reportlist-6554a0.asm), range `0x1406554a0–0x140655980`; it reaches `ret` at `0x14065597e`. The file name is retained as a descriptive label from the initial inspection, but this is the DBLIST target under the corrected call assignment. Capture SHA-256: `D02BD67B3B8E1864DF5B03CB87DB6C9068418FA7C50D5456DEE35701F09F0DA1`.

The call site supplies the part-level receiver `param_1`, the DBLIST DOM node, and `param_3` (a QString copied into a local at the call site). The caller's global predicate `FUN_1402cbec0(0x143d766b8)` is the first gate before entry. Handler-local checks then branch around its main work: at `0x1406555528` a boolean result controls entry to the later path; on that path, the handler checks a local initialization/state byte (`0x1406555742`) and calls `FUN_1402cb7a0(0x143d766b8)`, bypassing processing when the result is exactly 5 (`0x1406555756–0x1406555759`). The state meaning and the first predicate's relation to this second state test remain unresolved.

Past those checks, the handler iterates a collection (`FUN_140004ae0` count and `FUN_14014b5b0` indexed lookup), constructs/compares QString values, then reaches `FUN_140004a60` with a `0x400` size argument at `0x140655582b–0x1406555838`; it passes constructed values and a zero argument into unresolved indirect call `0x141e0bf40` at `0x14065558cc`. Locals are cleaned and a byte from stack offset `+0xd8` is returned. This establishes a parsing/dispatch-style path after guards, but not the DBLIST field written, ownership target, validation semantics, or any file side effect: helper/import identities and their implementations were not resolved here. The returned byte is not proof of persistence.

## Relation to `slotGetTaskBase(QString)`

This handler trace does not resolve the main-window `slotGetTaskBase(QString)` body or return object. That is a separate signal/slot path identified in the earlier CP60 note and remains unresolved; do not infer that this DBLIST handler's `param_1` or QString is the returned `SfTaskBase*` owner. The result here is the corrected first DBLIST helper call and its ordered local guards.

## Scope and next action

Evidence is bounded static disassembly/decompilation only. No source, build, GUI, runtime, or mutable REA/database access occurred. Before changing candidate dispatch, resolve the exact guards and the indirect call's behavior (especially the buffer's meaning and destination model field) from retained static evidence, then correct the old reversed call-site prose. Do not enable public B127 handling based on this trace.


