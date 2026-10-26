Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065062460: 40 56              push        rsi
  0000000065062462: 57                 push        rdi
  0000000065062463: 41 54              push        r12
  0000000065062465: 48 83 EC 30        sub         rsp,30h
  0000000065062469: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065062472: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  0000000065062477: 48 89 6C 24 68     mov         qword ptr [rsp+68h],rbp
  000000006506247C: 0F B6 F2           movzx       esi,dl
  000000006506247F: 48 8B D9           mov         rbx,rcx
  0000000065062482: 48 8B 69 08        mov         rbp,qword ptr [rcx+8]
  0000000065062486: 48 8B B9 C0 00 00  mov         rdi,qword ptr [rcx+00000000000000C0h]
                    00
  000000006506248D: 45 33 E4           xor         r12d,r12d
  0000000065062490: 48 85 FF           test        rdi,rdi
  0000000065062493: 74 14              je          00000000650624A9
  0000000065062495: 48 8D 4F 18        lea         rcx,[rdi+18h]
  0000000065062499: 41 8D 54 24 85     lea         edx,[r12-7Bh]
  000000006506249E: FF 15 EC 49 63 00  call        qword ptr [0000000065696E90h]
  00000000650624A4: 66 44 89 67 4A     mov         word ptr [rdi+4Ah],r12w
  00000000650624A9: 40 84 F6           test        sil,sil
  00000000650624AC: 75 24              jne         00000000650624D2
  00000000650624AE: 4C 39 A3 88 00 00  cmp         qword ptr [rbx+0000000000000088h],r12
                    00
  00000000650624B5: 74 1B              je          00000000650624D2
  00000000650624B7: 48 8B 8B 88 00 00  mov         rcx,qword ptr [rbx+0000000000000088h]
                    00
  00000000650624BE: 8B 41 48           mov         eax,dword ptr [rcx+48h]
  00000000650624C1: 39 41 40           cmp         dword ptr [rcx+40h],eax
  00000000650624C4: 75 0C              jne         00000000650624D2
  00000000650624C6: 8B 41 4C           mov         eax,dword ptr [rcx+4Ch]
  00000000650624C9: 39 41 44           cmp         dword ptr [rcx+44h],eax
  00000000650624CC: 0F 84 9C 00 00 00  je          000000006506256E
  00000000650624D2: 48 8B 55 20        mov         rdx,qword ptr [rbp+20h]
  00000000650624D6: 0F B6 42 0C        movzx       eax,byte ptr [rdx+0Ch]
  00000000650624DA: A8 01              test        al,1
  00000000650624DC: 0F 85 8C 00 00 00  jne         000000006506256E
  00000000650624E2: 0F B6 42 0A        movzx       eax,byte ptr [rdx+0Ah]
  00000000650624E6: 24 01              and         al,1
  00000000650624E8: 0F 85 80 00 00 00  jne         000000006506256E
  00000000650624EE: 48 8B CD           mov         rcx,rbp
  00000000650624F1: FF 15 71 43 63 00  call        qword ptr [0000000065696868h]
  00000000650624F7: 48 8B F8           mov         rdi,rax
  00000000650624FA: 48 85 C0           test        rax,rax
  00000000650624FD: 74 6F              je          000000006506256E
  00000000650624FF: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065062503: 4C 39 A0 A8 00 00  cmp         qword ptr [rax+00000000000000A8h],r12
                    00
  000000006506250A: 74 13              je          000000006506251F
  000000006506250C: 48 8B 88 A8 00 00  mov         rcx,qword ptr [rax+00000000000000A8h]
                    00
  0000000065062513: 48 83 C1 10        add         rcx,10h
  0000000065062517: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006506251A: FF 50 58           call        qword ptr [rax+58h]
  000000006506251D: EB 4F              jmp         000000006506256E
  000000006506251F: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065062523: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065062526: C1 E9 0F           shr         ecx,0Fh
  0000000065062529: 80 E1 01           and         cl,1
  000000006506252C: 74 40              je          000000006506256E
  000000006506252E: B9 18 00 00 00     mov         ecx,18h
  0000000065062533: E8 3A EC 57 00     call        00000000655E1172
  0000000065062538: 48 8B D8           mov         rbx,rax
  000000006506253B: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000065062540: 48 85 C0           test        rax,rax
  0000000065062543: 74 1A              je          000000006506255F
  0000000065062545: BA 4C 00 00 00     mov         edx,4Ch
  000000006506254A: 48 8B C8           mov         rcx,rax
  000000006506254D: FF 15 C5 42 63 00  call        qword ptr [0000000065696818h]
  0000000065062553: 4C 8D 1D F6 BD 63  lea         r11,[000000006569E350h]
                    00
  000000006506255A: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006506255D: EB 03              jmp         0000000065062562
  000000006506255F: 49 8B DC           mov         rbx,r12
  0000000065062562: 48 8B D3           mov         rdx,rbx
  0000000065062565: 48 8B CF           mov         rcx,rdi
  0000000065062568: FF 15 82 46 63 00  call        qword ptr [0000000065696BF0h]
  000000006506256E: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  0000000065062573: 48 8B 6C 24 68     mov         rbp,qword ptr [rsp+68h]
  0000000065062578: 48 83 C4 30        add         rsp,30h
  000000006506257C: 41 5C              pop         r12
  000000006506257E: 5F                 pop         rdi
  000000006506257F: 5E                 pop         rsi
  0000000065062580: C3                 ret

  Summary

      695000 .text
