Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014018BB30: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  000000014018BB35: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014018BB3A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014018BB3E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014018BB43: 53                 push        rbx
  000000014018BB44: 57                 push        rdi
  000000014018BB45: 48 81 EC F8 02 00  sub         rsp,2F8h
                    00
  000000014018BB4C: 48 C7 84 24 40 02  mov         qword ptr [rsp+0000000000000240h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014018BB58: 83 BC 24 18 03 00  cmp         dword ptr [rsp+0000000000000318h],0
                    00 00
  000000014018BB60: 0F 85 A4 14 00 00  jne         000000014018D00A
  000000014018BB66: E8 15 90 E7 FF     call        0000000140004B80
  000000014018BB6B: 4C 8B 9C 24 10 03  mov         r11,qword ptr [rsp+0000000000000310h]
                    00 00
  000000014018BB73: 4C 89 5C 24 30     mov         qword ptr [rsp+30h],r11
  000000014018BB78: 8B 84 24 20 03 00  mov         eax,dword ptr [rsp+0000000000000320h]
                    00
  000000014018BB7F: 89 84 24 48 02 00  mov         dword ptr [rsp+0000000000000248h],eax
                    00
  000000014018BB86: 83 BC 24 48 02 00  cmp         dword ptr [rsp+0000000000000248h],54h
                    00 54
  000000014018BB8E: 0F 87 76 14 00 00  ja          000000014018D00A
  000000014018BB94: 48 63 84 24 48 02  movsxd      rax,dword ptr [rsp+0000000000000248h]
                    00 00
  000000014018BB9C: 48 8D 0D 5D 44 E7  lea         rcx,[0000000140000000h]
                    FF
  000000014018BBA3: 8B 84 81 14 D0 18  mov         eax,dword ptr [rcx+rax*4+000000000018D014h]
                    00
  000000014018BBAA: 48 03 C1           add         rax,rcx
  000000014018BBAD: FF E0              jmp         rax
  000000014018BBAF: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BBB7: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014018BBBB: 8B 10              mov         edx,dword ptr [rax]
  000000014018BBBD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BBC2: E8 F9 16 00 00     call        000000014018D2C0
  000000014018BBC7: E9 3E 14 00 00     jmp         000000014018D00A
  000000014018BBCC: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BBD4: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014018BBD8: F2 0F 10 08        movsd       xmm1,mmword ptr [rax]
  000000014018BBDC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BBE1: E8 2A 17 00 00     call        000000014018D310
  000000014018BBE6: E9 1F 14 00 00     jmp         000000014018D00A
  000000014018BBEB: 48 8D 84 24 A0 00  lea         rax,[rsp+00000000000000A0h]
                    00 00
  000000014018BBF3: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  000000014018BBFB: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BC03: 48 8B 52 20        mov         rdx,qword ptr [rdx+20h]
  000000014018BC07: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014018BC0F: E8 1C 84 BD 00     call        0000000140D64030
  000000014018BC14: 4C 8B D8           mov         r11,rax
  000000014018BC17: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BC1F: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  000000014018BC23: 4C 8B 84 24 28 03  mov         r8,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BC2B: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  000000014018BC2F: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BC37: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018BC3B: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  000000014018BC40: 44 8B 08           mov         r9d,dword ptr [rax]
  000000014018BC43: 4D 8B 00           mov         r8,qword ptr [r8]
  000000014018BC46: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018BC49: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BC4E: E8 0D 17 00 00     call        000000014018D360
  000000014018BC53: E9 B2 13 00 00     jmp         000000014018D00A
  000000014018BC58: 48 8D 84 24 B0 00  lea         rax,[rsp+00000000000000B0h]
                    00 00
  000000014018BC60: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  000000014018BC68: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BC70: 48 8B 52 20        mov         rdx,qword ptr [rdx+20h]
  000000014018BC74: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014018BC7C: E8 AF 83 BD 00     call        0000000140D64030
  000000014018BC81: 4C 8B D8           mov         r11,rax
  000000014018BC84: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BC8C: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  000000014018BC90: 4C 8B 84 24 28 03  mov         r8,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BC98: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  000000014018BC9C: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BCA4: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018BCA8: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  000000014018BCAD: 44 8B 08           mov         r9d,dword ptr [rax]
  000000014018BCB0: 4D 8B 00           mov         r8,qword ptr [r8]
  000000014018BCB3: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018BCB6: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BCBB: E8 20 17 00 00     call        000000014018D3E0
  000000014018BCC0: E9 45 13 00 00     jmp         000000014018D00A
  000000014018BCC5: 48 8D 84 24 C0 00  lea         rax,[rsp+00000000000000C0h]
                    00 00
  000000014018BCCD: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  000000014018BCD5: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BCDD: 48 8B 52 18        mov         rdx,qword ptr [rdx+18h]
  000000014018BCE1: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014018BCE9: E8 42 83 BD 00     call        0000000140D64030
  000000014018BCEE: 4C 8B 84 24 28 03  mov         r8,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BCF6: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  000000014018BCFA: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BD02: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018BD06: 4C 8B C8           mov         r9,rax
  000000014018BD09: 4D 8B 00           mov         r8,qword ptr [r8]
  000000014018BD0C: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018BD0F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BD14: E8 47 17 00 00     call        000000014018D460
  000000014018BD19: E9 EC 12 00 00     jmp         000000014018D00A
  000000014018BD1E: 48 8D 84 24 D0 00  lea         rax,[rsp+00000000000000D0h]
                    00 00
  000000014018BD26: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  000000014018BD2E: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BD36: 48 8B 52 20        mov         rdx,qword ptr [rdx+20h]
  000000014018BD3A: 48 8B 8C 24 D8 00  mov         rcx,qword ptr [rsp+00000000000000D8h]
                    00 00
  000000014018BD42: E8 E9 82 BD 00     call        0000000140D64030
  000000014018BD47: 4C 8B D8           mov         r11,rax
  000000014018BD4A: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BD52: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  000000014018BD56: 4C 8B 84 24 28 03  mov         r8,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BD5E: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  000000014018BD62: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BD6A: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018BD6E: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  000000014018BD73: 44 8B 08           mov         r9d,dword ptr [rax]
  000000014018BD76: 4D 8B 00           mov         r8,qword ptr [r8]
  000000014018BD79: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018BD7C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BD81: E8 4A 17 00 00     call        000000014018D4D0
  000000014018BD86: E9 7F 12 00 00     jmp         000000014018D00A
  000000014018BD8B: 48 8D 84 24 E0 00  lea         rax,[rsp+00000000000000E0h]
                    00 00
  000000014018BD93: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  000000014018BD9B: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BDA3: 48 8B 52 20        mov         rdx,qword ptr [rdx+20h]
  000000014018BDA7: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000014018BDAF: E8 7C 82 BD 00     call        0000000140D64030
  000000014018BDB4: 4C 8B D8           mov         r11,rax
  000000014018BDB7: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BDBF: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  000000014018BDC3: 4C 8B 84 24 28 03  mov         r8,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BDCB: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  000000014018BDCF: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BDD7: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018BDDB: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  000000014018BDE0: 44 8B 08           mov         r9d,dword ptr [rax]
  000000014018BDE3: 4D 8B 00           mov         r8,qword ptr [r8]
  000000014018BDE6: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018BDE9: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BDEE: E8 5D 17 00 00     call        000000014018D550
  000000014018BDF3: E9 12 12 00 00     jmp         000000014018D00A
  000000014018BDF8: 48 8D 84 24 F0 00  lea         rax,[rsp+00000000000000F0h]
                    00 00
  000000014018BE00: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  000000014018BE08: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BE10: 48 8B 52 18        mov         rdx,qword ptr [rdx+18h]
  000000014018BE14: 48 8B 8C 24 F8 00  mov         rcx,qword ptr [rsp+00000000000000F8h]
                    00 00
  000000014018BE1C: E8 0F 82 BD 00     call        0000000140D64030
  000000014018BE21: 4C 8B 84 24 28 03  mov         r8,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BE29: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  000000014018BE2D: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BE35: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018BE39: 4C 8B C8           mov         r9,rax
  000000014018BE3C: 4D 8B 00           mov         r8,qword ptr [r8]
  000000014018BE3F: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018BE42: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BE47: E8 84 17 00 00     call        000000014018D5D0
  000000014018BE4C: E9 B9 11 00 00     jmp         000000014018D00A
  000000014018BE51: 48 8D 84 24 00 01  lea         rax,[rsp+0000000000000100h]
                    00 00
  000000014018BE59: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  000000014018BE61: 48 8D 84 24 10 01  lea         rax,[rsp+0000000000000110h]
                    00 00
  000000014018BE69: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  000000014018BE71: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BE79: 48 8B 52 20        mov         rdx,qword ptr [rdx+20h]
  000000014018BE7D: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  000000014018BE85: E8 A6 81 BD 00     call        0000000140D64030
  000000014018BE8A: 48 8B D8           mov         rbx,rax
  000000014018BE8D: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BE95: 48 8B 52 18        mov         rdx,qword ptr [rdx+18h]
  000000014018BE99: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014018BEA1: FF 15 31 9B C7 01  call        qword ptr [0000000141E059D8h]
  000000014018BEA7: 48 89 84 24 50 02  mov         qword ptr [rsp+0000000000000250h],rax
                    00 00
  000000014018BEAF: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BEB7: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  000000014018BEBB: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BEC3: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018BEC7: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000014018BECC: 4C 8B 8C 24 50 02  mov         r9,qword ptr [rsp+0000000000000250h]
                    00 00
  000000014018BED4: 44 8B 00           mov         r8d,dword ptr [rax]
  000000014018BED7: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018BEDA: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BEDF: E8 5C 17 00 00     call        000000014018D640
  000000014018BEE4: E9 21 11 00 00     jmp         000000014018D00A
  000000014018BEE9: 48 8D 84 24 20 01  lea         rax,[rsp+0000000000000120h]
                    00 00
  000000014018BEF1: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  000000014018BEF9: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  000000014018BF01: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  000000014018BF09: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BF11: 48 8B 52 20        mov         rdx,qword ptr [rdx+20h]
  000000014018BF15: 48 8B 8C 24 28 01  mov         rcx,qword ptr [rsp+0000000000000128h]
                    00 00
  000000014018BF1D: E8 0E 81 BD 00     call        0000000140D64030
  000000014018BF22: 48 8B D8           mov         rbx,rax
  000000014018BF25: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BF2D: 48 8B 52 18        mov         rdx,qword ptr [rdx+18h]
  000000014018BF31: 48 8B 8C 24 38 01  mov         rcx,qword ptr [rsp+0000000000000138h]
                    00 00
  000000014018BF39: FF 15 99 9A C7 01  call        qword ptr [0000000141E059D8h]
  000000014018BF3F: 48 89 84 24 58 02  mov         qword ptr [rsp+0000000000000258h],rax
                    00 00
  000000014018BF47: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BF4F: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  000000014018BF53: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BF5B: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018BF5F: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000014018BF64: 4C 8B 8C 24 58 02  mov         r9,qword ptr [rsp+0000000000000258h]
                    00 00
  000000014018BF6C: 44 8B 00           mov         r8d,dword ptr [rax]
  000000014018BF6F: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018BF72: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BF77: E8 54 17 00 00     call        000000014018D6D0
  000000014018BF7C: E9 89 10 00 00     jmp         000000014018D00A
  000000014018BF81: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BF89: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  000000014018BF8D: F2 0F 10 10        movsd       xmm2,mmword ptr [rax]
  000000014018BF91: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BF99: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014018BF9D: F2 0F 10 08        movsd       xmm1,mmword ptr [rax]
  000000014018BFA1: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BFA6: E8 B5 17 00 00     call        000000014018D760
  000000014018BFAB: E9 5A 10 00 00     jmp         000000014018D00A
  000000014018BFB0: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BFB8: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014018BFBC: F2 0F 10 08        movsd       xmm1,mmword ptr [rax]
  000000014018BFC0: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BFC5: E8 F6 17 00 00     call        000000014018D7C0
  000000014018BFCA: E9 3B 10 00 00     jmp         000000014018D00A
  000000014018BFCF: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BFD7: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018BFDB: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018BFDE: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018BFE3: E8 28 18 00 00     call        000000014018D810
  000000014018BFE8: E9 1D 10 00 00     jmp         000000014018D00A
  000000014018BFED: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018BFF5: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018BFF9: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018BFFC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C001: E8 5A 18 00 00     call        000000014018D860
  000000014018C006: E9 FF 0F 00 00     jmp         000000014018D00A
  000000014018C00B: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  000000014018C013: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  000000014018C01B: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C023: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C027: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  000000014018C02F: FF 15 A3 99 C7 01  call        qword ptr [0000000141E059D8h]
  000000014018C035: 48 89 84 24 60 02  mov         qword ptr [rsp+0000000000000260h],rax
                    00 00
  000000014018C03D: 48 8B 94 24 60 02  mov         rdx,qword ptr [rsp+0000000000000260h]
                    00 00
  000000014018C045: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C04A: E8 61 18 00 00     call        000000014018D8B0
  000000014018C04F: E9 B6 0F 00 00     jmp         000000014018D00A
  000000014018C054: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C05C: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C060: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C063: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C068: E8 A3 18 00 00     call        000000014018D910
  000000014018C06D: E9 98 0F 00 00     jmp         000000014018D00A
  000000014018C072: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C07A: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C07E: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C081: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C086: E8 D5 18 00 00     call        000000014018D960
  000000014018C08B: E9 7A 0F 00 00     jmp         000000014018D00A
  000000014018C090: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C095: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C098: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C09D: FF 90 48 02 00 00  call        qword ptr [rax+0000000000000248h]
  000000014018C0A3: E9 62 0F 00 00     jmp         000000014018D00A
  000000014018C0A8: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C0AD: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C0B0: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C0B5: FF 90 50 02 00 00  call        qword ptr [rax+0000000000000250h]
  000000014018C0BB: E9 4A 0F 00 00     jmp         000000014018D00A
  000000014018C0C0: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C0C5: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C0C8: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C0CD: FF 90 58 02 00 00  call        qword ptr [rax+0000000000000258h]
  000000014018C0D3: E9 32 0F 00 00     jmp         000000014018D00A
  000000014018C0D8: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C0DD: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C0E0: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C0E5: FF 90 60 02 00 00  call        qword ptr [rax+0000000000000260h]
  000000014018C0EB: E9 1A 0F 00 00     jmp         000000014018D00A
  000000014018C0F0: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  000000014018C0F8: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  000000014018C100: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C108: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C10C: 48 8B 8C 24 58 01  mov         rcx,qword ptr [rsp+0000000000000158h]
                    00 00
  000000014018C114: FF 15 BE 98 C7 01  call        qword ptr [0000000141E059D8h]
  000000014018C11A: 48 89 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],rax
                    00 00
  000000014018C122: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C127: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C12A: 48 8B 94 24 68 02  mov         rdx,qword ptr [rsp+0000000000000268h]
                    00 00
  000000014018C132: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C137: FF 90 68 02 00 00  call        qword ptr [rax+0000000000000268h]
  000000014018C13D: E9 C8 0E 00 00     jmp         000000014018D00A
  000000014018C142: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  000000014018C14A: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  000000014018C152: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C15A: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C15E: 48 8B 8C 24 68 01  mov         rcx,qword ptr [rsp+0000000000000168h]
                    00 00
  000000014018C166: FF 15 6C 98 C7 01  call        qword ptr [0000000141E059D8h]
  000000014018C16C: 48 89 84 24 70 02  mov         qword ptr [rsp+0000000000000270h],rax
                    00 00
  000000014018C174: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C179: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C17C: 48 8B 94 24 70 02  mov         rdx,qword ptr [rsp+0000000000000270h]
                    00 00
  000000014018C184: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C189: FF 90 70 02 00 00  call        qword ptr [rax+0000000000000270h]
  000000014018C18F: E9 76 0E 00 00     jmp         000000014018D00A
  000000014018C194: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C199: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C19C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C1A1: FF 90 78 02 00 00  call        qword ptr [rax+0000000000000278h]
  000000014018C1A7: E9 5E 0E 00 00     jmp         000000014018D00A
  000000014018C1AC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C1B1: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C1B4: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C1B9: FF 90 48 03 00 00  call        qword ptr [rax+0000000000000348h]
  000000014018C1BF: E9 46 0E 00 00     jmp         000000014018D00A
  000000014018C1C4: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C1C9: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C1CC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C1D1: FF 90 50 03 00 00  call        qword ptr [rax+0000000000000350h]
  000000014018C1D7: E9 2E 0E 00 00     jmp         000000014018D00A
  000000014018C1DC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C1E1: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C1E4: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C1E9: FF 90 58 03 00 00  call        qword ptr [rax+0000000000000358h]
  000000014018C1EF: E9 16 0E 00 00     jmp         000000014018D00A
  000000014018C1F4: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C1FC: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014018C200: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C205: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C208: 0F B6 11           movzx       edx,byte ptr [rcx]
  000000014018C20B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C210: FF 90 60 03 00 00  call        qword ptr [rax+0000000000000360h]
  000000014018C216: E9 EF 0D 00 00     jmp         000000014018D00A
  000000014018C21B: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C220: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C223: 33 D2              xor         edx,edx
  000000014018C225: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C22A: FF 90 60 03 00 00  call        qword ptr [rax+0000000000000360h]
  000000014018C230: E9 D5 0D 00 00     jmp         000000014018D00A
  000000014018C235: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C23A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C23D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C242: FF 90 68 03 00 00  call        qword ptr [rax+0000000000000368h]
  000000014018C248: E9 BD 0D 00 00     jmp         000000014018D00A
  000000014018C24D: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C252: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C255: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C25A: FF 90 70 03 00 00  call        qword ptr [rax+0000000000000370h]
  000000014018C260: E9 A5 0D 00 00     jmp         000000014018D00A
  000000014018C265: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C26A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C26D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C272: FF 90 78 03 00 00  call        qword ptr [rax+0000000000000378h]
  000000014018C278: E9 8D 0D 00 00     jmp         000000014018D00A
  000000014018C27D: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C282: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C285: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C28A: FF 90 80 03 00 00  call        qword ptr [rax+0000000000000380h]
  000000014018C290: E9 75 0D 00 00     jmp         000000014018D00A
  000000014018C295: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C29A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C29D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C2A2: FF 90 88 03 00 00  call        qword ptr [rax+0000000000000388h]
  000000014018C2A8: E9 5D 0D 00 00     jmp         000000014018D00A
  000000014018C2AD: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C2B2: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C2B5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C2BA: FF 90 90 03 00 00  call        qword ptr [rax+0000000000000390h]
  000000014018C2C0: E9 45 0D 00 00     jmp         000000014018D00A
  000000014018C2C5: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  000000014018C2CD: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  000000014018C2D5: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C2DD: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C2E1: 48 8B 8C 24 78 01  mov         rcx,qword ptr [rsp+0000000000000178h]
                    00 00
  000000014018C2E9: E8 E2 D7 0C 00     call        0000000140259AD0
  000000014018C2EE: 48 89 84 24 78 02  mov         qword ptr [rsp+0000000000000278h],rax
                    00 00
  000000014018C2F6: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C2FB: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C2FE: 48 8B 94 24 78 02  mov         rdx,qword ptr [rsp+0000000000000278h]
                    00 00
  000000014018C306: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C30B: FF 90 98 03 00 00  call        qword ptr [rax+0000000000000398h]
  000000014018C311: E9 F4 0C 00 00     jmp         000000014018D00A
  000000014018C316: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C31B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C31E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C323: FF 90 A0 03 00 00  call        qword ptr [rax+00000000000003A0h]
  000000014018C329: E9 DC 0C 00 00     jmp         000000014018D00A
  000000014018C32E: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C336: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014018C33A: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C33F: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C342: 8B 11              mov         edx,dword ptr [rcx]
  000000014018C344: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C349: FF 90 A8 03 00 00  call        qword ptr [rax+00000000000003A8h]
  000000014018C34F: E9 B6 0C 00 00     jmp         000000014018D00A
  000000014018C354: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C359: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C35C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C361: FF 90 B0 03 00 00  call        qword ptr [rax+00000000000003B0h]
  000000014018C367: E9 9E 0C 00 00     jmp         000000014018D00A
  000000014018C36C: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C374: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014018C378: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C37D: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C380: 0F B6 11           movzx       edx,byte ptr [rcx]
  000000014018C383: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C388: FF 90 B8 03 00 00  call        qword ptr [rax+00000000000003B8h]
  000000014018C38E: E9 77 0C 00 00     jmp         000000014018D00A
  000000014018C393: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C398: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C39B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C3A0: FF 90 C0 03 00 00  call        qword ptr [rax+00000000000003C0h]
  000000014018C3A6: E9 5F 0C 00 00     jmp         000000014018D00A
  000000014018C3AB: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C3B3: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C3B7: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C3BC: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C3BF: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C3C2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C3C7: FF 90 C8 03 00 00  call        qword ptr [rax+00000000000003C8h]
  000000014018C3CD: E9 38 0C 00 00     jmp         000000014018D00A
  000000014018C3D2: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C3D7: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C3DA: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C3DF: FF 90 D0 03 00 00  call        qword ptr [rax+00000000000003D0h]
  000000014018C3E5: E9 20 0C 00 00     jmp         000000014018D00A
  000000014018C3EA: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C3EF: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C3F2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C3F7: FF 90 D8 03 00 00  call        qword ptr [rax+00000000000003D8h]
  000000014018C3FD: E9 08 0C 00 00     jmp         000000014018D00A
  000000014018C402: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C40A: 48 8B 40 20        mov         rax,qword ptr [rax+20h]
  000000014018C40E: 4C 8B 8C 24 28 03  mov         r9,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C416: 4D 8B 49 18        mov         r9,qword ptr [r9+18h]
  000000014018C41A: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C422: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  000000014018C426: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C42E: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C432: 4C 8B 54 24 30     mov         r10,qword ptr [rsp+30h]
  000000014018C437: 4D 8B 12           mov         r10,qword ptr [r10]
  000000014018C43A: 8B 00              mov         eax,dword ptr [rax]
  000000014018C43C: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014018C440: 4D 8B 09           mov         r9,qword ptr [r9]
  000000014018C443: 44 8B 01           mov         r8d,dword ptr [rcx]
  000000014018C446: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C449: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C44E: 41 FF 92 E0 03 00  call        qword ptr [r10+00000000000003E0h]
                    00
  000000014018C455: E9 B0 0B 00 00     jmp         000000014018D00A
  000000014018C45A: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C462: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C466: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C46B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C46E: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C471: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C476: FF 90 E8 03 00 00  call        qword ptr [rax+00000000000003E8h]
  000000014018C47C: E9 89 0B 00 00     jmp         000000014018D00A
  000000014018C481: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C489: 48 8B 40 20        mov         rax,qword ptr [rax+20h]
  000000014018C48D: 4C 8B 8C 24 28 03  mov         r9,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C495: 4D 8B 49 18        mov         r9,qword ptr [r9+18h]
  000000014018C499: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C4A1: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  000000014018C4A5: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C4AD: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C4B1: 4C 8B 54 24 30     mov         r10,qword ptr [rsp+30h]
  000000014018C4B6: 4D 8B 12           mov         r10,qword ptr [r10]
  000000014018C4B9: 8B 00              mov         eax,dword ptr [rax]
  000000014018C4BB: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014018C4BF: 4D 8B 09           mov         r9,qword ptr [r9]
  000000014018C4C2: 44 8B 01           mov         r8d,dword ptr [rcx]
  000000014018C4C5: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C4C8: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C4CD: 41 FF 92 F0 03 00  call        qword ptr [r10+00000000000003F0h]
                    00
  000000014018C4D4: E9 31 0B 00 00     jmp         000000014018D00A
  000000014018C4D9: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C4E1: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C4E5: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C4EA: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C4ED: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C4F0: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C4F5: FF 90 F8 03 00 00  call        qword ptr [rax+00000000000003F8h]
  000000014018C4FB: E9 0A 0B 00 00     jmp         000000014018D00A
  000000014018C500: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C508: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  000000014018C50C: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C514: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C518: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C51D: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C520: 44 8B 01           mov         r8d,dword ptr [rcx]
  000000014018C523: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C526: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C52B: FF 90 00 04 00 00  call        qword ptr [rax+0000000000000400h]
  000000014018C531: E9 D4 0A 00 00     jmp         000000014018D00A
  000000014018C536: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C53B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C53E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C543: FF 90 08 04 00 00  call        qword ptr [rax+0000000000000408h]
  000000014018C549: 88 44 24 38        mov         byte ptr [rsp+38h],al
  000000014018C54D: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C555: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018C559: 74 12              je          000000014018C56D
  000000014018C55B: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C563: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018C566: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  000000014018C56B: 88 01              mov         byte ptr [rcx],al
  000000014018C56D: E9 98 0A 00 00     jmp         000000014018D00A
  000000014018C572: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C577: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C57A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C57F: FF 90 10 04 00 00  call        qword ptr [rax+0000000000000410h]
  000000014018C585: 88 44 24 39        mov         byte ptr [rsp+39h],al
  000000014018C589: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C591: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018C595: 74 12              je          000000014018C5A9
  000000014018C597: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C59F: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018C5A2: 0F B6 44 24 39     movzx       eax,byte ptr [rsp+39h]
  000000014018C5A7: 88 01              mov         byte ptr [rcx],al
  000000014018C5A9: E9 5C 0A 00 00     jmp         000000014018D00A
  000000014018C5AE: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C5B6: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014018C5BA: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C5BF: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C5C2: 0F B6 11           movzx       edx,byte ptr [rcx]
  000000014018C5C5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C5CA: FF 90 18 04 00 00  call        qword ptr [rax+0000000000000418h]
  000000014018C5D0: E9 35 0A 00 00     jmp         000000014018D00A
  000000014018C5D5: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C5DD: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C5E1: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C5E6: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C5E9: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C5EC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C5F1: FF 90 20 04 00 00  call        qword ptr [rax+0000000000000420h]
  000000014018C5F7: E9 0E 0A 00 00     jmp         000000014018D00A
  000000014018C5FC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C601: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C604: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C609: FF 90 60 05 00 00  call        qword ptr [rax+0000000000000560h]
  000000014018C60F: E9 F6 09 00 00     jmp         000000014018D00A
  000000014018C614: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C61C: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014018C620: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C625: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C628: 0F B6 11           movzx       edx,byte ptr [rcx]
  000000014018C62B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C630: FF 90 68 05 00 00  call        qword ptr [rax+0000000000000568h]
  000000014018C636: E9 CF 09 00 00     jmp         000000014018D00A
  000000014018C63B: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C643: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014018C647: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C64C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C64F: 0F B6 11           movzx       edx,byte ptr [rcx]
  000000014018C652: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C657: FF 90 70 05 00 00  call        qword ptr [rax+0000000000000570h]
  000000014018C65D: E9 A8 09 00 00     jmp         000000014018D00A
  000000014018C662: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C66A: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C66E: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C673: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C676: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C679: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C67E: FF 90 78 05 00 00  call        qword ptr [rax+0000000000000578h]
  000000014018C684: E9 81 09 00 00     jmp         000000014018D00A
  000000014018C689: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C691: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C695: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C69A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C69D: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C6A0: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C6A5: FF 90 80 05 00 00  call        qword ptr [rax+0000000000000580h]
  000000014018C6AB: E9 5A 09 00 00     jmp         000000014018D00A
  000000014018C6B0: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C6B8: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C6BC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C6C1: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C6C4: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C6C7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C6CC: FF 90 88 05 00 00  call        qword ptr [rax+0000000000000588h]
  000000014018C6D2: E9 33 09 00 00     jmp         000000014018D00A
  000000014018C6D7: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C6DF: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014018C6E3: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C6E8: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C6EB: 8B 11              mov         edx,dword ptr [rcx]
  000000014018C6ED: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C6F2: FF 90 90 05 00 00  call        qword ptr [rax+0000000000000590h]
  000000014018C6F8: E9 0D 09 00 00     jmp         000000014018D00A
  000000014018C6FD: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C702: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C705: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C70A: FF 90 98 05 00 00  call        qword ptr [rax+0000000000000598h]
  000000014018C710: E9 F5 08 00 00     jmp         000000014018D00A
  000000014018C715: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C71A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C71D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C722: FF 90 A0 05 00 00  call        qword ptr [rax+00000000000005A0h]
  000000014018C728: E9 DD 08 00 00     jmp         000000014018D00A
  000000014018C72D: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  000000014018C735: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  000000014018C73D: 48 8B 9C 24 28 03  mov         rbx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C745: 48 8B 5B 18        mov         rbx,qword ptr [rbx+18h]
  000000014018C749: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C751: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  000000014018C755: 48 8B 8C 24 88 01  mov         rcx,qword ptr [rsp+0000000000000188h]
                    00 00
  000000014018C75D: FF 15 75 92 C7 01  call        qword ptr [0000000141E059D8h]
  000000014018C763: 48 89 84 24 80 02  mov         qword ptr [rsp+0000000000000280h],rax
                    00 00
  000000014018C76B: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C773: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014018C777: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C77C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C77F: 44 8B 0B           mov         r9d,dword ptr [rbx]
  000000014018C782: 4C 8B 84 24 80 02  mov         r8,qword ptr [rsp+0000000000000280h]
                    00 00
  000000014018C78A: 8B 11              mov         edx,dword ptr [rcx]
  000000014018C78C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C791: FF 90 A8 05 00 00  call        qword ptr [rax+00000000000005A8h]
  000000014018C797: E9 6E 08 00 00     jmp         000000014018D00A
  000000014018C79C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014018C7A1: 48 8B 00           mov         rax,qword ptr [rax]
  000000014018C7A4: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C7A9: FF 90 B0 05 00 00  call        qword ptr [rax+00000000000005B0h]
  000000014018C7AF: E9 56 08 00 00     jmp         000000014018D00A
  000000014018C7B4: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C7B9: E8 52 43 FF FF     call        0000000140180B10
  000000014018C7BE: E9 47 08 00 00     jmp         000000014018D00A
  000000014018C7C3: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  000000014018C7CB: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  000000014018C7D3: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C7DB: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C7DF: 48 8B 8C 24 98 01  mov         rcx,qword ptr [rsp+0000000000000198h]
                    00 00
  000000014018C7E7: FF 15 EB 91 C7 01  call        qword ptr [0000000141E059D8h]
  000000014018C7ED: 48 89 84 24 88 02  mov         qword ptr [rsp+0000000000000288h],rax
                    00 00
  000000014018C7F5: 48 8B 94 24 88 02  mov         rdx,qword ptr [rsp+0000000000000288h]
                    00 00
  000000014018C7FD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C802: E8 79 33 FF FF     call        000000014017FB80
  000000014018C807: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014018C80C: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C814: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018C818: 74 13              je          000000014018C82D
  000000014018C81A: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C822: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018C825: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014018C82A: 48 89 01           mov         qword ptr [rcx],rax
  000000014018C82D: E9 D8 07 00 00     jmp         000000014018D00A
  000000014018C832: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C83A: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C83E: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018C841: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C846: E8 F5 34 FF FF     call        000000014017FD40
  000000014018C84B: 88 44 24 48        mov         byte ptr [rsp+48h],al
  000000014018C84F: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C857: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018C85B: 74 12              je          000000014018C86F
  000000014018C85D: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C865: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018C868: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  000000014018C86D: 88 01              mov         byte ptr [rcx],al
  000000014018C86F: E9 96 07 00 00     jmp         000000014018D00A
  000000014018C874: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C879: E8 22 35 FF FF     call        000000014017FDA0
  000000014018C87E: 88 44 24 49        mov         byte ptr [rsp+49h],al
  000000014018C882: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C88A: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018C88E: 74 12              je          000000014018C8A2
  000000014018C890: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C898: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018C89B: 0F B6 44 24 49     movzx       eax,byte ptr [rsp+49h]
  000000014018C8A0: 88 01              mov         byte ptr [rcx],al
  000000014018C8A2: E9 63 07 00 00     jmp         000000014018D00A
  000000014018C8A7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C8AC: E8 4F 35 FF FF     call        000000014017FE00
  000000014018C8B1: 89 44 24 4C        mov         dword ptr [rsp+4Ch],eax
  000000014018C8B5: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C8BD: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018C8C1: 74 11              je          000000014018C8D4
  000000014018C8C3: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C8CB: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018C8CE: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  000000014018C8D2: 89 01              mov         dword ptr [rcx],eax
  000000014018C8D4: E9 31 07 00 00     jmp         000000014018D00A
  000000014018C8D9: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C8E1: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014018C8E5: 8B 10              mov         edx,dword ptr [rax]
  000000014018C8E7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C8EC: E8 3F 35 FF FF     call        000000014017FE30
  000000014018C8F1: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014018C8F6: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C8FE: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018C902: 74 13              je          000000014018C917
  000000014018C904: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C90C: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018C90F: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014018C914: 48 89 01           mov         qword ptr [rcx],rax
  000000014018C917: E9 EE 06 00 00     jmp         000000014018D00A
  000000014018C91C: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  000000014018C924: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  000000014018C92C: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C934: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C938: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  000000014018C940: FF 15 92 90 C7 01  call        qword ptr [0000000141E059D8h]
  000000014018C946: 48 89 84 24 90 02  mov         qword ptr [rsp+0000000000000290h],rax
                    00 00
  000000014018C94E: 4C 8B 84 24 90 02  mov         r8,qword ptr [rsp+0000000000000290h]
                    00 00
  000000014018C956: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000014018C95B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018C960: E8 1B 35 FF FF     call        000000014017FE80
  000000014018C965: 90                 nop
  000000014018C966: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C96E: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018C972: 74 16              je          000000014018C98A
  000000014018C974: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000014018C979: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C981: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018C984: E8 E7 06 F2 FF     call        00000001400AD070
  000000014018C989: 90                 nop
  000000014018C98A: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000014018C98F: E8 0C B8 07 00     call        00000001402081A0
  000000014018C994: E9 71 06 00 00     jmp         000000014018D00A
  000000014018C999: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  000000014018C9A1: 48 89 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],rax
                    00 00
  000000014018C9A9: 48 8B BC 24 28 03  mov         rdi,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C9B1: 48 8B 7F 18        mov         rdi,qword ptr [rdi+18h]
  000000014018C9B5: 48 8B 9C 24 28 03  mov         rbx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C9BD: 48 8B 5B 10        mov         rbx,qword ptr [rbx+10h]
  000000014018C9C1: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018C9C9: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018C9CD: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  000000014018C9D5: FF 15 FD 8F C7 01  call        qword ptr [0000000141E059D8h]
  000000014018C9DB: 48 89 84 24 98 02  mov         qword ptr [rsp+0000000000000298h],rax
                    00 00
  000000014018C9E3: 0F B6 07           movzx       eax,byte ptr [rdi]
  000000014018C9E6: 88 44 24 20        mov         byte ptr [rsp+20h],al
  000000014018C9EA: 44 0F B6 0B        movzx       r9d,byte ptr [rbx]
  000000014018C9EE: 4C 8B 84 24 98 02  mov         r8,qword ptr [rsp+0000000000000298h]
                    00 00
  000000014018C9F6: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  000000014018C9FB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CA00: E8 2B 36 FF FF     call        0000000140180030
  000000014018CA05: 90                 nop
  000000014018CA06: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CA0E: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CA12: 74 16              je          000000014018CA2A
  000000014018CA14: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  000000014018CA19: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CA21: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CA24: E8 47 06 F2 FF     call        00000001400AD070
  000000014018CA29: 90                 nop
  000000014018CA2A: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014018CA2F: E8 6C B7 07 00     call        00000001402081A0
  000000014018CA34: E9 D1 05 00 00     jmp         000000014018D00A
  000000014018CA39: 48 8D 84 24 C0 01  lea         rax,[rsp+00000000000001C0h]
                    00 00
  000000014018CA41: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  000000014018CA49: 48 8B 9C 24 28 03  mov         rbx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CA51: 48 8B 5B 10        mov         rbx,qword ptr [rbx+10h]
  000000014018CA55: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CA5D: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CA61: 48 8B 8C 24 C8 01  mov         rcx,qword ptr [rsp+00000000000001C8h]
                    00 00
  000000014018CA69: FF 15 69 8F C7 01  call        qword ptr [0000000141E059D8h]
  000000014018CA6F: 48 89 84 24 A0 02  mov         qword ptr [rsp+00000000000002A0h],rax
                    00 00
  000000014018CA77: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014018CA7C: 44 0F B6 0B        movzx       r9d,byte ptr [rbx]
  000000014018CA80: 4C 8B 84 24 A0 02  mov         r8,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014018CA88: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000014018CA8D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CA92: E8 99 35 FF FF     call        0000000140180030
  000000014018CA97: 90                 nop
  000000014018CA98: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CAA0: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CAA4: 74 16              je          000000014018CABC
  000000014018CAA6: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000014018CAAB: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CAB3: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CAB6: E8 B5 05 F2 FF     call        00000001400AD070
  000000014018CABB: 90                 nop
  000000014018CABC: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000014018CAC1: E8 DA B6 07 00     call        00000001402081A0
  000000014018CAC6: E9 3F 05 00 00     jmp         000000014018D00A
  000000014018CACB: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  000000014018CAD3: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  000000014018CADB: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CAE3: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CAE7: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  000000014018CAEF: FF 15 E3 8E C7 01  call        qword ptr [0000000141E059D8h]
  000000014018CAF5: 48 89 84 24 A8 02  mov         qword ptr [rsp+00000000000002A8h],rax
                    00 00
  000000014018CAFD: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014018CB02: 41 B1 01           mov         r9b,1
  000000014018CB05: 4C 8B 84 24 A8 02  mov         r8,qword ptr [rsp+00000000000002A8h]
                    00 00
  000000014018CB0D: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  000000014018CB12: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CB17: E8 14 35 FF FF     call        0000000140180030
  000000014018CB1C: 90                 nop
  000000014018CB1D: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CB25: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CB29: 74 16              je          000000014018CB41
  000000014018CB2B: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  000000014018CB30: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CB38: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CB3B: E8 30 05 F2 FF     call        00000001400AD070
  000000014018CB40: 90                 nop
  000000014018CB41: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000014018CB46: E8 55 B6 07 00     call        00000001402081A0
  000000014018CB4B: E9 BA 04 00 00     jmp         000000014018D00A
  000000014018CB50: 4C 8B 84 24 28 03  mov         r8,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CB58: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  000000014018CB5C: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CB64: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CB68: 4D 8B 00           mov         r8,qword ptr [r8]
  000000014018CB6B: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018CB6E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CB73: E8 98 37 FF FF     call        0000000140180310
  000000014018CB78: 88 44 24 78        mov         byte ptr [rsp+78h],al
  000000014018CB7C: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CB84: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CB88: 74 12              je          000000014018CB9C
  000000014018CB8A: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CB92: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CB95: 0F B6 44 24 78     movzx       eax,byte ptr [rsp+78h]
  000000014018CB9A: 88 01              mov         byte ptr [rcx],al
  000000014018CB9C: E9 69 04 00 00     jmp         000000014018D00A
  000000014018CBA1: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CBA9: 48 8B 40 20        mov         rax,qword ptr [rax+20h]
  000000014018CBAD: 4C 8B 8C 24 28 03  mov         r9,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CBB5: 4D 8B 49 18        mov         r9,qword ptr [r9+18h]
  000000014018CBB9: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CBC1: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  000000014018CBC5: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CBCD: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CBD1: 8B 00              mov         eax,dword ptr [rax]
  000000014018CBD3: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014018CBD7: 4D 8B 09           mov         r9,qword ptr [r9]
  000000014018CBDA: 44 8B 01           mov         r8d,dword ptr [rcx]
  000000014018CBDD: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018CBE0: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CBE5: E8 46 37 FF FF     call        0000000140180330
  000000014018CBEA: 88 44 24 79        mov         byte ptr [rsp+79h],al
  000000014018CBEE: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CBF6: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CBFA: 74 12              je          000000014018CC0E
  000000014018CBFC: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CC04: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CC07: 0F B6 44 24 79     movzx       eax,byte ptr [rsp+79h]
  000000014018CC0C: 88 01              mov         byte ptr [rcx],al
  000000014018CC0E: E9 F7 03 00 00     jmp         000000014018D00A
  000000014018CC13: 4C 8B 84 24 28 03  mov         r8,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CC1B: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  000000014018CC1F: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CC27: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CC2B: 4D 8B 00           mov         r8,qword ptr [r8]
  000000014018CC2E: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018CC31: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CC36: E8 45 37 FF FF     call        0000000140180380
  000000014018CC3B: 88 44 24 7A        mov         byte ptr [rsp+7Ah],al
  000000014018CC3F: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CC47: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CC4B: 74 12              je          000000014018CC5F
  000000014018CC4D: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CC55: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CC58: 0F B6 44 24 7A     movzx       eax,byte ptr [rsp+7Ah]
  000000014018CC5D: 88 01              mov         byte ptr [rcx],al
  000000014018CC5F: E9 A6 03 00 00     jmp         000000014018D00A
  000000014018CC64: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CC6C: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CC70: 48 8B 12           mov         rdx,qword ptr [rdx]
  000000014018CC73: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CC78: E8 63 37 FF FF     call        00000001401803E0
  000000014018CC7D: 88 44 24 7B        mov         byte ptr [rsp+7Bh],al
  000000014018CC81: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CC89: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CC8D: 74 12              je          000000014018CCA1
  000000014018CC8F: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CC97: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CC9A: 0F B6 44 24 7B     movzx       eax,byte ptr [rsp+7Bh]
  000000014018CC9F: 88 01              mov         byte ptr [rcx],al
  000000014018CCA1: E9 64 03 00 00     jmp         000000014018D00A
  000000014018CCA6: 48 8D 84 24 E0 01  lea         rax,[rsp+00000000000001E0h]
                    00 00
  000000014018CCAE: 48 89 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],rax
                    00 00
  000000014018CCB6: 48 8D 84 24 F0 01  lea         rax,[rsp+00000000000001F0h]
                    00 00
  000000014018CCBE: 48 89 84 24 F8 01  mov         qword ptr [rsp+00000000000001F8h],rax
                    00 00
  000000014018CCC6: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CCCE: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  000000014018CCD2: 48 8B 8C 24 E8 01  mov         rcx,qword ptr [rsp+00000000000001E8h]
                    00 00
  000000014018CCDA: FF 15 F8 8C C7 01  call        qword ptr [0000000141E059D8h]
  000000014018CCE0: 48 89 84 24 B0 02  mov         qword ptr [rsp+00000000000002B0h],rax
                    00 00
  000000014018CCE8: 48 8B 84 24 B0 02  mov         rax,qword ptr [rsp+00000000000002B0h]
                    00 00
  000000014018CCF0: 48 89 84 24 B8 02  mov         qword ptr [rsp+00000000000002B8h],rax
                    00 00
  000000014018CCF8: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CD00: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CD04: 48 8B 8C 24 F8 01  mov         rcx,qword ptr [rsp+00000000000001F8h]
                    00 00
  000000014018CD0C: FF 15 C6 8C C7 01  call        qword ptr [0000000141E059D8h]
  000000014018CD12: 48 89 84 24 C0 02  mov         qword ptr [rsp+00000000000002C0h],rax
                    00 00
  000000014018CD1A: 4C 8B 84 24 B8 02  mov         r8,qword ptr [rsp+00000000000002B8h]
                    00 00
  000000014018CD22: 48 8B 94 24 C0 02  mov         rdx,qword ptr [rsp+00000000000002C0h]
                    00 00
  000000014018CD2A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CD2F: E8 FC 36 FF FF     call        0000000140180430
  000000014018CD34: 88 44 24 7C        mov         byte ptr [rsp+7Ch],al
  000000014018CD38: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CD40: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CD44: 74 12              je          000000014018CD58
  000000014018CD46: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CD4E: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CD51: 0F B6 44 24 7C     movzx       eax,byte ptr [rsp+7Ch]
  000000014018CD56: 88 01              mov         byte ptr [rcx],al
  000000014018CD58: E9 AD 02 00 00     jmp         000000014018D00A
  000000014018CD5D: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  000000014018CD65: 48 89 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],rax
                    00 00
  000000014018CD6D: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CD75: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CD79: 48 8B 8C 24 08 02  mov         rcx,qword ptr [rsp+0000000000000208h]
                    00 00
  000000014018CD81: FF 15 51 8C C7 01  call        qword ptr [0000000141E059D8h]
  000000014018CD87: 48 89 84 24 C8 02  mov         qword ptr [rsp+00000000000002C8h],rax
                    00 00
  000000014018CD8F: 48 8B 94 24 C8 02  mov         rdx,qword ptr [rsp+00000000000002C8h]
                    00 00
  000000014018CD97: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CD9C: E8 8F 37 FF FF     call        0000000140180530
  000000014018CDA1: 88 44 24 7D        mov         byte ptr [rsp+7Dh],al
  000000014018CDA5: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CDAD: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CDB1: 74 12              je          000000014018CDC5
  000000014018CDB3: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CDBB: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CDBE: 0F B6 44 24 7D     movzx       eax,byte ptr [rsp+7Dh]
  000000014018CDC3: 88 01              mov         byte ptr [rcx],al
  000000014018CDC5: E9 40 02 00 00     jmp         000000014018D00A
  000000014018CDCA: 48 8D 84 24 10 02  lea         rax,[rsp+0000000000000210h]
                    00 00
  000000014018CDD2: 48 89 84 24 18 02  mov         qword ptr [rsp+0000000000000218h],rax
                    00 00
  000000014018CDDA: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CDE2: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CDE6: 48 8B 8C 24 18 02  mov         rcx,qword ptr [rsp+0000000000000218h]
                    00 00
  000000014018CDEE: FF 15 E4 8B C7 01  call        qword ptr [0000000141E059D8h]
  000000014018CDF4: 48 89 84 24 D0 02  mov         qword ptr [rsp+00000000000002D0h],rax
                    00 00
  000000014018CDFC: 48 8B 94 24 D0 02  mov         rdx,qword ptr [rsp+00000000000002D0h]
                    00 00
  000000014018CE04: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CE09: E8 D2 37 FF FF     call        00000001401805E0
  000000014018CE0E: 88 44 24 7E        mov         byte ptr [rsp+7Eh],al
  000000014018CE12: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CE1A: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CE1E: 74 12              je          000000014018CE32
  000000014018CE20: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CE28: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CE2B: 0F B6 44 24 7E     movzx       eax,byte ptr [rsp+7Eh]
  000000014018CE30: 88 01              mov         byte ptr [rcx],al
  000000014018CE32: E9 D3 01 00 00     jmp         000000014018D00A
  000000014018CE37: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CE3F: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014018CE43: 44 8B 00           mov         r8d,dword ptr [rax]
  000000014018CE46: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000014018CE4E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CE53: E8 18 38 FF FF     call        0000000140180670
  000000014018CE58: 90                 nop
  000000014018CE59: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CE61: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CE65: 74 1A              je          000000014018CE81
  000000014018CE67: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000014018CE6F: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CE77: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CE7A: FF 15 F0 75 C7 01  call        qword ptr [0000000141E04470h]
  000000014018CE80: 90                 nop
  000000014018CE81: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014018CE89: FF 15 51 8B C7 01  call        qword ptr [0000000141E059E0h]
  000000014018CE8F: E9 76 01 00 00     jmp         000000014018D00A
  000000014018CE94: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  000000014018CE9C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CEA1: E8 7A 38 FF FF     call        0000000140180720
  000000014018CEA6: 90                 nop
  000000014018CEA7: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CEAF: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CEB3: 74 19              je          000000014018CECE
  000000014018CEB5: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  000000014018CEBD: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CEC5: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CEC8: E8 73 BB EF FF     call        0000000140088A40
  000000014018CECD: 90                 nop
  000000014018CECE: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000014018CED6: E8 F5 79 E7 FF     call        00000001400048D0
  000000014018CEDB: E9 2A 01 00 00     jmp         000000014018D00A
  000000014018CEE0: 48 8D 84 24 20 02  lea         rax,[rsp+0000000000000220h]
                    00 00
  000000014018CEE8: 48 89 84 24 28 02  mov         qword ptr [rsp+0000000000000228h],rax
                    00 00
  000000014018CEF0: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CEF8: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CEFC: 48 8B 8C 24 28 02  mov         rcx,qword ptr [rsp+0000000000000228h]
                    00 00
  000000014018CF04: FF 15 CE 8A C7 01  call        qword ptr [0000000141E059D8h]
  000000014018CF0A: 48 89 84 24 D8 02  mov         qword ptr [rsp+00000000000002D8h],rax
                    00 00
  000000014018CF12: 4C 8B 84 24 D8 02  mov         r8,qword ptr [rsp+00000000000002D8h]
                    00 00
  000000014018CF1A: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  000000014018CF22: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CF27: E8 E4 38 FF FF     call        0000000140180810
  000000014018CF2C: 90                 nop
  000000014018CF2D: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CF35: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CF39: 74 19              je          000000014018CF54
  000000014018CF3B: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  000000014018CF43: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CF4B: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CF4E: E8 ED BA EF FF     call        0000000140088A40
  000000014018CF53: 90                 nop
  000000014018CF54: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000014018CF5C: E8 6F 79 E7 FF     call        00000001400048D0
  000000014018CF61: E9 A4 00 00 00     jmp         000000014018D00A
  000000014018CF66: 48 8D 84 24 30 02  lea         rax,[rsp+0000000000000230h]
                    00 00
  000000014018CF6E: 48 89 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],rax
                    00 00
  000000014018CF76: 48 8B BC 24 28 03  mov         rdi,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CF7E: 48 8B 7F 18        mov         rdi,qword ptr [rdi+18h]
  000000014018CF82: 48 8B 9C 24 28 03  mov         rbx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CF8A: 48 8B 5B 10        mov         rbx,qword ptr [rbx+10h]
  000000014018CF8E: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CF96: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014018CF9A: 48 8B 8C 24 38 02  mov         rcx,qword ptr [rsp+0000000000000238h]
                    00 00
  000000014018CFA2: FF 15 30 8A C7 01  call        qword ptr [0000000141E059D8h]
  000000014018CFA8: 48 89 84 24 E0 02  mov         qword ptr [rsp+00000000000002E0h],rax
                    00 00
  000000014018CFB0: 0F B6 07           movzx       eax,byte ptr [rdi]
  000000014018CFB3: 88 44 24 20        mov         byte ptr [rsp+20h],al
  000000014018CFB7: 44 0F B6 0B        movzx       r9d,byte ptr [rbx]
  000000014018CFBB: 4C 8B 84 24 E0 02  mov         r8,qword ptr [rsp+00000000000002E0h]
                    00 00
  000000014018CFC3: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  000000014018CFCB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014018CFD0: E8 CB 39 FF FF     call        00000001401809A0
  000000014018CFD5: 90                 nop
  000000014018CFD6: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CFDE: 48 83 38 00        cmp         qword ptr [rax],0
  000000014018CFE2: 74 19              je          000000014018CFFD
  000000014018CFE4: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  000000014018CFEC: 48 8B 8C 24 28 03  mov         rcx,qword ptr [rsp+0000000000000328h]
                    00 00
  000000014018CFF4: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014018CFF7: E8 44 BA EF FF     call        0000000140088A40
  000000014018CFFC: 90                 nop
  000000014018CFFD: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014018D005: E8 C6 78 E7 FF     call        00000001400048D0
  000000014018D00A: 48 81 C4 F8 02 00  add         rsp,2F8h
                    00
  000000014018D011: 5F                 pop         rdi
  000000014018D012: 5B                 pop         rbx
  000000014018D013: C3                 ret
  000000014018D014: AF                 scas        dword ptr [rdi]
  000000014018D015: BB 18 00 CC BB     mov         ebx,0BBCC0018h
  000000014018D01A: 18 00              sbb         byte ptr [rax],al
  000000014018D01C: EB BB              jmp         000000014018CFD9
  000000014018D01E: 18 00              sbb         byte ptr [rax],al
  000000014018D020: 58                 pop         rax
  000000014018D021: BC 18 00 C5 BC     mov         esp,0BCC50018h
  000000014018D026: 18 00              sbb         byte ptr [rax],al
  000000014018D028: 1E
  000000014018D029: BD 18 00 8B BD     mov         ebp,0BD8B0018h
  000000014018D02E: 18 00              sbb         byte ptr [rax],al
  000000014018D030: F8                 clc
  000000014018D031: BD 18 00 51 BE     mov         ebp,0BE510018h
  000000014018D036: 18 00              sbb         byte ptr [rax],al
  000000014018D038: E9 BE 18 00 81     jmp         00000000C118E8FB
  000000014018D03D: BF 18 00 B0 BF     mov         edi,0BFB00018h
  000000014018D042: 18 00              sbb         byte ptr [rax],al
  000000014018D044: CF                 iretd
  000000014018D045: BF 18 00 ED BF     mov         edi,0BFED0018h
  000000014018D04A: 18 00              sbb         byte ptr [rax],al
  000000014018D04C: 0B C0              or          eax,eax
  000000014018D04E: 18 00              sbb         byte ptr [rax],al
  000000014018D050: 54                 push        rsp
  000000014018D051: C0 18 00           rcr         byte ptr [rax],0
  000000014018D054: 72 C0              jb          000000014018D016
  000000014018D056: 18 00              sbb         byte ptr [rax],al
  000000014018D058: 90                 nop
  000000014018D059: C0 18 00           rcr         byte ptr [rax],0
  000000014018D05C: A8 C0              test        al,0C0h
  000000014018D05E: 18 00              sbb         byte ptr [rax],al
  000000014018D060: C0 C0 18           rol         al,18h
  000000014018D063: 00 D8              add         al,bl
  000000014018D065: C0 18 00           rcr         byte ptr [rax],0
  000000014018D068: F0 C0 18 00        lock rcr    byte ptr [rax],0
  000000014018D06C: 42 C1 18 00        rcr         dword ptr [rax],0
  000000014018D070: 94                 xchg        eax,esp
  000000014018D071: C1 18 00           rcr         dword ptr [rax],0
  000000014018D074: AC                 lods        byte ptr [rsi]
  000000014018D075: C1 18 00           rcr         dword ptr [rax],0
  000000014018D078: C4
  000000014018D079: C1 18 00           rcr         dword ptr [rax],0
  000000014018D07C: DC C1              fadd        st(1),st
  000000014018D07E: 18 00              sbb         byte ptr [rax],al
  000000014018D080: F4                 hlt
  000000014018D081: C1 18 00           rcr         dword ptr [rax],0
  000000014018D084: 1B C2              sbb         eax,edx
  000000014018D086: 18 00              sbb         byte ptr [rax],al
  000000014018D088: 35 C2 18 00 4D     xor         eax,4D0018C2h
  000000014018D08D: C2 18 00           ret         18h
  000000014018D090: 65 C2 18 00        ret         18h
  000000014018D094: 7D C2              jge         000000014018D058
  000000014018D096: 18 00              sbb         byte ptr [rax],al
  000000014018D098: 95                 xchg        eax,ebp
  000000014018D099: C2 18 00           ret         18h
  000000014018D09C: AD                 lods        dword ptr [rsi]
  000000014018D09D: C2 18 00           ret         18h
  000000014018D0A0: C5
  000000014018D0A1: C2 18 00           ret         18h
  000000014018D0A4: 16
  000000014018D0A5: C3                 ret
  000000014018D0A6: 18 00              sbb         byte ptr [rax],al
  000000014018D0A8: 2E C3              ret
  000000014018D0AA: 18 00              sbb         byte ptr [rax],al
  000000014018D0AC: 54                 push        rsp
  000000014018D0AD: C3                 ret
  000000014018D0AE: 18 00              sbb         byte ptr [rax],al
  000000014018D0B0: 6C                 ins         byte ptr [rdi],dx
  000000014018D0B1: C3                 ret
  000000014018D0B2: 18 00              sbb         byte ptr [rax],al
  000000014018D0B4: 93                 xchg        eax,ebx
  000000014018D0B5: C3                 ret
  000000014018D0B6: 18 00              sbb         byte ptr [rax],al
  000000014018D0B8: AB                 stos        dword ptr [rdi]
  000000014018D0B9: C3                 ret
  000000014018D0BA: 18 00              sbb         byte ptr [rax],al
  000000014018D0BC: D2 C3              rol         bl,cl
  000000014018D0BE: 18 00              sbb         byte ptr [rax],al
  000000014018D0C0: EA
  000000014018D0C1: C3                 ret
  000000014018D0C2: 18 00              sbb         byte ptr [rax],al
  000000014018D0C4: 02 C4              add         al,ah
  000000014018D0C6: 18 00              sbb         byte ptr [rax],al
  000000014018D0C8: 5A                 pop         rdx
  000000014018D0C9: C4
  000000014018D0CA: 18 00              sbb         byte ptr [rax],al
  000000014018D0CC: 81 C4 18 00 D9 C4  add         esp,0C4D90018h
  000000014018D0D2: 18 00              sbb         byte ptr [rax],al
  000000014018D0D4: 00 C5              add         ch,al
  000000014018D0D6: 18 00              sbb         byte ptr [rax],al
  000000014018D0D8: 36
  000000014018D0D9: C5
  000000014018D0DA: 18 00              sbb         byte ptr [rax],al
  000000014018D0DC: 72 C5              jb          000000014018D0A3
  000000014018D0DE: 18 00              sbb         byte ptr [rax],al
  000000014018D0E0: AE                 scas        byte ptr [rdi]
  000000014018D0E1: C5
  000000014018D0E2: 18 00              sbb         byte ptr [rax],al
  000000014018D0E4: D5
  000000014018D0E5: C5
  000000014018D0E6: 18 00              sbb         byte ptr [rax],al
  000000014018D0E8: FC                 cld
  000000014018D0E9: C5
  000000014018D0EA: 18 00              sbb         byte ptr [rax],al
  000000014018D0EC: 14 C6              adc         al,0C6h
  000000014018D0EE: 18 00              sbb         byte ptr [rax],al
  000000014018D0F0: 3B C6              cmp         eax,esi
  000000014018D0F2: 18 00              sbb         byte ptr [rax],al
  000000014018D0F4: 62
  000000014018D0F5: C6
  000000014018D0F6: 18 00              sbb         byte ptr [rax],al
  000000014018D0F8: 89 C6              mov         esi,eax
  000000014018D0FA: 18 00              sbb         byte ptr [rax],al
  000000014018D0FC: B0 C6              mov         al,0C6h
  000000014018D0FE: 18 00              sbb         byte ptr [rax],al
  000000014018D100: D7                 xlat        byte ptr [rbx]
  000000014018D101: C6
  000000014018D102: 18 00              sbb         byte ptr [rax],al
  000000014018D104: FD                 std
  000000014018D105: C6
  000000014018D106: 18 00              sbb         byte ptr [rax],al
  000000014018D108: 15 C7 18 00 2D     adc         eax,2D0018C7h
  000000014018D10D: C7
  000000014018D10E: 18 00              sbb         byte ptr [rax],al
  000000014018D110: 9C                 pushfq
  000000014018D111: C7
  000000014018D112: 18 00              sbb         byte ptr [rax],al
  000000014018D114: B4 C7              mov         ah,0C7h
  000000014018D116: 18 00              sbb         byte ptr [rax],al
  000000014018D118: C3                 ret
  000000014018D119: C7
  000000014018D11A: 18 00              sbb         byte ptr [rax],al
  000000014018D11C: 32 C8              xor         cl,al
  000000014018D11E: 18 00              sbb         byte ptr [rax],al
  000000014018D120: 74 C8              je          000000014018D0EA
  000000014018D122: 18 00              sbb         byte ptr [rax],al
  000000014018D124: A7                 cmps        dword ptr [rsi],dword ptr [rdi]
  000000014018D125: C8 18 00 D9        enter       18h,0D9h
  000000014018D129: C8 18 00 1C        enter       18h,1Ch
  000000014018D12D: C9                 leave
  000000014018D12E: 18 00              sbb         byte ptr [rax],al
  000000014018D130: 99                 cdq
  000000014018D131: C9                 leave
  000000014018D132: 18 00              sbb         byte ptr [rax],al
  000000014018D134: 39 CA              cmp         edx,ecx
  000000014018D136: 18 00              sbb         byte ptr [rax],al
  000000014018D138: CB                 retf
  000000014018D139: CA 18 00           retf        18h
  000000014018D13C: 50                 push        rax
  000000014018D13D: CB                 retf
  000000014018D13E: 18 00              sbb         byte ptr [rax],al
  000000014018D140: A1 CB 18 00 13 CC  mov         eax,dword ptr [640018CC130018CBh]
                    18 00 64
  000000014018D149: CC                 int         3
  000000014018D14A: 18 00              sbb         byte ptr [rax],al
  000000014018D14C: A6                 cmps        byte ptr [rsi],byte ptr [rdi]
  000000014018D14D: CC                 int         3
  000000014018D14E: 18 00              sbb         byte ptr [rax],al
  000000014018D150: 5D                 pop         rbp
  000000014018D151: CD 18              int         18h
  000000014018D153: 00 CA              add         dl,cl
  000000014018D155: CD 18              int         18h
  000000014018D157: 00 37              add         byte ptr [rdi],dh
  000000014018D159: CE
  000000014018D15A: 18 00              sbb         byte ptr [rax],al
  000000014018D15C: 94                 xchg        eax,esp
  000000014018D15D: CE
  000000014018D15E: 18 00              sbb         byte ptr [rax],al
  000000014018D160: E0 CE              loopne      000000014018D130
  000000014018D162: 18 00              sbb         byte ptr [rax],al
  000000014018D164: 66 CF              iret
  000000014018D166: 18 00              sbb         byte ptr [rax],al

  Summary

     1E02000 .text
