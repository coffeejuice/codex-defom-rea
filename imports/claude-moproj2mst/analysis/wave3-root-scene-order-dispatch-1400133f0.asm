Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400133F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400133F5: 48 83 EC 48        sub         rsp,48h
  00000001400133F9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140013402: B9 28 00 00 00     mov         ecx,28h
  0000000140013407: E8 B0 B6 45 01     call        000000014146EABC
  000000014001340C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140013411: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140013417: 74 16              je          000000014001342F
  0000000140013419: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  000000014001341E: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140013423: E8 F8 1F 03 00     call        0000000140045420
  0000000140013428: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014001342D: EB 09              jmp         0000000140013438
  000000014001342F: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140013438: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014001343D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140013442: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140013447: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014001344C: 48 89 41 50        mov         qword ptr [rcx+50h],rax
  0000000140013450: 48 83 C4 48        add         rsp,48h
  0000000140013454: C3                 ret

  Summary

     1E02000 .text
