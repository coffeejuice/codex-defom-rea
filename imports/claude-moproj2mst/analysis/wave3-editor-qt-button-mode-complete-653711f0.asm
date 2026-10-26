Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653711F0: 4C 8B 49 08        mov         r9,qword ptr [rcx+8]
  00000000653711F4: 0F B6 CA           movzx       ecx,dl
  00000000653711F7: 45 8B 81 04 02 00  mov         r8d,dword ptr [r9+0000000000000204h]
                    00
  00000000653711FE: 41 8B C0           mov         eax,r8d
  0000000065371201: 83 E0 01           and         eax,1
  0000000065371204: 3B C1              cmp         eax,ecx
  0000000065371206: 74 11              je          0000000065371219
  0000000065371208: 83 E1 01           and         ecx,1
  000000006537120B: 41 83 E0 FC        and         r8d,0FFFFFFFCh
  000000006537120F: 41 0B C8           or          ecx,r8d
  0000000065371212: 41 89 89 04 02 00  mov         dword ptr [r9+0000000000000204h],ecx
                    00
  0000000065371219: F3 C3              rep ret
  000000006537121B: CC                 int         3
  000000006537121C: CC                 int         3
  000000006537121D: CC                 int         3
  000000006537121E: CC                 int         3
  000000006537121F: CC                 int         3

  Summary

      695000 .text
