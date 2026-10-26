Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140322420: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140322425: 48 83 EC 58        sub         rsp,58h
  0000000140322429: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014032242E: E8 8D F1 FF FF     call        00000001403215C0
  0000000140322433: 0F B6 C0           movzx       eax,al
  0000000140322436: 85 C0              test        eax,eax
  0000000140322438: 0F 84 A8 00 00 00  je          00000001403224E6
  000000014032243E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140322443: E8 18 F2 FF FF     call        0000000140321660
  0000000140322448: 0F B6 C0           movzx       eax,al
  000000014032244B: 85 C0              test        eax,eax
  000000014032244D: 0F 84 93 00 00 00  je          00000001403224E6
  0000000140322453: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140322458: E8 53 7E FE FF     call        000000014030A2B0
  000000014032245D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140322462: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140322468: 75 02              jne         000000014032246C
  000000014032246A: EB 7A              jmp         00000001403224E6
  000000014032246C: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140322471: E8 3A E1 96 00     call        0000000140C905B0
  0000000140322476: 48 8B C8           mov         rcx,rax
  0000000140322479: E8 E2 42 D4 FF     call        0000000140066760
  000000014032247E: 85 C0              test        eax,eax
  0000000140322480: 75 02              jne         0000000140322484
  0000000140322482: EB 62              jmp         00000001403224E6
  0000000140322484: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140322489: E8 02 89 FE FF     call        000000014030AD90
  000000014032248E: 0F B6 C0           movzx       eax,al
  0000000140322491: 85 C0              test        eax,eax
  0000000140322493: 75 02              jne         0000000140322497
  0000000140322495: EB 4F              jmp         00000001403224E6
  0000000140322497: 48 8D 44 24 30     lea         rax,[rsp+30h]
  000000014032249C: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001403224A1: 48 8D 15 88 AB FE  lea         rdx,[000000014230D030h]
                    01
  00000001403224A8: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001403224AD: FF 15 CD 1F AE 01  call        qword ptr [0000000141E04480h]
  00000001403224B3: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001403224B8: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  00000001403224BD: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001403224C2: E8 A9 FC FF FF     call        0000000140322170
  00000001403224C7: 33 D2              xor         edx,edx
  00000001403224C9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001403224CE: E8 3D F6 FF FF     call        0000000140321B10
  00000001403224D3: 88 44 24 28        mov         byte ptr [rsp+28h],al
  00000001403224D7: 0F B6 54 24 28     movzx       edx,byte ptr [rsp+28h]
  00000001403224DC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001403224E1: E8 BA F1 FF FF     call        00000001403216A0
  00000001403224E6: 48 83 C4 58        add         rsp,58h
  00000001403224EA: C3                 ret

  Summary

     1E02000 .text
