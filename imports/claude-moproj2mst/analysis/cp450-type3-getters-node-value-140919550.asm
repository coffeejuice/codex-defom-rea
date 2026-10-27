Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140919550: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140919555: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091955A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091955F: 48 83 EC 48        sub         rsp,48h
  0000000140919563: 33 C0              xor         eax,eax
  0000000140919565: 83 F8 01           cmp         eax,1
  0000000140919568: 74 5C              je          00000001409195C6
  000000014091956A: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014091956F: 48 39 44 24 58     cmp         qword ptr [rsp+58h],rax
  0000000140919574: 74 4E              je          00000001409195C4
  0000000140919576: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014091957B: 48 83 E8 08        sub         rax,8
  000000014091957F: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140919584: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140919589: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091958C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140919591: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140919596: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014091959B: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001409195A1: 74 16              je          00000001409195B9
  00000001409195A3: BA 01 00 00 00     mov         edx,1
  00000001409195A8: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001409195AD: E8 AE 00 00 00     call        0000000140919660
  00000001409195B2: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001409195B7: EB 09              jmp         00000001409195C2
  00000001409195B9: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001409195C2: EB A6              jmp         000000014091956A
  00000001409195C4: EB 2F              jmp         00000001409195F5
  00000001409195C6: 33 C0              xor         eax,eax
  00000001409195C8: 83 F8 01           cmp         eax,1
  00000001409195CB: 74 28              je          00000001409195F5
  00000001409195CD: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001409195D2: 48 39 44 24 58     cmp         qword ptr [rsp+58h],rax
  00000001409195D7: 74 1C              je          00000001409195F5
  00000001409195D9: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001409195DE: 48 83 E8 08        sub         rax,8
  00000001409195E2: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001409195E7: 33 D2              xor         edx,edx
  00000001409195E9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001409195EE: E8 6D 00 00 00     call        0000000140919660
  00000001409195F3: EB D8              jmp         00000001409195CD
  00000001409195F5: 48 83 C4 48        add         rsp,48h
  00000001409195F9: C3                 ret

  Summary

     1E02000 .text
