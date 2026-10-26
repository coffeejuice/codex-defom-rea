Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653713E0: 4C 8B 41 08        mov         r8,qword ptr [rcx+8]
  00000000653713E4: 0F B6 C2           movzx       eax,dl
  00000000653713E7: 41 83 A0 04 02 00  and         dword ptr [r8+0000000000000204h],0FFFFFFF7h
                    00 F7
  00000000653713EF: 83 E0 01           and         eax,1
  00000000653713F2: C1 E0 03           shl         eax,3
  00000000653713F5: 41 09 80 04 02 00  or          dword ptr [r8+0000000000000204h],eax
                    00
  00000000653713FC: C3                 ret
  00000000653713FD: CC                 int         3
  00000000653713FE: CC                 int         3
  00000000653713FF: CC                 int         3
  0000000065371400: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  0000000065371404: 8B
  0000000065371405: 80
  0000000065371406: 04 02              add         al,2

  Summary

      695000 .text
