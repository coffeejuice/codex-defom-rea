Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EC8DA0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140EC8DA5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140EC8DAA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EC8DAF: 48 83 EC 58        sub         rsp,58h
  0000000140EC8DB3: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140EC8DBC: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140EC8DC1: FF 15 F9 B6 F3 00  call        qword ptr [0000000141E044C0h]
  0000000140EC8DC7: 90                 nop
  0000000140EC8DC8: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140EC8DD0: EB 0B              jmp         0000000140EC8DDD
  0000000140EC8DD2: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140EC8DD6: 83 C0 01           add         eax,1
  0000000140EC8DD9: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140EC8DDD: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140EC8DE2: FF 15 B8 26 F4 00  call        qword ptr [0000000141E0B4A0h]
  0000000140EC8DE8: 39 44 24 28        cmp         dword ptr [rsp+28h],eax
  0000000140EC8DEC: 7D 6B              jge         0000000140EC8E59
  0000000140EC8DEE: 44 8B 44 24 28     mov         r8d,dword ptr [rsp+28h]
  0000000140EC8DF3: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC8DF8: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140EC8DFD: FF 15 4D 01 F4 00  call        qword ptr [0000000141E08F50h]
  0000000140EC8E03: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140EC8E08: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140EC8E0D: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140EC8E12: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140EC8E17: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140EC8E1C: FF 15 4E B6 F3 00  call        qword ptr [0000000141E04470h]
  0000000140EC8E22: 90                 nop
  0000000140EC8E23: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC8E28: FF 15 B2 CB F3 00  call        qword ptr [0000000141E059E0h]
  0000000140EC8E2E: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140EC8E33: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC8E38: E8 43 0B 00 00     call        0000000140EC9980
  0000000140EC8E3D: 39 44 24 70        cmp         dword ptr [rsp+70h],eax
  0000000140EC8E41: 75 11              jne         0000000140EC8E54
  0000000140EC8E43: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  0000000140EC8E47: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140EC8E4C: FF 15 76 26 F4 00  call        qword ptr [0000000141E0B4C8h]
  0000000140EC8E52: EB 05              jmp         0000000140EC8E59
  0000000140EC8E54: E9 79 FF FF FF     jmp         0000000140EC8DD2
  0000000140EC8E59: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140EC8E5E: FF 15 7C CB F3 00  call        qword ptr [0000000141E059E0h]
  0000000140EC8E64: 48 83 C4 58        add         rsp,58h
  0000000140EC8E68: C3                 ret

  Summary

     1E02000 .text
