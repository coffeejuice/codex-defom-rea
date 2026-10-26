Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014147EAB8: 85 C9              test        ecx,ecx
  000000014147EABA: 74 30              je          000000014147EAEC
  000000014147EABC: 53                 push        rbx
  000000014147EABD: 48 83 EC 20        sub         rsp,20h
  000000014147EAC1: 0F BA 62 18 0C     bt          dword ptr [rdx+18h],0Ch
  000000014147EAC6: 48 8B DA           mov         rbx,rdx
  000000014147EAC9: 73 1C              jae         000000014147EAE7
  000000014147EACB: 48 8B CA           mov         rcx,rdx
  000000014147EACE: E8 65 80 FF FF     call        0000000141476B38
  000000014147EAD3: 81 63 18 FF EE FF  and         dword ptr [rbx+18h],0FFFFEEFFh
                    FF
  000000014147EADA: 83 63 24 00        and         dword ptr [rbx+24h],0
  000000014147EADE: 48 83 23 00        and         qword ptr [rbx],0
  000000014147EAE2: 48 83 63 10 00     and         qword ptr [rbx+10h],0
  000000014147EAE7: 48 83 C4 20        add         rsp,20h
  000000014147EAEB: 5B                 pop         rbx
  000000014147EAEC: C3                 ret

  Summary

     1E02000 .text
