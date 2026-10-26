Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140857990: 44 89 4C 24 20     mov         dword ptr [rsp+20h],r9d
  0000000140857995: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000014085799A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014085799E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408579A3: 48 83 EC 38        sub         rsp,38h
  00000001408579A7: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  00000001408579AC: 7F 05              jg          00000001408579B3
  00000001408579AE: E9 99 00 00 00     jmp         0000000140857A4C
  00000001408579B3: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  00000001408579B7: 0F AF 44 24 58     imul        eax,dword ptr [rsp+58h]
  00000001408579BC: 48 63 D0           movsxd      rdx,eax
  00000001408579BF: 48 C1 E2 02        shl         rdx,2
  00000001408579C3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408579C8: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001408579CC: E8 F3 D9 C1 00     call        00000001414753C4
  00000001408579D1: 4C 8B D8           mov         r11,rax
  00000001408579D4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408579D9: 4C 89 58 08        mov         qword ptr [rax+8],r11
  00000001408579DD: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001408579E2: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  00000001408579E6: 0F AF 81 F4 00 00  imul        eax,dword ptr [rcx+00000000000000F4h]
                    00
  00000001408579ED: 48 63 D0           movsxd      rdx,eax
  00000001408579F0: 48 C1 E2 02        shl         rdx,2
  00000001408579F4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408579F9: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  00000001408579FD: E8 C2 D9 C1 00     call        00000001414753C4
  0000000140857A02: 4C 8B D8           mov         r11,rax
  0000000140857A05: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140857A0A: 4C 89 58 18        mov         qword ptr [rax+18h],r11
  0000000140857A0E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140857A13: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  0000000140857A17: 89 81 E8 00 00 00  mov         dword ptr [rcx+00000000000000E8h],eax
  0000000140857A1D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140857A22: 8B 44 24 58        mov         eax,dword ptr [rsp+58h]
  0000000140857A26: 89 81 EC 00 00 00  mov         dword ptr [rcx+00000000000000ECh],eax
  0000000140857A2C: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  0000000140857A34: 44 8B 4C 24 20     mov         r9d,dword ptr [rsp+20h]
  0000000140857A39: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  0000000140857A3E: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  0000000140857A42: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140857A47: E8 94 6E 2E 00     call        0000000140B3E8E0
  0000000140857A4C: 48 83 C4 38        add         rsp,38h
  0000000140857A50: C3                 ret

  Summary

     1E02000 .text
