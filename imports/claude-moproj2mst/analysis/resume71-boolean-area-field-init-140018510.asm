Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140018510: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140018515: 48 83 EC 48        sub         rsp,48h
  0000000140018519: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014001851E: 48 83 B8 C8 03 00  cmp         qword ptr [rax+00000000000003C8h],0
                    00 00
  0000000140018526: 74 58              je          0000000140018580
  0000000140018528: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014001852D: 48 8B 80 C8 03 00  mov         rax,qword ptr [rax+00000000000003C8h]
                    00
  0000000140018534: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140018539: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014001853E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140018543: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140018549: 74 1C              je          0000000140018567
  000000014001854B: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140018550: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140018553: BA 01 00 00 00     mov         edx,1
  0000000140018558: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014001855D: FF 50 18           call        qword ptr [rax+18h]
  0000000140018560: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140018565: EB 09              jmp         0000000140018570
  0000000140018567: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140018570: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140018575: 48 C7 80 C8 03 00  mov         qword ptr [rax+00000000000003C8h],0
                    00 00 00 00 00
  0000000140018580: 48 83 C4 48        add         rsp,48h
  0000000140018584: C3                 ret

  Summary

     1E02000 .text
