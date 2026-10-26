Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014049D190: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014049D195: 48 83 EC 28        sub         rsp,28h
  000000014049D199: 45 33 C9           xor         r9d,r9d
  000000014049D19C: 45 33 C0           xor         r8d,r8d
  000000014049D19F: B2 01              mov         dl,1
  000000014049D1A1: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014049D1A6: 48 8B 49 58        mov         rcx,qword ptr [rcx+58h]
  000000014049D1AA: E8 11 58 EC FF     call        00000001403629C0
  000000014049D1AF: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014049D1B4: 0F B6 80 89 00 00  movzx       eax,byte ptr [rax+0000000000000089h]
                    00
  000000014049D1BB: 85 C0              test        eax,eax
  000000014049D1BD: 75 0A              jne         000000014049D1C9
  000000014049D1BF: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014049D1C4: E8 B7 91 FE FF     call        0000000140486380
  000000014049D1C9: 48 83 C4 28        add         rsp,28h
  000000014049D1CD: C3                 ret

  Summary

     1E02000 .text
