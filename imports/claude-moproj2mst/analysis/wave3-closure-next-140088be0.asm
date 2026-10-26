Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140088BE0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140088BE5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140088BEA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140088BEF: 48 83 EC 48        sub         rsp,48h
  0000000140088BF3: 83 7C 24 60 00     cmp         dword ptr [rsp+60h],0
  0000000140088BF8: 7D 36              jge         0000000140088C30
  0000000140088BFA: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140088C02: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140088C07: FF 15 03 B6 D7 01  call        qword ptr [0000000141E04210h]
  0000000140088C0D: 44 8B D8           mov         r11d,eax
  0000000140088C10: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  0000000140088C14: 41 03 C3           add         eax,r11d
  0000000140088C17: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  0000000140088C1B: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140088C20: 48 8D 4C 24 34     lea         rcx,[rsp+34h]
  0000000140088C25: E8 76 A9 F7 FF     call        00000001400035A0
  0000000140088C2A: 8B 00              mov         eax,dword ptr [rax]
  0000000140088C2C: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  0000000140088C30: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140088C35: FF 15 D5 B5 D7 01  call        qword ptr [0000000141E04210h]
  0000000140088C3B: 39 44 24 60        cmp         dword ptr [rsp+60h],eax
  0000000140088C3F: 7D 79              jge         0000000140088CBA
  0000000140088C41: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  0000000140088C45: 83 EA 01           sub         edx,1
  0000000140088C48: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140088C4D: FF 15 B5 B5 D7 01  call        qword ptr [0000000141E04208h]
  0000000140088C53: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140088C58: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140088C5D: FF 15 8D B5 D7 01  call        qword ptr [0000000141E041F0h]
  0000000140088C63: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140088C68: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140088C6D: 48 83 C0 08        add         rax,8
  0000000140088C71: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140088C76: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140088C7B: 48 39 44 24 20     cmp         qword ptr [rsp+20h],rax
  0000000140088C80: 74 38              je          0000000140088CBA
  0000000140088C82: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140088C87: E8 44 FA 2F 01     call        00000001413886D0
  0000000140088C8C: 4C 8B D8           mov         r11,rax
  0000000140088C8F: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140088C94: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140088C97: 49 39 03           cmp         qword ptr [r11],rax
  0000000140088C9A: 75 1C              jne         0000000140088CB8
  0000000140088C9C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140088CA1: FF 15 39 B5 D7 01  call        qword ptr [0000000141E041E0h]
  0000000140088CA7: 4C 8B D8           mov         r11,rax
  0000000140088CAA: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140088CAF: 49 2B C3           sub         rax,r11
  0000000140088CB2: 48 C1 F8 03        sar         rax,3
  0000000140088CB6: EB 07              jmp         0000000140088CBF
  0000000140088CB8: EB AE              jmp         0000000140088C68
  0000000140088CBA: B8 FF FF FF FF     mov         eax,0FFFFFFFFh
  0000000140088CBF: 48 83 C4 48        add         rsp,48h
  0000000140088CC3: C3                 ret

  Summary

     1E02000 .text
