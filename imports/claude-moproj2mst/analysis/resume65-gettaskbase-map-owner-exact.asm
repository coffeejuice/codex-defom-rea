Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014001D960: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014001D965: 48 83 EC 68        sub         rsp,68h
  000000014001D969: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014001D972: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000014001D977: 48 83 B8 A0 04 00  cmp         qword ptr [rax+00000000000004A0h],0
                    00 00
  000000014001D97F: 0F 85 B3 00 00 00  jne         000000014001DA38
  000000014001D985: B9 10 00 00 00     mov         ecx,10h
  000000014001D98A: E8 2D 11 45 01     call        000000014146EABC
  000000014001D98F: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014001D994: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014001D99A: 74 11              je          000000014001D9AD
  000000014001D99C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014001D9A1: E8 0A A0 7A 00     call        00000001407C79B0
  000000014001D9A6: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014001D9AB: EB 09              jmp         000000014001D9B6
  000000014001D9AD: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  000000014001D9B6: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014001D9BB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014001D9C0: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014001D9C5: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014001D9CA: 48 89 81 A0 04 00  mov         qword ptr [rcx+00000000000004A0h],rax
                    00
  000000014001D9D1: B9 20 00 00 00     mov         ecx,20h
  000000014001D9D6: E8 E1 10 45 01     call        000000014146EABC
  000000014001D9DB: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014001D9E0: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  000000014001D9E6: 74 1D              je          000000014001DA05
  000000014001D9E8: 48 8B 54 24 70     mov         rdx,qword ptr [rsp+70h]
  000000014001D9ED: 48 8B 92 A0 04 00  mov         rdx,qword ptr [rdx+00000000000004A0h]
                    00
  000000014001D9F4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014001D9F9: E8 D2 9B 11 00     call        00000001401375D0
  000000014001D9FE: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014001DA03: EB 09              jmp         000000014001DA0E
  000000014001DA05: 48 C7 44 24 58 00  mov         qword ptr [rsp+58h],0
                    00 00 00
  000000014001DA0E: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014001DA13: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014001DA18: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014001DA1D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014001DA22: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  000000014001DA27: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014001DA2C: 48 8B 89 A0 04 00  mov         rcx,qword ptr [rcx+00000000000004A0h]
                    00
  000000014001DA33: E8 38 A0 7A 00     call        00000001407C7A70
  000000014001DA38: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000014001DA3D: 48 8B 80 A0 04 00  mov         rax,qword ptr [rax+00000000000004A0h]
                    00
  000000014001DA44: 48 83 C4 68        add         rsp,68h
  000000014001DA48: C3                 ret
  000000014001DA49: CC                 int         3

  Summary

     1E02000 .text
