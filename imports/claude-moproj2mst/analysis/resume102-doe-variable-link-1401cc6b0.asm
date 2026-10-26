Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001401CC6B0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401CC6B5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001401CC6BA: 48 8B 40 40        mov         rax,qword ptr [rax+40h]
  00000001401CC6BE: C3                 ret
  00000001401CC6BF: CC                 int         3
  00000001401CC6C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401CC6C5: 48 83 EC 28        sub         rsp,28h
  00000001401CC6C9: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001401CC6CE: 48 83 78 20 00     cmp         qword ptr [rax+20h],0
  00000001401CC6D3: 75 06              jne         00000001401CC6DB
  00000001401CC6D5: 32 C0              xor         al,al
  00000001401CC6D7: EB 1C              jmp         00000001401CC6F5
  00000001401CC6D9: EB 1A              jmp         00000001401CC6F5
  00000001401CC6DB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001401CC6E0: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  00000001401CC6E4: E8 F7 44 38 00     call        0000000140550BE0
  00000001401CC6E9: 85 C0              test        eax,eax
  00000001401CC6EB: 7E 06              jle         00000001401CC6F3
  00000001401CC6ED: B0 01              mov         al,1
  00000001401CC6EF: EB

  Summary

     1E02000 .text
