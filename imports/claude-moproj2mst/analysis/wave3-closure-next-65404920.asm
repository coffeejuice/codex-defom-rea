Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065404920: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000065404924: 53                 push        rbx
  0000000065404925: 55                 push        rbp
  0000000065404926: 56                 push        rsi
  0000000065404927: 57                 push        rdi
  0000000065404928: 41 54              push        r12
  000000006540492A: 41 55              push        r13
  000000006540492C: 41 56              push        r14
  000000006540492E: 41 57              push        r15
  0000000065404930: 48 83 EC 68        sub         rsp,68h
  0000000065404934: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  0000000065404938: 4C 8B E1           mov         r12,rcx
  000000006540493B: 48 81 C1 10 02 00  add         rcx,210h
                    00
  0000000065404942: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000065404947: FF 15 6B 1F 29 00  call        qword ptr [00000000656968B8h]
  000000006540494D: 40 B6 01           mov         sil,1
  0000000065404950: 85 C0              test        eax,eax
  0000000065404952: 44 8B F0           mov         r14d,eax
  0000000065404955: 40 0F 95 C7        setne       dil
  0000000065404959: 45 33 FF           xor         r15d,r15d
  000000006540495C: 41 8B EF           mov         ebp,r15d
  000000006540495F: 85 C0              test        eax,eax
  0000000065404961: 7E 74              jle         00000000654049D7
  0000000065404963: 66 66 66 66 66 0F  nop         word ptr [rax+rax+0000000000000000h]
                    1F 84 00 00 00 00
                    00
  0000000065404970: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  0000000065404978: 8B D5              mov         edx,ebp
  000000006540497A: FF 15 C0 1F 29 00  call        qword ptr [0000000065696940h]
  0000000065404980: 4C 8B 00           mov         r8,qword ptr [rax]
  0000000065404983: 49 8B 40 18        mov         rax,qword ptr [r8+18h]
  0000000065404987: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  000000006540498B: 0F B6 59 0A        movzx       ebx,byte ptr [rcx+0Ah]
  000000006540498F: 80 E3 01           and         bl,1
  0000000065404992: 40 84 FF           test        dil,dil
  0000000065404995: 74 11              je          00000000654049A8
  0000000065404997: 84 DB              test        bl,bl
  0000000065404999: 75 0D              jne         00000000654049A8
  000000006540499B: 41 F6 40 14 01     test        byte ptr [r8+14h],1
  00000000654049A0: 40 0F B6 FF        movzx       edi,dil
  00000000654049A4: 41 0F 44 FF        cmove       edi,r15d
  00000000654049A8: 40 84 F6           test        sil,sil
  00000000654049AB: 75 08              jne         00000000654049B5
  00000000654049AD: 84 DB              test        bl,bl
  00000000654049AF: 75 04              jne         00000000654049B5
  00000000654049B1: 32 D2              xor         dl,dl
  00000000654049B3: EB 02              jmp         00000000654049B7
  00000000654049B5: B2 01              mov         dl,1
  00000000654049B7: 49 8B 48 20        mov         rcx,qword ptr [r8+20h]
  00000000654049BB: 84 D2              test        dl,dl
  00000000654049BD: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000654049C0: 0F 94 C2           sete        dl
  00000000654049C3: FF 50 58           call        qword ptr [rax+58h]
  00000000654049C6: 84 DB              test        bl,bl
  00000000654049C8: 40 0F B6 F6        movzx       esi,sil
  00000000654049CC: 41 0F 44 F7        cmove       esi,r15d
  00000000654049D0: FF C5              inc         ebp
  00000000654049D2: 41 3B EE           cmp         ebp,r14d
  00000000654049D5: 7C 99              jl          0000000065404970
  00000000654049D7: 40 84 FF           test        dil,dil
  00000000654049DA: 74 42              je          0000000065404A1E
  00000000654049DC: 41 8B DF           mov         ebx,r15d
  00000000654049DF: 45 85 F6           test        r14d,r14d
  00000000654049E2: 7E 3A              jle         0000000065404A1E
  00000000654049E4: 66 66 66 66 0F 1F  nop         word ptr [rax+rax+0000000000000000h]
                    84 00 00 00 00 00
  00000000654049F0: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  00000000654049F8: 8B D3              mov         edx,ebx
  00000000654049FA: FF 15 40 1F 29 00  call        qword ptr [0000000065696940h]
  0000000065404A00: 48 8B 10           mov         rdx,qword ptr [rax]
  0000000065404A03: 48 8B 42 18        mov         rax,qword ptr [rdx+18h]
  0000000065404A07: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  0000000065404A0B: F6 41 0A 01        test        byte ptr [rcx+0Ah],1
  0000000065404A0F: 74 09              je          0000000065404A1A
  0000000065404A11: FF C3              inc         ebx
  0000000065404A13: 41 3B DE           cmp         ebx,r14d
  0000000065404A16: 7C D8              jl          00000000654049F0
  0000000065404A18: EB 04              jmp         0000000065404A1E
  0000000065404A1A: 83 62 14 FE        and         dword ptr [rdx+14h],0FFFFFFFEh
  0000000065404A1E: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000065404A23: E8 B8 DB FA FF     call        00000000653B25E0
  0000000065404A28: BD FF FF FF 00     mov         ebp,0FFFFFFh
  0000000065404A2D: C6 84 24 B0 00 00  mov         byte ptr [rsp+00000000000000B0h],1
                    00 01
  0000000065404A35: 8B F8              mov         edi,eax
  0000000065404A37: 89 6C 24 24        mov         dword ptr [rsp+24h],ebp
  0000000065404A3B: 45 8B EF           mov         r13d,r15d
  0000000065404A3E: 03 FF              add         edi,edi
  0000000065404A40: 8B DF              mov         ebx,edi
  0000000065404A42: 89 7C 24 28        mov         dword ptr [rsp+28h],edi
  0000000065404A46: 8B F7              mov         esi,edi
  0000000065404A48: 89 9C 24 C8 00 00  mov         dword ptr [rsp+00000000000000C8h],ebx
                    00
  0000000065404A4F: 89 BC 24 C0 00 00  mov         dword ptr [rsp+00000000000000C0h],edi
                    00
  0000000065404A56: 45 85 F6           test        r14d,r14d
  0000000065404A59: 0F 8E E9 01 00 00  jle         0000000065404C48
  0000000065404A5F: 44 8B FF           mov         r15d,edi
  0000000065404A62: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  0000000065404A6A: 41 8B D5           mov         edx,r13d
  0000000065404A6D: FF 15 CD 1E 29 00  call        qword ptr [0000000065696940h]
  0000000065404A73: 48 8B 30           mov         rsi,qword ptr [rax]
  0000000065404A76: 48 8B 46 18        mov         rax,qword ptr [rsi+18h]
  0000000065404A7A: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  0000000065404A7E: F6 41 0A 01        test        byte ptr [rcx+0Ah],1
  0000000065404A82: 0F 85 74 01 00 00  jne         0000000065404BFC
  0000000065404A88: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  0000000065404A8C: C6 84 24 B0 00 00  mov         byte ptr [rsp+00000000000000B0h],0
                    00 00
  0000000065404A94: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  0000000065404A98: F6 41 0A 01        test        byte ptr [rcx+0Ah],1
  0000000065404A9C: 75 69              jne         0000000065404B07
  0000000065404A9E: 41 8B 9C 24 18 02  mov         ebx,dword ptr [r12+0000000000000218h]
                    00 00
  0000000065404AA6: 48 8B C8           mov         rcx,rax
  0000000065404AA9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000065404AAC: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000065404AB1: FF 50 60           call        qword ptr [rax+60h]
  0000000065404AB4: 48 8B C8           mov         rcx,rax
  0000000065404AB7: 83 FB 01           cmp         ebx,1
  0000000065404ABA: 75 08              jne         0000000065404AC4
  0000000065404ABC: FF 15 4E 1E 29 00  call        qword ptr [0000000065696910h]
  0000000065404AC2: EB 06              jmp         0000000065404ACA
  0000000065404AC4: FF 15 4E 1E 29 00  call        qword ptr [0000000065696918h]
  0000000065404ACA: 48 8B 4E 20        mov         rcx,qword ptr [rsi+20h]
  0000000065404ACE: 01 84 24 C8 00 00  add         dword ptr [rsp+00000000000000C8h],eax
                    00
  0000000065404AD5: 41 8B 9C 24 18 02  mov         ebx,dword ptr [r12+0000000000000218h]
                    00 00
  0000000065404ADD: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065404AE0: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000065404AE5: FF 50 60           call        qword ptr [rax+60h]
  0000000065404AE8: 48 8B C8           mov         rcx,rax
  0000000065404AEB: 83 FB 01           cmp         ebx,1
  0000000065404AEE: 75 08              jne         0000000065404AF8
  0000000065404AF0: FF 15 1A 1E 29 00  call        qword ptr [0000000065696910h]
  0000000065404AF6: EB 06              jmp         0000000065404AFE
  0000000065404AF8: FF 15 1A 1E 29 00  call        qword ptr [0000000065696918h]
  0000000065404AFE: 8B 9C 24 C8 00 00  mov         ebx,dword ptr [rsp+00000000000000C8h]
                    00
  0000000065404B05: 03 F8              add         edi,eax
  0000000065404B07: 48 8B 56 18        mov         rdx,qword ptr [rsi+18h]
  0000000065404B0B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000065404B10: E8 0B 28 C4 FF     call        0000000065047320
  0000000065404B15: 41 83 BC 24 18 02  cmp         dword ptr [r12+0000000000000218h],1
                    00 00 01
  0000000065404B1E: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000065404B23: 75 08              jne         0000000065404B2D
  0000000065404B25: FF 15 E5 1D 29 00  call        qword ptr [0000000065696910h]
  0000000065404B2B: EB 06              jmp         0000000065404B33
  0000000065404B2D: FF 15 E5 1D 29 00  call        qword ptr [0000000065696918h]
  0000000065404B33: 48 8B 4E 18        mov         rcx,qword ptr [rsi+18h]
  0000000065404B37: 03 D8              add         ebx,eax
  0000000065404B39: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000065404B3E: 89 9C 24 C8 00 00  mov         dword ptr [rsp+00000000000000C8h],ebx
                    00
  0000000065404B45: E8 96 8B C5 FF     call        000000006505D6E0
  0000000065404B4A: 41 83 BC 24 18 02  cmp         dword ptr [r12+0000000000000218h],1
                    00 00 01
  0000000065404B53: 48 8B C8           mov         rcx,rax
  0000000065404B56: 75 08              jne         0000000065404B60
  0000000065404B58: FF 15 B2 1D 29 00  call        qword ptr [0000000065696910h]
  0000000065404B5E: EB 06              jmp         0000000065404B66
  0000000065404B60: FF 15 B2 1D 29 00  call        qword ptr [0000000065696918h]
  0000000065404B66: 03 F8              add         edi,eax
  0000000065404B68: 41 83 BC 24 18 02  cmp         dword ptr [r12+0000000000000218h],2
                    00 00 02
  0000000065404B71: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000065404B76: 89 7C 24 28        mov         dword ptr [rsp+28h],edi
  0000000065404B7A: 75 08              jne         0000000065404B84
  0000000065404B7C: FF 15 8E 1D 29 00  call        qword ptr [0000000065696910h]
  0000000065404B82: EB 06              jmp         0000000065404B8A
  0000000065404B84: FF 15 8E 1D 29 00  call        qword ptr [0000000065696918h]
  0000000065404B8A: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000065404B8E: 44 8B D8           mov         r11d,eax
  0000000065404B91: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000065404B99: 45 3B FB           cmp         r15d,r11d
  0000000065404B9C: 48 8D 44 24 20     lea         rax,[rsp+20h]
  0000000065404BA1: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  0000000065404BA6: 48 0F 4D C1        cmovge      rax,rcx
  0000000065404BAA: 48 8B 4E 18        mov         rcx,qword ptr [rsi+18h]
  0000000065404BAE: 44 8B 38           mov         r15d,dword ptr [rax]
  0000000065404BB1: 44 89 BC 24 C0 00  mov         dword ptr [rsp+00000000000000C0h],r15d
                    00 00
  0000000065404BB9: E8 22 8B C5 FF     call        000000006505D6E0
  0000000065404BBE: 41 83 BC 24 18 02  cmp         dword ptr [r12+0000000000000218h],2
                    00 00 02
  0000000065404BC7: 48 8B C8           mov         rcx,rax
  0000000065404BCA: 75 08              jne         0000000065404BD4
  0000000065404BCC: FF 15 3E 1D 29 00  call        qword ptr [0000000065696910h]
  0000000065404BD2: EB 06              jmp         0000000065404BDA
  0000000065404BD4: FF 15 3E 1D 29 00  call        qword ptr [0000000065696918h]
  0000000065404BDA: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000065404BDE: 44 8B D8           mov         r11d,eax
  0000000065404BE1: 85 C0              test        eax,eax
  0000000065404BE3: 7E 17              jle         0000000065404BFC
  0000000065404BE5: 41 3B EB           cmp         ebp,r11d
  0000000065404BE8: 48 8D 44 24 24     lea         rax,[rsp+24h]
  0000000065404BED: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000065404BF2: 48 0F 4D C1        cmovge      rax,rcx
  0000000065404BF6: 8B 28              mov         ebp,dword ptr [rax]
  0000000065404BF8: 89 6C 24 24        mov         dword ptr [rsp+24h],ebp
  0000000065404BFC: 41 FF C5           inc         r13d
  0000000065404BFF: 45 3B EE           cmp         r13d,r14d
  0000000065404C02: 0F 8C 5A FE FF FF  jl          0000000065404A62
  0000000065404C08: 0F B6 84 24 B0 00  movzx       eax,byte ptr [rsp+00000000000000B0h]
                    00 00
  0000000065404C10: 8B B4 24 C0 00 00  mov         esi,dword ptr [rsp+00000000000000C0h]
                    00
  0000000065404C17: 41 BF 00 00 00 00  mov         r15d,0
  0000000065404C1D: 84 C0              test        al,al
  0000000065404C1F: 75 27              jne         0000000065404C48
  0000000065404C21: 48 8D 44 24 28     lea         rax,[rsp+28h]
  0000000065404C26: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000065404C2E: 81 FF FF FF FF 00  cmp         edi,0FFFFFFh
  0000000065404C34: 48 0F 4D C1        cmovge      rax,rcx
  0000000065404C38: C7 84 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],0FFFFFFh
                    00 FF FF FF 00
  0000000065404C43: 44 8B 38           mov         r15d,dword ptr [rax]
  0000000065404C46: EB 22              jmp         0000000065404C6A
  0000000065404C48: 49 8B 54 24 10     mov         rdx,qword ptr [r12+10h]
  0000000065404C4D: 48 8D 0D 5C 37 35  lea         rcx,[00000000657583B0h]
                    00
  0000000065404C54: FF 15 AE 1D 29 00  call        qword ptr [0000000065696A08h]
  0000000065404C5A: 48 85 C0           test        rax,rax
  0000000065404C5D: 74 05              je          0000000065404C64
  0000000065404C5F: 41 8B EF           mov         ebp,r15d
  0000000065404C62: EB 06              jmp         0000000065404C6A
  0000000065404C64: 41 BF FF FF FF 00  mov         r15d,0FFFFFFh
  0000000065404C6A: 3B EE              cmp         ebp,esi
  0000000065404C6C: 0F 4C EE           cmovl       ebp,esi
  0000000065404C6F: 80 BC 24 B8 00 00  cmp         byte ptr [rsp+00000000000000B8h],0
                    00 00
  0000000065404C77: 74 64              je          0000000065404CDD
  0000000065404C79: 41 83 BC 24 18 02  cmp         dword ptr [r12+0000000000000218h],1
                    00 00 01
  0000000065404C82: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  0000000065404C87: 48 8B CF           mov         rcx,rdi
  0000000065404C8A: 75 1D              jne         0000000065404CA9
  0000000065404C8C: 44 8B C5           mov         r8d,ebp
  0000000065404C8F: 41 8B D7           mov         edx,r15d
  0000000065404C92: E8 19 A6 C6 FF     call        000000006506F2B0
  0000000065404C97: 4C 8B 5F 20        mov         r11,qword ptr [rdi+20h]
  0000000065404C9B: 41 F6 43 0C 01     test        byte ptr [r11+0Ch],1
  0000000065404CA0: 74 29              je          0000000065404CCB
  0000000065404CA2: 44 8B C6           mov         r8d,esi
  0000000065404CA5: 8B D3              mov         edx,ebx
  0000000065404CA7: EB 1A              jmp         0000000065404CC3
  0000000065404CA9: 45 8B C7           mov         r8d,r15d
  0000000065404CAC: 8B D5              mov         edx,ebp
  0000000065404CAE: E8 FD A5 C6 FF     call        000000006506F2B0
  0000000065404CB3: 4C 8B 5F 20        mov         r11,qword ptr [rdi+20h]
  0000000065404CB7: 41 F6 43 0C 01     test        byte ptr [r11+0Ch],1
  0000000065404CBC: 74 0D              je          0000000065404CCB
  0000000065404CBE: 44 8B C3           mov         r8d,ebx
  0000000065404CC1: 8B D6              mov         edx,esi
  0000000065404CC3: 48 8B CF           mov         rcx,rdi
  0000000065404CC6: E8 35 A4 C6 FF     call        000000006506F100
  0000000065404CCB: 49 8B CC           mov         rcx,r12
  0000000065404CCE: E8 2D F2 FF FF     call        0000000065403F00
  0000000065404CD3: 48 8B CF           mov         rcx,rdi
  0000000065404CD6: E8 B5 D8 C5 FF     call        0000000065062590
  0000000065404CDB: EB 09              jmp         0000000065404CE6
  0000000065404CDD: 41 C6 84 24 1D 02  mov         byte ptr [r12+000000000000021Dh],1
                    00 00 01
  0000000065404CE6: 48 83 C4 68        add         rsp,68h
  0000000065404CEA: 41 5F              pop         r15
  0000000065404CEC: 41 5E              pop         r14
  0000000065404CEE: 41 5D              pop         r13
  0000000065404CF0: 41 5C              pop         r12
  0000000065404CF2: 5F                 pop         rdi
  0000000065404CF3: 5E                 pop         rsi
  0000000065404CF4: 5D                 pop         rbp
  0000000065404CF5: 5B                 pop         rbx
  0000000065404CF6: C3                 ret

  Summary

      695000 .text
