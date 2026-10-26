Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140047DC0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140047DC5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140047DCA: 48 83 EC 38        sub         rsp,38h
  0000000140047DCE: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140047DD3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140047DD8: E8 73 FB FF FF     call        0000000140047950
  0000000140047DDD: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140047DE2: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140047DE8: 75 04              jne         0000000140047DEE
  0000000140047DEA: 33 C0              xor         eax,eax
  0000000140047DEC: EB 0A              jmp         0000000140047DF8
  0000000140047DEE: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140047DF3: E8 98 E1 FF FF     call        0000000140045F90
  0000000140047DF8: 48 83 C4 38        add         rsp,38h
  0000000140047DFC: C3                 ret

  Summary

     1E02000 .text
