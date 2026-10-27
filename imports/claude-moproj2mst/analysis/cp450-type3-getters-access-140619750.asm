Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140619750: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140619755: 48 83 EC 18        sub         rsp,18h
  0000000140619759: 33 C0              xor         eax,eax
  000000014061975B: 83 F8 01           cmp         eax,1
  000000014061975E: 74 0E              je          000000014061976E
  0000000140619760: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140619765: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140619768: 48 89 04 24        mov         qword ptr [rsp],rax
  000000014061976C: EB 09              jmp         0000000140619777
  000000014061976E: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140619773: 48 89 04 24        mov         qword ptr [rsp],rax
  0000000140619777: 48 8B 04 24        mov         rax,qword ptr [rsp]
  000000014061977B: 48 83 C4 18        add         rsp,18h
  000000014061977F: C3                 ret

  Summary

     1E02000 .text
