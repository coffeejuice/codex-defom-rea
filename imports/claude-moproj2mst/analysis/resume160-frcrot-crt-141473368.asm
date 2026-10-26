Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141473368: 40 53              push        rbx
  000000014147336A: 48 83 EC 20        sub         rsp,20h
  000000014147336E: 83 64 24 40 00     and         dword ptr [rsp+40h],0
  0000000141473373: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  0000000141473378: E8 E7 2E 01 00     call        0000000141486264
  000000014147337D: 48 8B D8           mov         rbx,rax
  0000000141473380: 48 85 C0           test        rax,rax
  0000000141473383: 75 1B              jne         00000001414733A0
  0000000141473385: 39 44 24 40        cmp         dword ptr [rsp+40h],eax
  0000000141473389: 74 15              je          00000001414733A0
  000000014147338B: E8 A4 0D 00 00     call        0000000141474134
  0000000141473390: 48 85 C0           test        rax,rax
  0000000141473393: 74 0B              je          00000001414733A0
  0000000141473395: E8 9A 0D 00 00     call        0000000141474134
  000000014147339A: 8B 4C 24 40        mov         ecx,dword ptr [rsp+40h]
  000000014147339E: 89 08              mov         dword ptr [rax],ecx
  00000001414733A0: 48 8B C3           mov         rax,rbx
  00000001414733A3: 48 83 C4 20        add         rsp,20h
  00000001414733A7: 5B                 pop         rbx
  00000001414733A8: C3                 ret

  Summary

     1E02000 .text
