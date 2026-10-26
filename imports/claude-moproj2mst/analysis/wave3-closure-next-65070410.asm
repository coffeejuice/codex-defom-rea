Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065070410: 40 56              push        rsi
  0000000065070412: 57                 push        rdi
  0000000065070413: 41 54              push        r12
  0000000065070415: 48 83 EC 40        sub         rsp,40h
  0000000065070419: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065070422: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  0000000065070427: 48 89 6C 24 78     mov         qword ptr [rsp+78h],rbp
  000000006507042C: 48 8B F9           mov         rdi,rcx
  000000006507042F: 84 D2              test        dl,dl
  0000000065070431: 0F 84 28 03 00 00  je          000000006507075F
  0000000065070437: 48 8B 69 08        mov         rbp,qword ptr [rcx+8]
  000000006507043B: 8B 85 CC 01 00 00  mov         eax,dword ptr [rbp+00000000000001CCh]
  0000000065070441: C1 E8 05           shr         eax,5
  0000000065070444: 24 01              and         al,1
  0000000065070446: 74 11              je          0000000065070459
  0000000065070448: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006507044C: 0F B6 48 0A        movzx       ecx,byte ptr [rax+0Ah]
  0000000065070450: 80 E1 01           and         cl,1
  0000000065070453: 0F 84 E6 04 00 00  je          000000006507093F
  0000000065070459: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006507045D: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  0000000065070461: A8 01              test        al,1
  0000000065070463: 75 59              jne         00000000650704BE
  0000000065070465: 48 8B CF           mov         rcx,rdi
  0000000065070468: FF 15 FA 63 62 00  call        qword ptr [0000000065696868h]
  000000006507046E: 48 85 C0           test        rax,rax
  0000000065070471: 74 4B              je          00000000650704BE
  0000000065070473: 48 8B CF           mov         rcx,rdi
  0000000065070476: FF 15 EC 63 62 00  call        qword ptr [0000000065696868h]
  000000006507047C: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  0000000065070480: 8B 41 08           mov         eax,dword ptr [rcx+8]
  0000000065070483: C1 E8 0F           shr         eax,0Fh
  0000000065070486: 24 01              and         al,1
  0000000065070488: 74 34              je          00000000650704BE
  000000006507048A: 48 8B CF           mov         rcx,rdi
  000000006507048D: FF 15 D5 63 62 00  call        qword ptr [0000000065696868h]
  0000000065070493: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065070497: 8B 81 C8 01 00 00  mov         eax,dword ptr [rcx+00000000000001C8h]
  000000006507049D: C1 E8 1C           shr         eax,1Ch
  00000000650704A0: 24 01              and         al,1
  00000000650704A2: 75 1A              jne         00000000650704BE
  00000000650704A4: 48 8B CF           mov         rcx,rdi
  00000000650704A7: FF 15 BB 63 62 00  call        qword ptr [0000000065696868h]
  00000000650704AD: 48 8B C8           mov         rcx,rax
  00000000650704B0: E8 AB C6 FE FF     call        000000006505CB60
  00000000650704B5: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000000650704B9: E8 E2 88 FF FF     call        0000000065068DA0
  00000000650704BE: 48 8B CF           mov         rcx,rdi
  00000000650704C1: FF 15 A1 63 62 00  call        qword ptr [0000000065696868h]
  00000000650704C7: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  00000000650704CB: 8B 91 C8 01 00 00  mov         edx,dword ptr [rcx+00000000000001C8h]
  00000000650704D1: C1 EA 1C           shr         edx,1Ch
  00000000650704D4: 80 E2 01           and         dl,1
  00000000650704D7: 75 30              jne         0000000065070509
  00000000650704D9: 48 8B 4F 20        mov         rcx,qword ptr [rdi+20h]
  00000000650704DD: 0F B6 49 0C        movzx       ecx,byte ptr [rcx+0Ch]
  00000000650704E1: F6 C1 01           test        cl,1
  00000000650704E4: 75 12              jne         00000000650704F8
  00000000650704E6: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000650704EA: 8B 88 C8 01 00 00  mov         ecx,dword ptr [rax+00000000000001C8h]
  00000000650704F0: C1 E9 1C           shr         ecx,1Ch
  00000000650704F3: 80 E1 01           and         cl,1
  00000000650704F6: 74 11              je          0000000065070509
  00000000650704F8: 41 B1 01           mov         r9b,1
  00000000650704FB: 45 0F B6 C1        movzx       r8d,r9b
  00000000650704FF: 33 D2              xor         edx,edx
  0000000065070501: 48 8B CF           mov         rcx,rdi
  0000000065070504: E8 F7 73 FF FF     call        0000000065067900
  0000000065070509: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006507050D: 44 8B A0 C8 01 00  mov         r12d,dword ptr [rax+00000000000001C8h]
                    00
  0000000065070514: 41 C1 EC 0A        shr         r12d,0Ah
  0000000065070518: 41 80 E4 01        and         r12b,1
  000000006507051C: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065070520: 8B 50 10           mov         edx,dword ptr [rax+10h]
  0000000065070523: 83 E2 0F           and         edx,0Fh
  0000000065070526: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000006507052B: FF 15 4F 69 62 00  call        qword ptr [0000000065696E80h]
  0000000065070531: 8B 08              mov         ecx,dword ptr [rax]
  0000000065070533: 89 4C 24 68        mov         dword ptr [rsp+68h],ecx
  0000000065070537: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006507053C: FF 15 16 69 62 00  call        qword ptr [0000000065696E58h]
  0000000065070542: 8B D8              mov         ebx,eax
  0000000065070544: 48 8B CF           mov         rcx,rdi
  0000000065070547: E8 04 4A FF FF     call        0000000065064F50
  000000006507054C: 41 B0 01           mov         r8b,1
  000000006507054F: BA 45 00 00 00     mov         edx,45h
  0000000065070554: 48 8B CF           mov         rcx,rdi
  0000000065070557: E8 84 8A FF FF     call        0000000065068FE0
  000000006507055C: 4C 8B 5F 20        mov         r11,qword ptr [rdi+20h]
  0000000065070560: 41 0F B6 4B 0C     movzx       ecx,byte ptr [r11+0Ch]
  0000000065070565: F6 C1 01           test        cl,1
  0000000065070568: 75 0E              jne         0000000065070578
  000000006507056A: 41 0F B6 43 0A     movzx       eax,byte ptr [r11+0Ah]
  000000006507056F: 24 01              and         al,1
  0000000065070571: 74 05              je          0000000065070578
  0000000065070573: 40 B6 01           mov         sil,1
  0000000065070576: EB 03              jmp         000000006507057B
  0000000065070578: 40 32 F6           xor         sil,sil
  000000006507057B: 45 33 C0           xor         r8d,r8d
  000000006507057E: 41 8D 50 10        lea         edx,[r8+10h]
  0000000065070582: 48 8B CF           mov         rcx,rdi
  0000000065070585: E8 56 8A FF FF     call        0000000065068FE0
  000000006507058A: 40 84 F6           test        sil,sil
  000000006507058D: 74 0A              je          0000000065070599
  000000006507058F: B2 01              mov         dl,1
  0000000065070591: 48 8B CD           mov         rcx,rbp
  0000000065070594: E8 C7 1E FF FF     call        0000000065062460
  0000000065070599: BA 46 00 00 00     mov         edx,46h
  000000006507059E: 48 8B CF           mov         rcx,rdi
  00000000650705A1: FF 15 99 62 62 00  call        qword ptr [0000000065696840h]
  00000000650705A7: 48 8B 8D A8 00 00  mov         rcx,qword ptr [rbp+00000000000000A8h]
                    00
  00000000650705AE: 48 85 C9           test        rcx,rcx
  00000000650705B1: 74 05              je          00000000650705B8
  00000000650705B3: E8 28 54 FD FF     call        00000000650459E0
  00000000650705B8: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  00000000650705BC: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  00000000650705C0: A8 01              test        al,1
  00000000650705C2: 75 75              jne         0000000065070639
  00000000650705C4: 48 8B CF           mov         rcx,rdi
  00000000650705C7: FF 15 9B 62 62 00  call        qword ptr [0000000065696868h]
  00000000650705CD: 48 8B F0           mov         rsi,rax
  00000000650705D0: 48 85 C0           test        rax,rax
  00000000650705D3: 74 64              je          0000000065070639
  00000000650705D5: 66 66 66 0F 1F 84  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00 00
  00000000650705E0: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  00000000650705E4: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000650705E7: C1 E9 0F           shr         ecx,0Fh
  00000000650705EA: 80 E1 01           and         cl,1
  00000000650705ED: 74 41              je          0000000065070630
  00000000650705EF: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  00000000650705F3: 48 83 B9 A8 00 00  cmp         qword ptr [rcx+00000000000000A8h],0
                    00 00
  00000000650705FB: 74 33              je          0000000065070630
  00000000650705FD: 0F BA 60 10 0A     bt          dword ptr [rax+10h],0Ah
  0000000065070602: 72 2C              jb          0000000065070630
  0000000065070604: 48 8B 89 A8 00 00  mov         rcx,qword ptr [rcx+00000000000000A8h]
                    00
  000000006507060B: E8 D0 53 FD FF     call        00000000650459E0
  0000000065070610: 4C 8B 5E 20        mov         r11,qword ptr [rsi+20h]
  0000000065070614: 41 0F B6 43 0C     movzx       eax,byte ptr [r11+0Ch]
  0000000065070619: A8 01              test        al,1
  000000006507061B: 75 13              jne         0000000065070630
  000000006507061D: 48 8B CE           mov         rcx,rsi
  0000000065070620: FF 15 42 62 62 00  call        qword ptr [0000000065696868h]
  0000000065070626: 48 8B F0           mov         rsi,rax
  0000000065070629: 48 85 C0           test        rax,rax
  000000006507062C: 75 B2              jne         00000000650705E0
  000000006507062E: EB 09              jmp         0000000065070639
  0000000065070630: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  0000000065070634: E8 87 E5 FE FF     call        000000006505EBC0
  0000000065070639: 45 84 E4           test        r12b,r12b
  000000006507063C: 0F 85 8C 00 00 00  jne         00000000650706CE
  0000000065070642: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065070646: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006507064A: A8 01              test        al,1
  000000006507064C: 75 17              jne         0000000065070665
  000000006507064E: 48 8B CF           mov         rcx,rdi
  0000000065070651: FF 15 11 62 62 00  call        qword ptr [0000000065696868h]
  0000000065070657: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006507065B: 48 83 B9 A8 00 00  cmp         qword ptr [rcx+00000000000000A8h],0
                    00 00
  0000000065070663: 75 69              jne         00000000650706CE
  0000000065070665: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065070669: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006507066D: A8 01              test        al,1
  000000006507066F: 74 46              je          00000000650706B7
  0000000065070671: 48 8B CF           mov         rcx,rdi
  0000000065070674: E8 E7 C1 FF FF     call        000000006506C860
  0000000065070679: 4C 8B 5F 20        mov         r11,qword ptr [rdi+20h]
  000000006507067D: 41 8B 53 10        mov         edx,dword ptr [r11+10h]
  0000000065070681: 83 E2 0F           and         edx,0Fh
  0000000065070684: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000065070689: FF 15 F1 67 62 00  call        qword ptr [0000000065696E80h]
  000000006507068F: 8B 08              mov         ecx,dword ptr [rax]
  0000000065070691: 89 4C 24 68        mov         dword ptr [rsp+68h],ecx
  0000000065070695: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006507069A: FF 15 B8 67 62 00  call        qword ptr [0000000065696E58h]
  00000000650706A0: 3B C3              cmp         eax,ebx
  00000000650706A2: 74 1B              je          00000000650706BF
  00000000650706A4: 89 5C 24 68        mov         dword ptr [rsp+68h],ebx
  00000000650706A8: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000000650706AD: 48 8B CF           mov         rcx,rdi
  00000000650706B0: E8 FB 7D 02 00     call        00000000650984B0
  00000000650706B5: EB 08              jmp         00000000650706BF
  00000000650706B7: 48 8B CF           mov         rcx,rdi
  00000000650706BA: E8 A1 C1 FF FF     call        000000006506C860
  00000000650706BF: 45 33 C0           xor         r8d,r8d
  00000000650706C2: 41 8D 50 2A        lea         edx,[r8+2Ah]
  00000000650706C6: 48 8B CF           mov         rcx,rdi
  00000000650706C9: E8 12 89 FF FF     call        0000000065068FE0
  00000000650706CE: 45 33 C0           xor         r8d,r8d
  00000000650706D1: 41 8D 50 4D        lea         edx,[r8+4Dh]
  00000000650706D5: 48 8B CF           mov         rcx,rdi
  00000000650706D8: E8 03 89 FF FF     call        0000000065068FE0
  00000000650706DD: 4C 8B 5F 20        mov         r11,qword ptr [rdi+20h]
  00000000650706E1: 41 0F B6 43 0C     movzx       eax,byte ptr [r11+0Ch]
  00000000650706E6: A8 01              test        al,1
  00000000650706E8: 75 17              jne         0000000065070701
  00000000650706EA: 48 8B CF           mov         rcx,rdi
  00000000650706ED: FF 15 75 61 62 00  call        qword ptr [0000000065696868h]
  00000000650706F3: 48 8B 50 20        mov         rdx,qword ptr [rax+20h]
  00000000650706F7: 8B 42 08           mov         eax,dword ptr [rdx+8]
  00000000650706FA: C1 E8 0F           shr         eax,0Fh
  00000000650706FD: 24 01              and         al,1
  00000000650706FF: 74 2E              je          000000006507072F
  0000000065070701: FF 15 19 61 62 00  call        qword ptr [0000000065696820h]
  0000000065070707: 48 8B C8           mov         rcx,rax
  000000006507070A: BA 14 00 00 00     mov         edx,14h
  000000006507070F: FF 15 EB 6D 62 00  call        qword ptr [0000000065697500h]
  0000000065070715: 48 8B CD           mov         rcx,rbp
  0000000065070718: E8 D3 F6 FF FF     call        000000006506FDF0
  000000006507071D: FF 15 FD 60 62 00  call        qword ptr [0000000065696820h]
  0000000065070723: 48 8B D7           mov         rdx,rdi
  0000000065070726: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006507072A: E8 91 36 FB FF     call        0000000065023DC0
  000000006507072F: BA 1A 00 00 00     mov         edx,1Ah
  0000000065070734: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065070739: FF 15 D9 60 62 00  call        qword ptr [0000000065696818h]
  000000006507073F: 90                 nop
  0000000065070740: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065070745: 48 8B CF           mov         rcx,rdi
  0000000065070748: FF 15 C2 60 62 00  call        qword ptr [0000000065696810h]
  000000006507074E: 90                 nop
  000000006507074F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065070754: FF 15 AE 60 62 00  call        qword ptr [0000000065696808h]
  000000006507075A: E9 E0 01 00 00     jmp         000000006507093F
  000000006507075F: 48 8B 51 08        mov         rdx,qword ptr [rcx+8]
  0000000065070763: 8B 82 CC 01 00 00  mov         eax,dword ptr [rdx+00000000000001CCh]
  0000000065070769: C1 E8 05           shr         eax,5
  000000006507076C: 24 01              and         al,1
  000000006507076E: 74 11              je          0000000065070781
  0000000065070770: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  0000000065070774: 0F B6 48 0A        movzx       ecx,byte ptr [rax+0Ah]
  0000000065070778: 80 E1 01           and         cl,1
  000000006507077B: 0F 85 BE 01 00 00  jne         000000006507093F
  0000000065070781: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065070785: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  0000000065070789: A8 01              test        al,1
  000000006507078B: 74 4E              je          00000000650707DB
  000000006507078D: 8B 82 C8 01 00 00  mov         eax,dword ptr [rdx+00000000000001C8h]
  0000000065070793: C1 E8 1C           shr         eax,1Ch
  0000000065070796: 24 01              and         al,1
  0000000065070798: 74 41              je          00000000650707DB
  000000006507079A: 48 8B CF           mov         rcx,rdi
  000000006507079D: E8 9E 18 FB FF     call        0000000065022040
  00000000650707A2: 84 C0              test        al,al
  00000000650707A4: 74 35              je          00000000650707DB
  00000000650707A6: 48 8B CF           mov         rcx,rdi
  00000000650707A9: E8 92 95 FF FF     call        0000000065069D40
  00000000650707AE: 48 8B C8           mov         rcx,rax
  00000000650707B1: BA F0 FF FF FF     mov         edx,0FFFFFFF0h
  00000000650707B6: FF 15 04 93 62 00  call        qword ptr [0000000065699AC0h]
  00000000650707BC: 8B D8              mov         ebx,eax
  00000000650707BE: 0F BA F3 1B        btr         ebx,1Bh
  00000000650707C2: 48 8B CF           mov         rcx,rdi
  00000000650707C5: E8 76 95 FF FF     call        0000000065069D40
  00000000650707CA: 48 8B C8           mov         rcx,rax
  00000000650707CD: 44 8B C3           mov         r8d,ebx
  00000000650707D0: BA F0 FF FF FF     mov         edx,0FFFFFFF0h
  00000000650707D5: FF 15 ED 92 62 00  call        qword ptr [0000000065699AC8h]
  00000000650707DB: 48 8B 05 06 8F 95  mov         rax,qword ptr [00000000659C96E8h]
                    00
  00000000650707E2: 33 F6              xor         esi,esi
  00000000650707E4: 48 3B C7           cmp         rax,rdi
  00000000650707E7: 48 0F 44 C6        cmove       rax,rsi
  00000000650707EB: 48 89 05 F6 8E 95  mov         qword ptr [00000000659C96E8h],rax
                    00
  00000000650707F2: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  00000000650707F6: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  00000000650707FA: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  00000000650707FE: A8 01              test        al,1
  0000000065070800: 75 20              jne         0000000065070822
  0000000065070802: 48 8B CF           mov         rcx,rdi
  0000000065070805: FF 15 5D 60 62 00  call        qword ptr [0000000065696868h]
  000000006507080B: 48 85 C0           test        rax,rax
  000000006507080E: 74 12              je          0000000065070822
  0000000065070810: 48 8B CF           mov         rcx,rdi
  0000000065070813: FF 15 4F 60 62 00  call        qword ptr [0000000065696868h]
  0000000065070819: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006507081D: E8 9E E3 FE FF     call        000000006505EBC0
  0000000065070822: 41 B0 01           mov         r8b,1
  0000000065070825: BA 10 00 00 00     mov         edx,10h
  000000006507082A: 48 8B CF           mov         rcx,rdi
  000000006507082D: E8 AE 87 FF FF     call        0000000065068FE0
  0000000065070832: 41 B0 01           mov         r8b,1
  0000000065070835: BA 45 00 00 00     mov         edx,45h
  000000006507083A: 48 8B CF           mov         rcx,rdi
  000000006507083D: E8 9E 87 FF FF     call        0000000065068FE0
  0000000065070842: 4C 8B 5F 08        mov         r11,qword ptr [rdi+8]
  0000000065070846: 41 8B 83 C8 01 00  mov         eax,dword ptr [r11+00000000000001C8h]
                    00
  000000006507084D: C1 E8 1C           shr         eax,1Ch
  0000000065070850: 24 01              and         al,1
  0000000065070852: 74 08              je          000000006507085C
  0000000065070854: 48 8B CB           mov         rcx,rbx
  0000000065070857: E8 54 F8 FF FF     call        00000000650700B0
  000000006507085C: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065070860: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  0000000065070864: A8 01              test        al,1
  0000000065070866: 0F 85 A8 00 00 00  jne         0000000065070914
  000000006507086C: 48 8B CF           mov         rcx,rdi
  000000006507086F: FF 15 F3 5F 62 00  call        qword ptr [0000000065696868h]
  0000000065070875: 48 85 C0           test        rax,rax
  0000000065070878: 0F 84 96 00 00 00  je          0000000065070914
  000000006507087E: 48 8B CF           mov         rcx,rdi
  0000000065070881: FF 15 E1 5F 62 00  call        qword ptr [0000000065696868h]
  0000000065070887: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006507088B: 48 39 B1 A8 00 00  cmp         qword ptr [rcx+00000000000000A8h],rsi
                    00
  0000000065070892: 74 20              je          00000000650708B4
  0000000065070894: 48 8B CF           mov         rcx,rdi
  0000000065070897: FF 15 CB 5F 62 00  call        qword ptr [0000000065696868h]
  000000006507089D: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000000650708A1: 48 8B 89 A8 00 00  mov         rcx,qword ptr [rcx+00000000000000A8h]
                    00
  00000000650708A8: 48 83 C1 10        add         rcx,10h
  00000000650708AC: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000650708AF: FF 50 58           call        qword ptr [rax+58h]
  00000000650708B2: EB 60              jmp         0000000065070914
  00000000650708B4: 48 8B CF           mov         rcx,rdi
  00000000650708B7: FF 15 AB 5F 62 00  call        qword ptr [0000000065696868h]
  00000000650708BD: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  00000000650708C1: 8B 41 08           mov         eax,dword ptr [rcx+8]
  00000000650708C4: C1 E8 0F           shr         eax,0Fh
  00000000650708C7: 24 01              and         al,1
  00000000650708C9: 74 49              je          0000000065070914
  00000000650708CB: B9 18 00 00 00     mov         ecx,18h
  00000000650708D0: E8 9D 08 57 00     call        00000000655E1172
  00000000650708D5: 48 8B D8           mov         rbx,rax
  00000000650708D8: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000000650708DD: 48 85 C0           test        rax,rax
  00000000650708E0: 74 1A              je          00000000650708FC
  00000000650708E2: BA 4C 00 00 00     mov         edx,4Ch
  00000000650708E7: 48 8B C8           mov         rcx,rax
  00000000650708EA: FF 15 28 5F 62 00  call        qword ptr [0000000065696818h]
  00000000650708F0: 4C 8D 1D 59 DA 62  lea         r11,[000000006569E350h]
                    00
  00000000650708F7: 4C 89 1B           mov         qword ptr [rbx],r11
  00000000650708FA: EB 03              jmp         00000000650708FF
  00000000650708FC: 48 8B DE           mov         rbx,rsi
  00000000650708FF: 48 8B CF           mov         rcx,rdi
  0000000065070902: FF 15 60 5F 62 00  call        qword ptr [0000000065696868h]
  0000000065070908: 48 8B C8           mov         rcx,rax
  000000006507090B: 48 8B D3           mov         rdx,rbx
  000000006507090E: FF 15 DC 62 62 00  call        qword ptr [0000000065696BF0h]
  0000000065070914: BA 1B 00 00 00     mov         edx,1Bh
  0000000065070919: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006507091E: FF 15 F4 5E 62 00  call        qword ptr [0000000065696818h]
  0000000065070924: 90                 nop
  0000000065070925: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006507092A: 48 8B CF           mov         rcx,rdi
  000000006507092D: FF 15 DD 5E 62 00  call        qword ptr [0000000065696810h]
  0000000065070933: 90                 nop
  0000000065070934: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065070939: FF 15 C9 5E 62 00  call        qword ptr [0000000065696808h]
  000000006507093F: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  0000000065070944: 48 8B 6C 24 78     mov         rbp,qword ptr [rsp+78h]
  0000000065070949: 48 83 C4 40        add         rsp,40h
  000000006507094D: 41 5C              pop         r12
  000000006507094F: 5F                 pop         rdi
  0000000065070950: 5E                 pop         rsi
  0000000065070951: C3                 ret

  Summary

      695000 .text
