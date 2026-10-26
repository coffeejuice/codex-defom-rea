Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006502F490: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006502F495: 57                 push        rdi
  000000006502F496: 48 83 EC 20        sub         rsp,20h
  000000006502F49A: 48 C7 01 00 00 00  mov         qword ptr [rcx],0
                    00
  000000006502F4A1: 80 3D C8 A3 99 00  cmp         byte ptr [00000000659C9870h],0
                    00
  000000006502F4A8: 8B FA              mov         edi,edx
  000000006502F4AA: 48 8B D9           mov         rbx,rcx
  000000006502F4AD: 75 05              jne         000000006502F4B4
  000000006502F4AF: E8 EC F9 FF FF     call        000000006502EEA0
  000000006502F4B4: 8B D7              mov         edx,edi
  000000006502F4B6: 48 8B CB           mov         rcx,rbx
  000000006502F4B9: E8 F2 FA FF FF     call        000000006502EFB0
  000000006502F4BE: 48 8B C3           mov         rax,rbx
  000000006502F4C1: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006502F4C6: 48 83 C4 20        add         rsp,20h
  000000006502F4CA: 5F                 pop         rdi
  000000006502F4CB: C3                 ret

  Summary

      695000 .text
