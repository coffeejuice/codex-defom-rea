Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014001D770: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014001D775: 48 83 EC 38        sub         rsp,38h
  000000014001D779: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  000000014001D782: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014001D787: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001D78A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014001D78F: FF 90 78 07 00 00  call        qword ptr [rax+0000000000000778h]
  000000014001D795: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014001D79A: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014001D7A0: 74 13              je          000000014001D7B5
  000000014001D7A2: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014001D7A7: 48 83 C1 10        add         rcx,10h
  000000014001D7AB: E8 60 FE CE 00     call        0000000140D0D610
  000000014001D7B0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014001D7B5: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014001D7BA: 48 83 C4 38        add         rsp,38h
  000000014001D7BE: C3                 ret

  Summary

     1E02000 .text
