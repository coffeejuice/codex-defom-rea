Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067134C40: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000067134C45: 57                 push        rdi
  0000000067134C46: 48 83 EC 30        sub         rsp,30h
  0000000067134C4A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067134C53: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000067134C58: 8B FA              mov         edi,edx
  0000000067134C5A: 48 8B D9           mov         rbx,rcx
  0000000067134C5D: 48 8D 05 74 8E 10  lea         rax,[000000006723DAD8h]
                    00
  0000000067134C64: 48 89 01           mov         qword ptr [rcx],rax
  0000000067134C67: 8B 51 10           mov         edx,dword ptr [rcx+10h]
  0000000067134C6A: 85 D2              test        edx,edx
  0000000067134C6C: 7E 06              jle         0000000067134C74
  0000000067134C6E: E8 CD 65 FF FF     call        000000006712B240
  0000000067134C73: 90                 nop
  0000000067134C74: 48 8B CB           mov         rcx,rbx
  0000000067134C77: E8 F4 AB FF FF     call        000000006712F870
  0000000067134C7C: 40 F6 C7 01        test        dil,1
  0000000067134C80: 74 0D              je          0000000067134C8F
  0000000067134C82: 48 8B CB           mov         rcx,rbx
  0000000067134C85: E8 CC 3C 04 00     call        0000000067178956
  0000000067134C8A: 48 8B C3           mov         rax,rbx
  0000000067134C8D: EB 03              jmp         0000000067134C92
  0000000067134C8F: 48 8B C3           mov         rax,rbx
  0000000067134C92: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000067134C97: 48 83 C4 30        add         rsp,30h
  0000000067134C9B: 5F                 pop         rdi
  0000000067134C9C: C3                 ret

  Summary

      1A7000 .text
