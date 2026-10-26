Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654D5E21: 48 89 6C 24 30     mov         qword ptr [rsp+30h],rbp
  00000000654D5E26: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  00000000654D5E30: 48 8B 17           mov         rdx,qword ptr [rdi]
  00000000654D5E33: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000654D5E36: 48 8B 1A           mov         rbx,qword ptr [rdx]
  00000000654D5E39: 48 3B D9           cmp         rbx,rcx
  00000000654D5E3C: 74 0F              je          00000000654D5E4D
  00000000654D5E3E: 48 8B 42 10        mov         rax,qword ptr [rdx+10h]
  00000000654D5E42: 48 39 43 10        cmp         qword ptr [rbx+10h],rax
  00000000654D5E46: 75 05              jne         00000000654D5E4D
  00000000654D5E48: 40 B5 01           mov         bpl,1
  00000000654D5E4B: EB 03              jmp         00000000654D5E50
  00000000654D5E4D: 40 32 ED           xor         bpl,bpl
  00000000654D5E50: FF 15 CA 14 1C 00  call        qword ptr [0000000065697320h]
  00000000654D5E56: 48 89 1F           mov         qword ptr [rdi],rbx
  00000000654D5E59: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000654D5E5C: FF 49 14           dec         dword ptr [rcx+14h]
  00000000654D5E5F: 40 84 ED           test        bpl,bpl
  00000000654D5E62: 75 CC              jne         00000000654D5E30
  00000000654D5E64: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000654D5E67: FF 15 33 15 1C 00  call        qword ptr [00000000656973A0h]
  00000000654D5E6D: 48 8B 6C 24 30     mov         rbp,qword ptr [rsp+30h]

  Summary

      695000 .text
