Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141E00764: 48 83 EC 28        sub         rsp,28h
  0000000141E00768: 80 3D A1 0D 90 02  cmp         byte ptr [0000000144701510h],0
                    00
  0000000141E0076F: 48 8D 05 AA A1 11  lea         rax,[0000000142F1A920h]
                    01
  0000000141E00776: 48 89 05 83 0D 90  mov         qword ptr [0000000144701500h],rax
                    02
  0000000141E0077D: 74 12              je          0000000141E00791
  0000000141E0077F: 48 8B 0D 82 0D 90  mov         rcx,qword ptr [0000000144701508h]
                    02
  0000000141E00786: 48 85 C9           test        rcx,rcx
  0000000141E00789: 74 06              je          0000000141E00791
  0000000141E0078B: FF 15 07 2D 00 00  call        qword ptr [0000000141E03498h]
  0000000141E00791: 48 83 C4 28        add         rsp,28h
  0000000141E00795: C3                 ret

  Summary

     1E02000 .text
