Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140321520: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140321525: 48 83 EC 48        sub         rsp,48h
  0000000140321529: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014032152E: 48 83 B8 C8 04 00  cmp         qword ptr [rax+00000000000004C8h],0
                    00 00
  0000000140321536: 74 57              je          000000014032158F
  0000000140321538: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014032153D: 48 8B 80 C8 04 00  mov         rax,qword ptr [rax+00000000000004C8h]
                    00
  0000000140321544: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140321549: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014032154E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140321553: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140321559: 74 1B              je          0000000140321576
  000000014032155B: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140321560: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140321563: BA 01 00 00 00     mov         edx,1
  0000000140321568: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014032156D: FF 10              call        qword ptr [rax]
  000000014032156F: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140321574: EB 09              jmp         000000014032157F
  0000000140321576: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014032157F: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140321584: 48 C7 80 C8 04 00  mov         qword ptr [rax+00000000000004C8h],0
                    00 00 00 00 00
  000000014032158F: 48 83 C4 48        add         rsp,48h
  0000000140321593: C3                 ret

  Summary

     1E02000 .text
