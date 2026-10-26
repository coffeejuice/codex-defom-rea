Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400B9CD0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400B9CD5: 48 83 EC 68        sub         rsp,68h
  00000001400B9CD9: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400B9CE2: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400B9CE7: E8 54 BF FF FF     call        00000001400B5C40
  00000001400B9CEC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400B9CF1: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001400B9CF7: 75 07              jne         00000001400B9D00
  00000001400B9CF9: 33 C0              xor         eax,eax
  00000001400B9CFB: E9 DE 00 00 00     jmp         00000001400B9DDE
  00000001400B9D00: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001400B9D05: E8 86 BB FE FF     call        00000001400A5890
  00000001400B9D0A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400B9D0F: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400B9D14: E8 07 66 F5 FF     call        0000000140010320
  00000001400B9D19: 83 F8 04           cmp         eax,4
  00000001400B9D1C: 0F 85 B7 00 00 00  jne         00000001400B9DD9
  00000001400B9D22: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400B9D27: 48 83 C1 48        add         rcx,48h
  00000001400B9D2B: E8 C0 11 B6 00     call        0000000140C1AEF0
  00000001400B9D30: 48 85 C0           test        rax,rax
  00000001400B9D33: 0F 85 8D 00 00 00  jne         00000001400B9DC6
  00000001400B9D39: 41 B0 01           mov         r8b,1
  00000001400B9D3C: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001400B9D41: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400B9D46: E8 35 01 00 00     call        00000001400B9E80
  00000001400B9D4B: 90                 nop
  00000001400B9D4C: B9 C0 19 00 00     mov         ecx,19C0h
  00000001400B9D51: E8 66 4D 3B 01     call        000000014146EABC
  00000001400B9D56: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400B9D5B: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001400B9D61: 74 13              je          00000001400B9D76
  00000001400B9D63: 33 D2              xor         edx,edx
  00000001400B9D65: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400B9D6A: E8 F1 06 C2 00     call        0000000140CDA460
  00000001400B9D6F: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400B9D74: EB 09              jmp         00000001400B9D7F
  00000001400B9D76: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  00000001400B9D7F: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400B9D84: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400B9D89: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400B9D8E: 48 83 C1 48        add         rcx,48h
  00000001400B9D92: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001400B9D97: E8 84 2B C0 00     call        0000000140CBC920
  00000001400B9D9C: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400B9DA1: 48 83 C1 48        add         rcx,48h
  00000001400B9DA5: E8 46 11 B6 00     call        0000000140C1AEF0
  00000001400B9DAA: 45 33 C0           xor         r8d,r8d
  00000001400B9DAD: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001400B9DB2: 48 8B C8           mov         rcx,rax
  00000001400B9DB5: E8 06 30 C2 00     call        0000000140CDCDC0
  00000001400B9DBA: 90                 nop
  00000001400B9DBB: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001400B9DC0: FF 15 1A BC D4 01  call        qword ptr [0000000141E059E0h]
  00000001400B9DC6: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400B9DCB: 48 83 C1 48        add         rcx,48h
  00000001400B9DCF: E8 1C 11 B6 00     call        0000000140C1AEF0
  00000001400B9DD4: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400B9DD9: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400B9DDE: 48 83 C4 68        add         rsp,68h
  00000001400B9DE2: C3                 ret

  Summary

     1E02000 .text
