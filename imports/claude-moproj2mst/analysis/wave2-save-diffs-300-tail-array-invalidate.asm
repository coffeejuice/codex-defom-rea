Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014082AAA0: mov         dword ptr [rsp+10h],edx
  000000014082AAA4: mov         qword ptr [rsp+8],rcx
  000000014082AAA9: sub         rsp,38h
  000000014082AAAD: mov         eax,dword ptr [rsp+48h]
  000000014082AAB1: mov         dword ptr [rsp+20h],eax
  000000014082AAB5: cmp         dword ptr [rsp+20h],22h
  000000014082AABA: je          000000014082AAE6
  000000014082AABC: cmp         dword ptr [rsp+20h],23h
  000000014082AAC1: je          000000014082AAFE
  000000014082AAC3: cmp         dword ptr [rsp+20h],36h
  000000014082AAC8: je          000000014082AADA
  000000014082AACA: cmp         dword ptr [rsp+20h],37h
  000000014082AACF: je          000000014082AB0A
  000000014082AAD1: cmp         dword ptr [rsp+20h],3Fh
  000000014082AAD6: je          000000014082AAF2
  000000014082AAD8: jmp         000000014082AB14
  000000014082AADA: mov         rcx,qword ptr [rsp+40h]
  000000014082AADF: call        000000014082AB20
  000000014082AAE4: jmp         000000014082AB14
  000000014082AAE6: mov         rcx,qword ptr [rsp+40h]
  000000014082AAEB: call        000000014082ACC0
  000000014082AAF0: jmp         000000014082AB14
  000000014082AAF2: mov         rcx,qword ptr [rsp+40h]
  000000014082AAF7: call        000000014082AD80
  000000014082AAFC: jmp         000000014082AB14
  000000014082AAFE: mov         rcx,qword ptr [rsp+40h]
  000000014082AB03: call        000000014082AE40
  000000014082AB08: jmp         000000014082AB14
  000000014082AB0A: mov         rcx,qword ptr [rsp+40h]
  000000014082AB0F: call        000000014082AF00
  000000014082AB14: add         rsp,38h
  000000014082AB18: ret
  000000014082AB19: int         3

  Summary

     1E02000 .text
