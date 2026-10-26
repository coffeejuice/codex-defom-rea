Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140896460: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140896465: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014089646A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089646F: 48 83 EC 58        sub         rsp,58h
  0000000140896473: C6 44 24 30 01     mov         byte ptr [rsp+30h],1
  0000000140896478: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140896481: C7 44 24 34 00 00  mov         dword ptr [rsp+34h],0
                    00 00
  0000000140896489: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  000000014089648F: 74 10              je          00000001408964A1
  0000000140896491: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140896496: E8 E5 A6 BD 00     call        0000000141470B80
  000000014089649B: 48 83 F8 01        cmp         rax,1
  000000014089649F: 73 0A              jae         00000001408964AB
  00000001408964A1: B8 01 00 00 00     mov         eax,1
  00000001408964A6: E9 86 00 00 00     jmp         0000000140896531
  00000001408964AB: 48 8D 15 DA 68 F1  lea         rdx,[00000001427ACD8Ch]
                    01
  00000001408964B2: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001408964B7: E8 1C 98 BD 00     call        000000014146FCD8
  00000001408964BC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001408964C1: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001408964C7: 75 07              jne         00000001408964D0
  00000001408964C9: B8 01 00 00 00     mov         eax,1
  00000001408964CE: EB 61              jmp         0000000140896531
  00000001408964D0: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  00000001408964D8: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  00000001408964DD: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001408964E2: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408964E7: E8 74 F0 FF FF     call        0000000140895560
  00000001408964EC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408964F1: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  00000001408964F8: 8B 54 24 70        mov         edx,dword ptr [rsp+70h]
  00000001408964FC: E8 0F 88 FB FF     call        000000014084ED10
  0000000140896501: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140896506: 44 8B 4C 24 70     mov         r9d,dword ptr [rsp+70h]
  000000014089650B: 4C 8B 44 24 38     mov         r8,qword ptr [rsp+38h]
  0000000140896510: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  0000000140896515: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014089651A: E8 11 95 0E 00     call        000000014097FA30
  000000014089651F: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  0000000140896523: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140896528: E8 FB 95 BD 00     call        000000014146FB28
  000000014089652D: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  0000000140896531: 48 83 C4 58        add         rsp,58h
  0000000140896535: C3                 ret

  Summary

     1E02000 .text
