Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014082BF10: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014082BF15: 48 83 EC 28        sub         rsp,28h
  000000014082BF19: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014082BF1E: 0F B6 80 58 1B 00  movzx       eax,byte ptr [rax+0000000000001B58h]
                    00
  000000014082BF25: 85 C0              test        eax,eax
  000000014082BF27: 75 2A              jne         000000014082BF53
  000000014082BF29: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014082BF2E: E8 CD F4 B2 FF     call        000000014035B400
  000000014082BF33: 0F B6 C0           movzx       eax,al
  000000014082BF36: 85 C0              test        eax,eax
  000000014082BF38: 74 15              je          000000014082BF4F
  000000014082BF3A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014082BF3F: E8 DC 94 FF FF     call        0000000140825420
  000000014082BF44: 3D 70 17 00 00     cmp         eax,1770h
  000000014082BF49: 7C 04              jl          000000014082BF4F
  000000014082BF4B: 32 C0              xor         al,al
  000000014082BF4D: EB 06              jmp         000000014082BF55
  000000014082BF4F: B0 01              mov         al,1
  000000014082BF51: EB 02              jmp         000000014082BF55
  000000014082BF53: B0 01              mov         al,1
  000000014082BF55: 48 83 C4 28        add         rsp,28h
  000000014082BF59: C3                 ret

  Summary

     1E02000 .text
