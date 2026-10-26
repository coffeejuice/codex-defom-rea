Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

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

  Summary

      1A7000 .text
