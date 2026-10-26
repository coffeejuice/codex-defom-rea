Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140181C80: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  0000000140181C85: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140181C8A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140181C8F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140181C94: 48 83 EC 38        sub         rsp,38h
  0000000140181C98: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140181C9E: 74 4C              je          0000000140181CEC
  0000000140181CA0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140181CA5: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140181CA8: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140181CAD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140181CB2: FF 90 D8 01 00 00  call        qword ptr [rax+00000000000001D8h]
  0000000140181CB8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140181CBD: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140181CC3: 74 27              je          0000000140181CEC
  0000000140181CC5: 83 7C 24 58 01     cmp         dword ptr [rsp+58h],1
  0000000140181CCA: 75 11              jne         0000000140181CDD
  0000000140181CCC: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140181CD1: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140181CD6: E8 05 BF FE FF     call        000000014016DBE0
  0000000140181CDB: EB 0F              jmp         0000000140181CEC
  0000000140181CDD: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140181CE2: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140181CE7: E8 84 BF FE FF     call        000000014016DC70
  0000000140181CEC: 48 83 C4 38        add         rsp,38h
  0000000140181CF0: C3                 ret

  Summary

     1E02000 .text
