Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140018460: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140018465: 48 83 EC 48        sub         rsp,48h
  0000000140018469: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140018472: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140018477: 48 83 B8 C8 03 00  cmp         qword ptr [rax+00000000000003C8h],0
                    00 00
  000000014001847F: 75 51              jne         00000001400184D2
  0000000140018481: B9 30 00 00 00     mov         ecx,30h
  0000000140018486: E8 31 66 45 01     call        000000014146EABC
  000000014001848B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140018490: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140018496: 74 16              je          00000001400184AE
  0000000140018498: 41 B0 01           mov         r8b,1
  000000014001849B: 33 D2              xor         edx,edx
  000000014001849D: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001400184A2: E8 89 C8 CF 00     call        0000000140D14D30
  00000001400184A7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400184AC: EB 09              jmp         00000001400184B7
  00000001400184AE: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001400184B7: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001400184BC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400184C1: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400184C6: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400184CB: 48 89 81 C8 03 00  mov         qword ptr [rcx+00000000000003C8h],rax
                    00
  00000001400184D2: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400184D7: 48 8B 80 C8 03 00  mov         rax,qword ptr [rax+00000000000003C8h]
                    00
  00000001400184DE: 48 83 C4 48        add         rsp,48h
  00000001400184E2: C3                 ret

  Summary

     1E02000 .text
