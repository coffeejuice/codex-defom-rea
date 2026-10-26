Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411E4F00: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411E4F05: 48 83 EC 38        sub         rsp,38h
  00000001411E4F09: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411E4F0E: 48 83 C1 08        add         rcx,8
  00000001411E4F12: E8 39 3B EF FE     call        00000001400D8A50
  00000001411E4F17: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  00000001411E4F1C: 74 19              je          00000001411E4F37
  00000001411E4F1E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411E4F23: 48 83 C1 08        add         rcx,8
  00000001411E4F27: E8 24 3B EF FE     call        00000001400D8A50
  00000001411E4F2C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001411E4F30: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411E4F35: EB 0C              jmp         00000001411E4F43
  00000001411E4F37: 48 8D 05 02 97 AF  lea         rax,[0000000142CDE640h]
                    01
  00000001411E4F3E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411E4F43: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001411E4F48: 48 83 C4 38        add         rsp,38h
  00000001411E4F4C: C3                 ret

  Summary

     1E02000 .text
