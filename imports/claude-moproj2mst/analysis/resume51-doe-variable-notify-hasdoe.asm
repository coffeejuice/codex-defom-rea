Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140086CA0: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140086CA5: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140086CA9: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140086CAE: 53                 push        rbx
  0000000140086CAF: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  0000000140086CB6: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140086CBF: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086CC7: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  0000000140086CCC: 75 05              jne         0000000140086CD3
  0000000140086CCE: E9 36 01 00 00     jmp         0000000140086E09
  0000000140086CD3: 8B 94 24 98 00 00  mov         edx,dword ptr [rsp+0000000000000098h]
                    00
  0000000140086CDA: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086CE2: E8 39 FB FF FF     call        0000000140086820
  0000000140086CE7: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140086CEC: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140086CF2: 0F 84 11 01 00 00  je          0000000140086E09
  0000000140086CF8: 0F B6 84 24 A0 00  movzx       eax,byte ptr [rsp+00000000000000A0h]
                    00 00
  0000000140086D00: 85 C0              test        eax,eax
  0000000140086D02: 0F 84 D2 00 00 00  je          0000000140086DDA
  0000000140086D08: B9 08 00 00 00     mov         ecx,8
  0000000140086D0D: E8 AA 7D 3E 01     call        000000014146EABC
  0000000140086D12: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140086D17: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140086D1D: 74 12              je          0000000140086D31
  0000000140086D1F: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140086D24: FF 15 5E 49 D8 01  call        qword ptr [0000000141E0B688h]
  0000000140086D2A: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140086D2F: EB 09              jmp         0000000140086D3A
  0000000140086D31: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  0000000140086D3A: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140086D3F: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140086D44: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140086D49: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140086D4E: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140086D53: FF 15 4F EC D7 01  call        qword ptr [0000000141E059A8h]
  0000000140086D59: 48 8B D8           mov         rbx,rax
  0000000140086D5C: 48 8D 15 75 42 0B  lea         rdx,[000000014213AFD8h]
                    02
  0000000140086D63: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140086D68: FF 15 12 D7 D7 01  call        qword ptr [0000000141E04480h]
  0000000140086D6E: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140086D73: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140086D78: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140086D7D: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140086D85: 45 33 C9           xor         r9d,r9d
  0000000140086D88: 4C 8B C3           mov         r8,rbx
  0000000140086D8B: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140086D90: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140086D95: FF 15 F5 48 D8 01  call        qword ptr [0000000141E0B690h]
  0000000140086D9B: 90                 nop
  0000000140086D9C: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140086DA1: FF 15 39 EC D7 01  call        qword ptr [0000000141E059E0h]
  0000000140086DA7: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086DAF: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  0000000140086DB3: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086DBB: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140086DBF: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140086DC2: 41 B9 01 00 00 00  mov         r9d,1
  0000000140086DC8: 4C 8B 44 24 38     mov         r8,qword ptr [rsp+38h]
  0000000140086DCD: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140086DD2: FF 90 38 02 00 00  call        qword ptr [rax+0000000000000238h]
  0000000140086DD8: EB 2F              jmp         0000000140086E09
  0000000140086DDA: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086DE2: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  0000000140086DE6: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086DEE: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140086DF2: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140086DF5: 41 B9 01 00 00 00  mov         r9d,1
  0000000140086DFB: 45 33 C0           xor         r8d,r8d
  0000000140086DFE: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140086E03: FF 90 38 02 00 00  call        qword ptr [rax+0000000000000238h]
  0000000140086E09: 48 81 C4 80 00 00  add         rsp,80h
                    00
  0000000140086E10: 5B                 pop         rbx
  0000000140086E11: C3                 ret

  Summary

     1E02000 .text
