Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C9C1D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C9C1D5: 48 83 EC 38        sub         rsp,38h
  0000000140C9C1D9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C9C1DE: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C9C1E1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C9C1E6: FF 90 78 07 00 00  call        qword ptr [rax+0000000000000778h]
  0000000140C9C1EC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C9C1F1: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140C9C1F7: 74 10              je          0000000140C9C209
  0000000140C9C1F9: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C9C1FE: 48 83 C0 10        add         rax,10h
  0000000140C9C202: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C9C207: EB 09              jmp         0000000140C9C212
  0000000140C9C209: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140C9C212: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C9C217: 48 83 C4 38        add         rsp,38h
  0000000140C9C21B: C3                 ret

  Summary

     1E02000 .text
