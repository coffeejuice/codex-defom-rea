Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141470568: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000014147056D: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  0000000141470572: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  0000000141470577: 57                 push        rdi
  0000000141470578: 48 83 EC 30        sub         rsp,30h
  000000014147057C: 48 8B E9           mov         rbp,rcx
  000000014147057F: 48 8B 0D BA F5 28  mov         rcx,qword ptr [00000001446FFB40h]
                    03
  0000000141470586: 41 8B D9           mov         ebx,r9d
  0000000141470589: 49 8B F8           mov         rdi,r8
  000000014147058C: 48 8B F2           mov         rsi,rdx
  000000014147058F: E8 58 0E 01 00     call        00000001414813EC
  0000000141470594: 48 85 C0           test        rax,rax
  0000000141470597: 74 1A              je          00000001414705B3
  0000000141470599: 4C 8B 54 24 60     mov         r10,qword ptr [rsp+60h]
  000000014147059E: 44 8B CB           mov         r9d,ebx
  00000001414705A1: 4C 8B C7           mov         r8,rdi
  00000001414705A4: 48 8B D6           mov         rdx,rsi
  00000001414705A7: 48 8B CD           mov         rcx,rbp
  00000001414705AA: 4C 89 54 24 20     mov         qword ptr [rsp+20h],r10
  00000001414705AF: FF D0              call        rax
  00000001414705B1: EB 25              jmp         00000001414705D8
  00000001414705B3: B9 02 00 00 00     mov         ecx,2
  00000001414705B8: E8 33 2F 01 00     call        00000001414834F0
  00000001414705BD: 4C 8B 5C 24 60     mov         r11,qword ptr [rsp+60h]
  00000001414705C2: 44 8B CB           mov         r9d,ebx
  00000001414705C5: 4C 8B C7           mov         r8,rdi
  00000001414705C8: 48 8B D6           mov         rdx,rsi
  00000001414705CB: 48 8B CD           mov         rcx,rbp
  00000001414705CE: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  00000001414705D3: E8 68 FE FF FF     call        0000000141470440
  00000001414705D8: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000001414705DD: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  00000001414705E2: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  00000001414705E7: 48 83 C4 30        add         rsp,30h
  00000001414705EB: 5F                 pop         rdi
  00000001414705EC: C3                 ret

  Summary

     1E02000 .text
