Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C80A20: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140C80A24: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C80A29: 48 83 EC 38        sub         rsp,38h
  0000000140C80A2D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C80A32: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  0000000140C80A37: 75 05              jne         0000000140C80A3E
  0000000140C80A39: E9 B4 00 00 00     jmp         0000000140C80AF2
  0000000140C80A3E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C80A43: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C80A47: C7 80 04 02 00 00  mov         dword ptr [rax+0000000000000204h],1
                    01 00 00 00
  0000000140C80A51: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  0000000140C80A55: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140C80A59: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80A5E: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C80A62: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140C80A66: 89 81 0C 02 00 00  mov         dword ptr [rcx+000000000000020Ch],eax
  0000000140C80A6C: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  0000000140C80A71: 75 10              jne         0000000140C80A83
  0000000140C80A73: B2 01              mov         dl,1
  0000000140C80A75: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80A7A: E8 21 A7 FF FF     call        0000000140C7B1A0
  0000000140C80A7F: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140C80A83: C7 44 24 24 00 00  mov         dword ptr [rsp+24h],0
                    00 00
  0000000140C80A8B: 41 B9 FF FF FF FF  mov         r9d,0FFFFFFFFh
  0000000140C80A91: 44 8B 44 24 24     mov         r8d,dword ptr [rsp+24h]
  0000000140C80A96: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140C80A9A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80A9F: E8 0C 07 00 00     call        0000000140C811B0
  0000000140C80AA4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80AA9: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C80AAD: E8 6E BE 08 00     call        0000000140D0C920
  0000000140C80AB2: 0F B6 C0           movzx       eax,al
  0000000140C80AB5: 85 C0              test        eax,eax
  0000000140C80AB7: 75 12              jne         0000000140C80ACB
  0000000140C80AB9: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140C80ABD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80AC2: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C80AC6: E8 05 EA 07 00     call        0000000140CFF4D0
  0000000140C80ACB: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80AD0: E8 3B F7 00 00     call        0000000140C90210
  0000000140C80AD5: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C80ADA: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C80ADF: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C80AE2: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140C80AE7: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140C80AEC: FF 90 40 01 00 00  call        qword ptr [rax+0000000000000140h]
  0000000140C80AF2: 48 83 C4 38        add         rsp,38h
  0000000140C80AF6: C3                 ret

  Summary

     1E02000 .text
