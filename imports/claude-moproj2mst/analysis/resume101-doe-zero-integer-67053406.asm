Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067053406: 4C 89 64 24 48     mov         qword ptr [rsp+48h],r12
  000000006705340B: 4C 8B FA           mov         r15,rdx
  000000006705340E: 66 90              nop
  0000000067053410: 44 0F B6 E3        movzx       r12d,bl
  0000000067053414: 41 8B CC           mov         ecx,r12d
  0000000067053417: FF 15 B3 52 15 00  call        qword ptr [00000000671A86D0h]
  000000006705341D: 85 C0              test        eax,eax
  000000006705341F: 74 05              je          0000000067053426
  0000000067053421: 80 C3 D0           add         bl,0D0h
  0000000067053424: EB 27              jmp         000000006705344D
  0000000067053426: 41 8B CC           mov         ecx,r12d
  0000000067053429: FF 15 99 52 15 00  call        qword ptr [00000000671A86C8h]
  000000006705342F: 85 C0              test        eax,eax
  0000000067053431: 74 56              je          0000000067053489
  0000000067053433: 41 8B CC           mov         ecx,r12d
  0000000067053436: FF 15 84 52 15 00  call        qword ptr [00000000671A86C0h]
  000000006705343C: B9 57 00 00 00     mov         ecx,57h
  0000000067053441: 85 C0              test        eax,eax
  0000000067053443: B8 37 00 00 00     mov         eax,37h
  0000000067053448: 0F 45 C8           cmovne      ecx,eax
  000000006705344B: 2A D9              sub         bl,cl
  000000006705344D: 0F B6 C3           movzx       eax,bl
  0000000067053450: 41 3B C5           cmp         eax,r13d
  0000000067053453: 7D 34              jge         0000000067053489
  0000000067053455: 85 ED              test        ebp,ebp
  0000000067053457: 78 23              js          000000006705347C
  0000000067053459: 49 3B F6           cmp         rsi,r14
  000000006705345C: 77 1E              ja          000000006705347C
  000000006705345E: 49 3B F6           cmp         rsi,r14
  0000000067053461: 75 05              jne         0000000067053468
  0000000067053463: 41 3B C7           cmp         eax,r15d
  0000000067053466: 7F 14              jg          000000006705347C
  0000000067053468: 41 8B C5           mov         eax,r13d
  000000006705346B: BD 01 00 00 00     mov         ebp,1
  0000000067053470: 48 0F AF F0        imul        rsi,rax
  0000000067053474: 0F B6 C3           movzx       eax,bl
  0000000067053477: 48 03 F0           add         rsi,rax
  000000006705347A: EB 03              jmp         000000006705347F
  000000006705347C: 83 CD FF           or          ebp,0FFFFFFFFh
  000000006705347F: 8A 1F              mov         bl,byte ptr [rdi]
  0000000067053481: 48 FF C7           inc         rdi
  0000000067053484: 80 FB 80           cmp         bl,80h
  0000000067053487: 72 87              jb          0000000067053410
  0000000067053489: 4C 8B 64 24 48     mov         r12,qword ptr [rsp+48h]
  000000006705348E: 4C 8B BC 24 98 00  mov         r15,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000067053496: 85 ED              test        ebp,ebp
  0000000067053498: 79 10              jns         00000000670534AA

  Summary

      1A7000 .text
