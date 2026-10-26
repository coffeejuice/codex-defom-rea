Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C90FF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C90FF5: 48 83 EC 48        sub         rsp,48h
  0000000140C90FF9: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C90FFE: 48 83 B8 48 02 00  cmp         qword ptr [rax+0000000000000248h],0
                    00 00
  0000000140C91006: 74 58              je          0000000140C91060
  0000000140C91008: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C9100D: 48 8B 80 48 02 00  mov         rax,qword ptr [rax+0000000000000248h]
                    00
  0000000140C91014: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C91019: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C9101E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C91023: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140C91029: 74 1C              je          0000000140C91047
  0000000140C9102B: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C91030: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C91033: BA 01 00 00 00     mov         edx,1
  0000000140C91038: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C9103D: FF 50 18           call        qword ptr [rax+18h]
  0000000140C91040: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C91045: EB 09              jmp         0000000140C91050
  0000000140C91047: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140C91050: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C91055: 48 C7 80 48 02 00  mov         qword ptr [rax+0000000000000248h],0
                    00 00 00 00 00
  0000000140C91060: 48 83 C4 48        add         rsp,48h
  0000000140C91064: C3                 ret

  Summary

     1E02000 .text
