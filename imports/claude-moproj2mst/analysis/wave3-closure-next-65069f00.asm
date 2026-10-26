Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065069F00: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000065069F04: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065069F09: 53                 push        rbx
  0000000065069F0A: 55                 push        rbp
  0000000065069F0B: 56                 push        rsi
  0000000065069F0C: 57                 push        rdi
  0000000065069F0D: 48 83 EC 58        sub         rsp,58h
  0000000065069F11: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065069F1A: 0F B6 FA           movzx       edi,dl
  0000000065069F1D: 48 8B E9           mov         rbp,rcx
  0000000065069F20: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  0000000065069F24: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  0000000065069F29: 84 D2              test        dl,dl
  0000000065069F2B: 74 33              je          0000000065069F60
  0000000065069F2D: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  0000000065069F31: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  0000000065069F35: A8 01              test        al,1
  0000000065069F37: 75 27              jne         0000000065069F60
  0000000065069F39: 48 8B CE           mov         rcx,rsi
  0000000065069F3C: FF 15 26 C9 62 00  call        qword ptr [0000000065696868h]
  0000000065069F42: 48 85 C0           test        rax,rax
  0000000065069F45: 74 19              je          0000000065069F60
  0000000065069F47: 48 8B CE           mov         rcx,rsi
  0000000065069F4A: FF 15 18 C9 62 00  call        qword ptr [0000000065696868h]
  0000000065069F50: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  0000000065069F54: 0F B6 41 08        movzx       eax,byte ptr [rcx+8]
  0000000065069F58: 24 01              and         al,1
  0000000065069F5A: 0F 85 B5 02 00 00  jne         000000006506A215
  0000000065069F60: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  0000000065069F64: 0F B6 48 08        movzx       ecx,byte ptr [rax+8]
  0000000065069F68: 80 E1 01           and         cl,1
  0000000065069F6B: 40 3A F9           cmp         dil,cl
  0000000065069F6E: 0F 85 A1 02 00 00  jne         000000006506A215
  0000000065069F74: 40 84 FF           test        dil,dil
  0000000065069F77: 0F 94 C0           sete        al
  0000000065069F7A: 88 84 24 90 00 00  mov         byte ptr [rsp+0000000000000090h],al
                    00
  0000000065069F81: 44 0F B6 C0        movzx       r8d,al
  0000000065069F85: 33 D2              xor         edx,edx
  0000000065069F87: 48 8B CE           mov         rcx,rsi
  0000000065069F8A: E8 51 F0 FF FF     call        0000000065068FE0
  0000000065069F8F: 48 8B CD           mov         rcx,rbp
  0000000065069F92: E8 19 53 03 00     call        000000006509F2B0
  0000000065069F97: 40 84 FF           test        dil,dil
  0000000065069F9A: 0F 85 8A 00 00 00  jne         000000006506A02A
  0000000065069FA0: 48 8B DE           mov         rbx,rsi
  0000000065069FA3: 48 8B CE           mov         rcx,rsi
  0000000065069FA6: FF 15 BC C8 62 00  call        qword ptr [0000000065696868h]
  0000000065069FAC: 4C 8B D8           mov         r11,rax
  0000000065069FAF: 48 8B 4E 20        mov         rcx,qword ptr [rsi+20h]
  0000000065069FB3: 0F B6 49 0C        movzx       ecx,byte ptr [rcx+0Ch]
  0000000065069FB7: F6 C1 01           test        cl,1
  0000000065069FBA: 75 24              jne         0000000065069FE0
  0000000065069FBC: 0F 1F 40 00        nop         dword ptr [rax]
  0000000065069FC0: 4D 85 DB           test        r11,r11
  0000000065069FC3: 74 1B              je          0000000065069FE0
  0000000065069FC5: 49 8B DB           mov         rbx,r11
  0000000065069FC8: 49 8B CB           mov         rcx,r11
  0000000065069FCB: FF 15 97 C8 62 00  call        qword ptr [0000000065696868h]
  0000000065069FD1: 4C 8B D8           mov         r11,rax
  0000000065069FD4: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  0000000065069FD8: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  0000000065069FDC: A8 01              test        al,1
  0000000065069FDE: 74 E0              je          0000000065069FC0
  0000000065069FE0: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  0000000065069FE4: 48 39 B0 A0 00 00  cmp         qword ptr [rax+00000000000000A0h],rsi
                    00
  0000000065069FEB: 75 3D              jne         000000006506A02A
  0000000065069FED: 48 8B CE           mov         rcx,rsi
  0000000065069FF0: FF 15 72 C8 62 00  call        qword ptr [0000000065696868h]
  0000000065069FF6: 48 85 C0           test        rax,rax
  0000000065069FF9: 74 15              je          000000006506A010
  0000000065069FFB: 48 8B CE           mov         rcx,rsi
  0000000065069FFE: FF 15 64 C8 62 00  call        qword ptr [0000000065696868h]
  000000006506A004: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  000000006506A008: 0F B6 41 08        movzx       eax,byte ptr [rcx+8]
  000000006506A00C: 24 01              and         al,1
  000000006506A00E: 75 12              jne         000000006506A022
  000000006506A010: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006506A013: B2 01              mov         dl,1
  000000006506A015: 48 8B CE           mov         rcx,rsi
  000000006506A018: FF 90 58 01 00 00  call        qword ptr [rax+0000000000000158h]
  000000006506A01E: 84 C0              test        al,al
  000000006506A020: 75 08              jne         000000006506A02A
  000000006506A022: 48 8B CE           mov         rcx,rsi
  000000006506A025: E8 46 5B FF FF     call        000000006505FB70
  000000006506A02A: 40 0F B6 C7        movzx       eax,dil
  000000006506A02E: F6 D8              neg         al
  000000006506A030: 1B ED              sbb         ebp,ebp
  000000006506A032: 83 E5 20           and         ebp,20h
  000000006506A035: 33 FF              xor         edi,edi
  000000006506A037: 48 8B 9C 24 80 00  mov         rbx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006506A03F: 48 8D 4B 18        lea         rcx,[rbx+18h]
  000000006506A043: 48 89 8C 24 98 00  mov         qword ptr [rsp+0000000000000098h],rcx
                    00 00
  000000006506A04B: FF 15 67 C8 62 00  call        qword ptr [00000000656968B8h]
  000000006506A051: 85 C0              test        eax,eax
  000000006506A053: 0F 8E A2 00 00 00  jle         000000006506A0FB
  000000006506A059: 48 8B B4 24 98 00  mov         rsi,qword ptr [rsp+0000000000000098h]
                    00 00
  000000006506A061: 8B D7              mov         edx,edi
  000000006506A063: 48 8B CE           mov         rcx,rsi
  000000006506A066: FF 15 D4 C8 62 00  call        qword ptr [0000000065696940h]
  000000006506A06C: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006506A06F: 48 85 DB           test        rbx,rbx
  000000006506A072: 74 67              je          000000006506A0DB
  000000006506A074: 48 8B CB           mov         rcx,rbx
  000000006506A077: FF 15 DB C7 62 00  call        qword ptr [0000000065696858h]
  000000006506A07D: 84 C0              test        al,al
  000000006506A07F: 74 5A              je          000000006506A0DB
  000000006506A081: 83 FD 20           cmp         ebp,20h
  000000006506A084: 7D 0F              jge         000000006506A095
  000000006506A086: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006506A08A: 8B 40 08           mov         eax,dword ptr [rax+8]
  000000006506A08D: 0F A3 E8           bt          eax,ebp
  000000006506A090: 0F 92 C0           setb        al
  000000006506A093: EB 31              jmp         000000006506A0C6
  000000006506A095: 4C 8B 4B 08        mov         r9,qword ptr [rbx+8]
  000000006506A099: 8D 45 E0           lea         eax,[rbp-20h]
  000000006506A09C: 48 98              cdqe
  000000006506A09E: 4C 8B C0           mov         r8,rax
  000000006506A0A1: 49 C1 E8 05        shr         r8,5
  000000006506A0A5: 42 8D 0C C5 00 00  lea         ecx,[r8*8+0000000000000000h]
                    00 00
  000000006506A0AD: C0 E1 02           shl         cl,2
  000000006506A0B0: 2A C1              sub         al,cl
  000000006506A0B2: 0F B6 D0           movzx       edx,al
  000000006506A0B5: 49 63 C0           movsxd      rax,r8d
  000000006506A0B8: 41 8B 8C 81 C8 01  mov         ecx,dword ptr [r9+rax*4+00000000000001C8h]
                    00 00
  000000006506A0C0: 0F A3 D1           bt          ecx,edx
  000000006506A0C3: 0F 92 C0           setb        al
  000000006506A0C6: 84 C0              test        al,al
  000000006506A0C8: 75 11              jne         000000006506A0DB
  000000006506A0CA: 0F B6 94 24 88 00  movzx       edx,byte ptr [rsp+0000000000000088h]
                    00 00
  000000006506A0D2: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006506A0D6: E8 25 FE FF FF     call        0000000065069F00
  000000006506A0DB: FF C7              inc         edi
  000000006506A0DD: 48 8B CE           mov         rcx,rsi
  000000006506A0E0: FF 15 D2 C7 62 00  call        qword ptr [00000000656968B8h]
  000000006506A0E6: 3B F8              cmp         edi,eax
  000000006506A0E8: 0F 8C 73 FF FF FF  jl          000000006506A061
  000000006506A0EE: 48 8B 74 24 20     mov         rsi,qword ptr [rsp+20h]
  000000006506A0F3: 48 8B 9C 24 80 00  mov         rbx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006506A0FB: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006506A0FF: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006506A102: C1 E9 0E           shr         ecx,0Eh
  000000006506A105: 80 E1 01           and         cl,1
  000000006506A108: 0F 84 C7 00 00 00  je          000000006506A1D5
  000000006506A10E: 48 8B CE           mov         rcx,rsi
  000000006506A111: E8 8A 59 FF FF     call        000000006505FAA0
  000000006506A116: 84 C0              test        al,al
  000000006506A118: 0F 84 B7 00 00 00  je          000000006506A1D5
  000000006506A11E: 48 8B 5B 08        mov         rbx,qword ptr [rbx+8]
  000000006506A122: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    0F 1F 84 00 00 00
                    00 00
  000000006506A130: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006506A134: 48 8B 81 88 00 00  mov         rax,qword ptr [rcx+0000000000000088h]
                    00
  000000006506A13B: 48 85 C0           test        rax,rax
  000000006506A13E: 74 16              je          000000006506A156
  000000006506A140: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  000000006506A145: 74 0F              je          000000006506A156
  000000006506A147: 48 85 C0           test        rax,rax
  000000006506A14A: 74 06              je          000000006506A152
  000000006506A14C: 48 8B 58 28        mov         rbx,qword ptr [rax+28h]
  000000006506A150: EB DE              jmp         000000006506A130
  000000006506A152: 33 DB              xor         ebx,ebx
  000000006506A154: EB DA              jmp         000000006506A130
  000000006506A156: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006506A15A: 48 85 C9           test        rcx,rcx
  000000006506A15D: 74 1F              je          000000006506A17E
  000000006506A15F: 90                 nop
  000000006506A160: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006506A164: 48 8B B8 E0 00 00  mov         rdi,qword ptr [rax+00000000000000E0h]
                    00
  000000006506A16B: 48 85 FF           test        rdi,rdi
  000000006506A16E: 75 1F              jne         000000006506A18F
  000000006506A170: FF 15 F2 C6 62 00  call        qword ptr [0000000065696868h]
  000000006506A176: 48 8B C8           mov         rcx,rax
  000000006506A179: 48 85 C0           test        rax,rax
  000000006506A17C: 75 E2              jne         000000006506A160
  000000006506A17E: FF 15 9C C6 62 00  call        qword ptr [0000000065696820h]
  000000006506A184: 48 8B C8           mov         rcx,rax
  000000006506A187: E8 A4 FC FA FF     call        0000000065019E30
  000000006506A18C: 48 8B F8           mov         rdi,rax
  000000006506A18F: 80 BC 24 88 00 00  cmp         byte ptr [rsp+0000000000000088h],0
                    00 00
  000000006506A197: 74 24              je          000000006506A1BD
  000000006506A199: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  000000006506A19D: 44 8B 41 08        mov         r8d,dword ptr [rcx+8]
  000000006506A1A1: 41 C1 E8 0E        shr         r8d,0Eh
  000000006506A1A5: 41 80 E0 01        and         r8b,1
  000000006506A1A9: 74 2A              je          000000006506A1D5
  000000006506A1AB: 4C 8B 07           mov         r8,qword ptr [rdi]
  000000006506A1AE: 48 8B D3           mov         rdx,rbx
  000000006506A1B1: 48 8B CF           mov         rcx,rdi
  000000006506A1B4: 41 FF 90 90 00 00  call        qword ptr [r8+0000000000000090h]
                    00
  000000006506A1BB: EB 18              jmp         000000006506A1D5
  000000006506A1BD: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506A1C0: 48 8B CF           mov         rcx,rdi
  000000006506A1C3: FF 50 68           call        qword ptr [rax+68h]
  000000006506A1C6: 4C 8B 1F           mov         r11,qword ptr [rdi]
  000000006506A1C9: 33 D2              xor         edx,edx
  000000006506A1CB: 48 8B CF           mov         rcx,rdi
  000000006506A1CE: 41 FF 93 90 00 00  call        qword ptr [r11+0000000000000090h]
                    00
  000000006506A1D5: BA 62 00 00 00     mov         edx,62h
  000000006506A1DA: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006506A1DF: FF 15 33 C6 62 00  call        qword ptr [0000000065696818h]
  000000006506A1E5: 90                 nop
  000000006506A1E6: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006506A1EB: 48 8B CE           mov         rcx,rsi
  000000006506A1EE: FF 15 1C C6 62 00  call        qword ptr [0000000065696810h]
  000000006506A1F4: 4C 8B 1E           mov         r11,qword ptr [rsi]
  000000006506A1F7: 0F B6 94 24 90 00  movzx       edx,byte ptr [rsp+0000000000000090h]
                    00 00
  000000006506A1FF: 48 8B CE           mov         rcx,rsi
  000000006506A202: 41 FF 93 68 01 00  call        qword ptr [r11+0000000000000168h]
                    00
  000000006506A209: 90                 nop
  000000006506A20A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006506A20F: FF 15 F3 C5 62 00  call        qword ptr [0000000065696808h]
  000000006506A215: 48 83 C4 58        add         rsp,58h
  000000006506A219: 5F                 pop         rdi
  000000006506A21A: 5E                 pop         rsi
  000000006506A21B: 5D                 pop         rbp
  000000006506A21C: 5B                 pop         rbx
  000000006506A21D: C3                 ret

  Summary

      695000 .text
