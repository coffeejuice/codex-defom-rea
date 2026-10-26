Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140050BC0: mov         qword ptr [rsp+8],rcx
  0000000140050BC5: sub         rsp,38h
  0000000140050BC9: mov         rcx,qword ptr [rsp+40h]
  0000000140050BCE: call        0000000140055150
  0000000140050BD3: mov         qword ptr [rsp+28h],rax
  0000000140050BD8: cmp         qword ptr [rsp+28h],0
  0000000140050BDE: jne         0000000140050BE4
  0000000140050BE0: xor         al,al
  0000000140050BE2: jmp         0000000140050C12
  0000000140050BE4: mov         byte ptr [rsp+20h],0
  0000000140050BE9: mov         rax,qword ptr [rsp+40h]
  0000000140050BEE: mov         edx,dword ptr [rax+0000000000000108h]
  0000000140050BF4: mov         rcx,qword ptr [rsp+28h]
  0000000140050BF9: call        0000000140086720
  0000000140050BFE: mov         dword ptr [rsp+24h],eax
  0000000140050C02: cmp         dword ptr [rsp+24h],0FFFFFFFFh
  0000000140050C07: jne         0000000140050C0E
  0000000140050C09: mov         byte ptr [rsp+20h],1
  0000000140050C0E: mov         al,byte ptr [rsp+20h]
  0000000140050C12: 48
  0000000140050C13: 83

  Summary

     1E02000 .text
