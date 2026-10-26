Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D48940: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D48945: 48 83 EC 38        sub         rsp,38h
  0000000140D48949: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D4894E: 48 83 C1 08        add         rcx,8
  0000000140D48952: E8 F9 00 39 FF     call        00000001400D8A50
  0000000140D48957: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  0000000140D4895C: 74 19              je          0000000140D48977
  0000000140D4895E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D48963: 48 83 C1 08        add         rcx,8
  0000000140D48967: E8 E4 00 39 FF     call        00000001400D8A50
  0000000140D4896C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140D48970: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D48975: EB 0C              jmp         0000000140D48983
  0000000140D48977: 48 8D 05 42 C1 96  lea         rax,[00000001446B4AC0h]
                    03
  0000000140D4897E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D48983: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D48988: 48 83 C4 38        add         rsp,38h
  0000000140D4898C: C3                 ret

  Summary

     1E02000 .text
