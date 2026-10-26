Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014082ACC0: mov         qword ptr [rsp+8],rcx
  000000014082ACC5: sub         rsp,48h
  000000014082ACC9: mov         qword ptr [rsp+28h],0
  000000014082ACD2: mov         rax,qword ptr [rsp+50h]
  000000014082ACD7: mov         eax,dword ptr [rax+00000000000011C4h]
  000000014082ACDD: mov         dword ptr [rsp+20h],eax
  000000014082ACE1: mov         rcx,qword ptr [rsp+50h]
  000000014082ACE6: mov         eax,dword ptr [rsp+20h]
  000000014082ACEA: cmp         dword ptr [rcx+0000000000001680h],eax
  000000014082ACF0: jne         000000014082ACF4
  000000014082ACF2: jmp         000000014082AD71
  000000014082ACF4: mov         rcx,qword ptr [rsp+50h]
  000000014082ACF9: mov         eax,dword ptr [rsp+20h]
  000000014082ACFD: mov         dword ptr [rcx+0000000000001680h],eax
  000000014082AD03: mov         dword ptr [rsp+30h],0
  000000014082AD0B: jmp         000000014082AD18
  000000014082AD0D: mov         eax,dword ptr [rsp+30h]
  000000014082AD11: add         eax,1
  000000014082AD14: mov         dword ptr [rsp+30h],eax
  000000014082AD18: mov         rcx,qword ptr [rsp+50h]
  000000014082AD1D: add         rcx,1380h
  000000014082AD24: call        00000001407D3070
  000000014082AD29: cmp         dword ptr [rsp+30h],eax
  000000014082AD2D: jge         000000014082AD71
  000000014082AD2F: mov         edx,dword ptr [rsp+30h]
  000000014082AD33: add         edx,1
  000000014082AD36: mov         rcx,qword ptr [rsp+50h]
  000000014082AD3B: add         rcx,1380h
  000000014082AD42: call        000000014084ECA0
  000000014082AD47: mov         qword ptr [rsp+28h],rax
  000000014082AD4C: cmp         qword ptr [rsp+28h],0
  000000014082AD52: je          000000014082AD6F
  000000014082AD54: mov         rcx,qword ptr [rsp+28h]
  000000014082AD59: add         rcx,8A0h
  000000014082AD60: mov         r8d,dword ptr [rsp+20h]
  000000014082AD65: mov         edx,22h
  000000014082AD6A: call        0000000140854530
  000000014082AD6F: jmp         000000014082AD0D
  000000014082AD71: add         rsp,48h
  000000014082AD75: ret
  000000014082AD76: int         3

  Summary

     1E02000 .text
