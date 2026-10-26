Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  000000006100F420: 48 8B C4           mov         rax,rsp
  000000006100F423: 57                 push        rdi
  000000006100F424: 48 83 EC 60        sub         rsp,60h
  000000006100F428: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006100F431: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006100F435: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  000000006100F439: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  000000006100F43D: 49 8B F9           mov         rdi,r9
  000000006100F440: 41 0F B6 F0        movzx       esi,r8b
  000000006100F444: 48 8B EA           mov         rbp,rdx
  000000006100F447: 48 8B D9           mov         rbx,rcx
  000000006100F44A: 48 83 39 00        cmp         qword ptr [rcx],0
  000000006100F44E: 75 23              jne         000000006100F473
  000000006100F450: B9 80 00 00 00     mov         ecx,80h
  000000006100F455: E8 E0 73 02 00     call        000000006103683A
  000000006100F45A: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000006100F45F: 48 85 C0           test        rax,rax
  000000006100F462: 74 0A              je          000000006100F46E
  000000006100F464: 48 8B C8           mov         rcx,rax
  000000006100F467: E8 84 E7 FF FF     call        000000006100DBF0
  000000006100F46C: EB 02              jmp         000000006100F470
  000000006100F46E: 33 C0              xor         eax,eax
  000000006100F470: 48 89 03           mov         qword ptr [rbx],rax
  000000006100F473: 48 8B D5           mov         rdx,rbp
  000000006100F476: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006100F47B: E8 A0 21 00 00     call        0000000061011620
  000000006100F480: 90                 nop
  000000006100F481: 48 8B 1B           mov         rbx,qword ptr [rbx]
  000000006100F484: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006100F489: E8 62 C9 00 00     call        000000006101BDF0
  000000006100F48E: 90                 nop
  000000006100F48F: 40 0F B6 D6        movzx       edx,sil
  000000006100F493: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006100F498: E8 D3 41 FF FF     call        0000000061003670
  000000006100F49D: 4C 8B 9C 24 98 00  mov         r11,qword ptr [rsp+0000000000000098h]
                    00 00
  000000006100F4A5: 4C 89 5C 24 28     mov         qword ptr [rsp+28h],r11
  000000006100F4AA: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  000000006100F4B2: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006100F4B7: 4C 8B CF           mov         r9,rdi
  000000006100F4BA: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  000000006100F4BF: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006100F4C4: 48 8B CB           mov         rcx,rbx
  000000006100F4C7: E8 94 EA FF FF     call        000000006100DF60
  000000006100F4CC: 0F B6 D8           movzx       ebx,al
  000000006100F4CF: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006100F4D4: E8 67 C9 00 00     call        000000006101BE40
  000000006100F4D9: 90                 nop
  000000006100F4DA: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006100F4DF: E8 3C 12 00 00     call        0000000061010720
  000000006100F4E4: 8A C3              mov         al,bl
  000000006100F4E6: 4C 8D 5C 24 60     lea         r11,[rsp+60h]
  000000006100F4EB: 49 8B 5B 18        mov         rbx,qword ptr [r11+18h]
  000000006100F4EF: 49 8B 6B 20        mov         rbp,qword ptr [r11+20h]
  000000006100F4F3: 49 8B 73 28        mov         rsi,qword ptr [r11+28h]
  000000006100F4F7: 49 8B E3           mov         rsp,r11
  000000006100F4FA: 5F                 pop         rdi
  000000006100F4FB: C3                 ret

  Summary

       42000 .text
