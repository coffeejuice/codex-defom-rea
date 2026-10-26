Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712E539: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  000000006712E53E: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  000000006712E543: 48 89 7C 24 60     mov         qword ptr [rsp+60h],rdi
  000000006712E548: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006712E54D: 4C 89 7C 24 20     mov         qword ptr [rsp+20h],r15
  000000006712E552: 4C 8D 3D E7 5D 1C  lea         r15,[00000000672F4340h]
                    00
  000000006712E559: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006712E560: 45 85 ED           test        r13d,r13d
  000000006712E563: 79 06              jns         000000006712E56B
  000000006712E565: 48 8D 73 10        lea         rsi,[rbx+10h]
  000000006712E569: EB 29              jmp         000000006712E594
  000000006712E56B: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712E56E: 8B 08              mov         ecx,dword ptr [rax]
  000000006712E570: 83 F9 01           cmp         ecx,1
  000000006712E573: 74 12              je          000000006712E587
  000000006712E575: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712E578: 48 8B CB           mov         rcx,rbx
  000000006712E57B: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006712E57F: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006712E582: E8 39 C4 FF FF     call        000000006712A9C0
  000000006712E587: 49 63 F5           movsxd      rsi,r13d
  000000006712E58A: 48 FF C6           inc         rsi
  000000006712E58D: 48 C1 E6 04        shl         rsi,4
  000000006712E591: 48 03 33           add         rsi,qword ptr [rbx]
  000000006712E594: 48 8B 1E           mov         rbx,qword ptr [rsi]
  000000006712E597: 45 33 E4           xor         r12d,r12d
  000000006712E59A: 48 8B FE           mov         rdi,rsi
  000000006712E59D: 48 85 DB           test        rbx,rbx
  000000006712E5A0: 74 3C              je          000000006712E5DE
  000000006712E5A2: 48 83 7B 08 00     cmp         qword ptr [rbx+8],0
  000000006712E5A7: 74 0C              je          000000006712E5B5
  000000006712E5A9: 48 8D 7B 18        lea         rdi,[rbx+18h]
  000000006712E5AD: 4C 8B E3           mov         r12,rbx
  000000006712E5B0: 48 8B 1F           mov         rbx,qword ptr [rdi]
  000000006712E5B3: EB 24              jmp         000000006712E5D9
  000000006712E5B5: 48 8B 6B 18        mov         rbp,qword ptr [rbx+18h]
  000000006712E5B9: 48 89 2F           mov         qword ptr [rdi],rbp
  000000006712E5BC: 48 8B 43 30        mov         rax,qword ptr [rbx+30h]
  000000006712E5C0: 49 3B C7           cmp         rax,r15
  000000006712E5C3: 74 09              je          000000006712E5CE
  000000006712E5C5: 48 8B 4B 30        mov         rcx,qword ptr [rbx+30h]
  000000006712E5C9: E8 8E A3 04 00     call        000000006717895C
  000000006712E5CE: 48 8B CB           mov         rcx,rbx
  000000006712E5D1: E8 80 A3 04 00     call        0000000067178956
  000000006712E5D6: 48 8B DD           mov         rbx,rbp
  000000006712E5D9: 48 85 DB           test        rbx,rbx
  000000006712E5DC: 75 C4              jne         000000006712E5A2
  000000006712E5DE: 4C 89 66 08        mov         qword ptr [rsi+8],r12
  000000006712E5E2: 49 8B 5E 48        mov         rbx,qword ptr [r14+48h]
  000000006712E5E6: 41 FF C5           inc         r13d
  000000006712E5E9: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712E5EC: 44 3B 68 08        cmp         r13d,dword ptr [rax+8]
  000000006712E5F0: 0F 8C 6A FF FF FF  jl          000000006712E560
  000000006712E5F6: 4C 8B 7C 24 20     mov         r15,qword ptr [rsp+20h]
  000000006712E5FB: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712E600: 48 8B 7C 24 60     mov         rdi,qword ptr [rsp+60h]
  000000006712E605: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  000000006712E60A: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]

  Summary

      1A7000 .text
