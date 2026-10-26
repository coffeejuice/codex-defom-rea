Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000671196D0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000671196D5: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  00000000671196DA: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  00000000671196DF: 57                 push        rdi
  00000000671196E0: 48 83 EC 20        sub         rsp,20h
  00000000671196E4: 49 8B E8           mov         rbp,r8
  00000000671196E7: 48 8B DA           mov         rbx,rdx
  00000000671196EA: 48 3B 51 08        cmp         rdx,qword ptr [rcx+8]
  00000000671196EE: 74 7E              je          000000006711976E
  00000000671196F0: 4C 8B 4A 08        mov         r9,qword ptr [rdx+8]
  00000000671196F4: 33 F6              xor         esi,esi
  00000000671196F6: 49 8B 49 70        mov         rcx,qword ptr [r9+70h]
  00000000671196FA: 48 8B FE           mov         rdi,rsi
  00000000671196FD: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  0000000067119700: 2B 41 08           sub         eax,dword ptr [rcx+8]
  0000000067119703: 85 C0              test        eax,eax
  0000000067119705: 7E 67              jle         000000006711976E
  0000000067119707: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  0000000067119710: 49 8B 49 70        mov         rcx,qword ptr [r9+70h]
  0000000067119714: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  0000000067119718: 48 03 C7           add         rax,rdi
  000000006711971B: 48 8B 44 C1 18     mov         rax,qword ptr [rcx+rax*8+18h]
  0000000067119720: 4C 8B 10           mov         r10,qword ptr [rax]
  0000000067119723: 4D 85 D2           test        r10,r10
  0000000067119726: 74 2F              je          0000000067119757
  0000000067119728: 49 8B 4A 08        mov         rcx,qword ptr [r10+8]
  000000006711972C: 49 8B 41 40        mov         rax,qword ptr [r9+40h]
  0000000067119730: 48 39 41 40        cmp         qword ptr [rcx+40h],rax
  0000000067119734: 74 0E              je          0000000067119744
  0000000067119736: 48 8D 0D 73 03 12  lea         rcx,[0000000067239AB0h]
                    00
  000000006711973D: E8 9E 37 F0 FF     call        000000006701CEE0
  0000000067119742: EB 13              jmp         0000000067119757
  0000000067119744: 49 8B 02           mov         rax,qword ptr [r10]
  0000000067119747: 4C 8B C5           mov         r8,rbp
  000000006711974A: 48 8B D3           mov         rdx,rbx
  000000006711974D: 49 8B CA           mov         rcx,r10
  0000000067119750: FF 50 28           call        qword ptr [rax+28h]
  0000000067119753: 84 C0              test        al,al
  0000000067119755: 75 2E              jne         0000000067119785
  0000000067119757: 4C 8B 4B 08        mov         r9,qword ptr [rbx+8]
  000000006711975B: FF C6              inc         esi
  000000006711975D: 48 FF C7           inc         rdi
  0000000067119760: 49 8B 41 70        mov         rax,qword ptr [r9+70h]
  0000000067119764: 8B 48 0C           mov         ecx,dword ptr [rax+0Ch]
  0000000067119767: 2B 48 08           sub         ecx,dword ptr [rax+8]
  000000006711976A: 3B F1              cmp         esi,ecx
  000000006711976C: 7C A2              jl          0000000067119710
  000000006711976E: 32 C0              xor         al,al
  0000000067119770: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000067119775: 48 8B 6C 24 38     mov         rbp,qword ptr [rsp+38h]
  000000006711977A: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  000000006711977F: 48 83 C4 20        add         rsp,20h
  0000000067119783: 5F                 pop         rdi
  0000000067119784: C3                 ret
  0000000067119785: B0 01              mov         al,1
  0000000067119787: EB E7              jmp         0000000067119770

  Summary

      1A7000 .text
