Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670EA8E0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670EA8E5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000670EA8EA: 57                 push        rdi
  00000000670EA8EB: 48 83 EC 20        sub         rsp,20h
  00000000670EA8EF: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000670EA8F3: 49 8B F8           mov         rdi,r8
  00000000670EA8F6: 48 8B F2           mov         rsi,rdx
  00000000670EA8F9: 83 BB 9C 00 00 00  cmp         dword ptr [rbx+000000000000009Ch],2
                    02
  00000000670EA900: 74 10              je          00000000670EA912
  00000000670EA902: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670EA905: FF 50 18           call        qword ptr [rax+18h]
  00000000670EA908: C7 83 9C 00 00 00  mov         dword ptr [rbx+000000000000009Ch],2
                    02 00 00 00
  00000000670EA912: 4C 8B C7           mov         r8,rdi
  00000000670EA915: 48 8B D6           mov         rdx,rsi
  00000000670EA918: 48 8B CB           mov         rcx,rbx
  00000000670EA91B: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000670EA920: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  00000000670EA925: 48 83 C4 20        add         rsp,20h
  00000000670EA929: 5F                 pop         rdi
  00000000670EA92A: E9 61 DD 00 00     jmp         00000000670F8690

  Summary

      1A7000 .text
