Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140854200: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  0000000140854205: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000014085420A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014085420E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140854213: 48 83 EC 28        sub         rsp,28h
  0000000140854217: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  000000014085421C: 7F 05              jg          0000000140854223
  000000014085421E: E9 3D 01 00 00     jmp         0000000140854360
  0000000140854223: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  0000000140854228: 74 0E              je          0000000140854238
  000000014085422A: 83 7C 24 48 FE     cmp         dword ptr [rsp+48h],0FFFFFFFEh
  000000014085422F: 74 07              je          0000000140854238
  0000000140854231: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  0000000140854236: 75 29              jne         0000000140854261
  0000000140854238: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014085423D: 48 83 78 08 00     cmp         qword ptr [rax+8],0
  0000000140854242: 75 1D              jne         0000000140854261
  0000000140854244: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140854249: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  000000014085424D: 0F AF 08           imul        ecx,dword ptr [rax]
  0000000140854250: E8 5B 41 17 00     call        00000001409C83B0
  0000000140854255: 4C 8B D8           mov         r11,rax
  0000000140854258: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014085425D: 4C 89 58 08        mov         qword ptr [rax+8],r11
  0000000140854261: 83 7C 24 48 FE     cmp         dword ptr [rsp+48h],0FFFFFFFEh
  0000000140854266: 74 0B              je          0000000140854273
  0000000140854268: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  000000014085426D: 0F 85 D5 00 00 00  jne         0000000140854348
  0000000140854273: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140854278: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  000000014085427C: 0F AF 48 48        imul        ecx,dword ptr [rax+48h]
  0000000140854280: E8 9B 3D 17 00     call        00000001409C8020
  0000000140854285: 4C 8B D8           mov         r11,rax
  0000000140854288: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014085428D: 4C 89 98 F0 00 00  mov         qword ptr [rax+00000000000000F0h],r11
                    00
  0000000140854294: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140854299: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  000000014085429D: 0F AF 48 4C        imul        ecx,dword ptr [rax+4Ch]
  00000001408542A1: E8 7A 3D 17 00     call        00000001409C8020
  00000001408542A6: 4C 8B D8           mov         r11,rax
  00000001408542A9: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408542AE: 4C 89 98 F8 00 00  mov         qword ptr [rax+00000000000000F8h],r11
                    00
  00000001408542B5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408542BA: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  00000001408542BE: 0F AF 48 50        imul        ecx,dword ptr [rax+50h]
  00000001408542C2: E8 59 3D 17 00     call        00000001409C8020
  00000001408542C7: 4C 8B D8           mov         r11,rax
  00000001408542CA: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408542CF: 4C 89 98 00 01 00  mov         qword ptr [rax+0000000000000100h],r11
                    00
  00000001408542D6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408542DB: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  00000001408542DF: 0F AF 48 54        imul        ecx,dword ptr [rax+54h]
  00000001408542E3: E8 38 3D 17 00     call        00000001409C8020
  00000001408542E8: 4C 8B D8           mov         r11,rax
  00000001408542EB: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408542F0: 4C 89 98 08 01 00  mov         qword ptr [rax+0000000000000108h],r11
                    00
  00000001408542F7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408542FC: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  0000000140854300: 0F AF 48 58        imul        ecx,dword ptr [rax+58h]
  0000000140854304: E8 17 3D 17 00     call        00000001409C8020
  0000000140854309: 4C 8B D8           mov         r11,rax
  000000014085430C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140854311: 4C 89 98 10 01 00  mov         qword ptr [rax+0000000000000110h],r11
                    00
  0000000140854318: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014085431D: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  0000000140854321: 0F AF 48 5C        imul        ecx,dword ptr [rax+5Ch]
  0000000140854325: E8 F6 3C 17 00     call        00000001409C8020
  000000014085432A: 4C 8B D8           mov         r11,rax
  000000014085432D: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140854332: 4C 89 98 18 01 00  mov         qword ptr [rax+0000000000000118h],r11
                    00
  0000000140854339: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014085433E: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140854342: 89 81 2C 01 00 00  mov         dword ptr [rcx+000000000000012Ch],eax
  0000000140854348: 44 8B 4C 24 48     mov         r9d,dword ptr [rsp+48h]
  000000014085434D: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  0000000140854352: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140854356: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014085435B: E8 80 A5 2E 00     call        0000000140B3E8E0
  0000000140854360: 48 83 C4 28        add         rsp,28h
  0000000140854364: C3                 ret

  Summary

     1E02000 .text
