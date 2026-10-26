Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C15540: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C15545: 48 83 EC 48        sub         rsp,48h
  0000000140C15549: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C1554E: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  0000000140C15553: 74 52              je          0000000140C155A7
  0000000140C15555: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C1555A: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  0000000140C1555E: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C15563: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C15568: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C1556D: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140C15573: 74 1C              je          0000000140C15591
  0000000140C15575: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C1557A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C1557D: BA 01 00 00 00     mov         edx,1
  0000000140C15582: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C15587: FF 50 18           call        qword ptr [rax+18h]
  0000000140C1558A: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C1558F: EB 09              jmp         0000000140C1559A
  0000000140C15591: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140C1559A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C1559F: 48 C7 40 18 00 00  mov         qword ptr [rax+18h],0
                    00 00
  0000000140C155A7: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C155AC: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C155AF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C155B4: FF 50 58           call        qword ptr [rax+58h]
  0000000140C155B7: 4C 8B D8           mov         r11,rax
  0000000140C155BA: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C155BF: 4C 89 58 18        mov         qword ptr [rax+18h],r11
  0000000140C155C3: 48 83 C4 48        add         rsp,48h
  0000000140C155C7: C3                 ret

  Summary

     1E02000 .text
