Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414813EC: 48 FF 25 35 20 98  jmp         qword ptr [0000000141E03428h]
                    00
  00000001414813F3: CC                 int         3
  00000001414813F4: 48 FF 25 35 20 98  jmp         qword ptr [0000000141E03430h]
                    00
  00000001414813FB: CC                 int         3
  00000001414813FC: 8B 05 8E D5 6D 02  mov         eax,dword ptr [0000000143B5E990h]
  0000000141481402: C3                 ret
  0000000141481403: CC                 int         3
  0000000141481404: 48 FF 25 2D 20 98  jmp         qword ptr [0000000141E03438h]
                    00
  000000014148140B: CC                 int         3
  000000014148140C: 48 83 EC 28        sub         rsp,28h
  0000000141481410: 8B 0D 7A D5 6D 02  mov         ecx,dword ptr [0000000143B5E990h]
  0000000141481416: 83 F9 FF           cmp         ecx,0FFFFFFFFh
  0000000141481419: 74 0D              je          0000000141481428
  000000014148141B: FF 15 1F 20 98 00  call        qword ptr [0000000141E03440h]
  0000000141481421: 83 0D 68 D5 6D 02  or          dword ptr [0000000143B5E990h],0FFFFFFFFh
                    FF
  0000000141481428: 48 83 C4 28        add         rsp,28h

  Summary

     1E02000 .text
