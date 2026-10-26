Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D79750: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140D79755: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D7975A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D7975F: 48 81 EC 58 03 00  sub         rsp,358h
                    00
  0000000140D79766: 48 C7 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D79772: 48 8B 84 24 60 03  mov         rax,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D7977A: 48 8B 40 50        mov         rax,qword ptr [rax+50h]
  0000000140D7977E: 48 39 84 24 68 03  cmp         qword ptr [rsp+0000000000000368h],rax
                    00 00
  0000000140D79786: 75 0D              jne         0000000140D79795
  0000000140D79788: C7 84 24 40 02 00  mov         dword ptr [rsp+0000000000000240h],1
                    00 01 00 00 00
  0000000140D79793: EB 0B              jmp         0000000140D797A0
  0000000140D79795: C7 84 24 40 02 00  mov         dword ptr [rsp+0000000000000240h],0
                    00 00 00 00 00
  0000000140D797A0: 0F B6 84 24 40 02  movzx       eax,byte ptr [rsp+0000000000000240h]
                    00 00
  0000000140D797A8: 88 44 24 48        mov         byte ptr [rsp+48h],al
  0000000140D797AC: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D797B1: E8 2A 62 41 FF     call        000000014018F9E0
  0000000140D797B6: 90                 nop
  0000000140D797B7: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  0000000140D797C3: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00 00
  0000000140D797CF: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000140D797D8: C7 44 24 4C 00 00  mov         dword ptr [rsp+4Ch],0
                    00 00
  0000000140D797E0: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140D797E8: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D797F0: E8 AB 29 F0 FF     call        0000000140C7C1A0
  0000000140D797F5: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  0000000140D797F9: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00
  0000000140D79804: B2 01              mov         dl,1
  0000000140D79806: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D7980E: E8 8D 19 F0 FF     call        0000000140C7B1A0
  0000000140D79813: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140D79817: C6 44 24 49 00     mov         byte ptr [rsp+49h],0
  0000000140D7981C: C6 44 24 68 00     mov         byte ptr [rsp+68h],0
  0000000140D79821: 48 C7 44 24 58 00  mov         qword ptr [rsp+58h],0
                    00 00 00
  0000000140D7982A: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  0000000140D79833: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00 00
  0000000140D7983F: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140D79848: C6 44 24 69 00     mov         byte ptr [rsp+69h],0
  0000000140D7984D: C6 44 24 34 00     mov         byte ptr [rsp+34h],0
  0000000140D79852: 48 8B 84 24 60 03  mov         rax,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D7985A: 48 83 78 50 00     cmp         qword ptr [rax+50h],0
  0000000140D7985F: 74 53              je          0000000140D798B4
  0000000140D79861: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D79869: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D7986D: E8 2E 28 F0 FF     call        0000000140C7C0A0
  0000000140D79872: 88 44 24 69        mov         byte ptr [rsp+69h],al
  0000000140D79876: 0F B6 44 24 69     movzx       eax,byte ptr [rsp+69h]
  0000000140D7987B: 85 C0              test        eax,eax
  0000000140D7987D: 75 35              jne         0000000140D798B4
  0000000140D7987F: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D79887: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D7988B: E8 70 15 F0 FF     call        0000000140C7AE00
  0000000140D79890: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140D79898: 48 83 BC 24 A0 00  cmp         qword ptr [rsp+00000000000000A0h],0
                    00 00 00
  0000000140D798A1: 74 11              je          0000000140D798B4
  0000000140D798A3: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140D798AB: E8 70 30 F9 FF     call        0000000140D0C920
  0000000140D798B0: 88 44 24 34        mov         byte ptr [rsp+34h],al
  0000000140D798B4: C7 44 24 4C 00 00  mov         dword ptr [rsp+4Ch],0
                    00 00
  0000000140D798BC: EB 0B              jmp         0000000140D798C9
  0000000140D798BE: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140D798C2: 83 C0 01           add         eax,1
  0000000140D798C5: 89 44 24 4C        mov         dword ptr [rsp+4Ch],eax
  0000000140D798C9: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  0000000140D798CD: 39 44 24 4C        cmp         dword ptr [rsp+4Ch],eax
  0000000140D798D1: 0F 8D 10 0B 00 00  jge         0000000140D7A3E7
  0000000140D798D7: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140D798DB: 83 C0 01           add         eax,1
  0000000140D798DE: 89 84 24 D0 00 00  mov         dword ptr [rsp+00000000000000D0h],eax
                    00
  0000000140D798E5: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  0000000140D798ED: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D798F5: E8 56 94 F0 FF     call        0000000140C82D50
  0000000140D798FA: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140D79902: 48 83 BC 24 80 00  cmp         qword ptr [rsp+0000000000000080h],0
                    00 00 00
  0000000140D7990B: 75 02              jne         0000000140D7990F
  0000000140D7990D: EB AF              jmp         0000000140D798BE
  0000000140D7990F: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D79917: E8 64 82 47 00     call        00000001411F1B80
  0000000140D7991C: 0F B6 C0           movzx       eax,al
  0000000140D7991F: 85 C0              test        eax,eax
  0000000140D79921: 75 02              jne         0000000140D79925
  0000000140D79923: EB 99              jmp         0000000140D798BE
  0000000140D79925: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D7992D: E8 7E 81 47 00     call        00000001411F1AB0
  0000000140D79932: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  0000000140D79939: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D7993E: E8 4D 42 49 FF     call        000000014020DB90
  0000000140D79943: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D7994B: E8 A0 80 47 00     call        00000001411F19F0
  0000000140D79950: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140D79958: 48 83 BC 24 88 00  cmp         qword ptr [rsp+0000000000000088h],0
                    00 00 00
  0000000140D79961: 75 05              jne         0000000140D79968
  0000000140D79963: E9 56 FF FF FF     jmp         0000000140D798BE
  0000000140D79968: B9 38 00 00 00     mov         ecx,38h
  0000000140D7996D: E8 4A 51 6F 00     call        000000014146EABC
  0000000140D79972: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140D7997A: 48 83 BC 24 E0 00  cmp         qword ptr [rsp+00000000000000E0h],0
                    00 00 00
  0000000140D79983: 74 2F              je          0000000140D799B4
  0000000140D79985: 4C 8B 84 24 88 00  mov         r8,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D7998D: 49 83 C0 0C        add         r8,0Ch
  0000000140D79991: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D79999: 48 83 C2 08        add         rdx,8
  0000000140D7999D: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140D799A5: E8 A6 26 FD FF     call        0000000140D4C050
  0000000140D799AA: 48 89 84 24 48 02  mov         qword ptr [rsp+0000000000000248h],rax
                    00 00
  0000000140D799B2: EB 0C              jmp         0000000140D799C0
  0000000140D799B4: 48 C7 84 24 48 02  mov         qword ptr [rsp+0000000000000248h],0
                    00 00 00 00 00 00
  0000000140D799C0: 48 8B 84 24 48 02  mov         rax,qword ptr [rsp+0000000000000248h]
                    00 00
  0000000140D799C8: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000140D799D0: 48 8B 84 24 D8 00  mov         rax,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140D799D8: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140D799DD: 48 8D 94 24 E8 00  lea         rdx,[rsp+00000000000000E8h]
                    00 00
  0000000140D799E5: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D799ED: E8 FE 80 47 00     call        00000001411F1AF0
  0000000140D799F2: 48 89 84 24 50 02  mov         qword ptr [rsp+0000000000000250h],rax
                    00 00
  0000000140D799FA: 48 8B 84 24 50 02  mov         rax,qword ptr [rsp+0000000000000250h]
                    00 00
  0000000140D79A02: 48 89 84 24 58 02  mov         qword ptr [rsp+0000000000000258h],rax
                    00 00
  0000000140D79A0A: 48 8B 94 24 58 02  mov         rdx,qword ptr [rsp+0000000000000258h]
                    00 00
  0000000140D79A12: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D79A17: E8 E4 50 FD FF     call        0000000140D4EB00
  0000000140D79A1C: 90                 nop
  0000000140D79A1D: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140D79A25: FF 15 B5 BF 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D79A2B: 48 8D 84 24 F0 00  lea         rax,[rsp+00000000000000F0h]
                    00 00
  0000000140D79A33: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  0000000140D79A3B: 48 8B 94 24 F8 00  mov         rdx,qword ptr [rsp+00000000000000F8h]
                    00 00
  0000000140D79A43: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D79A4B: E8 C0 78 F1 FF     call        0000000140C91310
  0000000140D79A50: 48 89 84 24 60 02  mov         qword ptr [rsp+0000000000000260h],rax
                    00 00
  0000000140D79A58: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  0000000140D79A60: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D79A68: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  0000000140D79A6C: 4C 8B 84 24 60 02  mov         r8,qword ptr [rsp+0000000000000260h]
                    00 00
  0000000140D79A74: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140D79A79: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D79A81: E8 EA 12 00 00     call        0000000140D7AD70
  0000000140D79A86: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140D79A8B: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  0000000140D79A93: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  0000000140D79A9B: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D79AA0: E8 3B 3D 9B 00     call        000000014172D7E0
  0000000140D79AA5: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D79AAD: E8 8E B4 AB FF     call        0000000140834F40
  0000000140D79AB2: 0F B6 C0           movzx       eax,al
  0000000140D79AB5: 85 C0              test        eax,eax
  0000000140D79AB7: 74 7F              je          0000000140D79B38
  0000000140D79AB9: B9 30 00 00 00     mov         ecx,30h
  0000000140D79ABE: E8 F9 4F 6F 00     call        000000014146EABC
  0000000140D79AC3: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  0000000140D79ACB: 48 83 BC 24 10 01  cmp         qword ptr [rsp+0000000000000110h],0
                    00 00 00
  0000000140D79AD4: 74 23              je          0000000140D79AF9
  0000000140D79AD6: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D79ADE: 48 83 C2 08        add         rdx,8
  0000000140D79AE2: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D79AEA: E8 81 30 FD FF     call        0000000140D4CB70
  0000000140D79AEF: 48 89 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],rax
                    00 00
  0000000140D79AF7: EB 0C              jmp         0000000140D79B05
  0000000140D79AF9: 48 C7 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],0
                    00 00 00 00 00 00
  0000000140D79B05: 48 8B 84 24 68 02  mov         rax,qword ptr [rsp+0000000000000268h]
                    00 00
  0000000140D79B0D: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  0000000140D79B15: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140D79B1D: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140D79B25: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140D79B2D: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D79B32: FF 15 80 1A 09 01  call        qword ptr [0000000141E0B5B8h]
  0000000140D79B38: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D79B40: E8 9B B3 AB FF     call        0000000140834EE0
  0000000140D79B45: 0F B6 C0           movzx       eax,al
  0000000140D79B48: 85 C0              test        eax,eax
  0000000140D79B4A: 74 7F              je          0000000140D79BCB
  0000000140D79B4C: B9 30 00 00 00     mov         ecx,30h
  0000000140D79B51: E8 66 4F 6F 00     call        000000014146EABC
  0000000140D79B56: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  0000000140D79B5E: 48 83 BC 24 20 01  cmp         qword ptr [rsp+0000000000000120h],0
                    00 00 00
  0000000140D79B67: 74 23              je          0000000140D79B8C
  0000000140D79B69: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D79B71: 48 83 C2 08        add         rdx,8
  0000000140D79B75: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140D79B7D: E8 7E 31 FD FF     call        0000000140D4CD00
  0000000140D79B82: 48 89 84 24 70 02  mov         qword ptr [rsp+0000000000000270h],rax
                    00 00
  0000000140D79B8A: EB 0C              jmp         0000000140D79B98
  0000000140D79B8C: 48 C7 84 24 70 02  mov         qword ptr [rsp+0000000000000270h],0
                    00 00 00 00 00 00
  0000000140D79B98: 48 8B 84 24 70 02  mov         rax,qword ptr [rsp+0000000000000270h]
                    00 00
  0000000140D79BA0: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  0000000140D79BA8: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140D79BB0: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140D79BB8: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140D79BC0: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D79BC5: FF 15 ED 19 09 01  call        qword ptr [0000000141E0B5B8h]
  0000000140D79BCB: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140D79BD0: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D79BD5: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D79BDD: E8 2E 7E 47 00     call        00000001411F1A10
  0000000140D79BE2: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140D79BE7: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140D79BED: 75 18              jne         0000000140D79C07
  0000000140D79BEF: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140D79BF4: 48 8B 8C 24 70 03  mov         rcx,qword ptr [rsp+0000000000000370h]
                    00 00
  0000000140D79BFC: FF 15 4E 1A 09 01  call        qword ptr [0000000141E0B650h]
  0000000140D79C02: E9 B7 FC FF FF     jmp         0000000140D798BE
  0000000140D79C07: B9 10 00 00 00     mov         ecx,10h
  0000000140D79C0C: E8 AB 4E 6F 00     call        000000014146EABC
  0000000140D79C11: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  0000000140D79C19: 48 83 BC 24 30 01  cmp         qword ptr [rsp+0000000000000130h],0
                    00 00 00
  0000000140D79C22: 74 32              je          0000000140D79C56
  0000000140D79C24: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140D79C2C: FF 15 3E 1A 09 01  call        qword ptr [0000000141E0B670h]
  0000000140D79C32: 4C 8B 9C 24 30 01  mov         r11,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140D79C3A: 48 8D 05 D7 4E 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D79C41: 49 89 03           mov         qword ptr [r11],rax
  0000000140D79C44: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140D79C4C: 48 89 84 24 78 02  mov         qword ptr [rsp+0000000000000278h],rax
                    00 00
  0000000140D79C54: EB 0C              jmp         0000000140D79C62
  0000000140D79C56: 48 C7 84 24 78 02  mov         qword ptr [rsp+0000000000000278h],0
                    00 00 00 00 00 00
  0000000140D79C62: 48 8B 84 24 78 02  mov         rax,qword ptr [rsp+0000000000000278h]
                    00 00
  0000000140D79C6A: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  0000000140D79C72: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140D79C7A: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D79C82: B2 01              mov         dl,1
  0000000140D79C84: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D79C8C: FF 15 E6 19 09 01  call        qword ptr [0000000141E0B678h]
  0000000140D79C92: 33 D2              xor         edx,edx
  0000000140D79C94: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D79C99: E8 32 4F 47 00     call        00000001411EEBD0
  0000000140D79C9E: 8B D0              mov         edx,eax
  0000000140D79CA0: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D79CA8: FF 15 92 F5 08 01  call        qword ptr [0000000141E09240h]
  0000000140D79CAE: 48 8D 84 24 38 01  lea         rax,[rsp+0000000000000138h]
                    00 00
  0000000140D79CB6: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  0000000140D79CBE: 48 8B 94 24 40 01  mov         rdx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140D79CC6: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D79CCE: E8 3D 76 F1 FF     call        0000000140C91310
  0000000140D79CD3: 48 89 84 24 80 02  mov         qword ptr [rsp+0000000000000280h],rax
                    00 00
  0000000140D79CDB: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140D79CE3: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D79CEB: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  0000000140D79CEF: 4C 8B 84 24 80 02  mov         r8,qword ptr [rsp+0000000000000280h]
                    00 00
  0000000140D79CF7: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D79CFF: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D79D07: E8 64 10 00 00     call        0000000140D7AD70
  0000000140D79D0C: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140D79D14: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D79D19: E8 C2 3A 9B 00     call        000000014172D7E0
  0000000140D79D1E: B9 10 00 00 00     mov         ecx,10h
  0000000140D79D23: E8 94 4D 6F 00     call        000000014146EABC
  0000000140D79D28: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  0000000140D79D30: 48 83 BC 24 50 01  cmp         qword ptr [rsp+0000000000000150h],0
                    00 00 00
  0000000140D79D39: 74 32              je          0000000140D79D6D
  0000000140D79D3B: 48 8B 8C 24 50 01  mov         rcx,qword ptr [rsp+0000000000000150h]
                    00 00
  0000000140D79D43: FF 15 27 19 09 01  call        qword ptr [0000000141E0B670h]
  0000000140D79D49: 4C 8B 9C 24 50 01  mov         r11,qword ptr [rsp+0000000000000150h]
                    00 00
  0000000140D79D51: 48 8D 05 C0 4D 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D79D58: 49 89 03           mov         qword ptr [r11],rax
  0000000140D79D5B: 48 8B 84 24 50 01  mov         rax,qword ptr [rsp+0000000000000150h]
                    00 00
  0000000140D79D63: 48 89 84 24 88 02  mov         qword ptr [rsp+0000000000000288h],rax
                    00 00
  0000000140D79D6B: EB 0C              jmp         0000000140D79D79
  0000000140D79D6D: 48 C7 84 24 88 02  mov         qword ptr [rsp+0000000000000288h],0
                    00 00 00 00 00 00
  0000000140D79D79: 48 8B 84 24 88 02  mov         rax,qword ptr [rsp+0000000000000288h]
                    00 00
  0000000140D79D81: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  0000000140D79D89: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140D79D91: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D79D99: B2 01              mov         dl,1
  0000000140D79D9B: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D79DA3: FF 15 CF 18 09 01  call        qword ptr [0000000141E0B678h]
  0000000140D79DA9: BA 01 00 00 00     mov         edx,1
  0000000140D79DAE: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D79DB3: E8 18 4E 47 00     call        00000001411EEBD0
  0000000140D79DB8: 8B D0              mov         edx,eax
  0000000140D79DBA: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D79DC2: FF 15 78 F4 08 01  call        qword ptr [0000000141E09240h]
  0000000140D79DC8: 48 8D 84 24 58 01  lea         rax,[rsp+0000000000000158h]
                    00 00
  0000000140D79DD0: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  0000000140D79DD8: 48 8B 94 24 60 01  mov         rdx,qword ptr [rsp+0000000000000160h]
                    00 00
  0000000140D79DE0: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D79DE8: E8 23 75 F1 FF     call        0000000140C91310
  0000000140D79DED: 48 89 84 24 90 02  mov         qword ptr [rsp+0000000000000290h],rax
                    00 00
  0000000140D79DF5: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140D79DFD: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D79E05: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  0000000140D79E09: 4C 8B 84 24 90 02  mov         r8,qword ptr [rsp+0000000000000290h]
                    00 00
  0000000140D79E11: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D79E19: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D79E21: E8 4A 0F 00 00     call        0000000140D7AD70
  0000000140D79E26: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140D79E2E: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D79E33: E8 A8 39 9B 00     call        000000014172D7E0
  0000000140D79E38: B9 10 00 00 00     mov         ecx,10h
  0000000140D79E3D: E8 7A 4C 6F 00     call        000000014146EABC
  0000000140D79E42: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  0000000140D79E4A: 48 83 BC 24 70 01  cmp         qword ptr [rsp+0000000000000170h],0
                    00 00 00
  0000000140D79E53: 74 32              je          0000000140D79E87
  0000000140D79E55: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D79E5D: FF 15 0D 18 09 01  call        qword ptr [0000000141E0B670h]
  0000000140D79E63: 4C 8B 9C 24 70 01  mov         r11,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D79E6B: 48 8D 05 A6 4C 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D79E72: 49 89 03           mov         qword ptr [r11],rax
  0000000140D79E75: 48 8B 84 24 70 01  mov         rax,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D79E7D: 48 89 84 24 98 02  mov         qword ptr [rsp+0000000000000298h],rax
                    00 00
  0000000140D79E85: EB 0C              jmp         0000000140D79E93
  0000000140D79E87: 48 C7 84 24 98 02  mov         qword ptr [rsp+0000000000000298h],0
                    00 00 00 00 00 00
  0000000140D79E93: 48 8B 84 24 98 02  mov         rax,qword ptr [rsp+0000000000000298h]
                    00 00
  0000000140D79E9B: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  0000000140D79EA3: 48 8B 84 24 68 01  mov         rax,qword ptr [rsp+0000000000000168h]
                    00 00
  0000000140D79EAB: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D79EB3: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D79EBB: E8 00 7D 47 00     call        00000001411F1BC0
  0000000140D79EC0: 0F B6 C0           movzx       eax,al
  0000000140D79EC3: 85 C0              test        eax,eax
  0000000140D79EC5: 0F 84 8F 00 00 00  je          0000000140D79F5A
  0000000140D79ECB: B2 01              mov         dl,1
  0000000140D79ECD: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D79ED5: FF 15 9D 17 09 01  call        qword ptr [0000000141E0B678h]
  0000000140D79EDB: BA 02 00 00 00     mov         edx,2
  0000000140D79EE0: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D79EE5: E8 E6 4C 47 00     call        00000001411EEBD0
  0000000140D79EEA: 8B D0              mov         edx,eax
  0000000140D79EEC: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D79EF4: FF 15 46 F3 08 01  call        qword ptr [0000000141E09240h]
  0000000140D79EFA: 48 8D 84 24 78 01  lea         rax,[rsp+0000000000000178h]
                    00 00
  0000000140D79F02: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  0000000140D79F0A: 48 8B 94 24 80 01  mov         rdx,qword ptr [rsp+0000000000000180h]
                    00 00
  0000000140D79F12: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D79F1A: E8 F1 73 F1 FF     call        0000000140C91310
  0000000140D79F1F: 48 89 84 24 A0 02  mov         qword ptr [rsp+00000000000002A0h],rax
                    00 00
  0000000140D79F27: C7 44 24 20 02 00  mov         dword ptr [rsp+20h],2
                    00 00
  0000000140D79F2F: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D79F37: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  0000000140D79F3B: 4C 8B 84 24 A0 02  mov         r8,qword ptr [rsp+00000000000002A0h]
                    00 00
  0000000140D79F43: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D79F4B: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D79F53: E8 18 0E 00 00     call        0000000140D7AD70
  0000000140D79F58: EB 10              jmp         0000000140D79F6A
  0000000140D79F5A: 33 D2              xor         edx,edx
  0000000140D79F5C: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D79F64: FF 15 0E 17 09 01  call        qword ptr [0000000141E0B678h]
  0000000140D79F6A: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140D79F72: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D79F77: E8 64 38 9B 00     call        000000014172D7E0
  0000000140D79F7C: B9 10 00 00 00     mov         ecx,10h
  0000000140D79F81: E8 36 4B 6F 00     call        000000014146EABC
  0000000140D79F86: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  0000000140D79F8E: 48 83 BC 24 90 01  cmp         qword ptr [rsp+0000000000000190h],0
                    00 00 00
  0000000140D79F97: 74 32              je          0000000140D79FCB
  0000000140D79F99: 48 8B 8C 24 90 01  mov         rcx,qword ptr [rsp+0000000000000190h]
                    00 00
  0000000140D79FA1: FF 15 C9 16 09 01  call        qword ptr [0000000141E0B670h]
  0000000140D79FA7: 4C 8B 9C 24 90 01  mov         r11,qword ptr [rsp+0000000000000190h]
                    00 00
  0000000140D79FAF: 48 8D 05 62 4B 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D79FB6: 49 89 03           mov         qword ptr [r11],rax
  0000000140D79FB9: 48 8B 84 24 90 01  mov         rax,qword ptr [rsp+0000000000000190h]
                    00 00
  0000000140D79FC1: 48 89 84 24 A8 02  mov         qword ptr [rsp+00000000000002A8h],rax
                    00 00
  0000000140D79FC9: EB 0C              jmp         0000000140D79FD7
  0000000140D79FCB: 48 C7 84 24 A8 02  mov         qword ptr [rsp+00000000000002A8h],0
                    00 00 00 00 00 00
  0000000140D79FD7: 48 8B 84 24 A8 02  mov         rax,qword ptr [rsp+00000000000002A8h]
                    00 00
  0000000140D79FDF: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  0000000140D79FE7: 48 8B 84 24 88 01  mov         rax,qword ptr [rsp+0000000000000188h]
                    00 00
  0000000140D79FEF: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D79FF7: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D79FFF: E8 FC 7B 47 00     call        00000001411F1C00
  0000000140D7A004: 0F B6 C0           movzx       eax,al
  0000000140D7A007: 85 C0              test        eax,eax
  0000000140D7A009: 0F 84 8F 00 00 00  je          0000000140D7A09E
  0000000140D7A00F: B2 01              mov         dl,1
  0000000140D7A011: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A019: FF 15 59 16 09 01  call        qword ptr [0000000141E0B678h]
  0000000140D7A01F: BA 03 00 00 00     mov         edx,3
  0000000140D7A024: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D7A029: E8 A2 4B 47 00     call        00000001411EEBD0
  0000000140D7A02E: 8B D0              mov         edx,eax
  0000000140D7A030: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A038: FF 15 02 F2 08 01  call        qword ptr [0000000141E09240h]
  0000000140D7A03E: 48 8D 84 24 98 01  lea         rax,[rsp+0000000000000198h]
                    00 00
  0000000140D7A046: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  0000000140D7A04E: 48 8B 94 24 A0 01  mov         rdx,qword ptr [rsp+00000000000001A0h]
                    00 00
  0000000140D7A056: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D7A05E: E8 AD 72 F1 FF     call        0000000140C91310
  0000000140D7A063: 48 89 84 24 B0 02  mov         qword ptr [rsp+00000000000002B0h],rax
                    00 00
  0000000140D7A06B: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D7A073: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D7A07B: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  0000000140D7A07F: 4C 8B 84 24 B0 02  mov         r8,qword ptr [rsp+00000000000002B0h]
                    00 00
  0000000140D7A087: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A08F: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D7A097: E8 D4 0C 00 00     call        0000000140D7AD70
  0000000140D7A09C: EB 10              jmp         0000000140D7A0AE
  0000000140D7A09E: 33 D2              xor         edx,edx
  0000000140D7A0A0: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A0A8: FF 15 CA 15 09 01  call        qword ptr [0000000141E0B678h]
  0000000140D7A0AE: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140D7A0B6: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D7A0BB: E8 20 37 9B 00     call        000000014172D7E0
  0000000140D7A0C0: 0F B6 44 24 69     movzx       eax,byte ptr [rsp+69h]
  0000000140D7A0C5: 85 C0              test        eax,eax
  0000000140D7A0C7: 75 0D              jne         0000000140D7A0D6
  0000000140D7A0C9: 0F B6 44 24 34     movzx       eax,byte ptr [rsp+34h]
  0000000140D7A0CE: 85 C0              test        eax,eax
  0000000140D7A0D0: 0F 84 1A 01 00 00  je          0000000140D7A1F0
  0000000140D7A0D6: B9 10 00 00 00     mov         ecx,10h
  0000000140D7A0DB: E8 DC 49 6F 00     call        000000014146EABC
  0000000140D7A0E0: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  0000000140D7A0E8: 48 83 BC 24 B0 01  cmp         qword ptr [rsp+00000000000001B0h],0
                    00 00 00
  0000000140D7A0F1: 74 32              je          0000000140D7A125
  0000000140D7A0F3: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D7A0FB: FF 15 6F 15 09 01  call        qword ptr [0000000141E0B670h]
  0000000140D7A101: 4C 8B 9C 24 B0 01  mov         r11,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D7A109: 48 8D 05 08 4A 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D7A110: 49 89 03           mov         qword ptr [r11],rax
  0000000140D7A113: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D7A11B: 48 89 84 24 B8 02  mov         qword ptr [rsp+00000000000002B8h],rax
                    00 00
  0000000140D7A123: EB 0C              jmp         0000000140D7A131
  0000000140D7A125: 48 C7 84 24 B8 02  mov         qword ptr [rsp+00000000000002B8h],0
                    00 00 00 00 00 00
  0000000140D7A131: 48 8B 84 24 B8 02  mov         rax,qword ptr [rsp+00000000000002B8h]
                    00 00
  0000000140D7A139: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  0000000140D7A141: 48 8B 84 24 A8 01  mov         rax,qword ptr [rsp+00000000000001A8h]
                    00 00
  0000000140D7A149: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D7A151: B2 01              mov         dl,1
  0000000140D7A153: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A15B: FF 15 17 15 09 01  call        qword ptr [0000000141E0B678h]
  0000000140D7A161: BA 04 00 00 00     mov         edx,4
  0000000140D7A166: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D7A16B: E8 60 4A 47 00     call        00000001411EEBD0
  0000000140D7A170: 8B D0              mov         edx,eax
  0000000140D7A172: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A17A: FF 15 C0 F0 08 01  call        qword ptr [0000000141E09240h]
  0000000140D7A180: 48 8D 84 24 B8 01  lea         rax,[rsp+00000000000001B8h]
                    00 00
  0000000140D7A188: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  0000000140D7A190: 48 8B 94 24 C0 01  mov         rdx,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140D7A198: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D7A1A0: E8 6B 71 F1 FF     call        0000000140C91310
  0000000140D7A1A5: 48 89 84 24 C0 02  mov         qword ptr [rsp+00000000000002C0h],rax
                    00 00
  0000000140D7A1AD: C7 44 24 20 04 00  mov         dword ptr [rsp+20h],4
                    00 00
  0000000140D7A1B5: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D7A1BD: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  0000000140D7A1C1: 4C 8B 84 24 C0 02  mov         r8,qword ptr [rsp+00000000000002C0h]
                    00 00
  0000000140D7A1C9: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A1D1: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D7A1D9: E8 92 0B 00 00     call        0000000140D7AD70
  0000000140D7A1DE: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140D7A1E6: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D7A1EB: E8 F0 35 9B 00     call        000000014172D7E0
  0000000140D7A1F0: 0F B6 44 24 69     movzx       eax,byte ptr [rsp+69h]
  0000000140D7A1F5: 85 C0              test        eax,eax
  0000000140D7A1F7: 0F 84 44 01 00 00  je          0000000140D7A341
  0000000140D7A1FD: B9 10 00 00 00     mov         ecx,10h
  0000000140D7A202: E8 B5 48 6F 00     call        000000014146EABC
  0000000140D7A207: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
  0000000140D7A20F: 48 83 BC 24 D0 01  cmp         qword ptr [rsp+00000000000001D0h],0
                    00 00 00
  0000000140D7A218: 74 32              je          0000000140D7A24C
  0000000140D7A21A: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D7A222: FF 15 48 14 09 01  call        qword ptr [0000000141E0B670h]
  0000000140D7A228: 4C 8B 9C 24 D0 01  mov         r11,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D7A230: 48 8D 05 E1 48 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D7A237: 49 89 03           mov         qword ptr [r11],rax
  0000000140D7A23A: 48 8B 84 24 D0 01  mov         rax,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D7A242: 48 89 84 24 C8 02  mov         qword ptr [rsp+00000000000002C8h],rax
                    00 00
  0000000140D7A24A: EB 0C              jmp         0000000140D7A258
  0000000140D7A24C: 48 C7 84 24 C8 02  mov         qword ptr [rsp+00000000000002C8h],0
                    00 00 00 00 00 00
  0000000140D7A258: 48 8B 84 24 C8 02  mov         rax,qword ptr [rsp+00000000000002C8h]
                    00 00
  0000000140D7A260: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  0000000140D7A268: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000140D7A270: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D7A278: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D7A280: E8 DB 32 48 00     call        00000001411FD560
  0000000140D7A285: 0F B6 C0           movzx       eax,al
  0000000140D7A288: 85 C0              test        eax,eax
  0000000140D7A28A: 0F 84 8F 00 00 00  je          0000000140D7A31F
  0000000140D7A290: B2 01              mov         dl,1
  0000000140D7A292: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A29A: FF 15 D8 13 09 01  call        qword ptr [0000000141E0B678h]
  0000000140D7A2A0: BA 05 00 00 00     mov         edx,5
  0000000140D7A2A5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D7A2AA: E8 21 49 47 00     call        00000001411EEBD0
  0000000140D7A2AF: 8B D0              mov         edx,eax
  0000000140D7A2B1: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A2B9: FF 15 81 EF 08 01  call        qword ptr [0000000141E09240h]
  0000000140D7A2BF: 48 8D 84 24 D8 01  lea         rax,[rsp+00000000000001D8h]
                    00 00
  0000000140D7A2C7: 48 89 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],rax
                    00 00
  0000000140D7A2CF: 48 8B 94 24 E0 01  mov         rdx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140D7A2D7: 48 8B 8C 24 68 03  mov         rcx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D7A2DF: E8 2C 70 F1 FF     call        0000000140C91310
  0000000140D7A2E4: 48 89 84 24 D0 02  mov         qword ptr [rsp+00000000000002D0h],rax
                    00 00
  0000000140D7A2EC: C7 44 24 20 05 00  mov         dword ptr [rsp+20h],5
                    00 00
  0000000140D7A2F4: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D7A2FC: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  0000000140D7A300: 4C 8B 84 24 D0 02  mov         r8,qword ptr [rsp+00000000000002D0h]
                    00 00
  0000000140D7A308: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A310: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D7A318: E8 53 0A 00 00     call        0000000140D7AD70
  0000000140D7A31D: EB 10              jmp         0000000140D7A32F
  0000000140D7A31F: 33 D2              xor         edx,edx
  0000000140D7A321: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A329: FF 15 49 13 09 01  call        qword ptr [0000000141E0B678h]
  0000000140D7A32F: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140D7A337: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D7A33C: E8 9F 34 9B 00     call        000000014172D7E0
  0000000140D7A341: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140D7A346: 48 8B 8C 24 70 03  mov         rcx,qword ptr [rsp+0000000000000370h]
                    00 00
  0000000140D7A34E: FF 15 FC 12 09 01  call        qword ptr [0000000141E0B650h]
  0000000140D7A354: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000140D7A359: 85 C0              test        eax,eax
  0000000140D7A35B: 0F 84 81 00 00 00  je          0000000140D7A3E2
  0000000140D7A361: 48 8D 0D 50 C3 FF  lea         rcx,[0000000143D766B8h]
                    02
  0000000140D7A368: E8 43 DD 54 FF     call        00000001402C80B0
  0000000140D7A36D: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140D7A375: 48 83 BC 24 B8 00  cmp         qword ptr [rsp+00000000000000B8h],0
                    00 00 00
  0000000140D7A37E: 74 62              je          0000000140D7A3E2
  0000000140D7A380: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D7A388: E8 83 36 F6 FF     call        0000000140CDDA10
  0000000140D7A38D: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140D7A395: 48 83 BC 24 C0 00  cmp         qword ptr [rsp+00000000000000C0h],0
                    00 00 00
  0000000140D7A39E: 74 11              je          0000000140D7A3B1
  0000000140D7A3A0: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D7A3A8: E8 A3 50 97 00     call        00000001416EF450
  0000000140D7A3AD: 88 44 24 68        mov         byte ptr [rsp+68h],al
  0000000140D7A3B1: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D7A3B9: E8 C2 35 F6 FF     call        0000000140CDD980
  0000000140D7A3BE: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140D7A3C6: 48 83 BC 24 C8 00  cmp         qword ptr [rsp+00000000000000C8h],0
                    00 00 00
  0000000140D7A3CF: 74 11              je          0000000140D7A3E2
  0000000140D7A3D1: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140D7A3D9: E8 E2 71 5A FF     call        00000001403215C0
  0000000140D7A3DE: 88 44 24 49        mov         byte ptr [rsp+49h],al
  0000000140D7A3E2: E9 D7 F4 FF FF     jmp         0000000140D798BE
  0000000140D7A3E7: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000140D7A3EC: 85 C0              test        eax,eax
  0000000140D7A3EE: 0F 84 51 03 00 00  je          0000000140D7A745
  0000000140D7A3F4: 0F B6 44 24 49     movzx       eax,byte ptr [rsp+49h]
  0000000140D7A3F9: 85 C0              test        eax,eax
  0000000140D7A3FB: 0F 84 9B 01 00 00  je          0000000140D7A59C
  0000000140D7A401: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D7A406: E8 85 37 49 FF     call        000000014020DB90
  0000000140D7A40B: B9 30 00 00 00     mov         ecx,30h
  0000000140D7A410: E8 A7 46 6F 00     call        000000014146EABC
  0000000140D7A415: 48 89 84 24 F0 01  mov         qword ptr [rsp+00000000000001F0h],rax
                    00 00
  0000000140D7A41D: 48 83 BC 24 F0 01  cmp         qword ptr [rsp+00000000000001F0h],0
                    00 00 00
  0000000140D7A426: 74 17              je          0000000140D7A43F
  0000000140D7A428: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140D7A430: E8 5B 43 FD FF     call        0000000140D4E790
  0000000140D7A435: 48 89 84 24 D8 02  mov         qword ptr [rsp+00000000000002D8h],rax
                    00 00
  0000000140D7A43D: EB 0C              jmp         0000000140D7A44B
  0000000140D7A43F: 48 C7 84 24 D8 02  mov         qword ptr [rsp+00000000000002D8h],0
                    00 00 00 00 00 00
  0000000140D7A44B: 48 8B 84 24 D8 02  mov         rax,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D7A453: 48 89 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],rax
                    00 00
  0000000140D7A45B: 48 8B 84 24 E8 01  mov         rax,qword ptr [rsp+00000000000001E8h]
                    00 00
  0000000140D7A463: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D7A46B: 48 8D 15 F6 4E CC  lea         rdx,[0000000142A3F368h]
                    01
  0000000140D7A472: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A47A: E8 11 46 FD FF     call        0000000140D4EA90
  0000000140D7A47F: 45 33 C0           xor         r8d,r8d
  0000000140D7A482: 48 8D 15 EF 4E CC  lea         rdx,[0000000142A3F378h]
                    01
  0000000140D7A489: 48 8D 8C 24 F8 01  lea         rcx,[rsp+00000000000001F8h]
                    00 00
  0000000140D7A491: E8 2A 21 00 00     call        0000000140D7C5C0
  0000000140D7A496: 48 89 84 24 E0 02  mov         qword ptr [rsp+00000000000002E0h],rax
                    00 00
  0000000140D7A49E: 48 8B 84 24 E0 02  mov         rax,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140D7A4A6: 48 89 84 24 E8 02  mov         qword ptr [rsp+00000000000002E8h],rax
                    00 00
  0000000140D7A4AE: 48 8B 94 24 E8 02  mov         rdx,qword ptr [rsp+00000000000002E8h]
                    00 00
  0000000140D7A4B6: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A4BE: E8 3D 46 FD FF     call        0000000140D4EB00
  0000000140D7A4C3: 90                 nop
  0000000140D7A4C4: 48 8D 8C 24 F8 01  lea         rcx,[rsp+00000000000001F8h]
                    00 00
  0000000140D7A4CC: FF 15 0E B5 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7A4D2: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  0000000140D7A4D8: 48 8D 15 A9 4E CC  lea         rdx,[0000000142A3F388h]
                    01
  0000000140D7A4DF: 48 8D 8C 24 08 02  lea         rcx,[rsp+0000000000000208h]
                    00 00
  0000000140D7A4E7: FF 15 63 9C 08 01  call        qword ptr [0000000141E04150h]
  0000000140D7A4ED: 48 89 84 24 F0 02  mov         qword ptr [rsp+00000000000002F0h],rax
                    00 00
  0000000140D7A4F5: 48 8B 84 24 F0 02  mov         rax,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140D7A4FD: 48 89 84 24 F8 02  mov         qword ptr [rsp+00000000000002F8h],rax
                    00 00
  0000000140D7A505: 48 8B 94 24 F8 02  mov         rdx,qword ptr [rsp+00000000000002F8h]
                    00 00
  0000000140D7A50D: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  0000000140D7A515: FF 15 75 10 09 01  call        qword ptr [0000000141E0B590h]
  0000000140D7A51B: 48 89 84 24 00 03  mov         qword ptr [rsp+0000000000000300h],rax
                    00 00
  0000000140D7A523: 48 8B 84 24 00 03  mov         rax,qword ptr [rsp+0000000000000300h]
                    00 00
  0000000140D7A52B: 48 89 84 24 08 03  mov         qword ptr [rsp+0000000000000308h],rax
                    00 00
  0000000140D7A533: 48 8B 94 24 08 03  mov         rdx,qword ptr [rsp+0000000000000308h]
                    00 00
  0000000140D7A53B: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A543: FF 15 67 EC 08 01  call        qword ptr [0000000141E091B0h]
  0000000140D7A549: 90                 nop
  0000000140D7A54A: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  0000000140D7A552: FF 15 48 11 09 01  call        qword ptr [0000000141E0B6A0h]
  0000000140D7A558: 90                 nop
  0000000140D7A559: 48 8D 8C 24 08 02  lea         rcx,[rsp+0000000000000208h]
                    00 00
  0000000140D7A561: FF 15 79 B4 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7A567: B2 01              mov         dl,1
  0000000140D7A569: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A571: FF 15 81 0F 09 01  call        qword ptr [0000000141E0B4F8h]
  0000000140D7A577: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140D7A57F: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D7A584: E8 57 32 9B 00     call        000000014172D7E0
  0000000140D7A589: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140D7A58E: 48 8B 8C 24 70 03  mov         rcx,qword ptr [rsp+0000000000000370h]
                    00 00
  0000000140D7A596: FF 15 B4 10 09 01  call        qword ptr [0000000141E0B650h]
  0000000140D7A59C: 0F B6 44 24 68     movzx       eax,byte ptr [rsp+68h]
  0000000140D7A5A1: 85 C0              test        eax,eax
  0000000140D7A5A3: 0F 84 9C 01 00 00  je          0000000140D7A745
  0000000140D7A5A9: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D7A5AE: E8 DD 35 49 FF     call        000000014020DB90
  0000000140D7A5B3: B9 30 00 00 00     mov         ecx,30h
  0000000140D7A5B8: E8 FF 44 6F 00     call        000000014146EABC
  0000000140D7A5BD: 48 89 84 24 18 02  mov         qword ptr [rsp+0000000000000218h],rax
                    00 00
  0000000140D7A5C5: 48 83 BC 24 18 02  cmp         qword ptr [rsp+0000000000000218h],0
                    00 00 00
  0000000140D7A5CE: 74 17              je          0000000140D7A5E7
  0000000140D7A5D0: 48 8B 8C 24 18 02  mov         rcx,qword ptr [rsp+0000000000000218h]
                    00 00
  0000000140D7A5D8: E8 B3 41 FD FF     call        0000000140D4E790
  0000000140D7A5DD: 48 89 84 24 10 03  mov         qword ptr [rsp+0000000000000310h],rax
                    00 00
  0000000140D7A5E5: EB 0C              jmp         0000000140D7A5F3
  0000000140D7A5E7: 48 C7 84 24 10 03  mov         qword ptr [rsp+0000000000000310h],0
                    00 00 00 00 00 00
  0000000140D7A5F3: 48 8B 84 24 10 03  mov         rax,qword ptr [rsp+0000000000000310h]
                    00 00
  0000000140D7A5FB: 48 89 84 24 10 02  mov         qword ptr [rsp+0000000000000210h],rax
                    00 00
  0000000140D7A603: 48 8B 84 24 10 02  mov         rax,qword ptr [rsp+0000000000000210h]
                    00 00
  0000000140D7A60B: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D7A613: 48 8D 15 AE 4D CC  lea         rdx,[0000000142A3F3C8h]
                    01
  0000000140D7A61A: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A622: E8 69 44 FD FF     call        0000000140D4EA90
  0000000140D7A627: 45 33 C0           xor         r8d,r8d
  0000000140D7A62A: 48 8D 15 AF 4D CC  lea         rdx,[0000000142A3F3E0h]
                    01
  0000000140D7A631: 48 8D 8C 24 20 02  lea         rcx,[rsp+0000000000000220h]
                    00 00
  0000000140D7A639: E8 82 1F 00 00     call        0000000140D7C5C0
  0000000140D7A63E: 48 89 84 24 18 03  mov         qword ptr [rsp+0000000000000318h],rax
                    00 00
  0000000140D7A646: 48 8B 84 24 18 03  mov         rax,qword ptr [rsp+0000000000000318h]
                    00 00
  0000000140D7A64E: 48 89 84 24 20 03  mov         qword ptr [rsp+0000000000000320h],rax
                    00 00
  0000000140D7A656: 48 8B 94 24 20 03  mov         rdx,qword ptr [rsp+0000000000000320h]
                    00 00
  0000000140D7A65E: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A666: E8 95 44 FD FF     call        0000000140D4EB00
  0000000140D7A66B: 90                 nop
  0000000140D7A66C: 48 8D 8C 24 20 02  lea         rcx,[rsp+0000000000000220h]
                    00 00
  0000000140D7A674: FF 15 66 B3 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7A67A: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  0000000140D7A680: 48 8D 15 71 4D CC  lea         rdx,[0000000142A3F3F8h]
                    01
  0000000140D7A687: 48 8D 8C 24 30 02  lea         rcx,[rsp+0000000000000230h]
                    00 00
  0000000140D7A68F: FF 15 BB 9A 08 01  call        qword ptr [0000000141E04150h]
  0000000140D7A695: 48 89 84 24 28 03  mov         qword ptr [rsp+0000000000000328h],rax
                    00 00
  0000000140D7A69D: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  0000000140D7A6A5: 48 89 84 24 30 03  mov         qword ptr [rsp+0000000000000330h],rax
                    00 00
  0000000140D7A6AD: 48 8B 94 24 30 03  mov         rdx,qword ptr [rsp+0000000000000330h]
                    00 00
  0000000140D7A6B5: 48 8D 8C 24 28 02  lea         rcx,[rsp+0000000000000228h]
                    00 00
  0000000140D7A6BD: FF 15 CD 0E 09 01  call        qword ptr [0000000141E0B590h]
  0000000140D7A6C3: 48 89 84 24 38 03  mov         qword ptr [rsp+0000000000000338h],rax
                    00 00
  0000000140D7A6CB: 48 8B 84 24 38 03  mov         rax,qword ptr [rsp+0000000000000338h]
                    00 00
  0000000140D7A6D3: 48 89 84 24 40 03  mov         qword ptr [rsp+0000000000000340h],rax
                    00 00
  0000000140D7A6DB: 48 8B 94 24 40 03  mov         rdx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140D7A6E3: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A6EB: FF 15 BF EA 08 01  call        qword ptr [0000000141E091B0h]
  0000000140D7A6F1: 90                 nop
  0000000140D7A6F2: 48 8D 8C 24 28 02  lea         rcx,[rsp+0000000000000228h]
                    00 00
  0000000140D7A6FA: FF 15 A0 0F 09 01  call        qword ptr [0000000141E0B6A0h]
  0000000140D7A700: 90                 nop
  0000000140D7A701: 48 8D 8C 24 30 02  lea         rcx,[rsp+0000000000000230h]
                    00 00
  0000000140D7A709: FF 15 D1 B2 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7A70F: B2 01              mov         dl,1
  0000000140D7A711: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D7A719: FF 15 D9 0D 09 01  call        qword ptr [0000000141E0B4F8h]
  0000000140D7A71F: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140D7A727: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D7A72C: E8 AF 30 9B 00     call        000000014172D7E0
  0000000140D7A731: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140D7A736: 48 8B 8C 24 70 03  mov         rcx,qword ptr [rsp+0000000000000370h]
                    00 00
  0000000140D7A73E: FF 15 0C 0F 09 01  call        qword ptr [0000000141E0B650h]
  0000000140D7A744: 90                 nop
  0000000140D7A745: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D7A74A: E8 51 DA 48 FF     call        00000001402081A0
  0000000140D7A74F: 48 81 C4 58 03 00  add         rsp,358h
                    00
  0000000140D7A756: C3                 ret

  Summary

     1E02000 .text
