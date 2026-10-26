Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141412510: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000141412514: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141412519: 48 83 EC 28        sub         rsp,28h
  000000014141251D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141412522: E8 39 FE FF FF     call        0000000141412360
  0000000141412527: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014141252B: 83 E0 01           and         eax,1
  000000014141252E: 85 C0              test        eax,eax
  0000000141412530: 74 0A              je          000000014141253C
  0000000141412532: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141412537: E8 2C D7 0C 00     call        00000001414DFC68
  000000014141253C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000141412541: 48 83 C4 28        add         rsp,28h
  0000000141412545: C3                 ret

  Summary

     1E02000 .text
