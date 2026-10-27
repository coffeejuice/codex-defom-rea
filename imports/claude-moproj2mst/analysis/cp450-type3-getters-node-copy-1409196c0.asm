Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001409196C0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001409196C5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001409196CA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001409196CF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001409196D4: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  00000001409196DB: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001409196E4: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001409196EC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001409196F1: 33 C0              xor         eax,eax
  00000001409196F3: 83 F8 01           cmp         eax,1
  00000001409196F6: 0F 84 A6 00 00 00  je          00000001409197A2
  00000001409196FC: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140919704: 48 39 44 24 20     cmp         qword ptr [rsp+20h],rax
  0000000140919709: 0F 84 8E 00 00 00  je          000000014091979D
  000000014091970F: B9 28 00 00 00     mov         ecx,28h
  0000000140919714: E8 A3 53 B5 00     call        000000014146EABC
  0000000140919719: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014091971E: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140919723: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140919728: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014091972E: 74 26              je          0000000140919756
  0000000140919730: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140919738: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014091973B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140919740: E8 5B 03 00 00     call        0000000140919AA0
  0000000140919745: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014091974A: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  000000014091974F: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140919754: EB 09              jmp         000000014091975F
  0000000140919756: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  000000014091975F: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140919764: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140919769: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014091976E: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140919773: 48 89 01           mov         qword ptr [rcx],rax
  0000000140919776: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014091977B: 48 83 C0 08        add         rax,8
  000000014091977F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140919784: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091978C: 48 83 C0 08        add         rax,8
  0000000140919790: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140919798: E9 5F FF FF FF     jmp         00000001409196FC
  000000014091979D: E9 06 01 00 00     jmp         00000001409198A8
  00000001409197A2: 33 C0              xor         eax,eax
  00000001409197A4: 83 F8 01           cmp         eax,1
  00000001409197A7: 0F 84 9D 00 00 00  je          000000014091984A
  00000001409197AD: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001409197B5: 48 39 44 24 20     cmp         qword ptr [rsp+20h],rax
  00000001409197BA: 0F 84 88 00 00 00  je          0000000140919848
  00000001409197C0: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001409197C5: B9 28 00 00 00     mov         ecx,28h
  00000001409197CA: E8 B1 9A 6E FF     call        0000000140003280
  00000001409197CF: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001409197D4: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  00000001409197DA: 74 2C              je          0000000140919808
  00000001409197DC: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001409197E4: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001409197E9: E8 B2 02 00 00     call        0000000140919AA0
  00000001409197EE: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001409197F6: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001409197FE: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140919806: EB 0C              jmp         0000000140919814
  0000000140919808: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  0000000140919814: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091981C: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140919821: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140919826: 48 83 C0 08        add         rax,8
  000000014091982A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014091982F: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140919837: 48 83 C0 08        add         rax,8
  000000014091983B: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140919843: E9 65 FF FF FF     jmp         00000001409197AD
  0000000140919848: EB 5E              jmp         00000001409198A8
  000000014091984A: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140919852: 48 39 84 24 B8 00  cmp         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  000000014091985A: 74 4C              je          00000001409198A8
  000000014091985C: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140919864: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091986C: 48 2B C1           sub         rax,rcx
  000000014091986F: 48 C1 F8 03        sar         rax,3
  0000000140919873: 48 85 C0           test        rax,rax
  0000000140919876: 7E 30              jle         00000001409198A8
  0000000140919878: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140919880: 4C 8B 84 24 B0 00  mov         r8,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140919888: 4C 2B C0           sub         r8,rax
  000000014091988B: 49 C1 F8 03        sar         r8,3
  000000014091988F: 49 C1 E0 03        shl         r8,3
  0000000140919893: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091989B: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001409198A3: E8 58 58 B5 00     call        000000014146F100
  00000001409198A8: 48 81 C4 98 00 00  add         rsp,98h
                    00
  00000001409198AF: C3                 ret

  Summary

     1E02000 .text
