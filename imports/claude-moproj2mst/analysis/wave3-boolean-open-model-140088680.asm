Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140088680: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140088685: 48 81 EC A8 00 00  sub         rsp,0A8h
                    00
  000000014008868C: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140088695: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014008869D: 48 83 78 08 00     cmp         qword ptr [rax+8],0
  00000001400886A2: 75 05              jne         00000001400886A9
  00000001400886A4: E9 21 02 00 00     jmp         00000001400888CA
  00000001400886A9: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400886B1: 48 83 E9 10        sub         rcx,10h
  00000001400886B5: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001400886BA: E8 11 C4 FF FF     call        0000000140084AD0
  00000001400886BF: 90                 nop
  00000001400886C0: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001400886C5: E8 46 ED 27 00     call        0000000140307410
  00000001400886CA: 0F B6 C0           movzx       eax,al
  00000001400886CD: 85 C0              test        eax,eax
  00000001400886CF: 0F 85 EB 01 00 00  jne         00000001400888C0
  00000001400886D5: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001400886DA: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001400886DF: E8 0C 54 FE FF     call        000000014006DAF0
  00000001400886E4: 90                 nop
  00000001400886E5: 48 8D 44 24 30     lea         rax,[rsp+30h]
  00000001400886EA: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400886EF: EB 3C              jmp         000000014008872D
  00000001400886F1: 33 C0              xor         eax,eax
  00000001400886F3: 83 F8 01           cmp         eax,1
  00000001400886F6: 74 0B              je          0000000140088703
  00000001400886F8: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  0000000140088701: EB 0F              jmp         0000000140088712
  0000000140088703: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140088708: E8 43 C5 0F 00     call        0000000140184C50
  000000014008870D: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140088712: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  0000000140088717: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014008871C: E8 5F 3C D7 00     call        0000000140DFC380
  0000000140088721: 48 8B C8           mov         rcx,rax
  0000000140088724: 48 83 C1 10        add         rcx,10h
  0000000140088728: E8 D3 10 D6 00     call        0000000140DE9800
  000000014008872D: 33 C0              xor         eax,eax
  000000014008872F: 83 F8 01           cmp         eax,1
  0000000140088732: 74 0B              je          000000014008873F
  0000000140088734: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  000000014008873D: EB 0F              jmp         000000014008874E
  000000014008873F: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140088744: E8 07 C5 0F 00     call        0000000140184C50
  0000000140088749: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000014008874E: 48 8B 54 24 70     mov         rdx,qword ptr [rsp+70h]
  0000000140088753: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140088758: E8 23 3C D7 00     call        0000000140DFC380
  000000014008875D: 48 8B C8           mov         rcx,rax
  0000000140088760: E8 1B 39 D7 00     call        0000000140DFC080
  0000000140088765: 0F B6 C0           movzx       eax,al
  0000000140088768: 85 C0              test        eax,eax
  000000014008876A: 0F 84 45 01 00 00  je          00000001400888B5
  0000000140088770: 33 C0              xor         eax,eax
  0000000140088772: 83 F8 01           cmp         eax,1
  0000000140088775: 74 0B              je          0000000140088782
  0000000140088777: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  0000000140088780: EB 0F              jmp         0000000140088791
  0000000140088782: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140088787: E8 C4 C4 0F 00     call        0000000140184C50
  000000014008878C: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140088791: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140088796: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014008879B: E8 E0 3B D7 00     call        0000000140DFC380
  00000001400887A0: 48 8B C8           mov         rcx,rax
  00000001400887A3: 48 83 C1 10        add         rcx,10h
  00000001400887A7: E8 54 6C 5C 00     call        000000014064F400
  00000001400887AC: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400887AF: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400887B4: EB 5C              jmp         0000000140088812
  00000001400887B6: 33 C0              xor         eax,eax
  00000001400887B8: 83 F8 01           cmp         eax,1
  00000001400887BB: 74 0E              je          00000001400887CB
  00000001400887BD: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00 00
  00000001400887C9: EB 12              jmp         00000001400887DD
  00000001400887CB: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001400887D0: E8 7B C4 0F 00     call        0000000140184C50
  00000001400887D5: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400887DD: 48 8B 94 24 80 00  mov         rdx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400887E5: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001400887EA: E8 91 3B D7 00     call        0000000140DFC380
  00000001400887EF: 48 83 C0 08        add         rax,8
  00000001400887F3: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001400887FB: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140088803: 8B 08              mov         ecx,dword ptr [rax]
  0000000140088805: 83 E9 01           sub         ecx,1
  0000000140088808: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140088810: 89 08              mov         dword ptr [rax],ecx
  0000000140088812: 33 C0              xor         eax,eax
  0000000140088814: 83 F8 01           cmp         eax,1
  0000000140088817: 74 0E              je          0000000140088827
  0000000140088819: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00 00
  0000000140088825: EB 12              jmp         0000000140088839
  0000000140088827: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014008882C: E8 1F C4 0F 00     call        0000000140184C50
  0000000140088831: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140088839: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140088841: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140088846: E8 35 3B D7 00     call        0000000140DFC380
  000000014008884B: 83 78 08 00        cmp         dword ptr [rax+8],0
  000000014008884F: 74 5F              je          00000001400888B0
  0000000140088851: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140088859: 48 83 E9 10        sub         rcx,10h
  000000014008885D: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140088862: E8 59 E9 FF FF     call        00000001400871C0
  0000000140088867: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014008886C: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  0000000140088872: 74 37              je          00000001400888AB
  0000000140088874: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140088879: E8 C2 BE FD FF     call        0000000140064740
  000000014008887E: 0F B6 C0           movzx       eax,al
  0000000140088881: 85 C0              test        eax,eax
  0000000140088883: 74 26              je          00000001400888AB
  0000000140088885: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014008888A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014008888D: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140088892: FF 50 28           call        qword ptr [rax+28h]
  0000000140088895: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014008889D: 48 83 E9 10        sub         rcx,10h
  00000001400888A1: 41 B0 01           mov         r8b,1
  00000001400888A4: 8B D0              mov         edx,eax
  00000001400888A6: E8 35 E7 FF FF     call        0000000140086FE0
  00000001400888AB: E9 06 FF FF FF     jmp         00000001400887B6
  00000001400888B0: E9 3C FE FF FF     jmp         00000001400886F1
  00000001400888B5: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001400888BA: E8 61 8C BC 00     call        0000000140C51520
  00000001400888BF: 90                 nop
  00000001400888C0: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001400888C5: E8 D6 F8 17 00     call        00000001402081A0
  00000001400888CA: 48 81 C4 A8 00 00  add         rsp,0A8h
                    00
  00000001400888D1: C3                 ret

  Summary

     1E02000 .text
