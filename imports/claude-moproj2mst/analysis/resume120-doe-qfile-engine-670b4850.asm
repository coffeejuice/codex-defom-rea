Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B4850: 48 83 EC 38        sub         rsp,38h
  00000000670B4854: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B485D: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000670B4861: C7 80 20 01 00 00  mov         dword ptr [rax+0000000000000120h],0
                    00 00 00 00
  00000000670B486B: 48 8D 88 90 00 00  lea         rcx,[rax+0000000000000090h]
                    00
  00000000670B4872: 48 8D 05 B7 C2 23  lea         rax,[00000000672F0B30h]
                    00
  00000000670B4879: 48 39 01           cmp         qword ptr [rcx],rax
  00000000670B487C: 74 2D              je          00000000670B48AB
  00000000670B487E: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670B4883: F0 83 05 A5 C2 23  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670B488B: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670B4890: E8 3B BB FB FF     call        00000000670703D0
  00000000670B4895: 90                 nop
  00000000670B4896: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670B489B: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B489F: 75 0A              jne         00000000670B48AB
  00000000670B48A1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670B48A6: E8 25 B8 FB FF     call        00000000670700D0
  00000000670B48AB: 48 83 C4 38        add         rsp,38h
  00000000670B48AF: C3                 ret

  Summary

      1A7000 .text
