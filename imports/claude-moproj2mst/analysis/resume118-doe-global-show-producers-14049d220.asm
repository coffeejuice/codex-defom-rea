Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014049D220: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014049D225: 56                 push        rsi
  000000014049D226: 57                 push        rdi
  000000014049D227: 48 81 EC 88 02 00  sub         rsp,288h
                    00
  000000014049D22E: 48 C7 84 24 60 02  mov         qword ptr [rsp+0000000000000260h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014049D23A: 33 D2              xor         edx,edx
  000000014049D23C: 48 8B 8C 24 A0 02  mov         rcx,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D244: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  000000014049D248: E8 A3 EF 3C 00     call        000000014086C1F0
  000000014049D24D: 0F B6 C0           movzx       eax,al
  000000014049D250: 85 C0              test        eax,eax
  000000014049D252: 0F 85 BC 00 00 00  jne         000000014049D314
  000000014049D258: 48 8B 8C 24 A0 02  mov         rcx,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D260: 48 81 C1 80 00 00  add         rcx,80h
                    00
  000000014049D267: FF 15 5B 6D 96 01  call        qword ptr [0000000141E03FC8h]
  000000014049D26D: 0F B6 C0           movzx       eax,al
  000000014049D270: 85 C0              test        eax,eax
  000000014049D272: 0F 85 9C 00 00 00  jne         000000014049D314
  000000014049D278: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014049D27D: E8 8E 9A 45 00     call        00000001408F6D10
  000000014049D282: 90                 nop
  000000014049D283: C6 84 24 A0 00 00  mov         byte ptr [rsp+00000000000000A0h],0
                    00 00
  000000014049D28B: 48 8B 8C 24 A0 02  mov         rcx,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D293: 48 81 C1 80 00 00  add         rcx,80h
                    00
  000000014049D29A: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  000000014049D2A2: FF 15 A8 6C 96 01  call        qword ptr [0000000141E03F50h]
  000000014049D2A8: 48 89 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],rax
                    00 00
  000000014049D2B0: 48 8B 84 24 68 02  mov         rax,qword ptr [rsp+0000000000000268h]
                    00 00
  000000014049D2B8: 48 89 84 24 70 02  mov         qword ptr [rsp+0000000000000270h],rax
                    00 00
  000000014049D2C0: 48 8B 8C 24 70 02  mov         rcx,qword ptr [rsp+0000000000000270h]
                    00 00
  000000014049D2C8: FF 15 5A 6F 96 01  call        qword ptr [0000000141E04228h]
  000000014049D2CE: 48 8B D0           mov         rdx,rax
  000000014049D2D1: 48 8B 84 24 A0 02  mov         rax,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D2D9: 48 8B 40 48        mov         rax,qword ptr [rax+48h]
  000000014049D2DD: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014049D2E2: 41 B9 02 00 00 00  mov         r9d,2
  000000014049D2E8: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  000000014049D2F0: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014049D2F5: E8 86 9C 45 00     call        00000001408F6F80
  000000014049D2FA: 90                 nop
  000000014049D2FB: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014049D303: FF 15 87 6F 96 01  call        qword ptr [0000000141E04290h]
  000000014049D309: 90                 nop
  000000014049D30A: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014049D30F: E8 3C 9A 45 00     call        00000001408F6D50
  000000014049D314: 48 8B 84 24 A0 02  mov         rax,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D31C: C6 80 88 00 00 00  mov         byte ptr [rax+0000000000000088h],1
                    01
  000000014049D323: C6 84 24 40 01 00  mov         byte ptr [rsp+0000000000000140h],0
                    00 00
  000000014049D32B: 48 8B 84 24 A0 02  mov         rax,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D333: 48 8B 00           mov         rax,qword ptr [rax]
  000000014049D336: 48 8B 8C 24 A0 02  mov         rcx,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D33E: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  000000014049D344: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  000000014049D34C: 4C 8D 84 24 40 01  lea         r8,[rsp+0000000000000140h]
                    00 00
  000000014049D354: 48 8D 15 21 8B F4  lea         rdx,[00000001423E5E7Ch]
                    01
  000000014049D35B: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  000000014049D363: E8 18 7D B8 FF     call        0000000140025080
  000000014049D368: 48 8D BC 24 50 02  lea         rdi,[rsp+0000000000000250h]
                    00 00
  000000014049D370: 48 8B F0           mov         rsi,rax
  000000014049D373: B9 10 00 00 00     mov         ecx,10h
  000000014049D378: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D37A: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  000000014049D382: 48 8D B4 24 50 02  lea         rsi,[rsp+0000000000000250h]
                    00 00
  000000014049D38A: B9 10 00 00 00     mov         ecx,10h
  000000014049D38F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D391: 4C 8D 84 24 68 01  lea         r8,[rsp+0000000000000168h]
                    00 00
  000000014049D399: 48 8D 15 E8 8A F4  lea         rdx,[00000001423E5E88h]
                    01
  000000014049D3A0: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  000000014049D3A8: E8 D3 7C B8 FF     call        0000000140025080
  000000014049D3AD: 48 8D BC 24 40 02  lea         rdi,[rsp+0000000000000240h]
                    00 00
  000000014049D3B5: 48 8B F0           mov         rsi,rax
  000000014049D3B8: B9 10 00 00 00     mov         ecx,10h
  000000014049D3BD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D3BF: 48 8D BC 24 48 01  lea         rdi,[rsp+0000000000000148h]
                    00 00
  000000014049D3C7: 48 8D B4 24 40 02  lea         rsi,[rsp+0000000000000240h]
                    00 00
  000000014049D3CF: B9 10 00 00 00     mov         ecx,10h
  000000014049D3D4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D3D6: 4C 8B 84 24 A0 02  mov         r8,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D3DE: 49 83 C0 48        add         r8,48h
  000000014049D3E2: 48 8D 15 AF 8A F4  lea         rdx,[00000001423E5E98h]
                    01
  000000014049D3E9: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000014049D3F1: E8 8A 7C B8 FF     call        0000000140025080
  000000014049D3F6: 48 8D BC 24 30 02  lea         rdi,[rsp+0000000000000230h]
                    00 00
  000000014049D3FE: 48 8B F0           mov         rsi,rax
  000000014049D401: B9 10 00 00 00     mov         ecx,10h
  000000014049D406: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D408: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  000000014049D410: 48 8D B4 24 30 02  lea         rsi,[rsp+0000000000000230h]
                    00 00
  000000014049D418: B9 10 00 00 00     mov         ecx,10h
  000000014049D41D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D41F: 45 33 C0           xor         r8d,r8d
  000000014049D422: 33 D2              xor         edx,edx
  000000014049D424: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000014049D42C: FF 15 36 6D 96 01  call        qword ptr [0000000141E04168h]
  000000014049D432: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  000000014049D43A: 48 8B F0           mov         rsi,rax
  000000014049D43D: B9 10 00 00 00     mov         ecx,10h
  000000014049D442: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D444: 45 33 C0           xor         r8d,r8d
  000000014049D447: 33 D2              xor         edx,edx
  000000014049D449: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000014049D451: FF 15 11 6D 96 01  call        qword ptr [0000000141E04168h]
  000000014049D457: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  000000014049D45F: 48 8B F0           mov         rsi,rax
  000000014049D462: B9 10 00 00 00     mov         ecx,10h
  000000014049D467: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D469: 45 33 C0           xor         r8d,r8d
  000000014049D46C: 33 D2              xor         edx,edx
  000000014049D46E: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  000000014049D476: FF 15 EC 6C 96 01  call        qword ptr [0000000141E04168h]
  000000014049D47C: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  000000014049D484: 48 8B F0           mov         rsi,rax
  000000014049D487: B9 10 00 00 00     mov         ecx,10h
  000000014049D48C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D48E: 45 33 C0           xor         r8d,r8d
  000000014049D491: 33 D2              xor         edx,edx
  000000014049D493: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  000000014049D49B: FF 15 C7 6C 96 01  call        qword ptr [0000000141E04168h]
  000000014049D4A1: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  000000014049D4A9: 48 8B F0           mov         rsi,rax
  000000014049D4AC: B9 10 00 00 00     mov         ecx,10h
  000000014049D4B1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D4B3: 45 33 C0           xor         r8d,r8d
  000000014049D4B6: 33 D2              xor         edx,edx
  000000014049D4B8: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000014049D4C0: FF 15 A2 6C 96 01  call        qword ptr [0000000141E04168h]
  000000014049D4C6: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  000000014049D4CE: 48 8B F0           mov         rsi,rax
  000000014049D4D1: B9 10 00 00 00     mov         ecx,10h
  000000014049D4D6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D4D8: 45 33 C0           xor         r8d,r8d
  000000014049D4DB: 33 D2              xor         edx,edx
  000000014049D4DD: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  000000014049D4E5: FF 15 7D 6C 96 01  call        qword ptr [0000000141E04168h]
  000000014049D4EB: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  000000014049D4F3: 48 8B F0           mov         rsi,rax
  000000014049D4F6: B9 10 00 00 00     mov         ecx,10h
  000000014049D4FB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D4FD: 45 33 C0           xor         r8d,r8d
  000000014049D500: 33 D2              xor         edx,edx
  000000014049D502: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000014049D50A: FF 15 58 6C 96 01  call        qword ptr [0000000141E04168h]
  000000014049D510: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  000000014049D518: 48 8B F0           mov         rsi,rax
  000000014049D51B: B9 10 00 00 00     mov         ecx,10h
  000000014049D520: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D522: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  000000014049D52A: 48 8D B4 24 20 01  lea         rsi,[rsp+0000000000000120h]
                    00 00
  000000014049D532: B9 10 00 00 00     mov         ecx,10h
  000000014049D537: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D539: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  000000014049D541: 48 8D B4 24 48 01  lea         rsi,[rsp+0000000000000148h]
                    00 00
  000000014049D549: B9 10 00 00 00     mov         ecx,10h
  000000014049D54E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D550: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  000000014049D558: 48 8D B4 24 70 01  lea         rsi,[rsp+0000000000000170h]
                    00 00
  000000014049D560: B9 10 00 00 00     mov         ecx,10h
  000000014049D565: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D567: 48 8D 84 24 20 02  lea         rax,[rsp+0000000000000220h]
                    00 00
  000000014049D56F: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014049D574: 48 8D 84 24 10 02  lea         rax,[rsp+0000000000000210h]
                    00 00
  000000014049D57C: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014049D581: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  000000014049D589: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014049D58E: 48 8D 84 24 F0 01  lea         rax,[rsp+00000000000001F0h]
                    00 00
  000000014049D596: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014049D59B: 48 8D 84 24 E0 01  lea         rax,[rsp+00000000000001E0h]
                    00 00
  000000014049D5A3: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014049D5A8: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  000000014049D5B0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014049D5B5: 48 8D 84 24 C0 01  lea         rax,[rsp+00000000000001C0h]
                    00 00
  000000014049D5BD: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014049D5C2: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  000000014049D5CA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014049D5CF: 4C 8D 8C 24 A0 01  lea         r9,[rsp+00000000000001A0h]
                    00 00
  000000014049D5D7: 4C 8D 84 24 90 01  lea         r8,[rsp+0000000000000190h]
                    00 00
  000000014049D5DF: 48 8D 15 C2 88 F4  lea         rdx,[00000001423E5EA8h]
                    01
  000000014049D5E6: 48 8D 0D 13 91 8D  lea         rcx,[0000000143D76700h]
                    03
  000000014049D5ED: E8 CE 1D E3 FF     call        00000001402CF3C0
  000000014049D5F2: 48 81 C4 88 02 00  add         rsp,288h
                    00
  000000014049D5F9: 5F                 pop         rdi
  000000014049D5FA: 5E                 pop         rsi
  000000014049D5FB: C3                 ret

  Summary

     1E02000 .text
