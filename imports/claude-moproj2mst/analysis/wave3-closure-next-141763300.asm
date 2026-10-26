Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141763300: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000141763305: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014176330A: 48 83 EC 28        sub         rsp,28h
  000000014176330E: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000141763314: 75 04              jne         000000014176331A
  0000000141763316: 33 C0              xor         eax,eax
  0000000141763318: EB 2C              jmp         0000000141763346
  000000014176331A: 48 8D 15 3F 2C 8D  lea         rdx,[0000000143035F60h]
                    01
  0000000141763321: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000141763326: E8 15 BD D0 FF     call        000000014146F040
  000000014176332B: 85 C0              test        eax,eax
  000000014176332D: 75 07              jne         0000000141763336
  000000014176332F: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000141763334: EB 10              jmp         0000000141763346
  0000000141763336: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  000000014176333B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000141763340: FF 15 22 87 6A 00  call        qword ptr [0000000141E0BA68h]
  0000000141763346: 48 83 C4 28        add         rsp,28h
  000000014176334A: C3                 ret

  Summary

     1E02000 .text
