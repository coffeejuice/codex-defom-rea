Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140038380: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140038384: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140038389: 48 81 EC C8 00 00  sub         rsp,0C8h
                    00
  0000000140038390: 48 C7 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014003839C: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400383A4: E8 F7 AF FD FF     call        00000001400133A0
  00000001400383A9: 48 8B C8           mov         rcx,rax
  00000001400383AC: E8 CF D3 00 00     call        0000000140045780
  00000001400383B1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400383B6: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001400383BC: 74 11              je          00000001400383CF
  00000001400383BE: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001400383C3: E8 A8 72 00 00     call        000000014003F670
  00000001400383C8: 0F B6 C0           movzx       eax,al
  00000001400383CB: 85 C0              test        eax,eax
  00000001400383CD: 74 05              je          00000001400383D4
  00000001400383CF: E9 57 02 00 00     jmp         000000014003862B
  00000001400383D4: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400383DC: E8 BF AB FD FF     call        0000000140012FA0
  00000001400383E1: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400383E6: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001400383EB: E8 A0 EF 06 00     call        00000001400A7390
  00000001400383F0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400383F5: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001400383FB: 0F 84 2A 02 00 00  je          000000014003862B
  0000000140038401: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140038407: 0F 84 1E 02 00 00  je          000000014003862B
  000000014003840D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140038412: E8 59 D6 07 00     call        00000001400B5A70
  0000000140038417: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014003841C: 0F B6 84 24 D8 00  movzx       eax,byte ptr [rsp+00000000000000D8h]
                    00 00
  0000000140038424: 85 C0              test        eax,eax
  0000000140038426: 0F 84 BD 01 00 00  je          00000001400385E9
  000000014003842C: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140038431: 48 83 C1 10        add         rcx,10h
  0000000140038435: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014003843A: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  000000014003843E: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140038443: FF 90 60 01 00 00  call        qword ptr [rax+0000000000000160h]
  0000000140038449: 90                 nop
  000000014003844A: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014003844F: E8 8C C6 FC FF     call        0000000140004AE0
  0000000140038454: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  0000000140038458: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000140038460: EB 0B              jmp         000000014003846D
  0000000140038462: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140038466: 83 C0 01           add         eax,1
  0000000140038469: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000014003846D: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  0000000140038471: 39 44 24 40        cmp         dword ptr [rsp+40h],eax
  0000000140038475: 0F 8D 64 01 00 00  jge         00000001400385DF
  000000014003847B: 8B 54 24 40        mov         edx,dword ptr [rsp+40h]
  000000014003847F: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140038484: E8 87 87 12 00     call        0000000140160C10
  0000000140038489: 48 8B 00           mov         rax,qword ptr [rax]
  000000014003848C: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140038491: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  0000000140038497: 0F 84 3D 01 00 00  je          00000001400385DA
  000000014003849D: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400384A2: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400384A5: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400384AA: FF 50 28           call        qword ptr [rax+28h]
  00000001400384AD: 89 44 24 70        mov         dword ptr [rsp+70h],eax
  00000001400384B1: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400384B6: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400384B9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400384BE: FF 50 08           call        qword ptr [rax+8]
  00000001400384C1: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001400384C6: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  00000001400384CB: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400384CE: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000001400384D3: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400384D8: FF 50 38           call        qword ptr [rax+38h]
  00000001400384DB: 90                 nop
  00000001400384DC: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400384E4: E8 E7 52 FE FF     call        000000014001D7D0
  00000001400384E9: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001400384EE: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  00000001400384F3: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001400384F8: E8 C3 F8 00 00     call        0000000140047DC0
  00000001400384FD: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140038502: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  0000000140038508: 0F 84 C1 00 00 00  je          00000001400385CF
  000000014003850E: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000140038516: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014003851B: E8 E0 87 01 00     call        0000000140050D00
  0000000140038520: 90                 nop
  0000000140038521: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000140038526: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014003852E: FF 15 9C BB DC 01  call        qword ptr [0000000141E040D0h]
  0000000140038534: 0F B6 C0           movzx       eax,al
  0000000140038537: 85 C0              test        eax,eax
  0000000140038539: 0F 84 81 00 00 00  je          00000001400385C0
  000000014003853F: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140038547: E8 54 B0 FD FF     call        00000001400135A0
  000000014003854C: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140038554: 48 8D 84 24 98 00  lea         rax,[rsp+0000000000000098h]
                    00 00
  000000014003855C: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140038564: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000014003856C: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140038574: FF 15 5E D4 DC 01  call        qword ptr [0000000141E059D8h]
  000000014003857A: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140038582: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014003858A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014003858D: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140038595: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014003859D: FF 50 08           call        qword ptr [rax+8]
  00000001400385A0: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001400385A8: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400385AD: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400385B0: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001400385B8: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400385BD: FF 10              call        qword ptr [rax]
  00000001400385BF: 90                 nop
  00000001400385C0: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000001400385C8: FF 15 12 D4 DC 01  call        qword ptr [0000000141E059E0h]
  00000001400385CE: 90                 nop
  00000001400385CF: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400385D4: FF 15 06 D4 DC 01  call        qword ptr [0000000141E059E0h]
  00000001400385DA: E9 83 FE FF FF     jmp         0000000140038462
  00000001400385DF: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001400385E4: E8 B7 FB 1C 00     call        00000001402081A0
  00000001400385E9: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001400385EF: 74 13              je          0000000140038604
  00000001400385F1: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001400385F6: 48 83 C0 10        add         rax,10h
  00000001400385FA: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140038602: EB 0C              jmp         0000000140038610
  0000000140038604: 48 C7 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],0
                    00 00 00 00 00 00
  0000000140038610: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140038615: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140038618: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140038620: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140038625: FF 90 B8 01 00 00  call        qword ptr [rax+00000000000001B8h]
  000000014003862B: 48 81 C4 C8 00 00  add         rsp,0C8h
                    00
  0000000140038632: C3                 ret

  Summary

     1E02000 .text
