Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006500F8F0: 48 83 EC 28        sub         rsp,28h
  000000006500F8F4: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006500F8F8: E8 63 D2 04 00     call        000000006505CB60
  000000006500F8FD: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006500F901: 48 8B 81 88 00 00  mov         rax,qword ptr [rcx+0000000000000088h]
                    00
  000000006500F908: 48 85 C0           test        rax,rax
  000000006500F90B: 74 12              je          000000006500F91F
  000000006500F90D: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006500F911: 48 85 C0           test        rax,rax
  000000006500F914: 74 09              je          000000006500F91F
  000000006500F916: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  000000006500F91A: 48 83 C4 28        add         rsp,28h
  000000006500F91E: C3                 ret
  000000006500F91F: 33 C0              xor         eax,eax
  000000006500F921: 48 83 C4 28        add         rsp,28h
  000000006500F925: C3                 ret

  Summary

      695000 .text
