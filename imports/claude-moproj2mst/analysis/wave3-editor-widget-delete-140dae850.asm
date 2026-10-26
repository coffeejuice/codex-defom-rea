Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DAE850: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DAE855: 48 83 EC 48        sub         rsp,48h
  0000000140DAE859: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140DAE85E: 48 83 C1 38        add         rcx,38h
  0000000140DAE862: E8 89 C6 E6 FF     call        0000000140C1AEF0
  0000000140DAE867: 48 85 C0           test        rax,rax
  0000000140DAE86A: 74 5A              je          0000000140DAE8C6
  0000000140DAE86C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140DAE871: 48 83 C1 38        add         rcx,38h
  0000000140DAE875: E8 76 C6 E6 FF     call        0000000140C1AEF0
  0000000140DAE87A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140DAE87F: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140DAE884: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DAE889: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140DAE88F: 74 1C              je          0000000140DAE8AD
  0000000140DAE891: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140DAE896: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140DAE899: BA 01 00 00 00     mov         edx,1
  0000000140DAE89E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DAE8A3: FF 50 18           call        qword ptr [rax+18h]
  0000000140DAE8A6: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140DAE8AB: EB 09              jmp         0000000140DAE8B6
  0000000140DAE8AD: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140DAE8B6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140DAE8BB: 48 83 C1 38        add         rcx,38h
  0000000140DAE8BF: 33 D2              xor         edx,edx
  0000000140DAE8C1: E8 5A E0 F0 FF     call        0000000140CBC920
  0000000140DAE8C6: 48 83 C4 48        add         rsp,48h
  0000000140DAE8CA: C3                 ret

  Summary

     1E02000 .text
