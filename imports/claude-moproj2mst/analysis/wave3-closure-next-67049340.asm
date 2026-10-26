Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067049340: 0F B7 01           movzx       eax,word ptr [rcx]
  0000000067049343: B9 87 01 00 00     mov         ecx,187h
  0000000067049348: 66 3B C1           cmp         ax,cx
  000000006704934B: 75 08              jne         0000000067049355
  000000006704934D: 48 8B 05 E4 A9 2A  mov         rax,qword ptr [00000000672F3D38h]
                    00
  0000000067049354: C3                 ret
  0000000067049355: 0F B7 C0           movzx       eax,ax
  0000000067049358: 48 8D 0D E1 8C 16  lea         rcx,[00000000671B2040h]
                    00
  000000006704935F: 48 8D 04 C0        lea         rax,[rax+rax*8]
  0000000067049363: 48 C1 E0 04        shl         rax,4
  0000000067049367: 48 03 C1           add         rax,rcx
  000000006704936A: C3                 ret
  000000006704936B: CC                 int         3
  000000006704936C: CC                 int         3
  000000006704936D: CC                 int         3
  000000006704936E: CC                 int         3
  000000006704936F: CC                 int         3
  0000000067049370: 0F B7 01           movzx       eax,word ptr [rcx]
  0000000067049373: B9 87 01 00 00     mov         ecx,187h
  0000000067049378: 66 3B C1           cmp         ax,cx
  000000006704937B: 75 0B              jne         0000000067049388
  000000006704937D: 48 8B 05 B4 A9 2A  mov         rax,qword ptr [00000000672F3D38h]
                    00
  0000000067049384: 0F B7 00           movzx       eax,word ptr [rax]
  0000000067049387: C3                 ret
  0000000067049388: 0F B7 C0           movzx       eax,ax
  000000006704938B: 48 8D 0C C0        lea         rcx,[rax+rax*8]
  000000006704938F: 48

  Summary

      1A7000 .text
