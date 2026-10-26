Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006509915A: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006509915F: 48 89 7C 24 48     mov         qword ptr [rsp+48h],rdi
  0000000065099164: 4C 89 6C 24 50     mov         qword ptr [rsp+50h],r13
  0000000065099169: FF 15 B1 D6 5F 00  call        qword ptr [0000000065696820h]
  000000006509916F: 48 8B C8           mov         rcx,rax
  0000000065099172: FF 15 50 E5 5F 00  call        qword ptr [00000000656976C8h]
  0000000065099178: 48 8B D8           mov         rbx,rax
  000000006509917B: FF 15 8F E6 5F 00  call        qword ptr [0000000065697810h]
  0000000065099181: 48 3B C3           cmp         rax,rbx
  0000000065099184: 41 0F 95 C5        setne       r13b
  0000000065099188: 45 84 ED           test        r13b,r13b
  000000006509918B: 74 0D              je          000000006509919A
  000000006509918D: 33 C9              xor         ecx,ecx
  000000006509918F: FF 15 23 09 60 00  call        qword ptr [0000000065699AB8h]
  0000000065099195: 48 8B F8           mov         rdi,rax
  0000000065099198: EB 08              jmp         00000000650991A2
  000000006509919A: E8 31 68 FE FF     call        000000006507F9D0
  000000006509919F: 48 8B F8           mov         rdi,rax
  00000000650991A2: 8D 45 FD           lea         eax,[rbp-3]
  00000000650991A5: 83 F8 07           cmp         eax,7
  00000000650991A8: 0F 87 D3 01 00 00  ja          0000000065099381
  00000000650991AE: 48 8D 15 4B 6E F6  lea         rdx,[0000000065000000h]
                    FF
  00000000650991B5: 48 98              cdqe
  00000000650991B7: 8B 8C 82 BC 93 09  mov         ecx,dword ptr [rdx+rax*4+00000000000993BCh]
                    00
  00000000650991BE: 48 03 CA           add         rcx,rdx
  00000000650991C1: FF E1              jmp         rcx
  00000000650991C3: 48 8B 86 88 00 00  mov         rax,qword ptr [rsi+0000000000000088h]
                    00
  00000000650991CA: 48 85 C0           test        rax,rax
  00000000650991CD: 74 11              je          00000000650991E0
  00000000650991CF: 0F B7 48 50        movzx       ecx,word ptr [rax+50h]
  00000000650991D3: 66 85 C9           test        cx,cx
  00000000650991D6: 74 08              je          00000000650991E0
  00000000650991D8: 0F B7 D9           movzx       ebx,cx
  00000000650991DB: E9 B0 01 00 00     jmp         0000000065099390
  00000000650991E0: 48 83 7E 10 00     cmp         qword ptr [rsi+10h],0
  00000000650991E5: 74 18              je          00000000650991FF
  00000000650991E7: 48 8B 4E 10        mov         rcx,qword ptr [rsi+10h]
  00000000650991EB: 8B D5              mov         edx,ebp
  00000000650991ED: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000650991F1: 48 83 C1 10        add         rcx,10h
  00000000650991F5: FF 50 28           call        qword ptr [rax+28h]
  00000000650991F8: 8B D8              mov         ebx,eax
  00000000650991FA: E9 91 01 00 00     jmp         0000000065099390
  00000000650991FF: BA 58 00 00 00     mov         edx,58h
  0000000065099204: 48 8B CF           mov         rcx,rdi
  0000000065099207: FF 15 1B CF 5F 00  call        qword ptr [0000000065696128h]
  000000006509920D: 8B D8              mov         ebx,eax
  000000006509920F: E9 7C 01 00 00     jmp         0000000065099390
  0000000065099214: 48 8B 86 88 00 00  mov         rax,qword ptr [rsi+0000000000000088h]
                    00
  000000006509921B: 48 85 C0           test        rax,rax
  000000006509921E: 74 11              je          0000000065099231
  0000000065099220: 0F B7 48 52        movzx       ecx,word ptr [rax+52h]
  0000000065099224: 66 85 C9           test        cx,cx
  0000000065099227: 74 08              je          0000000065099231
  0000000065099229: 0F B7 D9           movzx       ebx,cx
  000000006509922C: E9 5F 01 00 00     jmp         0000000065099390
  0000000065099231: 48 83 7E 10 00     cmp         qword ptr [rsi+10h],0
  0000000065099236: 74 18              je          0000000065099250
  0000000065099238: 48 8B 4E 10        mov         rcx,qword ptr [rsi+10h]
  000000006509923C: 8B D5              mov         edx,ebp
  000000006509923E: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  0000000065099242: 48 83 C1 10        add         rcx,10h
  0000000065099246: FF 50 28           call        qword ptr [rax+28h]
  0000000065099249: 8B D8              mov         ebx,eax
  000000006509924B: E9 40 01 00 00     jmp         0000000065099390
  0000000065099250: BA 5A 00 00 00     mov         edx,5Ah
  0000000065099255: 48 8B CF           mov         rcx,rdi
  0000000065099258: FF 15 CA CE 5F 00  call        qword ptr [0000000065696128h]
  000000006509925E: 8B D8              mov         ebx,eax
  0000000065099260: E9 2B 01 00 00     jmp         0000000065099390
  0000000065099265: 49 8B 4C 24 10     mov         rcx,qword ptr [r12+10h]
  000000006509926A: 48 83 C1 14        add         rcx,14h
  000000006509926E: FF 15 EC D5 5F 00  call        qword ptr [0000000065696860h]
  0000000065099274: BA 04 00 00 00     mov         edx,4
  0000000065099279: 48 8B CF           mov         rcx,rdi
  000000006509927C: 8B D8              mov         ebx,eax
  000000006509927E: FF 15 A4 CE 5F 00  call        qword ptr [0000000065696128h]
  0000000065099284: BA 08 00 00 00     mov         edx,8
  0000000065099289: 48 8B CF           mov         rcx,rdi
  000000006509928C: 0F AF D8           imul        ebx,eax
  000000006509928F: FF 15 93 CE 5F 00  call        qword ptr [0000000065696128h]
  0000000065099295: 44 8B D8           mov         r11d,eax
  0000000065099298: 8B C3              mov         eax,ebx
  000000006509929A: 99                 cdq
  000000006509929B: 41 F7 FB           idiv        eax,r11d
  000000006509929E: 8B D8              mov         ebx,eax
  00000000650992A0: E9 EB 00 00 00     jmp         0000000065099390
  00000000650992A5: 49 8B 4C 24 10     mov         rcx,qword ptr [r12+10h]
  00000000650992AA: 48 83 C1 14        add         rcx,14h
  00000000650992AE: FF 15 74 D4 5F 00  call        qword ptr [0000000065696728h]
  00000000650992B4: BA 06 00 00 00     mov         edx,6
  00000000650992B9: 48 8B CF           mov         rcx,rdi
  00000000650992BC: 8B D8              mov         ebx,eax
  00000000650992BE: FF 15 64 CE 5F 00  call        qword ptr [0000000065696128h]
  00000000650992C4: BA 0A 00 00 00     mov         edx,0Ah
  00000000650992C9: 48 8B CF           mov         rcx,rdi
  00000000650992CC: 0F AF D8           imul        ebx,eax
  00000000650992CF: FF 15 53 CE 5F 00  call        qword ptr [0000000065696128h]
  00000000650992D5: 44 8B D8           mov         r11d,eax
  00000000650992D8: 8B C3              mov         eax,ebx
  00000000650992DA: 99                 cdq
  00000000650992DB: 41 F7 FB           idiv        eax,r11d
  00000000650992DE: 8B D8              mov         ebx,eax
  00000000650992E0: E9 AB 00 00 00     jmp         0000000065099390
  00000000650992E5: BA 26 00 00 00     mov         edx,26h
  00000000650992EA: 48 8B CF           mov         rcx,rdi
  00000000650992ED: FF 15 35 CE 5F 00  call        qword ptr [0000000065696128h]
  00000000650992F3: 0F BA E0 08        bt          eax,8
  00000000650992F7: 73 15              jae         000000006509930E
  00000000650992F9: BA 68 00 00 00     mov         edx,68h
  00000000650992FE: 48 8B CF           mov         rcx,rdi
  0000000065099301: FF 15 21 CE 5F 00  call        qword ptr [0000000065696128h]
  0000000065099307: 8B D8              mov         ebx,eax
  0000000065099309: E9 82 00 00 00     jmp         0000000065099390
  000000006509930E: 48 8B 86 40 01 00  mov         rax,qword ptr [rsi+0000000000000140h]
                    00
  0000000065099315: 48 8B DF           mov         rbx,rdi
  0000000065099318: BA 0C 00 00 00     mov         edx,0Ch
  000000006509931D: 48 85 C0           test        rax,rax
  0000000065099320: 48 0F 45 D8        cmovne      rbx,rax
  0000000065099324: 48 8B CB           mov         rcx,rbx
  0000000065099327: FF 15 FB CD 5F 00  call        qword ptr [0000000065696128h]
  000000006509932D: 8B F0              mov         esi,eax
  000000006509932F: 83 F8 20           cmp         eax,20h
  0000000065099332: 75 07              jne         000000006509933B
  0000000065099334: BB FF FF FF 7F     mov         ebx,7FFFFFFFh
  0000000065099339: EB 55              jmp         0000000065099390
  000000006509933B: 83 F8 08           cmp         eax,8
  000000006509933E: 7F 12              jg          0000000065099352
  0000000065099340: BA 18 00 00 00     mov         edx,18h
  0000000065099345: 48 8B CB           mov         rcx,rbx
  0000000065099348: FF 15 DA CD 5F 00  call        qword ptr [0000000065696128h]
  000000006509934E: 8B D8              mov         ebx,eax
  0000000065099350: EB 3E              jmp         0000000065099390
  0000000065099352: BA 0E 00 00 00     mov         edx,0Eh
  0000000065099357: 48 8B CB           mov         rcx,rbx
  000000006509935A: FF 15 C8 CD 5F 00  call        qword ptr [0000000065696128h]
  0000000065099360: BB 01 00 00 00     mov         ebx,1
  0000000065099365: 0F AF C6           imul        eax,esi
  0000000065099368: 0F B6 C8           movzx       ecx,al
  000000006509936B: D3 E3              shl         ebx,cl
  000000006509936D: EB 21              jmp         0000000065099390
  000000006509936F: BA 0C 00 00 00     mov         edx,0Ch
  0000000065099374: 48 8B CF           mov         rcx,rdi
  0000000065099377: FF 15 AB CD 5F 00  call        qword ptr [0000000065696128h]
  000000006509937D: 8B D8              mov         ebx,eax
  000000006509937F: EB 0F              jmp         0000000065099390
  0000000065099381: 48 8D 0D 38 70 61  lea         rcx,[00000000656B03C0h]
                    00
  0000000065099388: 33 DB              xor         ebx,ebx
  000000006509938A: FF 15 08 D6 5F 00  call        qword ptr [0000000065696998h]
  0000000065099390: 45 84 ED           test        r13b,r13b
  0000000065099393: 4C 8B 6C 24 50     mov         r13,qword ptr [rsp+50h]
  0000000065099398: 74 0B              je          00000000650993A5
  000000006509939A: 48 8B D7           mov         rdx,rdi
  000000006509939D: 33 C9              xor         ecx,ecx
  000000006509939F: FF 15 0B 07 60 00  call        qword ptr [0000000065699AB0h]
  00000000650993A5: 48 8B 7C 24 48     mov         rdi,qword ptr [rsp+48h]
  00000000650993AA: 8B C3              mov         eax,ebx
  00000000650993AC: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000650993B1: 48 83 C4 20        add         rsp,20h
  00000000650993B5: 41 5C              pop         r12
  00000000650993B7: 5E                 pop         rsi
  00000000650993B8: 5D                 pop         rbp
  00000000650993B9: C3                 ret
  00000000650993BA: 66 90              nop
  00000000650993BC: 65 92              xchg        eax,edx
  00000000650993BE: 09 00              or          dword ptr [rax],eax
  00000000650993C0: A5                 movs        dword ptr [rdi],dword ptr [rsi]
  00000000650993C1: 92                 xchg        eax,edx
  00000000650993C2: 09 00              or          dword ptr [rax],eax
  00000000650993C4: E5 92              in          eax,92h
  00000000650993C6: 09 00              or          dword ptr [rax],eax
  00000000650993C8: 6F                 outs        dx,dword ptr [rsi]
  00000000650993C9: 93                 xchg        eax,ebx
  00000000650993CA: 09 00              or          dword ptr [rax],eax
  00000000650993CC: C3                 ret
  00000000650993CD: 91                 xchg        eax,ecx
  00000000650993CE: 09 00              or          dword ptr [rax],eax
  00000000650993D0: 14 92              adc         al,92h
  00000000650993D2: 09 00              or          dword ptr [rax],eax
  00000000650993D4: C3                 ret
  00000000650993D5: 91                 xchg        eax,ecx
  00000000650993D6: 09 00              or          dword ptr [rax],eax
  00000000650993D8: 14 92              adc         al,92h
  00000000650993DA: 09 00              or          dword ptr [rax],eax
  00000000650993DC: CC                 int         3
  00000000650993DD: CC                 int         3
  00000000650993DE: CC                 int         3
  00000000650993DF: CC                 int         3

  Summary

      695000 .text
