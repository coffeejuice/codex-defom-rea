Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400AE510: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001400AE515: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400AE51A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400AE51F: 48 83 EC 48        sub         rsp,48h
  00000001400AE523: 33 C0              xor         eax,eax
  00000001400AE525: 83 F8 01           cmp         eax,1
  00000001400AE528: 74 5C              je          00000001400AE586
  00000001400AE52A: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400AE52F: 48 39 44 24 58     cmp         qword ptr [rsp+58h],rax
  00000001400AE534: 74 4E              je          00000001400AE584
  00000001400AE536: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400AE53B: 48 83 E8 08        sub         rax,8
  00000001400AE53F: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001400AE544: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400AE549: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400AE54C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400AE551: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001400AE556: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400AE55B: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001400AE561: 74 16              je          00000001400AE579
  00000001400AE563: BA 01 00 00 00     mov         edx,1
  00000001400AE568: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400AE56D: E8 2E 03 00 00     call        00000001400AE8A0
  00000001400AE572: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400AE577: EB 09              jmp         00000001400AE582
  00000001400AE579: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001400AE582: EB A6              jmp         00000001400AE52A
  00000001400AE584: EB 2F              jmp         00000001400AE5B5
  00000001400AE586: 33 C0              xor         eax,eax
  00000001400AE588: 83 F8 01           cmp         eax,1
  00000001400AE58B: 74 28              je          00000001400AE5B5
  00000001400AE58D: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400AE592: 48 39 44 24 58     cmp         qword ptr [rsp+58h],rax
  00000001400AE597: 74 1C              je          00000001400AE5B5
  00000001400AE599: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400AE59E: 48 83 E8 08        sub         rax,8
  00000001400AE5A2: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001400AE5A7: 33 D2              xor         edx,edx
  00000001400AE5A9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400AE5AE: E8 ED 02 00 00     call        00000001400AE8A0
  00000001400AE5B3: EB D8              jmp         00000001400AE58D
  00000001400AE5B5: 48 83 C4 48        add         rsp,48h
  00000001400AE5B9: C3                 ret

  Summary

     1E02000 .text
