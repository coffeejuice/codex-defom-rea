Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CFC950: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CFC955: 48 83 EC 18        sub         rsp,18h
  0000000140CFC959: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140CFC95E: 48 83 B8 30 02 00  cmp         qword ptr [rax+0000000000000230h],0
                    00 00
  0000000140CFC966: 74 06              je          0000000140CFC96E
  0000000140CFC968: C6 04 24 01        mov         byte ptr [rsp],1
  0000000140CFC96C: EB 04              jmp         0000000140CFC972
  0000000140CFC96E: C6 04 24 00        mov         byte ptr [rsp],0
  0000000140CFC972: 8A 04 24           mov         al,byte ptr [rsp]
  0000000140CFC975: 48 83 C4 18        add         rsp,18h
  0000000140CFC979: C3                 ret
  0000000140CFC97A: CC                 int         3

  Summary

     1E02000 .text
