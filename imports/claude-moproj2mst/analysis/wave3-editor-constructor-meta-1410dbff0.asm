Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001410DBFF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001410DBFF5: 48 83 EC 38        sub         rsp,38h
  00000001410DBFF9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001410DBFFE: 48 83 C1 08        add         rcx,8
  00000001410DC002: E8 49 CA FF FE     call        00000001400D8A50
  00000001410DC007: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  00000001410DC00C: 74 19              je          00000001410DC027
  00000001410DC00E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001410DC013: 48 83 C1 08        add         rcx,8
  00000001410DC017: E8 34 CA FF FE     call        00000001400D8A50
  00000001410DC01C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001410DC020: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001410DC025: EB 0C              jmp         00000001410DC033
  00000001410DC027: 48 8D 05 7A CC B3  lea         rax,[0000000142C18CA8h]
                    01
  00000001410DC02E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001410DC033: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001410DC038: 48 83 C4 38        add         rsp,38h
  00000001410DC03C: C3                 ret

  Summary

     1E02000 .text
