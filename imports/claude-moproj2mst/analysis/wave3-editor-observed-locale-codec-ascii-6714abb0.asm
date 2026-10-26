Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714ABB0: 48 8B C4           mov         rax,rsp
  000000006714ABB3: 48 89 50 10        mov         qword ptr [rax+10h],rdx
  000000006714ABB7: 56                 push        rsi
  000000006714ABB8: 57                 push        rdi
  000000006714ABB9: 41 54              push        r12
  000000006714ABBB: 48 83 EC 60        sub         rsp,60h
  000000006714ABBF: 48 C7 44 24 50 FE  mov         qword ptr [rsp+50h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006714ABC8: 48 89 58 08        mov         qword ptr [rax+8],rbx
  000000006714ABCC: 48 89 68 20        mov         qword ptr [rax+20h],rbp
  000000006714ABD0: 41 8B F1           mov         esi,r9d
  000000006714ABD3: 49 8B E8           mov         rbp,r8
  000000006714ABD6: 48 8B DA           mov         rbx,rdx
  000000006714ABD9: 45 33 E4           xor         r12d,r12d
  000000006714ABDC: 44 89 60 C8        mov         dword ptr [rax-38h],r12d
  000000006714ABE0: 4D 85 C0           test        r8,r8
  000000006714ABE3: 75 1A              jne         000000006714ABFF
  000000006714ABE5: 48 8D 05 14 5D 1A  lea         rax,[00000000672F0900h]
                    00
  000000006714ABEC: 48 89 02           mov         qword ptr [rdx],rax
  000000006714ABEF: F0 83 05 09 5D 1A  lock add    dword ptr [00000000672F0900h],1
                    00 01
  000000006714ABF7: 48 8B C2           mov         rax,rdx
  000000006714ABFA: E9 1B 01 00 00     jmp         000000006714AD1A
  000000006714ABFF: 45 85 C9           test        r9d,r9d
  000000006714AC02: 75 18              jne         000000006714AC1C
  000000006714AC04: 48 8D 15 35 90 0A  lea         rdx,[00000000671F3C40h]
                    00
  000000006714AC0B: 48 8B CB           mov         rcx,rbx
  000000006714AC0E: E8 6D 28 EE FF     call        000000006702D480
  000000006714AC13: 90                 nop
  000000006714AC14: 48 8B C3           mov         rax,rbx
  000000006714AC17: E9 FE 00 00 00     jmp         000000006714AD1A
  000000006714AC1C: 45 33 C0           xor         r8d,r8d
  000000006714AC1F: BA 00 10 00 00     mov         edx,1000h
  000000006714AC24: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006714AC29: E8 C2 29 EE FF     call        000000006702D5F0
  000000006714AC2E: 90                 nop
  000000006714AC2F: 90                 nop
  000000006714AC30: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006714AC35: 8B 79 08           mov         edi,dword ptr [rcx+8]
  000000006714AC38: 8B 01              mov         eax,dword ptr [rcx]
  000000006714AC3A: 83 F8 01           cmp         eax,1
  000000006714AC3D: 75 0F              jne         000000006714AC4E
  000000006714AC3F: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000006714AC44: 48 8D 50 18        lea         rdx,[rax+18h]
  000000006714AC48: 48 39 50 10        cmp         qword ptr [rax+10h],rdx
  000000006714AC4C: 74 12              je          000000006714AC60
  000000006714AC4E: 8B 51 08           mov         edx,dword ptr [rcx+8]
  000000006714AC51: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006714AC56: E8 B5 2A EE FF     call        000000006702D710
  000000006714AC5B: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000006714AC60: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  000000006714AC64: 8D 47 FF           lea         eax,[rdi-1]
  000000006714AC67: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  000000006714AC6F: 48 89 54 24 38     mov         qword ptr [rsp+38h],rdx
  000000006714AC74: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006714AC79: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000006714AC7D: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  000000006714AC82: 44 8B CE           mov         r9d,esi
  000000006714AC85: 4C 8B C5           mov         r8,rbp
  000000006714AC88: 33 D2              xor         edx,edx
  000000006714AC8A: 33 C9              xor         ecx,ecx
  000000006714AC8C: FF 15 9E D6 05 00  call        qword ptr [00000000671A8330h]
  000000006714AC92: 8B F8              mov         edi,eax
  000000006714AC94: 85 C0              test        eax,eax
  000000006714AC96: 75 49              jne         000000006714ACE1
  000000006714AC98: FF 15 42 D7 05 00  call        qword ptr [00000000671A83E0h]
  000000006714AC9E: 83 F8 7A           cmp         eax,7Ah
  000000006714ACA1: 75 3E              jne         000000006714ACE1
  000000006714ACA3: 48 8D 84 24 90 00  lea         rax,[rsp+0000000000000090h]
                    00 00
  000000006714ACAB: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000006714ACB0: 4C 89 64 24 30     mov         qword ptr [rsp+30h],r12
  000000006714ACB5: 44 89 64 24 28     mov         dword ptr [rsp+28h],r12d
  000000006714ACBA: 4C 89 64 24 20     mov         qword ptr [rsp+20h],r12
  000000006714ACBF: 44 8B CE           mov         r9d,esi
  000000006714ACC2: 4C 8B C5           mov         r8,rbp
  000000006714ACC5: 33 D2              xor         edx,edx
  000000006714ACC7: 33 C9              xor         ecx,ecx
  000000006714ACC9: FF 15 61 D6 05 00  call        qword ptr [00000000671A8330h]
  000000006714ACCF: 8D 50 01           lea         edx,[rax+1]
  000000006714ACD2: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006714ACD7: E8 24 3F EE FF     call        000000006702EC00
  000000006714ACDC: E9 4F FF FF FF     jmp         000000006714AC30
  000000006714ACE1: 8B D7              mov         edx,edi
  000000006714ACE3: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006714ACE8: E8 13 3F EE FF     call        000000006702EC00
  000000006714ACED: 4C 8B 5C 24 48     mov         r11,qword ptr [rsp+48h]
  000000006714ACF2: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714ACF5: F0 41 83 03 01     lock add    dword ptr [r11],1
  000000006714ACFA: C7 44 24 40 01 00  mov         dword ptr [rsp+40h],1
                    00 00
  000000006714AD02: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000006714AD07: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006714AD0B: 75 0A              jne         000000006714AD17
  000000006714AD0D: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006714AD12: E8 99 56 ED FF     call        00000000670203B0
  000000006714AD17: 48 8B C3           mov         rax,rbx
  000000006714AD1A: 4C 8D 5C 24 60     lea         r11,[rsp+60h]
  000000006714AD1F: 49 8B 5B 20        mov         rbx,qword ptr [r11+20h]
  000000006714AD23: 49 8B 6B 38        mov         rbp,qword ptr [r11+38h]
  000000006714AD27: 49 8B E3           mov         rsp,r11
  000000006714AD2A: 41 5C              pop         r12
  000000006714AD2C: 5F                 pop         rdi
  000000006714AD2D: 5E                 pop         rsi
  000000006714AD2E: C3                 ret
  000000006714AD2F: CC                 int         3

  Summary

      1A7000 .text
