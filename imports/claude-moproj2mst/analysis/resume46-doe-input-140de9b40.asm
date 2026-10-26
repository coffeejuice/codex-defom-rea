Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DE9B40: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140DE9B45: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140DE9B4A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DE9B4F: 48 83 EC 48        sub         rsp,48h
  0000000140DE9B53: 33 C0              xor         eax,eax
  0000000140DE9B55: 85 C0              test        eax,eax
  0000000140DE9B57: 74 4B              je          0000000140DE9BA4
  0000000140DE9B59: B9 08 00 00 00     mov         ecx,8
  0000000140DE9B5E: E8 59 4F 68 00     call        000000014146EABC
  0000000140DE9B63: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DE9B68: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140DE9B6E: 74 1C              je          0000000140DE9B8C
  0000000140DE9B70: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DE9B75: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140DE9B7A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140DE9B7D: 48 89 01           mov         qword ptr [rcx],rax
  0000000140DE9B80: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140DE9B85: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140DE9B8A: EB 09              jmp         0000000140DE9B95
  0000000140DE9B8C: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140DE9B95: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140DE9B9A: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140DE9B9F: 48 89 01           mov         qword ptr [rcx],rax
  0000000140DE9BA2: EB 5E              jmp         0000000140DE9C02
  0000000140DE9BA4: 33 C0              xor         eax,eax
  0000000140DE9BA6: 85 C0              test        eax,eax
  0000000140DE9BA8: 74 43              je          0000000140DE9BED
  0000000140DE9BAA: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140DE9BAF: B9 08 00 00 00     mov         ecx,8
  0000000140DE9BB4: E8 C7 96 21 FF     call        0000000140003280
  0000000140DE9BB9: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140DE9BBE: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140DE9BC4: 74 1C              je          0000000140DE9BE2
  0000000140DE9BC6: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140DE9BCB: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140DE9BD0: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140DE9BD3: 48 89 01           mov         qword ptr [rcx],rax
  0000000140DE9BD6: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140DE9BDB: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140DE9BE0: EB 09              jmp         0000000140DE9BEB
  0000000140DE9BE2: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140DE9BEB: EB 15              jmp         0000000140DE9C02
  0000000140DE9BED: 41 B8 08 00 00 00  mov         r8d,8
  0000000140DE9BF3: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140DE9BF8: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140DE9BFD: E8 FE 54 68 00     call        000000014146F100
  0000000140DE9C02: 48 83 C4 48        add         rsp,48h
  0000000140DE9C06: C3                 ret

  Summary

     1E02000 .text
