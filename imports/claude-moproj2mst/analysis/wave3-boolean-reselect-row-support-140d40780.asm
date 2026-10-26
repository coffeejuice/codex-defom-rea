Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D40780: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140D40785: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140D40789: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D4078E: 48 83 EC 78        sub         rsp,78h
  0000000140D40792: 48 C7 44 24 50 FE  mov         qword ptr [rsp+50h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D4079B: 48 83 BC 24 90 00  cmp         qword ptr [rsp+0000000000000090h],0
                    00 00 00
  0000000140D407A4: 75 16              jne         0000000140D407BC
  0000000140D407A6: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D407AE: FF 15 8C AE 0C 01  call        qword ptr [0000000141E0B640h]
  0000000140D407B4: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D407BC: B9 30 00 00 00     mov         ecx,30h
  0000000140D407C1: E8 F6 E2 72 00     call        000000014146EABC
  0000000140D407C6: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D407CB: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140D407D1: 74 11              je          0000000140D407E4
  0000000140D407D3: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140D407D8: E8 23 7A 09 00     call        0000000140DD8200
  0000000140D407DD: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140D407E2: EB 09              jmp         0000000140D407ED
  0000000140D407E4: 48 C7 44 24 58 00  mov         qword ptr [rsp+58h],0
                    00 00 00
  0000000140D407ED: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140D407F2: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D407F7: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140D407FC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D40801: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140D40806: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D4080E: FF 15 A4 AD 0C 01  call        qword ptr [0000000141E0B5B8h]
  0000000140D40814: 0F B6 84 24 88 00  movzx       eax,byte ptr [rsp+0000000000000088h]
                    00 00
  0000000140D4081C: 85 C0              test        eax,eax
  0000000140D4081E: 74 55              je          0000000140D40875
  0000000140D40820: B9 30 00 00 00     mov         ecx,30h
  0000000140D40825: E8 92 E2 72 00     call        000000014146EABC
  0000000140D4082A: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D4082F: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140D40835: 74 11              je          0000000140D40848
  0000000140D40837: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D4083C: E8 3F 7B 09 00     call        0000000140DD8380
  0000000140D40841: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140D40846: EB 09              jmp         0000000140D40851
  0000000140D40848: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000140D40851: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140D40856: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D4085B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D40860: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D40865: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  0000000140D4086A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D4086F: FF 15 43 AD 0C 01  call        qword ptr [0000000141E0B5B8h]
  0000000140D40875: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D4087A: 48 83 C4 78        add         rsp,78h
  0000000140D4087E: C3                 ret

  Summary

     1E02000 .text
