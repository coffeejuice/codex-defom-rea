Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DAE910: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140DAE915: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DAE91A: 48 83 EC 38        sub         rsp,38h
  0000000140DAE91E: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140DAE923: E8 C8 C5 E6 FF     call        0000000140C1AEF0
  0000000140DAE928: 48 39 44 24 40     cmp         qword ptr [rsp+40h],rax
  0000000140DAE92D: 74 0A              je          0000000140DAE939
  0000000140DAE92F: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140DAE937: EB 08              jmp         0000000140DAE941
  0000000140DAE939: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140DAE941: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  0000000140DAE945: 48 83 C4 38        add         rsp,38h
  0000000140DAE949: C3                 ret

  Summary

     1E02000 .text
