Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065377030: 48 8B C4           mov         rax,rsp
  0000000065377033: 57                 push        rdi
  0000000065377034: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  000000006537703B: 48 C7 44 24 68 FE  mov         qword ptr [rsp+68h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065377044: 48 89 58 08        mov         qword ptr [rax+8],rbx
  0000000065377048: 48 89 70 10        mov         qword ptr [rax+10h],rsi
  000000006537704C: 0F 29 70 E8        movaps      xmmword ptr [rax-18h],xmm6
  0000000065377050: 48 8B FA           mov         rdi,rdx
  0000000065377053: 48 8B F1           mov         rsi,rcx
  0000000065377056: 48 8B CA           mov         rcx,rdx
  0000000065377059: FF 15 E9 FF 31 00  call        qword ptr [0000000065697048h]
  000000006537705F: 83 F8 10           cmp         eax,10h
  0000000065377062: 0F 87 3B 02 00 00  ja          00000000653772A3
  0000000065377068: 48 98              cdqe
  000000006537706A: 48 8D 0D 8F 8F C8  lea         rcx,[0000000065000000h]
                    FF
  0000000065377071: 44 8B 84 81 C4 72  mov         r8d,dword ptr [rcx+rax*4+00000000003772C4h]
                    37 00
  0000000065377079: 4C 03 C1           add         r8,rcx
  000000006537707C: 41 FF E0           jmp         r8
  000000006537707F: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  0000000065377087: 48 8B CF           mov         rcx,rdi
  000000006537708A: FF 15 D8 18 32 00  call        qword ptr [0000000065698968h]
  0000000065377090: 48 8B D8           mov         rbx,rax
  0000000065377093: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  000000006537709B: 48 8B CE           mov         rcx,rsi
  000000006537709E: FF 15 C4 18 32 00  call        qword ptr [0000000065698968h]
  00000000653770A4: 8B 0B              mov         ecx,dword ptr [rbx]
  00000000653770A6: 39 08              cmp         dword ptr [rax],ecx
  00000000653770A8: 75 07              jne         00000000653770B1
  00000000653770AA: 33 C0              xor         eax,eax
  00000000653770AC: E9 F7 01 00 00     jmp         00000000653772A8
  00000000653770B1: 48 8D 54 24 24     lea         rdx,[rsp+24h]
  00000000653770B6: 48 8B CF           mov         rcx,rdi
  00000000653770B9: FF 15 A9 18 32 00  call        qword ptr [0000000065698968h]
  00000000653770BF: 48 8B D8           mov         rbx,rax
  00000000653770C2: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000653770C7: 48 8B CE           mov         rcx,rsi
  00000000653770CA: FF 15 98 18 32 00  call        qword ptr [0000000065698968h]
  00000000653770D0: BA 01 00 00 00     mov         edx,1
  00000000653770D5: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  00000000653770D9: 8B 0B              mov         ecx,dword ptr [rbx]
  00000000653770DB: 39 08              cmp         dword ptr [rax],ecx
  00000000653770DD: 41 0F 42 D0        cmovb       edx,r8d
  00000000653770E1: 8B C2              mov         eax,edx
  00000000653770E3: E9 C0 01 00 00     jmp         00000000653772A8
  00000000653770E8: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000653770ED: 48 8B CF           mov         rcx,rdi
  00000000653770F0: FF 15 8A 18 32 00  call        qword ptr [0000000065698980h]
  00000000653770F6: 48 8B D8           mov         rbx,rax
  00000000653770F9: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000653770FE: 48 8B CE           mov         rcx,rsi
  0000000065377101: FF 15 79 18 32 00  call        qword ptr [0000000065698980h]
  0000000065377107: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000065377109: 39 08              cmp         dword ptr [rax],ecx
  000000006537710B: 75 07              jne         0000000065377114
  000000006537710D: 33 C0              xor         eax,eax
  000000006537710F: E9 94 01 00 00     jmp         00000000653772A8
  0000000065377114: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000065377119: 48 8B CF           mov         rcx,rdi
  000000006537711C: FF 15 5E 18 32 00  call        qword ptr [0000000065698980h]
  0000000065377122: 48 8B D8           mov         rbx,rax
  0000000065377125: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000006537712A: 48 8B CE           mov         rcx,rsi
  000000006537712D: FF 15 4D 18 32 00  call        qword ptr [0000000065698980h]
  0000000065377133: BA 01 00 00 00     mov         edx,1
  0000000065377138: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  000000006537713C: 8B 0B              mov         ecx,dword ptr [rbx]
  000000006537713E: 39 08              cmp         dword ptr [rax],ecx
  0000000065377140: 41 0F 4C D0        cmovl       edx,r8d
  0000000065377144: 8B C2              mov         eax,edx
  0000000065377146: E9 5D 01 00 00     jmp         00000000653772A8
  000000006537714B: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000065377150: 48 8B CF           mov         rcx,rdi
  0000000065377153: FF 15 F7 17 32 00  call        qword ptr [0000000065698950h]
  0000000065377159: 48 8B D8           mov         rbx,rax
  000000006537715C: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065377161: 48 8B CE           mov         rcx,rsi
  0000000065377164: FF 15 E6 17 32 00  call        qword ptr [0000000065698950h]
  000000006537716A: 90                 nop
  000000006537716B: 48 8B D3           mov         rdx,rbx
  000000006537716E: 48 8B C8           mov         rcx,rax
  0000000065377171: FF 15 01 18 32 00  call        qword ptr [0000000065698978h]
  0000000065377177: 0F B6 D8           movzx       ebx,al
  000000006537717A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006537717F: FF 15 83 0A 32 00  call        qword ptr [0000000065697C08h]
  0000000065377185: 90                 nop
  0000000065377186: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006537718B: FF 15 77 0A 32 00  call        qword ptr [0000000065697C08h]
  0000000065377191: 84 DB              test        bl,bl
  0000000065377193: 74 07              je          000000006537719C
  0000000065377195: 33 C0              xor         eax,eax
  0000000065377197: E9 0C 01 00 00     jmp         00000000653772A8
  000000006537719C: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  00000000653771A1: 48 8B CF           mov         rcx,rdi
  00000000653771A4: FF 15 A6 17 32 00  call        qword ptr [0000000065698950h]
  00000000653771AA: 48 8B D8           mov         rbx,rax
  00000000653771AD: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000000653771B2: 48 8B CE           mov         rcx,rsi
  00000000653771B5: FF 15 95 17 32 00  call        qword ptr [0000000065698950h]
  00000000653771BB: 90                 nop
  00000000653771BC: 48 8B D3           mov         rdx,rbx
  00000000653771BF: 48 8B C8           mov         rcx,rax
  00000000653771C2: FF 15 A8 17 32 00  call        qword ptr [0000000065698970h]
  00000000653771C8: 0F B6 D8           movzx       ebx,al
  00000000653771CB: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000000653771D0: FF 15 32 0A 32 00  call        qword ptr [0000000065697C08h]
  00000000653771D6: 90                 nop
  00000000653771D7: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000653771DC: FF 15 26 0A 32 00  call        qword ptr [0000000065697C08h]
  00000000653771E2: B8 01 00 00 00     mov         eax,1
  00000000653771E7: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  00000000653771EB: 84 DB              test        bl,bl
  00000000653771ED: 41 0F 45 C0        cmovne      eax,r8d
  00000000653771F1: E9 B2 00 00 00     jmp         00000000653772A8
  00000000653771F6: 33 D2              xor         edx,edx
  00000000653771F8: 48 8B CE           mov         rcx,rsi
  00000000653771FB: FF 15 E7 06 32 00  call        qword ptr [00000000656978E8h]
  0000000065377201: 8B D8              mov         ebx,eax
  0000000065377203: 33 D2              xor         edx,edx
  0000000065377205: 48 8B CF           mov         rcx,rdi
  0000000065377208: FF 15 DA 06 32 00  call        qword ptr [00000000656978E8h]
  000000006537720E: 3B D8              cmp         ebx,eax
  0000000065377210: 75 07              jne         0000000065377219
  0000000065377212: 33 C0              xor         eax,eax
  0000000065377214: E9 8F 00 00 00     jmp         00000000653772A8
  0000000065377219: 33 D2              xor         edx,edx
  000000006537721B: 48 8B CE           mov         rcx,rsi
  000000006537721E: FF 15 C4 06 32 00  call        qword ptr [00000000656978E8h]
  0000000065377224: 8B D8              mov         ebx,eax
  0000000065377226: 33 D2              xor         edx,edx
  0000000065377228: 48 8B CF           mov         rcx,rdi
  000000006537722B: FF 15 B7 06 32 00  call        qword ptr [00000000656978E8h]
  0000000065377231: BA 01 00 00 00     mov         edx,1
  0000000065377236: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  000000006537723A: 3B D8              cmp         ebx,eax
  000000006537723C: 41 0F 4C D0        cmovl       edx,r8d
  0000000065377240: 8B C2              mov         eax,edx
  0000000065377242: EB 64              jmp         00000000653772A8
  0000000065377244: 33 D2              xor         edx,edx
  0000000065377246: 48 8B CE           mov         rcx,rsi
  0000000065377249: FF 15 89 10 32 00  call        qword ptr [00000000656982D8h]
  000000006537724F: 66 0F 28 F0        movapd      xmm6,xmm0
  0000000065377253: 33 D2              xor         edx,edx
  0000000065377255: 48 8B CF           mov         rcx,rdi
  0000000065377258: FF 15 7A 10 32 00  call        qword ptr [00000000656982D8h]
  000000006537725E: 66 0F 2E F0        ucomisd     xmm6,xmm0
  0000000065377262: 7A 06              jp          000000006537726A
  0000000065377264: 75 04              jne         000000006537726A
  0000000065377266: 33 C0              xor         eax,eax
  0000000065377268: EB 3E              jmp         00000000653772A8
  000000006537726A: 33 D2              xor         edx,edx
  000000006537726C: 48 8B CE           mov         rcx,rsi
  000000006537726F: FF 15 63 10 32 00  call        qword ptr [00000000656982D8h]
  0000000065377275: 66 0F 28 F0        movapd      xmm6,xmm0
  0000000065377279: 33 D2              xor         edx,edx
  000000006537727B: 48 8B CF           mov         rcx,rdi
  000000006537727E: FF 15 54 10 32 00  call        qword ptr [00000000656982D8h]
  0000000065377284: 66 0F 2F C6        comisd      xmm0,xmm6
  0000000065377288: 76 05              jbe         000000006537728F
  000000006537728A: 83 C8 FF           or          eax,0FFFFFFFFh
  000000006537728D: EB 19              jmp         00000000653772A8
  000000006537728F: B8 01 00 00 00     mov         eax,1
  0000000065377294: EB 12              jmp         00000000653772A8
  0000000065377296: 48 8B CF           mov         rcx,rdi
  0000000065377299: FF 15 A9 FD 31 00  call        qword ptr [0000000065697048h]
  000000006537729F: 85 C0              test        eax,eax
  00000000653772A1: 74 05              je          00000000653772A8
  00000000653772A3: B8 FE FF FF FF     mov         eax,0FFFFFFFEh
  00000000653772A8: 4C 8D 9C 24 80 00  lea         r11,[rsp+0000000000000080h]
                    00 00
  00000000653772B0: 49 8B 5B 10        mov         rbx,qword ptr [r11+10h]
  00000000653772B4: 49 8B 73 18        mov         rsi,qword ptr [r11+18h]
  00000000653772B8: 0F 28 74 24 70     movaps      xmm6,xmmword ptr [rsp+70h]
  00000000653772BD: 49 8B E3           mov         rsp,r11
  00000000653772C0: 5F                 pop         rdi
  00000000653772C1: C3                 ret
  00000000653772C2: 66 90              nop
  00000000653772C4: 96                 xchg        eax,esi
  00000000653772C5: 72 37              jb          00000000653772FE
  00000000653772C7: 00 A3 72 37 00 F6  add         byte ptr [rbx+FFFFFFFFF6003772h],ah
  00000000653772CD: 71 37              jno         0000000065377306
  00000000653772CF: 00 A3 72 37 00 A3  add         byte ptr [rbx+FFFFFFFFA3003772h],ah
  00000000653772D5: 72 37              jb          000000006537730E
  00000000653772D7: 00 A3 72 37 00 44  add         byte ptr [rbx+0000000044003772h],ah
  00000000653772DD: 72 37              jb          0000000065377316
  00000000653772DF: 00 A3 72 37 00 A3  add         byte ptr [rbx+FFFFFFFFA3003772h],ah
  00000000653772E5: 72 37              jb          000000006537731E
  00000000653772E7: 00 A3 72 37 00 A3  add         byte ptr [rbx+FFFFFFFFA3003772h],ah
  00000000653772ED: 72 37              jb          0000000065377326
  00000000653772EF: 00 A3 72 37 00 A3  add         byte ptr [rbx+FFFFFFFFA3003772h],ah
  00000000653772F5: 72 37              jb          000000006537732E
  00000000653772F7: 00 A3 72 37 00 7F  add         byte ptr [rbx+000000007F003772h],ah
  00000000653772FD: 70 37              jo          0000000065377336
  00000000653772FF: 00 E8              add         al,ch
  0000000065377301: 70 37              jo          000000006537733A
  0000000065377303: 00 4B 71           add         byte ptr [rbx+71h],cl
  0000000065377306: 37
  0000000065377307: 00

  Summary

      695000 .text
