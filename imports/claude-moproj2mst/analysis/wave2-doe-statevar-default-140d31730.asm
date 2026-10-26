Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D31730: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D31735: 48 83 EC 18        sub         rsp,18h
  0000000140D31739: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D3173E: 8B 05 08 4F 04 03  mov         eax,dword ptr [0000000143D7664Ch]
  0000000140D31744: 89 81 30 05 00 00  mov         dword ptr [rcx+0000000000000530h],eax
  0000000140D3174A: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  0000000140D31751: EB 09              jmp         0000000140D3175C
  0000000140D31753: 8B 04 24           mov         eax,dword ptr [rsp]
  0000000140D31756: 83 C0 01           add         eax,1
  0000000140D31759: 89 04 24           mov         dword ptr [rsp],eax
  0000000140D3175C: 83 3C 24 0C        cmp         dword ptr [rsp],0Ch
  0000000140D31760: 7D 25              jge         0000000140D31787
  0000000140D31762: 4C 63 04 24        movsxd      r8,dword ptr [rsp]
  0000000140D31766: 48 8D 15 E3 4E 04  lea         rdx,[0000000143D76650h]
                    03
  0000000140D3176D: 48 63 0C 24        movsxd      rcx,dword ptr [rsp]
  0000000140D31771: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D31776: F3 42 0F 10 04 82  movss       xmm0,dword ptr [rdx+r8*4]
  0000000140D3177C: F3 0F 11 84 88 34  movss       dword ptr [rax+rcx*4+0000000000000534h],xmm0
                    05 00 00
  0000000140D31785: EB CC              jmp         0000000140D31753
  0000000140D31787: 48 83 C4 18        add         rsp,18h
  0000000140D3178B: C3                 ret
  0000000140D3178C: CC                 int         3

  Summary

     1E02000 .text
