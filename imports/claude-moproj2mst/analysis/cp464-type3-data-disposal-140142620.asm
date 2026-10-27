Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140142620: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140142625: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014014262A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014014262F: 48 83 EC 38        sub         rsp,38h
  0000000140142633: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014014263C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140142641: E8 6A 65 4B 00     call        00000001405F8BB0
  0000000140142646: 90                 nop
  0000000140142647: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014014264C: 48 8D 05 85 D6 07  lea         rax,[00000001421BFCD8h]
                    02
  0000000140142653: 48 89 01           mov         qword ptr [rcx],rax
  0000000140142656: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014014265B: 48 8D 05 3E D6 07  lea         rax,[00000001421BFCA0h]
                    02
  0000000140142662: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140142666: C7 44 24 24 03 00  mov         dword ptr [rsp+24h],3
                    00 00
  000000014014266E: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140142673: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140142676: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014014267B: FF 50 58           call        qword ptr [rax+58h]
  000000014014267E: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140142682: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140142686: 83 E0 01           and         eax,1
  0000000140142689: 85 C0              test        eax,eax
  000000014014268B: 74 08              je          0000000140142695
  000000014014268D: C7 44 24 24 02 00  mov         dword ptr [rsp+24h],2
                    00 00
  0000000140142695: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014014269A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014014269F: 48 89 41 40        mov         qword ptr [rcx+40h],rax
  00000001401426A3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001401426A8: E8 73 00 00 00     call        0000000140142720
  00000001401426AD: 90                 nop
  00000001401426AE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001401426B3: 48 83 C4 38        add         rsp,38h
  00000001401426B7: C3                 ret

  Summary

     1E02000 .text
