Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140896750: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140896755: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014089675A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089675F: 48 83 EC 58        sub         rsp,58h
  0000000140896763: 8B 44 24 70        mov         eax,dword ptr [rsp+70h]
  0000000140896767: 89 44 24 2C        mov         dword ptr [rsp+2Ch],eax
  000000014089676B: C6 44 24 28 01     mov         byte ptr [rsp+28h],1
  0000000140896770: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140896779: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140896781: 48 8D 15 10 66 F1  lea         rdx,[00000001427ACD98h]
                    01
  0000000140896788: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  000000014089678D: E8 46 95 BD 00     call        000000014146FCD8
  0000000140896792: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140896797: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014089679D: 75 0A              jne         00000001408967A9
  000000014089679F: B8 01 00 00 00     mov         eax,1
  00000001408967A4: E9 8A 00 00 00     jmp         0000000140896833
  00000001408967A9: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  00000001408967B1: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  00000001408967B6: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001408967BB: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408967C0: E8 9B ED FF FF     call        0000000140895560
  00000001408967C5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408967CA: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  00000001408967D1: 8B 54 24 70        mov         edx,dword ptr [rsp+70h]
  00000001408967D5: E8 36 85 FB FF     call        000000014084ED10
  00000001408967DA: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001408967DF: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001408967E4: E8 B7 E7 F9 FF     call        0000000140834FA0
  00000001408967E9: 0F B6 C0           movzx       eax,al
  00000001408967EC: 85 C0              test        eax,eax
  00000001408967EE: 74 11              je          0000000140896801
  00000001408967F0: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001408967F5: E8 86 E4 F9 FF     call        0000000140834C80
  00000001408967FA: 0F B6 C0           movzx       eax,al
  00000001408967FD: 85 C0              test        eax,eax
  00000001408967FF: 75 07              jne         0000000140896808
  0000000140896801: B8 03 00 00 00     mov         eax,3
  0000000140896806: EB 2B              jmp         0000000140896833
  0000000140896808: 44 8B 4C 24 2C     mov         r9d,dword ptr [rsp+2Ch]
  000000014089680D: 4C 8B 44 24 38     mov         r8,qword ptr [rsp+38h]
  0000000140896812: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140896817: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014089681C: E8 1F B4 0E 00     call        0000000140981C40
  0000000140896821: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140896825: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014089682A: E8 F9 92 BD 00     call        000000014146FB28
  000000014089682F: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140896833: 48 83 C4 58        add         rsp,58h
  0000000140896837: C3                 ret

  Summary

     1E02000 .text
