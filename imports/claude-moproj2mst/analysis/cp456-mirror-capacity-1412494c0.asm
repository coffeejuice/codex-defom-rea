Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412494C0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001412494C5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001412494CA: 48 83 EC 28        sub         rsp,28h
  00000001412494CE: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001412494D3: 83 38 00           cmp         dword ptr [rax],0
  00000001412494D6: 7E 37              jle         000000014124950F
  00000001412494D8: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001412494DD: E8 3E FF FF FF     call        0000000141249420
  00000001412494E2: 44 8B D8           mov         r11d,eax
  00000001412494E5: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001412494EA: 44 39 18           cmp         dword ptr [rax],r11d
  00000001412494ED: 7F 20              jg          000000014124950F
  00000001412494EF: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001412494F4: 8B 00              mov         eax,dword ptr [rax]
  00000001412494F6: 83 E8 01           sub         eax,1
  00000001412494F9: 48 63 C8           movsxd      rcx,eax
  00000001412494FC: 48 69 C9 08 01 00  imul        rcx,rcx,108h
                    00
  0000000141249503: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000141249508: 48 03 C1           add         rax,rcx
  000000014124950B: EB 04              jmp         0000000141249511
  000000014124950D: EB 02              jmp         0000000141249511
  000000014124950F: 33 C0              xor         eax,eax
  0000000141249511: 48 83 C4 28        add         rsp,28h
  0000000141249515: C3                 ret

  Summary

     1E02000 .text
