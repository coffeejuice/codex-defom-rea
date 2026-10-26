Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067053320: 48 8B C4           mov         rax,rsp
  0000000067053323: 4C 89 48 20        mov         qword ptr [rax+20h],r9
  0000000067053327: 48 89 50 10        mov         qword ptr [rax+10h],rdx
  000000006705332B: 48 89 48 08        mov         qword ptr [rax+8],rcx
  000000006705332F: 55                 push        rbp
  0000000067053330: 56                 push        rsi
  0000000067053331: 41 57              push        r15
  0000000067053333: 48 83 EC 60        sub         rsp,60h
  0000000067053337: 48 89 58 E0        mov         qword ptr [rax-20h],rbx
  000000006705333B: 48 89 78 D8        mov         qword ptr [rax-28h],rdi
  000000006705333F: 4C 89 68 C8        mov         qword ptr [rax-38h],r13
  0000000067053343: 4C 89 70 C0        mov         qword ptr [rax-40h],r14
  0000000067053347: 45 8B E8           mov         r13d,r8d
  000000006705334A: 4D 8B F9           mov         r15,r9
  000000006705334D: 48 8B F9           mov         rdi,rcx
  0000000067053350: 0F B6 1F           movzx       ebx,byte ptr [rdi]
  0000000067053353: 48 FF C7           inc         rdi
  0000000067053356: 8B CB              mov         ecx,ebx
  0000000067053358: FF 15 FA 52 15 00  call        qword ptr [00000000671A8658h]
  000000006705335E: 85 C0              test        eax,eax
  0000000067053360: 75 EE              jne         0000000067053350
  0000000067053362: 33 F6              xor         esi,esi
  0000000067053364: 80 FB 2D           cmp         bl,2Dh
  0000000067053367: 75 14              jne         000000006705337D
  0000000067053369: 0F B6 1F           movzx       ebx,byte ptr [rdi]
  000000006705336C: B9 01 00 00 00     mov         ecx,1
  0000000067053371: 48 FF C7           inc         rdi
  0000000067053374: 89 8C 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],ecx
                    00
  000000006705337B: EB 13              jmp         0000000067053390
  000000006705337D: 8B CE              mov         ecx,esi
  000000006705337F: 89 8C 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],ecx
                    00
  0000000067053386: 80 FB 2B           cmp         bl,2Bh
  0000000067053389: 75 05              jne         0000000067053390
  000000006705338B: 8A 1F              mov         bl,byte ptr [rdi]
  000000006705338D: 48 FF C7           inc         rdi
  0000000067053390: 45 85 ED           test        r13d,r13d
  0000000067053393: 74 06              je          000000006705339B
  0000000067053395: 41 83 FD 10        cmp         r13d,10h
  0000000067053399: 75 1F              jne         00000000670533BA
  000000006705339B: 80 FB 30           cmp         bl,30h
  000000006705339E: 75 1A              jne         00000000670533BA
  00000000670533A0: 0F B6 07           movzx       eax,byte ptr [rdi]
  00000000670533A3: 3C 78              cmp         al,78h
  00000000670533A5: 74 04              je          00000000670533AB
  00000000670533A7: 3C 58              cmp         al,58h
  00000000670533A9: 75 0F              jne         00000000670533BA
  00000000670533AB: 8A 5F 01           mov         bl,byte ptr [rdi+1]
  00000000670533AE: 48 83 C7 02        add         rdi,2
  00000000670533B2: 41 BD 10 00 00 00  mov         r13d,10h
  00000000670533B8: EB 17              jmp         00000000670533D1
  00000000670533BA: 45 85 ED           test        r13d,r13d
  00000000670533BD: 75 12              jne         00000000670533D1
  00000000670533BF: B8 08 00 00 00     mov         eax,8
  00000000670533C4: 41 BD 0A 00 00 00  mov         r13d,0Ah
  00000000670533CA: 80 FB 30           cmp         bl,30h
  00000000670533CD: 44 0F 44 E8        cmove       r13d,eax
  00000000670533D1: 85 C9              test        ecx,ecx
  00000000670533D3: 48 8B C6           mov         rax,rsi
  00000000670533D6: 48 BA FF FF FF FF  mov         rdx,7FFFFFFFFFFFFFFFh
                    FF FF FF 7F
  00000000670533E0: 0F 95 C0           setne       al
  00000000670533E3: 45 8B C5           mov         r8d,r13d
  00000000670533E6: 8B EE              mov         ebp,esi
  00000000670533E8: 48 03 C2           add         rax,rdx
  00000000670533EB: 33 D2              xor         edx,edx
  00000000670533ED: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000000670533F2: 49 F7 F0           div         rax,r8
  00000000670533F5: 48 89 54 24 20     mov         qword ptr [rsp+20h],rdx
  00000000670533FA: 4C 8B F0           mov         r14,rax
  00000000670533FD: 80 FB 80           cmp         bl,80h
  0000000067053400: 0F 83 AB 00 00 00  jae         00000000670534B1
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
  000000006705349A: 48 8B 74 24 28     mov         rsi,qword ptr [rsp+28h]
  000000006705349F: 4D 85 FF           test        r15,r15
  00000000670534A2: 74 17              je          00000000670534BB
  00000000670534A4: 41 C6 07 00        mov         byte ptr [r15],0
  00000000670534A8: EB 11              jmp         00000000670534BB
  00000000670534AA: 8B 8C 24 90 00 00  mov         ecx,dword ptr [rsp+0000000000000090h]
                    00
  00000000670534B1: 85 C9              test        ecx,ecx
  00000000670534B3: 74 06              je          00000000670534BB
  00000000670534B5: 48 F7 D6           not         rsi
  00000000670534B8: 48 FF C6           inc         rsi
  00000000670534BB: 48
  00000000670534BC: 8B
  00000000670534BD: 84 24 88           test        byte ptr [rax+rcx*4],ah
  00000000670534C0: 00

  Summary

      1A7000 .text
