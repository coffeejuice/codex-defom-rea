Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067119880: 40 53              push        rbx
  0000000067119882: 48 83 EC 20        sub         rsp,20h
  0000000067119886: 44 0F B7 52 10     movzx       r10d,word ptr [rdx+10h]
  000000006711988B: 4D 8B D8           mov         r11,r8
  000000006711988E: 48 8B DA           mov         rbx,rdx
  0000000067119891: 66 41 83 FA 01     cmp         r10w,1
  0000000067119896: 75 72              jne         000000006711990A
  0000000067119898: 49 8B 40 08        mov         rax,qword ptr [r8+8]
  000000006711989C: 83 78 24 00        cmp         dword ptr [rax+24h],0
  00000000671198A0: 7E 68              jle         000000006711990A
  00000000671198A2: 4D 8B 01           mov         r8,qword ptr [r9]
  00000000671198A5: 44 8B 52 18        mov         r10d,dword ptr [rdx+18h]
  00000000671198A9: 33 C9              xor         ecx,ecx
  00000000671198AB: 49 63 40 08        movsxd      rax,dword ptr [r8+8]
  00000000671198AF: 45 8B 48 0C        mov         r9d,dword ptr [r8+0Ch]
  00000000671198B3: 44 2B C8           sub         r9d,eax
  00000000671198B6: 45 85 C9           test        r9d,r9d
  00000000671198B9: 0F 8E A4 00 00 00  jle         0000000067119963
  00000000671198BF: 49 8D 54 C0 18     lea         rdx,[r8+rax*8+18h]
  00000000671198C4: 48 8B 02           mov         rax,qword ptr [rdx]
  00000000671198C7: 4C 39 18           cmp         qword ptr [rax],r11
  00000000671198CA: 75 16              jne         00000000671198E2
  00000000671198CC: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000671198D0: 48 85 C0           test        rax,rax
  00000000671198D3: 74 0D              je          00000000671198E2
  00000000671198D5: 66 83 78 10 01     cmp         word ptr [rax+10h],1
  00000000671198DA: 75 06              jne         00000000671198E2
  00000000671198DC: 44 39 50 18        cmp         dword ptr [rax+18h],r10d
  00000000671198E0: 74 13              je          00000000671198F5
  00000000671198E2: FF C1              inc         ecx
  00000000671198E4: 48 83 C2 08        add         rdx,8
  00000000671198E8: 41 3B C9           cmp         ecx,r9d
  00000000671198EB: 7C D7              jl          00000000671198C4
  00000000671198ED: 32 C0              xor         al,al
  00000000671198EF: 48 83 C4 20        add         rsp,20h
  00000000671198F3: 5B                 pop         rbx
  00000000671198F4: C3                 ret
  00000000671198F5: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000671198F8: BA 01 00 00 00     mov         edx,1
  00000000671198FD: 48 8B CB           mov         rcx,rbx
  0000000067119900: FF 10              call        qword ptr [rax]
  0000000067119902: B0 01              mov         al,1
  0000000067119904: 48 83 C4 20        add         rsp,20h
  0000000067119908: 5B                 pop         rbx
  0000000067119909: C3                 ret
  000000006711990A: 66 41 83 FA 34     cmp         r10w,34h
  000000006711990F: 74 07              je          0000000067119918
  0000000067119911: 66 41 83 FA 14     cmp         r10w,14h
  0000000067119916: 75 4B              jne         0000000067119963
  0000000067119918: 49 8B 40 08        mov         rax,qword ptr [r8+8]
  000000006711991C: 83 78 24 00        cmp         dword ptr [rax+24h],0
  0000000067119920: 7E 41              jle         0000000067119963
  0000000067119922: 49 8B 11           mov         rdx,qword ptr [r9]
  0000000067119925: 33 C9              xor         ecx,ecx
  0000000067119927: 48 63 42 08        movsxd      rax,dword ptr [rdx+8]
  000000006711992B: 44 8B 4A 0C        mov         r9d,dword ptr [rdx+0Ch]
  000000006711992F: 44 2B C8           sub         r9d,eax
  0000000067119932: 45 85 C9           test        r9d,r9d
  0000000067119935: 7E 2C              jle         0000000067119963
  0000000067119937: 48 8D 54 C2 18     lea         rdx,[rdx+rax*8+18h]
  000000006711993C: 0F 1F 40 00        nop         dword ptr [rax]
  0000000067119940: 48 8B 02           mov         rax,qword ptr [rdx]
  0000000067119943: 4C 39 18           cmp         qword ptr [rax],r11
  0000000067119946: 75 10              jne         0000000067119958
  0000000067119948: 4C 8B 40 08        mov         r8,qword ptr [rax+8]
  000000006711994C: 4D 85 C0           test        r8,r8
  000000006711994F: 74 07              je          0000000067119958
  0000000067119951: 66 45 39 50 10     cmp         word ptr [r8+10h],r10w
  0000000067119956: 74 9D              je          00000000671198F5
  0000000067119958: FF C1              inc         ecx
  000000006711995A: 48 83 C2 08        add         rdx,8
  000000006711995E: 41 3B C9           cmp         ecx,r9d
  0000000067119961: 7C DD              jl          0000000067119940
  0000000067119963: 32 C0              xor         al,al
  0000000067119965: 48 83 C4 20        add         rsp,20h
  0000000067119969: 5B                 pop         rbx
  000000006711996A: C3                 ret

  Summary

      1A7000 .text
