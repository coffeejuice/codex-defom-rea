Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065040290: 48 8B C4           mov         rax,rsp
  0000000065040293: 41 54              push        r12
  0000000065040295: 41 55              push        r13
  0000000065040297: 41 56              push        r14
  0000000065040299: 48 83 EC 70        sub         rsp,70h
  000000006504029D: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650402A6: 48 89 58 08        mov         qword ptr [rax+8],rbx
  00000000650402AA: 48 89 68 10        mov         qword ptr [rax+10h],rbp
  00000000650402AE: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  00000000650402B2: 48 89 78 20        mov         qword ptr [rax+20h],rdi
  00000000650402B6: 45 8B E1           mov         r12d,r9d
  00000000650402B9: 45 8B E8           mov         r13d,r8d
  00000000650402BC: 4C 8B F2           mov         r14,rdx
  00000000650402BF: 48 8B E9           mov         rbp,rcx
  00000000650402C2: E8 29 BA FF FF     call        000000006503BCF0
  00000000650402C7: 84 C0              test        al,al
  00000000650402C9: 0F 84 5C 01 00 00  je          000000006504042B
  00000000650402CF: 45 85 ED           test        r13d,r13d
  00000000650402D2: 0F 88 80 00 00 00  js          0000000065040358
  00000000650402D8: 45 85 E4           test        r12d,r12d
  00000000650402DB: 78 7B              js          0000000065040358
  00000000650402DD: 49 8B D6           mov         rdx,r14
  00000000650402E0: 48 8B CD           mov         rcx,rbp
  00000000650402E3: E8 58 52 00 00     call        0000000065045540
  00000000650402E8: 49 8B D6           mov         rdx,r14
  00000000650402EB: 48 8B CD           mov         rcx,rbp
  00000000650402EE: E8 3D 36 00 00     call        0000000065043930
  00000000650402F3: 48 8B F0           mov         rsi,rax
  00000000650402F6: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000000650402FE: 8B 19              mov         ebx,dword ptr [rcx]
  0000000065040300: 4C 8B 75 08        mov         r14,qword ptr [rbp+8]
  0000000065040304: B9 18 00 00 00     mov         ecx,18h
  0000000065040309: E8 64 0E 5A 00     call        00000000655E1172
  000000006504030E: 48 8B F8           mov         rdi,rax
  0000000065040311: 48 85 C0           test        rax,rax
  0000000065040314: 74 05              je          000000006504031B
  0000000065040316: 48 89 30           mov         qword ptr [rax],rsi
  0000000065040319: EB 02              jmp         000000006504031D
  000000006504031B: 33 FF              xor         edi,edi
  000000006504031D: 89 5C 24 40        mov         dword ptr [rsp+40h],ebx
  0000000065040321: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000065040326: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000065040329: E8 52 81 00 00     call        0000000065048480
  000000006504032E: 44 89 64 24 28     mov         dword ptr [rsp+28h],r12d
  0000000065040333: 44 89 64 24 20     mov         dword ptr [rsp+20h],r12d
  0000000065040338: 45 8B CD           mov         r9d,r13d
  000000006504033B: 45 8B C5           mov         r8d,r13d
  000000006504033E: 48 8B D7           mov         rdx,rdi
  0000000065040341: 49 8B CE           mov         rcx,r14
  0000000065040344: E8 07 EF FF FF     call        000000006503F250
  0000000065040349: 48 8D 4D 10        lea         rcx,[rbp+10h]
  000000006504034D: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065040350: FF 50 58           call        qword ptr [rax+58h]
  0000000065040353: E9 D3 00 00 00     jmp         000000006504042B
  0000000065040358: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006504035D: 48 8B CD           mov         rcx,rbp
  0000000065040360: FF 15 EA 6A 65 00  call        qword ptr [0000000065696E50h]
  0000000065040366: 90                 nop
  0000000065040367: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006504036C: 48 8B C8           mov         rcx,rax
  000000006504036F: FF 15 D3 6A 65 00  call        qword ptr [0000000065696E48h]
  0000000065040375: 48 8B F8           mov         rdi,rax
  0000000065040378: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006504037D: 49 8B CE           mov         rcx,r14
  0000000065040380: FF 15 CA 6A 65 00  call        qword ptr [0000000065696E50h]
  0000000065040386: 90                 nop
  0000000065040387: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006504038C: 48 8B C8           mov         rcx,rax
  000000006504038F: FF 15 B3 6A 65 00  call        qword ptr [0000000065696E48h]
  0000000065040395: 48 8B D8           mov         rbx,rax
  0000000065040398: 48 8B CF           mov         rcx,rdi
  000000006504039B: FF 15 9F 6A 65 00  call        qword ptr [0000000065696E40h]
  00000000650403A1: 48 8B F0           mov         rsi,rax
  00000000650403A4: 48 8B 55 00        mov         rdx,qword ptr [rbp]
  00000000650403A8: 48 8B CD           mov         rcx,rbp
  00000000650403AB: FF 12              call        qword ptr [rdx]
  00000000650403AD: 48 8B C8           mov         rcx,rax
  00000000650403B0: FF 15 B2 69 65 00  call        qword ptr [0000000065696D68h]
  00000000650403B6: 48 8B F8           mov         rdi,rax
  00000000650403B9: 48 8B CB           mov         rcx,rbx
  00000000650403BC: FF 15 7E 6A 65 00  call        qword ptr [0000000065696E40h]
  00000000650403C2: 48 8B D8           mov         rbx,rax
  00000000650403C5: 49 8B 16           mov         rdx,qword ptr [r14]
  00000000650403C8: 49 8B CE           mov         rcx,r14
  00000000650403CB: FF 12              call        qword ptr [rdx]
  00000000650403CD: 48 8B C8           mov         rcx,rax
  00000000650403D0: FF 15 92 69 65 00  call        qword ptr [0000000065696D68h]
  00000000650403D6: 48 8B D0           mov         rdx,rax
  00000000650403D9: 44 89 64 24 30     mov         dword ptr [rsp+30h],r12d
  00000000650403DE: 44 89 6C 24 28     mov         dword ptr [rsp+28h],r13d
  00000000650403E3: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  00000000650403E8: 4C 8B CF           mov         r9,rdi
  00000000650403EB: 4C 8B C3           mov         r8,rbx
  00000000650403EE: 48 8D 0D 4B 27 66  lea         rcx,[00000000656A2B40h]
                    00
  00000000650403F5: FF 15 9D 65 65 00  call        qword ptr [0000000065696998h]
  00000000650403FB: 90                 nop
  00000000650403FC: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065040401: FF 15 49 66 65 00  call        qword ptr [0000000065696A50h]
  0000000065040407: 90                 nop
  0000000065040408: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006504040D: FF 15 4D 63 65 00  call        qword ptr [0000000065696760h]
  0000000065040413: 90                 nop
  0000000065040414: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065040419: FF 15 31 66 65 00  call        qword ptr [0000000065696A50h]
  000000006504041F: 90                 nop
  0000000065040420: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000065040425: FF 15 35 63 65 00  call        qword ptr [0000000065696760h]
  000000006504042B: 4C 8D 5C 24 70     lea         r11,[rsp+70h]
  0000000065040430: 49 8B 5B 20        mov         rbx,qword ptr [r11+20h]
  0000000065040434: 49 8B 6B 28        mov         rbp,qword ptr [r11+28h]
  0000000065040438: 49 8B 73 30        mov         rsi,qword ptr [r11+30h]
  000000006504043C: 49 8B 7B 38        mov         rdi,qword ptr [r11+38h]
  0000000065040440: 49 8B E3           mov         rsp,r11
  0000000065040443: 41 5E              pop         r14
  0000000065040445: 41 5D              pop         r13
  0000000065040447: 41 5C              pop         r12
  0000000065040449: C3                 ret

  Summary

      695000 .text
