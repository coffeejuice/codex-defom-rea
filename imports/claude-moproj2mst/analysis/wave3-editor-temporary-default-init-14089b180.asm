Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014089B180: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089B185: 48 83 EC 48        sub         rsp,48h
  000000014089B189: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014089B18E: E8 5D 00 00 00     call        000000014089B1F0
  000000014089B193: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014089B198: 48 83 78 68 00     cmp         qword ptr [rax+68h],0
  000000014089B19D: 74 3F              je          000000014089B1DE
  000000014089B19F: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014089B1A4: 48 8B 40 68        mov         rax,qword ptr [rax+68h]
  000000014089B1A8: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014089B1AD: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014089B1B2: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014089B1B7: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014089B1BD: 74 16              je          000000014089B1D5
  000000014089B1BF: BA 01 00 00 00     mov         edx,1
  000000014089B1C4: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014089B1C9: E8 02 58 86 FF     call        00000001401009D0
  000000014089B1CE: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014089B1D3: EB 09              jmp         000000014089B1DE
  000000014089B1D5: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014089B1DE: 48 83 C4 48        add         rsp,48h
  000000014089B1E2: C3                 ret

  Summary

     1E02000 .text
