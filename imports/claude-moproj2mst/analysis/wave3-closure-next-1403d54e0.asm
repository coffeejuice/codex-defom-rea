Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001403D54E0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001403D54E5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001403D54EA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001403D54EE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001403D54F3: 48 83 EC 28        sub         rsp,28h
  00000001403D54F7: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  00000001403D54FC: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  00000001403D5501: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  00000001403D5505: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001403D550A: FF 15 68 F1 A2 01  call        qword ptr [0000000141E04678h]
  00000001403D5510: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000001403D5514: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  00000001403D5519: 7D 06              jge         00000001403D5521
  00000001403D551B: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001403D551F: EB 35              jmp         00000001403D5556
  00000001403D5521: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  00000001403D5526: 75 2A              jne         00000001403D5552
  00000001403D5528: 83 7C 24 40 0F     cmp         dword ptr [rsp+40h],0Fh
  00000001403D552D: 7D 18              jge         00000001403D5547
  00000001403D552F: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  00000001403D5534: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  00000001403D5539: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  00000001403D553D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001403D5542: E8 B9 FC FF FF     call        00000001403D5200
  00000001403D5547: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001403D554B: 83 E8 0F           sub         eax,0Fh
  00000001403D554E: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000001403D5552: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  00000001403D5556: 48 83 C4 28        add         rsp,28h
  00000001403D555A: C3                 ret

  Summary

     1E02000 .text
