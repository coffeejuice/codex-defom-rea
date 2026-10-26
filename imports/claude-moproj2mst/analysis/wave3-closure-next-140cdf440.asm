Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDF440: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDF445: 48 83 EC 28        sub         rsp,28h
  0000000140CDF449: BA 01 00 00 00     mov         edx,1
  0000000140CDF44E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CDF453: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140CDF456: FF 15 7C 4E 12 01  call        qword ptr [0000000141E042D8h]
  0000000140CDF45C: 0F B6 C0           movzx       eax,al
  0000000140CDF45F: 85 C0              test        eax,eax
  0000000140CDF461: 74 0A              je          0000000140CDF46D
  0000000140CDF463: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CDF468: E8 03 EC F2 FF     call        0000000140C0E070
  0000000140CDF46D: 48 83 C4 28        add         rsp,28h
  0000000140CDF471: C3                 ret

  Summary

     1E02000 .text
