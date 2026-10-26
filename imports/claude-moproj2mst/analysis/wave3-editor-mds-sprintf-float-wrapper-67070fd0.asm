Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067070FD0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000067070FD5: 53                 push        rbx
  0000000067070FD6: 48 83 EC 30        sub         rsp,30h
  0000000067070FDA: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067070FE3: 48 8B D9           mov         rbx,rcx
  0000000067070FE6: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000067070FEE: 48 8D 05 3B FB 27  lea         rax,[00000000672F0B30h]
                    00
  0000000067070FF5: 48 89 01           mov         qword ptr [rcx],rax
  0000000067070FF8: F0 83 05 30 FB 27  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  0000000067071000: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000067071008: E8 23 FE FF FF     call        0000000067070E30
  000000006707100D: 48 8B C3           mov         rax,rbx
  0000000067071010: 48 83 C4 30        add         rsp,30h
  0000000067071014: 5B                 pop         rbx
  0000000067071015: C3                 ret
  0000000067071016: CC                 int         3

  Summary

      1A7000 .text
