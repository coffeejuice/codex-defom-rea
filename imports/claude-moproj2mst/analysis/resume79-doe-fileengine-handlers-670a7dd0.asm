Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670A7DD0: 40 57              push        rdi
  00000000670A7DD2: 41 54              push        r12
  00000000670A7DD4: 41 55              push        r13
  00000000670A7DD6: 48 83 EC 30        sub         rsp,30h
  00000000670A7DDA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670A7DE3: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000670A7DE8: 48 89 6C 24 60     mov         qword ptr [rsp+60h],rbp
  00000000670A7DED: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  00000000670A7DF2: 4C 8B E9           mov         r13,rcx
  00000000670A7DF5: 33 FF              xor         edi,edi
  00000000670A7DF7: 40 38 3D D2 C0 24  cmp         byte ptr [00000000672F3ED0h],dil
                    00
  00000000670A7DFE: 0F 84 A4 00 00 00  je          00000000670A7EA8
  00000000670A7E04: E8 F7 FC FF FF     call        00000000670A7B00
  00000000670A7E09: 48 8B D8           mov         rbx,rax
  00000000670A7E0C: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000000670A7E11: 48 85 C0           test        rax,rax
  00000000670A7E14: 74 19              je          00000000670A7E2F
  00000000670A7E16: A8 01              test        al,1
  00000000670A7E18: 75 15              jne         00000000670A7E2F
  00000000670A7E1A: 48 8B C8           mov         rcx,rax
  00000000670A7E1D: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  00000000670A7E21: E8 5A E6 F7 FF     call        0000000067026480
  00000000670A7E26: 48 83 CB 01        or          rbx,1
  00000000670A7E2A: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000670A7E2F: E8 2C FE FF FF     call        00000000670A7C60
  00000000670A7E34: 4C 8B E0           mov         r12,rax
  00000000670A7E37: 8B F7              mov         esi,edi
  00000000670A7E39: 48 8B 10           mov         rdx,qword ptr [rax]
  00000000670A7E3C: 44 8B 42 08        mov         r8d,dword ptr [rdx+8]
  00000000670A7E40: 48 8B EF           mov         rbp,rdi
  00000000670A7E43: 8B 4A 0C           mov         ecx,dword ptr [rdx+0Ch]
  00000000670A7E46: 41 2B C8           sub         ecx,r8d
  00000000670A7E49: 85 C9              test        ecx,ecx
  00000000670A7E4B: 7E 36              jle         00000000670A7E83
  00000000670A7E4D: 0F 1F 00           nop         dword ptr [rax]
  00000000670A7E50: 49 63 C8           movsxd      rcx,r8d
  00000000670A7E53: 48 03 CD           add         rcx,rbp
  00000000670A7E56: 48 8B 4C CA 18     mov         rcx,qword ptr [rdx+rcx*8+18h]
  00000000670A7E5B: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670A7E5E: 49 8B D5           mov         rdx,r13
  00000000670A7E61: FF 50 08           call        qword ptr [rax+8]
  00000000670A7E64: 48 8B F8           mov         rdi,rax
  00000000670A7E67: 48 85 C0           test        rax,rax
  00000000670A7E6A: 75 17              jne         00000000670A7E83
  00000000670A7E6C: FF C6              inc         esi
  00000000670A7E6E: 48 FF C5           inc         rbp
  00000000670A7E71: 49 8B 14 24        mov         rdx,qword ptr [r12]
  00000000670A7E75: 44 8B 42 08        mov         r8d,dword ptr [rdx+8]
  00000000670A7E79: 8B 4A 0C           mov         ecx,dword ptr [rdx+0Ch]
  00000000670A7E7C: 41 2B C8           sub         ecx,r8d
  00000000670A7E7F: 3B F1              cmp         esi,ecx
  00000000670A7E81: 7C CD              jl          00000000670A7E50
  00000000670A7E83: 48 85 DB           test        rbx,rbx
  00000000670A7E86: 74 1B              je          00000000670A7EA3
  00000000670A7E88: F6 C3 01           test        bl,1
  00000000670A7E8B: 74 11              je          00000000670A7E9E
  00000000670A7E8D: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  00000000670A7E91: 48 8B CB           mov         rcx,rbx
  00000000670A7E94: E8 17 EC F7 FF     call        0000000067026AB0
  00000000670A7E99: 48 8B C7           mov         rax,rdi
  00000000670A7E9C: EB 0D              jmp         00000000670A7EAB
  00000000670A7E9E: 48 8B C7           mov         rax,rdi
  00000000670A7EA1: EB 08              jmp         00000000670A7EAB
  00000000670A7EA3: 48 8B C7           mov         rax,rdi
  00000000670A7EA6: EB 03              jmp         00000000670A7EAB
  00000000670A7EA8: 48 8B C7           mov         rax,rdi
  00000000670A7EAB: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000670A7EB0: 48 8B 6C 24 60     mov         rbp,qword ptr [rsp+60h]
  00000000670A7EB5: 48 8B 74 24 68     mov         rsi,qword ptr [rsp+68h]
  00000000670A7EBA: 48 83 C4 30        add         rsp,30h
  00000000670A7EBE: 41 5D              pop         r13
  00000000670A7EC0: 41 5C              pop         r12
  00000000670A7EC2: 5F                 pop         rdi
  00000000670A7EC3: C3                 ret

  Summary

      1A7000 .text
