Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CE9C0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402CE9C5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CE9CA: 48 83 EC 28        sub         rsp,28h
  00000001402CE9CE: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402CE9D3: 0F B6 80 41 01 00  movzx       eax,byte ptr [rax+0000000000000141h]
                    00
  00000001402CE9DA: 85 C0              test        eax,eax
  00000001402CE9DC: 75 0F              jne         00000001402CE9ED
  00000001402CE9DE: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001402CE9E3: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001402CE9E8: E8 03 20 10 00     call        00000001403D09F0
  00000001402CE9ED: 48 83 C4 28        add         rsp,28h
  00000001402CE9F1: C3                 ret

  Summary

     1E02000 .text
