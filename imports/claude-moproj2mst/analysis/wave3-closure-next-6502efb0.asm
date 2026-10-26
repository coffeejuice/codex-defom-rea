Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006502EFB0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  000000006502EFB5: 57                 push        rdi
  000000006502EFB6: 48 83 EC 20        sub         rsp,20h
  000000006502EFBA: 80 3D AF A8 99 00  cmp         byte ptr [00000000659C9870h],0
                    00
  000000006502EFC1: 48 63 DA           movsxd      rbx,edx
  000000006502EFC4: 48 8B F9           mov         rdi,rcx
  000000006502EFC7: 75 05              jne         000000006502EFCE
  000000006502EFC9: E8 D2 FE FF FF     call        000000006502EEA0
  000000006502EFCE: 83 FB 15           cmp         ebx,15h
  000000006502EFD1: 77 13              ja          000000006502EFE6
  000000006502EFD3: 48 8B C3           mov         rax,rbx
  000000006502EFD6: 48 8D 1D E3 A7 99  lea         rbx,[00000000659C97C0h]
                    00
  000000006502EFDD: 48 8B 1C C3        mov         rbx,qword ptr [rbx+rax*8]
  000000006502EFE1: 48 85 DB           test        rbx,rbx
  000000006502EFE4: 75 07              jne         000000006502EFED
  000000006502EFE6: 48 8B 1D D3 A7 99  mov         rbx,qword ptr [00000000659C97C0h]
                    00
  000000006502EFED: 48 8B CB           mov         rcx,rbx
  000000006502EFF0: FF 15 CA 78 66 00  call        qword ptr [00000000656968C0h]
  000000006502EFF6: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006502EFF9: 48 85 C9           test        rcx,rcx
  000000006502EFFC: 74 2C              je          000000006502F02A
  000000006502EFFE: FF 15 74 79 66 00  call        qword ptr [0000000065696978h]
  000000006502F004: 84 C0              test        al,al
  000000006502F006: 75 22              jne         000000006502F02A

  Summary

      695000 .text
