Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C7C220: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C7C225: 48 83 EC 28        sub         rsp,28h
  0000000140C7C229: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C7C22E: E8 9D EA FF FF     call        0000000140C7ACD0
  0000000140C7C233: 48 85 C0           test        rax,rax
  0000000140C7C236: 74 14              je          0000000140C7C24C
  0000000140C7C238: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C7C23D: E8 8E EA FF FF     call        0000000140C7ACD0
  0000000140C7C242: 48 8B C8           mov         rcx,rax
  0000000140C7C245: E8 76 3F BB FF     call        00000001408301C0
  0000000140C7C24A: EB 02              jmp         0000000140C7C24E
  0000000140C7C24C: 33 C0              xor         eax,eax
  0000000140C7C24E: 48 83 C4 28        add         rsp,28h
  0000000140C7C252: C3                 ret

  Summary

     1E02000 .text
