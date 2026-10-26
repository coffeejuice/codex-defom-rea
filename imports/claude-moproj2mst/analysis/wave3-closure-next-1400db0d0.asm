Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400DB0D0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400DB0D5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400DB0DA: 48 83 EC 28        sub         rsp,28h
  00000001400DB0DE: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001400DB0E4: 75 04              jne         00000001400DB0EA
  00000001400DB0E6: 33 C0              xor         eax,eax
  00000001400DB0E8: EB 2B              jmp         00000001400DB115
  00000001400DB0EA: 48 8D 15 5F FC 08  lea         rdx,[000000014216AD50h]
                    02
  00000001400DB0F1: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001400DB0F6: E8 45 3F 39 01     call        000000014146F040
  00000001400DB0FB: 85 C0              test        eax,eax
  00000001400DB0FD: 75 07              jne         00000001400DB106
  00000001400DB0FF: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001400DB104: EB 0F              jmp         00000001400DB115
  00000001400DB106: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001400DB10B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400DB110: E8 4B 41 C7 00     call        0000000140D4F260
  00000001400DB115: 48 83 C4 28        add         rsp,28h
  00000001400DB119: C3                 ret

  Summary

     1E02000 .text
