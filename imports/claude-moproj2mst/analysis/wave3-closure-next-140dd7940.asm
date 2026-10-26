Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DD7940: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DD7945: 48 83 EC 38        sub         rsp,38h
  0000000140DD7949: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DD794E: 48 83 C1 08        add         rcx,8
  0000000140DD7952: E8 F9 10 30 FF     call        00000001400D8A50
  0000000140DD7957: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  0000000140DD795C: 74 19              je          0000000140DD7977
  0000000140DD795E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DD7963: 48 83 C1 08        add         rcx,8
  0000000140DD7967: E8 E4 10 30 FF     call        00000001400D8A50
  0000000140DD796C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140DD7970: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DD7975: EB 0C              jmp         0000000140DD7983
  0000000140DD7977: 48 8D 05 C2 0A 8E  lea         rax,[00000001446B8440h]
                    03
  0000000140DD797E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DD7983: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140DD7988: 48 83 C4 38        add         rsp,38h
  0000000140DD798C: C3                 ret

  Summary

     1E02000 .text
