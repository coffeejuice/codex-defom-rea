Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_PRE_64.exe

File Type: EXECUTABLE IMAGE

  0000000140183840: mov         dword ptr [rsp+18h],r8d
  0000000140183845: mov         dword ptr [rsp+10h],edx
  0000000140183849: mov         qword ptr [rsp+8],rcx
  000000014018384E: sub         rsp,38h
  0000000140183852: mov         rax,qword ptr [rsp+40h]
  0000000140183857: cmp         dword ptr [rax+00000000000000E8h],0
  000000014018385E: jg          0000000140183865
  0000000140183860: jmp         00000001401838ED
  0000000140183865: cmp         dword ptr [rsp+50h],0
  000000014018386A: jge         000000014018386E
  000000014018386C: jmp         00000001401838ED
  000000014018386E: mov         rax,qword ptr [rsp+40h]
  0000000140183873: cmp         qword ptr [rax+00000000000000F0h],0
  000000014018387B: jne         000000014018387F
  000000014018387D: jmp         00000001401838ED
  000000014018387F: mov         rax,qword ptr [rsp+40h]
  0000000140183884: mov         rax,qword ptr [rax+00000000000000F0h]
  000000014018388B: add         rax,15F8h
  0000000140183891: mov         qword ptr [rsp+20h],rax
  0000000140183896: mov         r9d,dword ptr [rsp+48h]
  000000014018389B: mov         r8,qword ptr [rsp+20h]
  00000001401838A0: mov         rax,qword ptr [rsp+40h]
  00000001401838A5: mov         edx,dword ptr [rax+00000000000000E8h]
  00000001401838AB: mov         rcx,qword ptr [rsp+40h]
  00000001401838B0: call        0000000140329E60
  00000001401838B5: cmp         dword ptr [rsp+48h],36h
  00000001401838BA: jne         00000001401838D1
  00000001401838BC: mov         rax,qword ptr [rsp+20h]
  00000001401838C1: mov         edx,dword ptr [rax+00000000000000D8h]
  00000001401838C7: mov         rcx,qword ptr [rsp+40h]
  00000001401838CC: call        000000014019A410
  00000001401838D1: cmp         dword ptr [rsp+48h],37h
  00000001401838D6: jne         00000001401838ED
  00000001401838D8: mov         rax,qword ptr [rsp+20h]
  00000001401838DD: mov         edx,dword ptr [rax+00000000000000DCh]
  00000001401838E3: mov         rcx,qword ptr [rsp+40h]
  00000001401838E8: call        000000014019A430
  00000001401838ED: add         rsp,38h
  00000001401838F1: ret
  00000001401838F2: int         3

  Summary

      504000 .text
