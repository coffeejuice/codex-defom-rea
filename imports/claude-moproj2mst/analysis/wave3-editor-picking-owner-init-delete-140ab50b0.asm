Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140AB50B0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140AB50B5: 48 83 EC 48        sub         rsp,48h
  0000000140AB50B9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140AB50C2: B9 20 00 00 00     mov         ecx,20h
  0000000140AB50C7: E8 F0 99 9B 00     call        000000014146EABC
  0000000140AB50CC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140AB50D1: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140AB50D7: 74 11              je          0000000140AB50EA
  0000000140AB50D9: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140AB50DE: E8 0D F8 DD FF     call        00000001408948F0
  0000000140AB50E3: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140AB50E8: EB 09              jmp         0000000140AB50F3
  0000000140AB50EA: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140AB50F3: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140AB50F8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140AB50FD: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140AB5102: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140AB5107: 48 89 01           mov         qword ptr [rcx],rax
  0000000140AB510A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140AB510F: 48 83 C4 48        add         rsp,48h
  0000000140AB5113: C3                 ret

  Summary

     1E02000 .text
