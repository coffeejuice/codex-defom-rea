Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B7A70: 40 53              push        rbx
  00000000670B7A72: 48 83 EC 20        sub         rsp,20h
  00000000670B7A76: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000670B7A79: 48 8B DA           mov         rbx,rdx
  00000000670B7A7C: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000000670B7A84: F6 81 B4 00 00 00  test        byte ptr [rcx+00000000000000B4h],1
                    01
  00000000670B7A8B: 74 1B              je          00000000670B7AA8
  00000000670B7A8D: 48 8D 0D AC C1 13  lea         rcx,[00000000671F3C40h]
                    00
  00000000670B7A94: 83 CA FF           or          edx,0FFFFFFFFh
  00000000670B7A97: E8 A4 44 FB FF     call        000000006706BF40
  00000000670B7A9C: 48 89 03           mov         qword ptr [rbx],rax
  00000000670B7A9F: 48 8B C3           mov         rax,rbx
  00000000670B7AA2: 48 83 C4 20        add         rsp,20h
  00000000670B7AA6: 5B                 pop         rbx
  00000000670B7AA7: C3                 ret
  00000000670B7AA8: 48 83 C1 08        add         rcx,8
  00000000670B7AAC: E8 FF 79 03 00     call        00000000670EF4B0
  00000000670B7AB1: 48 8B C3           mov         rax,rbx
  00000000670B7AB4: 48 83 C4 20        add         rsp,20h
  00000000670B7AB8: 5B                 pop         rbx
  00000000670B7AB9: C3                 ret

  Summary

      1A7000 .text
