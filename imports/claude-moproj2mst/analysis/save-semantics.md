# Original Save failure behavior: initial static findings

Purpose: resolve whether faithful reconstruction should implement an all-or-nothing Save. The user explicitly withdrew that proposal and requires original behavior. The observed paths below do not implement a project-wide transaction.

## Target and evidence qualification

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, image base `0x140000000`. This imported file previously matched the installed original by SHA-256.

Evidence combines imported Ghidra decompilation with fresh static disassembly of the copied original using installed Microsoft DUMPBIN. No original process was run, no fault was injected and no analysis database was opened or modified.

`save-entry-disassembly.txt` was obtained with `/DISASM:BYTES /RANGE:0x1400980d0,0x1400982df`; the Save function itself returns at `0x1400982ab`. The tail of that requested range contains a subsequent function and is not part of the Save claim.

All 205 address/byte records in the imported `evidence/gui_mo/s1/mdt/17e0.asm` were independently matched against the copied executable's PE section bytes, with zero mismatches. The original string at virtual address `0x142674bfc` is `wt\0`.

## Save sequence and continuation after failure

`0x1400980d0` performs sub-project checks before the writes. A check result other than `0x10000` returns early. After the checks, it calls:

1. `0x140052ec0`: save the active operation if one is found.
2. `0x1400983c0`: write the `.moproj`.
3. `0x1400982e0`: invoke operation property-save methods.
4. `0x1400a20d0`: write MST and relevant subsequent study output.
5. The remaining local-remesh handling.

Fresh disassembly confirms the `.moproj` call at `0x1400981f0`, the property-save call at `0x140098201` and the MST call at `0x14009820e`. There is no conditional branch between these calls on the `.moproj` result. Only after the MST call does it combine the two Boolean results.

The shown top-level function returns the earlier check/status local (`[rsp+24h]`) at `0x1400982a0`, rather than that combined Boolean. Do not map this directly to a new DLL status without tracing the external caller and associated messages.

The imported decompile `evidence/gui_mo/s1/mdt/save_d0.c` corroborates this sequence. Its `0x1400982e0` loop calls properties vtable slot `+0xb8` and ignores individual return values.

## Direct writes and a silent failure

- `.moproj`: `0x1400983c0` constructs QFile on the destination and opens it with mode value 2. It returns false on open failure and checks QFile error after writing. Its caller nevertheless proceeds to the next stages. Source: `evidence/gui_mo/s1/mdt/L3/save0.c`, lines 197-232 and 310-356.
- MST: `0x1401313c0` constructs QFile on the MST destination, opens it with mode value 2 and writes the stream directly. It returns false on open failure; the shown successful-open path returns true without an explicit QTextStream-status check. Source: `evidence/gui_mo/s1/s1-02/a1s.c`, lines 94-120. Exact Qt write/flush error behavior still needs its own trace and runtime confirmation.
- Cogging.MDT: `0x1407617e0` opens the destination using the original CRT fopen with mode `wt`. The null-file branch at `0x1407618bf` jumps to `0x140761c4b`, which sets the return value to 1. Thus failure to open skips the write body but still returns 1. Source: `evidence/gui_mo/s1/mdt/w_0.c`, lines 31-63, and byte-verified `17e0.asm`.

These paths provide direct evidence against imposing a project-wide rollback guarantee. Earlier writes can remain when a later operation fails. This is not a claim that every possible error reaches every later stage: checks, exceptions, crashes and class-specific branches require individual tracing.

## Difference in Claude's implementation

`source/apps/moproj2mst/src/core/convert.cpp:62` implements `writeAtomically`: write a temporary file, replace the target with MoveFileExW, delete the temporary on error and throw `M2M_E_WRITE`. `applyEffects` uses this helper for output files. This differs from the inspected original direct-write paths and their handling of failures, even when successful output bytes match.

This is a fidelity finding for the research backlog. No converter implementation was changed during this investigation. An evidence-led correction must recover exact order, return handling and file effects per writer, rather than replace one blanket policy with another.

## API qualification

The copied executable's PE export table has five named entries: DllCanUnloadNow, DllGetClassObject, DllMain, DllRegisterServer and DllUnregisterServer. There are no `m2m_*` named exports. This inspection does not classify or recover those exported implementations; they are not automatically in converter scope.

Claude's `m2m_convert_w` / `m2m_convert_utf8` interfaces are reconstruction adapters. Original internal method interfaces, state and return contracts must be evaluated separately. The user confirmed preserving the existing adapter ABI as a thin compatibility layer without silently changing recovered original behavior.

## Remaining validation

Trace the Save action's caller and messages, every relevant writer's error/flush handling, any backup or rename branches, and open-time effects before generalizing. Then compare original and reconstructed behavior on disposable projects for selected failed-open and failed-write conditions, preserving before/after file manifests and process/session state. The completed faithful baseline must reproduce original outcomes rather than improve them silently.
