Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C1F740: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  0000000140C1F745: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140C1F74A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140C1F74E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C1F753: 83 7C 24 10 FF     cmp         dword ptr [rsp+10h],0FFFFFFFFh
  0000000140C1F758: 7E 14              jle         0000000140C1F76E
  0000000140C1F75A: 66 0F 6E 44 24 10  movd        xmm0,dword ptr [rsp+10h]
  0000000140C1F760: F3 0F E6 C0        cvtdq2pd    xmm0,xmm0
  0000000140C1F764: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140C1F769: F2 0F 11 40 10     movsd       mmword ptr [rax+10h],xmm0
  0000000140C1F76E: 83 7C 24 18 FF     cmp         dword ptr [rsp+18h],0FFFFFFFFh
  0000000140C1F773: 7E 14              jle         0000000140C1F789
  0000000140C1F775: 66 0F 6E 44 24 18  movd        xmm0,dword ptr [rsp+18h]
  0000000140C1F77B: F3 0F E6 C0        cvtdq2pd    xmm0,xmm0
  0000000140C1F77F: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140C1F784: F2 0F 11 40 20     movsd       mmword ptr [rax+20h],xmm0
  0000000140C1F789: 83 7C 24 20 FF     cmp         dword ptr [rsp+20h],0FFFFFFFFh
  0000000140C1F78E: 7E 14              jle         0000000140C1F7A4
  0000000140C1F790: 66 0F 6E 44 24 20  movd        xmm0,dword ptr [rsp+20h]
  0000000140C1F796: F3 0F E6 C0        cvtdq2pd    xmm0,xmm0
  0000000140C1F79A: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140C1F79F: F2 0F 11 40 18     movsd       mmword ptr [rax+18h],xmm0
  0000000140C1F7A4: 83 7C 24 28 FF     cmp         dword ptr [rsp+28h],0FFFFFFFFh
  0000000140C1F7A9: 7E 14              jle         0000000140C1F7BF
  0000000140C1F7AB: 66 0F 6E 44 24 28  movd        xmm0,dword ptr [rsp+28h]
  0000000140C1F7B1: F3 0F E6 C0        cvtdq2pd    xmm0,xmm0
  0000000140C1F7B5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140C1F7BA: F2 0F 11 40 28     movsd       mmword ptr [rax+28h],xmm0
  0000000140C1F7BF: F3 C3              rep ret
  0000000140C1F7C1: CC                 int         3
  0000000140C1F7C2: CC                 int         3
  0000000140C1F7C3: CC                 int         3
  0000000140C1F7C4: CC                 int         3

  Summary

     1E02000 .text
