Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408169D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408169D5: 48 83 EC 68        sub         rsp,68h
  00000001408169D9: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001408169E2: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001408169E7: 48 83 C1 30        add         rcx,30h
  00000001408169EB: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001408169F0: E8 0B AE 43 00     call        0000000140C51800
  00000001408169F5: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001408169FA: 48 83 C1 30        add         rcx,30h
  00000001408169FE: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140816A03: E8 68 51 5A 00     call        0000000140DBBB70
  0000000140816A08: 48 8B D0           mov         rdx,rax
  0000000140816A0B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140816A10: E8 5B FA 90 FF     call        0000000140126470
  0000000140816A15: 0F B6 C0           movzx       eax,al
  0000000140816A18: 85 C0              test        eax,eax
  0000000140816A1A: 74 57              je          0000000140816A73
  0000000140816A1C: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140816A21: E8 DA 89 E3 FF     call        000000014064F400
  0000000140816A26: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140816A29: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140816A2E: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140816A33: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140816A38: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140816A3E: 74 16              je          0000000140816A56
  0000000140816A40: BA 01 00 00 00     mov         edx,1
  0000000140816A45: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140816A4A: E8 01 1D 00 00     call        0000000140818750
  0000000140816A4F: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140816A54: EB 09              jmp         0000000140816A5F
  0000000140816A56: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  0000000140816A5F: 45 33 C0           xor         r8d,r8d
  0000000140816A62: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140816A67: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140816A6C: E8 AF 11 BC 00     call        00000001413D7C20
  0000000140816A71: EB 82              jmp         00000001408169F5
  0000000140816A73: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816A78: E8 93 12 00 00     call        0000000140817D10
  0000000140816A7D: 90                 nop
  0000000140816A7E: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816A83: 48 83 C1 38        add         rcx,38h
  0000000140816A87: E8 14 17 9F FF     call        00000001402081A0
  0000000140816A8C: 90                 nop
  0000000140816A8D: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816A92: 48 83 C1 30        add         rcx,30h
  0000000140816A96: E8 05 17 9F FF     call        00000001402081A0
  0000000140816A9B: 90                 nop
  0000000140816A9C: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816AA1: 48 83 C1 28        add         rcx,28h
  0000000140816AA5: E8 F6 16 9F FF     call        00000001402081A0
  0000000140816AAA: 90                 nop
  0000000140816AAB: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816AB0: 48 83 C1 08        add         rcx,8
  0000000140816AB4: FF 15 26 EF 5E 01  call        qword ptr [0000000141E059E0h]
  0000000140816ABA: 90                 nop
  0000000140816ABB: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140816AC0: FF 15 1A EF 5E 01  call        qword ptr [0000000141E059E0h]
  0000000140816AC6: 48 83 C4 68        add         rsp,68h
  0000000140816ACA: C3                 ret

  Summary

     1E02000 .text
