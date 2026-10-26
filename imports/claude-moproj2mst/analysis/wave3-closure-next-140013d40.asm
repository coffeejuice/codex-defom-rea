Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140013D40: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140013D45: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140013D49: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140013D4E: 56                 push        rsi
  0000000140013D4F: 57                 push        rdi
  0000000140013D50: 48 81 EC D8 01 00  sub         rsp,1D8h
                    00
  0000000140013D57: 0F B6 84 24 00 02  movzx       eax,byte ptr [rsp+0000000000000200h]
                    00 00
  0000000140013D5F: 85 C0              test        eax,eax
  0000000140013D61: 74 1C              je          0000000140013D7F
  0000000140013D63: B2 01              mov         dl,1
  0000000140013D65: 48 8D 0D 4C 29 D6  lea         rcx,[0000000143D766B8h]
                    03
  0000000140013D6C: E8 AF 6B 2B 00     call        00000001402CA920
  0000000140013D71: 0F B6 C0           movzx       eax,al
  0000000140013D74: 85 C0              test        eax,eax
  0000000140013D76: 74 07              je          0000000140013D7F
  0000000140013D78: 32 C0              xor         al,al
  0000000140013D7A: E9 90 03 00 00     jmp         000000014001410F
  0000000140013D7F: 48 8D 0D 32 29 D6  lea         rcx,[0000000143D766B8h]
                    03
  0000000140013D86: E8 15 7A 2B 00     call        00000001402CB7A0
  0000000140013D8B: 89 84 24 C0 01 00  mov         dword ptr [rsp+00000000000001C0h],eax
                    00
  0000000140013D92: 83 BC 24 C0 01 00  cmp         dword ptr [rsp+00000000000001C0h],1
                    00 01
  0000000140013D9A: 74 2F              je          0000000140013DCB
  0000000140013D9C: 83 BC 24 C0 01 00  cmp         dword ptr [rsp+00000000000001C0h],3
                    00 03
  0000000140013DA4: 74 43              je          0000000140013DE9
  0000000140013DA6: 83 BC 24 C0 01 00  cmp         dword ptr [rsp+00000000000001C0h],4
                    00 04
  0000000140013DAE: 74 2A              je          0000000140013DDA
  0000000140013DB0: 83 BC 24 C0 01 00  cmp         dword ptr [rsp+00000000000001C0h],5
                    00 05
  0000000140013DB8: 74 02              je          0000000140013DBC
  0000000140013DBA: EB 3A              jmp         0000000140013DF6
  0000000140013DBC: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140013DC4: E8 E7 70 02 00     call        000000014003AEB0
  0000000140013DC9: EB 2B              jmp         0000000140013DF6
  0000000140013DCB: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140013DD3: E8 58 74 02 00     call        000000014003B230
  0000000140013DD8: EB 1C              jmp         0000000140013DF6
  0000000140013DDA: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140013DE2: E8 A9 77 02 00     call        000000014003B590
  0000000140013DE7: EB 0D              jmp         0000000140013DF6
  0000000140013DE9: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140013DF1: E8 2A 7B 02 00     call        000000014003B920
  0000000140013DF6: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140013DFE: E8 9D F5 FF FF     call        00000001400133A0
  0000000140013E03: 48 8B C8           mov         rcx,rax
  0000000140013E06: E8 75 19 03 00     call        0000000140045780
  0000000140013E0B: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140013E10: 8B 84 24 F8 01 00  mov         eax,dword ptr [rsp+00000000000001F8h]
                    00
  0000000140013E17: 89 84 24 C4 01 00  mov         dword ptr [rsp+00000000000001C4h],eax
                    00
  0000000140013E1E: 83 BC 24 C4 01 00  cmp         dword ptr [rsp+00000000000001C4h],0
                    00 00
  0000000140013E26: 74 1D              je          0000000140013E45
  0000000140013E28: 83 BC 24 C4 01 00  cmp         dword ptr [rsp+00000000000001C4h],1
                    00 01
  0000000140013E30: 74 7D              je          0000000140013EAF
  0000000140013E32: 83 BC 24 C4 01 00  cmp         dword ptr [rsp+00000000000001C4h],2
                    00 02
  0000000140013E3A: 0F 84 8A 00 00 00  je          0000000140013ECA
  0000000140013E40: E9 9E 00 00 00     jmp         0000000140013EE3
  0000000140013E45: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140013E4B: 75 1B              jne         0000000140013E68
  0000000140013E4D: C7 44 24 68 05 00  mov         dword ptr [rsp+68h],5
                    00 00
  0000000140013E55: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  0000000140013E5A: 48 8D 0D 57 28 D6  lea         rcx,[0000000143D766B8h]
                    03
  0000000140013E61: E8 1A 79 2B 00     call        00000001402CB780
  0000000140013E66: EB 45              jmp         0000000140013EAD
  0000000140013E68: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140013E6D: E8 1E 02 09 00     call        00000001400A4090
  0000000140013E72: 0F B6 C0           movzx       eax,al
  0000000140013E75: 85 C0              test        eax,eax
  0000000140013E77: 74 1B              je          0000000140013E94
  0000000140013E79: C7 44 24 6C 05 00  mov         dword ptr [rsp+6Ch],5
                    00 00
  0000000140013E81: 48 8D 54 24 6C     lea         rdx,[rsp+6Ch]
  0000000140013E86: 48 8D 0D 2B 28 D6  lea         rcx,[0000000143D766B8h]
                    03
  0000000140013E8D: E8 EE 78 2B 00     call        00000001402CB780
  0000000140013E92: EB 19              jmp         0000000140013EAD
  0000000140013E94: C7 44 24 70 01 00  mov         dword ptr [rsp+70h],1
                    00 00
  0000000140013E9C: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140013EA1: 48 8D 0D 10 28 D6  lea         rcx,[0000000143D766B8h]
                    03
  0000000140013EA8: E8 D3 78 2B 00     call        00000001402CB780
  0000000140013EAD: EB 34              jmp         0000000140013EE3
  0000000140013EAF: C7 44 24 74 04 00  mov         dword ptr [rsp+74h],4
                    00 00
  0000000140013EB7: 48 8D 54 24 74     lea         rdx,[rsp+74h]
  0000000140013EBC: 48 8D 0D F5 27 D6  lea         rcx,[0000000143D766B8h]
                    03
  0000000140013EC3: E8 B8 78 2B 00     call        00000001402CB780
  0000000140013EC8: EB 19              jmp         0000000140013EE3
  0000000140013ECA: C7 44 24 78 03 00  mov         dword ptr [rsp+78h],3
                    00 00
  0000000140013ED2: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140013ED7: 48 8D 0D DA 27 D6  lea         rcx,[0000000143D766B8h]
                    03
  0000000140013EDE: E8 9D 78 2B 00     call        00000001402CB780
  0000000140013EE3: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140013EEB: 48 83 B8 E0 02 00  cmp         qword ptr [rax+00000000000002E0h],0
                    00 00
  0000000140013EF3: 74 1B              je          0000000140013F10
  0000000140013EF5: 8B 94 24 F8 01 00  mov         edx,dword ptr [rsp+00000000000001F8h]
                    00
  0000000140013EFC: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140013F04: 48 8B 89 E0 02 00  mov         rcx,qword ptr [rcx+00000000000002E0h]
                    00
  0000000140013F0B: E8 C0 2F 10 00     call        0000000140116ED0
  0000000140013F10: 45 33 C0           xor         r8d,r8d
  0000000140013F13: 33 D2              xor         edx,edx
  0000000140013F15: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140013F1D: FF 15 45 02 DF 01  call        qword ptr [0000000141E04168h]
  0000000140013F23: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  0000000140013F2B: 48 8B F0           mov         rsi,rax
  0000000140013F2E: B9 10 00 00 00     mov         ecx,10h
  0000000140013F33: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140013F35: 45 33 C0           xor         r8d,r8d
  0000000140013F38: 33 D2              xor         edx,edx
  0000000140013F3A: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140013F42: FF 15 20 02 DF 01  call        qword ptr [0000000141E04168h]
  0000000140013F48: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  0000000140013F50: 48 8B F0           mov         rsi,rax
  0000000140013F53: B9 10 00 00 00     mov         ecx,10h
  0000000140013F58: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140013F5A: 45 33 C0           xor         r8d,r8d
  0000000140013F5D: 33 D2              xor         edx,edx
  0000000140013F5F: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140013F67: FF 15 FB 01 DF 01  call        qword ptr [0000000141E04168h]
  0000000140013F6D: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140013F75: 48 8B F0           mov         rsi,rax
  0000000140013F78: B9 10 00 00 00     mov         ecx,10h
  0000000140013F7D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140013F7F: 45 33 C0           xor         r8d,r8d
  0000000140013F82: 33 D2              xor         edx,edx
  0000000140013F84: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140013F8C: FF 15 D6 01 DF 01  call        qword ptr [0000000141E04168h]
  0000000140013F92: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140013F9A: 48 8B F0           mov         rsi,rax
  0000000140013F9D: B9 10 00 00 00     mov         ecx,10h
  0000000140013FA2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140013FA4: 45 33 C0           xor         r8d,r8d
  0000000140013FA7: 33 D2              xor         edx,edx
  0000000140013FA9: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140013FB1: FF 15 B1 01 DF 01  call        qword ptr [0000000141E04168h]
  0000000140013FB7: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140013FBF: 48 8B F0           mov         rsi,rax
  0000000140013FC2: B9 10 00 00 00     mov         ecx,10h
  0000000140013FC7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140013FC9: 45 33 C0           xor         r8d,r8d
  0000000140013FCC: 33 D2              xor         edx,edx
  0000000140013FCE: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140013FD6: FF 15 8C 01 DF 01  call        qword ptr [0000000141E04168h]
  0000000140013FDC: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  0000000140013FE4: 48 8B F0           mov         rsi,rax
  0000000140013FE7: B9 10 00 00 00     mov         ecx,10h
  0000000140013FEC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140013FEE: 45 33 C0           xor         r8d,r8d
  0000000140013FF1: 33 D2              xor         edx,edx
  0000000140013FF3: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140013FFB: FF 15 67 01 DF 01  call        qword ptr [0000000141E04168h]
  0000000140014001: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  0000000140014009: 48 8B F0           mov         rsi,rax
  000000014001400C: B9 10 00 00 00     mov         ecx,10h
  0000000140014011: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140014013: 45 33 C0           xor         r8d,r8d
  0000000140014016: 33 D2              xor         edx,edx
  0000000140014018: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000140014020: FF 15 42 01 DF 01  call        qword ptr [0000000141E04168h]
  0000000140014026: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  000000014001402E: 48 8B F0           mov         rsi,rax
  0000000140014031: B9 10 00 00 00     mov         ecx,10h
  0000000140014036: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140014038: 45 33 C0           xor         r8d,r8d
  000000014001403B: 33 D2              xor         edx,edx
  000000014001403D: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140014045: FF 15 1D 01 DF 01  call        qword ptr [0000000141E04168h]
  000000014001404B: 48 8D BC 24 30 01  lea         rdi,[rsp+0000000000000130h]
                    00 00
  0000000140014053: 48 8B F0           mov         rsi,rax
  0000000140014056: B9 10 00 00 00     mov         ecx,10h
  000000014001405B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001405D: 45 33 C0           xor         r8d,r8d
  0000000140014060: 33 D2              xor         edx,edx
  0000000140014062: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000014001406A: FF 15 F8 00 DF 01  call        qword ptr [0000000141E04168h]
  0000000140014070: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  0000000140014078: 48 8B F0           mov         rsi,rax
  000000014001407B: B9 10 00 00 00     mov         ecx,10h
  0000000140014080: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140014082: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  000000014001408A: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014001408F: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140014097: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014001409C: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  00000001400140A4: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400140A9: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  00000001400140B1: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400140B6: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  00000001400140BE: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400140C3: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  00000001400140CB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400140D0: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  00000001400140D8: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400140DD: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  00000001400140E5: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400140EA: 4C 8D 8C 24 30 01  lea         r9,[rsp+0000000000000130h]
                    00 00
  00000001400140F2: 4C 8D 84 24 20 01  lea         r8,[rsp+0000000000000120h]
                    00 00
  00000001400140FA: 48 8D 15 DF 5A 0E  lea         rdx,[00000001420F9BE0h]
                    02
  0000000140014101: 48 8D 0D F8 25 D6  lea         rcx,[0000000143D76700h]
                    03
  0000000140014108: E8 B3 B2 2B 00     call        00000001402CF3C0
  000000014001410D: B0 01              mov         al,1
  000000014001410F: 48 81 C4 D8 01 00  add         rsp,1D8h
                    00
  0000000140014116: 5F                 pop         rdi
  0000000140014117: 5E                 pop         rsi
  0000000140014118: C3                 ret

  Summary

     1E02000 .text
