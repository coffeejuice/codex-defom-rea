Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654B0790: 48 8B C4           mov         rax,rsp
  00000000654B0793: 41 54              push        r12
  00000000654B0795: 48 83 EC 60        sub         rsp,60h
  00000000654B0799: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654B07A2: 48 89 58 08        mov         qword ptr [rax+8],rbx
  00000000654B07A6: 48 89 68 10        mov         qword ptr [rax+10h],rbp
  00000000654B07AA: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  00000000654B07AE: 48 89 78 20        mov         qword ptr [rax+20h],rdi
  00000000654B07B2: 48 8B E9           mov         rbp,rcx
  00000000654B07B5: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  00000000654B07B9: 48 8D 8E 94 03 00  lea         rcx,[rsi+0000000000000394h]
                    00
  00000000654B07C0: FF 15 82 65 1E 00  call        qword ptr [0000000065696D48h]
  00000000654B07C6: 48 8D 96 C0 02 00  lea         rdx,[rsi+00000000000002C0h]
                    00
  00000000654B07CD: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000654B07D2: E8 99 FF FF FF     call        00000000654B0770
  00000000654B07D7: 90                 nop
  00000000654B07D8: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000654B07DD: 0F 1F 00           nop         dword ptr [rax]
  00000000654B07E0: 8B 4C 24 48        mov         ecx,dword ptr [rsp+48h]
  00000000654B07E4: 8B C1              mov         eax,ecx
  00000000654B07E6: FF C1              inc         ecx
  00000000654B07E8: 89 4C 24 48        mov         dword ptr [rsp+48h],ecx
  00000000654B07EC: 85 C0              test        eax,eax
  00000000654B07EE: 0F 85 C7 00 00 00  jne         00000000654B08BB
  00000000654B07F4: 48 3B 5C 24 58     cmp         rbx,qword ptr [rsp+58h]
  00000000654B07F9: 0F 84 BC 00 00 00  je          00000000654B08BB
  00000000654B07FF: 90                 nop
  00000000654B0800: 85 C9              test        ecx,ecx
  00000000654B0802: 0F 84 9B 00 00 00  je          00000000654B08A3
  00000000654B0808: 48 8B 43 18        mov         rax,qword ptr [rbx+18h]
  00000000654B080C: 48 85 C0           test        rax,rax
  00000000654B080F: 0F 84 83 00 00 00  je          00000000654B0898
  00000000654B0815: 48 8D 48 0C        lea         rcx,[rax+0Ch]
  00000000654B0819: 33 D2              xor         edx,edx
  00000000654B081B: FF 15 2F 63 1E 00  call        qword ptr [0000000065696B50h]
  00000000654B0821: 84 C0              test        al,al
  00000000654B0823: 75 6F              jne         00000000654B0894
  00000000654B0825: 48 83 7B 20 00     cmp         qword ptr [rbx+20h],0
  00000000654B082A: 74 68              je          00000000654B0894
  00000000654B082C: 48 8B 4B 18        mov         rcx,qword ptr [rbx+18h]
  00000000654B0830: 48 85 C9           test        rcx,rcx
  00000000654B0833: 74 5F              je          00000000654B0894
  00000000654B0835: 48 83 C1 0C        add         rcx,0Ch
  00000000654B0839: 33 D2              xor         edx,edx
  00000000654B083B: FF 15 0F 63 1E 00  call        qword ptr [0000000065696B50h]
  00000000654B0841: 84 C0              test        al,al
  00000000654B0843: 75 4F              jne         00000000654B0894
  00000000654B0845: 48 8B 7B 20        mov         rdi,qword ptr [rbx+20h]
  00000000654B0849: 48 85 FF           test        rdi,rdi
  00000000654B084C: 74 46              je          00000000654B0894
  00000000654B084E: 4C 8D 0D 53 A0 27  lea         r9,[000000006572A8A8h]
                    00
  00000000654B0855: 4C 8B 46 08        mov         r8,qword ptr [rsi+8]
  00000000654B0859: 48 8D 15 08 B7 1F  lea         rdx,[00000000656ABF68h]
                    00
  00000000654B0860: 48 8B CF           mov         rcx,rdi
  00000000654B0863: FF 15 B7 5E 1E 00  call        qword ptr [0000000065696720h]
  00000000654B0869: 48 8B 96 88 02 00  mov         rdx,qword ptr [rsi+0000000000000288h]
                    00
  00000000654B0870: 48 8B CF           mov         rcx,rdi
  00000000654B0873: FF 15 6F 6E 1E 00  call        qword ptr [00000000656976E8h]
  00000000654B0879: 4C 8B 1F           mov         r11,qword ptr [rdi]
  00000000654B087C: 33 D2              xor         edx,edx
  00000000654B087E: 48 8B CF           mov         rcx,rdi
  00000000654B0881: 41 FF 53 58        call        qword ptr [r11+58h]
  00000000654B0885: 48 8B CF           mov         rcx,rdi
  00000000654B0888: FF 15 8A 69 1E 00  call        qword ptr [0000000065697218h]
  00000000654B088E: 8B 4C 24 48        mov         ecx,dword ptr [rsp+48h]
  00000000654B0892: EB 04              jmp         00000000654B0898
  00000000654B0894: 8B 4C 24 48        mov         ecx,dword ptr [rsp+48h]
  00000000654B0898: FF C9              dec         ecx
  00000000654B089A: 89 4C 24 48        mov         dword ptr [rsp+48h],ecx
  00000000654B089E: E9 5D FF FF FF     jmp         00000000654B0800
  00000000654B08A3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000654B08A8: FF 15 9A 62 1E 00  call        qword ptr [0000000065696B48h]
  00000000654B08AE: 48 8B D8           mov         rbx,rax
  00000000654B08B1: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000000654B08B6: E9 25 FF FF FF     jmp         00000000654B07E0
  00000000654B08BB: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000654B08C0: 48 83 C1 10        add         rcx,10h
  00000000654B08C4: FF 15 AE 60 1E 00  call        qword ptr [0000000065696978h]
  00000000654B08CA: 84 C0              test        al,al
  00000000654B08CC: 75 12              jne         00000000654B08E0
  00000000654B08CE: 48 8D 15 BB 94 FF  lea         rdx,[00000000654A9D90h]
                    FF
  00000000654B08D5: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000654B08DA: FF 15 D0 62 1E 00  call        qword ptr [0000000065696BB0h]
  00000000654B08E0: 48 8D 8E B8 02 00  lea         rcx,[rsi+00000000000002B8h]
                    00
  00000000654B08E7: E8 D4 D4 FF FF     call        00000000654ADDC0
  00000000654B08EC: 48 8D 8E C0 02 00  lea         rcx,[rsi+00000000000002C0h]
                    00
  00000000654B08F3: E8 98 E7 FF FF     call        00000000654AF090
  00000000654B08F8: 48 8D 8E C8 02 00  lea         rcx,[rsi+00000000000002C8h]
                    00
  00000000654B08FF: E8 CC 59 0B 00     call        00000000655662D0
  00000000654B0904: C6 86 A0 03 00 00  mov         byte ptr [rsi+00000000000003A0h],0
                    00
  00000000654B090B: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  00000000654B090F: 33 C9              xor         ecx,ecx
  00000000654B0911: 89 88 F8 02 00 00  mov         dword ptr [rax+00000000000002F8h],ecx
  00000000654B0917: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  00000000654B091F: C7 44 24 2C FF FF  mov         dword ptr [rsp+2Ch],0FFFFFFFFh
                    FF FF
  00000000654B0927: 48 89 4C 24 30     mov         qword ptr [rsp+30h],rcx
  00000000654B092C: 48 89 4C 24 38     mov         qword ptr [rsp+38h],rcx
  00000000654B0931: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000000654B0935: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000654B093A: 48 8B CD           mov         rcx,rbp
  00000000654B093D: FF 90 E8 01 00 00  call        qword ptr [rax+00000000000001E8h]
  00000000654B0943: 90                 nop
  00000000654B0944: 48 8B 8E A0 02 00  mov         rcx,qword ptr [rsi+00000000000002A0h]
                    00
  00000000654B094B: 48 85 C9           test        rcx,rcx
  00000000654B094E: 74 06              je          00000000654B0956
  00000000654B0950: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000654B0953: FF 50 70           call        qword ptr [rax+70h]
  00000000654B0956: 4C 8D 5C 24 60     lea         r11,[rsp+60h]
  00000000654B095B: 49 8B 5B 10        mov         rbx,qword ptr [r11+10h]
  00000000654B095F: 49 8B 6B 18        mov         rbp,qword ptr [r11+18h]
  00000000654B0963: 49 8B 73 20        mov         rsi,qword ptr [r11+20h]
  00000000654B0967: 49 8B 7B 28        mov         rdi,qword ptr [r11+28h]
  00000000654B096B: 49 8B E3           mov         rsp,r11
  00000000654B096E: 41 5C              pop         r12
  00000000654B0970: C3                 ret

  Summary

      695000 .text
