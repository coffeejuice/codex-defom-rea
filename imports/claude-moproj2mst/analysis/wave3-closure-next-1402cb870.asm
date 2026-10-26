Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CB870: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CB875: 48 83 EC 18        sub         rsp,18h
  00000001402CB879: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001402CB87E: 83 78 0C 05        cmp         dword ptr [rax+0Ch],5
  00000001402CB882: 75 09              jne         00000001402CB88D
  00000001402CB884: C7 04 24 01 00 00  mov         dword ptr [rsp],1
                    00
  00000001402CB88B: EB 07              jmp         00000001402CB894
  00000001402CB88D: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  00000001402CB894: 8A 04 24           mov         al,byte ptr [rsp]
  00000001402CB897: 48 83 C4 18        add         rsp,18h
  00000001402CB89B: C3                 ret

  Summary

     1E02000 .text
