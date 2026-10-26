Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140050690: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140050695: 48 83 EC 78        sub         rsp,78h
  0000000140050699: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400506A1: E8 1A 0C 00 00     call        00000001400512C0
  00000001400506A6: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400506AE: E8 CD F2 00 00     call        000000014005F980
  00000001400506B3: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400506BB: E8 C0 0A 01 00     call        0000000140061180
  00000001400506C0: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400506C8: E8 A3 18 01 00     call        0000000140061F70
  00000001400506CD: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400506D5: 48 83 78 20 00     cmp         qword ptr [rax+20h],0
  00000001400506DA: 74 52              je          000000014005072E
  00000001400506DC: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400506E4: 48 8B 40 20        mov         rax,qword ptr [rax+20h]
  00000001400506E8: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400506ED: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001400506F2: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400506F7: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001400506FD: 74 16              je          0000000140050715
  00000001400506FF: BA 01 00 00 00     mov         edx,1
  0000000140050704: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140050709: E8 32 4D 01 00     call        0000000140065440
  000000014005070E: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140050713: EB 09              jmp         000000014005071E
  0000000140050715: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  000000014005071E: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140050726: 48 C7 40 20 00 00  mov         qword ptr [rax+20h],0
                    00 00
  000000014005072E: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140050736: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  000000014005073B: 74 52              je          000000014005078F
  000000014005073D: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140050745: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140050749: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014005074E: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140050753: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140050758: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014005075E: 74 16              je          0000000140050776
  0000000140050760: BA 01 00 00 00     mov         edx,1
  0000000140050765: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014005076A: E8 91 4C 01 00     call        0000000140065400
  000000014005076F: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140050774: EB 09              jmp         000000014005077F
  0000000140050776: 48 C7 44 24 58 00  mov         qword ptr [rsp+58h],0
                    00 00 00
  000000014005077F: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140050787: 48 C7 40 28 00 00  mov         qword ptr [rax+28h],0
                    00 00
  000000014005078F: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140050797: 48 81 C1 60 01 00  add         rcx,160h
                    00
  000000014005079E: E8 4D A7 BC 00     call        0000000140C1AEF0
  00000001400507A3: 48 85 C0           test        rax,rax
  00000001400507A6: 74 66              je          000000014005080E
  00000001400507A8: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400507B0: 48 81 C1 60 01 00  add         rcx,160h
                    00
  00000001400507B7: E8 34 A7 BC 00     call        0000000140C1AEF0
  00000001400507BC: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400507C1: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001400507C6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400507CB: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001400507D1: 74 1C              je          00000001400507EF
  00000001400507D3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400507D8: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400507DB: BA 01 00 00 00     mov         edx,1
  00000001400507E0: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400507E5: FF 50 18           call        qword ptr [rax+18h]
  00000001400507E8: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001400507ED: EB 09              jmp         00000001400507F8
  00000001400507EF: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  00000001400507F8: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140050800: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140050807: 33 D2              xor         edx,edx
  0000000140050809: E8 12 C1 C6 00     call        0000000140CBC920
  000000014005080E: 48 83 C4 78        add         rsp,78h
  0000000140050812: C3                 ret

  Summary

     1E02000 .text
