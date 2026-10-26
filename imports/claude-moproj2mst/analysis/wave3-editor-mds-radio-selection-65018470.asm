Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065018470: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  0000000065018474: 8A 48 08           mov         cl,byte ptr [rax+8]
  0000000065018477: F6 D1              not         cl
  0000000065018479: 83 E1 01           and         ecx,1
  000000006501847C: 8A C1              mov         al,cl
  000000006501847E: C3                 ret
  000000006501847F: CC                 int         3
  0000000065018480: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  0000000065018484: 8B 40 08           mov         eax,dword ptr [rax+8]
  0000000065018487: C1 E8 02           shr         eax,2
  000000006501848A: 24 01              and         al,1
  000000006501848C: C3                 ret
  000000006501848D: CC                 int         3
  000000006501848E: CC                 int         3
  000000006501848F: CC                 int         3

  Summary

      695000 .text
