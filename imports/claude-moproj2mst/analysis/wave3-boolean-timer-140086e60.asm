Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140086E60: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140086E65: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140086E69: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140086E6E: 53                 push        rbx
  0000000140086E6F: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  0000000140086E76: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140086E7F: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086E87: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  0000000140086E8C: 75 05              jne         0000000140086E93
  0000000140086E8E: E9 36 01 00 00     jmp         0000000140086FC9
  0000000140086E93: 8B 94 24 98 00 00  mov         edx,dword ptr [rsp+0000000000000098h]
                    00
  0000000140086E9A: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086EA2: E8 79 F9 FF FF     call        0000000140086820
  0000000140086EA7: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140086EAC: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140086EB2: 0F 84 11 01 00 00  je          0000000140086FC9
  0000000140086EB8: 0F B6 84 24 A0 00  movzx       eax,byte ptr [rsp+00000000000000A0h]
                    00 00
  0000000140086EC0: 85 C0              test        eax,eax
  0000000140086EC2: 0F 84 D2 00 00 00  je          0000000140086F9A
  0000000140086EC8: B9 08 00 00 00     mov         ecx,8
  0000000140086ECD: E8 EA 7B 3E 01     call        000000014146EABC
  0000000140086ED2: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140086ED7: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140086EDD: 74 12              je          0000000140086EF1
  0000000140086EDF: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140086EE4: FF 15 9E 47 D8 01  call        qword ptr [0000000141E0B688h]
  0000000140086EEA: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140086EEF: EB 09              jmp         0000000140086EFA
  0000000140086EF1: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  0000000140086EFA: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140086EFF: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140086F04: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140086F09: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140086F0E: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140086F13: FF 15 8F EA D7 01  call        qword ptr [0000000141E059A8h]
  0000000140086F19: 48 8B D8           mov         rbx,rax
  0000000140086F1C: 48 8D 15 DD 40 0B  lea         rdx,[000000014213B000h]
                    02
  0000000140086F23: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140086F28: FF 15 52 D5 D7 01  call        qword ptr [0000000141E04480h]
  0000000140086F2E: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140086F33: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140086F38: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140086F3D: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140086F45: 45 33 C9           xor         r9d,r9d
  0000000140086F48: 4C 8B C3           mov         r8,rbx
  0000000140086F4B: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140086F50: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140086F55: FF 15 35 47 D8 01  call        qword ptr [0000000141E0B690h]
  0000000140086F5B: 90                 nop
  0000000140086F5C: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140086F61: FF 15 79 EA D7 01  call        qword ptr [0000000141E059E0h]
  0000000140086F67: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086F6F: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  0000000140086F73: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086F7B: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140086F7F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140086F82: 41 B9 02 00 00 00  mov         r9d,2
  0000000140086F88: 4C 8B 44 24 38     mov         r8,qword ptr [rsp+38h]
  0000000140086F8D: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140086F92: FF 90 38 02 00 00  call        qword ptr [rax+0000000000000238h]
  0000000140086F98: EB 2F              jmp         0000000140086FC9
  0000000140086F9A: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086FA2: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  0000000140086FA6: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140086FAE: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140086FB2: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140086FB5: 41 B9 02 00 00 00  mov         r9d,2
  0000000140086FBB: 45 33 C0           xor         r8d,r8d
  0000000140086FBE: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140086FC3: FF 90 38 02 00 00  call        qword ptr [rax+0000000000000238h]
  0000000140086FC9: 48 81 C4 80 00 00  add         rsp,80h
                    00
  0000000140086FD0: 5B                 pop         rbx
  0000000140086FD1: C3                 ret

  Summary

     1E02000 .text
