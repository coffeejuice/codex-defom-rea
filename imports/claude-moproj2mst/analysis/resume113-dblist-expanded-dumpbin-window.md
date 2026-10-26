# CP113 — expanded local DUMPBIN window for DBLIST admission

## Result

The local scan added no setter caller or receiver proof. It streamed the original PE's DUMPBIN `/DISASM:NOBYTES` output and expanded the earlier same-register check from 12 to 63 subsequent disassembled instructions after each exact `lea reg,[0x143d766b8]` match. It counted 1,991 such singleton-base LEA sites; none was followed within that window by `mov dword ptr [same-register+0x1c],...`. A direct `call 0x1402cbf30` match count was also zero.

This is bounded static negative evidence only. It does not follow aliases/copies through other registers, calls that receive an object through another access path, indirect/vtable call targets, whole-object writes, or runtime-created values. It does not establish that the setter is never called or establish the field's reset lifecycle. No `rcx == 0x143d766b8` setter occurrence was recovered.

## Method and identity

* Input: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
* Disassembler: `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`, SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
* Invocation: DUMPBIN `/DISASM:NOBYTES`, streamed through a PowerShell filter; the output was not written over the PE or any retained evidence.
* Filter: match exact disassembly instruction `lea <reg>,[0000000143D766B8h]`; inspect the next at most 63 emitted disassembly lines for a DWORD store through that same register at `+0x1c`; separately count direct `call 00000001402CBF30h` instructions.
* Result: 1,991 exact singleton-base LEA sites, zero same-register `+0x1c` stores within the 63-line window, zero direct setter calls.

The setter's previously captured body is the function span `0x1402cbf30..0x1402cbf41` (exclusive end) in `analysis/resume65-dblist-global-owner.asm`, SHA-256 `5ce9990c8bc0e7cdcef3333c5a98c973b44e2ac83290d347764e39ebb455e62e`. The prior full-image direct-reference and 12-instruction scan is documented in `analysis/resume81-dblist-setter-pe.md`; this scan expands only that local same-register window. The original active gate still reads `0x143d766b8+0x1c`, but this scan does not join the setter receiver to that gate receiver.

## Exact remaining witness

The unresolved static witness is a callsite or indirect-target chain for `0x1402cbf30` whose argument flow proves `rcx` is singleton `0x143d766b8` (or an alias to that same object), plus the same-instance writer/reset chain needed to determine `+0x1c` at DoeOutput admission. A fresh-image zero-fill, the distinct `+0x0c` process-type writer, and an unrelated same-layout object's `+0x1c` zero initializer cannot supply this witness. Keep candidate DBLIST admission unavailable and the public parser refusal in place.

## Scope

The task used local read-only disassembly only. It did not open REA, modify an analysis database, execute the original app, touch GUI/project data, change source, or build. An initial exploratory filter used PowerShell's automatic `$Matches` variable as a counter and was discarded; the reported counts above come from a corrected independent streaming pass with explicit numeric counters. No disassembly transcript was persisted, so the scan is reproducible from the stated input, tool, command mode, and filter but has no separate transcript digest.
