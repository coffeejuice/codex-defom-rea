Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDFBD0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CDFBD5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDFBDA: 48 83 EC 78        sub         rsp,78h
  0000000140CDFBDE: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFBE6: 48 83 C1 30        add         rcx,30h
  0000000140CDFBEA: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CDFBF2: E8 F9 8A 01 00     call        0000000140CF86F0
  0000000140CDFBF7: 88 44 24 41        mov         byte ptr [rsp+41h],al
  0000000140CDFBFB: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFC03: 48 83 C1 30        add         rcx,30h
  0000000140CDFC07: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CDFC0F: E8 0C CD FD FF     call        0000000140CBC920
  0000000140CDFC14: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFC1C: 48 83 C1 30        add         rcx,30h
  0000000140CDFC20: E8 CB B2 F3 FF     call        0000000140C1AEF0
  0000000140CDFC25: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFC2D: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140CDFC31: 48 8B D0           mov         rdx,rax
  0000000140CDFC34: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140CDFC38: E8 F3 19 0D 00     call        0000000140DB1630
  0000000140CDFC3D: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFC45: 48 83 C1 30        add         rcx,30h
  0000000140CDFC49: E8 A2 B2 F3 FF     call        0000000140C1AEF0
  0000000140CDFC4E: 48 85 C0           test        rax,rax
  0000000140CDFC51: 75 2F              jne         0000000140CDFC82
  0000000140CDFC53: 33 D2              xor         edx,edx
  0000000140CDFC55: 48 8D 0D A4 6A 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CDFC5C: E8 7F EE 5E FF     call        00000001402CEAE0
  0000000140CDFC61: 33 D2              xor         edx,edx
  0000000140CDFC63: 48 8D 0D 96 6A 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CDFC6A: E8 51 ED 5E FF     call        00000001402CE9C0
  0000000140CDFC6F: 33 D2              xor         edx,edx
  0000000140CDFC71: 48 8D 0D 88 6A 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CDFC78: E8 13 EB 5E FF     call        00000001402CE790
  0000000140CDFC7D: E9 70 02 00 00     jmp         0000000140CDFEF2
  0000000140CDFC82: B2 01              mov         dl,1
  0000000140CDFC84: 48 8D 0D 2D 6A 09  lea         rcx,[0000000143D766B8h]
                    03
  0000000140CDFC8B: E8 10 AB 5E FF     call        00000001402CA7A0
  0000000140CDFC90: 33 D2              xor         edx,edx
  0000000140CDFC92: 48 8D 0D 67 6A 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CDFC99: E8 F2 EA 5E FF     call        00000001402CE790
  0000000140CDFC9E: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFCA6: 48 83 C1 30        add         rcx,30h
  0000000140CDFCAA: E8 41 B2 F3 FF     call        0000000140C1AEF0
  0000000140CDFCAF: 48 8B C8           mov         rcx,rax
  0000000140CDFCB2: E8 89 B1 FF FF     call        0000000140CDAE40
  0000000140CDFCB7: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFCBF: 48 83 C1 30        add         rcx,30h
  0000000140CDFCC3: E8 28 B2 F3 FF     call        0000000140C1AEF0
  0000000140CDFCC8: 48 8B C8           mov         rcx,rax
  0000000140CDFCCB: E8 B0 B3 FF FF     call        0000000140CDB080
  0000000140CDFCD0: 0F B6 44 24 41     movzx       eax,byte ptr [rsp+41h]
  0000000140CDFCD5: 85 C0              test        eax,eax
  0000000140CDFCD7: 74 21              je          0000000140CDFCFA
  0000000140CDFCD9: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFCE1: 48 83 C1 30        add         rcx,30h
  0000000140CDFCE5: E8 06 B2 F3 FF     call        0000000140C1AEF0
  0000000140CDFCEA: 48 8B C8           mov         rcx,rax
  0000000140CDFCED: E8 7E AE FF FF     call        0000000140CDAB70
  0000000140CDFCF2: 48 8B C8           mov         rcx,rax
  0000000140CDFCF5: E8 56 9C 05 00     call        0000000140D39950
  0000000140CDFCFA: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFD02: 48 83 C1 30        add         rcx,30h
  0000000140CDFD06: E8 E5 B1 F3 FF     call        0000000140C1AEF0
  0000000140CDFD0B: 48 8B C8           mov         rcx,rax
  0000000140CDFD0E: E8 5D AE FF FF     call        0000000140CDAB70
  0000000140CDFD13: 33 D2              xor         edx,edx
  0000000140CDFD15: 48 8B C8           mov         rcx,rax
  0000000140CDFD18: E8 83 9A 05 00     call        0000000140D397A0
  0000000140CDFD1D: 88 44 24 40        mov         byte ptr [rsp+40h],al
  0000000140CDFD21: 33 D2              xor         edx,edx
  0000000140CDFD23: 48 8D 0D 8E 69 09  lea         rcx,[0000000143D766B8h]
                    03
  0000000140CDFD2A: E8 71 AA 5E FF     call        00000001402CA7A0
  0000000140CDFD2F: 0F B6 44 24 40     movzx       eax,byte ptr [rsp+40h]
  0000000140CDFD34: 85 C0              test        eax,eax
  0000000140CDFD36: 75 2F              jne         0000000140CDFD67
  0000000140CDFD38: 33 D2              xor         edx,edx
  0000000140CDFD3A: 48 8D 0D BF 69 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CDFD41: E8 9A ED 5E FF     call        00000001402CEAE0
  0000000140CDFD46: 33 D2              xor         edx,edx
  0000000140CDFD48: 48 8D 0D B1 69 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CDFD4F: E8 6C EC 5E FF     call        00000001402CE9C0
  0000000140CDFD54: 33 D2              xor         edx,edx
  0000000140CDFD56: 48 8D 0D A3 69 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CDFD5D: E8 2E EA 5E FF     call        00000001402CE790
  0000000140CDFD62: E9 8B 01 00 00     jmp         0000000140CDFEF2
  0000000140CDFD67: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFD6F: 48 83 C1 30        add         rcx,30h
  0000000140CDFD73: E8 78 B1 F3 FF     call        0000000140C1AEF0
  0000000140CDFD78: 48 8B C8           mov         rcx,rax
  0000000140CDFD7B: E8 E0 D9 FF FF     call        0000000140CDD760
  0000000140CDFD80: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CDFD85: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140CDFD8B: 75 05              jne         0000000140CDFD92
  0000000140CDFD8D: E9 60 01 00 00     jmp         0000000140CDFEF2
  0000000140CDFD92: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140CDFD97: E8 F4 07 08 00     call        0000000140D60590
  0000000140CDFD9C: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140CDFDA0: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  0000000140CDFDA5: 75 08              jne         0000000140CDFDAF
  0000000140CDFDA7: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  0000000140CDFDAF: C6 44 24 42 01     mov         byte ptr [rsp+42h],1
  0000000140CDFDB4: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFDBC: 48 83 C1 30        add         rcx,30h
  0000000140CDFDC0: E8 2B B1 F3 FF     call        0000000140C1AEF0
  0000000140CDFDC5: 44 0F B6 44 24 42  movzx       r8d,byte ptr [rsp+42h]
  0000000140CDFDCB: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  0000000140CDFDCF: 48 8B C8           mov         rcx,rax
  0000000140CDFDD2: E8 A9 C3 FF FF     call        0000000140CDC180
  0000000140CDFDD7: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CDFDDC: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140CDFDE2: 75 05              jne         0000000140CDFDE9
  0000000140CDFDE4: E9 09 01 00 00     jmp         0000000140CDFEF2
  0000000140CDFDE9: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  0000000140CDFDED: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140CDFDF2: E8 A9 05 08 00     call        0000000140D603A0
  0000000140CDFDF7: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140CDFDFC: E8 7F 9F 5C FF     call        00000001402A9D80
  0000000140CDFE01: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  0000000140CDFE05: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFE0D: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140CDFE11: 8B 54 24 50        mov         edx,dword ptr [rsp+50h]
  0000000140CDFE15: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  0000000140CDFE1C: FF 15 06 84 12 01  call        qword ptr [0000000141E08228h]
  0000000140CDFE22: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CDFE27: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140CDFE2D: 74 0D              je          0000000140CDFE3C
  0000000140CDFE2F: B2 01              mov         dl,1
  0000000140CDFE31: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140CDFE36: FF 15 5C 74 12 01  call        qword ptr [0000000141E07298h]
  0000000140CDFE3C: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140CDFE41: 48 8D 0D B8 68 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CDFE48: E8 43 E9 5E FF     call        00000001402CE790
  0000000140CDFE4D: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFE55: E8 C6 26 00 00     call        0000000140CE2520
  0000000140CDFE5A: 48 8D 0D 57 68 09  lea         rcx,[0000000143D766B8h]
                    03
  0000000140CDFE61: E8 CA B9 5E FF     call        00000001402CB830
  0000000140CDFE66: 0F B6 C0           movzx       eax,al
  0000000140CDFE69: 85 C0              test        eax,eax
  0000000140CDFE6B: 74 32              je          0000000140CDFE9F
  0000000140CDFE6D: C6 44 24 60 01     mov         byte ptr [rsp+60h],1
  0000000140CDFE72: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140CDFE77: E8 94 03 FB FF     call        0000000140C90210
  0000000140CDFE7C: 0F B6 54 24 60     movzx       edx,byte ptr [rsp+60h]
  0000000140CDFE81: 48 8B C8           mov         rcx,rax
  0000000140CDFE84: E8 57 AC 09 00     call        0000000140D7AAE0
  0000000140CDFE89: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CDFE8E: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140CDFE93: 48 8D 0D 66 68 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CDFE9A: E8 41 EC 5E FF     call        00000001402CEAE0
  0000000140CDFE9F: 48 8D 0D 12 68 09  lea         rcx,[0000000143D766B8h]
                    03
  0000000140CDFEA6: E8 E5 81 5E FF     call        00000001402C8090
  0000000140CDFEAB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CDFEB0: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140CDFEB6: 74 18              je          0000000140CDFED0
  0000000140CDFEB8: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140CDFEBD: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CDFEC0: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140CDFEC5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CDFECA: FF 90 18 07 00 00  call        qword ptr [rax+0000000000000718h]
  0000000140CDFED0: 48 8D 0D E1 67 09  lea         rcx,[0000000143D766B8h]
                    03
  0000000140CDFED7: E8 C4 B8 5E FF     call        00000001402CB7A0
  0000000140CDFEDC: 83 F8 01           cmp         eax,1
  0000000140CDFEDF: 75 11              jne         0000000140CDFEF2
  0000000140CDFEE1: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  0000000140CDFEE5: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDFEED: E8 BE 00 00 00     call        0000000140CDFFB0
  0000000140CDFEF2: 48 83 C4 78        add         rsp,78h
  0000000140CDFEF6: C3                 ret

  Summary

     1E02000 .text
