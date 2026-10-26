Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014082EBF0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014082EBF5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014082EBFA: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  000000014082EC01: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  000000014082EC0A: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  000000014082EC12: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014082EC1A: E8 41 D3 FF FF     call        000000014082BF60
  000000014082EC1F: 0F B6 C0           movzx       eax,al
  000000014082EC22: 85 C0              test        eax,eax
  000000014082EC24: 74 07              je          000000014082EC2D
  000000014082EC26: 33 C0              xor         eax,eax
  000000014082EC28: E9 97 03 00 00     jmp         000000014082EFC4

  Summary

     1E02000 .text
