# CP117 — bounded static search for DBLIST setter receiver

## Result

No caller that binds `FUN_1402cbf30`'s `rcx` receiver to AppState singleton `0x143d766b8` was recovered in this slice. The input is the unchanged original PE. This result adds a narrow non-code-section target-pointer search and a constructor call-chain check; it does not repeat the prior 1,991-reference scan.

CP116 remains the controlling positive lifecycle fact: the pre-main AppState constructor call chain reaches `0x1402c7f80` on `0x143d766b8`, and that constructor explicitly writes DWORD zero to `this+0x1c` at `0x1402c8030`. This proves startup admission-field zero after that constructor completes. It does not establish the field's later value.

## Slice checked

* The exact AppState constructor span `0x1402c7f80..0x1402c8056` contains the explicit `+0x1c=0` write and its source-ordered calls/stores; it contains no direct call to setter `0x1402cbf30`. Its caller `0x1402c7ee0` forwards its saved `this` to that constructor. Global initializer `0x141ca0bb0` passes exact global `0x143d766b8` to the caller. Raw-range hashes for these three spans are in `analysis/resume116-dblist-setter-slice.md`.
* I parsed the immutable PE section table and searched every file-backed **non-executable** section (`.rdata`, `.data`, `.pdata`, `.rodata`, `data`, `.debug_l`, `.rsrc`, `.reloc`) for both the absolute 64-bit setter VA `0x1402cbf30` and the 32-bit image RVA `0x2cbf30`. There were zero hits for either form. This is a narrow check for statically materialized absolute/RVA function-pointer entries; it does not exclude encoded relative pointers, runtime construction, or non-data dispatch.
* The retained full-image DUMPBIN scan in `analysis/resume81-dblist-setter-pe.md` found no direct `E8` call to the setter and no exact raw absolute code-pointer literal. CP113 extended the singleton-base local store window to 63 following instructions at each of 1,991 `lea reg,[0x143d766b8]` references and found no same-register `+0x1c` store. These retained results are bounded and are not a complete call graph.

The PE section parser identified `.rdata` VA/RVA `0x1e03000`, raw size `0x1748800`; `.data` `0x354c000`, raw size `0x82a400`; `.pdata` `0x5056000`, raw size `0x22bc00`; `.rodata` `0x5282000`, raw size `0xa400`; `data` `0x528f000`, raw size `0x4000`; `.debug_l` `0x5293000`, raw size `0x3e00`; `.rsrc` `0x5297000`, raw size `0x400`; and `.reloc` `0x5298000`, raw size `0xc6a00`. None contained either target encoding in its raw bytes.

## Meaning for admission and next edge

The evidence now supports this ordered statement: fresh native image load zero-fills the field; CRT runs the AppState constructor before `main`; the constructor writes the singleton's `+0x1c` to zero. The DBLIST predicate therefore sees a known zero if reached after constructor completion and before any later write. The only identified `+0x1c=1` body still stores through its caller-supplied receiver, and no native caller/receiver occurrence is known. Successful Open's separate `+0x0c` process-type refresh does not close this edge.

The exact missing edge is a call/dispatch occurrence targeting `0x1402cbf30` (or a proven equivalent `+0x1c=1` writer) with the receiver dataflow resolved to `0x143d766b8`, together with later same-instance reset/write ordering before DoeOutput dispatch. A static setter call alone would not prove runtime invocation; a table candidate alone would not bind `rcx`. If these cannot be established with a source-qualified static reference chain, the remaining witness is a same-instance native startup-to-DoeOutput trace recording the field and setter receiver.

Keep the candidate value known only for the explicit post-constructor startup point, and keep public DBLIST dispatch guarded until the import-time state is determined. Do not use the startup zero as a per-ConversionSession default.

## Identity and limits

Original `DEF_GUI_MO_64.exe` SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The PE is unchanged. This was a local read-only static scan only; no REA session/database, original runtime, GUI, source, build, or project data was used or changed.
