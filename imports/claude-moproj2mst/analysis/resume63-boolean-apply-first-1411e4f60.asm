Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411E4F60: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001411E4F65: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411E4F6A: 48 83 EC 28        sub         rsp,28h
  00000001411E4F6E: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001411E4F74: 75 04              jne         00000001411E4F7A
  00000001411E4F76: 33 C0              xor         eax,eax
  00000001411E4F78: EB 2B              jmp         00000001411E4FA5
  00000001411E4F7A: 48 8D 15 97 96 AF  lea         rdx,[0000000142CDE618h]
                    01
  00000001411E4F81: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001411E4F86: E8 B5 A0 28 00     call        000000014146F040
  00000001411E4F8B: 85 C0              test        eax,eax
  00000001411E4F8D: 75 07              jne         00000001411E4F96
  00000001411E4F8F: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001411E4F94: EB 0F              jmp         00000001411E4FA5
  00000001411E4F96: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001411E4F9B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411E4FA0: E8 CB 1B 19 FF     call        0000000140376B70
  00000001411E4FA5: 48 83 C4 28        add         rsp,28h
  00000001411E4FA9: C3                 ret

  Summary

     1E02000 .text
