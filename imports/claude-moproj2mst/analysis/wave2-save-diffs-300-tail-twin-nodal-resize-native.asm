Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_PRE_64.exe

File Type: EXECUTABLE IMAGE

  0000000140180310: mov         dword ptr [rsp+18h],r8d
  0000000140180315: mov         dword ptr [rsp+10h],edx
  0000000140180319: mov         qword ptr [rsp+8],rcx
  000000014018031E: sub         rsp,38h
  0000000140180322: mov         rax,qword ptr [rsp+40h]
  0000000140180327: cmp         dword ptr [rax+000000000000012Ch],0
  000000014018032E: jg          0000000140180332
  0000000140180330: jmp         0000000140180382
  0000000140180332: cmp         dword ptr [rsp+50h],0
  0000000140180337: jge         000000014018033B
  0000000140180339: jmp         0000000140180382
  000000014018033B: mov         rax,qword ptr [rsp+40h]
  0000000140180340: cmp         qword ptr [rax+0000000000000130h],0
  0000000140180348: jne         000000014018034C
  000000014018034A: jmp         0000000140180382
  000000014018034C: mov         rax,qword ptr [rsp+40h]
  0000000140180351: mov         rax,qword ptr [rax+0000000000000130h]
  0000000140180358: add         rax,15F8h
  000000014018035E: mov         qword ptr [rsp+20h],rax
  0000000140180363: mov         r9d,dword ptr [rsp+48h]
  0000000140180368: mov         r8,qword ptr [rsp+20h]
  000000014018036D: mov         rax,qword ptr [rsp+40h]
  0000000140180372: mov         edx,dword ptr [rax+000000000000012Ch]
  0000000140180378: mov         rcx,qword ptr [rsp+40h]
  000000014018037D: call        0000000140329E60
  0000000140180382: add         rsp,38h
  0000000140180386: ret
  0000000140180387: int         3

  Summary

      504000 .text
