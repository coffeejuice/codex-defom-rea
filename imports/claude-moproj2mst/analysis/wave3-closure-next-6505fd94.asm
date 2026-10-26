Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505FD94: 48 8B CE           mov         rcx,rsi
  000000006505FD97: 48 8B DE           mov         rbx,rsi
  000000006505FD9A: FF 15 C8 6A 63 00  call        qword ptr [0000000065696868h]
  000000006505FDA0: 48 8B 4E 20        mov         rcx,qword ptr [rsi+20h]
  000000006505FDA4: F6 41 0C 01        test        byte ptr [rcx+0Ch],1
  000000006505FDA8: 4C 8B D8           mov         r11,rax
  000000006505FDAB: 75 21              jne         000000006505FDCE
  000000006505FDAD: 0F 1F 00           nop         dword ptr [rax]
  000000006505FDB0: 4D 85 DB           test        r11,r11
  000000006505FDB3: 74 19              je          000000006505FDCE
  000000006505FDB5: 49 8B CB           mov         rcx,r11
  000000006505FDB8: 49 8B DB           mov         rbx,r11
  000000006505FDBB: FF 15 A7 6A 63 00  call        qword ptr [0000000065696868h]
  000000006505FDC1: 4C 8B D8           mov         r11,rax
  000000006505FDC4: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006505FDC8: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  000000006505FDCC: 74 E2              je          000000006505FDB0
  000000006505FDCE: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006505FDD2: 48 8B 88 98 00 00  mov         rcx,qword ptr [rax+0000000000000098h]
                    00
  000000006505FDD9: 4C 89 A0 98 00 00  mov         qword ptr [rax+0000000000000098h],r12
                    00
  000000006505FDE0: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006505FDE4: 48 89 B0 90 00 00  mov         qword ptr [rax+0000000000000090h],rsi
                    00
  000000006505FDEB: 49 89 8E 98 00 00  mov         qword ptr [r14+0000000000000098h],rcx
                    00
  000000006505FDF2: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  000000006505FDF7: 48 89 98 90 00 00  mov         qword ptr [rax+0000000000000090h],rbx
                    00
  000000006505FDFE: EB 13              jmp         000000006505FE13
  000000006505FE00: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  000000006505FE05: 48 89 B0 90 00 00  mov         qword ptr [rax+0000000000000090h],rsi
                    00
  000000006505FE0C: 4D 89 A6 98 00 00  mov         qword ptr [r14+0000000000000098h],r12
                    00
  000000006505FE13: 4C 8B 64 24 48     mov         r12,qword ptr [rsp+48h]

  Summary

      695000 .text
