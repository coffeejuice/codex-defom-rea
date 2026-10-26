Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505C560: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006505C564: 48 8B 48 28        mov         rcx,qword ptr [rax+28h]
  000000006505C568: 48 8D 05 E1 11 96  lea         rax,[00000000659BD750h]
                    00
  000000006505C56F: 48 85 C9           test        rcx,rcx
  000000006505C572: 48 0F 45 C1        cmovne      rax,rcx
  000000006505C576: C3                 ret
  000000006505C577: CC                 int         3

  Summary

      695000 .text
