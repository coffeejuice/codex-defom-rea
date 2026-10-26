Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014147295C: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000141472961: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  0000000141472966: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014147296B: 57                 push        rdi
  000000014147296C: 41 54              push        r12
  000000014147296E: 41 55              push        r13
  0000000141472970: 48 83 EC 30        sub         rsp,30h
  0000000141472974: 4D 8B E1           mov         r12,r9
  0000000141472977: 49 8B F0           mov         rsi,r8
  000000014147297A: 48 8B DA           mov         rbx,rdx
  000000014147297D: 4C 8B E9           mov         r13,rcx
  0000000141472980: 33 FF              xor         edi,edi
  0000000141472982: 33 C0              xor         eax,eax
  0000000141472984: 48 85 D2           test        rdx,rdx
  0000000141472987: 0F 95 C0           setne       al
  000000014147298A: 85 C0              test        eax,eax
  000000014147298C: 75 27              jne         00000001414729B5
  000000014147298E: E8 A1 17 00 00     call        0000000141474134
  0000000141472993: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  0000000141472999: 48 21 7C 24 20     and         qword ptr [rsp+20h],rdi
  000000014147299E: 45 33 C9           xor         r9d,r9d
  00000001414729A1: 45 33 C0           xor         r8d,r8d
  00000001414729A4: 33 D2              xor         edx,edx
  00000001414729A6: 33 C9              xor         ecx,ecx
  00000001414729A8: E8 BB DB FF FF     call        0000000141470568
  00000001414729AD: 83 C8 FF           or          eax,0FFFFFFFFh
  00000001414729B0: E9 FD 00 00 00     jmp         0000000141472AB2
  00000001414729B5: 33 C0              xor         eax,eax
  00000001414729B7: 4D 85 C0           test        r8,r8
  00000001414729BA: 0F 95 C0           setne       al
  00000001414729BD: 85 C0              test        eax,eax
  00000001414729BF: 75 27              jne         00000001414729E8
  00000001414729C1: E8 6E 17 00 00     call        0000000141474134
  00000001414729C6: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  00000001414729CC: 48 21 7C 24 20     and         qword ptr [rsp+20h],rdi
  00000001414729D1: 45 33 C9           xor         r9d,r9d
  00000001414729D4: 45 33 C0           xor         r8d,r8d
  00000001414729D7: 33 D2              xor         edx,edx
  00000001414729D9: 33 C9              xor         ecx,ecx
  00000001414729DB: E8 88 DB FF FF     call        0000000141470568
  00000001414729E0: 83 C8 FF           or          eax,0FFFFFFFFh
  00000001414729E3: E9 CA 00 00 00     jmp         0000000141472AB2
  00000001414729E8: 48 8B CA           mov         rcx,rdx
  00000001414729EB: E8 A8 B2 FF FF     call        000000014146DC98
  00000001414729F0: 90                 nop
  00000001414729F1: F6 43 18 40        test        byte ptr [rbx+18h],40h
  00000001414729F5: 0F 85 96 00 00 00  jne         0000000141472A91
  00000001414729FB: 48 8B CB           mov         rcx,rbx
  00000001414729FE: E8 45 70 00 00     call        0000000141479A48
  0000000141472A03: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141472A06: 74 2A              je          0000000141472A32
  0000000141472A08: 83 F8 FE           cmp         eax,0FFFFFFFEh
  0000000141472A0B: 74 25              je          0000000141472A32
  0000000141472A0D: 48 63 D0           movsxd      rdx,eax
  0000000141472A10: 48 8B CA           mov         rcx,rdx
  0000000141472A13: 48 C1 F9 05        sar         rcx,5
  0000000141472A17: 4C 8D 05 E2 16 BE  lea         r8,[0000000145054100h]
                    03
  0000000141472A1E: 83 E2 1F           and         edx,1Fh
  0000000141472A21: 48 6B D2 58        imul        rdx,rdx,58h
  0000000141472A25: 49 03 14 C8        add         rdx,qword ptr [r8+rcx*8]
  0000000141472A29: 48 8D 0D C0 BC 6E  lea         rcx,[0000000143B5E6F0h]
                    02
  0000000141472A30: EB 11              jmp         0000000141472A43
  0000000141472A32: 48 8D 0D B7 BC 6E  lea         rcx,[0000000143B5E6F0h]
                    02
  0000000141472A39: 48 8B D1           mov         rdx,rcx
  0000000141472A3C: 4C 8D 05 BD 16 BE  lea         r8,[0000000145054100h]
                    03
  0000000141472A43: F6 42 38 7F        test        byte ptr [rdx+38h],7Fh
  0000000141472A47: 75 25              jne         0000000141472A6E
  0000000141472A49: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141472A4C: 74 1A              je          0000000141472A68
  0000000141472A4E: 83 F8 FE           cmp         eax,0FFFFFFFEh
  0000000141472A51: 74 15              je          0000000141472A68
  0000000141472A53: 48 63 C8           movsxd      rcx,eax
  0000000141472A56: 48 8B C1           mov         rax,rcx
  0000000141472A59: 48 C1 F8 05        sar         rax,5
  0000000141472A5D: 83 E1 1F           and         ecx,1Fh
  0000000141472A60: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141472A64: 49 03 0C C0        add         rcx,qword ptr [r8+rax*8]
  0000000141472A68: F6 41 38 80        test        byte ptr [rcx+38h],80h
  0000000141472A6C: 74 23              je          0000000141472A91
  0000000141472A6E: E8 C1 16 00 00     call        0000000141474134
  0000000141472A73: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  0000000141472A79: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  0000000141472A7F: 45 33 C9           xor         r9d,r9d
  0000000141472A82: 45 33 C0           xor         r8d,r8d
  0000000141472A85: 33 D2              xor         edx,edx
  0000000141472A87: 33 C9              xor         ecx,ecx
  0000000141472A89: E8 DA DA FF FF     call        0000000141470568
  0000000141472A8E: 83 CF FF           or          edi,0FFFFFFFFh
  0000000141472A91: 85 FF              test        edi,edi
  0000000141472A93: 75 13              jne         0000000141472AA8
  0000000141472A95: 4C 8B 4C 24 70     mov         r9,qword ptr [rsp+70h]
  0000000141472A9A: 4D 8B C4           mov         r8,r12
  0000000141472A9D: 48 8B D6           mov         rdx,rsi
  0000000141472AA0: 48 8B CB           mov         rcx,rbx
  0000000141472AA3: 41 FF D5           call        r13
  0000000141472AA6: 8B F8              mov         edi,eax
  0000000141472AA8: 48 8B CB           mov         rcx,rbx
  0000000141472AAB: E8 78 B2 FF FF     call        000000014146DD28
  0000000141472AB0: 8B C7              mov         eax,edi
  0000000141472AB2: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  0000000141472AB7: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  0000000141472ABC: 48 83 C4 30        add         rsp,30h
  0000000141472AC0: 41 5D              pop         r13
  0000000141472AC2: 41 5C              pop         r12
  0000000141472AC4: 5F                 pop         rdi
  0000000141472AC5: C3                 ret

  Summary

     1E02000 .text
