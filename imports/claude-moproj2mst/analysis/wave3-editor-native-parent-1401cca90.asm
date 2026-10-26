Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001401CCA90: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001401CCA95: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401CCA9A: 48 81 EC F8 00 00  sub         rsp,0F8h
                    00
  00000001401CCAA1: 48 C7 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001401CCAAD: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001401CCAB6: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001401CCABB: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001401CCAC3: E8 D8 1E B8 00     call        0000000140D4E9A0
  00000001401CCAC8: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001401CCAD0: E8 9B 25 43 00     call        00000001405FF070
  00000001401CCAD5: 48 8B C8           mov         rcx,rax
  00000001401CCAD8: E8 D3 27 42 00     call        00000001405EF2B0
  00000001401CCADD: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001401CCAE2: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001401CCAE8: 75 07              jne         00000001401CCAF1
  00000001401CCAEA: 33 C0              xor         eax,eax
  00000001401CCAEC: E9 51 03 00 00     jmp         00000001401CCE42
  00000001401CCAF1: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001401CCAF6: E8 75 2C E9 FF     call        000000014005F770
  00000001401CCAFB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001401CCB00: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001401CCB06: 75 07              jne         00000001401CCB0F
  00000001401CCB08: 33 C0              xor         eax,eax
  00000001401CCB0A: E9 33 03 00 00     jmp         00000001401CCE42
  00000001401CCB0F: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001401CCB17: E8 54 1F B8 00     call        0000000140D4EA70
  00000001401CCB1C: 45 33 C0           xor         r8d,r8d
  00000001401CCB1F: 8B D0              mov         edx,eax
  00000001401CCB21: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001401CCB26: E8 C5 E6 AA 00     call        0000000140C7B1F0
  00000001401CCB2B: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001401CCB33: E8 88 59 F7 FF     call        00000001401424C0
  00000001401CCB38: 48 8D 15 E9 13 04  lea         rdx,[000000014220DF28h]
                    02
  00000001401CCB3F: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001401CCB44: FF 15 C6 75 C3 01  call        qword ptr [0000000141E04110h]
  00000001401CCB4A: 48 8B D0           mov         rdx,rax
  00000001401CCB4D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001401CCB52: E8 09 59 F7 FF     call        0000000140142460
  00000001401CCB57: 0F B6 C0           movzx       eax,al
  00000001401CCB5A: 85 C0              test        eax,eax
  00000001401CCB5C: 74 05              je          00000001401CCB63
  00000001401CCB5E: E9 54 02 00 00     jmp         00000001401CCDB7
  00000001401CCB63: 48 8D 15 C6 13 04  lea         rdx,[000000014220DF30h]
                    02
  00000001401CCB6A: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001401CCB6F: FF 15 9B 75 C3 01  call        qword ptr [0000000141E04110h]
  00000001401CCB75: 48 8B D0           mov         rdx,rax
  00000001401CCB78: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001401CCB7D: E8 DE 58 F7 FF     call        0000000140142460
  00000001401CCB82: 0F B6 C0           movzx       eax,al
  00000001401CCB85: 85 C0              test        eax,eax
  00000001401CCB87: 74 68              je          00000001401CCBF1
  00000001401CCB89: B9 A8 01 00 00     mov         ecx,1A8h
  00000001401CCB8E: E8 29 1F 2A 01     call        000000014146EABC
  00000001401CCB93: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001401CCB98: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  00000001401CCB9E: 74 29              je          00000001401CCBC9
  00000001401CCBA0: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001401CCBA8: E8 C3 1E B8 00     call        0000000140D4EA70
  00000001401CCBAD: 44 8B C0           mov         r8d,eax
  00000001401CCBB0: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001401CCBB5: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001401CCBBA: E8 C1 4E 01 00     call        00000001401E1A80
  00000001401CCBBF: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000001401CCBC7: EB 0C              jmp         00000001401CCBD5
  00000001401CCBC9: 48 C7 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],0
                    00 00 00 00 00 00
  00000001401CCBD5: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001401CCBDD: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001401CCBE2: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001401CCBE7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001401CCBEC: E9 C6 01 00 00     jmp         00000001401CCDB7
  00000001401CCBF1: 48 8D 15 48 13 04  lea         rdx,[000000014220DF40h]
                    02
  00000001401CCBF8: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001401CCBFD: FF 15 0D 75 C3 01  call        qword ptr [0000000141E04110h]
  00000001401CCC03: 48 8B D0           mov         rdx,rax
  00000001401CCC06: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001401CCC0B: E8 50 58 F7 FF     call        0000000140142460
  00000001401CCC10: 0F B6 C0           movzx       eax,al
  00000001401CCC13: 85 C0              test        eax,eax
  00000001401CCC15: 74 68              je          00000001401CCC7F
  00000001401CCC17: B9 A8 01 00 00     mov         ecx,1A8h
  00000001401CCC1C: E8 9B 1E 2A 01     call        000000014146EABC
  00000001401CCC21: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001401CCC26: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  00000001401CCC2C: 74 29              je          00000001401CCC57
  00000001401CCC2E: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001401CCC36: E8 35 1E B8 00     call        0000000140D4EA70
  00000001401CCC3B: 44 8B C0           mov         r8d,eax
  00000001401CCC3E: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001401CCC43: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001401CCC48: E8 D3 CC 00 00     call        00000001401D9920
  00000001401CCC4D: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  00000001401CCC55: EB 0C              jmp         00000001401CCC63
  00000001401CCC57: 48 C7 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],0
                    00 00 00 00 00 00
  00000001401CCC63: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000001401CCC6B: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001401CCC70: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  00000001401CCC75: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001401CCC7A: E9 38 01 00 00     jmp         00000001401CCDB7
  00000001401CCC7F: 48 8D 15 C6 12 04  lea         rdx,[000000014220DF4Ch]
                    02
  00000001401CCC86: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001401CCC8B: FF 15 7F 74 C3 01  call        qword ptr [0000000141E04110h]
  00000001401CCC91: 48 8B D0           mov         rdx,rax
  00000001401CCC94: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001401CCC99: E8 C2 57 F7 FF     call        0000000140142460
  00000001401CCC9E: 0F B6 C0           movzx       eax,al
  00000001401CCCA1: 85 C0              test        eax,eax
  00000001401CCCA3: 74 77              je          00000001401CCD1C
  00000001401CCCA5: B9 08 03 00 00     mov         ecx,308h
  00000001401CCCAA: E8 0D 1E 2A 01     call        000000014146EABC
  00000001401CCCAF: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001401CCCB7: 48 83 BC 24 88 00  cmp         qword ptr [rsp+0000000000000088h],0
                    00 00 00
  00000001401CCCC0: 74 2C              je          00000001401CCCEE
  00000001401CCCC2: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001401CCCCA: E8 A1 1D B8 00     call        0000000140D4EA70
  00000001401CCCCF: 44 8B C0           mov         r8d,eax
  00000001401CCCD2: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001401CCCD7: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001401CCCDF: E8 9C 4E 00 00     call        00000001401D1B80
  00000001401CCCE4: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  00000001401CCCEC: EB 0C              jmp         00000001401CCCFA
  00000001401CCCEE: 48 C7 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],0
                    00 00 00 00 00 00
  00000001401CCCFA: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  00000001401CCD02: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001401CCD0A: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001401CCD12: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001401CCD17: E9 9B 00 00 00     jmp         00000001401CCDB7
  00000001401CCD1C: 48 8D 15 35 12 04  lea         rdx,[000000014220DF58h]
                    02
  00000001401CCD23: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001401CCD2B: FF 15 DF 73 C3 01  call        qword ptr [0000000141E04110h]
  00000001401CCD31: 48 8B D0           mov         rdx,rax
  00000001401CCD34: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001401CCD39: E8 22 57 F7 FF     call        0000000140142460
  00000001401CCD3E: 0F B6 C0           movzx       eax,al
  00000001401CCD41: 85 C0              test        eax,eax
  00000001401CCD43: 74 72              je          00000001401CCDB7
  00000001401CCD45: B9 50 00 00 00     mov         ecx,50h
  00000001401CCD4A: E8 6D 1D 2A 01     call        000000014146EABC
  00000001401CCD4F: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001401CCD57: 48 83 BC 24 A0 00  cmp         qword ptr [rsp+00000000000000A0h],0
                    00 00 00
  00000001401CCD60: 74 2C              je          00000001401CCD8E
  00000001401CCD62: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001401CCD6A: E8 01 1D B8 00     call        0000000140D4EA70
  00000001401CCD6F: 44 8B C0           mov         r8d,eax
  00000001401CCD72: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001401CCD77: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401CCD7F: E8 3C 1A 00 00     call        00000001401CE7C0
  00000001401CCD84: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  00000001401CCD8C: EB 0C              jmp         00000001401CCD9A
  00000001401CCD8E: 48 C7 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],0
                    00 00 00 00 00 00
  00000001401CCD9A: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001401CCDA2: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000001401CCDAA: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001401CCDB2: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001401CCDB7: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001401CCDBD: 75 1A              jne         00000001401CCDD9
  00000001401CCDBF: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001401CCDC7: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001401CCDCF: E8 DC 22 43 00     call        00000001405FF0B0
  00000001401CCDD4: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001401CCDD9: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001401CCDE1: E8 FA 3A 43 00     call        00000001406008E0
  00000001401CCDE6: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001401CCDEC: 74 4F              je          00000001401CCE3D
  00000001401CCDEE: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  00000001401CCDF6: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001401CCDFE: E8 CD 1B B8 00     call        0000000140D4E9D0
  00000001401CCE03: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  00000001401CCE0B: 48 8B 84 24 D8 00  mov         rax,qword ptr [rsp+00000000000000D8h]
                    00 00
  00000001401CCE13: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  00000001401CCE1B: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001401CCE23: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001401CCE28: FF 15 B2 EE C3 01  call        qword ptr [0000000141E0BCE0h]
  00000001401CCE2E: 90                 nop
  00000001401CCE2F: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  00000001401CCE37: FF 15 A3 8B C3 01  call        qword ptr [0000000141E059E0h]
  00000001401CCE3D: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001401CCE42: 48 81 C4 F8 00 00  add         rsp,0F8h
                    00
  00000001401CCE49: C3                 ret

  Summary

     1E02000 .text
