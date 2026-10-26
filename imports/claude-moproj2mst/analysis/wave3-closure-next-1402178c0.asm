Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402178C0: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001402178C5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402178CA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402178CF: 48 83 EC 38        sub         rsp,38h
  00000001402178D3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001402178D8: FF 15 02 C9 BE 01  call        qword ptr [0000000141E041E0h]
  00000001402178DE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001402178E3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001402178E8: FF 15 02 C9 BE 01  call        qword ptr [0000000141E041F0h]
  00000001402178EE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001402178F3: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001402178F8: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001402178FD: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140217902: 48 83 E8 08        sub         rax,8
  0000000140217906: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014021790B: 48 3B D1           cmp         rdx,rcx
  000000014021790E: 74 2F              je          000000014021793F
  0000000140217910: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140217915: E8 B6 0D 17 01     call        00000001413886D0
  000000014021791A: 4C 8B D8           mov         r11,rax
  000000014021791D: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140217922: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140217925: 49 39 03           cmp         qword ptr [r11],rax
  0000000140217928: 75 13              jne         000000014021793D
  000000014021792A: B2 01              mov         dl,1
  000000014021792C: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140217931: E8 2A AF DE FF     call        0000000140002860
  0000000140217936: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014021793B: EB 13              jmp         0000000140217950
  000000014021793D: EB B4              jmp         00000001402178F3
  000000014021793F: 33 D2              xor         edx,edx
  0000000140217941: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140217946: E8 15 AF DE FF     call        0000000140002860
  000000014021794B: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140217950: 48 83 C4 38        add         rsp,38h
  0000000140217954: C3                 ret

  Summary

     1E02000 .text
