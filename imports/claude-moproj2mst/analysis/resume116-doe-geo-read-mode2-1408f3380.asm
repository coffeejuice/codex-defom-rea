Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408F3380: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001408F3385: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408F338A: 48 83 EC 38        sub         rsp,38h
  00000001408F338E: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  00000001408F3396: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408F339B: E8 E0 D7 B7 00     call        0000000141470B80
  00000001408F33A0: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  00000001408F33A4: 83 7C 24 24 04     cmp         dword ptr [rsp+24h],4
  00000001408F33A9: 7C 23              jl          00000001408F33CE
  00000001408F33AB: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F33AF: 83 E8 03           sub         eax,3
  00000001408F33B2: 48 63 C8           movsxd      rcx,eax
  00000001408F33B5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408F33BA: 0F BE 04 08        movsx       eax,byte ptr [rax+rcx]
  00000001408F33BE: 83 F8 2E           cmp         eax,2Eh
  00000001408F33C1: 75 0B              jne         00000001408F33CE
  00000001408F33C3: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F33C7: 83 E8 02           sub         eax,2
  00000001408F33CA: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001408F33CE: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  00000001408F33D3: 7D 2A              jge         00000001408F33FF
  00000001408F33D5: 83 7C 24 24 05     cmp         dword ptr [rsp+24h],5
  00000001408F33DA: 7C 23              jl          00000001408F33FF
  00000001408F33DC: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F33E0: 83 E8 04           sub         eax,4
  00000001408F33E3: 48 63 C8           movsxd      rcx,eax
  00000001408F33E6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408F33EB: 0F BE 04 08        movsx       eax,byte ptr [rax+rcx]
  00000001408F33EF: 83 F8 2E           cmp         eax,2Eh
  00000001408F33F2: 75 0B              jne         00000001408F33FF
  00000001408F33F4: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F33F8: 83 E8 03           sub         eax,3
  00000001408F33FB: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001408F33FF: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  00000001408F3404: 7D 2A              jge         00000001408F3430
  00000001408F3406: 83 7C 24 24 06     cmp         dword ptr [rsp+24h],6
  00000001408F340B: 7C 23              jl          00000001408F3430
  00000001408F340D: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F3411: 83 E8 05           sub         eax,5
  00000001408F3414: 48 63 C8           movsxd      rcx,eax
  00000001408F3417: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408F341C: 0F BE 04 08        movsx       eax,byte ptr [rax+rcx]
  00000001408F3420: 83 F8 2E           cmp         eax,2Eh
  00000001408F3423: 75 0B              jne         00000001408F3430
  00000001408F3425: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F3429: 83 E8 04           sub         eax,4
  00000001408F342C: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001408F3430: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  00000001408F3435: 7D 2A              jge         00000001408F3461
  00000001408F3437: 83 7C 24 24 07     cmp         dword ptr [rsp+24h],7
  00000001408F343C: 7C 23              jl          00000001408F3461
  00000001408F343E: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F3442: 83 E8 06           sub         eax,6
  00000001408F3445: 48 63 C8           movsxd      rcx,eax
  00000001408F3448: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408F344D: 0F BE 04 08        movsx       eax,byte ptr [rax+rcx]
  00000001408F3451: 83 F8 2E           cmp         eax,2Eh
  00000001408F3454: 75 0B              jne         00000001408F3461
  00000001408F3456: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F345A: 83 E8 05           sub         eax,5
  00000001408F345D: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001408F3461: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  00000001408F3466: 7D 2A              jge         00000001408F3492
  00000001408F3468: 83 7C 24 24 08     cmp         dword ptr [rsp+24h],8
  00000001408F346D: 7C 23              jl          00000001408F3492
  00000001408F346F: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F3473: 83 E8 07           sub         eax,7
  00000001408F3476: 48 63 C8           movsxd      rcx,eax
  00000001408F3479: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408F347E: 0F BE 04 08        movsx       eax,byte ptr [rax+rcx]
  00000001408F3482: 83 F8 2E           cmp         eax,2Eh
  00000001408F3485: 75 0B              jne         00000001408F3492
  00000001408F3487: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F348B: 83 E8 06           sub         eax,6
  00000001408F348E: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001408F3492: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  00000001408F3497: 7D 04              jge         00000001408F349D
  00000001408F3499: 32 C0              xor         al,al
  00000001408F349B: EB 5D              jmp         00000001408F34FA
  00000001408F349D: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  00000001408F34A1: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001408F34A5: EB 0B              jmp         00000001408F34B2
  00000001408F34A7: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001408F34AB: 83 C0 01           add         eax,1
  00000001408F34AE: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001408F34B2: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001408F34B6: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  00000001408F34BA: 7D 26              jge         00000001408F34E2
  00000001408F34BC: 4C 63 44 24 20     movsxd      r8,dword ptr [rsp+20h]
  00000001408F34C1: 8B 4C 24 28        mov         ecx,dword ptr [rsp+28h]
  00000001408F34C5: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001408F34C9: 2B C1              sub         eax,ecx
  00000001408F34CB: 48 63 D0           movsxd      rdx,eax
  00000001408F34CE: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001408F34D3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408F34D8: 42 0F B6 04 00     movzx       eax,byte ptr [rax+r8]
  00000001408F34DD: 88 04 11           mov         byte ptr [rcx+rdx],al
  00000001408F34E0: EB C5              jmp         00000001408F34A7
  00000001408F34E2: 8B 4C 24 28        mov         ecx,dword ptr [rsp+28h]
  00000001408F34E6: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001408F34EA: 2B C1              sub         eax,ecx
  00000001408F34EC: 48 63 C8           movsxd      rcx,eax
  00000001408F34EF: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001408F34F4: C6 04 08 00        mov         byte ptr [rax+rcx],0
  00000001408F34F8: B0 01              mov         al,1
  00000001408F34FA: 48 83 C4 38        add         rsp,38h
  00000001408F34FE: C3                 ret

  Summary

     1E02000 .text
