Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000652BCE60: 48 8B C4           mov         rax,rsp
  00000000652BCE63: 44 89 40 18        mov         dword ptr [rax+18h],r8d
  00000000652BCE67: 48 89 50 10        mov         qword ptr [rax+10h],rdx
  00000000652BCE6B: 55                 push        rbp
  00000000652BCE6C: 56                 push        rsi
  00000000652BCE6D: 57                 push        rdi
  00000000652BCE6E: 41 54              push        r12
  00000000652BCE70: 41 55              push        r13
  00000000652BCE72: 41 56              push        r14
  00000000652BCE74: 41 57              push        r15
  00000000652BCE76: 48 81 EC F0 06 00  sub         rsp,6F0h
                    00
  00000000652BCE7D: 48 C7 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000000652BCE89: 48 89 58 20        mov         qword ptr [rax+20h],rbx
  00000000652BCE8D: 4D 8B F1           mov         r14,r9
  00000000652BCE90: 49 63 E8           movsxd      rbp,r8d
  00000000652BCE93: 48 8B FA           mov         rdi,rdx
  00000000652BCE96: 4C 8B F9           mov         r15,rcx
  00000000652BCE99: 45 33 ED           xor         r13d,r13d
  00000000652BCE9C: 44 89 68 08        mov         dword ptr [rax+8],r13d
  00000000652BCEA0: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000652BCEA4: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000652BCEA9: FF 15 89 99 3D 00  call        qword ptr [0000000065696838h]
  00000000652BCEAF: 83 FD 3F           cmp         ebp,3Fh
  00000000652BCEB2: 0F 87 10 34 00 00  ja          00000000652C02C8
  00000000652BCEB8: 48 8D 35 41 31 D4  lea         rsi,[0000000065000000h]
                    FF
  00000000652BCEBF: 0F B6 84 2E 90 03  movzx       eax,byte ptr [rsi+rbp+00000000002C0390h]
                    2C 00
  00000000652BCEC7: 8B 8C 86 0C 03 2C  mov         ecx,dword ptr [rsi+rax*4+00000000002C030Ch]
                    00
  00000000652BCECE: 48 03 CE           add         rcx,rsi
  00000000652BCED1: FF E1              jmp         rcx
  00000000652BCED3: 4D 85 F6           test        r14,r14
  00000000652BCED6: 0F 84 EC 33 00 00  je          00000000652C02C8
  00000000652BCEDC: 41 83 3E 01        cmp         dword ptr [r14],1
  00000000652BCEE0: 0F 8C E2 33 00 00  jl          00000000652C02C8
  00000000652BCEE6: 41 83 7E 04 02     cmp         dword ptr [r14+4],2
  00000000652BCEEB: 0F 85 D7 33 00 00  jne         00000000652C02C8
  00000000652BCEF1: 49 8B CF           mov         rcx,r15
  00000000652BCEF4: E8 A7 B0 FE FF     call        00000000652A7FA0
  00000000652BCEF9: 4C 8B 10           mov         r10,qword ptr [rax]
  00000000652BCEFC: 48 8B 9C 24 50 07  mov         rbx,qword ptr [rsp+0000000000000750h]
                    00 00
  00000000652BCF04: 4C 8B CB           mov         r9,rbx
  00000000652BCF07: 4D 8B C6           mov         r8,r14
  00000000652BCF0A: BA 05 00 00 00     mov         edx,5
  00000000652BCF0F: 48 8B C8           mov         rcx,rax
  00000000652BCF12: 41 FF 92 D8 00 00  call        qword ptr [r10+00000000000000D8h]
                    00
  00000000652BCF19: 44 8B E0           mov         r12d,eax
  00000000652BCF1C: 41
  00000000652BCF1D: F6
  00000000652BCF1E: 46
  00000000652BCF1F: 38

  Summary

      695000 .text
