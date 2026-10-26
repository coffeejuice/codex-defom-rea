Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F01D0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  00000001412F01D5: 48 89 6C 24 18     mov         qword ptr [rsp+18h],rbp
  00000001412F01DA: 56                 push        rsi
  00000001412F01DB: 57                 push        rdi
  00000001412F01DC: 41 54              push        r12
  00000001412F01DE: 41 56              push        r14
  00000001412F01E0: 41 57              push        r15
  00000001412F01E2: 48 83 EC 40        sub         rsp,40h
  00000001412F01E6: 48 8B 79 30        mov         rdi,qword ptr [rcx+30h]
  00000001412F01EA: 4D 8B F0           mov         r14,r8
  00000001412F01ED: 4C 8B E2           mov         r12,rdx
  00000001412F01F0: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  00000001412F01F4: 4C 8B F9           mov         r15,rcx
  00000001412F01F7: 40 B5 01           mov         bpl,1
  00000001412F01FA: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F01FE: 75 2F              jne         00000001412F022F
  00000001412F0200: 4D 8B 08           mov         r9,qword ptr [r8]
  00000001412F0203: 66 66 66 66 66 0F  nop         word ptr [rax+rax+0000000000000000h]
                    1F 84 00 00 00 00
                    00
  00000001412F0210: 4C 3B 48 18        cmp         r9,qword ptr [rax+18h]
  00000001412F0214: 48 8B F8           mov         rdi,rax
  00000001412F0217: 40 0F 92 C5        setb        bpl
  00000001412F021B: 40 84 ED           test        bpl,bpl
  00000001412F021E: 74 05              je          00000001412F0225
  00000001412F0220: 48 8B 00           mov         rax,qword ptr [rax]
  00000001412F0223: EB 04              jmp         00000001412F0229
  00000001412F0225: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001412F0229: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412F022D: 74 E1              je          00000001412F0210
  00000001412F022F: 48 8B 31           mov         rsi,qword ptr [rcx]
  00000001412F0232: 48 8B DF           mov         rbx,rdi
  00000001412F0235: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  00000001412F023A: 48 89 5C 24 38     mov         qword ptr [rsp+38h],rbx
  00000001412F023F: 40 84 ED           test        bpl,bpl
  00000001412F0242: 0F 84 E4 00 00 00  je          00000001412F032C
  00000001412F0248: 48 8B 41 30        mov         rax,qword ptr [rcx+30h]

  Summary

     1E02000 .text
