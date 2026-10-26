Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670A7CDE: 8B 05 1C C2 24 00  mov         eax,dword ptr [00000000672F3F00h]
  00000000670A7CE4: A8 01              test        al,1
  00000000670A7CE6: 75 23              jne         00000000670A7D0B
  00000000670A7CE8: 83 C8 01           or          eax,1
  00000000670A7CEB: 48 8D 0D 8E E8 0F  lea         rcx,[00000000671A6580h]
                    00
  00000000670A7CF2: 89 05 08 C2 24 00  mov         dword ptr [00000000672F3F00h],eax
  00000000670A7CF8: 48 8D 05 09 C2 24  lea         rax,[00000000672F3F08h]
                    00
  00000000670A7CFF: 48 89 05 F2 C1 24  mov         qword ptr [00000000672F3EF8h],rax
                    00
  00000000670A7D06: E8 49 0E 0D 00     call        0000000067178B54
  00000000670A7D0B: 48 8B 05 F6 C1 24  mov         rax,qword ptr [00000000672F3F08h]
                    00
  00000000670A7D12: 48 8B 5C 24 20     mov         rbx,qword ptr [rsp+20h]
  00000000670A7D17: 48 83 C4 28        add         rsp,28h
  00000000670A7D1B: C3                 ret

  Summary

      1A7000 .text
