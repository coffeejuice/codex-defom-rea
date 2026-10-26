# Checkpoint 56 bounded caller search for graph manager wrapper

## Target and method

Verified immutable target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, size 67,611,136 bytes.

Read-only byte search covered the complete PE `.text` raw interval `[0x400, 0x1e01800)` (RVA `[0x1000, 0x1e022a7)`) for direct `E8 rel32 CALL` and `E9 rel32 JMP` to manager wrapper `0x140c15910`. Neither had a match. A complete image search for the wrapper's absolute 64-bit VA also had no match. The 32-bit RVA `0x00c15910` occurred once, in the `.pdata` entry describing this function, not in a code/data caller slot. No candidate incoming reference was available to disassemble.

New bounded DUMPBIN receipt `analysis/resume56-graph-manager-wrapper.asm` covers `[0x140c15910,0x140c15963)`. It confirms the wrapper returns null when manager+`0x18` is null; otherwise it dispatches through that factory object's vtable slot `+0x60`, stores the resulting pointer and returns it in RAX. This is a manager-to-factory call, not a caller of the manager wrapper.

## Negative-search boundary

This static search rules out direct relative CALL/JMP references and direct absolute-VA function-pointer references in the checked image. It does **not** rule out an indirect caller using a different vtable slot, a thunk, an encoded/relocated function pointer, or computed dispatch. Generic `call [reg+0x60]` opcodes cannot be attributed to this wrapper without a reference to its address or a recovered receiver vtable. No actual caller-side RAX consumer was found in these bounded byte searches.

The exact remaining query is a read-only code/data xref query to function `0x140c15910` in the root-owned static analysis view, including indirect/table references; if it yields a caller, inspect only that caller's return-value use. Do not infer scene insertion, manager-list ownership, or scene lifetime from the factory return path.
