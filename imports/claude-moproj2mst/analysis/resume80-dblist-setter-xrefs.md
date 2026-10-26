# CP80: setter cross-reference availability and residual owner gap

## Result

No new native cross-reference was obtained. The shared REA slot was released,
but its `binary_session` reports no native PE provider (only artifact graph
and .NET static analysis). I did not attempt to open a native target. An
offline tool check found no `dumpbin.exe`, `llvm-objdump.exe`, or `objdump.exe`
on `PATH`; the available Python environment has none of `capstone`, `pefile`,
`iced_x86`, `lief`, or `distorm3` installed. I did not install or download a
disassembler. Thus this checkpoint makes no new claim about native xrefs.

## Retained native evidence

The original PE is
`evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Existing bounded captures establish:

* `FUN_1402cbf30` stores constant 1 to the `+0x1c` field of its receiver;
  `FUN_1402cbec0` admits only when that receiver field equals zero.
* On the recovered Type2 DoeOutput route, caller `0x140651eba` passes
  `0x143d766b8` to `FUN_1402cbec0` after its per-call argument byte is zero;
  a true return is required before DBLIST handler `FUN_1406554a0`.
* The gate precedes DBLIST parsing and its file/path/ModelManager effects.
* The address lies in `.data` virtual zero-fill tail, establishing zero only
  at initial image load. The retained Open/Close trace does not show a `+0x1c`
  reset.
* CP66's recorded broad raw `.text` scan found no direct `E8` call or `E9`
  jump to `FUN_1402cbf30`, nor an exact raw code-pointer literal. This is a
  bounded negative search, not a proof against indirect or constructed calls.

The direct helper body does not identify its caller or establish that the
setter's runtime receiver is the gate's global receiver. Static compatibility
of the receiver-relative offset is not owner proof. No retained evidence
enumerates every store to `0x143d766b8+0x1c`, identifies a clearing writer, or
orders the first runtime write relative to initialization/Open/Close.

## Smallest missing dependency

The next static step requires a supported offline native PE disassembler/xref
capability against the already hashed original, with coverage sufficient to
resolve direct and data references and inspect indirect-call setup. It must
identify the concrete receiver passed to `FUN_1402cbf30` and enumerate writes
to the same instance's `+0x1c`. If this still leaves a dynamically constructed
caller or receiver ambiguous, the remaining dependency is an original
application-scope runtime trace that records the setter call stack, receiver,
and field value across startup initialization, Open, DBLIST dispatch, Close,
and later Open. Runtime invocation must not be inferred from a static xref
alone.

Until one of those dependencies closes the owner and transitions, keep the
candidate admission observation unavailable and the public B127 DBLIST guard
in place. Do not substitute the `+0x0c` process-type refresh or fresh Session
construction for the missing producer.

## Receipts and scope

* `analysis/resume65-dblist-global-owner.asm` — helper bodies, SHA-256
  `5ce9990c8bc0e7cdcef3333c5a98c973b44e2ac83290d347764e39ebb455e62e`.
* `analysis/resume62-dblist-register-and-caller.asm` — active gate callsite,
  SHA-256 `438bbe2b9ff00ab35f7ad8554e762044fdfa589182d61143157fb02f8ab39af1`.
* `analysis/resume66-dblist-global-lifecycle.md` — prior scan coverage and
  lifecycle boundary, SHA-256
  `e57b855a498e2cdc8d760663a829e7fbe849f73b925a2c4e64440d122bbbf90e`.
* `analysis/resume79-dblist-admission-writer.md` — prior gate/writer audit,
  SHA-256 `d53d4bc87812e242d669e2ad24f6f444bd646e04a19d20d638e60b7c065bd07b`.

Read-only retained-evidence and local-tool availability check only. No GUI,
mutable REA database, source, build, or tests were used or changed.
