Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140044C80: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140044C85: 48 83 EC 48        sub         rsp,48h
  0000000140044C89: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140044C92: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140044C97: 48 83 B8 C8 19 00  cmp         qword ptr [rax+00000000000019C8h],0
                    00 00
  0000000140044C9F: 75 59              jne         0000000140044CFA
  0000000140044CA1: B9 38 00 00 00     mov         ecx,38h
  0000000140044CA6: E8 11 9E 42 01     call        000000014146EABC
  0000000140044CAB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140044CB0: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140044CB6: 74 1E              je          0000000140044CD6
  0000000140044CB8: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140044CBD: E8 CE FD FF FF     call        0000000140044A90
  0000000140044CC2: 48 8B D0           mov         rdx,rax
  0000000140044CC5: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140044CCA: E8 11 13 00 00     call        0000000140045FE0
  0000000140044CCF: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140044CD4: EB 09              jmp         0000000140044CDF
  0000000140044CD6: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140044CDF: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140044CE4: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140044CE9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140044CEE: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140044CF3: 48 89 81 C8 19 00  mov         qword ptr [rcx+00000000000019C8h],rax
                    00
  0000000140044CFA: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140044CFF: 48 8B 80 C8 19 00  mov         rax,qword ptr [rax+00000000000019C8h]
                    00
  0000000140044D06: 48 83 C4 48        add         rsp,48h
  0000000140044D0A: C3                 ret

  Summary

     1E02000 .text
