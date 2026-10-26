Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006503F250: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000006503F255: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000006503F25A: 55                 push        rbp
  000000006503F25B: 56                 push        rsi
  000000006503F25C: 57                 push        rdi
  000000006503F25D: 41 54              push        r12
  000000006503F25F: 41 55              push        r13
  000000006503F261: 48 83 EC 20        sub         rsp,20h
  000000006503F265: 41 8B F9           mov         edi,r9d
  000000006503F268: 41 8B E8           mov         ebp,r8d
  000000006503F26B: 4C 8B EA           mov         r13,rdx
  000000006503F26E: 4C 8B E1           mov         r12,rcx
  000000006503F271: 45 85 C9           test        r9d,r9d
  000000006503F274: 78 12              js          000000006503F288
  000000006503F276: 45 3B C8           cmp         r9d,r8d
  000000006503F279: 7D 0D              jge         000000006503F288
  000000006503F27B: 48 8D 0D 96 37 66  lea         rcx,[00000000656A2A18h]
                    00
  000000006503F282: FF 15 10 77 65 00  call        qword ptr [0000000065696998h]
  000000006503F288: 8B 5C 24 78        mov         ebx,dword ptr [rsp+78h]
  000000006503F28C: 8B 74 24 70        mov         esi,dword ptr [rsp+70h]
  000000006503F290: 85 DB              test        ebx,ebx
  000000006503F292: 78 11              js          000000006503F2A5
  000000006503F294: 3B DE              cmp         ebx,esi
  000000006503F296: 7D 0D              jge         000000006503F2A5
  000000006503F298: 48 8D 0D 41 37 66  lea         rcx,[00000000656A29E0h]
                    00
  000000006503F29F: FF 15 F3 76 65 00  call        qword ptr [0000000065696998h]
  000000006503F2A5: 3B EF              cmp         ebp,edi
  000000006503F2A7: 75 25              jne         000000006503F2CE
  000000006503F2A9: 3B F3              cmp         esi,ebx
  000000006503F2AB: 75 21              jne         000000006503F2CE
  000000006503F2AD: 44 8B CE           mov         r9d,esi
  000000006503F2B0: 44 8B C5           mov         r8d,ebp
  000000006503F2B3: 49 8B D5           mov         rdx,r13
  000000006503F2B6: 49 8B CC           mov         rcx,r12
  000000006503F2B9: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  000000006503F2BE: 48 83 C4 20        add         rsp,20h
  000000006503F2C2: 41 5D              pop         r13
  000000006503F2C4: 41 5C              pop         r12
  000000006503F2C6: 5F                 pop         rdi
  000000006503F2C7: 5E                 pop         rsi
  000000006503F2C8: 5D                 pop         rbp
  000000006503F2C9: E9 D2 FE FF FF     jmp         000000006503F1A0
  000000006503F2CE: 44 8D 47 01        lea         r8d,[rdi+1]
  000000006503F2D2: 8D 43 01           lea         eax,[rbx+1]
  000000006503F2D5: 49 8D 8C 24 C0 00  lea         rcx,[r12+00000000000000C0h]
                    00 00
  000000006503F2DD: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  000000006503F2E1: 44 89 44 24 60     mov         dword ptr [rsp+60h],r8d
  000000006503F2E6: 4C 89 74 24 50     mov         qword ptr [rsp+50h],r14
  000000006503F2EB: 4D 8D B4 24 C4 00  lea         r14,[r12+00000000000000C4h]
                    00 00
  000000006503F2F3: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  000000006503F2F8: 41 3B 06           cmp         eax,dword ptr [r14]
  000000006503F2FB: 48 8D 44 24 60     lea         rax,[rsp+60h]
  000000006503F300: 49 0F 4C D6        cmovl       rdx,r14
  000000006503F304: 44 3B 01           cmp         r8d,dword ptr [rcx]
  000000006503F307: 44 8B 02           mov         r8d,dword ptr [rdx]
  000000006503F30A: 48 0F 4C C1        cmovl       rax,rcx
  000000006503F30E: 49 8B CC           mov         rcx,r12
  000000006503F311: 8B 10              mov         edx,dword ptr [rax]
  000000006503F313: E8 C8 D8 FF FF     call        000000006503CBE0
  000000006503F318: 41 89 6D 08        mov         dword ptr [r13+8],ebp
  000000006503F31C: 41 89 75 0C        mov         dword ptr [r13+0Ch],esi
  000000006503F320: 41 89 7D 10        mov         dword ptr [r13+10h],edi
  000000006503F324: 49 8D 8C 24 00 01  lea         rcx,[r12+0000000000000100h]
                    00 00
  000000006503F32C: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006503F331: 41 89 5D 14        mov         dword ptr [r13+14h],ebx
  000000006503F335: E8 46 4D 16 00     call        00000000651A4080
  000000006503F33A: 41 83 8C 24 34 01  or          dword ptr [r12+0000000000000134h],4
                    00 00 04
  000000006503F343: 41 C7 84 24 08 01  mov         dword ptr [r12+0000000000000108h],0FFFFFFFFh
                    00 00 FF FF FF FF
  000000006503F34F: 85 DB              test        ebx,ebx
  000000006503F351: 79 05              jns         000000006503F358
  000000006503F353: 41 8B 1E           mov         ebx,dword ptr [r14]
  000000006503F356: FF CB              dec         ebx
  000000006503F358: 44 8B C3           mov         r8d,ebx
  000000006503F35B: 8B D7              mov         edx,edi
  000000006503F35D: 49 8B CC           mov         rcx,r12
  000000006503F360: 4C 8B 74 24 50     mov         r14,qword ptr [rsp+50h]
  000000006503F365: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  000000006503F36A: 48 83 C4 20        add         rsp,20h
  000000006503F36E: 41 5D              pop         r13
  000000006503F370: 41 5C              pop         r12
  000000006503F372: 5F                 pop         rdi
  000000006503F373: 5E                 pop         rsi
  000000006503F374: 5D                 pop         rbp
  000000006503F375: E9 E6 C8 FF FF     jmp         000000006503BC60

  Summary

      695000 .text
