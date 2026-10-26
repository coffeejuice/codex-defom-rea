Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DD77D0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140DD77D5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DD77DA: 48 83 EC 48        sub         rsp,48h
  0000000140DD77DE: 33 C0              xor         eax,eax
  0000000140DD77E0: 83 F8 01           cmp         eax,1
  0000000140DD77E3: 74 40              je          0000000140DD7825
  0000000140DD77E5: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140DD77EA: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140DD77ED: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140DD77F2: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140DD77F7: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DD77FC: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140DD7802: 74 16              je          0000000140DD781A
  0000000140DD7804: BA 01 00 00 00     mov         edx,1
  0000000140DD7809: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD780E: E8 2D 69 E3 FF     call        0000000140C0E140
  0000000140DD7813: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140DD7818: EB 09              jmp         0000000140DD7823
  0000000140DD781A: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140DD7823: EB 13              jmp         0000000140DD7838
  0000000140DD7825: 33 C0              xor         eax,eax
  0000000140DD7827: 83 F8 01           cmp         eax,1
  0000000140DD782A: 74 0C              je          0000000140DD7838
  0000000140DD782C: 33 D2              xor         edx,edx
  0000000140DD782E: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140DD7833: E8 08 69 E3 FF     call        0000000140C0E140
  0000000140DD7838: 48 83 C4 48        add         rsp,48h
  0000000140DD783C: C3                 ret

  Summary

     1E02000 .text
