Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014175F450: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  000000014175F455: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014175F45A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014175F45E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014175F463: 48 83 EC 38        sub         rsp,38h
  000000014175F467: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  000000014175F46C: 75 5C              jne         000000014175F4CA
  000000014175F46E: E8 0D 57 8A FE     call        0000000140004B80
  000000014175F473: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  000000014175F478: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  000000014175F47D: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  000000014175F481: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000014175F485: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  000000014175F48A: 74 10              je          000000014175F49C
  000000014175F48C: 83 7C 24 28 01     cmp         dword ptr [rsp+28h],1
  000000014175F491: 74 21              je          000000014175F4B4
  000000014175F493: 83 7C 24 28 02     cmp         dword ptr [rsp+28h],2
  000000014175F498: 74 26              je          000000014175F4C0
  000000014175F49A: EB 2E              jmp         000000014175F4CA
  000000014175F49C: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  000000014175F4A1: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014175F4A5: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014175F4A8: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014175F4AD: E8 4E 86 F5 FF     call        00000001416B7B00
  000000014175F4B2: EB 16              jmp         000000014175F4CA
  000000014175F4B4: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014175F4B9: E8 72 86 F5 FF     call        00000001416B7B30
  000000014175F4BE: EB 0A              jmp         000000014175F4CA
  000000014175F4C0: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014175F4C5: E8 96 86 F5 FF     call        00000001416B7B60
  000000014175F4CA: 48 83 C4 38        add         rsp,38h
  000000014175F4CE: C3                 ret

  Summary

     1E02000 .text
