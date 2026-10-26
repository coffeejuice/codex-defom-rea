Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065477580: 4C 8B DC           mov         r11,rsp
  0000000065477583: 41 54              push        r12
  0000000065477585: 41 55              push        r13
  0000000065477587: 41 56              push        r14
  0000000065477589: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  0000000065477590: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065477599: 49 89 5B 08        mov         qword ptr [r11+8],rbx
  000000006547759D: 49 89 6B 10        mov         qword ptr [r11+10h],rbp
  00000000654775A1: 49 89 73 18        mov         qword ptr [r11+18h],rsi
  00000000654775A5: 49 89 7B 20        mov         qword ptr [r11+20h],rdi
  00000000654775A9: 4C 8B AC 24 C0 00  mov         r13,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000000654775B1: 44 8B A4 24 C8 00  mov         r12d,dword ptr [rsp+00000000000000C8h]
                    00 00
  00000000654775B9: 45 85 E4           test        r12d,r12d
  00000000654775BC: 74 22              je          00000000654775E0
  00000000654775BE: 45 8B 55 00        mov         r10d,dword ptr [r13]
  00000000654775C2: 41 8B C2           mov         eax,r10d
  00000000654775C5: 41 23 C4           and         eax,r12d
  00000000654775C8: 75 16              jne         00000000654775E0
  00000000654775CA: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  00000000654775CE: 45 89 63 90        mov         dword ptr [r11-70h],r12d
  00000000654775D2: 45 89 53 88        mov         dword ptr [r11-78h],r10d
  00000000654775D6: E8 A5 F9 FF FF     call        0000000065476F80
  00000000654775DB: E9 53 01 00 00     jmp         0000000065477733
  00000000654775E0: C7 84 24 C8 00 00  mov         dword ptr [rsp+00000000000000C8h],103h
                    00 03 01 00 00
  00000000654775EB: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  00000000654775F3: 48 8D 84 24 C8 00  lea         rax,[rsp+00000000000000C8h]
                    00 00
  00000000654775FB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000065477600: 48 89 4C 24 28     mov         qword ptr [rsp+28h],rcx
  0000000065477605: 48 8D 44 24 40     lea         rax,[rsp+40h]
  000000006547760A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006547760F: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065477614: E8 27 F7 FF FF     call        0000000065476D40
  0000000065477619: 90                 nop
  000000006547761A: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006547761F: FF 15 43 F1 21 00  call        qword ptr [0000000065696768h]
  0000000065477625: 90                 nop
  0000000065477626: 4C 8D 05 73 54 28  lea         r8,[00000000656FCAA0h]
                    00
  000000006547762D: 48 8B D0           mov         rdx,rax
  0000000065477630: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065477635: FF 15 4D 12 22 00  call        qword ptr [0000000065698888h]
  000000006547763B: 4C 8B F0           mov         r14,rax
  000000006547763E: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065477643: FF 15 17 F1 21 00  call        qword ptr [0000000065696760h]
  0000000065477649: BF 00 04 00 00     mov         edi,400h
  000000006547764E: 66 90              nop
  0000000065477650: 41 8B 5D 00        mov         ebx,dword ptr [r13]
  0000000065477654: 23 DF              and         ebx,edi
  0000000065477656: 03 FF              add         edi,edi
  0000000065477658: 85 DB              test        ebx,ebx
  000000006547765A: 74 59              je          00000000654776B5
  000000006547765C: 48 8B 6C 24 58     mov         rbp,qword ptr [rsp+58h]
  0000000065477661: 8B D3              mov         edx,ebx
  0000000065477663: 48 8B 8D 58 02 00  mov         rcx,qword ptr [rbp+0000000000000258h]
                    00
  000000006547766A: E8 31 5A F2 FF     call        000000006539D0A0
  000000006547766F: 48 8B F0           mov         rsi,rax
  0000000065477672: 48 85 C0           test        rax,rax
  0000000065477675: 74 07              je          000000006547767E
  0000000065477677: C6 85 91 02 00 00  mov         byte ptr [rbp+0000000000000291h],0
                    00
  000000006547767E: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000065477683: 48 83 B9 70 02 00  cmp         qword ptr [rcx+0000000000000270h],0
                    00 00
  000000006547768B: 75 28              jne         00000000654776B5
  000000006547768D: 45 85 E4           test        r12d,r12d
  0000000065477690: 75 11              jne         00000000654776A3
  0000000065477692: 48 8B D0           mov         rdx,rax
  0000000065477695: 49 8B CE           mov         rcx,r14
  0000000065477698: E8 C3 3B F2 FF     call        000000006539B260
  000000006547769D: 85 C0              test        eax,eax
  000000006547769F: 74 07              je          00000000654776A8
  00000000654776A1: EB 12              jmp         00000000654776B5
  00000000654776A3: 41 3B DC           cmp         ebx,r12d
  00000000654776A6: 75 0D              jne         00000000654776B5
  00000000654776A8: 48 8B D6           mov         rdx,rsi
  00000000654776AB: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000654776B0: E8 EB E9 FF FF     call        00000000654760A0
  00000000654776B5: 81 FF 00 00 00 08  cmp         edi,8000000h
  00000000654776BB: 76 93              jbe         0000000065477650
  00000000654776BD: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000654776C2: E8 59 1D FE FF     call        0000000065459420
  00000000654776C7: 83 F8 FF           cmp         eax,0FFFFFFFFh
  00000000654776CA: 75 29              jne         00000000654776F5
  00000000654776CC: 48 8D 05 E5 97 2A  lea         rax,[0000000065720EB8h]
                    00
  00000000654776D3: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000000654776D8: 48 8D 05 A1 97 2A  lea         rax,[0000000065720E80h]
                    00
  00000000654776DF: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000000654776E4: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000654776E9: E8 E2 19 FE FF     call        00000000654590D0
  00000000654776EE: B8 00 00 40 00     mov         eax,400000h
  00000000654776F3: EB 3E              jmp         0000000065477733
  00000000654776F5: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000000654776FA: 48 8B 91 78 02 00  mov         rdx,qword ptr [rcx+0000000000000278h]
                    00
  0000000065477701: 48 8B 89 58 02 00  mov         rcx,qword ptr [rcx+0000000000000258h]
                    00
  0000000065477708: E8 93 41 F2 FF     call        000000006539B8A0
  000000006547770D: 8B D8              mov         ebx,eax
  000000006547770F: 48 8D 05 A2 97 2A  lea         rax,[0000000065720EB8h]
                    00
  0000000065477716: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000006547771B: 48 8D 05 5E 97 2A  lea         rax,[0000000065720E80h]
                    00
  0000000065477722: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000065477727: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006547772C: E8 9F 19 FE FF     call        00000000654590D0
  0000000065477731: 8B C3              mov         eax,ebx
  0000000065477733: 4C 8D 9C 24 80 00  lea         r11,[rsp+0000000000000080h]
                    00 00
  000000006547773B: 49 8B 5B 20        mov         rbx,qword ptr [r11+20h]
  000000006547773F: 49 8B 6B 28        mov         rbp,qword ptr [r11+28h]
  0000000065477743: 49 8B 73 30        mov         rsi,qword ptr [r11+30h]
  0000000065477747: 49 8B 7B 38        mov         rdi,qword ptr [r11+38h]
  000000006547774B: 49 8B E3           mov         rsp,r11
  000000006547774E: 41 5E              pop         r14
  0000000065477750: 41 5D              pop         r13
  0000000065477752: 41 5C              pop         r12
  0000000065477754: C3                 ret

  Summary

      695000 .text
