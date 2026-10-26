Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714BDD0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006714BDD5: 53                 push        rbx
  000000006714BDD6: 48 83 EC 30        sub         rsp,30h
  000000006714BDDA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006714BDE3: 48 8B DA           mov         rbx,rdx
  000000006714BDE6: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006714BDE9: 44 8B 00           mov         r8d,dword ptr [rax]
  000000006714BDEC: 41 83 F8 01        cmp         r8d,1
  000000006714BDF0: 74 14              je          000000006714BE06
  000000006714BDF2: 33 D2              xor         edx,edx
  000000006714BDF4: 44 8D 42 01        lea         r8d,[rdx+1]
  000000006714BDF8: E8 13 FB FF FF     call        000000006714B910
  000000006714BDFD: 90                 nop
  000000006714BDFE: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006714BE01: 48 89 08           mov         qword ptr [rax],rcx
  000000006714BE04: EB 0C              jmp         000000006714BE12
  000000006714BE06: 48 8B 1A           mov         rbx,qword ptr [rdx]
  000000006714BE09: E8 52 CE EF FF     call        0000000067048C60
  000000006714BE0E: 90                 nop
  000000006714BE0F: 48 89 18           mov         qword ptr [rax],rbx
  000000006714BE12: 48 83 C4 30        add         rsp,30h
  000000006714BE16: 5B                 pop         rbx
  000000006714BE17: C3                 ret

  Summary

      1A7000 .text
