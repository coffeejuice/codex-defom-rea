Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654FF380: 48 83 EC 48        sub         rsp,48h
  00000000654FF384: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654FF38D: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000654FF391: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  00000000654FF399: C7 44 24 2C FF FF  mov         dword ptr [rsp+2Ch],0FFFFFFFFh
                    FF FF
  00000000654FF3A1: 33 C0              xor         eax,eax
  00000000654FF3A3: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000000654FF3A8: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000000654FF3AD: 48 8B 89 80 02 00  mov         rcx,qword ptr [rcx+0000000000000280h]
                    00
  00000000654FF3B4: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000654FF3B7: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000654FF3BC: FF 50 68           call        qword ptr [rax+68h]
  00000000654FF3BF: 90                 nop
  00000000654FF3C0: 48 83 C4 48        add         rsp,48h
  00000000654FF3C4: C3                 ret

  Summary

      695000 .text
