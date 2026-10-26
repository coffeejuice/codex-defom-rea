Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DD9380: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DD9385: 48 83 EC 38        sub         rsp,38h
  0000000140DD9389: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DD938E: 48 83 C1 08        add         rcx,8
  0000000140DD9392: E8 B9 F6 2F FF     call        00000001400D8A50
  0000000140DD9397: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  0000000140DD939C: 74 19              je          0000000140DD93B7
  0000000140DD939E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DD93A3: 48 83 C1 08        add         rcx,8
  0000000140DD93A7: E8 A4 F6 2F FF     call        00000001400D8A50
  0000000140DD93AC: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140DD93B0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DD93B5: EB 0C              jmp         0000000140DD93C3
  0000000140DD93B7: 48 8D 05 52 F3 8D  lea         rax,[00000001446B8710h]
                    03
  0000000140DD93BE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DD93C3: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140DD93C8: 48 83 C4 38        add         rsp,38h
  0000000140DD93CC: C3                 ret

  Summary

     1E02000 .text
