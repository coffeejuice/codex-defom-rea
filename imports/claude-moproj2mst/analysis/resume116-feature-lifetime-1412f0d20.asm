Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F0D20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001412F0D25: 53                 push        rbx
  00000001412F0D26: 48 83 EC 30        sub         rsp,30h
  00000001412F0D2A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412F0D33: 48 8B D9           mov         rbx,rcx
  00000001412F0D36: E8 D5 5B FF FF     call        00000001412E6910
  00000001412F0D3B: 85 C0              test        eax,eax
  00000001412F0D3D: 74 33              je          00000001412F0D72
  00000001412F0D3F: 48 83 B9 50 14 00  cmp         qword ptr [rcx+0000000000001450h],0
                    00 00
  00000001412F0D47: 74 29              je          00000001412F0D72
  00000001412F0D49: 8B 91 50 14 00 00  mov         edx,dword ptr [rcx+0000000000001450h]
  00000001412F0D4F: 48 8D 0D 3A F1 3F  lea         rcx,[00000001446EFE90h]
                    03
  00000001412F0D56: E8 55 44 00 00     call        00000001412F51B0
  00000001412F0D5B: 48 8B 8B 50 14 00  mov         rcx,qword ptr [rbx+0000000000001450h]
                    00
  00000001412F0D62: E8 15 0E 18 00     call        0000000141471B7C
  00000001412F0D67: 48 C7 83 50 14 00  mov         qword ptr [rbx+0000000000001450h],0
                    00 00 00 00 00
  00000001412F0D72: 48 81 C3 58 14 00  add         rbx,1458h
                    00
  00000001412F0D79: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000001412F0D7E: 48 8B CB           mov         rcx,rbx
  00000001412F0D81: E8 DA FA FF FF     call        00000001412F0860
  00000001412F0D86: 90                 nop
  00000001412F0D87: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000001412F0D8A: 48 83 C4 30        add         rsp,30h
  00000001412F0D8E: 5B                 pop         rbx
  00000001412F0D8F: E9 D4 EE 1E 00     jmp         00000001414DFC68

  Summary

     1E02000 .text
