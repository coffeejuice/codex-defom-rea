# CP120 — DBLIST setter receiver and later-value slice

## Result

No setter callsite or later write was bound to singleton `0x143d766b8`. The new narrow correlation uses the original PE's exact x64 `.pdata` function ranges: across the full DUMPBIN instruction stream, 1,299 functions contained an exact `lea reg,[0x143d766b8]`, and 39 functions contained a non-stack immediate DWORD store of zero or one to `[reg+0x1c]`. **No function range contained both.** No instruction operand referenced setter address `0x1402cbf30` (excluding the instruction address column itself).

This correlation avoids the earlier fixed instruction-distance scan. It rules out a direct owner-bearing store in a single `.pdata` function that explicitly LEAs the singleton, but not a receiver passed through another function, loaded from a separately initialized pointer, assembled indirectly, or updated through non-immediate/whole-object stores. It does not prove the setter is never called.

## Constructor value remains positively established

CP116's static chain remains valid and is the one qualified later-value fact at this checkpoint:

`CRT initializer 0x141ca0bb0` passes exact singleton `0x143d766b8` → `0x1402c7ee0` forwards unchanged `this` → `0x1402c7f80` stores DWORD zero at `this+0x1c` at `0x1402c8030`.

This establishes `+0x1c == 0` when pre-main AppState construction completes. It does not determine the value at a later DoeOutput import because no post-construction receiver-bound setter or other-writer chain is known. The `+0x0c` process-type refresh is a separate field and cannot supply the missing transition.

## Bounded correlation method

The immutable original `evidence/original-binaries/DEF_GUI_MO_64.exe` (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) was streamed through the verified DUMPBIN `/DISASM:NOBYTES` tool (SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`). A local PE-header parser read the `.pdata` exception directory and assigned each decoded instruction address to its containing half-open `RUNTIME_FUNCTION` range. Within those exact ranges it correlated:

* exact `lea <reg>,[0000000143D766B8h]` instructions;
* immediate stores matching `mov dword ptr [<non-stack-register>+1Ch], 0|1` (indexed stores and register-valued stores are outside this predicate);
* explicit setter-address operands containing `0x1402CBF30`.

The DUMPBIN stream contained 7,218,152 decoded instructions and `.pdata` contained 189,695 valid ranges. The analysis recorded aggregate counts and printed only functions with both predicates; there were none. The stream was not persisted, so these counts have no separate transcript digest; they are reproducible against the stated immutable input, verified tool, and explicit predicates.

## Exact remaining edge

The missing native evidence remains a setter/alternate-writer occurrence with the effective receiver proven to be `0x143d766b8`, and the write/reset ordering from constructor completion to a later DoeOutput gate read. The current data excludes the simple same-function direct pattern but not interprocedural receiver transfer or indirect dispatch. A next static attempt should follow a concrete setter target/dispatch table or a concrete receiver-producing call chain, not repeat global-offset scans. If no such static chain is recoverable, the needed witness is a same-instance original trace of startup through DoeOutput import with setter call stack, `rcx`, and `+0x1c` before/after.

Keep candidate admission known only at the explicit post-constructor point; do not default each conversion to zero and do not enable public DBLIST dispatch from this static slice.

## Scope

Read-only local PE parsing and DUMPBIN disassembly. No REA session/database, original runtime, GUI, source, build, or project data was used or changed.
