Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001413D0C60: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001413D0C65: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001413D0C6A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001413D0C6F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001413D0C74: 48 83 EC 58        sub         rsp,58h
  00000001413D0C78: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001413D0C7D: FF 15 4D 4D A3 00  call        qword ptr [0000000141E059D0h]
  00000001413D0C83: 83 F8 01           cmp         eax,1
  00000001413D0C86: 0F 85 56 01 00 00  jne         00000001413D0DE2
  00000001413D0C8C: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001413D0C91: 48 83 78 20 00     cmp         qword ptr [rax+20h],0
  00000001413D0C96: 0F 84 46 01 00 00  je          00000001413D0DE2
  00000001413D0C9C: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  00000001413D0CA1: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001413D0CA6: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  00000001413D0CAA: E8 51 13 00 00     call        00000001413D2000
  00000001413D0CAF: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001413D0CB4: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  00000001413D0CB9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001413D0CBE: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  00000001413D0CC2: E8 E9 13 00 00     call        00000001413D20B0
  00000001413D0CC7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001413D0CCC: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001413D0CD2: 0F 84 0A 01 00 00  je          00000001413D0DE2
  00000001413D0CD8: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001413D0CDE: 0F 84 FE 00 00 00  je          00000001413D0DE2
  00000001413D0CE4: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000001413D0CE9: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001413D0CEE: FF 15 A4 9C A3 00  call        qword ptr [0000000141E0A998h]
  00000001413D0CF4: 41 B8 20 00 00 00  mov         r8d,20h
  00000001413D0CFA: 48 8D 54 24 4C     lea         rdx,[rsp+4Ch]
  00000001413D0CFF: 48 8B C8           mov         rcx,rax
  00000001413D0D02: E8 F9 7C 9B FF     call        0000000140D88A00
  00000001413D0D07: 48 8B C8           mov         rcx,rax
  00000001413D0D0A: E8 31 D5 01 00     call        00000001413EE240
  00000001413D0D0F: 85 C0              test        eax,eax
  00000001413D0D11: 0F 84 CB 00 00 00  je          00000001413D0DE2
  00000001413D0D17: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  00000001413D0D1C: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000001413D0D21: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001413D0D26: 48 8B 49 20        mov         rcx,qword ptr [rcx+20h]
  00000001413D0D2A: E8 91 3B 00 00     call        00000001413D48C0
  00000001413D0D2F: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001413D0D34: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001413D0D3A: 0F 84 9B 00 00 00  je          00000001413D0DDB
  00000001413D0D40: B2 01              mov         dl,1
  00000001413D0D42: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001413D0D47: FF 15 9B 4D A3 00  call        qword ptr [0000000141E05AE8h]
  00000001413D0D4D: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  00000001413D0D52: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001413D0D57: FF 15 AB 48 A3 00  call        qword ptr [0000000141E05608h]
  00000001413D0D5D: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  00000001413D0D65: 4C 8D 0D 64 32 A8  lea         r9,[0000000142E53FD0h]
                    01
  00000001413D0D6C: 4C 8B 44 24 60     mov         r8,qword ptr [rsp+60h]
  00000001413D0D71: 48 8D 15 78 32 A8  lea         rdx,[0000000142E53FF0h]
                    01
  00000001413D0D78: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001413D0D7D: FF 15 4D 34 A3 00  call        qword ptr [0000000141E041D0h]
  00000001413D0D83: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001413D0D88: 48 83 C1 18        add         rcx,18h
  00000001413D0D8C: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001413D0D91: E8 3A 7A FF FF     call        00000001413C87D0
  00000001413D0D96: 4C 8B D8           mov         r11,rax
  00000001413D0D99: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001413D0D9E: 49 89 03           mov         qword ptr [r11],rax
  00000001413D0DA1: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001413D0DA6: 48 83 C1 10        add         rcx,10h
  00000001413D0DAA: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001413D0DAF: E8 1C 7A FF FF     call        00000001413C87D0
  00000001413D0DB4: 4C 8B D8           mov         r11,rax
  00000001413D0DB7: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001413D0DBC: 49 89 03           mov         qword ptr [r11],rax
  00000001413D0DBF: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001413D0DC4: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001413D0DC9: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  00000001413D0DCD: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001413D0DD2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001413D0DD7: 48 89 41 30        mov         qword ptr [rcx+30h],rax
  00000001413D0DDB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001413D0DE0: EB 02              jmp         00000001413D0DE4
  00000001413D0DE2: 33 C0              xor         eax,eax
  00000001413D0DE4: 48 83 C4 58        add         rsp,58h
  00000001413D0DE8: C3                 ret

  Summary

     1E02000 .text
