Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D7B4A0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D7B4A5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D7B4AA: 56                 push        rsi
  0000000140D7B4AB: 57                 push        rdi
  0000000140D7B4AC: 48 81 EC F8 00 00  sub         rsp,0F8h
                    00
  0000000140D7B4B3: 48 C7 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D7B4BF: 48 8B 84 24 10 01  mov         rax,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B4C7: 48 83 78 50 00     cmp         qword ptr [rax+50h],0
  0000000140D7B4CC: 75 05              jne         0000000140D7B4D3
  0000000140D7B4CE: E9 91 02 00 00     jmp         0000000140D7B764
  0000000140D7B4D3: 4C 8B 84 24 18 01  mov         r8,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140D7B4DB: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140D7B4E0: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B4E8: FF 15 C2 01 09 01  call        qword ptr [0000000141E0B6B0h]
  0000000140D7B4EE: 90                 nop
  0000000140D7B4EF: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140D7B4F8: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  0000000140D7B500: 4C 8D 4C 24 20     lea         r9,[rsp+20h]
  0000000140D7B505: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  0000000140D7B50A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140D7B50F: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B517: E8 44 F9 FF FF     call        0000000140D7AE60
  0000000140D7B51C: 89 44 24 70        mov         dword ptr [rsp+70h],eax
  0000000140D7B520: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140D7B526: 75 11              jne         0000000140D7B539
  0000000140D7B528: 48 8B 84 24 10 01  mov         rax,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B530: 48 8B 40 50        mov         rax,qword ptr [rax+50h]
  0000000140D7B534: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D7B539: 83 7C 24 70 00     cmp         dword ptr [rsp+70h],0
  0000000140D7B53E: 7F 10              jg          0000000140D7B550
  0000000140D7B540: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140D7B545: FF 15 95 89 08 01  call        qword ptr [0000000141E03EE0h]
  0000000140D7B54B: E9 14 02 00 00     jmp         0000000140D7B764
  0000000140D7B550: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140D7B558: FF 15 5A 01 09 01  call        qword ptr [0000000141E0B6B8h]
  0000000140D7B55E: 89 44 24 6C        mov         dword ptr [rsp+6Ch],eax
  0000000140D7B562: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140D7B567: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140D7B56C: E8 5F 7D F0 FF     call        0000000140C832D0
  0000000140D7B571: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D7B576: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140D7B57C: 75 10              jne         0000000140D7B58E
  0000000140D7B57E: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140D7B583: FF 15 57 89 08 01  call        qword ptr [0000000141E03EE0h]
  0000000140D7B589: E9 D6 01 00 00     jmp         0000000140D7B764
  0000000140D7B58E: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140D7B594: 74 1A              je          0000000140D7B5B0
  0000000140D7B596: 83 7C 24 20 00     cmp         dword ptr [rsp+20h],0
  0000000140D7B59B: 7C 13              jl          0000000140D7B5B0
  0000000140D7B59D: 44 8B 44 24 6C     mov         r8d,dword ptr [rsp+6Ch]
  0000000140D7B5A2: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140D7B5A6: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D7B5AB: E8 60 36 47 00     call        00000001411EEC10
  0000000140D7B5B0: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B5B8: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D7B5BC: E8 3F F8 EF FF     call        0000000140C7AE00
  0000000140D7B5C1: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140D7B5C6: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  0000000140D7B5CC: 74 1F              je          0000000140D7B5ED
  0000000140D7B5CE: 83 7C 24 20 00     cmp         dword ptr [rsp+20h],0
  0000000140D7B5D3: 7C 18              jl          0000000140D7B5ED
  0000000140D7B5D5: 44 8B 4C 24 6C     mov         r9d,dword ptr [rsp+6Ch]
  0000000140D7B5DA: 44 8B 44 24 20     mov         r8d,dword ptr [rsp+20h]
  0000000140D7B5DF: 8B 54 24 70        mov         edx,dword ptr [rsp+70h]
  0000000140D7B5E3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D7B5E8: E8 63 3E F8 FF     call        0000000140CFF450
  0000000140D7B5ED: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140D7B5F2: FF 15 90 A3 08 01  call        qword ptr [0000000141E05988h]
  0000000140D7B5F8: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  0000000140D7B5FC: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140D7B601: FF 15 C9 A3 08 01  call        qword ptr [0000000141E059D0h]
  0000000140D7B607: 89 44 24 58        mov         dword ptr [rsp+58h],eax
  0000000140D7B60B: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000140D7B614: 83 7C 24 58 02     cmp         dword ptr [rsp+58h],2
  0000000140D7B619: 0F 85 8F 00 00 00  jne         0000000140D7B6AE
  0000000140D7B61F: 41 B9 01 00 00 00  mov         r9d,1
  0000000140D7B625: 44 8B 44 24 68     mov         r8d,dword ptr [rsp+68h]
  0000000140D7B62A: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140D7B62F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140D7B634: FF 15 96 9A 08 01  call        qword ptr [0000000141E050D0h]
  0000000140D7B63A: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140D7B642: 48 8D BC 24 C0 00  lea         rdi,[rsp+00000000000000C0h]
                    00 00
  0000000140D7B64A: 48 8B B4 24 E0 00  mov         rsi,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140D7B652: B9 18 00 00 00     mov         ecx,18h
  0000000140D7B657: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D7B659: 48 8D 7C 24 30     lea         rdi,[rsp+30h]
  0000000140D7B65E: 48 8D B4 24 C0 00  lea         rsi,[rsp+00000000000000C0h]
                    00 00
  0000000140D7B666: B9 18 00 00 00     mov         ecx,18h
  0000000140D7B66B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D7B66D: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140D7B672: FF 15 68 88 08 01  call        qword ptr [0000000141E03EE0h]
  0000000140D7B678: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140D7B67D: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B685: FF 15 3D A5 08 01  call        qword ptr [0000000141E05BC8h]
  0000000140D7B68B: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140D7B690: 33 D2              xor         edx,edx
  0000000140D7B692: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D7B697: E8 34 35 47 00     call        00000001411EEBD0
  0000000140D7B69C: 8B D0              mov         edx,eax
  0000000140D7B69E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D7B6A3: FF 15 97 DB 08 01  call        qword ptr [0000000141E09240h]
  0000000140D7B6A9: E9 9E 00 00 00     jmp         0000000140D7B74C
  0000000140D7B6AE: 83 7C 24 58 01     cmp         dword ptr [rsp+58h],1
  0000000140D7B6B3: 0F 85 93 00 00 00  jne         0000000140D7B74C
  0000000140D7B6B9: 41 B9 02 00 00 00  mov         r9d,2
  0000000140D7B6BF: 44 8B 44 24 68     mov         r8d,dword ptr [rsp+68h]
  0000000140D7B6C4: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140D7B6CC: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140D7B6D1: FF 15 F9 99 08 01  call        qword ptr [0000000141E050D0h]
  0000000140D7B6D7: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000140D7B6DF: 48 8D BC 24 A8 00  lea         rdi,[rsp+00000000000000A8h]
                    00 00
  0000000140D7B6E7: 48 8B B4 24 E8 00  mov         rsi,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140D7B6EF: B9 18 00 00 00     mov         ecx,18h
  0000000140D7B6F4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D7B6F6: 48 8D 7C 24 30     lea         rdi,[rsp+30h]
  0000000140D7B6FB: 48 8D B4 24 A8 00  lea         rsi,[rsp+00000000000000A8h]
                    00 00
  0000000140D7B703: B9 18 00 00 00     mov         ecx,18h
  0000000140D7B708: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D7B70A: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140D7B712: FF 15 C8 87 08 01  call        qword ptr [0000000141E03EE0h]
  0000000140D7B718: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140D7B71D: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B725: FF 15 9D A4 08 01  call        qword ptr [0000000141E05BC8h]
  0000000140D7B72B: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140D7B730: BA 01 00 00 00     mov         edx,1
  0000000140D7B735: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D7B73A: E8 91 34 47 00     call        00000001411EEBD0
  0000000140D7B73F: 8B D0              mov         edx,eax
  0000000140D7B741: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D7B746: FF 15 F4 DA 08 01  call        qword ptr [0000000141E09240h]
  0000000140D7B74C: 48 8D 0D 65 AF FF  lea         rcx,[0000000143D766B8h]
                    02
  0000000140D7B753: E8 38 F1 54 FF     call        00000001402CA890
  0000000140D7B758: 90                 nop
  0000000140D7B759: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140D7B75E: FF 15 7C 87 08 01  call        qword ptr [0000000141E03EE0h]
  0000000140D7B764: 48 81 C4 F8 00 00  add         rsp,0F8h
                    00
  0000000140D7B76B: 5F                 pop         rdi
  0000000140D7B76C: 5E                 pop         rsi
  0000000140D7B76D: C3                 ret

  Summary

     1E02000 .text
