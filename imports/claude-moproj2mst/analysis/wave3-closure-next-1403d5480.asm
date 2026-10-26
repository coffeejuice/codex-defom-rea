Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001403D5480: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001403D5485: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001403D548A: 48 83 EC 28        sub         rsp,28h
  00000001403D548E: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001403D5494: 75 04              jne         00000001403D549A
  00000001403D5496: 33 C0              xor         eax,eax
  00000001403D5498: EB 2C              jmp         00000001403D54C6
  00000001403D549A: 48 8D 15 2F 79 F8  lea         rdx,[000000014235CDD0h]
                    01
  00000001403D54A1: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001403D54A6: E8 95 9B 09 01     call        000000014146F040
  00000001403D54AB: 85 C0              test        eax,eax
  00000001403D54AD: 75 07              jne         00000001403D54B6
  00000001403D54AF: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001403D54B4: EB 10              jmp         00000001403D54C6
  00000001403D54B6: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001403D54BB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001403D54C0: FF 15 BA F1 A2 01  call        qword ptr [0000000141E04680h]
  00000001403D54C6: 48 83 C4 28        add         rsp,28h
  00000001403D54CA: C3                 ret

  Summary

     1E02000 .text
