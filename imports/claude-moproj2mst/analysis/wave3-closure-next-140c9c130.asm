Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C9C130: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C9C135: 48 83 EC 28        sub         rsp,28h
  0000000140C9C139: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C9C13E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C9C141: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C9C146: FF 90 78 07 00 00  call        qword ptr [rax+0000000000000778h]
  0000000140C9C14C: 48 85 C0           test        rax,rax
  0000000140C9C14F: 75 04              jne         0000000140C9C155
  0000000140C9C151: 33 C0              xor         eax,eax
  0000000140C9C153: EB 1F              jmp         0000000140C9C174
  0000000140C9C155: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C9C15A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C9C15D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C9C162: FF 90 78 07 00 00  call        qword ptr [rax+0000000000000778h]
  0000000140C9C168: 48 8B C8           mov         rcx,rax
  0000000140C9C16B: 48 83 C1 10        add         rcx,10h
  0000000140C9C16F: E8 7C D3 05 00     call        0000000140CF94F0
  0000000140C9C174: 48 83 C4 28        add         rsp,28h
  0000000140C9C178: C3                 ret

  Summary

     1E02000 .text
