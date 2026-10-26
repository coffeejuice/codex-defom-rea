Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140896030: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140896035: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089603A: 48 83 EC 48        sub         rsp,48h
  000000014089603E: C6 44 24 30 01     mov         byte ptr [rsp+30h],1
  0000000140896043: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  000000014089604C: C7 44 24 34 00 00  mov         dword ptr [rsp+34h],0
                    00 00
  0000000140896054: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  000000014089605A: 74 10              je          000000014089606C
  000000014089605C: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140896061: E8 1A AB BD 00     call        0000000141470B80
  0000000140896066: 48 83 F8 01        cmp         rax,1
  000000014089606A: 73 07              jae         0000000140896073
  000000014089606C: B8 01 00 00 00     mov         eax,1
  0000000140896071: EB 62              jmp         00000001408960D5
  0000000140896073: 48 8D 15 82 6C F1  lea         rdx,[00000001427ACCFCh]
                    01
  000000014089607A: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014089607F: E8 54 9C BD 00     call        000000014146FCD8
  0000000140896084: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140896089: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014089608F: 75 07              jne         0000000140896098
  0000000140896091: B8 01 00 00 00     mov         eax,1
  0000000140896096: EB 3D              jmp         00000001408960D5
  0000000140896098: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  00000001408960A0: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  00000001408960A5: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001408960AA: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001408960AF: E8 AC F4 FF FF     call        0000000140895560
  00000001408960B4: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001408960B9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001408960BE: E8 CD 9E 2B 00     call        0000000140B4FF90
  00000001408960C3: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  00000001408960C7: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001408960CC: E8 57 9A BD 00     call        000000014146FB28
  00000001408960D1: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  00000001408960D5: 48 83 C4 48        add         rsp,48h
  00000001408960D9: C3                 ret

  Summary

     1E02000 .text
