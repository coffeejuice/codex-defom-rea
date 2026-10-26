# Checkpoint 60 graph-owner xref boundary

## Verified target and search

Immutable target `evidence/original-binaries/DEF_GUI_MO_64.exe` was rehashed before inspection: SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, 67,611,136 bytes.

Read-only PE-byte searches found no incoming caller reference to `c15910` (`0x140c15910`):

- Complete `.text` raw interval `[0x400, 0x1e01800)` was scanned for direct `E8 rel32 CALL` and `E9 rel32 JMP` resolving to the target; neither matched.
- The complete image was scanned for the 64-bit absolute VA pointer `0x140c15910`; none matched.
- The complete image was scanned for 32-bit RVA `0x00c15910`; the only occurrence is `0x3da9674` in the `.pdata` runtime-function record, not a caller slot.
- The same `.text` interval was scanned for common x64 RIP-relative `LEA`, `MOV`, and indirect `FF` memory forms with targets resolving exactly to `0x140c15910`; none matched.

The existing bounded disassembly `analysis/resume56-graph-manager-wrapper.asm` covers `[0x140c15910,0x140c15963)`. It checks manager+`0x18`, invokes the factory vtable's `+0x60`, and returns that result unchanged. No caller means no evidence for its first post-return RAX consumer. The generic `+0x60` call instruction found inside this wrapper is a factory dispatch and must not be mistaken for an incoming reference to `c15910`.

## Search limits and conclusion

These byte searches rule out the listed direct and common reference forms in this PE image. They do not resolve encoded or runtime-generated dispatch, function-address arithmetic, unusual prefixes/instruction forms, or external references from a separately loaded module. In the retained executable artifacts there is no bounded static path from `c15910`'s returned pointer to scene insertion, manager+`0x10` list insertion, or another owner store. Therefore no caller RAX use can be responsibly reported, and graph-scene ownership remains open.

The next smallest evidence step is a read-only xref/callgraph query in the root-owned static analysis for function `0x140c15910`, including indirect references and any importer/module references. If that analysis also returns no callers, retain the edge as unresolved and ask the specialist to classify whether this function is an unreferenced artifact or has a missed indirect dispatch; do not infer ownership or change the source model.
