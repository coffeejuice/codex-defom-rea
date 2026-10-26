Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140919CB0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140919CB5: 48 83 EC 68        sub         rsp,68h
  0000000140919CB9: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140919CC2: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140919CC7: 48 8D 05 DA 46 EA  lea         rax,[00000001427BE3A8h]
                    01
  0000000140919CCE: 48 89 01           mov         qword ptr [rcx],rax
  0000000140919CD1: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140919CD6: 48 83 C1 38        add         rcx,38h
  0000000140919CDA: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140919CDF: E8 1C 7B 33 00     call        0000000140C51800
  0000000140919CE4: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140919CE9: 48 83 C1 38        add         rcx,38h
  0000000140919CED: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140919CF2: E8 79 1E 4A 00     call        0000000140DBBB70
  0000000140919CF7: 48 8B D0           mov         rdx,rax
  0000000140919CFA: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140919CFF: E8 6C C7 80 FF     call        0000000140126470
  0000000140919D04: 0F B6 C0           movzx       eax,al
  0000000140919D07: 85 C0              test        eax,eax
  0000000140919D09: 74 57              je          0000000140919D62
  0000000140919D0B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140919D10: E8 EB 56 D3 FF     call        000000014064F400
  0000000140919D15: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140919D18: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140919D1D: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140919D22: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140919D27: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140919D2D: 74 16              je          0000000140919D45
  0000000140919D2F: BA 01 00 00 00     mov         edx,1
  0000000140919D34: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140919D39: E8 32 3E 00 00     call        000000014091DB70
  0000000140919D3E: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140919D43: EB 09              jmp         0000000140919D4E
  0000000140919D45: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  0000000140919D4E: 45 33 C0           xor         r8d,r8d
  0000000140919D51: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140919D56: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140919D5B: E8 C0 DE AB 00     call        00000001413D7C20
  0000000140919D60: EB 82              jmp         0000000140919CE4
  0000000140919D62: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140919D67: 48 81 C1 88 00 00  add         rcx,88h
                    00
  0000000140919D6E: FF 15 6C BC 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140919D74: 90                 nop
  0000000140919D75: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140919D7A: 48 81 C1 80 00 00  add         rcx,80h
                    00
  0000000140919D81: FF 15 59 BC 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140919D87: 90                 nop
  0000000140919D88: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140919D8D: 48 83 C1 58        add         rcx,58h
  0000000140919D91: 4C 8D 0D 68 29 7A  lea         r9,[00000001400BC700h]
                    FF
  0000000140919D98: 41 B8 03 00 00 00  mov         r8d,3
  0000000140919D9E: BA 08 00 00 00     mov         edx,8
  0000000140919DA3: E8 44 3B B5 00     call        000000014146D8EC
  0000000140919DA8: 90                 nop
  0000000140919DA9: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140919DAE: 48 83 C1 38        add         rcx,38h
  0000000140919DB2: E8 E9 E3 8E FF     call        00000001402081A0
  0000000140919DB7: 90                 nop
  0000000140919DB8: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140919DBD: 48 83 C1 30        add         rcx,30h
  0000000140919DC1: FF 15 19 BC 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140919DC7: 90                 nop
  0000000140919DC8: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140919DCD: E8 6E 42 00 00     call        000000014091E040
  0000000140919DD2: 48 83 C4 68        add         rsp,68h
  0000000140919DD6: C3                 ret

  Summary

     1E02000 .text
