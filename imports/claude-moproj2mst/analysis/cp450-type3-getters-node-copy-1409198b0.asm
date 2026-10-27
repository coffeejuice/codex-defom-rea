Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001409198B0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001409198B5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001409198BA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001409198BF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001409198C4: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  00000001409198CB: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001409198D4: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001409198DC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001409198E1: 33 C0              xor         eax,eax
  00000001409198E3: 83 F8 01           cmp         eax,1
  00000001409198E6: 0F 84 A6 00 00 00  je          0000000140919992
  00000001409198EC: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001409198F4: 48 39 44 24 20     cmp         qword ptr [rsp+20h],rax
  00000001409198F9: 0F 84 8E 00 00 00  je          000000014091998D
  00000001409198FF: B9 10 00 00 00     mov         ecx,10h
  0000000140919904: E8 B3 51 B5 00     call        000000014146EABC
  0000000140919909: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014091990E: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140919913: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140919918: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014091991E: 74 26              je          0000000140919946
  0000000140919920: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140919928: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014091992B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140919930: E8 0B 02 00 00     call        0000000140919B40
  0000000140919935: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014091993A: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  000000014091993F: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140919944: EB 09              jmp         000000014091994F
  0000000140919946: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  000000014091994F: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140919954: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140919959: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014091995E: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140919963: 48 89 01           mov         qword ptr [rcx],rax
  0000000140919966: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014091996B: 48 83 C0 08        add         rax,8
  000000014091996F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140919974: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091997C: 48 83 C0 08        add         rax,8
  0000000140919980: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140919988: E9 5F FF FF FF     jmp         00000001409198EC
  000000014091998D: E9 06 01 00 00     jmp         0000000140919A98
  0000000140919992: 33 C0              xor         eax,eax
  0000000140919994: 83 F8 01           cmp         eax,1
  0000000140919997: 0F 84 9D 00 00 00  je          0000000140919A3A
  000000014091999D: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001409199A5: 48 39 44 24 20     cmp         qword ptr [rsp+20h],rax
  00000001409199AA: 0F 84 88 00 00 00  je          0000000140919A38
  00000001409199B0: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001409199B5: B9 10 00 00 00     mov         ecx,10h
  00000001409199BA: E8 C1 98 6E FF     call        0000000140003280
  00000001409199BF: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001409199C4: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  00000001409199CA: 74 2C              je          00000001409199F8
  00000001409199CC: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001409199D4: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001409199D9: E8 62 01 00 00     call        0000000140919B40
  00000001409199DE: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001409199E6: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001409199EE: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001409199F6: EB 0C              jmp         0000000140919A04
  00000001409199F8: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  0000000140919A04: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140919A0C: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140919A11: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140919A16: 48 83 C0 08        add         rax,8
  0000000140919A1A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140919A1F: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140919A27: 48 83 C0 08        add         rax,8
  0000000140919A2B: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140919A33: E9 65 FF FF FF     jmp         000000014091999D
  0000000140919A38: EB 5E              jmp         0000000140919A98
  0000000140919A3A: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140919A42: 48 39 84 24 B8 00  cmp         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140919A4A: 74 4C              je          0000000140919A98
  0000000140919A4C: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140919A54: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140919A5C: 48 2B C1           sub         rax,rcx
  0000000140919A5F: 48 C1 F8 03        sar         rax,3
  0000000140919A63: 48 85 C0           test        rax,rax
  0000000140919A66: 7E 30              jle         0000000140919A98
  0000000140919A68: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140919A70: 4C 8B 84 24 B0 00  mov         r8,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140919A78: 4C 2B C0           sub         r8,rax
  0000000140919A7B: 49 C1 F8 03        sar         r8,3
  0000000140919A7F: 49 C1 E0 03        shl         r8,3
  0000000140919A83: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140919A8B: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140919A93: E8 68 56 B5 00     call        000000014146F100
  0000000140919A98: 48 81 C4 98 00 00  add         rsp,98h
                    00
  0000000140919A9F: C3                 ret

  Summary

     1E02000 .text
