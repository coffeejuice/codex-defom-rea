Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670703D0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670703D5: 57                 push        rdi
  00000000670703D6: 48 83 EC 20        sub         rsp,20h
  00000000670703DA: 48 8B 02           mov         rax,qword ptr [rdx]
  00000000670703DD: 48 8B FA           mov         rdi,rdx
  00000000670703E0: 48 8B D9           mov         rbx,rcx
  00000000670703E3: F0 83 00 01        lock add    dword ptr [rax],1
  00000000670703E7: 4C 8B 01           mov         r8,qword ptr [rcx]
  00000000670703EA: F0 41 83 00 FF     lock add    dword ptr [r8],0FFFFFFFFh
  00000000670703EF: 75 08              jne         00000000670703F9
  00000000670703F1: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000670703F4: E8 D7 FC FF FF     call        00000000670700D0
  00000000670703F9: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670703FC: 48 89 03           mov         qword ptr [rbx],rax
  00000000670703FF: 48 8B C3           mov         rax,rbx
  0000000067070402: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000067070407: 48 83 C4 20        add         rsp,20h
  000000006707040B: 5F                 pop         rdi
  000000006707040C: C3                 ret

  Summary

      1A7000 .text
