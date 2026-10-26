Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140892370: mov         qword ptr [rsp+8],rcx
  0000000140892375: sub         rsp,28h
  0000000140892379: mov         rax,qword ptr [rsp+30h]
  000000014089237E: cmp         dword ptr [rax+68h],6
  0000000140892382: je          0000000140892388
  0000000140892384: xor         al,al
  0000000140892386: jmp         00000001408923A6
  0000000140892388: mov         rcx,qword ptr [rsp+30h]
  000000014089238D: add         rcx,448h
  0000000140892394: call        000000014089BAF0
  0000000140892399: movzx       eax,al
  000000014089239C: test        eax,eax
  000000014089239E: je          00000001408923A4
  00000001408923A0: xor         al,al
  00000001408923A2: jmp         00000001408923A6
  00000001408923A4: mov         al,1
  00000001408923A6: add         rsp,28h
  00000001408923AA: ret
  00000001408923AB: int         3

  Summary

     1E02000 .text
