Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014001D440: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014001D445: 48 83 EC 48        sub         rsp,48h
  000000014001D449: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014001D44E: E8 BD 00 00 00     call        000000014001D510
  000000014001D453: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014001D458: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014001D45E: 75 04              jne         000000014001D464
  000000014001D460: 33 C0              xor         eax,eax
  000000014001D462: EB 2F              jmp         000000014001D493
  000000014001D464: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014001D469: E8 22 9F 08 00     call        00000001400A7390
  000000014001D46E: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014001D473: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014001D479: 75 04              jne         000000014001D47F
  000000014001D47B: 33 C0              xor         eax,eax
  000000014001D47D: EB 14              jmp         000000014001D493
  000000014001D47F: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014001D484: E8 47 C8 09 00     call        00000001400B9CD0
  000000014001D489: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014001D48E: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014001D493: 48 83 C4 48        add         rsp,48h
  000000014001D497: C3                 ret

  Summary

     1E02000 .text
