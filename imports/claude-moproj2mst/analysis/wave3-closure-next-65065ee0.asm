Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065065EE0: 40 53              push        rbx
  0000000065065EE2: 48 83 EC 20        sub         rsp,20h
  0000000065065EE6: 48 8B D9           mov         rbx,rcx
  0000000065065EE9: E8 D2 8C FF FF     call        000000006505EBC0
  0000000065065EEE: 48 83 BB D8 00 00  cmp         qword ptr [rbx+00000000000000D8h],0
                    00 00
  0000000065065EF6: 74 27              je          0000000065065F1F
  0000000065065EF8: 8B 83 D8 01 00 00  mov         eax,dword ptr [rbx+00000000000001D8h]
  0000000065065EFE: 0F BA E0 11        bt          eax,11h
  0000000065065F02: 0F 83 E9 00 00 00  jae         0000000065065FF1
  0000000065065F08: 0F BA F0 11        btr         eax,11h
  0000000065065F0C: 48 8B CB           mov         rcx,rbx
  0000000065065F0F: 89 83 D8 01 00 00  mov         dword ptr [rbx+00000000000001D8h],eax
  0000000065065F15: 48 83 C4 20        add         rsp,20h
  0000000065065F19: 5B                 pop         rbx
  0000000065065F1A: E9 21 1A 03 00     jmp         0000000065097940

  Summary

      695000 .text
