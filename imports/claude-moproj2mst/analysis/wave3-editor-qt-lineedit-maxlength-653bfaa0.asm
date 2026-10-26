Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653BFAA0: 48 83 EC 28        sub         rsp,28h
  00000000653BFAA4: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000653BFAA8: 48 8B 88 E0 01 00  mov         rcx,qword ptr [rax+00000000000001E0h]
                    00
  00000000653BFAAF: 48 83 B9 98 00 00  cmp         qword ptr [rcx+0000000000000098h],0
                    00 00
  00000000653BFAB7: 75 13              jne         00000000653BFACC
  00000000653BFAB9: 89 51 4C           mov         dword ptr [rcx+4Ch],edx
  00000000653BFABC: 48 8D 51 10        lea         rdx,[rcx+10h]
  00000000653BFAC0: 45 33 C9           xor         r9d,r9d
  00000000653BFAC3: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  00000000653BFAC7: E8 14 67 00 00     call        00000000653C61E0
  00000000653BFACC: 48 83 C4 28        add         rsp,28h
  00000000653BFAD0: C3                 ret

  Summary

      695000 .text
