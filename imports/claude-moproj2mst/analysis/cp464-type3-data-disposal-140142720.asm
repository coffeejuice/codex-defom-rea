Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140142720: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140142725: 48 83 EC 48        sub         rsp,48h
  0000000140142729: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140142732: B9 18 00 00 00     mov         ecx,18h
  0000000140142737: E8 80 C3 32 01     call        000000014146EABC
  000000014014273C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140142741: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140142747: 74 11              je          000000014014275A
  0000000140142749: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014014274E: E8 3D 06 00 00     call        0000000140142D90
  0000000140142753: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140142758: EB 09              jmp         0000000140142763
  000000014014275A: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140142763: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140142768: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014014276D: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140142772: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140142777: 48 89 41 38        mov         qword ptr [rcx+38h],rax
  000000014014277B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140142780: E8 7B 00 00 00     call        0000000140142800
  0000000140142785: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014014278A: 48 83 C1 10        add         rcx,10h
  000000014014278E: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140142793: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140142797: FF 50 20           call        qword ptr [rax+20h]
  000000014014279A: 48 83 C4 48        add         rsp,48h
  000000014014279E: C3                 ret

  Summary

     1E02000 .text
