Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067069C80: 0F B7 01           movzx       eax,word ptr [rcx]
  0000000067069C83: 66 83 F8 09        cmp         ax,9
  0000000067069C87: 72 09              jb          0000000067069C92
  0000000067069C89: 66 83 F8 0D        cmp         ax,0Dh
  0000000067069C8D: 77 03              ja          0000000067069C92
  0000000067069C8F: B0 01              mov         al,1
  0000000067069C91: C3                 ret
  0000000067069C92: 0F B7 D0           movzx       edx,ax
  0000000067069C95: 4C 8D 05 64 63 F9  lea         r8,[0000000067000000h]
                    FF
  0000000067069C9C: 48 8B C2           mov         rax,rdx
  0000000067069C9F: 83 E2 1F           and         edx,1Fh
  0000000067069CA2: 48 C1 E8 05        shr         rax,5
  0000000067069CA6: 41 0F B7 8C 40 50  movzx       ecx,word ptr [r8+rax*2+00000000001F4050h]
                    40 1F 00
  0000000067069CAF: 48 03 CA           add         rcx,rdx
  0000000067069CB2: 41 0F B7 84 48 50  movzx       eax,word ptr [r8+rcx*2+00000000001F4050h]
                    40 1F 00
  0000000067069CBB: 48 8D 0C 80        lea         rcx,[rax+rax*4]
  0000000067069CBF: B8 01 00 00 00     mov         eax,1
  0000000067069CC4: 41 0F B6 8C 88 B0  movzx       ecx,byte ptr [r8+rcx*4+00000000001FF7B0h]
                    F7 1F 00
  0000000067069CCD: D3 E0              shl         eax,cl
  0000000067069CCF: A9 80 03 00 00     test        eax,380h
  0000000067069CD4: 0F 95 C0           setne       al
  0000000067069CD7: C3                 ret
  0000000067069CD8: CC                 int         3
  0000000067069CD9: CC                 int         3
  0000000067069CDA: CC                 int         3
  0000000067069CDB: CC                 int         3
  0000000067069CDC: CC                 int         3
  0000000067069CDD: CC                 int         3
  0000000067069CDE: CC                 int         3
  0000000067069CDF: CC                 int         3
  0000000067069CE0: 0F B7 11           movzx       edx,word ptr [rcx]
  0000000067069CE3: 4C 8D 05 16 63 F9  lea         r8,[0000000067000000h]
                    FF
  0000000067069CEA: 48 8B C2           mov         rax,rdx
  0000000067069CED: 83 E2 1F           and         edx,1Fh
  0000000067069CF0: 48 C1 E8 05        shr         rax,5
  0000000067069CF4: 41 0F B7 8C 40 50  movzx       ecx,word ptr [r8+rax*2+00000000001F4050h]
                    40 1F 00
  0000000067069CFD: 48 03 CA           add         rcx,rdx
  0000000067069D00: 41 0F B7 84 48 50  movzx       eax,word ptr [r8+rcx*2+00000000001F4050h]
                    40 1F 00
  0000000067069D09: 48 8D 0C 80        lea         rcx,[rax+rax*4]
  0000000067069D0D: B8
  0000000067069D0E: 01 00              add         dword ptr [rax],eax

  Summary

      1A7000 .text
