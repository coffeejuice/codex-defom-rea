Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140895920: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140895925: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014089592A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089592F: 48 83 EC 38        sub         rsp,38h
  0000000140895933: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  000000014089593B: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140895941: 74 10              je          0000000140895953
  0000000140895943: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140895948: E8 33 B2 BD 00     call        0000000141470B80
  000000014089594D: 48 83 F8 01        cmp         rax,1
  0000000140895951: 73 07              jae         000000014089595A
  0000000140895953: B8 01 00 00 00     mov         eax,1
  0000000140895958: EB 74              jmp         00000001408959CE
  000000014089595A: 48 8D 15 EB 68 F1  lea         rdx,[00000001427AC24Ch]
                    01
  0000000140895961: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140895966: E8 6D A3 BD 00     call        000000014146FCD8
  000000014089596B: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140895970: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140895976: 75 07              jne         000000014089597F
  0000000140895978: B8 01 00 00 00     mov         eax,1
  000000014089597D: EB 4F              jmp         00000001408959CE
  000000014089597F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140895984: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140895989: 8B 00              mov         eax,dword ptr [rax]
  000000014089598B: 89 81 44 17 00 00  mov         dword ptr [rcx+0000000000001744h],eax
  0000000140895991: C7 44 24 2C 00 00  mov         dword ptr [rsp+2Ch],0
                    00 00
  0000000140895999: 4C 8D 44 24 2C     lea         r8,[rsp+2Ch]
  000000014089599E: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001408959A3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408959A8: E8 B3 FB FF FF     call        0000000140895560
  00000001408959AD: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001408959B2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408959B7: E8 E4 6B 15 00     call        00000001409EC5A0
  00000001408959BC: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001408959C0: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001408959C5: E8 5E A1 BD 00     call        000000014146FB28
  00000001408959CA: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  00000001408959CE: 48 83 C4 38        add         rsp,38h
  00000001408959D2: C3                 ret

  Summary

     1E02000 .text
