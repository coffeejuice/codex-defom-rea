Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D3C0C0: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140D3C0C4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D3C0C9: 48 83 EC 28        sub         rsp,28h
  0000000140D3C0CD: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140D3C0D2: 85 C0              test        eax,eax
  0000000140D3C0D4: 74 12              je          0000000140D3C0E8
  0000000140D3C0D6: BA 01 00 00 00     mov         edx,1
  0000000140D3C0DB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D3C0E0: FF 15 32 F7 0C 01  call        qword ptr [0000000141E0B818h]
  0000000140D3C0E6: EB 0D              jmp         0000000140D3C0F5
  0000000140D3C0E8: 33 D2              xor         edx,edx
  0000000140D3C0EA: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D3C0EF: FF 15 23 F7 0C 01  call        qword ptr [0000000141E0B818h]
  0000000140D3C0F5: 48 83 C4 28        add         rsp,28h
  0000000140D3C0F9: C3                 ret

  Summary

     1E02000 .text
