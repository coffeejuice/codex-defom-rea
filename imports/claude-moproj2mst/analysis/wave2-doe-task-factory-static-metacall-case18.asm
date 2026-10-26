Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400DB573: 48 8D 84 24 08 01  lea         rax,[rsp+0000000000000108h]
                    00 00
  00000001400DB57B: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  00000001400DB583: 48 8B 94 24 78 03  mov         rdx,qword ptr [rsp+0000000000000378h]
                    00 00
  00000001400DB58B: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  00000001400DB58F: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001400DB597: FF 15 3B A4 D2 01  call        qword ptr [0000000141E059D8h]
  00000001400DB59D: 48 89 84 24 80 02  mov         qword ptr [rsp+0000000000000280h],rax
                    00 00
  00000001400DB5A5: 48 8B 94 24 80 02  mov         rdx,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400DB5AD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400DB5B2: E8 F9 10 F6 FF     call        000000014003C6B0
  00000001400DB5B7: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400DB5BC: 48 8B 84 24 78 03  mov         rax,qword ptr [rsp+0000000000000378h]
                    00 00
  00000001400DB5C4: 48 83 38 00        cmp         qword ptr [rax],0
  00000001400DB5C8: 74 13              je          00000001400DB5DD
  00000001400DB5CA: 48 8B 8C 24 78 03  mov         rcx,qword ptr [rsp+0000000000000378h]
                    00 00
  00000001400DB5D2: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001400DB5D5: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001400DB5DA: 48 89 01           mov         qword ptr [rcx],rax
  00000001400DB5DD: E9 12 13 00 00     jmp         00000001400DC8F4
  00000001400DB5E2: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001400DB5E7: 48
  00000001400DB5E8: 8B

  Summary

     1E02000 .text
