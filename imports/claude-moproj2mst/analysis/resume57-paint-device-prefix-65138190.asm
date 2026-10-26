Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065138190: 40 53              push        rbx
  0000000065138192: 48 83 EC 20        sub         rsp,20h
  0000000065138196: 66 83 79 08 00     cmp         word ptr [rcx+8],0
  000000006513819B: 48 8D 05 36 A5 58  lea         rax,[00000000656C26D8h]
                    00
  00000000651381A2: 48 8B D9           mov         rbx,rcx
  00000000651381A5: 48 89 01           mov         qword ptr [rcx],rax
  00000000651381A8: 74 0D              je          00000000651381B7
  00000000651381AA: 48 8D 0D 8F A5 58  lea         rcx,[00000000656C2740h]
                    00
  00000000651381B1: FF 15 E1 E7 55 00  call        qword ptr [0000000065696998h]
  00000000651381B7: 48 8B CB           mov         rcx,rbx
  00000000651381BA: 48 83 C4 20        add         rsp,20h
  00000000651381BE: 5B                 pop         rbx
  00000000651381BF: E9 7C 06 01 00     jmp         0000000065148840

  Summary

      695000 .text
