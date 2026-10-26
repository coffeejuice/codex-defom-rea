Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408DFC30: mov         qword ptr [rsp+8],rcx
  00000001408DFC35: sub         rsp,18h
  00000001408DFC39: mov         rax,qword ptr [rsp+20h]
  00000001408DFC3E: cmp         dword ptr [rax+0000000000000650h],0
  00000001408DFC45: jne         00000001408DFC5B
  00000001408DFC47: mov         rax,qword ptr [rsp+20h]
  00000001408DFC4C: movsd       xmm0,mmword ptr [rax+0000000000000640h]
  00000001408DFC54: movsd       mmword ptr [rsp],xmm0
  00000001408DFC59: jmp         00000001408DFC6D
  00000001408DFC5B: mov         rax,qword ptr [rsp+20h]
  00000001408DFC60: movsd       xmm0,mmword ptr [rax+0000000000000648h]
  00000001408DFC68: movsd       mmword ptr [rsp],xmm0
  00000001408DFC6D: movsd       xmm0,mmword ptr [rsp]
  00000001408DFC72: add         rsp,18h
  00000001408DFC76: ret
  00000001408DFC77: int         3

  Summary

     1E02000 .text
