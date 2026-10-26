Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DABCB0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140DABCB4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DABCB9: 48 83 EC 38        sub         rsp,38h
  0000000140DABCBD: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  0000000140DABCC1: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140DABCC5: 83 7C 24 20 00     cmp         dword ptr [rsp+20h],0
  0000000140DABCCA: 7C 11              jl          0000000140DABCDD
  0000000140DABCCC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DABCD1: FF 15 29 E5 05 01  call        qword ptr [0000000141E0A200h]
  0000000140DABCD7: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  0000000140DABCDB: 7C 04              jl          0000000140DABCE1
  0000000140DABCDD: 33 C0              xor         eax,eax
  0000000140DABCDF: EB 0F              jmp         0000000140DABCF0
  0000000140DABCE1: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140DABCE5: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DABCEA: FF 15 08 E5 05 01  call        qword ptr [0000000141E0A1F8h]
  0000000140DABCF0: 48 83 C4 38        add         rsp,38h
  0000000140DABCF4: C3                 ret

  Summary

     1E02000 .text
