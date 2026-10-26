Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDD980: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDD985: 48 83 EC 48        sub         rsp,48h
  0000000140CDD989: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140CDD992: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140CDD997: 48 83 B8 78 19 00  cmp         qword ptr [rax+0000000000001978h],0
                    00 00
  0000000140CDD99F: 75 51              jne         0000000140CDD9F2
  0000000140CDD9A1: B9 E0 04 00 00     mov         ecx,4E0h
  0000000140CDD9A6: E8 11 11 79 00     call        000000014146EABC
  0000000140CDD9AB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CDD9B0: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140CDD9B6: 74 16              je          0000000140CDD9CE
  0000000140CDD9B8: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140CDD9BD: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140CDD9C2: E8 F9 C2 62 FF     call        0000000140309CC0
  0000000140CDD9C7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CDD9CC: EB 09              jmp         0000000140CDD9D7
  0000000140CDD9CE: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140CDD9D7: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140CDD9DC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CDD9E1: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDD9E6: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140CDD9EB: 48 89 81 78 19 00  mov         qword ptr [rcx+0000000000001978h],rax
                    00
  0000000140CDD9F2: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140CDD9F7: 48 8B 80 78 19 00  mov         rax,qword ptr [rax+0000000000001978h]
                    00
  0000000140CDD9FE: 48 83 C4 48        add         rsp,48h
  0000000140CDDA02: C3                 ret

  Summary

     1E02000 .text
