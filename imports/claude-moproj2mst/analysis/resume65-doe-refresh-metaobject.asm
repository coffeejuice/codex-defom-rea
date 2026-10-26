Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D9AF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D9AF5: 48 83 EC 38        sub         rsp,38h
  00000001400D9AF9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400D9AFE: 48 83 C1 08        add         rcx,8
  00000001400D9B02: E8 49 EF FF FF     call        00000001400D8A50
  00000001400D9B07: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  00000001400D9B0C: 74 19              je          00000001400D9B27
  00000001400D9B0E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400D9B13: 48 83 C1 08        add         rcx,8
  00000001400D9B17: E8 34 EF FF FF     call        00000001400D8A50
  00000001400D9B1C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001400D9B20: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400D9B25: EB 0C              jmp         00000001400D9B33
  00000001400D9B27: 48 8D 05 52 05 09  lea         rax,[000000014216A080h]
                    02
  00000001400D9B2E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400D9B33: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400D9B38: 48 83 C4 38        add         rsp,38h
  00000001400D9B3C: C3                 ret

  Summary

     1E02000 .text
