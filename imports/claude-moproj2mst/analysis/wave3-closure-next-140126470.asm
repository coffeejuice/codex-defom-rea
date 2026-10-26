Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140126470: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140126475: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014012647A: 48 83 EC 18        sub         rsp,18h
  000000014012647E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140126483: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140126488: 48 8B 00           mov         rax,qword ptr [rax]
  000000014012648B: 48 39 01           cmp         qword ptr [rcx],rax
  000000014012648E: 74 09              je          0000000140126499
  0000000140126490: C7 04 24 01 00 00  mov         dword ptr [rsp],1
                    00
  0000000140126497: EB 07              jmp         00000001401264A0
  0000000140126499: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  00000001401264A0: 8A 04 24           mov         al,byte ptr [rsp]
  00000001401264A3: 48 83 C4 18        add         rsp,18h
  00000001401264A7: C3                 ret

  Summary

     1E02000 .text
