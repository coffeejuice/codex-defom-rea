Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712C6F0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000006712C6F5: 53                 push        rbx
  000000006712C6F6: 48 83 EC 40        sub         rsp,40h
  000000006712C6FA: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006712C703: 49 8B C0           mov         rax,r8
  000000006712C706: 48 8B DA           mov         rbx,rdx
  000000006712C709: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  000000006712C711: 48 8D 15 38 43 1C  lea         rdx,[00000000672F0A50h]
                    00
  000000006712C718: 48 89 13           mov         qword ptr [rbx],rdx
  000000006712C71B: F0 83 05 2D 43 1C  lock add    dword ptr [00000000672F0A50h],1
                    00 01
  000000006712C723: C7 44 24 68 01 00  mov         dword ptr [rsp+68h],1
                    00 00
  000000006712C72B: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000006712C730: 4C 8D 0D B9 F8 10  lea         r9,[000000006723BFF0h]
                    00
  000000006712C737: 45 33 C0           xor         r8d,r8d
  000000006712C73A: 48 8B D0           mov         rdx,rax
  000000006712C73D: E8 CE F9 FF FF     call        000000006712C110
  000000006712C742: 48 8B C3           mov         rax,rbx
  000000006712C745: 48 83 C4 40        add         rsp,40h
  000000006712C749: 5B                 pop         rbx
  000000006712C74A: C3                 ret
  000000006712C74B: CC                 int         3

  Summary

      1A7000 .text
