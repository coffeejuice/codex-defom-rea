Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140896280: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140896285: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014089628A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089628F: 48 83 EC 48        sub         rsp,48h
  0000000140896293: C6 44 24 28 01     mov         byte ptr [rsp+28h],1
  0000000140896298: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  00000001408962A1: C7 44 24 2C 00 00  mov         dword ptr [rsp+2Ch],0
                    00 00
  00000001408962A9: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  00000001408962AF: 74 10              je          00000001408962C1
  00000001408962B1: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001408962B6: E8 C5 A8 BD 00     call        0000000141470B80
  00000001408962BB: 48 83 F8 01        cmp         rax,1
  00000001408962BF: 73 0A              jae         00000001408962CB
  00000001408962C1: B8 01 00 00 00     mov         eax,1
  00000001408962C6: E9 86 00 00 00     jmp         0000000140896351
  00000001408962CB: 48 8D 15 B2 6A F1  lea         rdx,[00000001427ACD84h]
                    01
  00000001408962D2: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001408962D7: E8 FC 99 BD 00     call        000000014146FCD8
  00000001408962DC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001408962E1: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001408962E7: 75 07              jne         00000001408962F0
  00000001408962E9: B8 01 00 00 00     mov         eax,1
  00000001408962EE: EB 61              jmp         0000000140896351
  00000001408962F0: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  00000001408962F8: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  00000001408962FD: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140896302: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140896307: E8 54 F2 FF FF     call        0000000140895560
  000000014089630C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140896311: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  0000000140896318: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  000000014089631C: E8 EF 89 FB FF     call        000000014084ED10
  0000000140896321: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140896326: 44 8B 4C 24 60     mov         r9d,dword ptr [rsp+60h]
  000000014089632B: 4C 8B 44 24 30     mov         r8,qword ptr [rsp+30h]
  0000000140896330: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140896335: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014089633A: E8 A1 B8 0E 00     call        0000000140981BE0
  000000014089633F: 89 44 24 2C        mov         dword ptr [rsp+2Ch],eax
  0000000140896343: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140896348: E8 DB 97 BD 00     call        000000014146FB28
  000000014089634D: 8B 44 24 2C        mov         eax,dword ptr [rsp+2Ch]
  0000000140896351: 48 83 C4 48        add         rsp,48h
  0000000140896355: C3                 ret

  Summary

     1E02000 .text
