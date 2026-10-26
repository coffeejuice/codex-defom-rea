Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EB52F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EB52F5: 48 83 EC 48        sub         rsp,48h
  0000000140EB52F9: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140EB52FE: 48 83 B8 F8 00 00  cmp         qword ptr [rax+00000000000000F8h],0
                    00 00
  0000000140EB5306: 74 42              je          0000000140EB534A
  0000000140EB5308: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140EB530D: 48 8B 80 F8 00 00  mov         rax,qword ptr [rax+00000000000000F8h]
                    00
  0000000140EB5314: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140EB5319: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140EB531E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140EB5323: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140EB5329: 74 16              je          0000000140EB5341
  0000000140EB532B: BA 03 00 00 00     mov         edx,3
  0000000140EB5330: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140EB5335: E8 06 F3 07 00     call        0000000140F34640
  0000000140EB533A: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140EB533F: EB 09              jmp         0000000140EB534A
  0000000140EB5341: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140EB534A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140EB534F: 48 C7 80 F8 00 00  mov         qword ptr [rax+00000000000000F8h],0
                    00 00 00 00 00
  0000000140EB535A: 48 83 C4 48        add         rsp,48h
  0000000140EB535E: C3                 ret

  Summary

     1E02000 .text
