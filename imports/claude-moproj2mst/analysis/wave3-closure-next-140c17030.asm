Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C17030: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140C17034: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C17039: 48 83 EC 28        sub         rsp,28h
  0000000140C1703D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C17042: E8 99 E4 FF FF     call        0000000140C154E0
  0000000140C17047: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140C1704B: 83 E0 01           and         eax,1
  0000000140C1704E: 85 C0              test        eax,eax
  0000000140C17050: 74 0A              je          0000000140C1705C
  0000000140C17052: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C17057: E8 0C 8C 8C 00     call        00000001414DFC68
  0000000140C1705C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C17061: 48 83 C4 28        add         rsp,28h
  0000000140C17065: C3                 ret

  Summary

     1E02000 .text
