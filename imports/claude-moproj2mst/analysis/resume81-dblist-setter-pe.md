# CP81: bounded DUMPBIN search for DBLIST admission setter and owner writes

## Identity and method

The target was the imported original
`evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The
tool was the verified DUMPBIN at
`C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`,
SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
All use was read-only and bounded to full-image disassembly output piped to
address/instruction filters; no output was saved over the binary or existing
evidence.

## Setter references and direct global writes

The setter body is confirmed at `0x1402cbf30`:

```asm
00000001402CBF30: mov qword ptr [rsp+8],rcx
00000001402CBF35: mov rax,qword ptr [rsp+8]
00000001402CBF3A: mov dword ptr [rax+1Ch],1
00000001402CBF41: ret
```

The full DUMPBIN `/DISASM:NOBYTES` stream had no direct `call
00000001402CBF30` match. It had no textual absolute operand for
`0x143d766d4` (the target global's absolute `+0x1c` address). A separate
bounded scan of `lea reg,[0x143d766b8]` sites found no direct
`mov dword ptr [same-reg+1Ch],...` within the next 12 disassembled
instructions. These negatives are limited: they do not rule out an indirect
call, a longer-lived/aliased base register, another register construction,
memcpy/whole-object writes, or runtime-created target/receiver values.

The stream contains many stores to generic `[reg+1Ch]` fields. Offset-only
matches are not owner evidence. One nearby explicit zero store illustrates
the ambiguity: `0x1402cc6b6` is `mov dword ptr [rax+1Ch],0` inside initializer
`0x1402cc690`. Its owner is traced below and is **not** the DBLIST global.

## Owner-qualified `+0x1c=0` candidate is a different global

The static callsite at `0x1401ca0a7d` sets `rcx` to `0x143d763a0`, then calls
`0x1402cc550`. That constructor installs vtable pointer `0x1422d6dd8` at
`[this]`; it forwards the same saved `this` to initializer `0x1402cc690` at
`0x1402cc5b5`. The initializer writes:

* `this+0x18 = 3` at `0x1402cc69e`;
* `this+0x24 = 0` at `0x1402cc6aa`;
* `this+0x1c = 0` at `0x1402cc6b6`;
* `this+0x20 = 3` and a byte at `this+0x28` afterward.

At this static constructor call, `this` is the distinct global base
`0x143d763a0`; the zeroed address is `0x143d763bc`. The DBLIST gate receiver
is `0x143d766b8`, whose `+0x1c` address is `0x143d766d4`—the bases differ by
`0x318`. This is a source-qualified zero initializer for another object at
the same relative offset, not evidence of a reset to DBLIST admission.
The static constructor call also does not prove runtime ordering or execution
of this constructor.

## Admission-state origin remains open

The active DoeOutput callsite `0x140651eba` passes the application-global
base `0x143d766b8` to `FUN_1402cbec0`; its return is tested before the
`FUN_1406554a0` DBLIST handler. The predicate reads `+0x1c` and admits only
zero. The `0x1402cbf30` setter's dynamic caller and actual receiver remain
unidentified. This pass found no direct call, no absolute direct store, and
no nearby direct store through a register just loaded with the exact gate
base. It does **not** establish that there is no writer, nor that the setter
is not invoked indirectly.

The minimal remaining dependency is a native cross-reference/dataflow source
with indirect-reference coverage (or a captured original runtime call to
`0x1402cbf30` recording caller and `rcx`). To close reset semantics, that
evidence must also cover other writes to the same `0x143d766b8` instance's
`+0x1c` and relevant lifecycle paths. DUMPBIN is a bounded disassembler and
does not provide a complete incoming-xref database. Do not map the unrelated
`0x143d763a0` constructor to the admission field. Keep `+0x1c` unknown in the
candidate and keep public B127 DBLIST guarded.

## Evidence limits

The prior active-gate/helper captures remain controlling for the branch and
predicate: `analysis/resume62-dblist-register-and-caller.asm` (SHA-256
`438bbe2b9ff00ab35f7ad8554e762044fdfa589182d61143157fb02f8ab39af1`) and
`analysis/resume65-dblist-global-owner.asm` (SHA-256
`5ce9990c8bc0e7cdcef3333c5a98c973b44e2ac83290d347764e39ebb455e62e`). This
checkpoint adds bounded DUMPBIN observations only. It does not establish
runtime invocation, complete indirect xrefs, all stores via aliases, or
startup/Open/Close ordering. No GUI, source, build, test, or mutable REA
database was used or changed.
