Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D4BAB0: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140D4BAB5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D4BABA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D4BABF: 56                 push        rsi
  0000000140D4BAC0: 57                 push        rdi
  0000000140D4BAC1: 48 83 EC 58        sub         rsp,58h
  0000000140D4BAC5: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140D4BACA: FF 15 10 87 0B 01  call        qword ptr [0000000141E041E0h]
  0000000140D4BAD0: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D4BAD5: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140D4BADA: FF 15 10 87 0B 01  call        qword ptr [0000000141E041F0h]
  0000000140D4BAE0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D4BAE5: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140D4BAEA: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140D4BAEF: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D4BAF4: 48 83 E8 08        sub         rax,8
  0000000140D4BAF8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D4BAFD: 48 3B D1           cmp         rdx,rcx
  0000000140D4BB00: 74 47              je          0000000140D4BB49
  0000000140D4BB02: 48 8D 7C 24 30     lea         rdi,[rsp+30h]
  0000000140D4BB07: 48 8B B4 24 80 00  mov         rsi,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D4BB0F: B9 20 00 00 00     mov         ecx,20h
  0000000140D4BB14: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D4BB16: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D4BB1B: E8 30 DC 8C FF     call        0000000140619750
  0000000140D4BB20: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140D4BB25: 48 8B C8           mov         rcx,rax
  0000000140D4BB28: E8 B3 FE FF FF     call        0000000140D4B9E0
  0000000140D4BB2D: 0F B6 C0           movzx       eax,al
  0000000140D4BB30: 85 C0              test        eax,eax
  0000000140D4BB32: 74 13              je          0000000140D4BB47
  0000000140D4BB34: B2 01              mov         dl,1
  0000000140D4BB36: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D4BB3B: E8 20 6D 2B FF     call        0000000140002860
  0000000140D4BB40: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140D4BB45: EB 13              jmp         0000000140D4BB5A
  0000000140D4BB47: EB 9C              jmp         0000000140D4BAE5
  0000000140D4BB49: 33 D2              xor         edx,edx
  0000000140D4BB4B: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D4BB50: E8 0B 6D 2B FF     call        0000000140002860
  0000000140D4BB55: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140D4BB5A: 48 83 C4 58        add         rsp,58h
  0000000140D4BB5E: 5F                 pop         rdi
  0000000140D4BB5F: 5E                 pop         rsi
  0000000140D4BB60: C3                 ret

  Summary

     1E02000 .text
