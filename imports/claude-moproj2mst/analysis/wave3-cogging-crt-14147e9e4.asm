Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014147E9E4: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000014147E9E9: 48 89 7C 24 10     mov         qword ptr [rsp+10h],rdi
  000000014147E9EE: 41 54              push        r12
  000000014147E9F0: 48 83 EC 20        sub         rsp,20h
  000000014147E9F4: 48 8B D9           mov         rbx,rcx
  000000014147E9F7: E8 4C B0 FF FF     call        0000000141479A48
  000000014147E9FC: 8B C8              mov         ecx,eax
  000000014147E9FE: E8 ED 17 01 00     call        00000001414901F0
  000000014147EA03: 85 C0              test        eax,eax
  000000014147EA05: 0F 84 97 00 00 00  je          000000014147EAA2
  000000014147EA0B: E8 74 F1 FE FF     call        000000014146DB84
  000000014147EA10: 48 83 C0 30        add         rax,30h
  000000014147EA14: 48 3B D8           cmp         rbx,rax
  000000014147EA17: 75 04              jne         000000014147EA1D
  000000014147EA19: 33 C0              xor         eax,eax
  000000014147EA1B: EB 13              jmp         000000014147EA30
  000000014147EA1D: E8 62 F1 FE FF     call        000000014146DB84
  000000014147EA22: 48 83 C0 60        add         rax,60h
  000000014147EA26: 48 3B D8           cmp         rbx,rax
  000000014147EA29: 75 77              jne         000000014147EAA2
  000000014147EA2B: B8 01 00 00 00     mov         eax,1
  000000014147EA30: FF 05 8A 10 28 03  inc         dword ptr [00000001446FFAC0h]
  000000014147EA36: F7 43 18 0C 01 00  test        dword ptr [rbx+18h],10Ch
                    00
  000000014147EA3D: 75 63              jne         000000014147EAA2
  000000014147EA3F: 4C 8D 25 C2 14 28  lea         r12,[00000001446FFF08h]
                    03
  000000014147EA46: 48 63 F8           movsxd      rdi,eax
  000000014147EA49: 49 83 3C FC 00     cmp         qword ptr [r12+rdi*8],0
  000000014147EA4E: 75 2B              jne         000000014147EA7B
  000000014147EA50: B9 00 10 00 00     mov         ecx,1000h
  000000014147EA55: E8 7A F9 FF FF     call        000000014147E3D4
  000000014147EA5A: 49 89 04 FC        mov         qword ptr [r12+rdi*8],rax
  000000014147EA5E: 48 85 C0           test        rax,rax
  000000014147EA61: 75 18              jne         000000014147EA7B
  000000014147EA63: 48 8D 43 20        lea         rax,[rbx+20h]
  000000014147EA67: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  000000014147EA6B: 48 89 03           mov         qword ptr [rbx],rax
  000000014147EA6E: B8 02 00 00 00     mov         eax,2
  000000014147EA73: 89 43 24           mov         dword ptr [rbx+24h],eax
  000000014147EA76: 89 43 08           mov         dword ptr [rbx+8],eax
  000000014147EA79: EB 19              jmp         000000014147EA94
  000000014147EA7B: 49 8B 0C FC        mov         rcx,qword ptr [r12+rdi*8]
  000000014147EA7F: C7 43 24 00 10 00  mov         dword ptr [rbx+24h],1000h
                    00
  000000014147EA86: C7 43 08 00 10 00  mov         dword ptr [rbx+8],1000h
                    00
  000000014147EA8D: 48 89 4B 10        mov         qword ptr [rbx+10h],rcx
  000000014147EA91: 48 89 0B           mov         qword ptr [rbx],rcx
  000000014147EA94: 81 4B 18 02 11 00  or          dword ptr [rbx+18h],1102h
                    00
  000000014147EA9B: B8 01 00 00 00     mov         eax,1
  000000014147EAA0: EB 02              jmp         000000014147EAA4
  000000014147EAA2: 33 C0              xor         eax,eax
  000000014147EAA4: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000014147EAA9: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000014147EAAE: 48 83 C4 20        add         rsp,20h
  000000014147EAB2: 41 5C              pop         r12
  000000014147EAB4: C3                 ret

  Summary

     1E02000 .text
