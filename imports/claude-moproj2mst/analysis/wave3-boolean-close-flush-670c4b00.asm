Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

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

  Summary

      1A7000 .text
