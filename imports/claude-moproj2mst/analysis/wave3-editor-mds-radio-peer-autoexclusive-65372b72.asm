Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065372B72: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  0000000065372B77: 48 83 C4 20        add         rsp,20h
  0000000065372B7B: 5E                 pop         rsi
  0000000065372B7C: C3                 ret
  0000000065372B7D: F6 81 04 02 00 00  test        byte ptr [rcx+0000000000000204h],8
                    08
  0000000065372B84: 74 EC              je          0000000065372B72
  0000000065372B86: E8 45 FE FF FF     call        00000000653729D0
  0000000065372B8B: 48 85 C0           test        rax,rax
  0000000065372B8E: 74 E2              je          0000000065372B72
  0000000065372B90: 33 D2              xor         edx,edx
  0000000065372B92: 48 8B C8           mov         rcx,rax
  0000000065372B95: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  0000000065372B9A: 48 83 C4 20        add         rsp,20h
  0000000065372B9E: 5E                 pop         rsi
  0000000065372B9F: E9 8C 08 00 00     jmp         0000000065373430

  Summary

      695000 .text
