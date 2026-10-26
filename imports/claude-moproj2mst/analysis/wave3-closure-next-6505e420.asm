Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505E420: 48 8B C4           mov         rax,rsp
  000000006505E423: 57                 push        rdi
  000000006505E424: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  000000006505E42B: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006505E434: 48 89 58 08        mov         qword ptr [rax+8],rbx
  000000006505E438: 48 89 68 10        mov         qword ptr [rax+10h],rbp
  000000006505E43C: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  000000006505E440: 49 8B F8           mov         rdi,r8
  000000006505E443: 8B EA              mov         ebp,edx
  000000006505E445: 48 8B F1           mov         rsi,rcx
  000000006505E448: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006505E44C: 45 33 C0           xor         r8d,r8d
  000000006505E44F: 41 8D 50 01        lea         edx,[r8+1]
  000000006505E453: 48 8D 48 C0        lea         rcx,[rax-40h]
  000000006505E457: E8 D4 B1 24 00     call        00000000652A9630
  000000006505E45C: 90                 nop
  000000006505E45D: 48 85 FF           test        rdi,rdi
  000000006505E460: 75 30              jne         000000006505E492
  000000006505E462: 48 8B D3           mov         rdx,rbx
  000000006505E465: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006505E46A: E8 41 D1 24 00     call        00000000652AB5B0
  000000006505E46F: C7 44 24 20 00 80  mov         dword ptr [rsp+20h],8000h
                    00 00
  000000006505E477: 41 B9 00 80 00 00  mov         r9d,8000h
  000000006505E47D: 45 33 C0           xor         r8d,r8d
  000000006505E480: 33 D2              xor         edx,edx
  000000006505E482: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006505E487: FF 15 8B 8E 63 00  call        qword ptr [0000000065697318h]
  000000006505E48D: 48 8D 7C 24 48     lea         rdi,[rsp+48h]
  000000006505E492: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006505E496: 48 8B 80 88 00 00  mov         rax,qword ptr [rax+0000000000000088h]
                    00
  000000006505E49D: 48 85 C0           test        rax,rax
  000000006505E4A0: 74 09              je          000000006505E4AB
  000000006505E4A2: 48 8B 40 20        mov         rax,qword ptr [rax+20h]
  000000006505E4A6: 48 85 C0           test        rax,rax
  000000006505E4A9: 75 05              jne         000000006505E4B0
  000000006505E4AB: E8 50 7C FC FF     call        0000000065026100
  000000006505E4B0: 4C 8B 10           mov         r10,qword ptr [rax]
  000000006505E4B3: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000006505E4B8: 4C 8B CF           mov         r9,rdi
  000000006505E4BB: 44 8B C5           mov         r8d,ebp
  000000006505E4BE: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006505E4C3: 48 8B C8           mov         rcx,rax
  000000006505E4C6: 41 FF 92 B8 00 00  call        qword ptr [r10+00000000000000B8h]
                    00
  000000006505E4CD: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006505E4D2: FF 15 38 8E 63 00  call        qword ptr [0000000065697310h]
  000000006505E4D8: 84 C0              test        al,al
  000000006505E4DA: 74 7E              je          000000006505E55A
  000000006505E4DC: 48 83 C7 10        add         rdi,10h
  000000006505E4E0: 48 8B CF           mov         rcx,rdi
  000000006505E4E3: FF 15 AF 89 63 00  call        qword ptr [0000000065696E98h]
  000000006505E4E9: 8B D8              mov         ebx,eax
  000000006505E4EB: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006505E4F0: FF 15 A2 89 63 00  call        qword ptr [0000000065696E98h]
  000000006505E4F6: 2A D8              sub         bl,al
  000000006505E4F8: 88 9E 38 01 00 00  mov         byte ptr [rsi+0000000000000138h],bl
  000000006505E4FE: 48 8B CF           mov         rcx,rdi
  000000006505E501: FF 15 99 89 63 00  call        qword ptr [0000000065696EA0h]
  000000006505E507: 8B D8              mov         ebx,eax
  000000006505E509: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006505E50E: FF 15 8C 89 63 00  call        qword ptr [0000000065696EA0h]
  000000006505E514: 2A D8              sub         bl,al
  000000006505E516: 88 9E 39 01 00 00  mov         byte ptr [rsi+0000000000000139h],bl
  000000006505E51C: 48 8B CF           mov         rcx,rdi
  000000006505E51F: FF 15 83 89 63 00  call        qword ptr [0000000065696EA8h]
  000000006505E525: 8B D8              mov         ebx,eax
  000000006505E527: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006505E52C: FF 15 76 89 63 00  call        qword ptr [0000000065696EA8h]
  000000006505E532: 2A C3              sub         al,bl
  000000006505E534: 88 86 3A 01 00 00  mov         byte ptr [rsi+000000000000013Ah],al
  000000006505E53A: 48 8B CF           mov         rcx,rdi
  000000006505E53D: FF 15 6D 89 63 00  call        qword ptr [0000000065696EB0h]
  000000006505E543: 8B D8              mov         ebx,eax
  000000006505E545: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006505E54A: FF 15 60 89 63 00  call        qword ptr [0000000065696EB0h]
  000000006505E550: 2A C3              sub         al,bl
  000000006505E552: 88 86 3B 01 00 00  mov         byte ptr [rsi+000000000000013Bh],al
  000000006505E558: EB 1C              jmp         000000006505E576
  000000006505E55A: C6 86 38 01 00 00  mov         byte ptr [rsi+0000000000000138h],0
                    00
  000000006505E561: C6 86 39 01 00 00  mov         byte ptr [rsi+0000000000000139h],0
                    00
  000000006505E568: C6 86 3A 01 00 00  mov         byte ptr [rsi+000000000000013Ah],0
                    00
  000000006505E56F: C6 86 3B 01 00 00  mov         byte ptr [rsi+000000000000013Bh],0
                    00
  000000006505E576: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006505E57B: E8 E0 AE 24 00     call        00000000652A9460
  000000006505E580: 4C 8D 9C 24 80 00  lea         r11,[rsp+0000000000000080h]
                    00 00
  000000006505E588: 49 8B 5B 10        mov         rbx,qword ptr [r11+10h]
  000000006505E58C: 49 8B 6B 18        mov         rbp,qword ptr [r11+18h]
  000000006505E590: 49 8B 73 20        mov         rsi,qword ptr [r11+20h]
  000000006505E594: 49 8B E3           mov         rsp,r11
  000000006505E597: 5F                 pop         rdi
  000000006505E598: C3                 ret

  Summary

      695000 .text
