Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140142E70: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140142E75: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140142E7A: 48 83 EC 38        sub         rsp,38h
  0000000140142E7E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140142E83: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140142E88: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140142E8B: 48 39 01           cmp         qword ptr [rcx],rax
  0000000140142E8E: 74 68              je          0000000140142EF8
  0000000140142E90: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140142E95: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140142E98: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140142E9D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140142EA2: FF 15 50 14 CC 01  call        qword ptr [0000000141E042F8h]
  0000000140142EA8: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140142EAD: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140142EB0: FF 15 32 14 CC 01  call        qword ptr [0000000141E042E8h]
  0000000140142EB6: 0F B6 C0           movzx       eax,al
  0000000140142EB9: 85 C0              test        eax,eax
  0000000140142EBB: 75 12              jne         0000000140142ECF
  0000000140142EBD: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140142EC2: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140142EC5: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140142ECA: E8 F1 AC F6 FF     call        00000001400ADBC0
  0000000140142ECF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140142ED4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140142ED9: 48 89 01           mov         qword ptr [rcx],rax
  0000000140142EDC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140142EE1: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140142EE4: 8B 40 10           mov         eax,dword ptr [rax+10h]
  0000000140142EE7: 83 E0 01           and         eax,1
  0000000140142EEA: 85 C0              test        eax,eax
  0000000140142EEC: 75 0A              jne         0000000140142EF8
  0000000140142EEE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140142EF3: E8 A8 A3 F6 FF     call        00000001400AD2A0
  0000000140142EF8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140142EFD: 48 83 C4 38        add         rsp,38h
  0000000140142F01: C3                 ret

  Summary

     1E02000 .text
