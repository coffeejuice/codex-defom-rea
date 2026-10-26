Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711D580: 48 83 EC 28        sub         rsp,28h
  000000006711D584: F6 41 12 01        test        byte ptr [rcx+12h],1
  000000006711D588: 48 8D 05 51 BC 08  lea         rax,[00000000671A91E0h]
                    00
  000000006711D58F: 48 89 01           mov         qword ptr [rcx],rax
  000000006711D592: 74 0F              je          000000006711D5A3
  000000006711D594: 48 83 3D 84 6C 1D  cmp         qword ptr [00000000672F4220h],0
                    00 00
  000000006711D59C: 74 05              je          000000006711D5A3
  000000006711D59E: E8 CD C3 FF FF     call        0000000067119970
  000000006711D5A3: 48 83 C4 28        add         rsp,28h
  000000006711D5A7: C3                 ret

  Summary

      1A7000 .text
