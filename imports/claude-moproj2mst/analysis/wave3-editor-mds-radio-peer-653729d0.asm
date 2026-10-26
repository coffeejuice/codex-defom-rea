Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653729D0: 40 56              push        rsi
  00000000653729D2: 57                 push        rdi
  00000000653729D3: 41 54              push        r12
  00000000653729D5: 48 83 EC 30        sub         rsp,30h
  00000000653729D9: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653729E2: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000653729E7: 48 89 6C 24 60     mov         qword ptr [rsp+60h],rbp
  00000000653729EC: 48 8B E9           mov         rbp,rcx
  00000000653729EF: 48 8B 81 08 02 00  mov         rax,qword ptr [rcx+0000000000000208h]
                    00
  00000000653729F6: 48 85 C0           test        rax,rax
  00000000653729F9: 74 10              je          0000000065372A0B
  00000000653729FB: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000653729FF: 48 8B 80 90 00 00  mov         rax,qword ptr [rax+0000000000000090h]
                    00
  0000000065372A06: E9 DF 00 00 00     jmp         0000000065372AEA
  0000000065372A0B: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  0000000065372A0F: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000065372A14: E8 27 FE FF FF     call        0000000065372840
  0000000065372A19: 90                 nop
  0000000065372A1A: F6 85 04 02 00 00  test        byte ptr [rbp+0000000000000204h],8
                    08
  0000000065372A21: 0F 84 A7 00 00 00  je          0000000065372ACE
  0000000065372A27: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065372A2C: FF 15 86 3E 32 00  call        qword ptr [00000000656968B8h]
  0000000065372A32: 83 F8 01           cmp         eax,1
  0000000065372A35: 0F 84 93 00 00 00  je          0000000065372ACE
  0000000065372A3B: 45 33 E4           xor         r12d,r12d
  0000000065372A3E: 41 8B DC           mov         ebx,r12d
  0000000065372A41: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065372A46: FF 15 6C 3E 32 00  call        qword ptr [00000000656968B8h]
  0000000065372A4C: 85 C0              test        eax,eax
  0000000065372A4E: 7E 54              jle         0000000065372AA4
  0000000065372A50: 8B D3              mov         edx,ebx
  0000000065372A52: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065372A57: FF 15 E3 3E 32 00  call        qword ptr [0000000065696940h]
  0000000065372A5D: 48 8B 38           mov         rdi,qword ptr [rax]
  0000000065372A60: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065372A64: F6 80 04 02 00 00  test        byte ptr [rax+0000000000000204h],2
                    02
  0000000065372A6B: 74 05              je          0000000065372A72
  0000000065372A6D: 48 3B FE           cmp         rdi,rsi
  0000000065372A70: 75 13              jne         0000000065372A85
  0000000065372A72: FF C3              inc         ebx
  0000000065372A74: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065372A79: FF 15 39 3E 32 00  call        qword ptr [00000000656968B8h]
  0000000065372A7F: 3B D8              cmp         ebx,eax
  0000000065372A81: 7D 21              jge         0000000065372AA4
  0000000065372A83: EB CB              jmp         0000000065372A50
  0000000065372A85: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000065372A8A: FF 15 E8 3E 32 00  call        qword ptr [0000000065696978h]
  0000000065372A90: 84 C0              test        al,al
  0000000065372A92: 75 0B              jne         0000000065372A9F
  0000000065372A94: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000065372A99: FF 15 C1 3E 32 00  call        qword ptr [0000000065696960h]
  0000000065372A9F: 48 8B C7           mov         rax,rdi
  0000000065372AA2: EB 46              jmp         0000000065372AEA
  0000000065372AA4: F6 85 04 02 00 00  test        byte ptr [rbp+0000000000000204h],2
                    02
  0000000065372AAB: 4C 0F 45 E6        cmovne      r12,rsi
  0000000065372AAF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000065372AB4: FF 15 BE 3E 32 00  call        qword ptr [0000000065696978h]
  0000000065372ABA: 84 C0              test        al,al
  0000000065372ABC: 75 0B              jne         0000000065372AC9
  0000000065372ABE: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000065372AC3: FF 15 97 3E 32 00  call        qword ptr [0000000065696960h]
  0000000065372AC9: 49 8B C4           mov         rax,r12
  0000000065372ACC: EB 1C              jmp         0000000065372AEA
  0000000065372ACE: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000065372AD3: FF 15 9F 3E 32 00  call        qword ptr [0000000065696978h]
  0000000065372AD9: 84 C0              test        al,al
  0000000065372ADB: 75 0B              jne         0000000065372AE8
  0000000065372ADD: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000065372AE2: FF 15 78 3E 32 00  call        qword ptr [0000000065696960h]
  0000000065372AE8: 33 C0              xor         eax,eax
  0000000065372AEA: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  0000000065372AEF: 48 8B 6C 24 60     mov         rbp,qword ptr [rsp+60h]
  0000000065372AF4: 48 83 C4 30        add         rsp,30h
  0000000065372AF8: 41 5C              pop         r12
  0000000065372AFA: 5F                 pop         rdi
  0000000065372AFB: 5E                 pop         rsi
  0000000065372AFC: C3                 ret

  Summary

      695000 .text
