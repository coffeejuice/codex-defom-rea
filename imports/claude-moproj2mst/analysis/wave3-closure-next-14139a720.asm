Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014139A720: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014139A725: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014139A72A: 48 83 EC 38        sub         rsp,38h
  000000014139A72E: 33 C0              xor         eax,eax
  000000014139A730: 85 C0              test        eax,eax
  000000014139A732: 74 19              je          000000014139A74D
  000000014139A734: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014139A739: 48 8B 00           mov         rax,qword ptr [rax]
  000000014139A73C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014139A741: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014139A746: E8 1D 55 14 00     call        00000001414DFC68
  000000014139A74B: EB 02              jmp         000000014139A74F
  000000014139A74D: 33 C0              xor         eax,eax
  000000014139A74F: 48 83 C4 38        add         rsp,38h
  000000014139A753: C3                 ret

  Summary

     1E02000 .text
