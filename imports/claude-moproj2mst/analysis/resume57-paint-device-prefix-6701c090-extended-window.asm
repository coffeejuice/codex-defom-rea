Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006701C090: 48 8B 05 A1 7B 2D  mov         rax,qword ptr [00000000672F3C38h]
                    00
  000000006701C097: 48 89 0D 9A 7B 2D  mov         qword ptr [00000000672F3C38h],rcx
                    00
  000000006701C09E: 48 85 C9           test        rcx,rcx
  000000006701C0A1: 75 18              jne         000000006701C0BB
  000000006701C0A3: 38 0D 4B 83 2D 00  cmp         byte ptr [00000000672F43F4h],cl
  000000006701C0A9: 48 8D 15 30 8E 12  lea         rdx,[0000000067144EE0h]
                    00
  000000006701C0B0: 48 0F 45 CA        cmovne      rcx,rdx
  000000006701C0B4: 48 89 0D 7D 7B 2D  mov         qword ptr [00000000672F3C38h],rcx
                    00
  000000006701C0BB: F3 C3              rep ret
  000000006701C0BD: CC                 int         3
  000000006701C0BE: CC                 int         3
  000000006701C0BF: CC                 int         3

  Summary

      1A7000 .text
