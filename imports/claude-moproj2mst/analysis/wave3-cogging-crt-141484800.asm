Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141484800: 89 4C 24 08        mov         dword ptr [rsp+8],ecx
  0000000141484804: 48 83 EC 28        sub         rsp,28h
  0000000141484808: 45 33 C0           xor         r8d,r8d
  000000014148480B: BA 00 10 00 00     mov         edx,1000h
  0000000141484810: 33 C9              xor         ecx,ecx
  0000000141484812: FF 15 78 EC 97 00  call        qword ptr [0000000141E03490h]
  0000000141484818: 48 89 05 F1 BF 27  mov         qword ptr [0000000144700810h],rax
                    03
  000000014148481F: 48 85 C0           test        rax,rax
  0000000141484822: 74 23              je          0000000141484847
  0000000141484824: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  0000000141484829: 41 B9 04 00 00 00  mov         r9d,4
  000000014148482F: 33 D2              xor         edx,edx
  0000000141484831: 48 8B C8           mov         rcx,rax
  0000000141484834: C7 44 24 30 02 00  mov         dword ptr [rsp+30h],2
                    00 00
  000000014148483C: FF 15 46 EC 97 00  call        qword ptr [0000000141E03488h]
  0000000141484842: B8 01 00 00 00     mov         eax,1
  0000000141484847: 48 83 C4 28        add         rsp,28h
  000000014148484B: C3                 ret

  Summary

     1E02000 .text
