Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EB7F50: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EB7F55: 48 83 EC 68        sub         rsp,68h
  0000000140EB7F59: 48 C7 44 24 50 FE  mov         qword ptr [rsp+50h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140EB7F62: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140EB7F67: FF 15 53 C5 F4 00  call        qword ptr [0000000141E044C0h]
  0000000140EB7F6D: 90                 nop
  0000000140EB7F6E: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140EB7F73: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB7F7A: BA 01 00 00 00     mov         edx,1
  0000000140EB7F7F: 48 8B 89 C0 11 00  mov         rcx,qword ptr [rcx+00000000000011C0h]
                    00
  0000000140EB7F86: FF 15 CC E3 F4 00  call        qword ptr [0000000141E06358h]
  0000000140EB7F8C: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140EB7F91: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB7F98: B2 01              mov         dl,1
  0000000140EB7F9A: 48 8B 89 C0 11 00  mov         rcx,qword ptr [rcx+00000000000011C0h]
                    00
  0000000140EB7FA1: FF 15 31 0F F5 00  call        qword ptr [0000000141E08ED8h]
  0000000140EB7FA7: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140EB7FAC: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB7FB3: BA 0A 00 00 00     mov         edx,0Ah
  0000000140EB7FB8: 48 8B 89 C0 11 00  mov         rcx,qword ptr [rcx+00000000000011C0h]
                    00
  0000000140EB7FBF: FF 15 83 E3 F4 00  call        qword ptr [0000000141E06348h]
  0000000140EB7FC5: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140EB7FCD: EB 0B              jmp         0000000140EB7FDA
  0000000140EB7FCF: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140EB7FD3: 83 C0 01           add         eax,1
  0000000140EB7FD6: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140EB7FDA: 83 7C 24 28 0A     cmp         dword ptr [rsp+28h],0Ah
  0000000140EB7FDF: 0F 8D A1 00 00 00  jge         0000000140EB8086
  0000000140EB7FE5: B9 30 00 00 00     mov         ecx,30h
  0000000140EB7FEA: E8 CD 6A 5B 00     call        000000014146EABC
  0000000140EB7FEF: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140EB7FF4: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140EB7FFA: 74 28              je          0000000140EB8024
  0000000140EB7FFC: 33 D2              xor         edx,edx
  0000000140EB7FFE: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140EB8003: FF 15 17 E4 F4 00  call        qword ptr [0000000141E06420h]
  0000000140EB8009: 4C 8B 5C 24 48     mov         r11,qword ptr [rsp+48h]
  0000000140EB800E: 48 8D 05 13 58 2D  lea         rax,[000000014218D828h]
                    01
  0000000140EB8015: 49 89 03           mov         qword ptr [r11],rax
  0000000140EB8018: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140EB801D: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140EB8022: EB 09              jmp         0000000140EB802D
  0000000140EB8024: 48 C7 44 24 58 00  mov         qword ptr [rsp+58h],0
                    00 00 00
  0000000140EB802D: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140EB8032: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140EB8037: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140EB803C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140EB8041: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140EB8046: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB804D: 4C 8B 4C 24 20     mov         r9,qword ptr [rsp+20h]
  0000000140EB8052: 45 33 C0           xor         r8d,r8d
  0000000140EB8055: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  0000000140EB8059: 48 8B 89 C0 11 00  mov         rcx,qword ptr [rcx+00000000000011C0h]
                    00
  0000000140EB8060: FF 15 C2 E3 F4 00  call        qword ptr [0000000141E06428h]
  0000000140EB8066: 44 8B 44 24 28     mov         r8d,dword ptr [rsp+28h]
  0000000140EB806B: 41 83 C0 01        add         r8d,1
  0000000140EB806F: 48 8D 15 FE 82 C5  lea         rdx,[0000000142B10374h]
                    01
  0000000140EB8076: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140EB807B: FF 15 97 BE F4 00  call        qword ptr [0000000141E03F18h]
  0000000140EB8081: E9 49 FF FF FF     jmp         0000000140EB7FCF
  0000000140EB8086: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140EB808B: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB8092: 41 B8 18 01 00 00  mov         r8d,118h
  0000000140EB8098: 33 D2              xor         edx,edx
  0000000140EB809A: 48 8B 89 C0 11 00  mov         rcx,qword ptr [rcx+00000000000011C0h]
                    00
  0000000140EB80A1: FF 15 D1 06 F5 00  call        qword ptr [0000000141E08778h]
  0000000140EB80A7: 90                 nop
  0000000140EB80A8: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140EB80AD: FF 15 2D D9 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EB80B3: 48 83 C4 68        add         rsp,68h
  0000000140EB80B7: C3                 ret

  Summary

     1E02000 .text
