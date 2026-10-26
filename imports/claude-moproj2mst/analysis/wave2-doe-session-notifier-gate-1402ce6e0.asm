Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CE6E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CE6E5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001402CE6EA: 48 C7 40 18 00 00  mov         qword ptr [rax+18h],0
                    00 00
  00000001402CE6F2: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001402CE6F7: C6 40 10 00        mov         byte ptr [rax+10h],0
  00000001402CE6FB: C3                 ret
  00000001402CE6FC: CC                 int         3
  00000001402CE6FD: CC                 int         3
  00000001402CE6FE: CC                 int         3
  00000001402CE6FF: CC                 int         3
  00000001402CE700: CC                 int         3
  00000001402CE701: CC                 int         3
  00000001402CE702: CC                 int         3
  00000001402CE703: CC                 int         3
  00000001402CE704: CC                 int         3
  00000001402CE705: CC                 int         3
  00000001402CE706: CC                 int         3
  00000001402CE707: CC                 int         3
  00000001402CE708: CC                 int         3
  00000001402CE709: CC                 int         3
  00000001402CE70A: CC                 int         3
  00000001402CE70B: CC                 int         3
  00000001402CE70C: CC                 int         3
  00000001402CE70D: CC                 int         3
  00000001402CE70E: CC                 int         3
  00000001402CE70F: CC                 int         3
  00000001402CE710: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CE715: 48 83 EC 28        sub         rsp,28h
  00000001402CE719: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402CE71E: 0F B6 40 10        movzx       eax,byte ptr [rax+10h]
  00000001402CE722: 85 C0              test        eax,eax
  00000001402CE724: 75 02              jne         00000001402CE728
  00000001402CE726: EB 23              jmp         00000001402CE74B
  00000001402CE728: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402CE72D: 0F B6 80 41 01 00  movzx       eax,byte ptr [rax+0000000000000141h]
                    00
  00000001402CE734: 85 C0              test        eax,eax
  00000001402CE736: 75 13              jne         00000001402CE74B
  00000001402CE738: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000001402CE73D: 48
  00000001402CE73E: 8B
  00000001402CE73F: 52                 push        rdx

  Summary

     1E02000 .text
