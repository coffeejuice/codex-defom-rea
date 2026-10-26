Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414034F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001414034F5: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  00000001414034FC: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141403504: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  0000000141403509: 74 57              je          0000000141403562
  000000014140350B: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141403513: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000141403517: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014140351C: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000141403521: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000141403526: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014140352C: 74 1B              je          0000000141403549
  000000014140352E: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141403533: 48 8B 00           mov         rax,qword ptr [rax]
  0000000141403536: BA 01 00 00 00     mov         edx,1
  000000014140353B: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000141403540: FF 10              call        qword ptr [rax]
  0000000141403542: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000141403547: EB 09              jmp         0000000141403552
  0000000141403549: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000141403552: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014140355A: 48 C7 40 10 00 00  mov         qword ptr [rax+10h],0
                    00 00
  0000000141403562: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014140356A: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  000000014140356F: 74 58              je          00000001414035C9
  0000000141403571: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141403579: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  000000014140357D: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000141403582: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000141403587: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014140358C: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000141403592: 74 1C              je          00000001414035B0
  0000000141403594: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000141403599: 48 8B 00           mov         rax,qword ptr [rax]
  000000014140359C: BA 01 00 00 00     mov         edx,1
  00000001414035A1: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001414035A6: FF 50 18           call        qword ptr [rax+18h]
  00000001414035A9: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001414035AE: EB 09              jmp         00000001414035B9
  00000001414035B0: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  00000001414035B9: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001414035C1: 48 C7 40 18 00 00  mov         qword ptr [rax+18h],0
                    00 00
  00000001414035C9: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001414035D1: 48 83 78 38 00     cmp         qword ptr [rax+38h],0
  00000001414035D6: 74 58              je          0000000141403630
  00000001414035D8: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001414035E0: 48 8B 40 38        mov         rax,qword ptr [rax+38h]
  00000001414035E4: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001414035E9: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001414035EE: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001414035F3: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001414035F9: 74 1C              je          0000000141403617
  00000001414035FB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000141403600: 48 8B 00           mov         rax,qword ptr [rax]
  0000000141403603: BA 01 00 00 00     mov         edx,1
  0000000141403608: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014140360D: FF 50 18           call        qword ptr [rax+18h]
  0000000141403610: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000141403615: EB 09              jmp         0000000141403620
  0000000141403617: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  0000000141403620: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141403628: 48 C7 40 38 00 00  mov         qword ptr [rax+38h],0
                    00 00
  0000000141403630: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141403638: 48 81 C1 80 00 00  add         rcx,80h
                    00
  000000014140363F: E8 CC 3D F0 FE     call        0000000140307410
  0000000141403644: 0F B6 C0           movzx       eax,al
  0000000141403647: 85 C0              test        eax,eax
  0000000141403649: 75 14              jne         000000014140365F
  000000014140364B: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141403653: 48 81 C1 80 00 00  add         rcx,80h
                    00
  000000014140365A: E8 31 A5 E0 FE     call        000000014020DB90
  000000014140365F: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141403667: 48 83 B8 A0 00 00  cmp         qword ptr [rax+00000000000000A0h],0
                    00 00
  000000014140366F: 74 4B              je          00000001414036BC
  0000000141403671: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141403679: 48 8B 80 A0 00 00  mov         rax,qword ptr [rax+00000000000000A0h]
                    00
  0000000141403680: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000141403685: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014140368A: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014140368F: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  0000000141403695: 74 1C              je          00000001414036B3
  0000000141403697: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014140369C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014140369F: BA 01 00 00 00     mov         edx,1
  00000001414036A4: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001414036A9: FF 50 18           call        qword ptr [rax+18h]
  00000001414036AC: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001414036B1: EB 09              jmp         00000001414036BC
  00000001414036B3: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  00000001414036BC: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001414036C4: E8 B7 90 00 00     call        000000014140C780
  00000001414036C9: 48 81 C4 88 00 00  add         rsp,88h
                    00
  00000001414036D0: C3                 ret

  Summary

     1E02000 .text
