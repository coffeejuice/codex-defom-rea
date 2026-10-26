Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400DCB20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400DCB25: 48 83 EC 38        sub         rsp,38h
  00000001400DCB29: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400DCB2E: 48 83 C1 08        add         rcx,8
  00000001400DCB32: E8 19 BF FF FF     call        00000001400D8A50
  00000001400DCB37: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  00000001400DCB3C: 74 19              je          00000001400DCB57
  00000001400DCB3E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400DCB43: 48 83 C1 08        add         rcx,8
  00000001400DCB47: E8 04 BF FF FF     call        00000001400D8A50
  00000001400DCB4C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001400DCB50: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400DCB55: EB 0C              jmp         00000001400DCB63
  00000001400DCB57: 48 8D 05 92 3A 09  lea         rax,[00000001421705F0h]
                    02
  00000001400DCB5E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400DCB63: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400DCB68: 48 83 C4 38        add         rsp,38h
  00000001400DCB6C: C3                 ret

  Summary

     1E02000 .text
