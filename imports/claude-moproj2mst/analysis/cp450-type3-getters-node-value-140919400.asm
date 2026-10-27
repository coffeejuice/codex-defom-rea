Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140919400: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140919405: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091940A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091940F: 48 83 EC 48        sub         rsp,48h
  0000000140919413: 33 C0              xor         eax,eax
  0000000140919415: 83 F8 01           cmp         eax,1
  0000000140919418: 74 5C              je          0000000140919476
  000000014091941A: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014091941F: 48 39 44 24 58     cmp         qword ptr [rsp+58h],rax
  0000000140919424: 74 4E              je          0000000140919474
  0000000140919426: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014091942B: 48 83 E8 08        sub         rax,8
  000000014091942F: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140919434: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140919439: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091943C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140919441: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140919446: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014091944B: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140919451: 74 16              je          0000000140919469
  0000000140919453: BA 01 00 00 00     mov         edx,1
  0000000140919458: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014091945D: E8 9E 01 00 00     call        0000000140919600
  0000000140919462: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140919467: EB 09              jmp         0000000140919472
  0000000140919469: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140919472: EB A6              jmp         000000014091941A
  0000000140919474: EB 2F              jmp         00000001409194A5
  0000000140919476: 33 C0              xor         eax,eax
  0000000140919478: 83 F8 01           cmp         eax,1
  000000014091947B: 74 28              je          00000001409194A5
  000000014091947D: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140919482: 48 39 44 24 58     cmp         qword ptr [rsp+58h],rax
  0000000140919487: 74 1C              je          00000001409194A5
  0000000140919489: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014091948E: 48 83 E8 08        sub         rax,8
  0000000140919492: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140919497: 33 D2              xor         edx,edx
  0000000140919499: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014091949E: E8 5D 01 00 00     call        0000000140919600
  00000001409194A3: EB D8              jmp         000000014091947D
  00000001409194A5: 48 83 C4 48        add         rsp,48h
  00000001409194A9: C3                 ret

  Summary

     1E02000 .text
