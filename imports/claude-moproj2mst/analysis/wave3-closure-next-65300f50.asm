Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065300F50: 48 8B C4           mov         rax,rsp
  0000000065300F53: 56                 push        rsi
  0000000065300F54: 57                 push        rdi
  0000000065300F55: 41 54              push        r12
  0000000065300F57: 48 81 EC A0 00 00  sub         rsp,0A0h
                    00
  0000000065300F5E: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065300F67: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  0000000065300F6B: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  0000000065300F6F: 49 8B E9           mov         rbp,r9
  0000000065300F72: 49 8B F8           mov         rdi,r8
  0000000065300F75: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  0000000065300F79: 33 DB              xor         ebx,ebx
  0000000065300F7B: 83 FA 22           cmp         edx,22h
  0000000065300F7E: 0F 84 30 01 00 00  je          00000000653010B4
  0000000065300F84: 83 FA 4A           cmp         edx,4Ah
  0000000065300F87: 0F 84 66 01 00 00  je          00000000653010F3
  0000000065300F8D: 83 FA 4D           cmp         edx,4Dh
  0000000065300F90: 74 19              je          0000000065300FAB
  0000000065300F92: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000065300F9A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000065300F9F: E8 6C E9 00 00     call        000000006530F910
  0000000065300FA4: 8B D8              mov         ebx,eax
  0000000065300FA6: E9 48 01 00 00     jmp         00000000653010F3
  0000000065300FAB: 4D 85 C0           test        r8,r8
  0000000065300FAE: 0F 84 3F 01 00 00  je          00000000653010F3
  0000000065300FB4: 48 8B B4 24 E0 00  mov         rsi,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000065300FBC: 48 85 F6           test        rsi,rsi
  0000000065300FBF: 0F 84 2E 01 00 00  je          00000000653010F3
  0000000065300FC5: 83 3E 01           cmp         dword ptr [rsi],1
  0000000065300FC8: 0F 8F 25 01 00 00  jg          00000000653010F3
  0000000065300FCE: 81 7E 04 01 F0 00  cmp         dword ptr [rsi+4],0F001h
                    00
  0000000065300FD5: 0F 85 18 01 00 00  jne         00000000653010F3
  0000000065300FDB: BB 01 00 00 00     mov         ebx,1
  0000000065300FE0: 48 8D 15 99 B1 3E  lea         rdx,[00000000656EC180h]
                    00
  0000000065300FE7: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065300FEC: FF 15 EE 59 39 00  call        qword ptr [00000000656969E0h]
  0000000065300FF2: 48 8B D0           mov         rdx,rax
  0000000065300FF5: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000065300FFD: FF 15 05 5C 39 00  call        qword ptr [0000000065696C08h]
  0000000065301003: 90                 nop
  0000000065301004: 48 89 6C 24 60     mov         qword ptr [rsp+60h],rbp
  0000000065301009: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  0000000065301012: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  000000006530101A: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000006530101F: FF 15 CB 59 39 00  call        qword ptr [00000000656969F0h]
  0000000065301025: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  000000006530102E: 89 9C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebx
                    00
  0000000065301035: 89 9C 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],ebx
                    00
  000000006530103C: 83 A4 24 88 00 00  and         dword ptr [rsp+0000000000000088h],0FFFFFFF0h
                    00 F0
  0000000065301044: C7 84 24 8C 00 00  mov         dword ptr [rsp+000000000000008Ch],0
                    00 00 00 00 00
  000000006530104F: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000065301057: 8B 47 10           mov         eax,dword ptr [rdi+10h]
  000000006530105A: 89 01              mov         dword ptr [rcx],eax
  000000006530105C: 8B 47 14           mov         eax,dword ptr [rdi+14h]
  000000006530105F: 89 41 04           mov         dword ptr [rcx+4],eax
  0000000065301062: 8B 47 18           mov         eax,dword ptr [rdi+18h]
  0000000065301065: 89 41 08           mov         dword ptr [rcx+8],eax
  0000000065301068: 8B 47 1C           mov         eax,dword ptr [rdi+1Ch]
  000000006530106B: 89 41 0C           mov         dword ptr [rcx+0Ch],eax
  000000006530106E: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000065301076: FF 15 E4 56 39 00  call        qword ptr [0000000065696760h]
  000000006530107C: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  0000000065301081: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000065301086: 49 8B CC           mov         rcx,r12
  0000000065301089: E8 C2 BB 00 00     call        000000006530CC50
  000000006530108E: 90                 nop
  000000006530108F: 48 8D 4E 08        lea         rcx,[rsi+8]
  0000000065301093: 48 8B D0           mov         rdx,rax
  0000000065301096: E8 B5 83 E8 FF     call        0000000065189450
  000000006530109B: 90                 nop
  000000006530109C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000653010A1: E8 7A 83 E8 FF     call        0000000065189420
  00000000653010A6: 90                 nop
  00000000653010A7: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000000653010AC: FF 15 AE 56 39 00  call        qword ptr [0000000065696760h]
  00000000653010B2: EB 3F              jmp         00000000653010F3
  00000000653010B4: 4D 85 C0           test        r8,r8
  00000000653010B7: 74 37              je          00000000653010F0
  00000000653010B9: 49 8D 48 28        lea         rcx,[r8+28h]
  00000000653010BD: BA 04 00 00 00     mov         edx,4
  00000000653010C2: 44 8D 42 05        lea         r8d,[rdx+5]
  00000000653010C6: E8 25 8B D4 FF     call        0000000065049BF0
  00000000653010CB: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000653010CE: 48 83 C1 08        add         rcx,8
  00000000653010D2: 41 B8 76 00 00 00  mov         r8d,76h
  00000000653010D8: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000000653010DD: E8 3E 11 E2 FF     call        0000000065122220
  00000000653010E2: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000000653010E7: E8 F4 06 E2 FF     call        00000000651217E0
  00000000653010EC: 8B D8              mov         ebx,eax
  00000000653010EE: EB 03              jmp         00000000653010F3
  00000000653010F0: 83 CB FF           or          ebx,0FFFFFFFFh
  00000000653010F3: 8B C3              mov         eax,ebx
  00000000653010F5: 4C 8D 9C 24 A0 00  lea         r11,[rsp+00000000000000A0h]
                    00 00
  00000000653010FD: 49 8B 5B 28        mov         rbx,qword ptr [r11+28h]
  0000000065301101: 49 8B 6B 30        mov         rbp,qword ptr [r11+30h]
  0000000065301105: 49 8B E3           mov         rsp,r11
  0000000065301108: 41 5C              pop         r12
  000000006530110A: 5F                 pop         rdi
  000000006530110B: 5E                 pop         rsi
  000000006530110C: C3                 ret

  Summary

      695000 .text
