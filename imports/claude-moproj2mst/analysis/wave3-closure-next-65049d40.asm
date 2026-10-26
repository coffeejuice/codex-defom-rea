Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065049D40: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  0000000065049D45: 41 54              push        r12
  0000000065049D47: 48 83 EC 20        sub         rsp,20h
  0000000065049D4B: 48 8B 02           mov         rax,qword ptr [rdx]
  0000000065049D4E: 48 8B EA           mov         rbp,rdx
  0000000065049D51: 4C 8B E1           mov         r12,rcx
  0000000065049D54: 48 39 01           cmp         qword ptr [rcx],rax
  0000000065049D57: 75 0E              jne         0000000065049D67
  0000000065049D59: B0 01              mov         al,1
  0000000065049D5B: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  0000000065049D60: 48 83 C4 20        add         rsp,20h
  0000000065049D64: 41 5C              pop         r12
  0000000065049D66: C3                 ret
  0000000065049D67: 48 89 5C 24 30     mov         qword ptr [rsp+30h],rbx
  0000000065049D6C: 48 89 74 24 38     mov         qword ptr [rsp+38h],rsi
  0000000065049D71: 48 89 7C 24 40     mov         qword ptr [rsp+40h],rdi
  0000000065049D76: 33 FF              xor         edi,edi
  0000000065049D78: 0F 1F 84 00 00 00  nop         dword ptr [rax+rax+0000000000000000h]
                    00 00
  0000000065049D80: 48 63 C7           movsxd      rax,edi
  0000000065049D83: 33 DB              xor         ebx,ebx
  0000000065049D85: 48 8D 34 80        lea         rsi,[rax+rax*4]
  0000000065049D89: 48 C1 E6 02        shl         rsi,2
  0000000065049D8D: 0F 1F 00           nop         dword ptr [rax]
  0000000065049D90: 48 8B 45 00        mov         rax,qword ptr [rbp]
  0000000065049D94: 48 63 CB           movsxd      rcx,ebx
  0000000065049D97: 48 03 CE           add         rcx,rsi
  0000000065049D9A: 48 8D 54 C8 08     lea         rdx,[rax+rcx*8+8]
  0000000065049D9F: 49 8B 04 24        mov         rax,qword ptr [r12]
  0000000065049DA3: 48 8D 4C C8 08     lea         rcx,[rax+rcx*8+8]
  0000000065049DA8: E8 03 25 0D 00     call        000000006511C2B0
  0000000065049DAD: 84 C0              test        al,al
  0000000065049DAF: 74 12              je          0000000065049DC3
  0000000065049DB1: FF C3              inc         ebx
  0000000065049DB3: 83 FB 14           cmp         ebx,14h
  0000000065049DB6: 7C D8              jl          0000000065049D90
  0000000065049DB8: FF C7              inc         edi
  0000000065049DBA: 83 FF 03           cmp         edi,3
  0000000065049DBD: 7C C1              jl          0000000065049D80
  0000000065049DBF: B0 01              mov         al,1
  0000000065049DC1: EB 02              jmp         0000000065049DC5
  0000000065049DC3: 32 C0              xor         al,al
  0000000065049DC5: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000065049DCA: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065049DCF: 48 8B 7C 24 40     mov         rdi,qword ptr [rsp+40h]
  0000000065049DD4: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  0000000065049DD9: 48 83 C4 20        add         rsp,20h
  0000000065049DDD: 41 5C              pop         r12
  0000000065049DDF: C3                 ret

  Summary

      695000 .text
