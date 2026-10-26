Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067134CA0: 40 53              push        rbx
  0000000067134CA2: 48 83 EC 20        sub         rsp,20h
  0000000067134CA6: 8B 51 10           mov         edx,dword ptr [rcx+10h]
  0000000067134CA9: 48 8B D9           mov         rbx,rcx
  0000000067134CAC: 85 D2              test        edx,edx
  0000000067134CAE: 7E 05              jle         0000000067134CB5
  0000000067134CB0: E8 8B 65 FF FF     call        000000006712B240
  0000000067134CB5: 48 8D 15 64 8D 10  lea         rdx,[000000006723DA20h]
                    00
  0000000067134CBC: 45 33 C9           xor         r9d,r9d
  0000000067134CBF: 45 33 C0           xor         r8d,r8d
  0000000067134CC2: 48 8B CB           mov         rcx,rbx
  0000000067134CC5: C7 43 10 FF FF FF  mov         dword ptr [rbx+10h],0FFFFFFFFh
                    FF
  0000000067134CCC: E8 9F 9F FF FF     call        000000006712EC70
  0000000067134CD1: 48 8B CB           mov         rcx,rbx
  0000000067134CD4: 48 83 C4 20        add         rsp,20h
  0000000067134CD8: 5B                 pop         rbx
  0000000067134CD9: E9 82 52 FF FF     jmp         0000000067129F60

  Summary

      1A7000 .text
