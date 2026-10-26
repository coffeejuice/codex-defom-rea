Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400538F0: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  00000001400538F5: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  00000001400538F9: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400538FE: 56                 push        rsi
  00000001400538FF: 57                 push        rdi
  0000000140053900: 48 81 EC D8 02 00  sub         rsp,2D8h
                    00
  0000000140053907: 48 C7 84 24 80 02  mov         qword ptr [rsp+0000000000000280h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140053913: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  000000014005391B: E8 50 BE 00 00     call        000000014005F770
  0000000140053920: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140053925: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  000000014005392B: 75 05              jne         0000000140053932
  000000014005392D: E9 0D 09 00 00     jmp         000000014005423F
  0000000140053932: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  000000014005393A: E8 D1 17 00 00     call        0000000140055110
  000000014005393F: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140053947: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  000000014005394F: E8 4C BE 00 00     call        000000014005F7A0
  0000000140053954: 8B 80 20 13 00 00  mov         eax,dword ptr [rax+0000000000001320h]
  000000014005395A: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  0000000140053961: C7 44 24 74 00 00  mov         dword ptr [rsp+74h],0
                    00 00
  0000000140053969: C7 44 24 74 00 00  mov         dword ptr [rsp+74h],0
                    00 00
  0000000140053971: EB 0B              jmp         000000014005397E
  0000000140053973: 8B 44 24 74        mov         eax,dword ptr [rsp+74h]
  0000000140053977: 83 C0 01           add         eax,1
  000000014005397A: 89 44 24 74        mov         dword ptr [rsp+74h],eax
  000000014005397E: 8B 84 24 80 00 00  mov         eax,dword ptr [rsp+0000000000000080h]
                    00
  0000000140053985: 39 44 24 74        cmp         dword ptr [rsp+74h],eax
  0000000140053989: 0F 8D 82 01 00 00  jge         0000000140053B11
  000000014005398F: 8B 7C 24 74        mov         edi,dword ptr [rsp+74h]
  0000000140053993: 83 C7 01           add         edi,1
  0000000140053996: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  000000014005399E: E8 FD BD 00 00     call        000000014005F7A0
  00000001400539A3: 8B D7              mov         edx,edi
  00000001400539A5: 48 8B C8           mov         rcx,rax
  00000001400539A8: E8 B3 CA 7D 00     call        0000000140830460
  00000001400539AD: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000001400539B5: 48 83 BC 24 98 00  cmp         qword ptr [rsp+0000000000000098h],0
                    00 00 00
  00000001400539BE: 75 02              jne         00000001400539C2
  00000001400539C0: EB B1              jmp         0000000140053973
  00000001400539C2: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001400539CA: 8B 00              mov         eax,dword ptr [rax]
  00000001400539CC: 89 84 24 A0 00 00  mov         dword ptr [rsp+00000000000000A0h],eax
                    00
  00000001400539D3: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001400539DB: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400539DE: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000001400539E6: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001400539EE: FF 90 C8 01 00 00  call        qword ptr [rax+00000000000001C8h]
  00000001400539F4: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001400539FC: 48 83 BC 24 B0 00  cmp         qword ptr [rsp+00000000000000B0h],0
                    00 00 00
  0000000140053A05: 75 20              jne         0000000140053A27
  0000000140053A07: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140053A0F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140053A12: 8B 94 24 A0 00 00  mov         edx,dword ptr [rsp+00000000000000A0h]
                    00
  0000000140053A19: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140053A21: FF 90 88 01 00 00  call        qword ptr [rax+0000000000000188h]
  0000000140053A27: 8B 94 24 A0 00 00  mov         edx,dword ptr [rsp+00000000000000A0h]
                    00
  0000000140053A2E: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053A36: E8 05 D2 00 00     call        0000000140060C40
  0000000140053A3B: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140053A43: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140053A46: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  0000000140053A4E: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  0000000140053A56: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140053A5E: FF 90 08 02 00 00  call        qword ptr [rax+0000000000000208h]
  0000000140053A64: 90                 nop
  0000000140053A65: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140053A6D: 83 B8 C8 1C 00 00  cmp         dword ptr [rax+0000000000001CC8h],2
                    02
  0000000140053A74: 0F 84 84 00 00 00  je          0000000140053AFE
  0000000140053A7A: 48 8D 94 24 08 01  lea         rdx,[rsp+0000000000000108h]
                    00 00
  0000000140053A82: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140053A8A: FF 15 C0 04 DB 01  call        qword ptr [0000000141E03F50h]
  0000000140053A90: 48 89 84 24 88 02  mov         qword ptr [rsp+0000000000000288h],rax
                    00 00
  0000000140053A98: 48 8B 84 24 88 02  mov         rax,qword ptr [rsp+0000000000000288h]
                    00 00
  0000000140053AA0: 48 89 84 24 90 02  mov         qword ptr [rsp+0000000000000290h],rax
                    00 00
  0000000140053AA8: 48 8B 8C 24 90 02  mov         rcx,qword ptr [rsp+0000000000000290h]
                    00 00
  0000000140053AB0: FF 15 72 07 DB 01  call        qword ptr [0000000141E04228h]
  0000000140053AB6: 48 8B F8           mov         rdi,rax
  0000000140053AB9: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053AC1: E8 DA BC 00 00     call        000000014005F7A0
  0000000140053AC6: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140053ACF: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140053AD8: 45 33 C9           xor         r9d,r9d
  0000000140053ADB: 44 8B 84 24 A0 00  mov         r8d,dword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140053AE3: 48 8B D7           mov         rdx,rdi
  0000000140053AE6: 48 8B C8           mov         rcx,rax
  0000000140053AE9: E8 F2 1E 84 00     call        00000001408959E0
  0000000140053AEE: 90                 nop
  0000000140053AEF: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  0000000140053AF7: FF 15 93 07 DB 01  call        qword ptr [0000000141E04290h]
  0000000140053AFD: 90                 nop
  0000000140053AFE: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140053B06: FF 15 D4 1E DB 01  call        qword ptr [0000000141E059E0h]
  0000000140053B0C: E9 62 FE FF FF     jmp         0000000140053973
  0000000140053B11: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053B19: E8 62 17 00 00     call        0000000140055280
  0000000140053B1E: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053B26: E8 75 BC 00 00     call        000000014005F7A0
  0000000140053B2B: 48 8B C8           mov         rcx,rax
  0000000140053B2E: 48 81 C1 28 13 00  add         rcx,1328h
                    00
  0000000140053B35: E8 66 89 86 00     call        00000001408BC4A0
  0000000140053B3A: 89 44 24 70        mov         dword ptr [rsp+70h],eax
  0000000140053B3E: C7 44 24 7C 00 00  mov         dword ptr [rsp+7Ch],0
                    00 00
  0000000140053B46: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000140053B4F: 83 7C 24 70 00     cmp         dword ptr [rsp+70h],0
  0000000140053B54: 7F 08              jg          0000000140053B5E
  0000000140053B56: C7 44 24 70 64 00  mov         dword ptr [rsp+70h],64h
                    00 00
  0000000140053B5E: 48 63 4C 24 70     movsxd      rcx,dword ptr [rsp+70h]
  0000000140053B63: B8 04 00 00 00     mov         eax,4
  0000000140053B68: 48 F7 E1           mul         rax,rcx
  0000000140053B6B: 48 8B C8           mov         rcx,rax
  0000000140053B6E: 48 C7 C0 FF FF FF  mov         rax,0FFFFFFFFFFFFFFFFh
                    FF
  0000000140053B75: 48 0F 40 C8        cmovo       rcx,rax
  0000000140053B79: E8 DE 86 41 01     call        000000014146C25C
  0000000140053B7E: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  0000000140053B86: 48 8B 84 24 10 01  mov         rax,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140053B8E: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140053B93: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140053B98: E8 03 86 C2 00     call        0000000140C7C1A0
  0000000140053B9D: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  0000000140053BA1: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  0000000140053BA5: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053BAD: E8 2E D3 00 00     call        0000000140060EE0
  0000000140053BB2: C7 84 24 B8 00 00  mov         dword ptr [rsp+00000000000000B8h],0
                    00 00 00 00 00
  0000000140053BBD: EB 11              jmp         0000000140053BD0
  0000000140053BBF: 8B 84 24 B8 00 00  mov         eax,dword ptr [rsp+00000000000000B8h]
                    00
  0000000140053BC6: 83 C0 01           add         eax,1
  0000000140053BC9: 89 84 24 B8 00 00  mov         dword ptr [rsp+00000000000000B8h],eax
                    00
  0000000140053BD0: 8B 44 24 78        mov         eax,dword ptr [rsp+78h]
  0000000140053BD4: 39 84 24 B8 00 00  cmp         dword ptr [rsp+00000000000000B8h],eax
                    00
  0000000140053BDB: 0F 8D BE 03 00 00  jge         0000000140053F9F
  0000000140053BE1: 8B 94 24 B8 00 00  mov         edx,dword ptr [rsp+00000000000000B8h]
                    00
  0000000140053BE8: 83 C2 01           add         edx,1
  0000000140053BEB: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140053BF0: E8 DB 72 C2 00     call        0000000140C7AED0
  0000000140053BF5: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000140053BFD: 48 83 BC 24 D0 00  cmp         qword ptr [rsp+00000000000000D0h],0
                    00 00 00
  0000000140053C06: 74 02              je          0000000140053C0A
  0000000140053C08: EB 02              jmp         0000000140053C0C
  0000000140053C0A: EB B3              jmp         0000000140053BBF
  0000000140053C0C: 8B 94 24 B8 00 00  mov         edx,dword ptr [rsp+00000000000000B8h]
                    00
  0000000140053C13: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053C1B: E8 10 BF 00 00     call        000000014005FB30
  0000000140053C20: 89 84 24 C8 00 00  mov         dword ptr [rsp+00000000000000C8h],eax
                    00
  0000000140053C27: 83 BC 24 C8 00 00  cmp         dword ptr [rsp+00000000000000C8h],0FFFFFFFFh
                    00 FF
  0000000140053C2F: 0F 85 65 01 00 00  jne         0000000140053D9A
  0000000140053C35: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140053C3D: 83 78 0C 07        cmp         dword ptr [rax+0Ch],7
  0000000140053C41: 0F 84 53 01 00 00  je          0000000140053D9A
  0000000140053C47: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140053C4F: 83 78 0C 05        cmp         dword ptr [rax+0Ch],5
  0000000140053C53: 75 10              jne         0000000140053C65
  0000000140053C55: C7 84 24 C8 00 00  mov         dword ptr [rsp+00000000000000C8h],0FFFFFFFEh
                    00 FE FF FF FF
  0000000140053C60: E9 C9 00 00 00     jmp         0000000140053D2E
  0000000140053C65: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053C6D: E8 5E 14 00 00     call        00000001400550D0
  0000000140053C72: 48 89 84 24 98 02  mov         qword ptr [rsp+0000000000000298h],rax
                    00 00
  0000000140053C7A: 48 8B 84 24 98 02  mov         rax,qword ptr [rsp+0000000000000298h]
                    00 00
  0000000140053C82: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140053C85: 48 8B 8C 24 98 02  mov         rcx,qword ptr [rsp+0000000000000298h]
                    00 00
  0000000140053C8D: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  0000000140053C93: 89 84 24 C8 00 00  mov         dword ptr [rsp+00000000000000C8h],eax
                    00
  0000000140053C9A: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053CA2: E8 29 14 00 00     call        00000001400550D0
  0000000140053CA7: 48 89 84 24 A0 02  mov         qword ptr [rsp+00000000000002A0h],rax
                    00 00
  0000000140053CAF: 48 8B 84 24 A0 02  mov         rax,qword ptr [rsp+00000000000002A0h]
                    00 00
  0000000140053CB7: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140053CBA: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  0000000140053CC2: 48 8B 8C 24 A0 02  mov         rcx,qword ptr [rsp+00000000000002A0h]
                    00 00
  0000000140053CCA: FF 90 C8 01 00 00  call        qword ptr [rax+00000000000001C8h]
  0000000140053CD0: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140053CD8: 48 83 BC 24 F0 00  cmp         qword ptr [rsp+00000000000000F0h],0
                    00 00 00
  0000000140053CE1: 74 4B              je          0000000140053D2E
  0000000140053CE3: 48 8B 94 24 D0 00  mov         rdx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140053CEB: 48 81 C2 28 08 00  add         rdx,828h
                    00
  0000000140053CF2: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000140053CFA: FF 15 80 07 DB 01  call        qword ptr [0000000141E04480h]
  0000000140053D00: 90                 nop
  0000000140053D01: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140053D09: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140053D0C: 48 8D 94 24 18 01  lea         rdx,[rsp+0000000000000118h]
                    00 00
  0000000140053D14: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140053D1C: FF 50 40           call        qword ptr [rax+40h]
  0000000140053D1F: 90                 nop
  0000000140053D20: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000140053D28: FF 15 B2 1C DB 01  call        qword ptr [0000000141E059E0h]
  0000000140053D2E: C6 84 24 D9 00 00  mov         byte ptr [rsp+00000000000000D9h],0
                    00 00
  0000000140053D36: C6 84 24 E8 00 00  mov         byte ptr [rsp+00000000000000E8h],0
                    00 00
  0000000140053D3E: 0F B6 84 24 F8 02  movzx       eax,byte ptr [rsp+00000000000002F8h]
                    00 00
  0000000140053D46: 88 84 24 D8 00 00  mov         byte ptr [rsp+00000000000000D8h],al
                    00
  0000000140053D4D: 0F B6 84 24 D8 00  movzx       eax,byte ptr [rsp+00000000000000D8h]
                    00 00
  0000000140053D55: 88 44 24 28        mov         byte ptr [rsp+28h],al
  0000000140053D59: 0F B6 84 24 E8 00  movzx       eax,byte ptr [rsp+00000000000000E8h]
                    00 00
  0000000140053D61: 88 44 24 20        mov         byte ptr [rsp+20h],al
  0000000140053D65: 44 0F B6 8C 24 D9  movzx       r9d,byte ptr [rsp+00000000000000D9h]
                    00 00 00
  0000000140053D6E: 44 8B 84 24 C8 00  mov         r8d,dword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140053D76: 8B 94 24 B8 00 00  mov         edx,dword ptr [rsp+00000000000000B8h]
                    00
  0000000140053D7D: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053D85: E8 D6 3E 00 00     call        0000000140057C60
  0000000140053D8A: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140053D92: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140053D9A: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140053DA2: 8B 50 08           mov         edx,dword ptr [rax+8]
  0000000140053DA5: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053DAD: E8 1E D5 00 00     call        00000001400612D0
  0000000140053DB2: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140053DBA: 48 8B 94 24 D0 00  mov         rdx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140053DC2: 48 81 C2 28 08 00  add         rdx,828h
                    00
  0000000140053DC9: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  0000000140053DD1: FF 15 A9 06 DB 01  call        qword ptr [0000000141E04480h]
  0000000140053DD7: 90                 nop
  0000000140053DD8: 48 8D 94 24 20 01  lea         rdx,[rsp+0000000000000120h]
                    00 00
  0000000140053DE0: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140053DE8: E8 13 99 FF FF     call        000000014004D700
  0000000140053DED: 90                 nop
  0000000140053DEE: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  0000000140053DF6: FF 15 E4 1B DB 01  call        qword ptr [0000000141E059E0h]
  0000000140053DFC: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140053E04: 8B 50 08           mov         edx,dword ptr [rax+8]
  0000000140053E07: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053E0F: E8 2C 0F 01 00     call        0000000140064D40
  0000000140053E14: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140053E1C: E8 7F 96 7F 00     call        000000014084D4A0
  0000000140053E21: 0F B6 C0           movzx       eax,al
  0000000140053E24: 85 C0              test        eax,eax
  0000000140053E26: 0F 84 50 01 00 00  je          0000000140053F7C
  0000000140053E2C: B2 01              mov         dl,1
  0000000140053E2E: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140053E36: E8 A5 A2 FF FF     call        000000014004E0E0
  0000000140053E3B: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140053E43: E8 48 A0 FF FF     call        000000014004DE90
  0000000140053E48: 89 84 24 F8 00 00  mov         dword ptr [rsp+00000000000000F8h],eax
                    00
  0000000140053E4F: 83 BC 24 F8 00 00  cmp         dword ptr [rsp+00000000000000F8h],0
                    00 00
  0000000140053E57: 7F 51              jg          0000000140053EAA
  0000000140053E59: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053E61: E8 CA 11 00 00     call        0000000140055030
  0000000140053E66: 48 8B C8           mov         rcx,rax
  0000000140053E69: E8 02 1B 05 00     call        00000001400A5970
  0000000140053E6E: 48 89 84 24 A8 02  mov         qword ptr [rsp+00000000000002A8h],rax
                    00 00
  0000000140053E76: 48 8B 84 24 A8 02  mov         rax,qword ptr [rsp+00000000000002A8h]
                    00 00
  0000000140053E7E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140053E81: 48 8B 8C 24 A8 02  mov         rcx,qword ptr [rsp+00000000000002A8h]
                    00 00
  0000000140053E89: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  0000000140053E8F: 89 84 24 F8 00 00  mov         dword ptr [rsp+00000000000000F8h],eax
                    00
  0000000140053E96: 8B 94 24 F8 00 00  mov         edx,dword ptr [rsp+00000000000000F8h]
                    00
  0000000140053E9D: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140053EA5: E8 06 A0 FF FF     call        000000014004DEB0
  0000000140053EAA: 0F B6 84 24 00 03  movzx       eax,byte ptr [rsp+0000000000000300h]
                    00 00
  0000000140053EB2: 85 C0              test        eax,eax
  0000000140053EB4: 0F 84 C0 00 00 00  je          0000000140053F7A
  0000000140053EBA: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053EC2: E8 69 11 00 00     call        0000000140055030
  0000000140053EC7: 48 8B C8           mov         rcx,rax
  0000000140053ECA: E8 A1 1A 05 00     call        00000001400A5970
  0000000140053ECF: 48 89 84 24 B0 02  mov         qword ptr [rsp+00000000000002B0h],rax
                    00 00
  0000000140053ED7: 48 8B 84 24 B0 02  mov         rax,qword ptr [rsp+00000000000002B0h]
                    00 00
  0000000140053EDF: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140053EE2: 4C 8D 84 24 F8 00  lea         r8,[rsp+00000000000000F8h]
                    00 00
  0000000140053EEA: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  0000000140053EF2: 48 8B 8C 24 B0 02  mov         rcx,qword ptr [rsp+00000000000002B0h]
                    00 00
  0000000140053EFA: FF 90 08 02 00 00  call        qword ptr [rax+0000000000000208h]
  0000000140053F00: 90                 nop
  0000000140053F01: 48 8D 84 24 28 01  lea         rax,[rsp+0000000000000128h]
                    00 00
  0000000140053F09: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  0000000140053F11: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  0000000140053F19: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140053F21: FF 15 B1 1A DB 01  call        qword ptr [0000000141E059D8h]
  0000000140053F27: 48 89 84 24 B8 02  mov         qword ptr [rsp+00000000000002B8h],rax
                    00 00
  0000000140053F2F: 48 8B 84 24 B8 02  mov         rax,qword ptr [rsp+00000000000002B8h]
                    00 00
  0000000140053F37: 48 89 84 24 C0 02  mov         qword ptr [rsp+00000000000002C0h],rax
                    00 00
  0000000140053F3F: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140053F44: E8 87 6D C2 00     call        0000000140C7ACD0
  0000000140053F49: 90                 nop
  0000000140053F4A: 41 B1 01           mov         r9b,1
  0000000140053F4D: 4C 8B 84 24 C0 02  mov         r8,qword ptr [rsp+00000000000002C0h]
                    00 00
  0000000140053F55: 48 8B 94 24 D0 00  mov         rdx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140053F5D: 48 8B C8           mov         rcx,rax
  0000000140053F60: E8 2B 42 30 00     call        0000000140358190
  0000000140053F65: 88 84 24 FC 00 00  mov         byte ptr [rsp+00000000000000FCh],al
                    00
  0000000140053F6C: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140053F74: FF 15 66 1A DB 01  call        qword ptr [0000000141E059E0h]
  0000000140053F7A: EB 1E              jmp         0000000140053F9A
  0000000140053F7C: 33 D2              xor         edx,edx
  0000000140053F7E: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140053F86: E8 55 A1 FF FF     call        000000014004E0E0
  0000000140053F8B: 33 D2              xor         edx,edx
  0000000140053F8D: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140053F95: E8 16 9F FF FF     call        000000014004DEB0
  0000000140053F9A: E9 20 FC FF FF     jmp         0000000140053BBF
  0000000140053F9F: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140053FA4: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  0000000140053FAC: 48 8B 8C 24 38 01  mov         rcx,qword ptr [rsp+0000000000000138h]
                    00 00
  0000000140053FB4: E8 AF BC 48 01     call        00000001414DFC68
  0000000140053FB9: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053FC1: E8 0A 11 00 00     call        00000001400550D0
  0000000140053FC6: 48 8B C8           mov         rcx,rax
  0000000140053FC9: E8 52 56 01 00     call        0000000140069620
  0000000140053FCE: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140053FD6: E8 B5 0E 00 00     call        0000000140054E90
  0000000140053FDB: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140053FE3: 48 83 BC 24 90 00  cmp         qword ptr [rsp+0000000000000090h],0
                    00 00 00
  0000000140053FEC: 74 19              je          0000000140054007
  0000000140053FEE: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140053FF6: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140053FF9: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140054001: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  0000000140054007: 0F B6 84 24 F8 02  movzx       eax,byte ptr [rsp+00000000000002F8h]
                    00 00
  000000014005400F: 85 C0              test        eax,eax
  0000000140054011: 0F 84 28 02 00 00  je          000000014005423F
  0000000140054017: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  000000014005401F: E8 EC C8 FF FF     call        0000000140050910
  0000000140054024: 8B F8              mov         edi,eax
  0000000140054026: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  000000014005402E: E8 1D 11 00 00     call        0000000140055150
  0000000140054033: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140054038: 8B D7              mov         edx,edi
  000000014005403A: 48 8B C8           mov         rcx,rax
  000000014005403D: E8 2E 32 03 00     call        0000000140087270
  0000000140054042: 45 33 C0           xor         r8d,r8d
  0000000140054045: 33 D2              xor         edx,edx
  0000000140054047: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  000000014005404F: FF 15 13 01 DB 01  call        qword ptr [0000000141E04168h]
  0000000140054055: 48 8D BC 24 70 02  lea         rdi,[rsp+0000000000000270h]
                    00 00
  000000014005405D: 48 8B F0           mov         rsi,rax
  0000000140054060: B9 10 00 00 00     mov         ecx,10h
  0000000140054065: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140054067: 45 33 C0           xor         r8d,r8d
  000000014005406A: 33 D2              xor         edx,edx
  000000014005406C: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  0000000140054074: FF 15 EE 00 DB 01  call        qword ptr [0000000141E04168h]
  000000014005407A: 48 8D BC 24 60 02  lea         rdi,[rsp+0000000000000260h]
                    00 00
  0000000140054082: 48 8B F0           mov         rsi,rax
  0000000140054085: B9 10 00 00 00     mov         ecx,10h
  000000014005408A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014005408C: 45 33 C0           xor         r8d,r8d
  000000014005408F: 33 D2              xor         edx,edx
  0000000140054091: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  0000000140054099: FF 15 C9 00 DB 01  call        qword ptr [0000000141E04168h]
  000000014005409F: 48 8D BC 24 50 02  lea         rdi,[rsp+0000000000000250h]
                    00 00
  00000001400540A7: 48 8B F0           mov         rsi,rax
  00000001400540AA: B9 10 00 00 00     mov         ecx,10h
  00000001400540AF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400540B1: 45 33 C0           xor         r8d,r8d
  00000001400540B4: 33 D2              xor         edx,edx
  00000001400540B6: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  00000001400540BE: FF 15 A4 00 DB 01  call        qword ptr [0000000141E04168h]
  00000001400540C4: 48 8D BC 24 40 02  lea         rdi,[rsp+0000000000000240h]
                    00 00
  00000001400540CC: 48 8B F0           mov         rsi,rax
  00000001400540CF: B9 10 00 00 00     mov         ecx,10h
  00000001400540D4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400540D6: 45 33 C0           xor         r8d,r8d
  00000001400540D9: 33 D2              xor         edx,edx
  00000001400540DB: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  00000001400540E3: FF 15 7F 00 DB 01  call        qword ptr [0000000141E04168h]
  00000001400540E9: 48 8D BC 24 30 02  lea         rdi,[rsp+0000000000000230h]
                    00 00
  00000001400540F1: 48 8B F0           mov         rsi,rax
  00000001400540F4: B9 10 00 00 00     mov         ecx,10h
  00000001400540F9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400540FB: 45 33 C0           xor         r8d,r8d
  00000001400540FE: 33 D2              xor         edx,edx
  0000000140054100: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  0000000140054108: FF 15 5A 00 DB 01  call        qword ptr [0000000141E04168h]
  000000014005410E: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  0000000140054116: 48 8B F0           mov         rsi,rax
  0000000140054119: B9 10 00 00 00     mov         ecx,10h
  000000014005411E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140054120: 45 33 C0           xor         r8d,r8d
  0000000140054123: 33 D2              xor         edx,edx
  0000000140054125: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  000000014005412D: FF 15 35 00 DB 01  call        qword ptr [0000000141E04168h]
  0000000140054133: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  000000014005413B: 48 8B F0           mov         rsi,rax
  000000014005413E: B9 10 00 00 00     mov         ecx,10h
  0000000140054143: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140054145: 45 33 C0           xor         r8d,r8d
  0000000140054148: 33 D2              xor         edx,edx
  000000014005414A: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  0000000140054152: FF 15 10 00 DB 01  call        qword ptr [0000000141E04168h]
  0000000140054158: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  0000000140054160: 48 8B F0           mov         rsi,rax
  0000000140054163: B9 10 00 00 00     mov         ecx,10h
  0000000140054168: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014005416A: 45 33 C0           xor         r8d,r8d
  000000014005416D: 33 D2              xor         edx,edx
  000000014005416F: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  0000000140054177: FF 15 EB FF DA 01  call        qword ptr [0000000141E04168h]
  000000014005417D: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  0000000140054185: 48 8B F0           mov         rsi,rax
  0000000140054188: B9 10 00 00 00     mov         ecx,10h
  000000014005418D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014005418F: 45 33 C0           xor         r8d,r8d
  0000000140054192: 33 D2              xor         edx,edx
  0000000140054194: 48 8D 8C 24 D0 01  lea         rcx,[rsp+00000000000001D0h]
                    00 00
  000000014005419C: FF 15 C6 FF DA 01  call        qword ptr [0000000141E04168h]
  00000001400541A2: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  00000001400541AA: 48 8B F0           mov         rsi,rax
  00000001400541AD: B9 10 00 00 00     mov         ecx,10h
  00000001400541B2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400541B4: 48 8D 84 24 70 02  lea         rax,[rsp+0000000000000270h]
                    00 00
  00000001400541BC: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400541C1: 48 8D 84 24 60 02  lea         rax,[rsp+0000000000000260h]
                    00 00
  00000001400541C9: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400541CE: 48 8D 84 24 50 02  lea         rax,[rsp+0000000000000250h]
                    00 00
  00000001400541D6: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400541DB: 48 8D 84 24 40 02  lea         rax,[rsp+0000000000000240h]
                    00 00
  00000001400541E3: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400541E8: 48 8D 84 24 30 02  lea         rax,[rsp+0000000000000230h]
                    00 00
  00000001400541F0: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400541F5: 48 8D 84 24 20 02  lea         rax,[rsp+0000000000000220h]
                    00 00
  00000001400541FD: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140054202: 48 8D 84 24 10 02  lea         rax,[rsp+0000000000000210h]
                    00 00
  000000014005420A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014005420F: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  0000000140054217: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014005421C: 4C 8D 8C 24 F0 01  lea         r9,[rsp+00000000000001F0h]
                    00 00
  0000000140054224: 4C 8D 84 24 E0 01  lea         r8,[rsp+00000000000001E0h]
                    00 00
  000000014005422C: 48 8D 15 6D EC 0C  lea         rdx,[0000000142122EA0h]
                    02
  0000000140054233: 48 8D 0D C6 24 D2  lea         rcx,[0000000143D76700h]
                    03
  000000014005423A: E8 81 B1 27 00     call        00000001402CF3C0
  000000014005423F: 48 81 C4 D8 02 00  add         rsp,2D8h
                    00
  0000000140054246: 5F                 pop         rdi
  0000000140054247: 5E                 pop         rsi
  0000000140054248: C3                 ret

  Summary

     1E02000 .text
