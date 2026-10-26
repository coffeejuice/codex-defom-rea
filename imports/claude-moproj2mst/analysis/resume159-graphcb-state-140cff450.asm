Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CFF450: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  0000000140CFF455: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140CFF45A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140CFF45E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CFF463: 48 83 EC 38        sub         rsp,38h
  0000000140CFF467: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140CFF46C: E8 AF D4 00 00     call        0000000140D0C920
  0000000140CFF471: 0F B6 C0           movzx       eax,al
  0000000140CFF474: 85 C0              test        eax,eax
  0000000140CFF476: 74 3E              je          0000000140CFF4B6
  0000000140CFF478: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140CFF47D: E8 DE A4 FF FF     call        0000000140CF9960
  0000000140CFF482: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CFF487: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140CFF48C: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140CFF491: E8 3A 3E F8 FF     call        0000000140C832D0
  0000000140CFF496: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CFF49B: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140CFF4A1: 74 13              je          0000000140CFF4B6
  0000000140CFF4A3: 44 8B 44 24 58     mov         r8d,dword ptr [rsp+58h]
  0000000140CFF4A8: 8B 54 24 50        mov         edx,dword ptr [rsp+50h]
  0000000140CFF4AC: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140CFF4B1: E8 5A F7 4E 00     call        00000001411EEC10
  0000000140CFF4B6: 48 83 C4 38        add         rsp,38h
  0000000140CFF4BA: C3                 ret

  Summary

     1E02000 .text
