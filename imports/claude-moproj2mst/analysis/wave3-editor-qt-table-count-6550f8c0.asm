Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006550F8C0: 48 83 EC 48        sub         rsp,48h
  000000006550F8C4: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006550F8CD: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006550F8D1: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  000000006550F8D9: C7 44 24 2C FF FF  mov         dword ptr [rsp+2Ch],0FFFFFFFFh
                    FF FF
  000000006550F8E1: 33 C0              xor         eax,eax
  000000006550F8E3: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006550F8E8: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000006550F8ED: 48 8B 89 80 02 00  mov         rcx,qword ptr [rcx+0000000000000280h]
                    00
  000000006550F8F4: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006550F8F7: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006550F8FC: FF 50 70           call        qword ptr [rax+70h]
  000000006550F8FF: 90                 nop
  000000006550F900: 48 83 C4 48        add         rsp,48h
  000000006550F904: C3                 ret

  Summary

      695000 .text
