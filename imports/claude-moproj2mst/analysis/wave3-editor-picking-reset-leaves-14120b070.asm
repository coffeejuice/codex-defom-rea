Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014120B070: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014120B075: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014120B07A: C7 80 C0 43 00 00  mov         dword ptr [rax+00000000000043C0h],0
                    00 00 00 00
  000000014120B084: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014120B089: C7 80 C4 43 00 00  mov         dword ptr [rax+00000000000043C4h],0
                    00 00 00 00
  000000014120B093: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014120B098: C7 80 C8 43 00 00  mov         dword ptr [rax+00000000000043C8h],0
                    00 00 00 00
  000000014120B0A2: C3                 ret
  000000014120B0A3: CC                 int         3
  000000014120B0A4: CC                 int         3
  000000014120B0A5: CC                 int         3
  000000014120B0A6: CC                 int         3
  000000014120B0A7: CC                 int         3
  000000014120B0A8: CC                 int         3
  000000014120B0A9: CC                 int         3
  000000014120B0AA: CC                 int         3
  000000014120B0AB: CC                 int         3
  000000014120B0AC: CC                 int         3
  000000014120B0AD: CC                 int         3
  000000014120B0AE: CC                 int         3
  000000014120B0AF: CC                 int         3
  000000014120B0B0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014120B0B5: 48 83 EC 18        sub         rsp,18h
  000000014120B0B9: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014120B0BE: 83 B8 C4 43 00 00  cmp         dword ptr [rax+00000000000043C4h],0
                    00
  000000014120B0C5: 7E 17              jle         000000014120B0DE
  000000014120B0C7: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014120B0CC: 83 B8 C8 43 00 00  cmp         dword ptr [rax+00000000000043C8h],0
                    00
  000000014120B0D3: 7E 09              jle         000000014120B0DE
  000000014120B0D5: C7 04 24 01 00 00  mov         dword ptr [rsp],1
                    00
  000000014120B0DC: EB 07              jmp         000000014120B0E5
  000000014120B0DE: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  000000014120B0E5: 8A 04 24           mov         al,byte ptr [rsp]
  000000014120B0E8: 48 83 C4 18        add         rsp,18h
  000000014120B0EC: C3                 ret
  000000014120B0ED: CC                 int         3
  000000014120B0EE: CC                 int         3
  000000014120B0EF: CC                 int         3

  Summary

     1E02000 .text
