Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067047CB0: 4C 8B 01           mov         r8,qword ptr [rcx]
  0000000067047CB3: 49 83 38 00        cmp         qword ptr [r8],0
  0000000067047CB7: 75 2F              jne         0000000067047CE8
  0000000067047CB9: 8B 41 08           mov         eax,dword ptr [rcx+8]
  0000000067047CBC: 45 8B 48 20        mov         r9d,dword ptr [r8+20h]
  0000000067047CC0: 33 D2              xor         edx,edx
  0000000067047CC2: 41 F7 F1           div         eax,r9d
  0000000067047CC5: 49 8B 40 08        mov         rax,qword ptr [r8+8]
  0000000067047CC9: FF C2              inc         edx
  0000000067047CCB: 44 2B CA           sub         r9d,edx
  0000000067047CCE: 48 63 CA           movsxd      rcx,edx
  0000000067047CD1: 48 8D 04 C8        lea         rax,[rax+rcx*8]
  0000000067047CD5: 74 11              je          0000000067047CE8
  0000000067047CD7: 41 FF C9           dec         r9d
  0000000067047CDA: 4C 39 00           cmp         qword ptr [rax],r8
  0000000067047CDD: 75 0D              jne         0000000067047CEC
  0000000067047CDF: 48 83 C0 08        add         rax,8
  0000000067047CE3: 45 85 C9           test        r9d,r9d
  0000000067047CE6: 75 EF              jne         0000000067047CD7
  0000000067047CE8: 49 8B C0           mov         rax,r8
  0000000067047CEB: C3                 ret
  0000000067047CEC: 48 8B 00           mov         rax,qword ptr [rax]
  0000000067047CEF: C3                 ret
  0000000067047CF0: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067047CF3: 4C 8B C9           mov         r9,rcx
  0000000067047CF6: 4C 8B C1           mov         r8,rcx
  0000000067047CF9: 48 85 C0           test        rax,rax
  0000000067047CFC: 74 20              je          0000000067047D1E
  0000000067047CFE: 66 90              nop
  0000000067047D00: 4C 8B C0           mov         r8,rax
  0000000067047D03: 48 8B 00           mov         rax,qword ptr [rax]
  0000000067047D06: 48 85 C0           test        rax,rax
  0000000067047D09: 75 F5              jne         0000000067047D00
  0000000067047D0B: 49 3B C8           cmp         rcx,r8
  0000000067047D0E: 74 0E              je          0000000067047D1E
  0000000067047D10: 8B 41 08           mov         eax,dword ptr [rcx+8]
  0000000067047D13: 33 D2              xor         edx,edx
  0000000067047D15: 41 F7 70 20        div         eax,dword ptr [r8+20h]
  0000000067047D19: 44 8B D2           mov         r10d,edx
  0000000067047D1C: EB 07              jmp         0000000067047D25
  0000000067047D1E: 45 8B 50 20        mov         r10d,dword ptr [r8+20h]
  0000000067047D22: 41 FF CA           dec         r10d
  0000000067047D25: 49 8B 40 08        mov         rax,qword ptr [r8+8]
  0000000067047D29: 49 63 CA           movsxd      rcx,r10d
  0000000067047D2C: 48 8D 14 C8        lea         rdx,[rax+rcx*8]

  Summary

      1A7000 .text
