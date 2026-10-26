Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091A090: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091A095: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091A09A: 48 81 EC B8 00 00  sub         rsp,0B8h
                    00
  000000014091A0A1: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014091A0AD: 48 8D 44 24 48     lea         rax,[rsp+48h]
  000000014091A0B2: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014091A0B7: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014091A0BF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091A0C4: FF 15 0E B9 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A0CA: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  000000014091A0D2: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014091A0DA: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A0DD: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014091A0E5: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014091A0ED: FF 90 30 01 00 00  call        qword ptr [rax+0000000000000130h]
  000000014091A0F3: 0F B6 C0           movzx       eax,al
  000000014091A0F6: 85 C0              test        eax,eax
  000000014091A0F8: 0F 84 BB 01 00 00  je          000000014091A2B9
  000000014091A0FE: 48 8D 44 24 58     lea         rax,[rsp+58h]
  000000014091A103: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014091A108: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014091A110: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014091A115: FF 15 BD B8 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A11B: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  000000014091A123: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014091A12B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A12E: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  000000014091A136: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014091A13E: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  000000014091A144: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014091A14C: 48 83 C1 38        add         rcx,38h
  000000014091A150: 8B D0              mov         edx,eax
  000000014091A152: E8 B9 6A 84 FF     call        0000000140160C10
  000000014091A157: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A15A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014091A15F: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014091A167: 48 83 C2 38        add         rdx,38h
  000000014091A16B: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091A170: E8 5B F9 93 FF     call        0000000140259AD0
  000000014091A175: 90                 nop
  000000014091A176: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091A17E: EB 0B              jmp         000000014091A18B
  000000014091A180: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091A184: 83 C0 01           add         eax,1
  000000014091A187: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091A18B: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091A190: E8 4B A9 6E FF     call        0000000140004AE0
  000000014091A195: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014091A199: 7D 3D              jge         000000014091A1D8
  000000014091A19B: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091A19F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091A1A4: E8 67 6A 84 FF     call        0000000140160C10
  000000014091A1A9: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A1AC: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014091A1B1: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  000000014091A1B7: 74 1D              je          000000014091A1D6
  000000014091A1B9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091A1BE: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014091A1C3: 48 39 41 40        cmp         qword ptr [rcx+40h],rax
  000000014091A1C7: 75 0D              jne         000000014091A1D6
  000000014091A1C9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014091A1CE: 48 C7 40 40 00 00  mov         qword ptr [rax+40h],0
                    00 00
  000000014091A1D6: EB A8              jmp         000000014091A180
  000000014091A1D8: 48 8D 44 24 68     lea         rax,[rsp+68h]
  000000014091A1DD: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000014091A1E2: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014091A1EA: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014091A1EF: FF 15 E3 B7 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A1F5: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  000000014091A1FD: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014091A205: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A208: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014091A210: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014091A218: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  000000014091A21E: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  000000014091A222: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014091A227: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014091A22F: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A237: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000014091A23C: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  000000014091A242: 74 19              je          000000014091A25D
  000000014091A244: BA 01 00 00 00     mov         edx,1
  000000014091A249: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  000000014091A24E: E8 1D 39 00 00     call        000000014091DB70
  000000014091A253: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  000000014091A25B: EB 0C              jmp         000000014091A269
  000000014091A25D: 48 C7 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],0
                    00 00 00 00 00 00
  000000014091A269: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014091A271: 48 83 C1 38        add         rcx,38h
  000000014091A275: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  000000014091A279: E8 E2 A3 8D FF     call        00000001401F4660
  000000014091A27E: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091A286: EB 0B              jmp         000000014091A293
  000000014091A288: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091A28C: 83 C0 01           add         eax,1
  000000014091A28F: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091A293: 83 7C 24 20 03     cmp         dword ptr [rsp+20h],3
  000000014091A298: 7D 14              jge         000000014091A2AE
  000000014091A29A: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014091A29F: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014091A2A7: C6 44 08 70 01     mov         byte ptr [rax+rcx+70h],1
  000000014091A2AC: EB DA              jmp         000000014091A288
  000000014091A2AE: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091A2B3: E8 E8 DE 8E FF     call        00000001402081A0
  000000014091A2B8: 90                 nop
  000000014091A2B9: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014091A2C1: FF 15 19 B7 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091A2C7: 48 81 C4 B8 00 00  add         rsp,0B8h
                    00
  000000014091A2CE: C3                 ret

  Summary

     1E02000 .text
