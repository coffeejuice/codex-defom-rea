Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650700B0: 48 8B C4           mov         rax,rsp
  00000000650700B3: 56                 push        rsi
  00000000650700B4: 57                 push        rdi
  00000000650700B5: 41 54              push        r12
  00000000650700B7: 48 83 EC 60        sub         rsp,60h
  00000000650700BB: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650700C4: 48 89 58 18        mov         qword ptr [rax+18h],rbx
  00000000650700C8: 48 89 68 20        mov         qword ptr [rax+20h],rbp
  00000000650700CC: 4C 8B E1           mov         r12,rcx
  00000000650700CF: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000650700D3: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  00000000650700D7: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  00000000650700DB: A8 01              test        al,1
  00000000650700DD: 74 5F              je          000000006507013E
  00000000650700DF: 48 8B CB           mov         rcx,rbx
  00000000650700E2: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  00000000650700E6: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  00000000650700E9: 25 00 00 00 20     and         eax,20000000h
  00000000650700EE: 75 4E              jne         000000006507013E
  00000000650700F0: FF 15 72 67 62 00  call        qword ptr [0000000065696868h]
  00000000650700F6: 48 8B C8           mov         rcx,rax
  00000000650700F9: 48 85 C0           test        rax,rax
  00000000650700FC: 75 E4              jne         00000000650700E2
  00000000650700FE: 48 8B CB           mov         rcx,rbx
  0000000065070101: FF 15 61 67 62 00  call        qword ptr [0000000065696868h]
  0000000065070107: 4C 8B D8           mov         r11,rax
  000000006507010A: 48 85 C0           test        rax,rax
  000000006507010D: 74 2F              je          000000006507013E
  000000006507010F: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065070113: 48 8B 81 88 00 00  mov         rax,qword ptr [rcx+0000000000000088h]
                    00
  000000006507011A: 48 85 C0           test        rax,rax
  000000006507011D: 74 09              je          0000000065070128
  000000006507011F: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000065070123: 48 85 C0           test        rax,rax
  0000000065070126: 75 11              jne         0000000065070139
  0000000065070128: 49 8B CB           mov         rcx,r11
  000000006507012B: FF 15 37 67 62 00  call        qword ptr [0000000065696868h]
  0000000065070131: 48 8B C8           mov         rcx,rax
  0000000065070134: E8 27 D8 FE FF     call        000000006505D960
  0000000065070139: 48 85 C0           test        rax,rax
  000000006507013C: 75 36              jne         0000000065070174
  000000006507013E: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  0000000065070142: 80 78 0C 09        cmp         byte ptr [rax+0Ch],9
  0000000065070146: 75 12              jne         000000006507015A
  0000000065070148: FF 15 D2 66 62 00  call        qword ptr [0000000065696820h]
  000000006507014E: 48 8B D3           mov         rdx,rbx
  0000000065070151: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065070155: E8 26 9D 01 00     call        0000000065089E80
  000000006507015A: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006507015E: F6 40 12 03        test        byte ptr [rax+12h],3
  0000000065070162: 74 10              je          0000000065070174
  0000000065070164: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  0000000065070168: A8 01              test        al,1
  000000006507016A: 74 08              je          0000000065070174
  000000006507016C: 48 8B CB           mov         rcx,rbx
  000000006507016F: E8 AC 24 FB FF     call        0000000065022620
  0000000065070174: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  0000000065070178: 0F B6 41 0C        movzx       eax,byte ptr [rcx+0Ch]
  000000006507017C: A8 01              test        al,1
  000000006507017E: 74 46              je          00000000650701C6
  0000000065070180: 80 79 0C 09        cmp         byte ptr [rcx+0Ch],9
  0000000065070184: 74 40              je          00000000650701C6
  0000000065070186: 48 8B CB           mov         rcx,rbx
  0000000065070189: FF 15 D9 66 62 00  call        qword ptr [0000000065696868h]
  000000006507018F: 48 85 C0           test        rax,rax
  0000000065070192: 74 32              je          00000000650701C6
  0000000065070194: 48 8B CB           mov         rcx,rbx
  0000000065070197: FF 15 CB 66 62 00  call        qword ptr [0000000065696868h]
  000000006507019D: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  00000000650701A1: 0F B6 41 0A        movzx       eax,byte ptr [rcx+0Ah]
  00000000650701A5: 24 01              and         al,1
  00000000650701A7: 75 1D              jne         00000000650701C6
  00000000650701A9: 48 8B CB           mov         rcx,rbx
  00000000650701AC: E8 1F 1B FF FF     call        0000000065061CD0
  00000000650701B1: 84 C0              test        al,al
  00000000650701B3: 74 11              je          00000000650701C6
  00000000650701B5: 48 8B CB           mov         rcx,rbx
  00000000650701B8: FF 15 AA 66 62 00  call        qword ptr [0000000065696868h]
  00000000650701BE: 48 8B C8           mov         rcx,rax
  00000000650701C1: E8 0A 6E 02 00     call        0000000065096FD0
  00000000650701C6: 45 33 C0           xor         r8d,r8d
  00000000650701C9: 41 8D 50 0B        lea         edx,[r8+0Bh]
  00000000650701CD: 48 8B CB           mov         rcx,rbx
  00000000650701D0: E8 0B 8E FF FF     call        0000000065068FE0
  00000000650701D5: 49 8B CC           mov         rcx,r12
  00000000650701D8: E8 53 95 02 00     call        0000000065099730
  00000000650701DD: 4C 8B 5B 20        mov         r11,qword ptr [rbx+20h]
  00000000650701E1: 41 8B 6B 08        mov         ebp,dword ptr [r11+8]
  00000000650701E5: C1 ED 0F           shr         ebp,0Fh
  00000000650701E8: 40 80 E5 01        and         bpl,1
  00000000650701EC: 74 0F              je          00000000650701FD
  00000000650701EE: 45 33 C0           xor         r8d,r8d
  00000000650701F1: 41 8D 50 0F        lea         edx,[r8+0Fh]
  00000000650701F5: 48 8B CB           mov         rcx,rbx
  00000000650701F8: E8 E3 8D FF FF     call        0000000065068FE0
  00000000650701FD: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065070202: E8 29 24 FC FF     call        0000000065032630
  0000000065070207: 90                 nop
  0000000065070208: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006507020D: 48 8B CB           mov         rcx,rbx
  0000000065070210: FF 15 FA 65 62 00  call        qword ptr [0000000065696810h]
  0000000065070216: 49 8B 4C 24 18     mov         rcx,qword ptr [r12+18h]
  000000006507021B: 48 89 8C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rcx
                    00 00
  0000000065070223: FF 15 97 66 62 00  call        qword ptr [00000000656968C0h]
  0000000065070229: 4C 8B 9C 24 88 00  mov         r11,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000065070231: 41 F6 43 10 01     test        byte ptr [r11+10h],1
  0000000065070236: 75 12              jne         000000006507024A
  0000000065070238: 41 8B 53 04        mov         edx,dword ptr [r11+4]
  000000006507023C: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000065070244: E8 27 F2 49 00     call        000000006550F470
  0000000065070249: 90                 nop
  000000006507024A: 33 F6              xor         esi,esi
  000000006507024C: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000065070254: FF 15 5E 66 62 00  call        qword ptr [00000000656968B8h]
  000000006507025A: 85 C0              test        eax,eax
  000000006507025C: 0F 8E EF 00 00 00  jle         0000000065070351
  0000000065070262: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    0F 1F 84 00 00 00
                    00 00
  0000000065070270: 8B D6              mov         edx,esi
  0000000065070272: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000006507027A: FF 15 C0 66 62 00  call        qword ptr [0000000065696940h]
  0000000065070280: 48 8B 38           mov         rdi,qword ptr [rax]
  0000000065070283: 48 85 FF           test        rdi,rdi
  0000000065070286: 0F 84 AD 00 00 00  je          0000000065070339
  000000006507028C: 48 8B CF           mov         rcx,rdi
  000000006507028F: FF 15 C3 65 62 00  call        qword ptr [0000000065696858h]
  0000000065070295: 84 C0              test        al,al
  0000000065070297: 0F 84 9C 00 00 00  je          0000000065070339
  000000006507029D: 48 8B 4F 20        mov         rcx,qword ptr [rdi+20h]
  00000000650702A1: 0F B6 41 0C        movzx       eax,byte ptr [rcx+0Ch]
  00000000650702A5: A8 01              test        al,1
  00000000650702A7: 0F 85 8C 00 00 00  jne         0000000065070339
  00000000650702AD: 0F B6 41 0A        movzx       eax,byte ptr [rcx+0Ah]
  00000000650702B1: 24 01              and         al,1
  00000000650702B3: 0F 85 80 00 00 00  jne         0000000065070339
  00000000650702B9: 45 33 C0           xor         r8d,r8d
  00000000650702BC: 41 8D 50 0F        lea         edx,[r8+0Fh]
  00000000650702C0: 48 8B CF           mov         rcx,rdi
  00000000650702C3: E8 18 8D FF FF     call        0000000065068FE0
  00000000650702C8: 33 D2              xor         edx,edx
  00000000650702CA: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  00000000650702CE: E8 5D C1 FF FF     call        000000006506C430
  00000000650702D3: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650702D8: E8 53 23 FC FF     call        0000000065032630
  00000000650702DD: 90                 nop
  00000000650702DE: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000650702E3: 48 8B CF           mov         rcx,rdi
  00000000650702E6: FF 15 24 65 62 00  call        qword ptr [0000000065696810h]
  00000000650702EC: 4C 8B 5F 20        mov         r11,qword ptr [rdi+20h]
  00000000650702F0: 49 83 3B 00        cmp         qword ptr [r11],0
  00000000650702F4: 74 16              je          000000006507030C
  00000000650702F6: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  00000000650702FA: 8B 81 D0 01 00 00  mov         eax,dword ptr [rcx+00000000000001D0h]
  0000000065070300: C1 E8 05           shr         eax,5
  0000000065070303: 24 01              and         al,1
  0000000065070305: 74 05              je          000000006507030C
  0000000065070307: E8 24 94 02 00     call        0000000065099730
  000000006507030C: FF 15 0E 65 62 00  call        qword ptr [0000000065696820h]
  0000000065070312: 48 8B D7           mov         rdx,rdi
  0000000065070315: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065070319: E8 A2 3A FB FF     call        0000000065023DC0
  000000006507031E: 33 D2              xor         edx,edx
  0000000065070320: 41 B8 03 80 00 00  mov         r8d,8003h
  0000000065070326: 48 8B CF           mov         rcx,rdi
  0000000065070329: E8 32 26 43 00     call        00000000654A2960
  000000006507032E: 90                 nop
  000000006507032F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065070334: E8 27 23 FC FF     call        0000000065032660
  0000000065070339: FF C6              inc         esi
  000000006507033B: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000065070343: FF 15 6F 65 62 00  call        qword ptr [00000000656968B8h]
  0000000065070349: 3B F0              cmp         esi,eax
  000000006507034B: 0F 8C 1F FF FF FF  jl          0000000065070270
  0000000065070351: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000065070359: FF 15 19 66 62 00  call        qword ptr [0000000065696978h]
  000000006507035F: 84 C0              test        al,al
  0000000065070361: 75 0E              jne         0000000065070371
  0000000065070363: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000006507036B: FF 15 EF 65 62 00  call        qword ptr [0000000065696960h]
  0000000065070371: 40 84 ED           test        bpl,bpl
  0000000065070374: 74 4C              je          00000000650703C2
  0000000065070376: FF 15 A4 64 62 00  call        qword ptr [0000000065696820h]
  000000006507037C: 48 8B D3           mov         rdx,rbx
  000000006507037F: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065070383: E8 38 3A FB FF     call        0000000065023DC0
  0000000065070388: E8 03 6D FA FF     call        0000000065017090
  000000006507038D: 48 85 C0           test        rax,rax
  0000000065070390: 74 30              je          00000000650703C2
  0000000065070392: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  0000000065070396: 0F B6 49 0C        movzx       ecx,byte ptr [rcx+0Ch]
  000000006507039A: F6 C1 01           test        cl,1
  000000006507039D: 75 23              jne         00000000650703C2
  000000006507039F: 48 3B C3           cmp         rax,rbx
  00000000650703A2: 74 10              je          00000000650703B4
  00000000650703A4: 48 8B C8           mov         rcx,rax
  00000000650703A7: FF 15 BB 64 62 00  call        qword ptr [0000000065696868h]
  00000000650703AD: 48 85 C0           test        rax,rax
  00000000650703B0: 75 E0              jne         0000000065070392
  00000000650703B2: EB 0E              jmp         00000000650703C2
  00000000650703B4: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650703B7: B2 01              mov         dl,1
  00000000650703B9: 48 8B CB           mov         rcx,rbx
  00000000650703BC: FF 90 58 01 00 00  call        qword ptr [rax+0000000000000158h]
  00000000650703C2: 49 8B CC           mov         rcx,r12
  00000000650703C5: E8 26 F5 F9 FF     call        000000006500F8F0
  00000000650703CA: 48 85 C0           test        rax,rax
  00000000650703CD: 74 0B              je          00000000650703DA
  00000000650703CF: 48 8B D3           mov         rdx,rbx
  00000000650703D2: 48 8B C8           mov         rcx,rax
  00000000650703D5: E8 46 06 18 00     call        00000000651F0A20
  00000000650703DA: 40 84 ED           test        bpl,bpl
  00000000650703DD: 74 11              je          00000000650703F0
  00000000650703DF: 33 D2              xor         edx,edx
  00000000650703E1: 41 B8 03 80 00 00  mov         r8d,8003h
  00000000650703E7: 48 8B CB           mov         rcx,rbx
  00000000650703EA: E8 71 25 43 00     call        00000000654A2960
  00000000650703EF: 90                 nop
  00000000650703F0: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000650703F5: E8 66 22 FC FF     call        0000000065032660
  00000000650703FA: 4C 8D 5C 24 60     lea         r11,[rsp+60h]
  00000000650703FF: 49 8B 5B 30        mov         rbx,qword ptr [r11+30h]
  0000000065070403: 49 8B 6B 38        mov         rbp,qword ptr [r11+38h]
  0000000065070407: 49 8B E3           mov         rsp,r11
  000000006507040A: 41 5C              pop         r12
  000000006507040C: 5F                 pop         rdi
  000000006507040D: 5E                 pop         rsi
  000000006507040E: C3                 ret

  Summary

      695000 .text
