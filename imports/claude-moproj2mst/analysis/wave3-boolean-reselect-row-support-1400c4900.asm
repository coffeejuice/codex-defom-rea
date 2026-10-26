Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400C4900: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400C4905: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400C490A: 48 83 EC 78        sub         rsp,78h
  00000001400C490E: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  00000001400C4917: 48 C7 44 24 58 00  mov         qword ptr [rsp+58h],0
                    00 00 00
  00000001400C4920: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  00000001400C4929: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  00000001400C4932: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001400C493B: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400C4943: FF 15 F7 6C D4 01  call        qword ptr [0000000141E0B640h]
  00000001400C4949: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400C494E: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  00000001400C4957: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001400C495F: E8 EC F0 FF FF     call        00000001400C3A50
  00000001400C4964: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  00000001400C4968: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001400C496D: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  00000001400C4971: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400C4979: E8 D2 0E 00 00     call        00000001400C5850
  00000001400C497E: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400C4983: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  00000001400C498B: EB 0B              jmp         00000001400C4998
  00000001400C498D: 8B 44 24 68        mov         eax,dword ptr [rsp+68h]
  00000001400C4991: 83 C0 01           add         eax,1
  00000001400C4994: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  00000001400C4998: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001400C499C: 39 44 24 68        cmp         dword ptr [rsp+68h],eax
  00000001400C49A0: 7D 59              jge         00000001400C49FB
  00000001400C49A2: 8B 44 24 68        mov         eax,dword ptr [rsp+68h]
  00000001400C49A6: 83 C0 01           add         eax,1
  00000001400C49A9: 89 44 24 6C        mov         dword ptr [rsp+6Ch],eax
  00000001400C49AD: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  00000001400C49B1: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001400C49B9: E8 A2 F2 FF FF     call        00000001400C3C60
  00000001400C49BE: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400C49C3: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001400C49C9: 75 02              jne         00000001400C49CD
  00000001400C49CB: EB C0              jmp         00000001400C498D
  00000001400C49CD: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  00000001400C49D5: 4C 8B 4C 24 58     mov         r9,qword ptr [rsp+58h]
  00000001400C49DA: 44 8B 44 24 6C     mov         r8d,dword ptr [rsp+6Ch]
  00000001400C49DF: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001400C49E7: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400C49EF: E8 1C 00 00 00     call        00000001400C4A10
  00000001400C49F4: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001400C49F9: EB 92              jmp         00000001400C498D
  00000001400C49FB: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000001400C4A00: 48 83 C4 78        add         rsp,78h
  00000001400C4A04: C3                 ret

  Summary

     1E02000 .text
