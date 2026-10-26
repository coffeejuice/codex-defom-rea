Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400DB070: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400DB075: 48 83 EC 38        sub         rsp,38h
  00000001400DB079: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400DB07E: 48 83 C1 08        add         rcx,8
  00000001400DB082: E8 C9 D9 FF FF     call        00000001400D8A50
  00000001400DB087: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  00000001400DB08C: 74 19              je          00000001400DB0A7
  00000001400DB08E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400DB093: 48 83 C1 08        add         rcx,8
  00000001400DB097: E8 B4 D9 FF FF     call        00000001400D8A50
  00000001400DB09C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001400DB0A0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400DB0A5: EB 0C              jmp         00000001400DB0B3
  00000001400DB0A7: 48 8D 05 D2 FD 08  lea         rax,[000000014216AE80h]
                    02
  00000001400DB0AE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400DB0B3: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400DB0B8: 48 83 C4 38        add         rsp,38h
  00000001400DB0BC: C3                 ret

  Summary

     1E02000 .text
