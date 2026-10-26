Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D40930: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D40935: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D4093A: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  0000000140D40941: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D4094A: C7 44 24 58 00 00  mov         dword ptr [rsp+58h],0
                    00 00
  0000000140D40952: 48 83 BC 24 98 00  cmp         qword ptr [rsp+0000000000000098h],0
                    00 00 00
  0000000140D4095B: 75 16              jne         0000000140D40973
  0000000140D4095D: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D40965: FF 15 D5 AC 0C 01  call        qword ptr [0000000141E0B640h]
  0000000140D4096B: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140D40973: B9 30 00 00 00     mov         ecx,30h
  0000000140D40978: E8 3F E1 72 00     call        000000014146EABC
  0000000140D4097D: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D40982: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140D40988: 74 11              je          0000000140D4099B
  0000000140D4098A: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140D4098F: E8 DC 7D 09 00     call        0000000140DD8770
  0000000140D40994: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140D40999: EB 09              jmp         0000000140D409A4
  0000000140D4099B: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  0000000140D409A4: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140D409A9: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D409AE: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140D409B3: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D409B8: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  0000000140D409BD: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140D409C5: FF 15 ED AB 0C 01  call        qword ptr [0000000141E0B5B8h]
  0000000140D409CB: B9 10 00 00 00     mov         ecx,10h
  0000000140D409D0: E8 E7 E0 72 00     call        000000014146EABC
  0000000140D409D5: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D409DA: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140D409E0: 74 49              je          0000000140D40A2B
  0000000140D409E2: 48 8D 15 96 AA CC  lea         rdx,[0000000142A0B47Fh]
                    01
  0000000140D409E9: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D409EE: FF 15 8C 3A 0C 01  call        qword ptr [0000000141E04480h]
  0000000140D409F4: 90                 nop
  0000000140D409F5: 8B 44 24 58        mov         eax,dword ptr [rsp+58h]
  0000000140D409F9: 83 C8 01           or          eax,1
  0000000140D409FC: 89 44 24 58        mov         dword ptr [rsp+58h],eax
  0000000140D40A00: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000140D40A05: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D40A0A: FF 15 F8 AA 0C 01  call        qword ptr [0000000141E0B508h]
  0000000140D40A10: 4C 8B 5C 24 48     mov         r11,qword ptr [rsp+48h]
  0000000140D40A15: 48 8D 05 FC E0 3C  lea         rax,[000000014210EB18h]
                    01
  0000000140D40A1C: 49 89 03           mov         qword ptr [r11],rax
  0000000140D40A1F: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140D40A24: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140D40A29: EB 09              jmp         0000000140D40A34
  0000000140D40A2B: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  0000000140D40A34: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140D40A39: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D40A3E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D40A43: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D40A48: 8B 44 24 58        mov         eax,dword ptr [rsp+58h]
  0000000140D40A4C: 83 E0 01           and         eax,1
  0000000140D40A4F: 85 C0              test        eax,eax
  0000000140D40A51: 74 10              je          0000000140D40A63
  0000000140D40A53: 83 64 24 58 FE     and         dword ptr [rsp+58h],0FFFFFFFEh
  0000000140D40A58: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D40A5D: FF 15 7D 4F 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D40A63: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140D40A68: FF 15 A2 AA 0C 01  call        qword ptr [0000000141E0B510h]
  0000000140D40A6E: 4C 8B 4C 24 20     mov         r9,qword ptr [rsp+20h]
  0000000140D40A73: 41 B8 01 00 00 00  mov         r8d,1
  0000000140D40A79: 8B D0              mov         edx,eax
  0000000140D40A7B: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140D40A83: FF 15 8F AA 0C 01  call        qword ptr [0000000141E0B518h]
  0000000140D40A89: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140D40A8E: 48 81 C4 88 00 00  add         rsp,88h
                    00
  0000000140D40A95: C3                 ret

  Summary

     1E02000 .text
