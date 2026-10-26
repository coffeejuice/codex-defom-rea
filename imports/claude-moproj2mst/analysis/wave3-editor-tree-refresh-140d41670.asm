Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D41670: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D41675: 56                 push        rsi
  0000000140D41676: 57                 push        rdi
  0000000140D41677: 48 81 EC D8 00 00  sub         rsp,0D8h
                    00
  0000000140D4167E: 48 C7 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D4168A: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140D41693: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  0000000140D4169C: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140D416A1: FF 15 29 42 0C 01  call        qword ptr [0000000141E058D0h]
  0000000140D416A7: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140D416AF: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D416B7: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140D416BF: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D416C7: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D416CA: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D416D2: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D416DA: FF 50 68           call        qword ptr [rax+68h]
  0000000140D416DD: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000140D416E1: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140D416E6: FF 15 F4 27 0C 01  call        qword ptr [0000000141E03EE0h]
  0000000140D416EC: C7 44 24 54 00 00  mov         dword ptr [rsp+54h],0
                    00 00
  0000000140D416F4: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140D416F9: FF 15 D1 41 0C 01  call        qword ptr [0000000141E058D0h]
  0000000140D416FF: 90                 nop
  0000000140D41700: C6 44 24 38 00     mov         byte ptr [rsp+38h],0
  0000000140D41705: C6 44 24 50 00     mov         byte ptr [rsp+50h],0
  0000000140D4170A: C7 44 24 58 00 00  mov         dword ptr [rsp+58h],0
                    00 00
  0000000140D41712: EB 0B              jmp         0000000140D4171F
  0000000140D41714: 8B 44 24 58        mov         eax,dword ptr [rsp+58h]
  0000000140D41718: 83 C0 01           add         eax,1
  0000000140D4171B: 89 44 24 58        mov         dword ptr [rsp+58h],eax
  0000000140D4171F: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140D41723: 39 44 24 58        cmp         dword ptr [rsp+58h],eax
  0000000140D41727: 0F 8D FD 00 00 00  jge         0000000140D4182A
  0000000140D4172D: 45 33 C0           xor         r8d,r8d
  0000000140D41730: 8B 54 24 58        mov         edx,dword ptr [rsp+58h]
  0000000140D41734: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D4173C: FF 15 16 9F 0C 01  call        qword ptr [0000000141E0B658h]
  0000000140D41742: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D41747: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140D4174D: 0F 84 D2 00 00 00  je          0000000140D41825
  0000000140D41753: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140D41758: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D41760: E8 BB FC FF FF     call        0000000140D41420
  0000000140D41765: 88 44 24 50        mov         byte ptr [rsp+50h],al
  0000000140D41769: 0F B6 44 24 50     movzx       eax,byte ptr [rsp+50h]
  0000000140D4176E: 85 C0              test        eax,eax
  0000000140D41770: 74 05              je          0000000140D41777
  0000000140D41772: E9 B3 00 00 00     jmp         0000000140D4182A
  0000000140D41777: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D4177C: FF 15 5E 9D 0C 01  call        qword ptr [0000000141E0B4E0h]
  0000000140D41782: 89 44 24 54        mov         dword ptr [rsp+54h],eax
  0000000140D41786: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  0000000140D4178B: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140D41790: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D41798: FF 15 12 9F 0C 01  call        qword ptr [0000000141E0B6B0h]
  0000000140D4179E: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140D417A6: 48 8D BC 24 98 00  lea         rdi,[rsp+0000000000000098h]
                    00 00
  0000000140D417AE: 48 8B B4 24 C8 00  mov         rsi,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140D417B6: B9 18 00 00 00     mov         ecx,18h
  0000000140D417BB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D417BD: 48 8D 7C 24 20     lea         rdi,[rsp+20h]
  0000000140D417C2: 48 8D B4 24 98 00  lea         rsi,[rsp+0000000000000098h]
                    00 00
  0000000140D417CA: B9 18 00 00 00     mov         ecx,18h
  0000000140D417CF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D417D1: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140D417D6: FF 15 04 27 0C 01  call        qword ptr [0000000141E03EE0h]
  0000000140D417DC: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D417E4: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D417E7: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140D417EC: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D417F4: FF 50 78           call        qword ptr [rax+78h]
  0000000140D417F7: 88 44 24 38        mov         byte ptr [rsp+38h],al
  0000000140D417FB: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140D41800: 85 C0              test        eax,eax
  0000000140D41802: 74 21              je          0000000140D41825
  0000000140D41804: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140D41809: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D41811: E8 4A 00 00 00     call        0000000140D41860
  0000000140D41816: 88 44 24 50        mov         byte ptr [rsp+50h],al
  0000000140D4181A: 0F B6 44 24 50     movzx       eax,byte ptr [rsp+50h]
  0000000140D4181F: 85 C0              test        eax,eax
  0000000140D41821: 74 02              je          0000000140D41825
  0000000140D41823: EB 05              jmp         0000000140D4182A
  0000000140D41825: E9 EA FE FF FF     jmp         0000000140D41714
  0000000140D4182A: 0F B6 44 24 50     movzx       eax,byte ptr [rsp+50h]
  0000000140D4182F: 88 84 24 90 00 00  mov         byte ptr [rsp+0000000000000090h],al
                    00
  0000000140D41836: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140D4183B: FF 15 9F 26 0C 01  call        qword ptr [0000000141E03EE0h]
  0000000140D41841: 8A 84 24 90 00 00  mov         al,byte ptr [rsp+0000000000000090h]
                    00
  0000000140D41848: 48 81 C4 D8 00 00  add         rsp,0D8h
                    00
  0000000140D4184F: 5F                 pop         rdi
  0000000140D41850: 5E                 pop         rsi
  0000000140D41851: C3                 ret

  Summary

     1E02000 .text
