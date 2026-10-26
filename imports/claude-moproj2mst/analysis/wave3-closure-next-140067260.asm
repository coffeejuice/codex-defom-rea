Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140067260: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140067265: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014006726A: 48 83 EC 38        sub         rsp,38h
  000000014006726E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140067273: E8 78 3C BB 00     call        0000000140C1AEF0
  0000000140067278: 48 3B 44 24 48     cmp         rax,qword ptr [rsp+48h]
  000000014006727D: 75 0A              jne         0000000140067289
  000000014006727F: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140067287: EB 08              jmp         0000000140067291
  0000000140067289: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140067291: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  0000000140067295: 48 83 C4 38        add         rsp,38h
  0000000140067299: C3                 ret

  Summary

     1E02000 .text
