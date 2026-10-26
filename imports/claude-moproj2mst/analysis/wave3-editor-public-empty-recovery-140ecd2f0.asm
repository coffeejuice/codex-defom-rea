Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140ECD2F0: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140ECD2F4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140ECD2F9: 48 83 EC 38        sub         rsp,38h
  0000000140ECD2FD: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000140ECD302: 85 C0              test        eax,eax
  0000000140ECD304: 0F 84 DA 00 00 00  je          0000000140ECD3E4
  0000000140ECD30A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD30F: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECD316: 33 D2              xor         edx,edx
  0000000140ECD318: 48 8B 89 C0 09 00  mov         rcx,qword ptr [rcx+00000000000009C0h]
                    00
  0000000140ECD31F: FF 15 93 B8 F3 00  call        qword ptr [0000000141E08BB8h]
  0000000140ECD325: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD32A: E8 B1 3B 26 FF     call        0000000140130EE0
  0000000140ECD32F: 0F B6 C0           movzx       eax,al
  0000000140ECD332: 85 C0              test        eax,eax
  0000000140ECD334: 74 48              je          0000000140ECD37E
  0000000140ECD336: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECD33B: 48 8B 80 50 01 00  mov         rax,qword ptr [rax+0000000000000150h]
                    00
  0000000140ECD342: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD347: 48 8B 89 50 01 00  mov         rcx,qword ptr [rcx+0000000000000150h]
                    00
  0000000140ECD34E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ECD351: BA 14 00 00 00     mov         edx,14h
  0000000140ECD356: FF 50 58           call        qword ptr [rax+58h]
  0000000140ECD359: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECD35E: 48 8B 80 50 01 00  mov         rax,qword ptr [rax+0000000000000150h]
                    00
  0000000140ECD365: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD36A: 48 8B 89 50 01 00  mov         rcx,qword ptr [rcx+0000000000000150h]
                    00
  0000000140ECD371: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ECD374: BA 02 00 00 00     mov         edx,2
  0000000140ECD379: FF 50 60           call        qword ptr [rax+60h]
  0000000140ECD37C: EB 46              jmp         0000000140ECD3C4
  0000000140ECD37E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECD383: 48 8B 80 50 01 00  mov         rax,qword ptr [rax+0000000000000150h]
                    00
  0000000140ECD38A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD38F: 48 8B 89 50 01 00  mov         rcx,qword ptr [rcx+0000000000000150h]
                    00
  0000000140ECD396: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ECD399: BA 08 00 00 00     mov         edx,8
  0000000140ECD39E: FF 50 58           call        qword ptr [rax+58h]
  0000000140ECD3A1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECD3A6: 48 8B 80 50 01 00  mov         rax,qword ptr [rax+0000000000000150h]
                    00
  0000000140ECD3AD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD3B2: 48 8B 89 50 01 00  mov         rcx,qword ptr [rcx+0000000000000150h]
                    00
  0000000140ECD3B9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ECD3BC: BA 04 00 00 00     mov         edx,4
  0000000140ECD3C1: FF 50 60           call        qword ptr [rax+60h]
  0000000140ECD3C4: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140ECD3C9: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140ECD3CE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD3D3: E8 C8 35 FF FF     call        0000000140EC09A0
  0000000140ECD3D8: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD3DD: E8 DE 11 00 00     call        0000000140ECE5C0
  0000000140ECD3E2: EB 34              jmp         0000000140ECD418
  0000000140ECD3E4: C6 44 24 21 00     mov         byte ptr [rsp+21h],0
  0000000140ECD3E9: 48 8D 54 24 21     lea         rdx,[rsp+21h]
  0000000140ECD3EE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD3F3: E8 A8 35 FF FF     call        0000000140EC09A0
  0000000140ECD3F8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECD3FD: 48 8B 80 50 01 00  mov         rax,qword ptr [rax+0000000000000150h]
                    00
  0000000140ECD404: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD409: 48 8B 89 50 01 00  mov         rcx,qword ptr [rcx+0000000000000150h]
                    00
  0000000140ECD410: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ECD413: 33 D2              xor         edx,edx
  0000000140ECD415: FF 50 68           call        qword ptr [rax+68h]
  0000000140ECD418: 48 83 C4 38        add         rsp,38h
  0000000140ECD41C: C3                 ret

  Summary

     1E02000 .text
