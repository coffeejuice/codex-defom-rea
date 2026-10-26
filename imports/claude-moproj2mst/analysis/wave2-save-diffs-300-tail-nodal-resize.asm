Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140854530: mov         dword ptr [rsp+18h],r8d
  0000000140854535: mov         dword ptr [rsp+10h],edx
  0000000140854539: mov         qword ptr [rsp+8],rcx
  000000014085453E: sub         rsp,38h
  0000000140854542: mov         rax,qword ptr [rsp+40h]
  0000000140854547: cmp         dword ptr [rax+000000000000012Ch],0
  000000014085454E: jg          0000000140854552
  0000000140854550: jmp         00000001408545A2
  0000000140854552: cmp         dword ptr [rsp+50h],0
  0000000140854557: jge         000000014085455B
  0000000140854559: jmp         00000001408545A2
  000000014085455B: mov         rax,qword ptr [rsp+40h]
  0000000140854560: cmp         qword ptr [rax+0000000000000130h],0
  0000000140854568: jne         000000014085456C
  000000014085456A: jmp         00000001408545A2
  000000014085456C: mov         rax,qword ptr [rsp+40h]
  0000000140854571: mov         rax,qword ptr [rax+0000000000000130h]
  0000000140854578: add         rax,15F8h
  000000014085457E: mov         qword ptr [rsp+20h],rax
  0000000140854583: mov         r9d,dword ptr [rsp+48h]
  0000000140854588: mov         r8,qword ptr [rsp+20h]
  000000014085458D: mov         rax,qword ptr [rsp+40h]
  0000000140854592: mov         edx,dword ptr [rax+000000000000012Ch]
  0000000140854598: mov         rcx,qword ptr [rsp+40h]
  000000014085459D: call        0000000140B3E8E0
  00000001408545A2: add         rsp,38h
  00000001408545A6: ret
  00000001408545A7: int         3

  Summary

     1E02000 .text
