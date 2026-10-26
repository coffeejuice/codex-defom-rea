Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140054950: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140054954: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140054959: 48 81 EC A8 00 00  sub         rsp,0A8h
                    00
  0000000140054960: 48 C7 44 24 68 FE  mov         qword ptr [rsp+68h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140054969: C7 44 24 60 00 00  mov         dword ptr [rsp+60h],0
                    00 00
  0000000140054971: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054979: E8 12 05 00 00     call        0000000140054E90
  000000014005497E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140054983: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140054989: 75 05              jne         0000000140054990
  000000014005498B: E9 7F 02 00 00     jmp         0000000140054C0F
  0000000140054990: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140054995: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140054998: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014005499D: FF 90 A0 00 00 00  call        qword ptr [rax+00000000000000A0h]
  00000001400549A3: 0F B6 C0           movzx       eax,al
  00000001400549A6: 85 C0              test        eax,eax
  00000001400549A8: 0F 84 43 02 00 00  je          0000000140054BF1
  00000001400549AE: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400549B3: E8 E8 A9 59 00     call        00000001405EF3A0
  00000001400549B8: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400549BD: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400549C5: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001400549CA: E8 B1 EF 06 00     call        00000001400C3980
  00000001400549CF: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400549D7: 48 81 C1 60 01 00  add         rcx,160h
                    00
  00000001400549DE: E8 0D 65 BC 00     call        0000000140C1AEF0
  00000001400549E3: 48 85 C0           test        rax,rax
  00000001400549E6: 0F 85 C5 00 00 00  jne         0000000140054AB1
  00000001400549EC: B9 50 00 00 00     mov         ecx,50h
  00000001400549F1: E8 C6 A0 41 01     call        000000014146EABC
  00000001400549F6: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400549FB: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140054A01: 74 11              je          0000000140054A14
  0000000140054A03: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140054A08: E8 83 FE 06 00     call        00000001400C4890
  0000000140054A0D: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140054A12: EB 09              jmp         0000000140054A1D
  0000000140054A14: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  0000000140054A1D: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140054A22: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140054A27: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054A2F: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140054A36: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140054A3B: E8 E0 7E C6 00     call        0000000140CBC920
  0000000140054A40: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140054A46: 74 64              je          0000000140054AAC
  0000000140054A48: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054A50: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140054A57: E8 94 64 BC 00     call        0000000140C1AEF0
  0000000140054A5C: 4C 8B C0           mov         r8,rax
  0000000140054A5F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140054A64: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140054A67: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054A6F: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140054A74: FF 90 A8 00 00 00  call        qword ptr [rax+00000000000000A8h]
  0000000140054A7A: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054A82: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140054A89: E8 62 64 BC 00     call        0000000140C1AEF0
  0000000140054A8E: 4C 8B C0           mov         r8,rax
  0000000140054A91: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140054A96: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140054A99: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054AA1: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140054AA6: FF 90 78 02 00 00  call        qword ptr [rax+0000000000000278h]
  0000000140054AAC: E9 1B 01 00 00     jmp         0000000140054BCC
  0000000140054AB1: 0F B6 84 24 B8 00  movzx       eax,byte ptr [rsp+00000000000000B8h]
                    00 00
  0000000140054AB9: 85 C0              test        eax,eax
  0000000140054ABB: 75 70              jne         0000000140054B2D
  0000000140054ABD: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054AC5: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140054ACC: E8 1F 64 BC 00     call        0000000140C1AEF0
  0000000140054AD1: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140054AD6: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140054ADB: FF 15 EF 0D DB 01  call        qword ptr [0000000141E058D0h]
  0000000140054AE1: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140054AE9: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140054AF1: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140054AF9: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  0000000140054AFD: 83 C8 01           or          eax,1
  0000000140054B00: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  0000000140054B04: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140054B09: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140054B0C: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140054B14: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140054B19: FF 50 68           call        qword ptr [rax+68h]
  0000000140054B1C: 85 C0              test        eax,eax
  0000000140054B1E: 74 0D              je          0000000140054B2D
  0000000140054B20: C7 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00
  0000000140054B2B: EB 0B              jmp         0000000140054B38
  0000000140054B2D: C7 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],1
                    00 01 00 00 00
  0000000140054B38: 0F B6 84 24 90 00  movzx       eax,byte ptr [rsp+0000000000000090h]
                    00 00
  0000000140054B40: 88 44 24 40        mov         byte ptr [rsp+40h],al
  0000000140054B44: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  0000000140054B48: 83 E0 01           and         eax,1
  0000000140054B4B: 85 C0              test        eax,eax
  0000000140054B4D: 74 10              je          0000000140054B5F
  0000000140054B4F: 83 64 24 60 FE     and         dword ptr [rsp+60h],0FFFFFFFEh
  0000000140054B54: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140054B59: FF 15 81 F3 DA 01  call        qword ptr [0000000141E03EE0h]
  0000000140054B5F: 0F B6 44 24 40     movzx       eax,byte ptr [rsp+40h]
  0000000140054B64: 85 C0              test        eax,eax
  0000000140054B66: 74 32              je          0000000140054B9A
  0000000140054B68: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054B70: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140054B77: E8 74 63 BC 00     call        0000000140C1AEF0
  0000000140054B7C: 4C 8B C0           mov         r8,rax
  0000000140054B7F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140054B84: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140054B87: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054B8F: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140054B94: FF 90 A8 00 00 00  call        qword ptr [rax+00000000000000A8h]
  0000000140054B9A: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054BA2: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140054BA9: E8 42 63 BC 00     call        0000000140C1AEF0
  0000000140054BAE: 4C 8B C0           mov         r8,rax
  0000000140054BB1: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140054BB6: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140054BB9: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054BC1: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140054BC6: FF 90 78 02 00 00  call        qword ptr [rax+0000000000000278h]
  0000000140054BCC: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054BD4: 48 81 C1 60 01 00  add         rcx,160h
                    00
  0000000140054BDB: E8 10 63 BC 00     call        0000000140C1AEF0
  0000000140054BE0: 48 8B D0           mov         rdx,rax
  0000000140054BE3: 48 8D 0D 16 1B D2  lea         rcx,[0000000143D76700h]
                    03
  0000000140054BEA: E8 F1 9E 27 00     call        00000001402CEAE0
  0000000140054BEF: EB 1E              jmp         0000000140054C0F
  0000000140054BF1: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140054BF6: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140054BF9: 45 33 C0           xor         r8d,r8d
  0000000140054BFC: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140054C04: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140054C09: FF 90 A8 00 00 00  call        qword ptr [rax+00000000000000A8h]
  0000000140054C0F: 48 81 C4 A8 00 00  add         rsp,0A8h
                    00
  0000000140054C16: C3                 ret

  Summary

     1E02000 .text
