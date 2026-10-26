Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140010330: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140010335: 48 83 EC 18        sub         rsp,18h
  0000000140010339: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014001033E: 83 78 18 02        cmp         dword ptr [rax+18h],2
  0000000140010342: 75 09              jne         000000014001034D
  0000000140010344: C7 04 24 01 00 00  mov         dword ptr [rsp],1
                    00
  000000014001034B: EB 07              jmp         0000000140010354
  000000014001034D: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  0000000140010354: 8A 04 24           mov         al,byte ptr [rsp]
  0000000140010357: 48 83 C4 18        add         rsp,18h
  000000014001035B: C3                 ret

  Summary

     1E02000 .text
