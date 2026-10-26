Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006502F160: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006502F165: 57                 push        rdi
  000000006502F166: 48 83 EC 20        sub         rsp,20h
  000000006502F16A: 80 3D FF A6 99 00  cmp         byte ptr [00000000659C9870h],0
                    00
  000000006502F171: 48 8B FA           mov         rdi,rdx
  000000006502F174: 48 8B D9           mov         rbx,rcx
  000000006502F177: 75 05              jne         000000006502F17E
  000000006502F179: E8 22 FD FF FF     call        000000006502EEA0
  000000006502F17E: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006502F181: 48 89 0B           mov         qword ptr [rbx],rcx
  000000006502F184: FF 15 36 77 66 00  call        qword ptr [00000000656968C0h]
  000000006502F18A: 48 8B C3           mov         rax,rbx
  000000006502F18D: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006502F192: 48 83 C4 20        add         rsp,20h
  000000006502F196: 5F                 pop         rdi
  000000006502F197: C3                 ret

  Summary

      695000 .text
