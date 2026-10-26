Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001413913E0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001413913E5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001413913EA: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  00000001413913F1: 48 C7 44 24 68 FE  mov         qword ptr [rsp+68h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001413913FA: 48 8D 44 24 28     lea         rax,[rsp+28h]
  00000001413913FF: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000141391404: 33 D2              xor         edx,edx
  0000000141391406: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014139140B: E8 60 58 1B FF     call        0000000140546C70
  0000000141391410: 4C 8B C0           mov         r8,rax
  0000000141391413: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  000000014139141B: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141391423: FF 15 57 A6 A7 00  call        qword ptr [0000000141E0BA80h]
  0000000141391429: 90                 nop
  000000014139142A: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141391432: 48 8D 05 5F 90 AB  lea         rax,[0000000142E4A498h]
                    01
  0000000141391439: 48 89 01           mov         qword ptr [rcx],rax
  000000014139143C: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141391444: 48 8D 05 0D 90 AB  lea         rax,[0000000142E4A458h]
                    01
  000000014139144B: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  000000014139144F: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141391457: 48 83 C1 28        add         rcx,28h
  000000014139145B: FF 15 27 30 A7 00  call        qword ptr [0000000141E04488h]
  0000000141391461: B9 28 00 00 00     mov         ecx,28h
  0000000141391466: E8 51 D6 0D 00     call        000000014146EABC
  000000014139146B: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000141391470: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000141391476: 74 3E              je          00000001413914B6
  0000000141391478: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141391480: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141391485: FF 15 0D 59 A7 00  call        qword ptr [0000000141E06D98h]
  000000014139148B: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  0000000141391490: 48 8D 05 51 AE DF  lea         rax,[000000014218C2E8h]
                    00
  0000000141391497: 49 89 03           mov         qword ptr [r11],rax
  000000014139149A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014139149F: 48 8D 05 0A AE DF  lea         rax,[000000014218C2B0h]
                    00
  00000001413914A6: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001413914AA: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001413914AF: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001413914B4: EB 09              jmp         00000001413914BF
  00000001413914B6: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  00000001413914BF: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  00000001413914C4: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001413914C9: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001413914D1: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001413914D6: 48 89 41 30        mov         qword ptr [rcx+30h],rax
  00000001413914DA: B9 20 00 00 00     mov         ecx,20h
  00000001413914DF: E8 D8 D5 0D 00     call        000000014146EABC
  00000001413914E4: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001413914E9: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  00000001413914EF: 74 3E              je          000000014139152F
  00000001413914F1: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001413914F9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001413914FE: FF 15 5C 58 A7 00  call        qword ptr [0000000141E06D60h]
  0000000141391504: 4C 8B 5C 24 50     mov         r11,qword ptr [rsp+50h]
  0000000141391509: 48 8D 05 98 26 DE  lea         rax,[0000000142173BA8h]
                    00
  0000000141391510: 49 89 03           mov         qword ptr [r11],rax
  0000000141391513: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000141391518: 48 8D 05 09 26 DE  lea         rax,[0000000142173B28h]
                    00
  000000014139151F: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000141391523: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000141391528: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000014139152D: EB 09              jmp         0000000141391538
  000000014139152F: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  0000000141391538: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  000000014139153D: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000141391542: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000141391547: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014139154C: 48 8D 44 24 58     lea         rax,[rsp+58h]
  0000000141391551: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000141391556: 33 D2              xor         edx,edx
  0000000141391558: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014139155D: E8 0E 57 1B FF     call        0000000140546C70
  0000000141391562: 4C 8B C8           mov         r9,rax
  0000000141391565: 45 33 C0           xor         r8d,r8d
  0000000141391568: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141391570: 48 8B 52 30        mov         rdx,qword ptr [rdx+30h]
  0000000141391574: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000141391579: FF 15 29 5D A7 00  call        qword ptr [0000000141E072A8h]
  000000014139157F: 33 D2              xor         edx,edx
  0000000141391581: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000141391586: FF 15 B4 A3 A7 00  call        qword ptr [0000000141E0B940h]
  000000014139158C: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141391594: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014139159C: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001413915A0: FF 15 62 40 A7 00  call        qword ptr [0000000141E05608h]
  00000001413915A6: B2 01              mov         dl,1
  00000001413915A8: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001413915B0: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001413915B4: FF 15 DE 9E A7 00  call        qword ptr [0000000141E0B498h]
  00000001413915BA: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001413915C2: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001413915CA: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001413915CE: FF 15 A4 95 A7 00  call        qword ptr [0000000141E0AB78h]
  00000001413915D4: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001413915DC: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001413915E0: FF 15 8A 95 A7 00  call        qword ptr [0000000141E0AB70h]
  00000001413915E6: 8B D0              mov         edx,eax
  00000001413915E8: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001413915F0: FF 15 C2 64 A7 00  call        qword ptr [0000000141E07AB8h]
  00000001413915F6: 41 B0 01           mov         r8b,1
  00000001413915F9: BA 0E 00 00 00     mov         edx,0Eh
  00000001413915FE: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141391606: FF 15 DC 64 A7 00  call        qword ptr [0000000141E07AE8h]
  000000014139160C: 90                 nop
  000000014139160D: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141391615: 48 81 C4 88 00 00  add         rsp,88h
                    00
  000000014139161C: C3                 ret

  Summary

     1E02000 .text
