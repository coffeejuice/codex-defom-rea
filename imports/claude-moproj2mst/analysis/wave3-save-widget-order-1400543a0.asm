Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400543A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400543A5: 48 83 EC 58        sub         rsp,58h
  00000001400543A9: 48 8D 0D 08 23 D2  lea         rcx,[0000000143D766B8h]
                    03
  00000001400543B0: E8 DB 3C 27 00     call        00000001402C8090
  00000001400543B5: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400543BA: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400543BF: E8 6C CE FF FF     call        0000000140051230
  00000001400543C4: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400543C9: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001400543CF: 74 10              je          00000001400543E1
  00000001400543D1: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001400543D6: 48 83 C0 10        add         rax,10h
  00000001400543DA: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400543DF: EB 09              jmp         00000001400543EA
  00000001400543E1: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001400543EA: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001400543EF: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400543F4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400543F9: E8 32 CE FF FF     call        0000000140051230
  00000001400543FE: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140054403: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140054408: 48 8B 00           mov         rax,qword ptr [rax]
  000000014005440B: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140054410: FF 50 60           call        qword ptr [rax+60h]
  0000000140054413: 48 83 C4 58        add         rsp,58h
  0000000140054417: C3                 ret

  Summary

     1E02000 .text
