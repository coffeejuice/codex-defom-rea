Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140452660: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140452665: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014045266A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014045266F: 53                 push        rbx
  0000000140452670: 48 83 EC 20        sub         rsp,20h
  0000000140452674: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140452679: E8 A2 0F C7 FF     call        00000001400C3620
  000000014045267E: 48 39 44 24 38     cmp         qword ptr [rsp+38h],rax
  0000000140452683: 73 32              jae         00000001404526B7
  0000000140452685: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014045268A: E8 01 E7 DF 00     call        0000000141250D90
  000000014045268F: 48 8B D8           mov         rbx,rax
  0000000140452692: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140452697: E8 E4 0F C7 FF     call        00000001400C3680
  000000014045269C: 4C 8B D8           mov         r11,rax
  000000014045269F: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001404526A4: 49 8D 14 83        lea         rdx,[r11+rax*4]
  00000001404526A8: 4C 8B C3           mov         r8,rbx
  00000001404526AB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001404526B0: E8 3B 00 00 00     call        00000001404526F0
  00000001404526B5: EB 31              jmp         00000001404526E8
  00000001404526B7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001404526BC: E8 5F 0F C7 FF     call        00000001400C3620
  00000001404526C1: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000001404526C6: 48 2B D8           sub         rbx,rax
  00000001404526C9: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001404526CE: E8 BD E6 DF 00     call        0000000141250D90
  00000001404526D3: 4C 8B 4C 24 40     mov         r9,qword ptr [rsp+40h]
  00000001404526D8: 4C 8B C3           mov         r8,rbx
  00000001404526DB: 48 8B D0           mov         rdx,rax
  00000001404526DE: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001404526E3: E8 88 00 00 00     call        0000000140452770
  00000001404526E8: 48 83 C4 20        add         rsp,20h
  00000001404526EC: 5B                 pop         rbx
  00000001404526ED: C3                 ret

  Summary

     1E02000 .text
