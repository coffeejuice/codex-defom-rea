Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C4B00: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670C4B05: 55                 push        rbp
  00000000670C4B06: 56                 push        rsi
  00000000670C4B07: 57                 push        rdi
  00000000670C4B08: 48 83 EC 40        sub         rsp,40h
  00000000670C4B0C: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C4B15: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  00000000670C4B1A: 48 8B EA           mov         rbp,rdx
  00000000670C4B1D: 48 8B D9           mov         rbx,rcx
  00000000670C4B20: 48 8D 05 B9 7B 16  lea         rax,[000000006722C6E0h]
                    00
  00000000670C4B27: 48 89 01           mov         qword ptr [rcx],rax
  00000000670C4B2A: B9 E0 00 00 00     mov         ecx,0E0h
  00000000670C4B2F: E8 52 3F 0B 00     call        0000000067178A86
  00000000670C4B34: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000000670C4B39: 48 85 C0           test        rax,rax
  00000000670C4B3C: 74 0D              je          00000000670C4B4B
  00000000670C4B3E: 48 8B D3           mov         rdx,rbx
  00000000670C4B41: 48 8B C8           mov         rcx,rax
  00000000670C4B44: E8 17 E0 FF FF     call        00000000670C2B60
  00000000670C4B49: EB 02              jmp         00000000670C4B4D
  00000000670C4B4B: 33 C0              xor         eax,eax
  00000000670C4B4D: 48 89 43 08        mov         qword ptr [rbx+8],rax
  00000000670C4B51: 48 8B F0           mov         rsi,rax
  00000000670C4B54: 48 89 28           mov         qword ptr [rax],rbp
  00000000670C4B57: 45 33 C9           xor         r9d,r9d
  00000000670C4B5A: 45 33 C0           xor         r8d,r8d
  00000000670C4B5D: 33 D2              xor         edx,edx
  00000000670C4B5F: 48 8D 48 08        lea         rcx,[rax+8]
  00000000670C4B63: E8 58 B2 06 00     call        000000006712FDC0
  00000000670C4B68: 48 85 ED           test        rbp,rbp
  00000000670C4B6B: 74 22              je          00000000670C4B8F
  00000000670C4B6D: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  00000000670C4B75: 4C 8D 0D 9C 77 16  lea         r9,[000000006722C318h]
                    00
  00000000670C4B7C: 4C 8D 46 08        lea         r8,[rsi+8]
  00000000670C4B80: 48 8D 15 81 77 16  lea         rdx,[000000006722C308h]
                    00
  00000000670C4B87: 48 8B CD           mov         rcx,rbp
  00000000670C4B8A: E8 31 CA 06 00     call        00000000671315C0
  00000000670C4B8F: 48 89 5E 18        mov         qword ptr [rsi+18h],rbx
  00000000670C4B93: C7 86 CC 00 00 00  mov         dword ptr [rsi+00000000000000CCh],0
                    00 00 00 00
  00000000670C4B9D: 48 8B C3           mov         rax,rbx
  00000000670C4BA0: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  00000000670C4BA5: 48 83 C4 40        add         rsp,40h
  00000000670C4BA9: 5F                 pop         rdi
  00000000670C4BAA: 5E                 pop         rsi
  00000000670C4BAB: 5D                 pop         rbp
  00000000670C4BAC: C3                 ret
  00000000670C4BAD: CC                 int         3
  00000000670C4BAE: CC                 int         3
  00000000670C4BAF: CC                 int         3
  00000000670C4BB0: 40 57              push        rdi
  00000000670C4BB2: 48 83 EC 30        sub         rsp,30h
  00000000670C4BB6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C4BBF: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670C4BC4: 48 89 74 24 50     mov         qword ptr [rsp+50h],rsi
  00000000670C4BC9: 49 8B F8           mov         rdi,r8
  00000000670C4BCC: 48 8B F2           mov         rsi,rdx
  00000000670C4BCF: 48 8B D9           mov         rbx,rcx
  00000000670C4BD2: 48 8D 05 07 7B 16  lea         rax,[000000006722C6E0h]
                    00
  00000000670C4BD9: 48 89 01           mov         qword ptr [rcx],rax
  00000000670C4BDC: B9 E0 00 00 00     mov         ecx,0E0h
  00000000670C4BE1: E8 A0 3E 0B 00     call        0000000067178A86
  00000000670C4BE6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670C4BEB: 48 85 C0           test        rax,rax
  00000000670C4BEE: 74 10              je          00000000670C4C00
  00000000670C4BF0: 48 8B D3           mov         rdx,rbx
  00000000670C4BF3: 48 8B C8           mov         rcx,rax
  00000000670C4BF6: E8 65 DF FF FF     call        00000000670C2B60
  00000000670C4BFB: 4C 8B D8           mov         r11,rax
  00000000670C4BFE: EB 03              jmp         00000000670C4C03
  00000000670C4C00: 45 33 DB           xor         r11d,r11d
  00000000670C4C03: 4C 89 5B 08        mov         qword ptr [rbx+8],r11
  00000000670C4C07: 49 89 73 28        mov         qword ptr [r11+28h],rsi
  00000000670C4C0B: 8B 07              mov         eax,dword ptr [rdi]
  00000000670C4C0D: 41 89 43 34        mov         dword ptr [r11+34h],eax
  00000000670C4C11: 41 C7 83 CC 00 00  mov         dword ptr [r11+00000000000000CCh],0
                    00 00 00 00 00
  00000000670C4C1C: 48 8B C3           mov         rax,rbx
  00000000670C4C1F: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670C4C24: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  00000000670C4C29: 48 83 C4 30        add         rsp,30h
  00000000670C4C2D: 5F                 pop         rdi
  00000000670C4C2E: C3                 ret
  00000000670C4C2F: CC                 int         3
  00000000670C4C30: 48

  Summary

      1A7000 .text
