Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140B3F0E0: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  0000000140B3F0E5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140B3F0EA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140B3F0EE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140B3F0F3: 48 83 EC 28        sub         rsp,28h
  0000000140B3F0F7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140B3F0FC: E8 DF 50 EA FF     call        00000001409E41E0
  0000000140B3F101: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  0000000140B3F105: 75 39              jne         0000000140B3F140
  0000000140B3F107: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140B3F10B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140B3F110: E8 5B 51 EA FF     call        00000001409E4270
  0000000140B3F115: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140B3F11A: E8 C1 50 EA FF     call        00000001409E41E0
  0000000140B3F11F: 48 63 C8           movsxd      rcx,eax
  0000000140B3F122: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140B3F127: 8B 14 88           mov         edx,dword ptr [rax+rcx*4]
  0000000140B3F12A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140B3F12F: E8 9C 51 EA FF     call        00000001409E42D0
  0000000140B3F134: 33 D2              xor         edx,edx
  0000000140B3F136: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140B3F13B: E8 80 55 EA FF     call        00000001409E46C0
  0000000140B3F140: 48 83 C4 28        add         rsp,28h
  0000000140B3F144: C3                 ret

  Summary

     1E02000 .text
