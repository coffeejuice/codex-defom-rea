Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140032800: 44 88 4C 24 20     mov         byte ptr [rsp+20h],r9b
  0000000140032805: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000014003280A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014003280F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140032814: 56                 push        rsi
  0000000140032815: 57                 push        rdi
  0000000140032816: 48 81 EC A8 06 00  sub         rsp,6A8h
                    00
  000000014003281D: 48 C7 84 24 30 06  mov         qword ptr [rsp+0000000000000630h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140032829: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032831: 83 B8 70 02 00 00  cmp         dword ptr [rax+0000000000000270h],2
                    02
  0000000140032838: 0F 84 D8 11 00 00  je          0000000140033A16
  000000014003283E: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032846: 48 8B 84 24 D0 06  mov         rax,qword ptr [rsp+00000000000006D0h]
                    00 00
  000000014003284E: 48 89 81 C0 04 00  mov         qword ptr [rcx+00000000000004C0h],rax
                    00
  0000000140032855: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003285D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140032860: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032868: FF 90 A0 02 00 00  call        qword ptr [rax+00000000000002A0h]
  000000014003286E: 33 D2              xor         edx,edx
  0000000140032870: 48 8B C8           mov         rcx,rax
  0000000140032873: FF 15 9F 8F DD 01  call        qword ptr [0000000141E0B818h]
  0000000140032879: B9 88 00 00 00     mov         ecx,88h
  000000014003287E: E8 39 C2 43 01     call        000000014146EABC
  0000000140032883: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  000000014003288B: 48 83 BC 24 00 01  cmp         qword ptr [rsp+0000000000000100h],0
                    00 00 00
  0000000140032894: 74 19              je          00000001400328AF
  0000000140032896: 33 D2              xor         edx,edx
  0000000140032898: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001400328A0: E8 0B 1F 1C 00     call        00000001401F47B0
  00000001400328A5: 48 89 84 24 38 06  mov         qword ptr [rsp+0000000000000638h],rax
                    00 00
  00000001400328AD: EB 0C              jmp         00000001400328BB
  00000001400328AF: 48 C7 84 24 38 06  mov         qword ptr [rsp+0000000000000638h],0
                    00 00 00 00 00 00
  00000001400328BB: 48 8B 84 24 38 06  mov         rax,qword ptr [rsp+0000000000000638h]
                    00 00
  00000001400328C3: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  00000001400328CB: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001400328D3: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400328DB: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400328E3: E8 A8 5C FE FF     call        0000000140018590
  00000001400328E8: 48 8B D0           mov         rdx,rax
  00000001400328EB: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400328F3: E8 58 21 1C 00     call        00000001401F4A50
  00000001400328F8: 0F B6 94 24 D8 06  movzx       edx,byte ptr [rsp+00000000000006D8h]
                    00 00
  0000000140032900: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140032908: E8 93 74 1C 00     call        00000001401F9DA0
  000000014003290D: 48 8B 94 24 C8 06  mov         rdx,qword ptr [rsp+00000000000006C8h]
                    00 00
  0000000140032915: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014003291D: E8 0E CD 00 00     call        000000014003F630
  0000000140032922: 48 8B 8C 24 D0 06  mov         rcx,qword ptr [rsp+00000000000006D0h]
                    00 00
  000000014003292A: E8 91 8A C4 00     call        0000000140C7B3C0
  000000014003292F: 0F B6 D0           movzx       edx,al
  0000000140032932: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003293A: 48 8B 89 60 02 00  mov         rcx,qword ptr [rcx+0000000000000260h]
                    00
  0000000140032941: E8 AA B9 1B 00     call        00000001401EE2F0
  0000000140032946: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003294E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140032951: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032959: FF 90 60 02 00 00  call        qword ptr [rax+0000000000000260h]
  000000014003295F: 8B D0              mov         edx,eax
  0000000140032961: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032969: 48 8B 89 60 02 00  mov         rcx,qword ptr [rcx+0000000000000260h]
                    00
  0000000140032970: E8 3B C2 1B 00     call        00000001401EEBB0
  0000000140032975: 48 8B 15 14 18 DD  mov         rdx,qword ptr [0000000141E04190h]
                    01
  000000014003297C: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  0000000140032984: FF 15 0E 18 DD 01  call        qword ptr [0000000141E04198h]
  000000014003298A: 90                 nop
  000000014003298B: 4C 8D 84 24 08 01  lea         r8,[rsp+0000000000000108h]
                    00 00
  0000000140032993: BA 03 00 00 00     mov         edx,3
  0000000140032998: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400329A0: E8 2B 35 00 00     call        0000000140035ED0
  00000001400329A5: 90                 nop
  00000001400329A6: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  00000001400329AE: FF 15 2C 30 DD 01  call        qword ptr [0000000141E059E0h]
  00000001400329B4: 48 8B 8C 24 C8 06  mov         rcx,qword ptr [rsp+00000000000006C8h]
                    00 00
  00000001400329BC: E8 EF 23 80 00     call        0000000140834DB0
  00000001400329C1: 0F B6 C0           movzx       eax,al
  00000001400329C4: 85 C0              test        eax,eax
  00000001400329C6: 75 1D              jne         00000001400329E5
  00000001400329C8: C6 84 24 10 01 00  mov         byte ptr [rsp+0000000000000110h],1
                    00 01
  00000001400329D0: 48 8D 94 24 10 01  lea         rdx,[rsp+0000000000000110h]
                    00 00
  00000001400329D8: 48 8B 8C 24 C8 06  mov         rcx,qword ptr [rsp+00000000000006C8h]
                    00 00
  00000001400329E0: E8 AB DB 83 00     call        0000000140870590
  00000001400329E5: 33 D2              xor         edx,edx
  00000001400329E7: 48 8B 8C 24 C8 06  mov         rcx,qword ptr [rsp+00000000000006C8h]
                    00 00
  00000001400329EF: E8 EC 96 83 00     call        000000014086C0E0
  00000001400329F4: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001400329F9: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032A01: 48 8B 84 24 C8 06  mov         rax,qword ptr [rsp+00000000000006C8h]
                    00 00
  0000000140032A09: 48 89 81 C8 04 00  mov         qword ptr [rcx+00000000000004C8h],rax
                    00
  0000000140032A10: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032A18: 0F B6 84 24 E8 06  movzx       eax,byte ptr [rsp+00000000000006E8h]
                    00 00
  0000000140032A20: 88 81 A8 04 00 00  mov         byte ptr [rcx+00000000000004A8h],al
  0000000140032A26: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032A2E: E8 6D 09 FE FF     call        00000001400133A0
  0000000140032A33: 48 8B C8           mov         rcx,rax
  0000000140032A36: E8 45 2D 01 00     call        0000000140045780
  0000000140032A3B: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140032A40: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140032A45: E8 46 49 07 00     call        00000001400A7390
  0000000140032A4A: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140032A52: C7 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00
  0000000140032A5D: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140032A65: E8 C6 D8 FD FF     call        0000000140010330
  0000000140032A6A: 0F B6 C0           movzx       eax,al
  0000000140032A6D: 85 C0              test        eax,eax
  0000000140032A6F: 75 18              jne         0000000140032A89
  0000000140032A71: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140032A79: E8 E2 D8 FD FF     call        0000000140010360
  0000000140032A7E: 0F B6 C0           movzx       eax,al
  0000000140032A81: 85 C0              test        eax,eax
  0000000140032A83: 0F 84 72 01 00 00  je          0000000140032BFB
  0000000140032A89: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032A91: E8 FA AA FE FF     call        000000014001D590
  0000000140032A96: 48 8B C8           mov         rcx,rax
  0000000140032A99: E8 72 DE 01 00     call        0000000140050910
  0000000140032A9E: 89 84 24 94 00 00  mov         dword ptr [rsp+0000000000000094h],eax
                    00
  0000000140032AA5: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140032AAD: E8 DE 2F 08 00     call        00000001400B5A90
  0000000140032AB2: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140032ABA: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032AC2: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140032AC5: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032ACD: FF 90 60 02 00 00  call        qword ptr [rax+0000000000000260h]
  0000000140032AD3: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140032AD7: 4C 8B 8C 24 C8 06  mov         r9,qword ptr [rsp+00000000000006C8h]
                    00 00
  0000000140032ADF: 44 8B 84 24 94 00  mov         r8d,dword ptr [rsp+0000000000000094h]
                    00 00
  0000000140032AE7: 48 8B 94 24 C0 06  mov         rdx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032AEF: 48 8B 92 60 02 00  mov         rdx,qword ptr [rdx+0000000000000260h]
                    00
  0000000140032AF6: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140032AFE: E8 5D 1A 0A 00     call        00000001400D4560
  0000000140032B03: 0F B6 C0           movzx       eax,al
  0000000140032B06: 85 C0              test        eax,eax
  0000000140032B08: 74 1C              je          0000000140032B26
  0000000140032B0A: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032B12: 48 8B 84 24 C8 06  mov         rax,qword ptr [rsp+00000000000006C8h]
                    00 00
  0000000140032B1A: 48 89 81 B0 04 00  mov         qword ptr [rcx+00000000000004B0h],rax
                    00
  0000000140032B21: E9 D0 00 00 00     jmp         0000000140032BF6
  0000000140032B26: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032B2E: 48 C7 80 B0 04 00  mov         qword ptr [rax+00000000000004B0h],0
                    00 00 00 00 00
  0000000140032B39: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032B41: 48 8B 84 24 C8 06  mov         rax,qword ptr [rsp+00000000000006C8h]
                    00 00
  0000000140032B49: 48 89 81 B8 04 00  mov         qword ptr [rcx+00000000000004B8h],rax
                    00
  0000000140032B50: 48 8D 84 24 18 01  lea         rax,[rsp+0000000000000118h]
                    00 00
  0000000140032B58: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  0000000140032B60: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140032B68: FF 15 52 19 DD 01  call        qword ptr [0000000141E044C0h]
  0000000140032B6E: 48 89 84 24 40 06  mov         qword ptr [rsp+0000000000000640h],rax
                    00 00
  0000000140032B76: 48 8B 84 24 40 06  mov         rax,qword ptr [rsp+0000000000000640h]
                    00 00
  0000000140032B7E: 48 89 84 24 48 06  mov         qword ptr [rsp+0000000000000648h],rax
                    00 00
  0000000140032B86: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032B8E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140032B91: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032B99: FF 90 60 02 00 00  call        qword ptr [rax+0000000000000260h]
  0000000140032B9F: 44 8B C8           mov         r9d,eax
  0000000140032BA2: 48 8B 84 24 48 06  mov         rax,qword ptr [rsp+0000000000000648h]
                    00 00
  0000000140032BAA: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140032BAF: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140032BB4: 4C 8B 44 24 78     mov         r8,qword ptr [rsp+78h]
  0000000140032BB9: 48 8B 94 24 C8 06  mov         rdx,qword ptr [rsp+00000000000006C8h]
                    00 00
  0000000140032BC1: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032BC9: 48 8B 89 60 02 00  mov         rcx,qword ptr [rcx+0000000000000260h]
                    00
  0000000140032BD0: E8 1B F6 1B 00     call        00000001401F21F0
  0000000140032BD5: 0F B6 84 24 E0 06  movzx       eax,byte ptr [rsp+00000000000006E0h]
                    00 00
  0000000140032BDD: 85 C0              test        eax,eax
  0000000140032BDF: 74 15              je          0000000140032BF6
  0000000140032BE1: 48 8B 94 24 D0 06  mov         rdx,qword ptr [rsp+00000000000006D0h]
                    00 00
  0000000140032BE9: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032BF1: E8 3A 0E 00 00     call        0000000140033A30
  0000000140032BF6: E9 A6 00 00 00     jmp         0000000140032CA1
  0000000140032BFB: 48 8D 84 24 28 01  lea         rax,[rsp+0000000000000128h]
                    00 00
  0000000140032C03: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  0000000140032C0B: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140032C13: FF 15 A7 18 DD 01  call        qword ptr [0000000141E044C0h]
  0000000140032C19: 48 89 84 24 50 06  mov         qword ptr [rsp+0000000000000650h],rax
                    00 00
  0000000140032C21: 48 8B 84 24 50 06  mov         rax,qword ptr [rsp+0000000000000650h]
                    00 00
  0000000140032C29: 48 89 84 24 58 06  mov         qword ptr [rsp+0000000000000658h],rax
                    00 00
  0000000140032C31: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032C39: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140032C3C: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032C44: FF 90 60 02 00 00  call        qword ptr [rax+0000000000000260h]
  0000000140032C4A: 44 8B C8           mov         r9d,eax
  0000000140032C4D: 48 8B 84 24 58 06  mov         rax,qword ptr [rsp+0000000000000658h]
                    00 00
  0000000140032C55: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140032C5A: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140032C5F: 4C 8B 44 24 78     mov         r8,qword ptr [rsp+78h]
  0000000140032C64: 48 8B 94 24 C8 06  mov         rdx,qword ptr [rsp+00000000000006C8h]
                    00 00
  0000000140032C6C: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032C74: 48 8B 89 60 02 00  mov         rcx,qword ptr [rcx+0000000000000260h]
                    00
  0000000140032C7B: E8 70 F5 1B 00     call        00000001401F21F0
  0000000140032C80: 0F B6 84 24 E0 06  movzx       eax,byte ptr [rsp+00000000000006E0h]
                    00 00
  0000000140032C88: 85 C0              test        eax,eax
  0000000140032C8A: 74 15              je          0000000140032CA1
  0000000140032C8C: 48 8B 94 24 D0 06  mov         rdx,qword ptr [rsp+00000000000006D0h]
                    00 00
  0000000140032C94: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032C9C: E8 8F 0D 00 00     call        0000000140033A30
  0000000140032CA1: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140032CA9: 0F 85 67 0D 00 00  jne         0000000140033A16
  0000000140032CAF: 33 D2              xor         edx,edx
  0000000140032CB1: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032CB9: FF 15 51 8B DD 01  call        qword ptr [0000000141E0B810h]
  0000000140032CBF: 48 8B 94 24 C0 06  mov         rdx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032CC7: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032CCF: 8B 80 70 02 00 00  mov         eax,dword ptr [rax+0000000000000270h]
  0000000140032CD5: 89 82 74 02 00 00  mov         dword ptr [rdx+0000000000000274h],eax
  0000000140032CDB: BA 02 00 00 00     mov         edx,2
  0000000140032CE0: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032CE8: 48 8B 89 68 02 00  mov         rcx,qword ptr [rcx+0000000000000268h]
                    00
  0000000140032CEF: FF 15 73 8E DD 01  call        qword ptr [0000000141E0BB68h]
  0000000140032CF5: 48 8B 94 24 C0 06  mov         rdx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032CFD: C7 82 70 02 00 00  mov         dword ptr [rdx+0000000000000270h],2
                    02 00 00 00
  0000000140032D07: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032D0F: 48 8B 89 80 04 00  mov         rcx,qword ptr [rcx+0000000000000480h]
                    00
  0000000140032D16: FF 15 1C 8D DD 01  call        qword ptr [0000000141E0BA38h]
  0000000140032D1C: 8B D0              mov         edx,eax
  0000000140032D1E: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032D26: 89 90 78 02 00 00  mov         dword ptr [rax+0000000000000278h],edx
  0000000140032D2C: 4C 8B 84 24 C0 06  mov         r8,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032D34: 49 81 C0 80 02 00  add         r8,280h
                    00
  0000000140032D3B: 48 8D 15 AE 23 0D  lea         rdx,[00000001421050F0h]
                    02
  0000000140032D42: 48 8D 8C 24 E8 01  lea         rcx,[rsp+00000000000001E8h]
                    00 00
  0000000140032D4A: E8 A1 99 07 00     call        00000001400AC6F0
  0000000140032D4F: 48 8D BC 24 20 06  lea         rdi,[rsp+0000000000000620h]
                    00 00
  0000000140032D57: 48 8B F0           mov         rsi,rax
  0000000140032D5A: B9 10 00 00 00     mov         ecx,10h
  0000000140032D5F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032D61: 48 8D BC 24 D8 01  lea         rdi,[rsp+00000000000001D8h]
                    00 00
  0000000140032D69: 48 8D B4 24 20 06  lea         rsi,[rsp+0000000000000620h]
                    00 00
  0000000140032D71: B9 10 00 00 00     mov         ecx,10h
  0000000140032D76: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032D78: 45 33 C0           xor         r8d,r8d
  0000000140032D7B: 33 D2              xor         edx,edx
  0000000140032D7D: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  0000000140032D85: FF 15 DD 13 DD 01  call        qword ptr [0000000141E04168h]
  0000000140032D8B: 48 8D BC 24 A0 04  lea         rdi,[rsp+00000000000004A0h]
                    00 00
  0000000140032D93: 48 8B F0           mov         rsi,rax
  0000000140032D96: B9 10 00 00 00     mov         ecx,10h
  0000000140032D9B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032D9D: 45 33 C0           xor         r8d,r8d
  0000000140032DA0: 33 D2              xor         edx,edx
  0000000140032DA2: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  0000000140032DAA: FF 15 B8 13 DD 01  call        qword ptr [0000000141E04168h]
  0000000140032DB0: 48 8D BC 24 90 04  lea         rdi,[rsp+0000000000000490h]
                    00 00
  0000000140032DB8: 48 8B F0           mov         rsi,rax
  0000000140032DBB: B9 10 00 00 00     mov         ecx,10h
  0000000140032DC0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032DC2: 45 33 C0           xor         r8d,r8d
  0000000140032DC5: 33 D2              xor         edx,edx
  0000000140032DC7: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  0000000140032DCF: FF 15 93 13 DD 01  call        qword ptr [0000000141E04168h]
  0000000140032DD5: 48 8D BC 24 80 04  lea         rdi,[rsp+0000000000000480h]
                    00 00
  0000000140032DDD: 48 8B F0           mov         rsi,rax
  0000000140032DE0: B9 10 00 00 00     mov         ecx,10h
  0000000140032DE5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032DE7: 45 33 C0           xor         r8d,r8d
  0000000140032DEA: 33 D2              xor         edx,edx
  0000000140032DEC: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  0000000140032DF4: FF 15 6E 13 DD 01  call        qword ptr [0000000141E04168h]
  0000000140032DFA: 48 8D BC 24 70 04  lea         rdi,[rsp+0000000000000470h]
                    00 00
  0000000140032E02: 48 8B F0           mov         rsi,rax
  0000000140032E05: B9 10 00 00 00     mov         ecx,10h
  0000000140032E0A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032E0C: 45 33 C0           xor         r8d,r8d
  0000000140032E0F: 33 D2              xor         edx,edx
  0000000140032E11: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  0000000140032E19: FF 15 49 13 DD 01  call        qword ptr [0000000141E04168h]
  0000000140032E1F: 48 8D BC 24 60 04  lea         rdi,[rsp+0000000000000460h]
                    00 00
  0000000140032E27: 48 8B F0           mov         rsi,rax
  0000000140032E2A: B9 10 00 00 00     mov         ecx,10h
  0000000140032E2F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032E31: 45 33 C0           xor         r8d,r8d
  0000000140032E34: 33 D2              xor         edx,edx
  0000000140032E36: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140032E3E: FF 15 24 13 DD 01  call        qword ptr [0000000141E04168h]
  0000000140032E44: 48 8D BC 24 50 04  lea         rdi,[rsp+0000000000000450h]
                    00 00
  0000000140032E4C: 48 8B F0           mov         rsi,rax
  0000000140032E4F: B9 10 00 00 00     mov         ecx,10h
  0000000140032E54: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032E56: 45 33 C0           xor         r8d,r8d
  0000000140032E59: 33 D2              xor         edx,edx
  0000000140032E5B: 48 8D 8C 24 98 01  lea         rcx,[rsp+0000000000000198h]
                    00 00
  0000000140032E63: FF 15 FF 12 DD 01  call        qword ptr [0000000141E04168h]
  0000000140032E69: 48 8D BC 24 40 04  lea         rdi,[rsp+0000000000000440h]
                    00 00
  0000000140032E71: 48 8B F0           mov         rsi,rax
  0000000140032E74: B9 10 00 00 00     mov         ecx,10h
  0000000140032E79: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032E7B: 45 33 C0           xor         r8d,r8d
  0000000140032E7E: 33 D2              xor         edx,edx
  0000000140032E80: 48 8D 8C 24 A8 01  lea         rcx,[rsp+00000000000001A8h]
                    00 00
  0000000140032E88: FF 15 DA 12 DD 01  call        qword ptr [0000000141E04168h]
  0000000140032E8E: 48 8D BC 24 30 04  lea         rdi,[rsp+0000000000000430h]
                    00 00
  0000000140032E96: 48 8B F0           mov         rsi,rax
  0000000140032E99: B9 10 00 00 00     mov         ecx,10h
  0000000140032E9E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032EA0: 45 33 C0           xor         r8d,r8d
  0000000140032EA3: 33 D2              xor         edx,edx
  0000000140032EA5: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  0000000140032EAD: FF 15 B5 12 DD 01  call        qword ptr [0000000141E04168h]
  0000000140032EB3: 48 8D BC 24 20 04  lea         rdi,[rsp+0000000000000420h]
                    00 00
  0000000140032EBB: 48 8B F0           mov         rsi,rax
  0000000140032EBE: B9 10 00 00 00     mov         ecx,10h
  0000000140032EC3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032EC5: 45 33 C0           xor         r8d,r8d
  0000000140032EC8: 33 D2              xor         edx,edx
  0000000140032ECA: 48 8D 8C 24 C8 01  lea         rcx,[rsp+00000000000001C8h]
                    00 00
  0000000140032ED2: FF 15 90 12 DD 01  call        qword ptr [0000000141E04168h]
  0000000140032ED8: 48 8D BC 24 10 04  lea         rdi,[rsp+0000000000000410h]
                    00 00
  0000000140032EE0: 48 8B F0           mov         rsi,rax
  0000000140032EE3: B9 10 00 00 00     mov         ecx,10h
  0000000140032EE8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032EEA: 48 8D BC 24 00 04  lea         rdi,[rsp+0000000000000400h]
                    00 00
  0000000140032EF2: 48 8D B4 24 D8 01  lea         rsi,[rsp+00000000000001D8h]
                    00 00
  0000000140032EFA: B9 10 00 00 00     mov         ecx,10h
  0000000140032EFF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140032F01: 48 8D 84 24 A0 04  lea         rax,[rsp+00000000000004A0h]
                    00 00
  0000000140032F09: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140032F0E: 48 8D 84 24 90 04  lea         rax,[rsp+0000000000000490h]
                    00 00
  0000000140032F16: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140032F1B: 48 8D 84 24 80 04  lea         rax,[rsp+0000000000000480h]
                    00 00
  0000000140032F23: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140032F28: 48 8D 84 24 70 04  lea         rax,[rsp+0000000000000470h]
                    00 00
  0000000140032F30: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140032F35: 48 8D 84 24 60 04  lea         rax,[rsp+0000000000000460h]
                    00 00
  0000000140032F3D: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140032F42: 48 8D 84 24 50 04  lea         rax,[rsp+0000000000000450h]
                    00 00
  0000000140032F4A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140032F4F: 48 8D 84 24 40 04  lea         rax,[rsp+0000000000000440h]
                    00 00
  0000000140032F57: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140032F5C: 48 8D 84 24 30 04  lea         rax,[rsp+0000000000000430h]
                    00 00
  0000000140032F64: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140032F69: 48 8D 84 24 20 04  lea         rax,[rsp+0000000000000420h]
                    00 00
  0000000140032F71: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140032F76: 4C 8D 8C 24 10 04  lea         r9,[rsp+0000000000000410h]
                    00 00
  0000000140032F7E: 4C 8D 84 24 00 04  lea         r8,[rsp+0000000000000400h]
                    00 00
  0000000140032F86: 48 8D 15 73 21 0D  lea         rdx,[0000000142105100h]
                    02
  0000000140032F8D: 48 8D 0D 6C 37 D4  lea         rcx,[0000000143D76700h]
                    03
  0000000140032F94: E8 A7 C7 29 00     call        00000001402CF740
  0000000140032F99: C6 84 24 B8 00 00  mov         byte ptr [rsp+00000000000000B8h],0
                    00 00
  0000000140032FA1: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032FA9: 83 B8 78 02 00 00  cmp         dword ptr [rax+0000000000000278h],2
                    02
  0000000140032FB0: 0F 85 5A 02 00 00  jne         0000000140033210
  0000000140032FB6: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032FBE: 48 83 B8 80 02 00  cmp         qword ptr [rax+0000000000000280h],0
                    00 00
  0000000140032FC6: 0F 84 44 02 00 00  je          0000000140033210
  0000000140032FCC: C6 84 24 B8 00 00  mov         byte ptr [rsp+00000000000000B8h],1
                    00 01
  0000000140032FD4: C6 84 24 A8 02 00  mov         byte ptr [rsp+00000000000002A8h],0
                    00 00
  0000000140032FDC: 4C 8D 84 24 A8 02  lea         r8,[rsp+00000000000002A8h]
                    00 00
  0000000140032FE4: 48 8D 15 35 21 0D  lea         rdx,[0000000142105120h]
                    02
  0000000140032FEB: 48 8D 8C 24 98 02  lea         rcx,[rsp+0000000000000298h]
                    00 00
  0000000140032FF3: E8 88 20 FF FF     call        0000000140025080
  0000000140032FF8: 48 8D BC 24 10 06  lea         rdi,[rsp+0000000000000610h]
                    00 00
  0000000140033000: 48 8B F0           mov         rsi,rax
  0000000140033003: B9 10 00 00 00     mov         ecx,10h
  0000000140033008: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014003300A: 48 8D BC 24 88 02  lea         rdi,[rsp+0000000000000288h]
                    00 00
  0000000140033012: 48 8D B4 24 10 06  lea         rsi,[rsp+0000000000000610h]
                    00 00
  000000014003301A: B9 10 00 00 00     mov         ecx,10h
  000000014003301F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033021: 45 33 C0           xor         r8d,r8d
  0000000140033024: 33 D2              xor         edx,edx
  0000000140033026: 48 8D 8C 24 F8 01  lea         rcx,[rsp+00000000000001F8h]
                    00 00
  000000014003302E: FF 15 34 11 DD 01  call        qword ptr [0000000141E04168h]
  0000000140033034: 48 8D BC 24 40 05  lea         rdi,[rsp+0000000000000540h]
                    00 00
  000000014003303C: 48 8B F0           mov         rsi,rax
  000000014003303F: B9 10 00 00 00     mov         ecx,10h
  0000000140033044: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033046: 45 33 C0           xor         r8d,r8d
  0000000140033049: 33 D2              xor         edx,edx
  000000014003304B: 48 8D 8C 24 08 02  lea         rcx,[rsp+0000000000000208h]
                    00 00
  0000000140033053: FF 15 0F 11 DD 01  call        qword ptr [0000000141E04168h]
  0000000140033059: 48 8D BC 24 30 05  lea         rdi,[rsp+0000000000000530h]
                    00 00
  0000000140033061: 48 8B F0           mov         rsi,rax
  0000000140033064: B9 10 00 00 00     mov         ecx,10h
  0000000140033069: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014003306B: 45 33 C0           xor         r8d,r8d
  000000014003306E: 33 D2              xor         edx,edx
  0000000140033070: 48 8D 8C 24 18 02  lea         rcx,[rsp+0000000000000218h]
                    00 00
  0000000140033078: FF 15 EA 10 DD 01  call        qword ptr [0000000141E04168h]
  000000014003307E: 48 8D BC 24 20 05  lea         rdi,[rsp+0000000000000520h]
                    00 00
  0000000140033086: 48 8B F0           mov         rsi,rax
  0000000140033089: B9 10 00 00 00     mov         ecx,10h
  000000014003308E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033090: 45 33 C0           xor         r8d,r8d
  0000000140033093: 33 D2              xor         edx,edx
  0000000140033095: 48 8D 8C 24 28 02  lea         rcx,[rsp+0000000000000228h]
                    00 00
  000000014003309D: FF 15 C5 10 DD 01  call        qword ptr [0000000141E04168h]
  00000001400330A3: 48 8D BC 24 10 05  lea         rdi,[rsp+0000000000000510h]
                    00 00
  00000001400330AB: 48 8B F0           mov         rsi,rax
  00000001400330AE: B9 10 00 00 00     mov         ecx,10h
  00000001400330B3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400330B5: 45 33 C0           xor         r8d,r8d
  00000001400330B8: 33 D2              xor         edx,edx
  00000001400330BA: 48 8D 8C 24 38 02  lea         rcx,[rsp+0000000000000238h]
                    00 00
  00000001400330C2: FF 15 A0 10 DD 01  call        qword ptr [0000000141E04168h]
  00000001400330C8: 48 8D BC 24 00 05  lea         rdi,[rsp+0000000000000500h]
                    00 00
  00000001400330D0: 48 8B F0           mov         rsi,rax
  00000001400330D3: B9 10 00 00 00     mov         ecx,10h
  00000001400330D8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400330DA: 45 33 C0           xor         r8d,r8d
  00000001400330DD: 33 D2              xor         edx,edx
  00000001400330DF: 48 8D 8C 24 48 02  lea         rcx,[rsp+0000000000000248h]
                    00 00
  00000001400330E7: FF 15 7B 10 DD 01  call        qword ptr [0000000141E04168h]
  00000001400330ED: 48 8D BC 24 F0 04  lea         rdi,[rsp+00000000000004F0h]
                    00 00
  00000001400330F5: 48 8B F0           mov         rsi,rax
  00000001400330F8: B9 10 00 00 00     mov         ecx,10h
  00000001400330FD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400330FF: 45 33 C0           xor         r8d,r8d
  0000000140033102: 33 D2              xor         edx,edx
  0000000140033104: 48 8D 8C 24 58 02  lea         rcx,[rsp+0000000000000258h]
                    00 00
  000000014003310C: FF 15 56 10 DD 01  call        qword ptr [0000000141E04168h]
  0000000140033112: 48 8D BC 24 E0 04  lea         rdi,[rsp+00000000000004E0h]
                    00 00
  000000014003311A: 48 8B F0           mov         rsi,rax
  000000014003311D: B9 10 00 00 00     mov         ecx,10h
  0000000140033122: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033124: 45 33 C0           xor         r8d,r8d
  0000000140033127: 33 D2              xor         edx,edx
  0000000140033129: 48 8D 8C 24 68 02  lea         rcx,[rsp+0000000000000268h]
                    00 00
  0000000140033131: FF 15 31 10 DD 01  call        qword ptr [0000000141E04168h]
  0000000140033137: 48 8D BC 24 D0 04  lea         rdi,[rsp+00000000000004D0h]
                    00 00
  000000014003313F: 48 8B F0           mov         rsi,rax
  0000000140033142: B9 10 00 00 00     mov         ecx,10h
  0000000140033147: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033149: 45 33 C0           xor         r8d,r8d
  000000014003314C: 33 D2              xor         edx,edx
  000000014003314E: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  0000000140033156: FF 15 0C 10 DD 01  call        qword ptr [0000000141E04168h]
  000000014003315C: 48 8D BC 24 C0 04  lea         rdi,[rsp+00000000000004C0h]
                    00 00
  0000000140033164: 48 8B F0           mov         rsi,rax
  0000000140033167: B9 10 00 00 00     mov         ecx,10h
  000000014003316C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014003316E: 48 8D BC 24 B0 04  lea         rdi,[rsp+00000000000004B0h]
                    00 00
  0000000140033176: 48 8D B4 24 88 02  lea         rsi,[rsp+0000000000000288h]
                    00 00
  000000014003317E: B9 10 00 00 00     mov         ecx,10h
  0000000140033183: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033185: 48 8D 84 24 40 05  lea         rax,[rsp+0000000000000540h]
                    00 00
  000000014003318D: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140033192: 48 8D 84 24 30 05  lea         rax,[rsp+0000000000000530h]
                    00 00
  000000014003319A: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014003319F: 48 8D 84 24 20 05  lea         rax,[rsp+0000000000000520h]
                    00 00
  00000001400331A7: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400331AC: 48 8D 84 24 10 05  lea         rax,[rsp+0000000000000510h]
                    00 00
  00000001400331B4: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400331B9: 48 8D 84 24 00 05  lea         rax,[rsp+0000000000000500h]
                    00 00
  00000001400331C1: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400331C6: 48 8D 84 24 F0 04  lea         rax,[rsp+00000000000004F0h]
                    00 00
  00000001400331CE: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400331D3: 48 8D 84 24 E0 04  lea         rax,[rsp+00000000000004E0h]
                    00 00
  00000001400331DB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400331E0: 48 8D 84 24 D0 04  lea         rax,[rsp+00000000000004D0h]
                    00 00
  00000001400331E8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400331ED: 4C 8D 8C 24 C0 04  lea         r9,[rsp+00000000000004C0h]
                    00 00
  00000001400331F5: 4C 8D 84 24 B0 04  lea         r8,[rsp+00000000000004B0h]
                    00 00
  00000001400331FD: 48 8D 15 24 1F 0D  lea         rdx,[0000000142105128h]
                    02
  0000000140033204: 48 8D 0D F5 34 D4  lea         rcx,[0000000143D76700h]
                    03
  000000014003320B: E8 B0 C1 29 00     call        00000001402CF3C0
  0000000140033210: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140033218: 48 89 84 24 70 03  mov         qword ptr [rsp+0000000000000370h],rax
                    00 00
  0000000140033220: 4C 8D 84 24 B8 00  lea         r8,[rsp+00000000000000B8h]
                    00 00
  0000000140033228: 48 8D 15 1D 1F 0D  lea         rdx,[000000014210514Ch]
                    02
  000000014003322F: 48 8D 8C 24 40 03  lea         rcx,[rsp+0000000000000340h]
                    00 00
  0000000140033237: E8 44 1E FF FF     call        0000000140025080
  000000014003323C: 48 8D BC 24 00 06  lea         rdi,[rsp+0000000000000600h]
                    00 00
  0000000140033244: 48 8B F0           mov         rsi,rax
  0000000140033247: B9 10 00 00 00     mov         ecx,10h
  000000014003324C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014003324E: 48 8D BC 24 30 03  lea         rdi,[rsp+0000000000000330h]
                    00 00
  0000000140033256: 48 8D B4 24 00 06  lea         rsi,[rsp+0000000000000600h]
                    00 00
  000000014003325E: B9 10 00 00 00     mov         ecx,10h
  0000000140033263: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033265: 4C 8D 84 24 70 03  lea         r8,[rsp+0000000000000370h]
                    00 00
  000000014003326D: 48 8D 15 E4 1E 0D  lea         rdx,[0000000142105158h]
                    02
  0000000140033274: 48 8D 8C 24 60 03  lea         rcx,[rsp+0000000000000360h]
                    00 00
  000000014003327C: E8 FF 1D FF FF     call        0000000140025080
  0000000140033281: 48 8D BC 24 F0 05  lea         rdi,[rsp+00000000000005F0h]
                    00 00
  0000000140033289: 48 8B F0           mov         rsi,rax
  000000014003328C: B9 10 00 00 00     mov         ecx,10h
  0000000140033291: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033293: 48 8D BC 24 50 03  lea         rdi,[rsp+0000000000000350h]
                    00 00
  000000014003329B: 48 8D B4 24 F0 05  lea         rsi,[rsp+00000000000005F0h]
                    00 00
  00000001400332A3: B9 10 00 00 00     mov         ecx,10h
  00000001400332A8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400332AA: 45 33 C0           xor         r8d,r8d
  00000001400332AD: 33 D2              xor         edx,edx
  00000001400332AF: 48 8D 8C 24 B0 02  lea         rcx,[rsp+00000000000002B0h]
                    00 00
  00000001400332B7: FF 15 AB 0E DD 01  call        qword ptr [0000000141E04168h]
  00000001400332BD: 48 8D BC 24 E0 05  lea         rdi,[rsp+00000000000005E0h]
                    00 00
  00000001400332C5: 48 8B F0           mov         rsi,rax
  00000001400332C8: B9 10 00 00 00     mov         ecx,10h
  00000001400332CD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400332CF: 45 33 C0           xor         r8d,r8d
  00000001400332D2: 33 D2              xor         edx,edx
  00000001400332D4: 48 8D 8C 24 C0 02  lea         rcx,[rsp+00000000000002C0h]
                    00 00
  00000001400332DC: FF 15 86 0E DD 01  call        qword ptr [0000000141E04168h]
  00000001400332E2: 48 8D BC 24 D0 05  lea         rdi,[rsp+00000000000005D0h]
                    00 00
  00000001400332EA: 48 8B F0           mov         rsi,rax
  00000001400332ED: B9 10 00 00 00     mov         ecx,10h
  00000001400332F2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400332F4: 45 33 C0           xor         r8d,r8d
  00000001400332F7: 33 D2              xor         edx,edx
  00000001400332F9: 48 8D 8C 24 D0 02  lea         rcx,[rsp+00000000000002D0h]
                    00 00
  0000000140033301: FF 15 61 0E DD 01  call        qword ptr [0000000141E04168h]
  0000000140033307: 48 8D BC 24 C0 05  lea         rdi,[rsp+00000000000005C0h]
                    00 00
  000000014003330F: 48 8B F0           mov         rsi,rax
  0000000140033312: B9 10 00 00 00     mov         ecx,10h
  0000000140033317: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033319: 45 33 C0           xor         r8d,r8d
  000000014003331C: 33 D2              xor         edx,edx
  000000014003331E: 48 8D 8C 24 E0 02  lea         rcx,[rsp+00000000000002E0h]
                    00 00
  0000000140033326: FF 15 3C 0E DD 01  call        qword ptr [0000000141E04168h]
  000000014003332C: 48 8D BC 24 B0 05  lea         rdi,[rsp+00000000000005B0h]
                    00 00
  0000000140033334: 48 8B F0           mov         rsi,rax
  0000000140033337: B9 10 00 00 00     mov         ecx,10h
  000000014003333C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014003333E: 45 33 C0           xor         r8d,r8d
  0000000140033341: 33 D2              xor         edx,edx
  0000000140033343: 48 8D 8C 24 F0 02  lea         rcx,[rsp+00000000000002F0h]
                    00 00
  000000014003334B: FF 15 17 0E DD 01  call        qword ptr [0000000141E04168h]
  0000000140033351: 48 8D BC 24 A0 05  lea         rdi,[rsp+00000000000005A0h]
                    00 00
  0000000140033359: 48 8B F0           mov         rsi,rax
  000000014003335C: B9 10 00 00 00     mov         ecx,10h
  0000000140033361: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033363: 45 33 C0           xor         r8d,r8d
  0000000140033366: 33 D2              xor         edx,edx
  0000000140033368: 48 8D 8C 24 00 03  lea         rcx,[rsp+0000000000000300h]
                    00 00
  0000000140033370: FF 15 F2 0D DD 01  call        qword ptr [0000000141E04168h]
  0000000140033376: 48 8D BC 24 90 05  lea         rdi,[rsp+0000000000000590h]
                    00 00
  000000014003337E: 48 8B F0           mov         rsi,rax
  0000000140033381: B9 10 00 00 00     mov         ecx,10h
  0000000140033386: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033388: 45 33 C0           xor         r8d,r8d
  000000014003338B: 33 D2              xor         edx,edx
  000000014003338D: 48 8D 8C 24 10 03  lea         rcx,[rsp+0000000000000310h]
                    00 00
  0000000140033395: FF 15 CD 0D DD 01  call        qword ptr [0000000141E04168h]
  000000014003339B: 48 8D BC 24 80 05  lea         rdi,[rsp+0000000000000580h]
                    00 00
  00000001400333A3: 48 8B F0           mov         rsi,rax
  00000001400333A6: B9 10 00 00 00     mov         ecx,10h
  00000001400333AB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400333AD: 45 33 C0           xor         r8d,r8d
  00000001400333B0: 33 D2              xor         edx,edx
  00000001400333B2: 48 8D 8C 24 20 03  lea         rcx,[rsp+0000000000000320h]
                    00 00
  00000001400333BA: FF 15 A8 0D DD 01  call        qword ptr [0000000141E04168h]
  00000001400333C0: 48 8D BC 24 70 05  lea         rdi,[rsp+0000000000000570h]
                    00 00
  00000001400333C8: 48 8B F0           mov         rsi,rax
  00000001400333CB: B9 10 00 00 00     mov         ecx,10h
  00000001400333D0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400333D2: 48 8D BC 24 60 05  lea         rdi,[rsp+0000000000000560h]
                    00 00
  00000001400333DA: 48 8D B4 24 30 03  lea         rsi,[rsp+0000000000000330h]
                    00 00
  00000001400333E2: B9 10 00 00 00     mov         ecx,10h
  00000001400333E7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400333E9: 48 8D BC 24 50 05  lea         rdi,[rsp+0000000000000550h]
                    00 00
  00000001400333F1: 48 8D B4 24 50 03  lea         rsi,[rsp+0000000000000350h]
                    00 00
  00000001400333F9: B9 10 00 00 00     mov         ecx,10h
  00000001400333FE: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140033400: 48 8D 84 24 E0 05  lea         rax,[rsp+00000000000005E0h]
                    00 00
  0000000140033408: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014003340D: 48 8D 84 24 D0 05  lea         rax,[rsp+00000000000005D0h]
                    00 00
  0000000140033415: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014003341A: 48 8D 84 24 C0 05  lea         rax,[rsp+00000000000005C0h]
                    00 00
  0000000140033422: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140033427: 48 8D 84 24 B0 05  lea         rax,[rsp+00000000000005B0h]
                    00 00
  000000014003342F: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140033434: 48 8D 84 24 A0 05  lea         rax,[rsp+00000000000005A0h]
                    00 00
  000000014003343C: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140033441: 48 8D 84 24 90 05  lea         rax,[rsp+0000000000000590h]
                    00 00
  0000000140033449: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014003344E: 48 8D 84 24 80 05  lea         rax,[rsp+0000000000000580h]
                    00 00
  0000000140033456: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014003345B: 48 8D 84 24 70 05  lea         rax,[rsp+0000000000000570h]
                    00 00
  0000000140033463: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140033468: 4C 8D 8C 24 60 05  lea         r9,[rsp+0000000000000560h]
                    00 00
  0000000140033470: 4C 8D 84 24 50 05  lea         r8,[rsp+0000000000000550h]
                    00 00
  0000000140033478: 48 8D 15 E9 1C 0D  lea         rdx,[0000000142105168h]
                    02
  000000014003347F: 48 8D 0D 7A 32 D4  lea         rcx,[0000000143D76700h]
                    03
  0000000140033486: E8 35 BF 29 00     call        00000001402CF3C0
  000000014003348B: 48 8B 94 24 80 00  mov         rdx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140033493: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003349B: E8 40 F0 FF FF     call        00000001400324E0
  00000001400334A0: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400334A8: 48 83 B8 E0 02 00  cmp         qword ptr [rax+00000000000002E0h],0
                    00 00
  00000001400334B0: 74 16              je          00000001400334C8
  00000001400334B2: 33 D2              xor         edx,edx
  00000001400334B4: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400334BC: 48 8B 89 E0 02 00  mov         rcx,qword ptr [rcx+00000000000002E0h]
                    00
  00000001400334C3: E8 E8 4A 0E 00     call        0000000140117FB0
  00000001400334C8: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400334D0: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400334D3: 33 D2              xor         edx,edx
  00000001400334D5: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400334DD: FF 90 D8 01 00 00  call        qword ptr [rax+00000000000001D8h]
  00000001400334E3: 48 8D 15 9E 1C 0D  lea         rdx,[0000000142105188h]
                    02
  00000001400334EA: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  00000001400334F2: FF 15 88 0F DD 01  call        qword ptr [0000000141E04480h]
  00000001400334F8: 90                 nop
  00000001400334F9: 48 8D 84 24 78 03  lea         rax,[rsp+0000000000000378h]
                    00 00
  0000000140033501: 48 89 84 24 80 03  mov         qword ptr [rsp+0000000000000380h],rax
                    00 00
  0000000140033509: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  0000000140033511: 48 8B 8C 24 80 03  mov         rcx,qword ptr [rsp+0000000000000380h]
                    00 00
  0000000140033519: FF 15 B9 24 DD 01  call        qword ptr [0000000141E059D8h]
  000000014003351F: 48 89 84 24 60 06  mov         qword ptr [rsp+0000000000000660h],rax
                    00 00
  0000000140033527: 48 8B 94 24 60 06  mov         rdx,qword ptr [rsp+0000000000000660h]
                    00 00
  000000014003352F: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140033537: E8 E4 3F C6 00     call        0000000140C97520
  000000014003353C: 48 8D 15 55 1C 0D  lea         rdx,[0000000142105198h]
                    02
  0000000140033543: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000014003354B: FF 15 2F 0F DD 01  call        qword ptr [0000000141E04480h]
  0000000140033551: 90                 nop
  0000000140033552: 48 8D 84 24 88 03  lea         rax,[rsp+0000000000000388h]
                    00 00
  000000014003355A: 48 89 84 24 90 03  mov         qword ptr [rsp+0000000000000390h],rax
                    00 00
  0000000140033562: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  000000014003356A: 48 8B 8C 24 90 03  mov         rcx,qword ptr [rsp+0000000000000390h]
                    00 00
  0000000140033572: FF 15 60 24 DD 01  call        qword ptr [0000000141E059D8h]
  0000000140033578: 48 89 84 24 68 06  mov         qword ptr [rsp+0000000000000668h],rax
                    00 00
  0000000140033580: 48 8B 94 24 68 06  mov         rdx,qword ptr [rsp+0000000000000668h]
                    00 00
  0000000140033588: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140033590: E8 8B 3F C6 00     call        0000000140C97520
  0000000140033595: 48 8D 15 14 1C 0D  lea         rdx,[00000001421051B0h]
                    02
  000000014003359C: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  00000001400335A4: FF 15 D6 0E DD 01  call        qword ptr [0000000141E04480h]
  00000001400335AA: 90                 nop
  00000001400335AB: 48 8D 84 24 98 03  lea         rax,[rsp+0000000000000398h]
                    00 00
  00000001400335B3: 48 89 84 24 A0 03  mov         qword ptr [rsp+00000000000003A0h],rax
                    00 00
  00000001400335BB: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  00000001400335C3: 48 8B 8C 24 A0 03  mov         rcx,qword ptr [rsp+00000000000003A0h]
                    00 00
  00000001400335CB: FF 15 07 24 DD 01  call        qword ptr [0000000141E059D8h]
  00000001400335D1: 48 89 84 24 70 06  mov         qword ptr [rsp+0000000000000670h],rax
                    00 00
  00000001400335D9: 48 8B 94 24 70 06  mov         rdx,qword ptr [rsp+0000000000000670h]
                    00 00
  00000001400335E1: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400335E9: E8 32 3F C6 00     call        0000000140C97520
  00000001400335EE: 48 8D 15 CB 1B 0D  lea         rdx,[00000001421051C0h]
                    02
  00000001400335F5: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  00000001400335FD: FF 15 7D 0E DD 01  call        qword ptr [0000000141E04480h]
  0000000140033603: 90                 nop
  0000000140033604: 48 8D 84 24 A8 03  lea         rax,[rsp+00000000000003A8h]
                    00 00
  000000014003360C: 48 89 84 24 B0 03  mov         qword ptr [rsp+00000000000003B0h],rax
                    00 00
  0000000140033614: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  000000014003361C: 48 8B 8C 24 B0 03  mov         rcx,qword ptr [rsp+00000000000003B0h]
                    00 00
  0000000140033624: FF 15 AE 23 DD 01  call        qword ptr [0000000141E059D8h]
  000000014003362A: 48 89 84 24 78 06  mov         qword ptr [rsp+0000000000000678h],rax
                    00 00
  0000000140033632: 48 8B 94 24 78 06  mov         rdx,qword ptr [rsp+0000000000000678h]
                    00 00
  000000014003363A: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140033642: E8 D9 3E C6 00     call        0000000140C97520
  0000000140033647: 48 8D 15 82 1B 0D  lea         rdx,[00000001421051D0h]
                    02
  000000014003364E: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140033656: FF 15 24 0E DD 01  call        qword ptr [0000000141E04480h]
  000000014003365C: 90                 nop
  000000014003365D: 48 8D 84 24 B8 03  lea         rax,[rsp+00000000000003B8h]
                    00 00
  0000000140033665: 48 89 84 24 C0 03  mov         qword ptr [rsp+00000000000003C0h],rax
                    00 00
  000000014003366D: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  0000000140033675: 48 8B 8C 24 C0 03  mov         rcx,qword ptr [rsp+00000000000003C0h]
                    00 00
  000000014003367D: FF 15 55 23 DD 01  call        qword ptr [0000000141E059D8h]
  0000000140033683: 48 89 84 24 80 06  mov         qword ptr [rsp+0000000000000680h],rax
                    00 00
  000000014003368B: 48 8B 94 24 80 06  mov         rdx,qword ptr [rsp+0000000000000680h]
                    00 00
  0000000140033693: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003369B: E8 80 3E C6 00     call        0000000140C97520
  00000001400336A0: 48 8D 15 39 1B 0D  lea         rdx,[00000001421051E0h]
                    02
  00000001400336A7: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400336AF: FF 15 CB 0D DD 01  call        qword ptr [0000000141E04480h]
  00000001400336B5: 90                 nop
  00000001400336B6: 48 8D 84 24 C8 03  lea         rax,[rsp+00000000000003C8h]
                    00 00
  00000001400336BE: 48 89 84 24 D0 03  mov         qword ptr [rsp+00000000000003D0h],rax
                    00 00
  00000001400336C6: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  00000001400336CE: 48 8B 8C 24 D0 03  mov         rcx,qword ptr [rsp+00000000000003D0h]
                    00 00
  00000001400336D6: FF 15 FC 22 DD 01  call        qword ptr [0000000141E059D8h]
  00000001400336DC: 48 89 84 24 88 06  mov         qword ptr [rsp+0000000000000688h],rax
                    00 00
  00000001400336E4: 48 8B 94 24 88 06  mov         rdx,qword ptr [rsp+0000000000000688h]
                    00 00
  00000001400336EC: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400336F4: E8 27 3E C6 00     call        0000000140C97520
  00000001400336F9: 48 8D 15 F0 1A 0D  lea         rdx,[00000001421051F0h]
                    02
  0000000140033700: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140033708: FF 15 72 0D DD 01  call        qword ptr [0000000141E04480h]
  000000014003370E: 90                 nop
  000000014003370F: 48 8D 84 24 D8 03  lea         rax,[rsp+00000000000003D8h]
                    00 00
  0000000140033717: 48 89 84 24 E0 03  mov         qword ptr [rsp+00000000000003E0h],rax
                    00 00
  000000014003371F: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  0000000140033727: 48 8B 8C 24 E0 03  mov         rcx,qword ptr [rsp+00000000000003E0h]
                    00 00
  000000014003372F: FF 15 A3 22 DD 01  call        qword ptr [0000000141E059D8h]
  0000000140033735: 48 89 84 24 90 06  mov         qword ptr [rsp+0000000000000690h],rax
                    00 00
  000000014003373D: 48 8B 94 24 90 06  mov         rdx,qword ptr [rsp+0000000000000690h]
                    00 00
  0000000140033745: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003374D: E8 CE 3D C6 00     call        0000000140C97520
  0000000140033752: 48 8D 15 A7 1A 0D  lea         rdx,[0000000142105200h]
                    02
  0000000140033759: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140033761: FF 15 19 0D DD 01  call        qword ptr [0000000141E04480h]
  0000000140033767: 90                 nop
  0000000140033768: 48 8D 84 24 E8 03  lea         rax,[rsp+00000000000003E8h]
                    00 00
  0000000140033770: 48 89 84 24 F0 03  mov         qword ptr [rsp+00000000000003F0h],rax
                    00 00
  0000000140033778: 48 8D 94 24 E8 00  lea         rdx,[rsp+00000000000000E8h]
                    00 00
  0000000140033780: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140033788: FF 15 4A 22 DD 01  call        qword ptr [0000000141E059D8h]
  000000014003378E: 48 89 84 24 98 06  mov         qword ptr [rsp+0000000000000698h],rax
                    00 00
  0000000140033796: 48 8B 94 24 98 06  mov         rdx,qword ptr [rsp+0000000000000698h]
                    00 00
  000000014003379E: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400337A6: E8 75 3D C6 00     call        0000000140C97520
  00000001400337AB: 48 8D 15 5E 1A 0D  lea         rdx,[0000000142105210h]
                    02
  00000001400337B2: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000001400337BA: FF 15 C0 0C DD 01  call        qword ptr [0000000141E04480h]
  00000001400337C0: 90                 nop
  00000001400337C1: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000001400337C9: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400337D1: E8 DA 1A FF FF     call        00000001400252B0
  00000001400337D6: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  00000001400337DE: 48 83 BC 24 E0 00  cmp         qword ptr [rsp+00000000000000E0h],0
                    00 00 00
  00000001400337E7: 74 4A              je          0000000140033833
  00000001400337E9: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400337F1: FF 15 61 80 DD 01  call        qword ptr [0000000141E0B858h]
  00000001400337F7: 0F B6 D0           movzx       edx,al
  00000001400337FA: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140033802: 88 90 A0 02 00 00  mov         byte ptr [rax+00000000000002A0h],dl
  0000000140033808: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140033810: 0F B6 80 A0 02 00  movzx       eax,byte ptr [rax+00000000000002A0h]
                    00
  0000000140033817: 85 C0              test        eax,eax
  0000000140033819: 74 18              je          0000000140033833
  000000014003381B: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140033823: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140033826: 33 D2              xor         edx,edx
  0000000140033828: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140033830: FF 50 58           call        qword ptr [rax+58h]
  0000000140033833: 33 D2              xor         edx,edx
  0000000140033835: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003383D: 48 8B 89 B0 01 00  mov         rcx,qword ptr [rcx+00000000000001B0h]
                    00
  0000000140033844: FF 15 CE 86 DD 01  call        qword ptr [0000000141E0BF18h]
  000000014003384A: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140033852: 48 83 B8 C0 01 00  cmp         qword ptr [rax+00000000000001C0h],0
                    00 00
  000000014003385A: 74 17              je          0000000140033873
  000000014003385C: 33 D2              xor         edx,edx
  000000014003385E: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140033866: 48 8B 89 C0 01 00  mov         rcx,qword ptr [rcx+00000000000001C0h]
                    00
  000000014003386D: FF 15 A5 86 DD 01  call        qword ptr [0000000141E0BF18h]
  0000000140033873: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003387B: 48 8B 89 18 04 00  mov         rcx,qword ptr [rcx+0000000000000418h]
                    00
  0000000140033882: FF 15 68 82 DD 01  call        qword ptr [0000000141E0BAF0h]
  0000000140033888: 33 D2              xor         edx,edx
  000000014003388A: 48 8B C8           mov         rcx,rax
  000000014003388D: FF 15 85 86 DD 01  call        qword ptr [0000000141E0BF18h]
  0000000140033893: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003389B: 48 8B 89 20 04 00  mov         rcx,qword ptr [rcx+0000000000000420h]
                    00
  00000001400338A2: FF 15 48 82 DD 01  call        qword ptr [0000000141E0BAF0h]
  00000001400338A8: 33 D2              xor         edx,edx
  00000001400338AA: 48 8B C8           mov         rcx,rax
  00000001400338AD: FF 15 65 86 DD 01  call        qword ptr [0000000141E0BF18h]
  00000001400338B3: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400338BB: 48 8B 89 D0 02 00  mov         rcx,qword ptr [rcx+00000000000002D0h]
                    00
  00000001400338C2: FF 15 28 82 DD 01  call        qword ptr [0000000141E0BAF0h]
  00000001400338C8: 33 D2              xor         edx,edx
  00000001400338CA: 48 8B C8           mov         rcx,rax
  00000001400338CD: FF 15 45 86 DD 01  call        qword ptr [0000000141E0BF18h]
  00000001400338D3: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400338DB: 48 8B 89 28 04 00  mov         rcx,qword ptr [rcx+0000000000000428h]
                    00
  00000001400338E2: FF 15 08 82 DD 01  call        qword ptr [0000000141E0BAF0h]
  00000001400338E8: 33 D2              xor         edx,edx
  00000001400338EA: 48 8B C8           mov         rcx,rax
  00000001400338ED: FF 15 25 86 DD 01  call        qword ptr [0000000141E0BF18h]
  00000001400338F3: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  00000001400338FB: 48 8B 89 30 04 00  mov         rcx,qword ptr [rcx+0000000000000430h]
                    00
  0000000140033902: FF 15 E8 81 DD 01  call        qword ptr [0000000141E0BAF0h]
  0000000140033908: 33 D2              xor         edx,edx
  000000014003390A: 48 8B C8           mov         rcx,rax
  000000014003390D: FF 15 05 86 DD 01  call        qword ptr [0000000141E0BF18h]
  0000000140033913: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003391B: 48 8B 89 38 04 00  mov         rcx,qword ptr [rcx+0000000000000438h]
                    00
  0000000140033922: FF 15 C8 81 DD 01  call        qword ptr [0000000141E0BAF0h]
  0000000140033928: 33 D2              xor         edx,edx
  000000014003392A: 48 8B C8           mov         rcx,rax
  000000014003392D: FF 15 E5 85 DD 01  call        qword ptr [0000000141E0BF18h]
  0000000140033933: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003393B: 48 8B 80 90 02 00  mov         rax,qword ptr [rax+0000000000000290h]
                    00
  0000000140033942: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  000000014003394A: 48 8B 89 90 02 00  mov         rcx,qword ptr [rcx+0000000000000290h]
                    00
  0000000140033951: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140033954: B2 01              mov         dl,1
  0000000140033956: FF 50 58           call        qword ptr [rax+58h]
  0000000140033959: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140033961: 48 8B 80 98 02 00  mov         rax,qword ptr [rax+0000000000000298h]
                    00
  0000000140033968: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140033970: 48 8B 89 98 02 00  mov         rcx,qword ptr [rcx+0000000000000298h]
                    00
  0000000140033977: 48 8B 00           mov         rax,qword ptr [rax]
  000000014003397A: B2 01              mov         dl,1
  000000014003397C: FF 50 58           call        qword ptr [rax+58h]
  000000014003397F: B2 01              mov         dl,1
  0000000140033981: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140033989: FF 15 81 7E DD 01  call        qword ptr [0000000141E0B810h]
  000000014003398F: 90                 nop
  0000000140033990: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140033998: FF 15 42 20 DD 01  call        qword ptr [0000000141E059E0h]
  000000014003399E: 90                 nop
  000000014003399F: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001400339A7: FF 15 33 20 DD 01  call        qword ptr [0000000141E059E0h]
  00000001400339AD: 90                 nop
  00000001400339AE: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400339B6: FF 15 24 20 DD 01  call        qword ptr [0000000141E059E0h]
  00000001400339BC: 90                 nop
  00000001400339BD: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400339C5: FF 15 15 20 DD 01  call        qword ptr [0000000141E059E0h]
  00000001400339CB: 90                 nop
  00000001400339CC: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000001400339D4: FF 15 06 20 DD 01  call        qword ptr [0000000141E059E0h]
  00000001400339DA: 90                 nop
  00000001400339DB: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  00000001400339E3: FF 15 F7 1F DD 01  call        qword ptr [0000000141E059E0h]
  00000001400339E9: 90                 nop
  00000001400339EA: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  00000001400339F2: FF 15 E8 1F DD 01  call        qword ptr [0000000141E059E0h]
  00000001400339F8: 90                 nop
  00000001400339F9: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140033A01: FF 15 D9 1F DD 01  call        qword ptr [0000000141E059E0h]
  0000000140033A07: 90                 nop
  0000000140033A08: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000140033A10: FF 15 CA 1F DD 01  call        qword ptr [0000000141E059E0h]
  0000000140033A16: 48 81 C4 A8 06 00  add         rsp,6A8h
                    00
  0000000140033A1D: 5F                 pop         rdi
  0000000140033A1E: 5E                 pop         rsi
  0000000140033A1F: C3                 ret

  Summary

     1E02000 .text
