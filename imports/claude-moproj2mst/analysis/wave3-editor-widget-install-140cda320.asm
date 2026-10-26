Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDA320: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CDA325: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDA32A: 48 83 EC 48        sub         rsp,48h
  0000000140CDA32E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA333: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140CDA337: E8 D4 52 36 FF     call        000000014003F610
  0000000140CDA33C: 48 85 C0           test        rax,rax
  0000000140CDA33F: 74 4E              je          0000000140CDA38F
  0000000140CDA341: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA346: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140CDA34A: E8 C1 52 36 FF     call        000000014003F610
  0000000140CDA34F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDA357: 4C 8D 0D 9A 2A 89  lea         r9,[000000014356CDF8h]
                    02
  0000000140CDA35E: 4C 8D 05 F3 D1 88  lea         r8,[0000000143567558h]
                    02
  0000000140CDA365: 33 D2              xor         edx,edx
  0000000140CDA367: 48 8B C8           mov         rcx,rax
  0000000140CDA36A: E8 8D 55 79 00     call        000000014146F8FC
  0000000140CDA36F: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CDA374: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140CDA37A: 74 13              je          0000000140CDA38F
  0000000140CDA37C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140CDA381: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CDA384: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CDA389: FF 90 D0 01 00 00  call        qword ptr [rax+00000000000001D0h]
  0000000140CDA38F: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140CDA394: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA399: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140CDA39D: E8 7E 38 0D 00     call        0000000140DADC20
  0000000140CDA3A2: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA3A7: FF 15 BB F6 12 01  call        qword ptr [0000000141E09A68h]
  0000000140CDA3AD: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140CDA3B2: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA3B7: FF 15 A3 F6 12 01  call        qword ptr [0000000141E09A60h]
  0000000140CDA3BD: 48 83 C4 48        add         rsp,48h
  0000000140CDA3C1: C3                 ret

  Summary

     1E02000 .text
