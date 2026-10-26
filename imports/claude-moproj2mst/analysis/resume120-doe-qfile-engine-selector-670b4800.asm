Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B4800: 40 53              push        rbx
  00000000670B4802: 48 83 EC 20        sub         rsp,20h
  00000000670B4806: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000670B480A: 48 83 BB F0 00 00  cmp         qword ptr [rbx+00000000000000F0h],0
                    00 00
  00000000670B4812: 75 13              jne         00000000670B4827
  00000000670B4814: 48 8D 8B E8 00 00  lea         rcx,[rbx+00000000000000E8h]
                    00
  00000000670B481B: E8 00 2E FF FF     call        00000000670A7620
  00000000670B4820: 48 89 83 F0 00 00  mov         qword ptr [rbx+00000000000000F0h],rax
                    00
  00000000670B4827: 48 8B 83 F0 00 00  mov         rax,qword ptr [rbx+00000000000000F0h]
                    00
  00000000670B482E: 48 83 C4 20        add         rsp,20h
  00000000670B4832: 5B                 pop         rbx
  00000000670B4833: C3                 ret

  Summary

      1A7000 .text
