Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014036F980: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014036F985: 48 83 EC 48        sub         rsp,48h
  000000014036F989: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014036F98E: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014036F992: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014036F997: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014036F99C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014036F9A1: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014036F9A7: 74 16              je          000000014036F9BF
  000000014036F9A9: BA 01 00 00 00     mov         edx,1
  000000014036F9AE: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014036F9B3: E8 C8 65 00 00     call        0000000140375F80
  000000014036F9B8: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014036F9BD: EB 09              jmp         000000014036F9C8
  000000014036F9BF: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014036F9C8: 48 83 C4 48        add         rsp,48h
  000000014036F9CC: C3                 ret

  Summary

     1E02000 .text
