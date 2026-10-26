Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C157F0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C157F5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C157FA: 48 83 EC 48        sub         rsp,48h
  0000000140C157FE: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C15803: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  0000000140C15808: 74 52              je          0000000140C1585C
  0000000140C1580A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C1580F: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  0000000140C15813: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C15818: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C1581D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C15822: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140C15828: 74 1C              je          0000000140C15846
  0000000140C1582A: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C1582F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C15832: BA 01 00 00 00     mov         edx,1
  0000000140C15837: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C1583C: FF 50 18           call        qword ptr [rax+18h]
  0000000140C1583F: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C15844: EB 09              jmp         0000000140C1584F
  0000000140C15846: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140C1584F: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C15854: 48 C7 40 18 00 00  mov         qword ptr [rax+18h],0
                    00 00
  0000000140C1585C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C15861: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140C15866: 48 89 41 18        mov         qword ptr [rcx+18h],rax
  0000000140C1586A: 48 83 C4 48        add         rsp,48h
  0000000140C1586E: C3                 ret

  Summary

     1E02000 .text
