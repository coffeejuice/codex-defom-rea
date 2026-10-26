Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140857A60: mov         dword ptr [rsp+18h],r8d
  0000000140857A65: mov         dword ptr [rsp+10h],edx
  0000000140857A69: mov         qword ptr [rsp+8],rcx
  0000000140857A6E: sub         rsp,38h
  0000000140857A72: mov         rax,qword ptr [rsp+40h]
  0000000140857A77: cmp         dword ptr [rax+00000000000000E8h],0
  0000000140857A7E: jg          0000000140857A85
  0000000140857A80: jmp         0000000140857B0D
  0000000140857A85: cmp         dword ptr [rsp+50h],0
  0000000140857A8A: jge         0000000140857A8E
  0000000140857A8C: jmp         0000000140857B0D
  0000000140857A8E: mov         rax,qword ptr [rsp+40h]
  0000000140857A93: cmp         qword ptr [rax+00000000000000F0h],0
  0000000140857A9B: jne         0000000140857A9F
  0000000140857A9D: jmp         0000000140857B0D
  0000000140857A9F: mov         rax,qword ptr [rsp+40h]
  0000000140857AA4: mov         rax,qword ptr [rax+00000000000000F0h]
  0000000140857AAB: add         rax,15F8h
  0000000140857AB1: mov         qword ptr [rsp+20h],rax
  0000000140857AB6: mov         r9d,dword ptr [rsp+48h]
  0000000140857ABB: mov         r8,qword ptr [rsp+20h]
  0000000140857AC0: mov         rax,qword ptr [rsp+40h]
  0000000140857AC5: mov         edx,dword ptr [rax+00000000000000E8h]
  0000000140857ACB: mov         rcx,qword ptr [rsp+40h]
  0000000140857AD0: call        0000000140B3E8E0
  0000000140857AD5: cmp         dword ptr [rsp+48h],36h
  0000000140857ADA: jne         0000000140857AF1
  0000000140857ADC: mov         rax,qword ptr [rsp+20h]
  0000000140857AE1: mov         edx,dword ptr [rax+00000000000000D8h]
  0000000140857AE7: mov         rcx,qword ptr [rsp+40h]
  0000000140857AEC: call        000000014086E190
  0000000140857AF1: cmp         dword ptr [rsp+48h],37h
  0000000140857AF6: jne         0000000140857B0D
  0000000140857AF8: mov         rax,qword ptr [rsp+20h]
  0000000140857AFD: mov         edx,dword ptr [rax+00000000000000DCh]
  0000000140857B03: mov         rcx,qword ptr [rsp+40h]
  0000000140857B08: call        0000000140C1AD30
  0000000140857B0D: add         rsp,38h
  0000000140857B11: ret
  0000000140857B12: int         3

  Summary

     1E02000 .text
