Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400AD070: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400AD075: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400AD07A: 48 83 EC 38        sub         rsp,38h
  00000001400AD07E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400AD083: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001400AD088: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400AD08B: 48 39 01           cmp         qword ptr [rcx],rax
  00000001400AD08E: 74 68              je          00000001400AD0F8
  00000001400AD090: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001400AD095: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400AD098: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400AD09D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400AD0A2: FF 15 50 72 D5 01  call        qword ptr [0000000141E042F8h]
  00000001400AD0A8: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400AD0AD: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001400AD0B0: FF 15 32 72 D5 01  call        qword ptr [0000000141E042E8h]
  00000001400AD0B6: 0F B6 C0           movzx       eax,al
  00000001400AD0B9: 85 C0              test        eax,eax
  00000001400AD0BB: 75 12              jne         00000001400AD0CF
  00000001400AD0BD: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  00000001400AD0C2: 48 8B 12           mov         rdx,qword ptr [rdx]
  00000001400AD0C5: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400AD0CA: E8 81 85 F7 FF     call        0000000140025650
  00000001400AD0CF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400AD0D4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400AD0D9: 48 89 01           mov         qword ptr [rcx],rax
  00000001400AD0DC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400AD0E1: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400AD0E4: 8B 40 10           mov         eax,dword ptr [rax+10h]
  00000001400AD0E7: 83 E0 01           and         eax,1
  00000001400AD0EA: 85 C0              test        eax,eax
  00000001400AD0EC: 75 0A              jne         00000001400AD0F8
  00000001400AD0EE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400AD0F3: E8 B8 23 C1 00     call        0000000140CBF4B0
  00000001400AD0F8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400AD0FD: 48 83 C4 38        add         rsp,38h
  00000001400AD101: C3                 ret
  00000001400AD102: CC                 int         3

  Summary

     1E02000 .text
