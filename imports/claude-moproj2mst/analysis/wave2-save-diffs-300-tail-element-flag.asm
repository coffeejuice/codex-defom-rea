Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014082AD80: mov         qword ptr [rsp+8],rcx
  000000014082AD85: sub         rsp,48h
  000000014082AD89: mov         qword ptr [rsp+28h],0
  000000014082AD92: mov         rax,qword ptr [rsp+50h]
  000000014082AD97: mov         eax,dword ptr [rax+000000000000123Ch]
  000000014082AD9D: mov         dword ptr [rsp+20h],eax
  000000014082ADA1: mov         rcx,qword ptr [rsp+50h]
  000000014082ADA6: mov         eax,dword ptr [rsp+20h]
  000000014082ADAA: cmp         dword ptr [rcx+00000000000016F4h],eax
  000000014082ADB0: jne         000000014082ADB4
  000000014082ADB2: jmp         000000014082AE31
  000000014082ADB4: mov         rcx,qword ptr [rsp+50h]
  000000014082ADB9: mov         eax,dword ptr [rsp+20h]
  000000014082ADBD: mov         dword ptr [rcx+00000000000016F4h],eax
  000000014082ADC3: mov         dword ptr [rsp+30h],0
  000000014082ADCB: jmp         000000014082ADD8
  000000014082ADCD: mov         eax,dword ptr [rsp+30h]
  000000014082ADD1: add         eax,1
  000000014082ADD4: mov         dword ptr [rsp+30h],eax
  000000014082ADD8: mov         rcx,qword ptr [rsp+50h]
  000000014082ADDD: add         rcx,1380h
  000000014082ADE4: call        00000001407D3070
  000000014082ADE9: cmp         dword ptr [rsp+30h],eax
  000000014082ADED: jge         000000014082AE31
  000000014082ADEF: mov         edx,dword ptr [rsp+30h]
  000000014082ADF3: add         edx,1
  000000014082ADF6: mov         rcx,qword ptr [rsp+50h]
  000000014082ADFB: add         rcx,1380h
  000000014082AE02: call        000000014084ECA0
  000000014082AE07: mov         qword ptr [rsp+28h],rax
  000000014082AE0C: cmp         qword ptr [rsp+28h],0
  000000014082AE12: je          000000014082AE2F
  000000014082AE14: mov         rcx,qword ptr [rsp+28h]
  000000014082AE19: add         rcx,9E0h
  000000014082AE20: mov         r8d,dword ptr [rsp+20h]
  000000014082AE25: mov         edx,3Fh
  000000014082AE2A: call        0000000140857A60
  000000014082AE2F: jmp         000000014082ADCD
  000000014082AE31: add         rsp,48h
  000000014082AE35: ret
  000000014082AE36: int         3

  Summary

     1E02000 .text
