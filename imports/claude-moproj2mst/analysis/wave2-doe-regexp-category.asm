Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067069FB0: 0F B7 11           movzx       edx,word ptr [rcx]
  0000000067069FB3: 4C 8D 05 46 60 F9  lea         r8,[0000000067000000h]
                    FF
  0000000067069FBA: 48 8B C2           mov         rax,rdx
  0000000067069FBD: 83 E2 1F           and         edx,1Fh
  0000000067069FC0: 48 C1 E8 05        shr         rax,5
  0000000067069FC4: 41 0F B7 8C 40 50  movzx       ecx,word ptr [r8+rax*2+00000000001F4050h]
                    40 1F 00
  0000000067069FCD: 48 03 CA           add         rcx,rdx
  0000000067069FD0: 41 0F B7 84 48 50  movzx       eax,word ptr [r8+rcx*2+00000000001F4050h]
                    40 1F 00
  0000000067069FD9: 48 8D 0C 80        lea         rcx,[rax+rax*4]
  0000000067069FDD: 41 0F B7 84 88 B0  movzx       eax,word ptr [r8+rcx*4+00000000001FF7B0h]
                    F7 1F 00
  0000000067069FE6: 0F B6 C0           movzx       eax,al
  0000000067069FE9: C3                 ret

  Summary

      1A7000 .text
