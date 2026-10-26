Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140127910: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140127915: 48 83 EC 28        sub         rsp,28h
  0000000140127919: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014012791E: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140127922: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  0000000140127929: FF 15 39 E4 CD 01  call        qword ptr [0000000141E05D68h]
  000000014012792F: 0F B6 C0           movzx       eax,al
  0000000140127932: 85 C0              test        eax,eax
  0000000140127934: 74 07              je          000000014012793D
  0000000140127936: B8 02 00 00 00     mov         eax,2
  000000014012793B: EB 05              jmp         0000000140127942
  000000014012793D: B8 01 00 00 00     mov         eax,1
  0000000140127942: 48 83 C4 28        add         rsp,28h
  0000000140127946: C3                 ret
  0000000140127947: CC                 int         3

  Summary

     1E02000 .text
