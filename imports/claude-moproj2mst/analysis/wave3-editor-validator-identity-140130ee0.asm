Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140130EE0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140130EE5: 48 83 EC 38        sub         rsp,38h
  0000000140130EE9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140130EEE: E8 5D FF FF FF     call        0000000140130E50
  0000000140130EF3: 83 F8 03           cmp         eax,3
  0000000140130EF6: 75 0A              jne         0000000140130F02
  0000000140130EF8: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140130F00: EB 08              jmp         0000000140130F0A
  0000000140130F02: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140130F0A: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  0000000140130F0E: 48 83 C4 38        add         rsp,38h
  0000000140130F12: C3                 ret

  Summary

     1E02000 .text
