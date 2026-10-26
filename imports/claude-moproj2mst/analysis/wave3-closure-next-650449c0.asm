Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650449C0: 48 83 EC 28        sub         rsp,28h
  00000000650449C4: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000650449C8: 44 8B 54 24 50     mov         r10d,dword ptr [rsp+50h]
  00000000650449CD: 39 90 8C 00 00 00  cmp         dword ptr [rax+000000000000008Ch],edx
  00000000650449D3: 75 1B              jne         00000000650449F0
  00000000650449D5: 44 39 80 90 00 00  cmp         dword ptr [rax+0000000000000090h],r8d
                    00
  00000000650449DC: 75 12              jne         00000000650449F0
  00000000650449DE: 44 39 88 94 00 00  cmp         dword ptr [rax+0000000000000094h],r9d
                    00
  00000000650449E5: 75 09              jne         00000000650449F0
  00000000650449E7: 44 39 90 98 00 00  cmp         dword ptr [rax+0000000000000098h],r10d
                    00
  00000000650449EE: 74 25              je          0000000065044A15
  00000000650449F0: 48 83 C1 10        add         rcx,10h
  00000000650449F4: 89 90 8C 00 00 00  mov         dword ptr [rax+000000000000008Ch],edx
  00000000650449FA: 44 89 80 90 00 00  mov         dword ptr [rax+0000000000000090h],r8d
                    00
  0000000065044A01: 44 89 88 94 00 00  mov         dword ptr [rax+0000000000000094h],r9d
                    00
  0000000065044A08: 44 89 90 98 00 00  mov         dword ptr [rax+0000000000000098h],r10d
                    00
  0000000065044A0F: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065044A12: FF 50 58           call        qword ptr [rax+58h]
  0000000065044A15: 48 83 C4 28        add         rsp,28h
  0000000065044A19: C3                 ret

  Summary

      695000 .text
