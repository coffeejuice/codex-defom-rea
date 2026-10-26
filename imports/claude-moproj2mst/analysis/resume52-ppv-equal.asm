Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D4B9E0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D4B9E5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D4B9EA: 48 83 EC 38        sub         rsp,38h
  0000000140D4B9EE: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D4B9F7: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D4B9FC: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140D4BA01: 8B 00              mov         eax,dword ptr [rax]
  0000000140D4BA03: 39 01              cmp         dword ptr [rcx],eax
  0000000140D4BA05: 75 7F              jne         0000000140D4BA86
  0000000140D4BA07: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D4BA0C: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140D4BA11: 8B 40 04           mov         eax,dword ptr [rax+4]
  0000000140D4BA14: 39 41 04           cmp         dword ptr [rcx+4],eax
  0000000140D4BA17: 75 6D              jne         0000000140D4BA86
  0000000140D4BA19: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D4BA1E: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140D4BA23: F2 0F 10 41 08     movsd       xmm0,mmword ptr [rcx+8]
  0000000140D4BA28: F2 0F 5C 40 08     subsd       xmm0,mmword ptr [rax+8]
  0000000140D4BA2D: E8 46 50 72 00     call        0000000141470A78
  0000000140D4BA32: 66 0F 28 E8        movapd      xmm5,xmm0
  0000000140D4BA36: F2 0F 10 05 E2 B9  movsd       xmm0,mmword ptr [0000000142157420h]
                    40 01
  0000000140D4BA3E: 66 0F 2F C5        comisd      xmm0,xmm5
  0000000140D4BA42: 76 42              jbe         0000000140D4BA86
  0000000140D4BA44: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D4BA49: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140D4BA4E: F2 0F 10 41 10     movsd       xmm0,mmword ptr [rcx+10h]
  0000000140D4BA53: F2 0F 5C 40 10     subsd       xmm0,mmword ptr [rax+10h]
  0000000140D4BA58: E8 1B 50 72 00     call        0000000141470A78
  0000000140D4BA5D: 66 0F 28 E8        movapd      xmm5,xmm0
  0000000140D4BA61: F2 0F 10 05 B7 B9  movsd       xmm0,mmword ptr [0000000142157420h]
                    40 01
  0000000140D4BA69: 66 0F 2F C5        comisd      xmm0,xmm5
  0000000140D4BA6D: 76 17              jbe         0000000140D4BA86
  0000000140D4BA6F: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140D4BA74: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D4BA79: E8 B2 06 F6 FF     call        0000000140CAC130
  0000000140D4BA7E: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  0000000140D4BA82: EB 22              jmp         0000000140D4BAA6
  0000000140D4BA84: EB 15              jmp         0000000140D4BA9B
  0000000140D4BA86: C6 44 24 21 00     mov         byte ptr [rsp+21h],0
  0000000140D4BA8B: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D4BA90: E8 9B 06 F6 FF     call        0000000140CAC130
  0000000140D4BA95: 8A 44 24 21        mov         al,byte ptr [rsp+21h]
  0000000140D4BA99: EB 0B              jmp         0000000140D4BAA6
  0000000140D4BA9B: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D4BAA0: E8 8B 06 F6 FF     call        0000000140CAC130
  0000000140D4BAA5: 90                 nop
  0000000140D4BAA6: 48 83 C4 38        add         rsp,38h
  0000000140D4BAAA: C3                 ret

  Summary

     1E02000 .text
