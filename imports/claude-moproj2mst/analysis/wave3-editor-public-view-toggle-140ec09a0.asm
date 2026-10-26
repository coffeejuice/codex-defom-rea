Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EC09A0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140EC09A5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EC09AA: 48 83 EC 38        sub         rsp,38h
  0000000140EC09AE: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  0000000140EC09B3: 48 8D 54 24 24     lea         rdx,[rsp+24h]
  0000000140EC09B8: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140EC09BD: E8 4E 58 FF FF     call        0000000140EB6210
  0000000140EC09C2: 83 7C 24 24 00     cmp         dword ptr [rsp+24h],0
  0000000140EC09C7: 7E 1D              jle         0000000140EC09E6
  0000000140EC09C9: 83 7C 24 20 00     cmp         dword ptr [rsp+20h],0
  0000000140EC09CE: 7E 16              jle         0000000140EC09E6
  0000000140EC09D0: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140EC09D5: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000140EC09D8: 85 C0              test        eax,eax
  0000000140EC09DA: 74 0A              je          0000000140EC09E6
  0000000140EC09DC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140EC09E1: E8 DA 8B 01 00     call        0000000140ED95C0
  0000000140EC09E6: 48 83 C4 38        add         rsp,38h
  0000000140EC09EA: C3                 ret

  Summary

     1E02000 .text
