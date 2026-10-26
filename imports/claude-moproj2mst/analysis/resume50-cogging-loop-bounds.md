# Resume 50: Cogging REHEAT loop bounds

## Immutable artifact and retained static records

The original target is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, as bound in `analysis/wave3-cogging-native-io-contract.json`. The address-level disassembly is retained in `evidence/gui_mo/s1/mdt/a8c0.asm` (SHA-256 `a5531be2b3b51389546c2d7243b1c940d334a6fc535e66c65c2e0aa4d9574550`); the corresponding decompilation is in `evidence/gui_mo/s1/mdt/annot_all.c` (SHA-256 `3d94a2e95465308b87bd3c1e9dc4906de9e618e11c1960076db45e604ec3428f`). This is static evidence from the immutable target, not runtime observation.

## Operand and allocation bounds

The first loop at `0x14076ab77..0x14076abd7` uses signed 32-bit `i` and the signed 32-bit `CogData.nReheats` at `[cog + 0xcc]`. It sign-extends `i` to 64 bits (`MOVSXD` at `0x14076ab8e`), multiplies by `0x190` (400) as a 64-bit value (`IMUL` at `0x14076ab96`), and adds that to the vector base loaded from `[cog + 0xd0]` (`0x14076aba5`). For positive `n`, its addresses are element indexes 0 through n-1. The call at `0x14076abd2` passes each in-bounds record pointer to `0x14076c1c0` for SIMLST enumeration.

After the `NoReheats` output, the second loop initializes a signed 32-bit index at `0x14076ac03`, compares it against the same signed 32-bit count at `0x14076ac29..0x14076ac36`, sign-extends the index (`MOVSXD` at `0x14076ac38`), and multiplies by 64-bit stride `0x190` (`IMUL` at `0x14076ac40`). Crucially, it adds that offset to the previously stored pointer at `[rsp+0x30]` (`ADD` at `0x14076ac4c`), then stores the result back (`0x14076ac4f`). The pointer is accumulated rather than recomputed from vector base. On loop index i it therefore addresses index `n-1 + i(i+1)/2` after the first pass's final SIMLST pointer. `0x14076ac6d` calls `0x14076cb50` with that pointer and one-based display index `i+1`.

The decompilation of `0x14076cb50` in `annot_all.c` establishes the first record access order: it first emits the `Reheat %d` banner, then formats `NoPass` from `*param_1` (a 32-bit `int` read at record offset 0), then reads/writes three heat records from `param_1+4`, `param_1+0x24`, and `param_1+0x44`, then uses the pass count and vector descriptor. The record stride `0x190` is also the allocation element size in the parser/decomp contract (`source/docs/re/moproj2mst/mdt/spec.md`, section 3; native read path at `0x14076c100`). For a two-element positive vector, valid bytes occupy `[base, base+0x320)`. At second-loop index 0 the pointer is `base+0x190` (element 1, valid) and the function prints `Reheat 1` while serializing element 1. At index 1, the accumulated pointer is `base+0x320`, exactly one-past-end. The callee prints `Reheat 2` before its first model-data read of a four-byte `NoPass` at `base+0x320`.

This establishes a reproducible invalid-read contract for `n=2`: the loop passes a one-past pointer to the record writer, and that callee attempts a four-byte read at that address after the banner. It does **not** establish an access violation, process exit status, value read, later accesses, or final file bytes: adjacent memory may be mapped/readable, and its contents are unspecified. Do not describe this as a guaranteed crash or predict output values.

## Safe count boundary

For `n=0`, both loops are skipped. The pointer local from the first loop is not initialized by an iteration, but the second loop also does not read it; therefore this loop itself does not dereference an absent REHEAT element. For `n=1`, the SIMLST loop leaves the pointer at element 0; the second loop has only index 0, adds a zero stride, and the callee reads the sole in-bounds record. These are the accepted count branches at this loop. The standard one-reheat writer path is already implemented. The n=0 path is deterministic at this loop and needs no change. Other writers/state before or after this loop remain outside this narrow bound claim.

For `n>=2`, no full serialization branch is safe to implement: at least the second iteration is out of bounds, and the callee's subsequent behavior depends on unspecified adjacent storage. It remains appropriate for the port to guard rather than synthesize an expected file. Keep the guard classified as a conservative unsupported policy, not an original native error result.

No source, build, GUI, or mutable REA state was changed for this static bounds audit.
