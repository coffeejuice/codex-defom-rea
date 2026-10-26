Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140376B10: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140376B15: 48 83 EC 38        sub         rsp,38h
  0000000140376B19: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140376B1E: 48 83 C1 08        add         rcx,8
  0000000140376B22: E8 29 1F D6 FF     call        00000001400D8A50
  0000000140376B27: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  0000000140376B2C: 74 19              je          0000000140376B47
  0000000140376B2E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140376B33: 48 83 C1 08        add         rcx,8
  0000000140376B37: E8 14 1F D6 FF     call        00000001400D8A50
  0000000140376B3C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140376B40: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140376B45: EB 0C              jmp         0000000140376B53
  0000000140376B47: 48 8D 05 82 CD A1  lea         rax,[0000000143D938D0h]
                    03
  0000000140376B4E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140376B53: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140376B58: 48 83 C4 38        add         rsp,38h
  0000000140376B5C: C3                 ret

  Summary

     1E02000 .text
