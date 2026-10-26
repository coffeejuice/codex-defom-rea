Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141207790: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141207795: 48 83 EC 38        sub         rsp,38h
  0000000141207799: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001412077A1: EB 0B              jmp         00000001412077AE
  00000001412077A3: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001412077A7: 83 C0 01           add         eax,1
  00000001412077AA: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001412077AE: 83 7C 24 20 2B     cmp         dword ptr [rsp+20h],2Bh
  00000001412077B3: 7D 17              jge         00000001412077CC
  00000001412077B5: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  00000001412077BA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001412077BF: C7 84 88 78 42 00  mov         dword ptr [rax+rcx*4+0000000000004278h],0
                    00 00 00 00 00
  00000001412077CA: EB D7              jmp         00000001412077A3
  00000001412077CC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412077D1: E8 0A D0 FF FF     call        00000001412047E0
  00000001412077D6: 48 83 C4 38        add         rsp,38h
  00000001412077DA: C3                 ret

  Summary

     1E02000 .text
