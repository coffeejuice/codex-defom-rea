Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CE7D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CE7D5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001402CE7DA: 48 C7 40 28 00 00  mov         qword ptr [rax+28h],0
                    00 00
  00000001402CE7E2: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001402CE7E7: C6 40 20 00        mov         byte ptr [rax+20h],0
  00000001402CE7EB: C3                 ret
  00000001402CE7EC: CC                 int         3
  00000001402CE7ED: CC                 int         3
  00000001402CE7EE: CC                 int         3
  00000001402CE7EF: CC                 int         3
  00000001402CE7F0: CC                 int         3
  00000001402CE7F1: CC                 int         3
  00000001402CE7F2: CC                 int         3
  00000001402CE7F3: CC                 int         3
  00000001402CE7F4: CC                 int         3
  00000001402CE7F5: CC                 int         3
  00000001402CE7F6: CC                 int         3
  00000001402CE7F7: CC                 int         3
  00000001402CE7F8: CC                 int         3
  00000001402CE7F9: CC                 int         3
  00000001402CE7FA: CC                 int         3
  00000001402CE7FB: CC                 int         3
  00000001402CE7FC: CC                 int         3
  00000001402CE7FD: CC                 int         3
  00000001402CE7FE: CC                 int         3
  00000001402CE7FF: CC                 int         3
  00000001402CE800: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CE805: 48 83 EC 28        sub         rsp,28h
  00000001402CE809: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402CE80E: 0F B6 40 20        movzx       eax,byte ptr [rax+20h]
  00000001402CE812: 85 C0              test        eax,eax
  00000001402CE814: 75 02              jne         00000001402CE818
  00000001402CE816: EB 23              jmp         00000001402CE83B
  00000001402CE818: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402CE81D: 0F B6 80 41 01 00  movzx       eax,byte ptr [rax+0000000000000141h]
                    00
  00000001402CE824: 85 C0              test        eax,eax
  00000001402CE826: 75 13              jne         00000001402CE83B
  00000001402CE828: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000001402CE82D: 48
  00000001402CE82E: 8B
  00000001402CE82F: 52                 push        rdx

  Summary

     1E02000 .text
