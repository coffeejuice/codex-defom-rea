Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000651F2E30: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000651F2E35: 55                 push        rbp
  00000000651F2E36: 56                 push        rsi
  00000000651F2E37: 57                 push        rdi
  00000000651F2E38: 48 83 EC 30        sub         rsp,30h
  00000000651F2E3C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000651F2E45: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  00000000651F2E4A: 49 8B F0           mov         rsi,r8
  00000000651F2E4D: 48 8B DA           mov         rbx,rdx
  00000000651F2E50: 48 8B F9           mov         rdi,rcx
  00000000651F2E53: 48 C7 01 00 00 00  mov         qword ptr [rcx],0
                    00
  00000000651F2E5A: 8B 42 08           mov         eax,dword ptr [rdx+8]
  00000000651F2E5D: 89 41 08           mov         dword ptr [rcx+8],eax
  00000000651F2E60: 49 8B 00           mov         rax,qword ptr [r8]
  00000000651F2E63: BA 08 00 00 00     mov         edx,8
  00000000651F2E68: 49 8B C8           mov         rcx,r8
  00000000651F2E6B: FF 50 28           call        qword ptr [rax+28h]
  00000000651F2E6E: 8B E8              mov         ebp,eax
  00000000651F2E70: 48 8B 13           mov         rdx,qword ptr [rbx]
  00000000651F2E73: 39 42 38           cmp         dword ptr [rdx+38h],eax
  00000000651F2E76: 75 10              jne         00000000651F2E88
  00000000651F2E78: 83 7A 3C 00        cmp         dword ptr [rdx+3Ch],0
  00000000651F2E7C: 75 0A              jne         00000000651F2E88
  00000000651F2E7E: 48 8B CF           mov         rcx,rdi
  00000000651F2E81: E8 BA F9 FF FF     call        00000000651F2840
  00000000651F2E86: EB 3F              jmp         00000000651F2EC7
  00000000651F2E88: B9 60 00 00 00     mov         ecx,60h
  00000000651F2E8D: E8 E0 E2 3E 00     call        00000000655E1172
  00000000651F2E92: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000000651F2E97: 48 85 C0           test        rax,rax
  00000000651F2E9A: 74 0D              je          00000000651F2EA9
  00000000651F2E9C: 48 8B 13           mov         rdx,qword ptr [rbx]
  00000000651F2E9F: 48 8B C8           mov         rcx,rax
  00000000651F2EA2: E8 A9 F0 FF FF     call        00000000651F1F50
  00000000651F2EA7: EB 02              jmp         00000000651F2EAB
  00000000651F2EA9: 33 C0              xor         eax,eax
  00000000651F2EAB: 48 8B D0           mov         rdx,rax
  00000000651F2EAE: 48 8B CF           mov         rcx,rdi
  00000000651F2EB1: E8 8A F9 FF FF     call        00000000651F2840
  00000000651F2EB6: 4C 8B 1F           mov         r11,qword ptr [rdi]
  00000000651F2EB9: 41 89 6B 38        mov         dword ptr [r11+38h],ebp
  00000000651F2EBD: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000651F2EC0: C7 40 3C 00 00 00  mov         dword ptr [rax+3Ch],0
                    00
  00000000651F2EC7: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000651F2ECA: 48 8B CE           mov         rcx,rsi
  00000000651F2ECD: FF 50 08           call        qword ptr [rax+8]
  00000000651F2ED0: 83 F8 04           cmp         eax,4
  00000000651F2ED3: 75 28              jne         00000000651F2EFD
  00000000651F2ED5: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000651F2ED8: 48 8B CE           mov         rcx,rsi
  00000000651F2EDB: FF 50 18           call        qword ptr [rax+18h]
  00000000651F2EDE: 48 85 C0           test        rax,rax
  00000000651F2EE1: 74 15              je          00000000651F2EF8
  00000000651F2EE3: 48 8B 1F           mov         rbx,qword ptr [rdi]
  00000000651F2EE6: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000651F2EE9: 48 8B CE           mov         rcx,rsi
  00000000651F2EEC: FF 50 18           call        qword ptr [rax+18h]
  00000000651F2EEF: 48 89 43 40        mov         qword ptr [rbx+40h],rax
  00000000651F2EF3: 48 8B C7           mov         rax,rdi
  00000000651F2EF6: EB 08              jmp         00000000651F2F00
  00000000651F2EF8: 48 8B C7           mov         rax,rdi
  00000000651F2EFB: EB 03              jmp         00000000651F2F00
  00000000651F2EFD: 48 8B C7           mov         rax,rdi
  00000000651F2F00: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  00000000651F2F05: 48 83 C4 30        add         rsp,30h
  00000000651F2F09: 5F                 pop         rdi
  00000000651F2F0A: 5E                 pop         rsi
  00000000651F2F0B: 5D                 pop         rbp
  00000000651F2F0C: C3                 ret

  Summary

      695000 .text
