Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065477E08: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  0000000065477E0D: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  0000000065477E12: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  0000000065477E16: 80 BF 91 02 00 00  cmp         byte ptr [rdi+0000000000000291h],0
                    00
  0000000065477E1D: 4C 8B E2           mov         r12,rdx
  0000000065477E20: 48 8B F1           mov         rsi,rcx
  0000000065477E23: 74 1D              je          0000000065477E42
  0000000065477E25: 48 8B 8F 58 02 00  mov         rcx,qword ptr [rdi+0000000000000258h]
                    00
  0000000065477E2C: BA 00 04 00 00     mov         edx,400h
  0000000065477E31: E8 6A 52 F2 FF     call        000000006539D0A0
  0000000065477E36: 48 85 C0           test        rax,rax
  0000000065477E39: 74 07              je          0000000065477E42
  0000000065477E3B: C6 87 91 02 00 00  mov         byte ptr [rdi+0000000000000291h],0
                    00
  0000000065477E42: 48 8B AF 80 02 00  mov         rbp,qword ptr [rdi+0000000000000280h]
                    00
  0000000065477E49: 48 85 ED           test        rbp,rbp
  0000000065477E4C: 74 4B              je          0000000065477E99
  0000000065477E4E: 48 8B D5           mov         rdx,rbp
  0000000065477E51: 48 8B CE           mov         rcx,rsi

  Summary

      695000 .text
