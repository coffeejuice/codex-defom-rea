Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C4F20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670C4F25: 57                 push        rdi
  00000000670C4F26: 48 83 EC 30        sub         rsp,30h
  00000000670C4F2A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C4F33: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670C4F38: 48 8B D9           mov         rbx,rcx
  00000000670C4F3B: 48 8D 05 9E 77 16  lea         rax,[000000006722C6E0h]
                    00
  00000000670C4F42: 48 89 01           mov         qword ptr [rcx],rax
  00000000670C4F45: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000670C4F49: 48 8B 81 90 00 00  mov         rax,qword ptr [rcx+0000000000000090h]
                    00
  00000000670C4F50: 83 78 08 00        cmp         dword ptr [rax+8],0
  00000000670C4F54: 74 06              je          00000000670C4F5C
  00000000670C4F56: E8 F5 DF FF FF     call        00000000670C2F50
  00000000670C4F5B: 90                 nop
  00000000670C4F5C: 48 8B 7B 08        mov         rdi,qword ptr [rbx+8]
  00000000670C4F60: 48 85 FF           test        rdi,rdi
  00000000670C4F63: 74 1A              je          00000000670C4F7F
  00000000670C4F65: 48 8B CF           mov         rcx,rdi
  00000000670C4F68: E8 C3 D0 FF FF     call        00000000670C2030
  00000000670C4F6D: 48 8B CF           mov         rcx,rdi
  00000000670C4F70: E8 E1 39 0B 00     call        0000000067178956
  00000000670C4F75: 48 C7 43 08 00 00  mov         qword ptr [rbx+8],0
                    00 00
  00000000670C4F7D: EB 08              jmp         00000000670C4F87
  00000000670C4F7F: 48 C7 43 08 00 00  mov         qword ptr [rbx+8],0
                    00 00
  00000000670C4F87: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670C4F8C: 48 83 C4 30        add         rsp,30h
  00000000670C4F90: 5F                 pop         rdi
  00000000670C4F91: C3                 ret

  Summary

      1A7000 .text
