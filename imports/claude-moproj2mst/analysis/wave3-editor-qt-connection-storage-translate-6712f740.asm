Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712F740: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006712F745: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  000000006712F74A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  000000006712F74F: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  000000006712F754: 41 54              push        r12
  000000006712F756: 48 83 EC 20        sub         rsp,20h
  000000006712F75A: 48 83 79 48 00     cmp         qword ptr [rcx+48h],0
  000000006712F75F: 49 8B E8           mov         rbp,r8
  000000006712F762: 48 63 F2           movsxd      rsi,edx
  000000006712F765: 48 8B F9           mov         rdi,rcx
  000000006712F768: 75 41              jne         000000006712F7AB
  000000006712F76A: B9 20 00 00 00     mov         ecx,20h
  000000006712F76F: E8 12 93 04 00     call        0000000067178A86
  000000006712F774: 48 85 C0           test        rax,rax
  000000006712F777: 74 29              je          000000006712F7A2
  000000006712F779: 48 8D 0D 38 14 1C  lea         rcx,[00000000672F0BB8h]
                    00
  000000006712F780: 48 89 08           mov         qword ptr [rax],rcx
  000000006712F783: F0 83 05 2D 14 1C  lock add    dword ptr [00000000672F0BB8h],1
                    00 01
  000000006712F78B: 33 C9              xor         ecx,ecx
  000000006712F78D: C6 40 08 00        mov         byte ptr [rax+8],0
  000000006712F791: C6 40 09 00        mov         byte ptr [rax+9],0
  000000006712F795: 89 48 0C           mov         dword ptr [rax+0Ch],ecx
  000000006712F798: 48 89 48 10        mov         qword ptr [rax+10h],rcx
  000000006712F79C: 48 89 48 18        mov         qword ptr [rax+18h],rcx
  000000006712F7A0: EB 05              jmp         000000006712F7A7
  000000006712F7A2: 33 C9              xor         ecx,ecx
  000000006712F7A4: 48 8B C1           mov         rax,rcx
  000000006712F7A7: 48 89 47 48        mov         qword ptr [rdi+48h],rax
  000000006712F7AB: 4C 8B 67 48        mov         r12,qword ptr [rdi+48h]
  000000006712F7AF: 49 8B 04 24        mov         rax,qword ptr [r12]
  000000006712F7B3: 3B 70 08           cmp         esi,dword ptr [rax+8]
  000000006712F7B6: 7C 41              jl          000000006712F7F9
  000000006712F7B8: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006712F7BC: 8D 5E 01           lea         ebx,[rsi+1]
  000000006712F7BF: 41 3B D8           cmp         ebx,r8d
  000000006712F7C2: 7F 14              jg          000000006712F7D8
  000000006712F7C4: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006712F7C8: 75 25              jne         000000006712F7EF
  000000006712F7CA: 3B 58 08           cmp         ebx,dword ptr [rax+8]
  000000006712F7CD: 7D 20              jge         000000006712F7EF
  000000006712F7CF: 41 8B C0           mov         eax,r8d
  000000006712F7D2: D1 F8              sar         eax,1
  000000006712F7D4: 3B D8              cmp         ebx,eax
  000000006712F7D6: 7D 17              jge         000000006712F7EF
  000000006712F7D8: 41 B8 10 00 00 00  mov         r8d,10h
  000000006712F7DE: 41 B1 01           mov         r9b,1
  000000006712F7E1: 8B D3              mov         edx,ebx
  000000006712F7E3: 41 8D 48 10        lea         ecx,[r8+10h]
  000000006712F7E7: E8 B4 CD F4 FF     call        000000006707C5A0
  000000006712F7EC: 44 8B C0           mov         r8d,eax
  000000006712F7EF: 8B D3              mov         edx,ebx
  000000006712F7F1: 49 8B CC           mov         rcx,r12
  000000006712F7F4: E8 C7 B1 FF FF     call        000000006712A9C0
  000000006712F7F9: 48 8B 5F 48        mov         rbx,qword ptr [rdi+48h]
  000000006712F7FD: 85 F6              test        esi,esi
  000000006712F7FF: 79 06              jns         000000006712F807
  000000006712F801: 48 8D 43 10        lea         rax,[rbx+10h]
  000000006712F805: EB 27              jmp         000000006712F82E
  000000006712F807: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712F80A: 8B 08              mov         ecx,dword ptr [rax]
  000000006712F80C: 83 F9 01           cmp         ecx,1
  000000006712F80F: 74 12              je          000000006712F823
  000000006712F811: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712F814: 48 8B CB           mov         rcx,rbx
  000000006712F817: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006712F81B: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006712F81E: E8 9D B1 FF FF     call        000000006712A9C0
  000000006712F823: 48 8D 46 01        lea         rax,[rsi+1]
  000000006712F827: 48 C1 E0 04        shl         rax,4
  000000006712F82B: 48 03 03           add         rax,qword ptr [rbx]
  000000006712F82E: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006712F832: 48 85 C9           test        rcx,rcx
  000000006712F835: 74 06              je          000000006712F83D
  000000006712F837: 48 89 69 18        mov         qword ptr [rcx+18h],rbp
  000000006712F83B: EB 03              jmp         000000006712F840
  000000006712F83D: 48 89 28           mov         qword ptr [rax],rbp
  000000006712F840: 48 8B CF           mov         rcx,rdi
  000000006712F843: 48 89 68 08        mov         qword ptr [rax+8],rbp
  000000006712F847: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006712F84C: 48 8B 6C 24 38     mov         rbp,qword ptr [rsp+38h]
  000000006712F851: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  000000006712F856: 48 8B 7C 24 48     mov         rdi,qword ptr [rsp+48h]
  000000006712F85B: 48 83 C4 20        add         rsp,20h
  000000006712F85F: 41 5C              pop         r12
  000000006712F861: E9 9A EC FF FF     jmp         000000006712E500

  Summary

      1A7000 .text
