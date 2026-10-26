Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067129F80: 4C 8B C9           mov         r9,rcx
  0000000067129F83: 48 85 D2           test        rdx,rdx
  0000000067129F86: 75 03              jne         0000000067129F8B
  0000000067129F88: 33 C0              xor         eax,eax
  0000000067129F8A: C3                 ret
  0000000067129F8B: 4C 8D 05 BE 1F 11  lea         r8,[000000006723BF50h]
                    00
  0000000067129F92: 4C 2B C2           sub         r8,rdx
  0000000067129F95: 66 66 66 0F 1F 84  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00 00
  0000000067129FA0: 0F B6 0A           movzx       ecx,byte ptr [rdx]
  0000000067129FA3: 42 0F B6 04 02     movzx       eax,byte ptr [rdx+r8]
  0000000067129FA8: 2B C8              sub         ecx,eax
  0000000067129FAA: 75 07              jne         0000000067129FB3
  0000000067129FAC: 48 FF C2           inc         rdx
  0000000067129FAF: 85 C0              test        eax,eax
  0000000067129FB1: 75 ED              jne         0000000067129FA0
  0000000067129FB3: 33 C0              xor         eax,eax
  0000000067129FB5: 85 C9              test        ecx,ecx
  0000000067129FB7: 49 0F 44 C1        cmove       rax,r9
  0000000067129FBB: C3                 ret
  0000000067129FBC: CC                 int         3
  0000000067129FBD: CC                 int         3
  0000000067129FBE: CC                 int         3
  0000000067129FBF: CC                 int         3

  Summary

      1A7000 .text
