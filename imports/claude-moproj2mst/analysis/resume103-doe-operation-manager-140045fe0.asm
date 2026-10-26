Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140045FE0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140045FE5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140045FEA: 48 83 EC 58        sub         rsp,58h
  0000000140045FEE: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140045FF7: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  0000000140045FFC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140046001: E8 1A 61 02 00     call        000000014006C120
  0000000140046006: 90                 nop
  0000000140046007: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014004600C: 48 8D 05 75 7E 0D  lea         rax,[000000014211DE88h]
                    02
  0000000140046013: 48 89 01           mov         qword ptr [rcx],rax
  0000000140046016: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014004601B: C6 40 28 00        mov         byte ptr [rax+28h],0
  000000014004601F: BA FF FF FF FF     mov         edx,0FFFFFFFFh
  0000000140046024: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140046029: E8 C2 1A 00 00     call        0000000140047AF0
  000000014004602E: 4C 8B 5C 24 60     mov         r11,qword ptr [rsp+60h]
  0000000140046033: 49 C7 43 30 00 00  mov         qword ptr [r11+30h],0
                    00 00
  000000014004603B: BA 02 00 00 00     mov         edx,2
  0000000140046040: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140046045: FF 15 35 57 DC 01  call        qword ptr [0000000141E0B780h]
  000000014004604B: B9 10 00 00 00     mov         ecx,10h
  0000000140046050: E8 67 8A 42 01     call        000000014146EABC
  0000000140046055: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014004605A: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140046060: 74 2B              je          000000014004608D
  0000000140046062: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140046067: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014004606C: FF 15 06 57 DC 01  call        qword ptr [0000000141E0B778h]
  0000000140046072: 4C 8B 5C 24 38     mov         r11,qword ptr [rsp+38h]
  0000000140046077: 48 8D 05 CA 87 0C  lea         rax,[000000014210E848h]
                    02
  000000014004607E: 49 89 03           mov         qword ptr [r11],rax
  0000000140046081: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140046086: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014004608B: EB 09              jmp         0000000140046096
  000000014004608D: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  0000000140046096: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014004609B: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400460A0: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400460A5: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001400460AA: 48 89 41 18        mov         qword ptr [rcx+18h],rax
  00000001400460AE: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400460B6: 4C 8D 0D DB 73 0D  lea         r9,[000000014211D498h]
                    02
  00000001400460BD: 4C 8B 44 24 60     mov         r8,qword ptr [rsp+60h]
  00000001400460C2: 48 8D 15 0F 74 0D  lea         rdx,[000000014211D4D8h]
                    02
  00000001400460C9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400460CE: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  00000001400460D2: FF 15 F8 E0 DB 01  call        qword ptr [0000000141E041D0h]
  00000001400460D8: 90                 nop
  00000001400460D9: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400460DE: 48 83 C4 58        add         rsp,58h
  00000001400460E2: C3                 ret

  Summary

     1E02000 .text
