Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014035B400: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014035B405: 48 83 EC 18        sub         rsp,18h
  000000014035B409: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014035B40E: 83 78 1C 03        cmp         dword ptr [rax+1Ch],3
  000000014035B412: 75 09              jne         000000014035B41D
  000000014035B414: C7 04 24 01 00 00  mov         dword ptr [rsp],1
                    00
  000000014035B41B: EB 07              jmp         000000014035B424
  000000014035B41D: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  000000014035B424: 8A 04 24           mov         al,byte ptr [rsp]
  000000014035B427: 48 83 C4 18        add         rsp,18h
  000000014035B42B: C3                 ret

  Summary

     1E02000 .text
