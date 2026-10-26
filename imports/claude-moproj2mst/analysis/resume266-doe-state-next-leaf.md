# CP266 DOE constructor-state versus lazy-read boundary

The next useful result is a more precise missing same-occurrence witness, not a new independently implementable production helper. Fresh Add Study marks the new SUB loaded before selecting it. The selection routine therefore skips the existing-project parser on that immediate call, while continuing into its model/UI effects and then the creator's initial DoeOutput request. Existing Type2 lazy-read admission, including CP215's reader-I/O guard, cannot establish the initial sample or Save effects of this path.

## Newly resolved distinction

CP240 already records the creator's `SUB+8=1`, append, activation and output-operation request. CP249 correctly keeps them coupled. This audit joins that store to the precise activation branch, rather than treating every activation as a lazy read:

1. In `0x1400A6A00`, the non-type-4 branch calls the GUI SUB constructor `0x1400B5680` at `0x1400A6E9D`. For type 2 this is the branch CP240 qualifies as sharing the main model. The selected constructor receives the first SUB pointer read earlier, not the ordinary Open project's constructor arguments. This is static call/argument evidence, not runtime constructor success.
2. The returned new SUB is stored at `[rsp+0x50]`; `0x1400A6EDA` writes byte 1 at that SUB's `+8`. `0x1400A6EEF` appends its address to the project's list. `0x1400A6F08` calls `0x1400A7B90` using the current project index. `0x1400A6F6D` subsequently calls `0x1400A63C0` with that same SUB, the retained DoeOutput class string and `r9b=1`.
3. In `0x1400A7B90`, `0x1400A7BD8` sets the current index. The exact list entry's byte `+8` is loaded at `0x1400A7BF6`, tested at `0x1400A7BFA`, and a nonzero value branches at `0x1400A7BFC` to `0x1400A7C47`. It bypasses the loading flag set at `0x1400A7C06`, parser call `0x1400A7C17`, loaded store `0x1400A7C37`, and loading flag clear `0x1400A7C43`.
4. This is a conditional static conclusion: if the same constructed/list-appended SUB and its loaded byte reach that selection call unchanged, the parser is skipped. No runtime occurrence or unchanged interval has been observed here. The routine continues after `0x1400A7C47`; skipping parsing is not skipping activation, model retargeting or subsequent signals. The retained body obtains the active model and invokes a virtual method at offset `+0x1b8` after that branch. Its effects must not be inferred from the skipped parser.

This rules out using a candidate-only `readLazyStudy` test as evidence for Add Study's initial state. It does not identify the producer of B120's 100 empty rows, establish that no later parse occurs, or resolve any Save callback. There is no demonstrated new standalone production leaf.

## Current source gap and correct prospective test owner

Current `source/apps/moproj2mst/src/model/subproject.cpp:290` builds existing records with `loaded=false`, concrete owned `flow`, and an owned Study for type 2. `activateSubProject` at line 345 has the corresponding loaded branch and then `refreshDiagramItems`; its comment explicitly excludes some native GUI/table signals from its recovered MST scope. `parseSubProject` reaches `readLazyStudy` only through an existing `SfDoeStudy` child. `src/doe/activation.cpp` reads InputTable and then fixed/current sample files; it does not implement the fresh GUI constructor/selection transition. `src/core/convert.cpp:249` still selects existing index 0. CP215 checks the actual InputTable reader identity only after this later lazy-read path is reached.

Do not implement Add Study by calling `makeSubProject` plus ordinary activation, forcing `loaded=true` on an Open record, injecting retained sample rows, or adding a selector argument to conversion solely to make B120 pass. Those choices change the starting action or elide the GUI model alias and downstream owners. CP249's absent explicit action contract and CP256's nonempty-variable/group-6 guards remain applicable.

The eventual actual-caller test must enter a source-qualified explicit Add Study owner, not a loaded-byte setter or direct `readLazyStudy` helper. Start with the no-study before project, assert one DOM/list append and the new current index, verify the GUI model/Study aliases, and observe the same new SUB's loaded state at activation. Assert parser/lazy-file-read non-entry only if that same occurrence qualifies the bypass. Then assert reached model/selection callbacks and initial DoeOutput request in their observed order, including failure/partial effects. No such public action owner exists in current converter source, so writing that caller test now would encode an invented contract. A direct model test would validate only its own supplied state.

## Smallest additional same-occurrence witness

Refine CP249's staged B120 capture with one precise join. In one fresh process/project generation, bind `0x1400A6E9D`'s returned SUB to the address appended at `0x1400A6EEF`, the list entry read at `0x1400A7BF3`, the byte read at `0x1400A7BF6`, and the actual `0x1400A7BFC` edge. Preserve the interval between constructor return and byte read so unknown writes are not assumed absent. Record the new and first SUB's model aliases, project current index and loading flag before/after. Retain parse-entry evidence if the zero edge unexpectedly occurs; preserve failed constructor/append/callback outcomes rather than imposing the expected branch.

Continue that same occurrence through the post-branch model/selection effects and `0x1400A63C0` return, recording variable/sample owner identities and rows before explicit Save. Observe subsequent `0x14002A9B0` selection and any later parser entry separately. Record ordered reached file writes and exact bytes before and during explicit Save. This distinguishes constructor/default sample state, later selection/sample production and Save effects. A final manifest or file mtime cannot make this attribution.

For B121, extend the same process/study occurrence through TLOC/op1/three-sample editing and CP256's exact extraction/status/output-pointer boundary. Separately qualify the nonempty key's ordered live weak receiver/backend/callback chain; the loaded-byte bypass does not resolve that delivery. Do not substitute CP75's retained candidate prefix owner, completed model projections or the public read/write guards for original completion.

## Historical and evidence limits

CP48/49's B120 post-state work concerns an already-existing inactive Type2 record while the converter selects Type0 index 0. Its seven unchanged Control files establish candidate-side preservation only. It does not witness fresh Add Study or fresh-original Save. The L6 DOE note attributes Add Study/pre-Save effects historically and labels B121 TLOC/op1/three samples, but its action narrative and mtimes do not bind native call/return values or writer order. CP218's historical B120/B121 differences therefore remain unresolved.

Source qualifications read: `evidence/gui_mo/s1/s1-01/spec.md` opening and section 8.2 explicitly qualify retained Ghidra analysis as static, GUI-vs-Open construction, and activation; `s1-05/spec.md` opening qualifies decompiles/annotated literals. `s1-05/sub0_s101.c` lines 263–337 and 590–635 supply the constructor/activation control flow. The fresh immutable-PE excerpts below corroborate exact instructions. These preferred image VAs are not live process addresses. No original execution, GUI session, REA target/database, source/test/shared-state mutation or build occurred. Only this report was written.

## Immutable-PE receipt

- Captured UTC: 2026-10-26T21:36:43.257953+00:00
- `evidence\original-binaries\DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
- `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe` SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`
- `evidence\gui_mo\s1\s1-05\sub0_s101.c` SHA-256 `aacc46bc344504b40d52e7661cabfb06c7086d102770770713dd21590e4e2d5f`
- `evidence\gui_mo\s1\s1-01\spec.md` SHA-256 `779c3a01f3bff4cb496199507f43abb1ea7414b2346a21c018193299034e02da`
- `analysis\wave3-boolean-timer-1400a7b90.asm` SHA-256 `5d5b02ae40ac4d8edc355f644e779548ee51a0dbd9ef2d245d8eb6d73c7be23b`

The two complete bounded stdout excerpts below are retained directly in this sole report. Neither range is a complete-function claim.

### creator loaded/append/select/output sequence

Command: `dumpbin.exe /disasm:bytes /range:0x1400a6e83,0x1400a6f83 evidence/original-binaries/DEF_GUI_MO_64.exe`

Exit 0; stdout SHA-256 `7679b592a866017b4787e9ce3bc2cef7fe6b0716af8fc62317cc5df06eaf8db1`; stderr 0 bytes.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400A6E83: 44 8B 4C 24 40     mov         r9d,dword ptr [rsp+40h]
  00000001400A6E88: 44 8B 84 24 F8 01  mov         r8d,dword ptr [rsp+00000000000001F8h]
                    00 00
  00000001400A6E90: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000001400A6E95: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400A6E9D: E8 DE E7 00 00     call        00000001400B5680
  00000001400A6EA2: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  00000001400A6EAA: EB 0C              jmp         00000001400A6EB8
  00000001400A6EAC: 48 C7 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],0
                    00 00 00 00 00 00
  00000001400A6EB8: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001400A6EC0: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  00000001400A6EC8: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001400A6ED0: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400A6ED5: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400A6EDA: C6 40 08 01        mov         byte ptr [rax+8],1
  00000001400A6EDE: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001400A6EE6: 48 83 C1 58        add         rcx,58h
  00000001400A6EEA: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000001400A6EEF: E8 EC 68 68 01     call        000000014172D7E0
  00000001400A6EF4: 4C 8B 9C 24 F0 01  mov         r11,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001400A6EFC: 41 8B 53 50        mov         edx,dword ptr [r11+50h]
  00000001400A6F00: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001400A6F08: E8 83 0C 00 00     call        00000001400A7B90
  00000001400A6F0D: 83 BC 24 F8 01 00  cmp         dword ptr [rsp+00000000000001F8h],2
                    00 02
  00000001400A6F15: 75 6C              jne         00000001400A6F83
  00000001400A6F17: 48 8D 15 A2 9D 09  lea         rdx,[0000000142140CC0h]
                    02
  00000001400A6F1E: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001400A6F23: FF 15 57 D5 D5 01  call        qword ptr [0000000141E04480h]
  00000001400A6F29: 90                 nop
  00000001400A6F2A: 48 8D 84 24 F8 00  lea         rax,[rsp+00000000000000F8h]
                    00 00
  00000001400A6F32: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  00000001400A6F3A: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000001400A6F3F: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001400A6F47: FF 15 8B EA D5 01  call        qword ptr [0000000141E059D8h]
  00000001400A6F4D: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  00000001400A6F55: 41 B1 01           mov         r9b,1
  00000001400A6F58: 4C 8B 84 24 C8 01  mov         r8,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000001400A6F60: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  00000001400A6F65: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001400A6F6D: E8 4E F4 FF FF     call        00000001400A63C0
  00000001400A6F72: 90                 nop
  00000001400A6F73: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001400A6F78: FF 15 62 EA D5 01  call        qword ptr [0000000141E059E0h]
  00000001400A6F7E: E9 E5 00 00 00     jmp         00000001400A7068
  00000001400A6F83: 83

  Summary

     1E02000 .text
```

### activation loaded conditional

Command: `dumpbin.exe /disasm:bytes /range:0x1400a7bf3,0x1400a7c54 evidence/original-binaries/DEF_GUI_MO_64.exe`

Exit 0; stdout SHA-256 `697b430519865137aa26a261c4f3cafe4a1959d1128c7615ddd74a727d89b007`; stderr 0 bytes.

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400A7BF3: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400A7BF6: 0F B6 40 08        movzx       eax,byte ptr [rax+8]
  00000001400A7BFA: 85 C0              test        eax,eax
  00000001400A7BFC: 75 49              jne         00000001400A7C47
  00000001400A7BFE: 48 8B 84 24 30 03  mov         rax,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C06: C6 40 54 01        mov         byte ptr [rax+54h],1
  00000001400A7C0A: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000001400A7C0F: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C17: E8 14 79 FE FF     call        000000014008F530
  00000001400A7C1C: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C24: 48 83 C1 58        add         rcx,58h
  00000001400A7C28: 8B 94 24 38 03 00  mov         edx,dword ptr [rsp+0000000000000338h]
                    00
  00000001400A7C2F: E8 DC 8F 0B 00     call        0000000140160C10
  00000001400A7C34: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400A7C37: C6 40 08 01        mov         byte ptr [rax+8],1
  00000001400A7C3B: 48 8B 84 24 30 03  mov         rax,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C43: C6 40 54 00        mov         byte ptr [rax+54h],0
  00000001400A7C47: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C4F: E8 8C 0D 00 00     call        00000001400A89E0
  00000001400A7C54: 48

  Summary

     1E02000 .text
```
