Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014010A400: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014010A405: 53                 push        rbx
  000000014010A406: 48 81 EC D0 00 00  sub         rsp,0D0h
                    00
  000000014010A40D: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014010A419: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A421: 48 83 C2 58        add         rdx,58h
  000000014010A425: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014010A42A: FF 15 A8 B5 CF 01  call        qword ptr [0000000141E059D8h]
  000000014010A430: 90                 nop
  000000014010A431: 48 8D 44 24 58     lea         rax,[rsp+58h]
  000000014010A436: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014010A43B: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000014010A440: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014010A445: FF 15 8D B5 CF 01  call        qword ptr [0000000141E059D8h]
  000000014010A44B: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  000000014010A453: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A45B: 48 8B 40 38        mov         rax,qword ptr [rax+38h]
  000000014010A45F: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A467: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  000000014010A46B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014010A46E: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014010A476: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  000000014010A47C: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  000000014010A480: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014010A489: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A491: 0F B6 80 E9 00 00  movzx       eax,byte ptr [rax+00000000000000E9h]
                    00
  000000014010A498: 85 C0              test        eax,eax
  000000014010A49A: 74 0F              je          000000014010A4AB
  000000014010A49C: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A4A4: C6 80 E8 00 00 00  mov         byte ptr [rax+00000000000000E8h],1
                    01
  000000014010A4AB: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  000000014010A4B0: 0F 8D 36 01 00 00  jge         000000014010A5EC
  000000014010A4B6: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A4BE: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014010A4C2: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000014010A4C7: 48 8B 89 80 00 00  mov         rcx,qword ptr [rcx+0000000000000080h]
                    00
  000000014010A4CE: FF 15 6C BC CF 01  call        qword ptr [0000000141E06140h]
  000000014010A4D4: 90                 nop
  000000014010A4D5: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014010A4DB: 33 D2              xor         edx,edx
  000000014010A4DD: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014010A4E2: FF 15 A8 9B CF 01  call        qword ptr [0000000141E04090h]
  000000014010A4E8: 89 44 24 4C        mov         dword ptr [rsp+4Ch],eax
  000000014010A4EC: 48 8D 44 24 68     lea         rax,[rsp+68h]
  000000014010A4F1: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000014010A4F6: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000014010A4FB: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014010A500: FF 15 D2 B4 CF 01  call        qword ptr [0000000141E059D8h]
  000000014010A506: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  000000014010A50E: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A516: 48 8B 40 38        mov         rax,qword ptr [rax+38h]
  000000014010A51A: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A522: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  000000014010A526: 48 8B 00           mov         rax,qword ptr [rax]
  000000014010A529: 41 B1 01           mov         r9b,1
  000000014010A52C: 44 8B 44 24 4C     mov         r8d,dword ptr [rsp+4Ch]
  000000014010A531: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014010A539: FF 90 F8 00 00 00  call        qword ptr [rax+00000000000000F8h]
  000000014010A53F: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014010A544: 48 8D 44 24 78     lea         rax,[rsp+78h]
  000000014010A549: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014010A551: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A559: 48 83 C2 48        add         rdx,48h
  000000014010A55D: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014010A565: FF 15 6D B4 CF 01  call        qword ptr [0000000141E059D8h]
  000000014010A56B: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  000000014010A573: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014010A57B: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  000000014010A583: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A58B: 48 83 C1 78        add         rcx,78h
  000000014010A58F: 33 D2              xor         edx,edx
  000000014010A591: E8 2A AB F1 FF     call        00000001400250C0
  000000014010A596: 4C 8B D8           mov         r11,rax
  000000014010A599: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014010A5A1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014010A5A6: 41 8B 03           mov         eax,dword ptr [r11]
  000000014010A5A9: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014010A5AD: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A5B5: 44 8B 48 70        mov         r9d,dword ptr [rax+70h]
  000000014010A5B9: 4C 8B 84 24 E0 00  mov         r8,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A5C1: 4D 8B 80 B0 00 00  mov         r8,qword ptr [r8+00000000000000B0h]
                    00
  000000014010A5C8: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  000000014010A5CD: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A5D5: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  000000014010A5D9: E8 A2 96 FC FF     call        00000001400D3C80
  000000014010A5DE: 90                 nop
  000000014010A5DF: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014010A5E4: FF 15 F6 B3 CF 01  call        qword ptr [0000000141E059E0h]
  000000014010A5EA: EB 2A              jmp         000000014010A616
  000000014010A5EC: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A5F4: 48 8B 40 38        mov         rax,qword ptr [rax+38h]
  000000014010A5F8: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A600: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  000000014010A604: 48 8B 00           mov         rax,qword ptr [rax]
  000000014010A607: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  000000014010A60B: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  000000014010A611: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014010A616: 48 8B 9C 24 E0 00  mov         rbx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A61E: 48 83 C3 70        add         rbx,70h
  000000014010A622: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A62A: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  000000014010A62E: E8 2D 56 FF FF     call        00000001400FFC60
  000000014010A633: 48 8B C8           mov         rcx,rax
  000000014010A636: E8 05 B6 FA FF     call        00000001400B5C40
  000000014010A63B: 48 8B C8           mov         rcx,rax
  000000014010A63E: E8 6D B2 F9 FF     call        00000001400A58B0
  000000014010A643: 48 8B D3           mov         rdx,rbx
  000000014010A646: 48 8B C8           mov         rcx,rax
  000000014010A649: E8 72 D7 F3 FF     call        0000000140047DC0
  000000014010A64E: 48 8B C8           mov         rcx,rax
  000000014010A651: E8 1A 51 F5 FF     call        000000014005F770
  000000014010A656: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014010A65B: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014010A660: E8 3B 1A B7 00     call        0000000140C7C0A0
  000000014010A665: 0F B6 C0           movzx       eax,al
  000000014010A668: 85 C0              test        eax,eax
  000000014010A66A: 74 4C              je          000000014010A6B8
  000000014010A66C: 48 8D 84 24 88 00  lea         rax,[rsp+0000000000000088h]
                    00 00
  000000014010A674: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  000000014010A67C: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000014010A681: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014010A689: FF 15 49 B3 CF 01  call        qword ptr [0000000141E059D8h]
  000000014010A68F: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  000000014010A697: 4C 8B 4C 24 30     mov         r9,qword ptr [rsp+30h]
  000000014010A69C: 4C 8B 84 24 C0 00  mov         r8,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014010A6A4: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  000000014010A6A9: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014010A6B1: E8 3A 00 00 00     call        000000014010A6F0
  000000014010A6B6: EB 13              jmp         000000014010A6CB
  000000014010A6B8: 41 B8 02 00 00 00  mov         r8d,2
  000000014010A6BE: 33 D2              xor         edx,edx
  000000014010A6C0: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014010A6C5: E8 26 CF 25 00     call        00000001403675F0
  000000014010A6CA: 90                 nop
  000000014010A6CB: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014010A6D0: FF 15 0A B3 CF 01  call        qword ptr [0000000141E059E0h]
  000000014010A6D6: 48 81 C4 D0 00 00  add         rsp,0D0h
                    00
  000000014010A6DD: 5B                 pop         rbx
  000000014010A6DE: C3                 ret

  Summary

     1E02000 .text
