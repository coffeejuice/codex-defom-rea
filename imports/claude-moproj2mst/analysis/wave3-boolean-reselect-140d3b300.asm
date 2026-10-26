Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D3B300: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D3B305: 48 81 EC F8 00 00  sub         rsp,0F8h
                    00
  0000000140D3B30C: 48 C7 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D3B318: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D3B320: FF 15 BA 04 0D 01  call        qword ptr [0000000141E0B7E0h]
  0000000140D3B326: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D3B32B: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140D3B331: 75 05              jne         0000000140D3B338
  0000000140D3B333: E9 AD 01 00 00     jmp         0000000140D3B4E5
  0000000140D3B338: C6 44 24 38 00     mov         byte ptr [rsp+38h],0
  0000000140D3B33D: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D3B345: FF 15 75 EA 0C 01  call        qword ptr [0000000141E09DC0h]
  0000000140D3B34B: 85 C0              test        eax,eax
  0000000140D3B34D: 75 05              jne         0000000140D3B354
  0000000140D3B34F: C6 44 24 38 01     mov         byte ptr [rsp+38h],1
  0000000140D3B354: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140D3B359: 85 C0              test        eax,eax
  0000000140D3B35B: 74 23              je          0000000140D3B380
  0000000140D3B35D: BA 01 00 00 00     mov         edx,1
  0000000140D3B362: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D3B36A: FF 15 A8 04 0D 01  call        qword ptr [0000000141E0B818h]
  0000000140D3B370: B2 01              mov         dl,1
  0000000140D3B372: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D3B37A: FF 15 48 8D 0C 01  call        qword ptr [0000000141E040C8h]
  0000000140D3B380: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140D3B385: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D3B38D: FF 15 B5 A6 0C 01  call        qword ptr [0000000141E05A48h]
  0000000140D3B393: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140D3B39B: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140D3B3A3: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140D3B3AB: 4C 8B 54 24 30     mov         r10,qword ptr [rsp+30h]
  0000000140D3B3B0: 4D 8B 12           mov         r10,qword ptr [r10]
  0000000140D3B3B3: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140D3B3BB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D3B3C0: 45 33 C9           xor         r9d,r9d
  0000000140D3B3C3: 45 33 C0           xor         r8d,r8d
  0000000140D3B3C6: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  0000000140D3B3CB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D3B3D0: 41 FF 52 58        call        qword ptr [r10+58h]
  0000000140D3B3D4: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140D3B3DC: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D3B3E4: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140D3B3EC: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D3B3F4: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D3B3FC: FF 15 EE 03 0D 01  call        qword ptr [0000000141E0B7F0h]
  0000000140D3B402: 90                 nop
  0000000140D3B403: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140D3B408: FF 15 D2 8A 0C 01  call        qword ptr [0000000141E03EE0h]
  0000000140D3B40E: 90                 nop
  0000000140D3B40F: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140D3B414: FF 15 C6 8A 0C 01  call        qword ptr [0000000141E03EE0h]
  0000000140D3B41A: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140D3B41F: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D3B427: FF 15 1B A6 0C 01  call        qword ptr [0000000141E05A48h]
  0000000140D3B42D: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140D3B435: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140D3B43D: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000140D3B445: 4C 8B 54 24 30     mov         r10,qword ptr [rsp+30h]
  0000000140D3B44A: 4D 8B 12           mov         r10,qword ptr [r10]
  0000000140D3B44D: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140D3B455: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D3B45A: 45 33 C9           xor         r9d,r9d
  0000000140D3B45D: 45 33 C0           xor         r8d,r8d
  0000000140D3B460: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000140D3B468: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D3B46D: 41 FF 52 58        call        qword ptr [r10+58h]
  0000000140D3B471: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000140D3B479: 48 8B 84 24 D8 00  mov         rax,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140D3B481: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140D3B489: 45 33 C0           xor         r8d,r8d
  0000000140D3B48C: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140D3B494: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D3B49C: E8 2F 03 00 00     call        0000000140D3B7D0
  0000000140D3B4A1: 90                 nop
  0000000140D3B4A2: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140D3B4AA: FF 15 30 8A 0C 01  call        qword ptr [0000000141E03EE0h]
  0000000140D3B4B0: 90                 nop
  0000000140D3B4B1: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D3B4B6: FF 15 24 8A 0C 01  call        qword ptr [0000000141E03EE0h]
  0000000140D3B4BC: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140D3B4C1: 85 C0              test        eax,eax
  0000000140D3B4C3: 74 20              je          0000000140D3B4E5
  0000000140D3B4C5: 33 D2              xor         edx,edx
  0000000140D3B4C7: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D3B4CF: FF 15 43 03 0D 01  call        qword ptr [0000000141E0B818h]
  0000000140D3B4D5: 33 D2              xor         edx,edx
  0000000140D3B4D7: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D3B4DF: FF 15 E3 8B 0C 01  call        qword ptr [0000000141E040C8h]
  0000000140D3B4E5: 48 81 C4 F8 00 00  add         rsp,0F8h
                    00
  0000000140D3B4EC: C3                 ret

  Summary

     1E02000 .text
