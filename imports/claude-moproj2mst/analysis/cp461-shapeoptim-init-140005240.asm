Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140005240: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140005245: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014000524A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014000524F: 48 83 EC 48        sub         rsp,48h
  0000000140005253: 33 C0              xor         eax,eax
  0000000140005255: 85 C0              test        eax,eax
  0000000140005257: 74 5C              je          00000001400052B5
  0000000140005259: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014000525E: 48 39 44 24 58     cmp         qword ptr [rsp+58h],rax
  0000000140005263: 74 4E              je          00000001400052B3
  0000000140005265: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014000526A: 48 83 E8 08        sub         rax,8
  000000014000526E: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140005273: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140005278: 48 8B 00           mov         rax,qword ptr [rax]
  000000014000527B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140005280: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140005285: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014000528A: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140005290: 74 16              je          00000001400052A8
  0000000140005292: BA 01 00 00 00     mov         edx,1
  0000000140005297: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014000529C: E8 DF DC FF FF     call        0000000140002F80
  00000001400052A1: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400052A6: EB 09              jmp         00000001400052B1
  00000001400052A8: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001400052B1: EB A6              jmp         0000000140005259
  00000001400052B3: EB 2F              jmp         00000001400052E4
  00000001400052B5: 33 C0              xor         eax,eax
  00000001400052B7: 83 F8 01           cmp         eax,1
  00000001400052BA: 74 28              je          00000001400052E4
  00000001400052BC: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400052C1: 48 39 44 24 58     cmp         qword ptr [rsp+58h],rax
  00000001400052C6: 74 1C              je          00000001400052E4
  00000001400052C8: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400052CD: 48 83 E8 08        sub         rax,8
  00000001400052D1: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001400052D6: 33 D2              xor         edx,edx
  00000001400052D8: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400052DD: E8 9E DC FF FF     call        0000000140002F80
  00000001400052E2: EB D8              jmp         00000001400052BC
  00000001400052E4: 48 83 C4 48        add         rsp,48h
  00000001400052E8: C3                 ret

  Summary

     1E02000 .text
