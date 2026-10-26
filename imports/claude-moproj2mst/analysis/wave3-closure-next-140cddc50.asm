Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDDC50: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDDC55: 48 83 EC 48        sub         rsp,48h
  0000000140CDDC59: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140CDDC62: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140CDDC67: 48 83 B8 A0 19 00  cmp         qword ptr [rax+00000000000019A0h],0
                    00 00
  0000000140CDDC6F: 75 51              jne         0000000140CDDCC2
  0000000140CDDC71: B9 18 00 00 00     mov         ecx,18h
  0000000140CDDC76: E8 41 0E 79 00     call        000000014146EABC
  0000000140CDDC7B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CDDC80: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140CDDC86: 74 16              je          0000000140CDDC9E
  0000000140CDDC88: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140CDDC8D: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140CDDC92: E8 89 9C 9D 00     call        00000001416B7920
  0000000140CDDC97: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CDDC9C: EB 09              jmp         0000000140CDDCA7
  0000000140CDDC9E: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140CDDCA7: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140CDDCAC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CDDCB1: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDDCB6: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140CDDCBB: 48 89 81 A0 19 00  mov         qword ptr [rcx+00000000000019A0h],rax
                    00
  0000000140CDDCC2: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140CDDCC7: 48 8B 80 A0 19 00  mov         rax,qword ptr [rax+00000000000019A0h]
                    00
  0000000140CDDCCE: 48 83 C4 48        add         rsp,48h
  0000000140CDDCD2: C3                 ret

  Summary

     1E02000 .text
