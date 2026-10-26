Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067119600: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000067119605: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  000000006711960A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  000000006711960F: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  0000000067119614: 41 54              push        r12
  0000000067119616: 48 83 EC 20        sub         rsp,20h
  000000006711961A: 4C 8B 4A 08        mov         r9,qword ptr [rdx+8]
  000000006711961E: 48 8B 41 40        mov         rax,qword ptr [rcx+40h]
  0000000067119622: 4D 8B E0           mov         r12,r8
  0000000067119625: 48 8B EA           mov         rbp,rdx
  0000000067119628: 48 8B D9           mov         rbx,rcx
  000000006711962B: 49 39 41 40        cmp         qword ptr [r9+40h],rax
  000000006711962F: 75 79              jne         00000000671196AA
  0000000067119631: 4C 8B 49 70        mov         r9,qword ptr [rcx+70h]
  0000000067119635: 33 FF              xor         edi,edi
  0000000067119637: 41 8B 41 0C        mov         eax,dword ptr [r9+0Ch]
  000000006711963B: 48 8B F7           mov         rsi,rdi
  000000006711963E: 41 2B 41 08        sub         eax,dword ptr [r9+8]
  0000000067119642: 85 C0              test        eax,eax
  0000000067119644: 7E 64              jle         00000000671196AA
  0000000067119646: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  0000000067119650: 48 8B 4B 70        mov         rcx,qword ptr [rbx+70h]
  0000000067119654: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  0000000067119658: 48 03 C6           add         rax,rsi
  000000006711965B: 48 8B 44 C1 18     mov         rax,qword ptr [rcx+rax*8+18h]
  0000000067119660: 4C 8B 08           mov         r9,qword ptr [rax]
  0000000067119663: 4D 85 C9           test        r9,r9
  0000000067119666: 74 2F              je          0000000067119697
  0000000067119668: 49 8B 49 08        mov         rcx,qword ptr [r9+8]
  000000006711966C: 48 8B 43 40        mov         rax,qword ptr [rbx+40h]
  0000000067119670: 48 39 41 40        cmp         qword ptr [rcx+40h],rax
  0000000067119674: 74 0E              je          0000000067119684
  0000000067119676: 48 8D 0D E3 03 12  lea         rcx,[0000000067239A60h]
                    00
  000000006711967D: E8 5E 38 F0 FF     call        000000006701CEE0
  0000000067119682: EB 13              jmp         0000000067119697
  0000000067119684: 49 8B 01           mov         rax,qword ptr [r9]
  0000000067119687: 4D 8B C4           mov         r8,r12
  000000006711968A: 48 8B D5           mov         rdx,rbp
  000000006711968D: 49 8B C9           mov         rcx,r9
  0000000067119690: FF 50 28           call        qword ptr [rax+28h]
  0000000067119693: 84 C0              test        al,al
  0000000067119695: 75 30              jne         00000000671196C7
  0000000067119697: 48 8B 43 70        mov         rax,qword ptr [rbx+70h]
  000000006711969B: FF C7              inc         edi
  000000006711969D: 48 FF C6           inc         rsi
  00000000671196A0: 8B 48 0C           mov         ecx,dword ptr [rax+0Ch]
  00000000671196A3: 2B 48 08           sub         ecx,dword ptr [rax+8]
  00000000671196A6: 3B F9              cmp         edi,ecx
  00000000671196A8: 7C A6              jl          0000000067119650
  00000000671196AA: 32 C0              xor         al,al
  00000000671196AC: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000671196B1: 48 8B 6C 24 38     mov         rbp,qword ptr [rsp+38h]
  00000000671196B6: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  00000000671196BB: 48 8B 7C 24 48     mov         rdi,qword ptr [rsp+48h]
  00000000671196C0: 48 83 C4 20        add         rsp,20h
  00000000671196C4: 41 5C              pop         r12
  00000000671196C6: C3                 ret
  00000000671196C7: B0 01              mov         al,1
  00000000671196C9: EB E1              jmp         00000000671196AC

  Summary

      1A7000 .text
