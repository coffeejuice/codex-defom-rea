Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067144490: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000067144495: 55                 push        rbp
  0000000067144496: 56                 push        rsi
  0000000067144497: 57                 push        rdi
  0000000067144498: 41 54              push        r12
  000000006714449A: 41 55              push        r13
  000000006714449C: 41 56              push        r14
  000000006714449E: 41 57              push        r15
  00000000671444A0: B8 50 33 00 00     mov         eax,3350h
  00000000671444A5: E8 C6 50 03 00     call        0000000067179570
  00000000671444AA: 48 2B E0           sub         rsp,rax
  00000000671444AD: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000000671444B9: 48 89 9C 24 98 33  mov         qword ptr [rsp+0000000000003398h],rbx
                    00 00
  00000000671444C1: 4C 8B FA           mov         r15,rdx
  00000000671444C4: 48 8B D9           mov         rbx,rcx
  00000000671444C7: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  00000000671444CB: 49 83 BC 24 98 00  cmp         qword ptr [r12+0000000000000098h],0
                    00 00 00
  00000000671444D4: 75 05              jne         00000000671444DB
  00000000671444D6: E8 F5 FC FF FF     call        00000000671441D0
  00000000671444DB: 41 C6 84 24 94 00  mov         byte ptr [r12+0000000000000094h],0
                    00 00 00
  00000000671444E4: 48 8B CB           mov         rcx,rbx
  00000000671444E7: E8 F4 DC 02 00     call        00000000671721E0
  00000000671444EC: 45 32 ED           xor         r13b,r13b
  00000000671444EF: 45 32 F6           xor         r14b,r14b
  00000000671444F2: 32 C0              xor         al,al
  00000000671444F4: 88 84 24 A0 33 00  mov         byte ptr [rsp+00000000000033A0h],al
                    00
  00000000671444FB: BD FD FF 00 00     mov         ebp,0FFFDh
  0000000067144500: C7 84 24 34 03 00  mov         dword ptr [rsp+0000000000000334h],0
                    00 00 00 00 00
  000000006714450B: 48 8D 84 24 40 03  lea         rax,[rsp+0000000000000340h]
                    00 00
  0000000067144513: 48 89 84 24 38 03  mov         qword ptr [rsp+0000000000000338h],rax
                    00 00
  000000006714451B: C7 84 24 30 03 00  mov         dword ptr [rsp+0000000000000330h],100h
                    00 00 01 00 00
  0000000067144526: 41 80 BC 24 94 00  cmp         byte ptr [r12+0000000000000094h],0
                    00 00 00
  000000006714452F: 0F 85 7D 05 00 00  jne         0000000067144AB2
  0000000067144535: 66 66 66 0F 1F 84  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00 00
  0000000067144540: 49 8B 84 24 E0 00  mov         rax,qword ptr [r12+00000000000000E0h]
                    00 00
  0000000067144548: 8B 70 0C           mov         esi,dword ptr [rax+0Ch]
  000000006714454B: 2B 70 08           sub         esi,dword ptr [rax+8]
  000000006714454E: 41 8B 17           mov         edx,dword ptr [r15]
  0000000067144551: 8B C2              mov         eax,edx
  0000000067144553: 83 E0 01           and         eax,1
  0000000067144556: 0F 85 0E 01 00 00  jne         000000006714466A
  000000006714455C: 49 8B 8C 24 E8 00  mov         rcx,qword ptr [r12+00000000000000E8h]
                    00 00
  0000000067144564: 8B 41 08           mov         eax,dword ptr [rcx+8]
  0000000067144567: 39 41 0C           cmp         dword ptr [rcx+0Ch],eax
  000000006714456A: 0F 84 FA 00 00 00  je          000000006714466A
  0000000067144570: 8B 09              mov         ecx,dword ptr [rcx]
  0000000067144572: 83 F9 01           cmp         ecx,1
  0000000067144575: 74 18              je          000000006714458F
  0000000067144577: 49 8B 84 24 E8 00  mov         rax,qword ptr [r12+00000000000000E8h]
                    00 00
  000000006714457F: 8B 50 04           mov         edx,dword ptr [rax+4]
  0000000067144582: 49 8D 8C 24 E8 00  lea         rcx,[r12+00000000000000E8h]
                    00 00
  000000006714458A: E8 31 D4 FF FF     call        00000000671419C0
  000000006714458F: 49 8B 94 24 E8 00  mov         rdx,qword ptr [r12+00000000000000E8h]
                    00 00
  0000000067144597: 48 63 42 08        movsxd      rax,dword ptr [rdx+8]
  000000006714459B: 48 8B 4C C2 18     mov         rcx,qword ptr [rdx+rax*8+18h]
  00000000671445A0: 4C 8D 84 24 F8 00  lea         r8,[rsp+00000000000000F8h]
                    00 00
  00000000671445A8: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000671445AB: 49 89 00           mov         qword ptr [r8],rax
  00000000671445AE: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000671445B2: 49 89 40 08        mov         qword ptr [r8+8],rax
  00000000671445B6: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000671445BA: 49 89 40 10        mov         qword ptr [r8+10h],rax
  00000000671445BE: 48 8B 41 18        mov         rax,qword ptr [rcx+18h]
  00000000671445C2: 49 89 40 18        mov         qword ptr [r8+18h],rax
  00000000671445C6: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  00000000671445CA: 49 89 40 20        mov         qword ptr [r8+20h],rax
  00000000671445CE: 48 8B 41 28        mov         rax,qword ptr [rcx+28h]
  00000000671445D2: 49 89 40 28        mov         qword ptr [r8+28h],rax
  00000000671445D6: 8B 02              mov         eax,dword ptr [rdx]
  00000000671445D8: 83 F8 01           cmp         eax,1
  00000000671445DB: 74 18              je          00000000671445F5
  00000000671445DD: 49 8B 84 24 E8 00  mov         rax,qword ptr [r12+00000000000000E8h]
                    00 00
  00000000671445E5: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000671445E8: 49 8D 8C 24 E8 00  lea         rcx,[r12+00000000000000E8h]
                    00 00
  00000000671445F0: E8 CB D3 FF FF     call        00000000671419C0
  00000000671445F5: 49 8B 94 24 E8 00  mov         rdx,qword ptr [r12+00000000000000E8h]
                    00 00
  00000000671445FD: 48 63 42 08        movsxd      rax,dword ptr [rdx+8]
  0000000067144601: 48 8D 5C C2 18     lea         rbx,[rdx+rax*8+18h]
  0000000067144606: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067144609: E8 48 43 03 00     call        0000000067178956
  000000006714460E: 48 8B D3           mov         rdx,rbx
  0000000067144611: 49 8D 8C 24 E8 00  lea         rcx,[r12+00000000000000E8h]
                    00 00
  0000000067144619: E8 E2 4A F0 FF     call        0000000067049100
  000000006714461E: 4C 8D 5C 24 30     lea         r11,[rsp+30h]
  0000000067144623: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000006714462B: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006714462E: 49 89 03           mov         qword ptr [r11],rax
  0000000067144631: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  0000000067144635: 49 89 43 08        mov         qword ptr [r11+8],rax
  0000000067144639: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  000000006714463D: 49 89 43 10        mov         qword ptr [r11+10h],rax
  0000000067144641: 48 8B 41 18        mov         rax,qword ptr [rcx+18h]
  0000000067144645: 49 89 43 18        mov         qword ptr [r11+18h],rax
  0000000067144649: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006714464D: 49 89 43 20        mov         qword ptr [r11+20h],rax
  0000000067144651: 48 8B 41 28        mov         rax,qword ptr [rcx+28h]
  0000000067144655: 49 89 43 28        mov         qword ptr [r11+28h],rax
  0000000067144659: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  000000006714465D: 48 8B 9C 24 90 33  mov         rbx,qword ptr [rsp+0000000000003390h]
                    00 00
  0000000067144665: E9 E8 01 00 00     jmp         0000000067144852
  000000006714466A: 83 E2 02           and         edx,2
  000000006714466D: 0F 85 0E 01 00 00  jne         0000000067144781
  0000000067144673: 49 8B 8C 24 F0 00  mov         rcx,qword ptr [r12+00000000000000F0h]
                    00 00
  000000006714467B: 8B 41 08           mov         eax,dword ptr [rcx+8]
  000000006714467E: 39 41 0C           cmp         dword ptr [rcx+0Ch],eax
  0000000067144681: 0F 84 FA 00 00 00  je          0000000067144781
  0000000067144687: 8B 09              mov         ecx,dword ptr [rcx]
  0000000067144689: 83 F9 01           cmp         ecx,1
  000000006714468C: 74 18              je          00000000671446A6
  000000006714468E: 49 8B 84 24 F0 00  mov         rax,qword ptr [r12+00000000000000F0h]
                    00 00
  0000000067144696: 8B 50 04           mov         edx,dword ptr [rax+4]
  0000000067144699: 49 8D 8C 24 F0 00  lea         rcx,[r12+00000000000000F0h]
                    00 00
  00000000671446A1: E8 1A D3 FF FF     call        00000000671419C0
  00000000671446A6: 49 8B 94 24 F0 00  mov         rdx,qword ptr [r12+00000000000000F0h]
                    00 00
  00000000671446AE: 48 63 42 08        movsxd      rax,dword ptr [rdx+8]
  00000000671446B2: 48 8B 4C C2 18     mov         rcx,qword ptr [rdx+rax*8+18h]
  00000000671446B7: 4C 8D 84 24 C8 00  lea         r8,[rsp+00000000000000C8h]
                    00 00
  00000000671446BF: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000671446C2: 49 89 00           mov         qword ptr [r8],rax
  00000000671446C5: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000671446C9: 49 89 40 08        mov         qword ptr [r8+8],rax
  00000000671446CD: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000671446D1: 49 89 40 10        mov         qword ptr [r8+10h],rax
  00000000671446D5: 48 8B 41 18        mov         rax,qword ptr [rcx+18h]
  00000000671446D9: 49 89 40 18        mov         qword ptr [r8+18h],rax
  00000000671446DD: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  00000000671446E1: 49 89 40 20        mov         qword ptr [r8+20h],rax
  00000000671446E5: 48 8B 41 28        mov         rax,qword ptr [rcx+28h]
  00000000671446E9: 49 89 40 28        mov         qword ptr [r8+28h],rax
  00000000671446ED: 8B 02              mov         eax,dword ptr [rdx]
  00000000671446EF: 83 F8 01           cmp         eax,1
  00000000671446F2: 74 18              je          000000006714470C
  00000000671446F4: 49 8B 84 24 F0 00  mov         rax,qword ptr [r12+00000000000000F0h]
                    00 00
  00000000671446FC: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000671446FF: 49 8D 8C 24 F0 00  lea         rcx,[r12+00000000000000F0h]
                    00 00
  0000000067144707: E8 B4 D2 FF FF     call        00000000671419C0
  000000006714470C: 49 8B 94 24 F0 00  mov         rdx,qword ptr [r12+00000000000000F0h]
                    00 00
  0000000067144714: 48 63 42 08        movsxd      rax,dword ptr [rdx+8]
  0000000067144718: 48 8D 5C C2 18     lea         rbx,[rdx+rax*8+18h]
  000000006714471D: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067144720: E8 31 42 03 00     call        0000000067178956
  0000000067144725: 48 8B D3           mov         rdx,rbx
  0000000067144728: 49 8D 8C 24 F0 00  lea         rcx,[r12+00000000000000F0h]
                    00 00
  0000000067144730: E8 CB 49 F0 FF     call        0000000067049100
  0000000067144735: 4C 8D 5C 24 30     lea         r11,[rsp+30h]
  000000006714473A: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000067144742: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067144745: 49 89 03           mov         qword ptr [r11],rax
  0000000067144748: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006714474C: 49 89 43 08        mov         qword ptr [r11+8],rax
  0000000067144750: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  0000000067144754: 49 89 43 10        mov         qword ptr [r11+10h],rax
  0000000067144758: 48 8B 41 18        mov         rax,qword ptr [rcx+18h]
  000000006714475C: 49 89 43 18        mov         qword ptr [r11+18h],rax
  0000000067144760: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  0000000067144764: 49 89 43 20        mov         qword ptr [r11+20h],rax
  0000000067144768: 48 8B 41 28        mov         rax,qword ptr [rcx+28h]
  000000006714476C: 49 89 43 28        mov         qword ptr [r11+28h],rax
  0000000067144770: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  0000000067144774: 48 8B 9C 24 90 33  mov         rbx,qword ptr [rsp+0000000000003390h]
                    00 00
  000000006714477C: E9 D1 00 00 00     jmp         0000000067144852
  0000000067144781: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000067144789: 45 33 C9           xor         r9d,r9d
  000000006714478C: 45 33 C0           xor         r8d,r8d
  000000006714478F: 33 D2              xor         edx,edx
  0000000067144791: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000067144796: FF 15 3C 40 06 00  call        qword ptr [00000000671A87D8h]
  000000006714479C: 85 C0              test        eax,eax
  000000006714479E: 0F 95 C0           setne       al
  00000000671447A1: 84 C0              test        al,al
  00000000671447A3: 0F 84 31 02 00 00  je          00000000671449DA
  00000000671447A9: 41 8B 17           mov         edx,dword ptr [r15]
  00000000671447AC: 8B C2              mov         eax,edx
  00000000671447AE: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  00000000671447B2: 83 E0 01           and         eax,1
  00000000671447B5: 74 64              je          000000006714481B
  00000000671447B7: 81 F9 00 01 00 00  cmp         ecx,100h
  00000000671447BD: 72 08              jb          00000000671447C7
  00000000671447BF: 81 F9 09 01 00 00  cmp         ecx,109h
  00000000671447C5: 76 3D              jbe         0000000067144804
  00000000671447C7: 81 F9 00 02 00 00  cmp         ecx,200h
  00000000671447CD: 72 08              jb          00000000671447D7
  00000000671447CF: 81 F9 0E 02 00 00  cmp         ecx,20Eh
  00000000671447D5: 76 2D              jbe         0000000067144804
  00000000671447D7: 81 F9 0A 02 00 00  cmp         ecx,20Ah
  00000000671447DD: 74 25              je          0000000067144804
  00000000671447DF: 81 F9 0E 02 00 00  cmp         ecx,20Eh
  00000000671447E5: 74 1D              je          0000000067144804
  00000000671447E7: 81 F9 40 02 00 00  cmp         ecx,240h
  00000000671447ED: 74 15              je          0000000067144804
  00000000671447EF: 81 F9 19 01 00 00  cmp         ecx,119h
  00000000671447F5: 74 0D              je          0000000067144804
  00000000671447F7: 81 F9 1A 01 00 00  cmp         ecx,11Ah
  00000000671447FD: 74 05              je          0000000067144804
  00000000671447FF: 83 F9 10           cmp         ecx,10h
  0000000067144802: 75 17              jne         000000006714481B
  0000000067144804: 49 8D 8C 24 E8 00  lea         rcx,[r12+00000000000000E8h]
                    00 00
  000000006714480C: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000067144811: E8 2A DD FF FF     call        0000000067142540
  0000000067144816: E9 BF 01 00 00     jmp         00000000671449DA
  000000006714481B: 83 E2 02           and         edx,2
  000000006714481E: 74 32              je          0000000067144852
  0000000067144820: 81 F9 00 04 00 00  cmp         ecx,400h
  0000000067144826: 75 2A              jne         0000000067144852
  0000000067144828: 49 8B 84 24 98 00  mov         rax,qword ptr [r12+0000000000000098h]
                    00 00
  0000000067144830: 48 39 44 24 30     cmp         qword ptr [rsp+30h],rax
  0000000067144835: 0F 85 6F 01 00 00  jne         00000000671449AA
  000000006714483B: 49 8D 8C 24 F0 00  lea         rcx,[r12+00000000000000F0h]
                    00 00
  0000000067144843: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000067144848: E8 F3 DC FF FF     call        0000000067142540
  000000006714484D: E9 88 01 00 00     jmp         00000000671449DA
  0000000067144852: 4C 8B 44 24 30     mov         r8,qword ptr [rsp+30h]
  0000000067144857: 4D 39 84 24 98 00  cmp         qword ptr [r12+0000000000000098h],r8
                    00 00
  000000006714485F: 75 22              jne         0000000067144883
  0000000067144861: 81 F9 01 04 00 00  cmp         ecx,401h
  0000000067144867: 75 1A              jne         0000000067144883
  0000000067144869: 45 84 F6           test        r14b,r14b
  000000006714486C: 74 0D              je          000000006714487B
  000000006714486E: C6 84 24 A0 33 00  mov         byte ptr [rsp+00000000000033A0h],1
                    00 01
  0000000067144876: E9 1F 02 00 00     jmp         0000000067144A9A
  000000006714487B: 41 B6 01           mov         r14b,1
  000000006714487E: E9 27 01 00 00     jmp         00000000671449AA
  0000000067144883: 81 F9 13 01 00 00  cmp         ecx,113h
  0000000067144889: 0F 85 12 01 00 00  jne         00000000671449A1
  000000006714488F: 32 C0              xor         al,al
  0000000067144891: 33 D2              xor         edx,edx
  0000000067144893: 48 8B 8C 24 38 03  mov         rcx,qword ptr [rsp+0000000000000338h]
                    00 00
  000000006714489B: 44 8B 8C 24 34 03  mov         r9d,dword ptr [rsp+0000000000000334h]
                    00 00
  00000000671448A3: 4C 8B 54 24 48     mov         r10,qword ptr [rsp+48h]
  00000000671448A8: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  00000000671448AD: 0F 1F 00           nop         dword ptr [rax]
  00000000671448B0: 41 3B D1           cmp         edx,r9d
  00000000671448B3: 7D 69              jge         000000006714491E
  00000000671448B5: 48 8D BC 24 98 00  lea         rdi,[rsp+0000000000000098h]
                    00 00
  00000000671448BD: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000671448C0: 48 89 07           mov         qword ptr [rdi],rax
  00000000671448C3: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000671448C7: 48 89 47 08        mov         qword ptr [rdi+8],rax
  00000000671448CB: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000671448CF: 48 89 47 10        mov         qword ptr [rdi+10h],rax
  00000000671448D3: 48 8B 41 18        mov         rax,qword ptr [rcx+18h]
  00000000671448D7: 48 89 47 18        mov         qword ptr [rdi+18h],rax
  00000000671448DB: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  00000000671448DF: 48 89 47 20        mov         qword ptr [rdi+20h],rax
  00000000671448E3: 48 8B 41 28        mov         rax,qword ptr [rcx+28h]
  00000000671448E7: 48 89 47 28        mov         qword ptr [rdi+28h],rax
  00000000671448EB: 4C 39 9C 24 A8 00  cmp         qword ptr [rsp+00000000000000A8h],r11
                    00 00
  00000000671448F3: 75 18              jne         000000006714490D
  00000000671448F5: 4C 39 84 24 98 00  cmp         qword ptr [rsp+0000000000000098h],r8
                    00 00
  00000000671448FD: 75 0E              jne         000000006714490D
  00000000671448FF: 4C 39 94 24 B0 00  cmp         qword ptr [rsp+00000000000000B0h],r10
                    00 00
  0000000067144907: 75 04              jne         000000006714490D
  0000000067144909: B0 01              mov         al,1
  000000006714490B: EB 02              jmp         000000006714490F
  000000006714490D: 32 C0              xor         al,al
  000000006714490F: FF C2              inc         edx
  0000000067144911: 48 83 C1 30        add         rcx,30h
  0000000067144915: 84 C0              test        al,al
  0000000067144917: 74 97              je          00000000671448B0
  0000000067144919: E9 7C 01 00 00     jmp         0000000067144A9A
  000000006714491E: 84 C0              test        al,al
  0000000067144920: 0F 85 74 01 00 00  jne         0000000067144A9A
  0000000067144926: 44 3B 8C 24 30 03  cmp         r9d,dword ptr [rsp+0000000000000330h]
                    00 00
  000000006714492E: 75 1C              jne         000000006714494C
  0000000067144930: 47 8D 04 09        lea         r8d,[r9+r9]
  0000000067144934: 41 8B D1           mov         edx,r9d
  0000000067144937: 48 8D 8C 24 30 03  lea         rcx,[rsp+0000000000000330h]
                    00 00
  000000006714493F: E8 5C CF FF FF     call        00000000671418A0
  0000000067144944: 44 8B 8C 24 34 03  mov         r9d,dword ptr [rsp+0000000000000334h]
                    00 00
  000000006714494C: 49 63 C1           movsxd      rax,r9d
  000000006714494F: 41 FF C1           inc         r9d
  0000000067144952: 44 89 8C 24 34 03  mov         dword ptr [rsp+0000000000000334h],r9d
                    00 00
  000000006714495A: 48 8D 0C 40        lea         rcx,[rax+rax*2]
  000000006714495E: 48 C1 E1 04        shl         rcx,4
  0000000067144962: 48 03 8C 24 38 03  add         rcx,qword ptr [rsp+0000000000000338h]
                    00 00
  000000006714496A: 74 3E              je          00000000671449AA
  000000006714496C: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000067144971: 48 8B 02           mov         rax,qword ptr [rdx]
  0000000067144974: 48 89 01           mov         qword ptr [rcx],rax
  0000000067144977: 48 8B 42 08        mov         rax,qword ptr [rdx+8]
  000000006714497B: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000006714497F: 48 8B 42 10        mov         rax,qword ptr [rdx+10h]
  0000000067144983: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000067144987: 48 8B 42 18        mov         rax,qword ptr [rdx+18h]
  000000006714498B: 48 89 41 18        mov         qword ptr [rcx+18h],rax
  000000006714498F: 48 8B 42 20        mov         rax,qword ptr [rdx+20h]
  0000000067144993: 48 89 41 20        mov         qword ptr [rcx+20h],rax
  0000000067144997: 48 8B 42 28        mov         rax,qword ptr [rdx+28h]
  000000006714499B: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  000000006714499F: EB 09              jmp         00000000671449AA
  00000000671449A1: 83 F9 12           cmp         ecx,12h
  00000000671449A4: 0F 84 3F 02 00 00  je          0000000067144BE9
  00000000671449AA: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000671449AF: 48 8B CB           mov         rcx,rbx
  00000000671449B2: E8 59 7C FC FF     call        000000006710C610
  00000000671449B7: 84 C0              test        al,al
  00000000671449B9: 0F 85 D8 00 00 00  jne         0000000067144A97
  00000000671449BF: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000671449C4: FF 15 06 3E 06 00  call        qword ptr [00000000671A87D0h]
  00000000671449CA: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000671449CF: FF 15 6B 3E 06 00  call        qword ptr [00000000671A8840h]
  00000000671449D5: E9 BD 00 00 00     jmp         0000000067144A97
  00000000671449DA: 33 DB              xor         ebx,ebx
  00000000671449DC: 85 F6              test        esi,esi
  00000000671449DE: 7E 3A              jle         0000000067144A1A
  00000000671449E0: 8B FE              mov         edi,esi
  00000000671449E2: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    0F 1F 84 00 00 00
                    00 00
  00000000671449F0: 49 8B 84 24 E0 00  mov         rax,qword ptr [r12+00000000000000E0h]
                    00 00
  00000000671449F8: 48 63 48 08        movsxd      rcx,dword ptr [rax+8]
  00000000671449FC: 48 03 CB           add         rcx,rbx
  00000000671449FF: 48 8B 4C C8 18     mov         rcx,qword ptr [rax+rcx*8+18h]
  0000000067144A04: E8 B7 35 00 00     call        0000000067147FC0
  0000000067144A09: 48 89 84 DC 30 01  mov         qword ptr [rsp+rbx*8+0000000000000130h],rax
                    00 00
  0000000067144A11: 48 FF C3           inc         rbx
  0000000067144A14: 48 83 EF 01        sub         rdi,1
  0000000067144A18: 75 D6              jne         00000000671449F0
  0000000067144A1A: C7 44 24 20 02 00  mov         dword ptr [rsp+20h],2
                    00 00
  0000000067144A22: 41 B9 FF 04 00 00  mov         r9d,4FFh
  0000000067144A28: 45 33 C0           xor         r8d,r8d
  0000000067144A2B: 48 8D 94 24 30 01  lea         rdx,[rsp+0000000000000130h]
                    00 00
  0000000067144A33: 8B CE              mov         ecx,esi
  0000000067144A35: FF 15 FD 3D 06 00  call        qword ptr [00000000671A8838h]
  0000000067144A3B: 3B C6              cmp         eax,esi
  0000000067144A3D: 0F 94 C1           sete        cl
  0000000067144A40: 84 C9              test        cl,cl
  0000000067144A42: 75 56              jne         0000000067144A9A
  0000000067144A44: 3B C6              cmp         eax,esi
  0000000067144A46: 73 6A              jae         0000000067144AB2
  0000000067144A48: 49 8B 94 24 E0 00  mov         rdx,qword ptr [r12+00000000000000E0h]
                    00 00
  0000000067144A50: 48 63 4A 08        movsxd      rcx,dword ptr [rdx+8]
  0000000067144A54: 48 98              cdqe
  0000000067144A56: 48 03 C8           add         rcx,rax
  0000000067144A59: 48 8B 5C CA 18     mov         rbx,qword ptr [rdx+rcx*8+18h]
  0000000067144A5E: BA 84 00 00 00     mov         edx,84h
  0000000067144A63: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000067144A68: E8 E3 8A FD FF     call        000000006711D550
  0000000067144A6D: 90                 nop
  0000000067144A6E: 66 21 6C 24 72     and         word ptr [rsp+72h],bp
  0000000067144A73: 48 8B 0D A6 F7 1A  mov         rcx,qword ptr [00000000672F4220h]
                    00
  0000000067144A7A: 48 85 C9           test        rcx,rcx
  0000000067144A7D: 74 0E              je          0000000067144A8D
  0000000067144A7F: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  0000000067144A84: 48 8B D3           mov         rdx,rbx
  0000000067144A87: E8 E4 4A FD FF     call        0000000067119570
  0000000067144A8C: 90                 nop
  0000000067144A8D: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000067144A92: E8 E9 8A FD FF     call        000000006711D580
  0000000067144A97: 41 B5 01           mov         r13b,1
  0000000067144A9A: 41 80 BC 24 94 00  cmp         byte ptr [r12+0000000000000094h],0
                    00 00 00
  0000000067144AA3: 75 0D              jne         0000000067144AB2
  0000000067144AA5: 48 8B 9C 24 90 33  mov         rbx,qword ptr [rsp+0000000000003390h]
                    00 00
  0000000067144AAD: E9 8E FA FF FF     jmp         0000000067144540
  0000000067144AB2: 45 84 ED           test        r13b,r13b
  0000000067144AB5: 0F 85 FA 00 00 00  jne         0000000067144BB5
  0000000067144ABB: 45 38 AC 24 94 00  cmp         byte ptr [r12+0000000000000094h],r13b
                    00 00
  0000000067144AC3: 0F 85 EC 00 00 00  jne         0000000067144BB5
  0000000067144AC9: 41 8B 07           mov         eax,dword ptr [r15]
  0000000067144ACC: 83 E0 04           and         eax,4
  0000000067144ACF: 0F 84 E0 00 00 00  je          0000000067144BB5
  0000000067144AD5: 40 B5 01           mov         bpl,1
  0000000067144AD8: 49 8B 84 24 E0 00  mov         rax,qword ptr [r12+00000000000000E0h]
                    00 00
  0000000067144AE0: 8B 70 0C           mov         esi,dword ptr [rax+0Ch]
  0000000067144AE3: 2B 70 08           sub         esi,dword ptr [rax+8]
  0000000067144AE6: 33 DB              xor         ebx,ebx
  0000000067144AE8: 85 F6              test        esi,esi
  0000000067144AEA: 7E 2E              jle         0000000067144B1A
  0000000067144AEC: 8B FE              mov         edi,esi
  0000000067144AEE: 66 90              nop
  0000000067144AF0: 49 8B 84 24 E0 00  mov         rax,qword ptr [r12+00000000000000E0h]
                    00 00
  0000000067144AF8: 48 63 48 08        movsxd      rcx,dword ptr [rax+8]
  0000000067144AFC: 48 03 CB           add         rcx,rbx
  0000000067144AFF: 48 8B 4C C8 18     mov         rcx,qword ptr [rax+rcx*8+18h]
  0000000067144B04: E8 B7 34 00 00     call        0000000067147FC0
  0000000067144B09: 48 89 84 DC 30 01  mov         qword ptr [rsp+rbx*8+0000000000000130h],rax
                    00 00
  0000000067144B11: 48 FF C3           inc         rbx
  0000000067144B14: 48 83 EF 01        sub         rdi,1
  0000000067144B18: 75 D6              jne         0000000067144AF0
  0000000067144B1A: 48 8B BC 24 90 33  mov         rdi,qword ptr [rsp+0000000000003390h]
                    00 00
  0000000067144B22: 48 8B CF           mov         rcx,rdi
  0000000067144B25: E8 96 D6 02 00     call        00000000671721C0
  0000000067144B2A: C7 44 24 20 06 00  mov         dword ptr [rsp+20h],6
                    00 00
  0000000067144B32: 41 B9 FF 04 00 00  mov         r9d,4FFh
  0000000067144B38: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  0000000067144B3C: 48 8D 94 24 30 01  lea         rdx,[rsp+0000000000000130h]
                    00 00
  0000000067144B44: 8B CE              mov         ecx,esi
  0000000067144B46: FF 15 EC 3C 06 00  call        qword ptr [00000000671A8838h]
  0000000067144B4C: 48 63 D8           movsxd      rbx,eax
  0000000067144B4F: 48 8B CF           mov         rcx,rdi
  0000000067144B52: E8 89 D6 02 00     call        00000000671721E0
  0000000067144B57: 3B DE              cmp         ebx,esi
  0000000067144B59: 73 5D              jae         0000000067144BB8
  0000000067144B5B: 49 8B 94 24 E0 00  mov         rdx,qword ptr [r12+00000000000000E0h]
                    00 00
  0000000067144B63: 48 63 4A 08        movsxd      rcx,dword ptr [rdx+8]
  0000000067144B67: 48 03 CB           add         rcx,rbx
  0000000067144B6A: 48 8B 5C CA 18     mov         rbx,qword ptr [rdx+rcx*8+18h]
  0000000067144B6F: BA 84 00 00 00     mov         edx,84h
  0000000067144B74: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000067144B79: E8 D2 89 FD FF     call        000000006711D550
  0000000067144B7E: 90                 nop
  0000000067144B7F: B8 FD FF 00 00     mov         eax,0FFFDh
  0000000067144B84: 66 21 84 24 8A 00  and         word ptr [rsp+000000000000008Ah],ax
                    00 00
  0000000067144B8C: 48 8B 0D 8D F6 1A  mov         rcx,qword ptr [00000000672F4220h]
                    00
  0000000067144B93: 48 85 C9           test        rcx,rcx
  0000000067144B96: 74 0E              je          0000000067144BA6
  0000000067144B98: 4C 8D 44 24 78     lea         r8,[rsp+78h]
  0000000067144B9D: 48 8B D3           mov         rdx,rbx
  0000000067144BA0: E8 CB 49 FD FF     call        0000000067119570
  0000000067144BA5: 90                 nop
  0000000067144BA6: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000067144BAB: E8 D0 89 FD FF     call        000000006711D580
  0000000067144BB0: 41 B5 01           mov         r13b,1
  0000000067144BB3: EB 03              jmp         0000000067144BB8
  0000000067144BB5: 40 32 ED           xor         bpl,bpl
  0000000067144BB8: 48 8D 84 24 40 03  lea         rax,[rsp+0000000000000340h]
                    00 00
  0000000067144BC0: 48 8B 8C 24 38 03  mov         rcx,qword ptr [rsp+0000000000000338h]
                    00 00
  0000000067144BC8: 48 3B C8           cmp         rcx,rax
  0000000067144BCB: 74 05              je          0000000067144BD2
  0000000067144BCD: E8 DE B7 ED FF     call        00000000670203B0
  0000000067144BD2: 40 84 ED           test        bpl,bpl
  0000000067144BD5: 74 40              je          0000000067144C17
  0000000067144BD7: 48 8B 9C 24 90 33  mov         rbx,qword ptr [rsp+0000000000003390h]
                    00 00
  0000000067144BDF: BD FD FF 00 00     mov         ebp,0FFFDh
  0000000067144BE4: E9 17 F9 FF FF     jmp         0000000067144500
  0000000067144BE9: 48 83 3D 2F F6 1A  cmp         qword ptr [00000000672F4220h],0
                    00 00
  0000000067144BF1: 74 06              je          0000000067144BF9
  0000000067144BF3: E8 18 4F FD FF     call        0000000067119B10
  0000000067144BF8: 90                 nop
  0000000067144BF9: 48 8D 84 24 40 03  lea         rax,[rsp+0000000000000340h]
                    00 00
  0000000067144C01: 48 8B 8C 24 38 03  mov         rcx,qword ptr [rsp+0000000000000338h]
                    00 00
  0000000067144C09: 48 3B C8           cmp         rcx,rax
  0000000067144C0C: 74 05              je          0000000067144C13
  0000000067144C0E: E8 9D B7 ED FF     call        00000000670203B0
  0000000067144C13: 32 C0              xor         al,al
  0000000067144C15: EB 41              jmp         0000000067144C58
  0000000067144C17: 45 84 F6           test        r14b,r14b
  0000000067144C1A: 75 14              jne         0000000067144C30
  0000000067144C1C: 41 F6 07 20        test        byte ptr [r15],20h
  0000000067144C20: 75 0E              jne         0000000067144C30
  0000000067144C22: 4D 8B 44 24 40     mov         r8,qword ptr [r12+40h]
  0000000067144C27: 33 D2              xor         edx,edx
  0000000067144C29: 33 C9              xor         ecx,ecx
  0000000067144C2B: E8 10 64 FD FF     call        000000006711B040
  0000000067144C30: 0F B6 84 24 A0 33  movzx       eax,byte ptr [rsp+00000000000033A0h]
                    00 00
  0000000067144C38: 84 C0              test        al,al
  0000000067144C3A: 74 19              je          0000000067144C55
  0000000067144C3C: 45 33 C9           xor         r9d,r9d
  0000000067144C3F: 45 33 C0           xor         r8d,r8d
  0000000067144C42: BA 01 04 00 00     mov         edx,401h
  0000000067144C47: 49 8B 8C 24 98 00  mov         rcx,qword ptr [r12+0000000000000098h]
                    00 00
  0000000067144C4F: FF 15 03 3C 06 00  call        qword ptr [00000000671A8858h]
  0000000067144C55: 41 8A C5           mov         al,r13b
  0000000067144C58: 48 8B 9C 24 98 33  mov         rbx,qword ptr [rsp+0000000000003398h]
                    00 00
  0000000067144C60: 48 81 C4 50 33 00  add         rsp,3350h
                    00
  0000000067144C67: 41 5F              pop         r15
  0000000067144C69: 41 5E              pop         r14
  0000000067144C6B: 41 5D              pop         r13
  0000000067144C6D: 41 5C              pop         r12
  0000000067144C6F: 5F                 pop         rdi
  0000000067144C70: 5E                 pop         rsi
  0000000067144C71: 5D                 pop         rbp
  0000000067144C72: C3                 ret

  Summary

      1A7000 .text
