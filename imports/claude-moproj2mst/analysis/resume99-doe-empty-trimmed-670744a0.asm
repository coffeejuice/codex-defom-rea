Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670744A0: 40 57              push        rdi
  00000000670744A2: 48 83 EC 20        sub         rsp,20h
  00000000670744A6: 4C 8B 09           mov         r9,qword ptr [rcx]
  00000000670744A9: 45 33 DB           xor         r11d,r11d
  00000000670744AC: 48 8B FA           mov         rdi,rdx
  00000000670744AF: 45 8B 51 08        mov         r10d,dword ptr [r9+8]
  00000000670744B3: 44 89 5C 24 30     mov         dword ptr [rsp+30h],r11d
  00000000670744B8: 45 85 D2           test        r10d,r10d
  00000000670744BB: 75 11              jne         00000000670744CE
  00000000670744BD: 4C 89 0A           mov         qword ptr [rdx],r9
  00000000670744C0: F0 41 83 01 01     lock add    dword ptr [r9],1
  00000000670744C5: 48 8B C2           mov         rax,rdx
  00000000670744C8: 48 83 C4 20        add         rsp,20h
  00000000670744CC: 5F                 pop         rdi
  00000000670744CD: C3                 ret

  Summary

      1A7000 .text
