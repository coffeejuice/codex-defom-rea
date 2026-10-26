Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C0D7E0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C0D7E5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C0D7EA: 56                 push        rsi
  0000000140C0D7EB: 57                 push        rdi
  0000000140C0D7EC: 48 81 EC 58 02 00  sub         rsp,258h
                    00
  0000000140C0D7F3: 48 C7 84 24 10 02  mov         qword ptr [rsp+0000000000000210h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140C0D7FF: C6 44 24 60 00     mov         byte ptr [rsp+60h],0
  0000000140C0D804: 48 8D 0D AD 8E 16  lea         rcx,[0000000143D766B8h]
                    03
  0000000140C0D80B: E8 80 A8 6B FF     call        00000001402C8090
  0000000140C0D810: 48 89 84 24 18 02  mov         qword ptr [rsp+0000000000000218h],rax
                    00 00
  0000000140C0D818: 48 8B 84 24 18 02  mov         rax,qword ptr [rsp+0000000000000218h]
                    00 00
  0000000140C0D820: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C0D823: 48 8B 8C 24 18 02  mov         rcx,qword ptr [rsp+0000000000000218h]
                    00 00
  0000000140C0D82B: FF 90 88 07 00 00  call        qword ptr [rax+0000000000000788h]
  0000000140C0D831: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140C0D836: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140C0D83B: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140C0D840: FF 15 62 A4 1F 01  call        qword ptr [0000000141E07CA8h]
  0000000140C0D846: 90                 nop
  0000000140C0D847: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140C0D84C: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140C0D851: E8 9A 02 46 FF     call        000000014006DAF0
  0000000140C0D856: 90                 nop
  0000000140C0D857: 48 8D 44 24 78     lea         rax,[rsp+78h]
  0000000140C0D85C: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140C0D864: EB 48              jmp         0000000140C0D8AE
  0000000140C0D866: 33 C0              xor         eax,eax
  0000000140C0D868: 83 F8 01           cmp         eax,1
  0000000140C0D86B: 74 0E              je          0000000140C0D87B
  0000000140C0D86D: 48 C7 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],0
                    00 00 00 00 00 00
  0000000140C0D879: EB 12              jmp         0000000140C0D88D
  0000000140C0D87B: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140C0D880: E8 CB 73 57 FF     call        0000000140184C50
  0000000140C0D885: 48 89 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],rax
                    00 00
  0000000140C0D88D: 48 8B 94 24 20 02  mov         rdx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140C0D895: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140C0D89D: E8 DE EA 1E 00     call        0000000140DFC380
  0000000140C0D8A2: 48 8B C8           mov         rcx,rax
  0000000140C0D8A5: 48 83 C1 10        add         rcx,10h
  0000000140C0D8A9: E8 52 BF 1D 00     call        0000000140DE9800
  0000000140C0D8AE: 33 C0              xor         eax,eax
  0000000140C0D8B0: 83 F8 01           cmp         eax,1
  0000000140C0D8B3: 74 0E              je          0000000140C0D8C3
  0000000140C0D8B5: 48 C7 84 24 28 02  mov         qword ptr [rsp+0000000000000228h],0
                    00 00 00 00 00 00
  0000000140C0D8C1: EB 12              jmp         0000000140C0D8D5
  0000000140C0D8C3: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140C0D8C8: E8 83 73 57 FF     call        0000000140184C50
  0000000140C0D8CD: 48 89 84 24 28 02  mov         qword ptr [rsp+0000000000000228h],rax
                    00 00
  0000000140C0D8D5: 48 8B 94 24 28 02  mov         rdx,qword ptr [rsp+0000000000000228h]
                    00 00
  0000000140C0D8DD: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140C0D8E5: E8 96 EA 1E 00     call        0000000140DFC380
  0000000140C0D8EA: 48 8B C8           mov         rcx,rax
  0000000140C0D8ED: E8 8E E7 1E 00     call        0000000140DFC080
  0000000140C0D8F2: 0F B6 C0           movzx       eax,al
  0000000140C0D8F5: 85 C0              test        eax,eax
  0000000140C0D8F7: 0F 84 DD 02 00 00  je          0000000140C0DBDA
  0000000140C0D8FD: 33 C0              xor         eax,eax
  0000000140C0D8FF: 83 F8 01           cmp         eax,1
  0000000140C0D902: 74 0E              je          0000000140C0D912
  0000000140C0D904: 48 C7 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],0
                    00 00 00 00 00 00
  0000000140C0D910: EB 12              jmp         0000000140C0D924
  0000000140C0D912: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140C0D917: E8 34 73 57 FF     call        0000000140184C50
  0000000140C0D91C: 48 89 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],rax
                    00 00
  0000000140C0D924: 48 8B 94 24 30 02  mov         rdx,qword ptr [rsp+0000000000000230h]
                    00 00
  0000000140C0D92C: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140C0D934: E8 47 EA 1E 00     call        0000000140DFC380
  0000000140C0D939: 48 8B C8           mov         rcx,rax
  0000000140C0D93C: 48 83 C1 10        add         rcx,10h
  0000000140C0D940: E8 BB 1A A4 FF     call        000000014064F400
  0000000140C0D945: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C0D948: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140C0D950: EB 5F              jmp         0000000140C0D9B1
  0000000140C0D952: 33 C0              xor         eax,eax
  0000000140C0D954: 83 F8 01           cmp         eax,1
  0000000140C0D957: 74 0E              je          0000000140C0D967
  0000000140C0D959: 48 C7 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],0
                    00 00 00 00 00 00
  0000000140C0D965: EB 12              jmp         0000000140C0D979
  0000000140C0D967: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140C0D96C: E8 DF 72 57 FF     call        0000000140184C50
  0000000140C0D971: 48 89 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],rax
                    00 00
  0000000140C0D979: 48 8B 94 24 38 02  mov         rdx,qword ptr [rsp+0000000000000238h]
                    00 00
  0000000140C0D981: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140C0D989: E8 F2 E9 1E 00     call        0000000140DFC380
  0000000140C0D98E: 48 83 C0 08        add         rax,8
  0000000140C0D992: 48 89 84 24 40 02  mov         qword ptr [rsp+0000000000000240h],rax
                    00 00
  0000000140C0D99A: 48 8B 84 24 40 02  mov         rax,qword ptr [rsp+0000000000000240h]
                    00 00
  0000000140C0D9A2: 8B 08              mov         ecx,dword ptr [rax]
  0000000140C0D9A4: 83 E9 01           sub         ecx,1
  0000000140C0D9A7: 48 8B 84 24 40 02  mov         rax,qword ptr [rsp+0000000000000240h]
                    00 00
  0000000140C0D9AF: 89 08              mov         dword ptr [rax],ecx
  0000000140C0D9B1: 33 C0              xor         eax,eax
  0000000140C0D9B3: 83 F8 01           cmp         eax,1
  0000000140C0D9B6: 74 0E              je          0000000140C0D9C6
  0000000140C0D9B8: 48 C7 84 24 48 02  mov         qword ptr [rsp+0000000000000248h],0
                    00 00 00 00 00 00
  0000000140C0D9C4: EB 12              jmp         0000000140C0D9D8
  0000000140C0D9C6: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140C0D9CB: E8 80 72 57 FF     call        0000000140184C50
  0000000140C0D9D0: 48 89 84 24 48 02  mov         qword ptr [rsp+0000000000000248h],rax
                    00 00
  0000000140C0D9D8: 48 8B 94 24 48 02  mov         rdx,qword ptr [rsp+0000000000000248h]
                    00 00
  0000000140C0D9E0: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140C0D9E8: E8 93 E9 1E 00     call        0000000140DFC380
  0000000140C0D9ED: 83 78 08 00        cmp         dword ptr [rax+8],0
  0000000140C0D9F1: 0F 84 DE 01 00 00  je          0000000140C0DBD5
  0000000140C0D9F7: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0D9FF: 4C 8D 0D E2 91 AF  lea         r9,[0000000143706BE8h]
                    02
  0000000140C0DA06: 4C 8D 05 E3 37 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0DA0D: 33 D2              xor         edx,edx
  0000000140C0DA0F: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140C0DA17: E8 E0 1E 86 00     call        000000014146F8FC
  0000000140C0DA1C: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140C0DA24: 48 83 BC 24 A8 00  cmp         qword ptr [rsp+00000000000000A8h],0
                    00 00 00
  0000000140C0DA2D: 74 2F              je          0000000140C0DA5E
  0000000140C0DA2F: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140C0DA37: E8 24 05 00 00     call        0000000140C0DF60
  0000000140C0DA3C: 48 8B 94 24 78 02  mov         rdx,qword ptr [rsp+0000000000000278h]
                    00 00
  0000000140C0DA44: 48 8B C8           mov         rcx,rax
  0000000140C0DA47: FF 15 BB 64 1F 01  call        qword ptr [0000000141E03F08h]
  0000000140C0DA4D: 0F B6 C0           movzx       eax,al
  0000000140C0DA50: 85 C0              test        eax,eax
  0000000140C0DA52: 74 0A              je          0000000140C0DA5E
  0000000140C0DA54: C6 44 24 60 01     mov         byte ptr [rsp+60h],1
  0000000140C0DA59: E9 77 01 00 00     jmp         0000000140C0DBD5
  0000000140C0DA5E: 48 8D 15 6B E6 C0  lea         rdx,[000000014281C0D0h]
                    01
  0000000140C0DA65: 48 8B 8C 24 78 02  mov         rcx,qword ptr [rsp+0000000000000278h]
                    00 00
  0000000140C0DA6D: FF 15 F5 69 1F 01  call        qword ptr [0000000141E04468h]
  0000000140C0DA73: 0F B6 C0           movzx       eax,al
  0000000140C0DA76: 85 C0              test        eax,eax
  0000000140C0DA78: 74 42              je          0000000140C0DABC
  0000000140C0DA7A: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0DA82: 4C 8D 0D 57 88 A2  lea         r9,[00000001436362E0h]
                    02
  0000000140C0DA89: 4C 8D 05 60 37 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0DA90: 33 D2              xor         edx,edx
  0000000140C0DA92: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140C0DA9A: E8 5D 1E 86 00     call        000000014146F8FC
  0000000140C0DA9F: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140C0DAA7: 48 83 BC 24 B0 00  cmp         qword ptr [rsp+00000000000000B0h],0
                    00 00 00
  0000000140C0DAB0: 74 0A              je          0000000140C0DABC
  0000000140C0DAB2: C6 44 24 60 01     mov         byte ptr [rsp+60h],1
  0000000140C0DAB7: E9 19 01 00 00     jmp         0000000140C0DBD5
  0000000140C0DABC: 48 8D 15 1D E6 C0  lea         rdx,[000000014281C0E0h]
                    01
  0000000140C0DAC3: 48 8B 8C 24 78 02  mov         rcx,qword ptr [rsp+0000000000000278h]
                    00 00
  0000000140C0DACB: FF 15 97 69 1F 01  call        qword ptr [0000000141E04468h]
  0000000140C0DAD1: 0F B6 C0           movzx       eax,al
  0000000140C0DAD4: 85 C0              test        eax,eax
  0000000140C0DAD6: 74 42              je          0000000140C0DB1A
  0000000140C0DAD8: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0DAE0: 4C 8D 0D F1 11 CD  lea         r9,[00000001438DECD8h]
                    02
  0000000140C0DAE7: 4C 8D 05 02 37 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0DAEE: 33 D2              xor         edx,edx
  0000000140C0DAF0: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140C0DAF8: E8 FF 1D 86 00     call        000000014146F8FC
  0000000140C0DAFD: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140C0DB05: 48 83 BC 24 B8 00  cmp         qword ptr [rsp+00000000000000B8h],0
                    00 00 00
  0000000140C0DB0E: 74 0A              je          0000000140C0DB1A
  0000000140C0DB10: C6 44 24 60 01     mov         byte ptr [rsp+60h],1
  0000000140C0DB15: E9 BB 00 00 00     jmp         0000000140C0DBD5
  0000000140C0DB1A: 48 8D 15 D7 E5 C0  lea         rdx,[000000014281C0F8h]
                    01
  0000000140C0DB21: 48 8B 8C 24 78 02  mov         rcx,qword ptr [rsp+0000000000000278h]
                    00 00
  0000000140C0DB29: FF 15 39 69 1F 01  call        qword ptr [0000000141E04468h]
  0000000140C0DB2F: 0F B6 C0           movzx       eax,al
  0000000140C0DB32: 85 C0              test        eax,eax
  0000000140C0DB34: 74 3F              je          0000000140C0DB75
  0000000140C0DB36: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0DB3E: 4C 8D 0D BB 11 CD  lea         r9,[00000001438DED00h]
                    02
  0000000140C0DB45: 4C 8D 05 A4 36 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0DB4C: 33 D2              xor         edx,edx
  0000000140C0DB4E: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140C0DB56: E8 A1 1D 86 00     call        000000014146F8FC
  0000000140C0DB5B: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140C0DB63: 48 83 BC 24 C0 00  cmp         qword ptr [rsp+00000000000000C0h],0
                    00 00 00
  0000000140C0DB6C: 74 07              je          0000000140C0DB75
  0000000140C0DB6E: C6 44 24 60 01     mov         byte ptr [rsp+60h],1
  0000000140C0DB73: EB 60              jmp         0000000140C0DBD5
  0000000140C0DB75: 48 8D 15 94 E5 C0  lea         rdx,[000000014281C110h]
                    01
  0000000140C0DB7C: 48 8B 8C 24 78 02  mov         rcx,qword ptr [rsp+0000000000000278h]
                    00 00
  0000000140C0DB84: FF 15 DE 68 1F 01  call        qword ptr [0000000141E04468h]
  0000000140C0DB8A: 0F B6 C0           movzx       eax,al
  0000000140C0DB8D: 85 C0              test        eax,eax
  0000000140C0DB8F: 74 3F              je          0000000140C0DBD0
  0000000140C0DB91: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0DB99: 4C 8D 0D A0 90 AF  lea         r9,[0000000143706C40h]
                    02
  0000000140C0DBA0: 4C 8D 05 49 36 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0DBA7: 33 D2              xor         edx,edx
  0000000140C0DBA9: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140C0DBB1: E8 46 1D 86 00     call        000000014146F8FC
  0000000140C0DBB6: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140C0DBBE: 48 83 BC 24 C8 00  cmp         qword ptr [rsp+00000000000000C8h],0
                    00 00 00
  0000000140C0DBC7: 74 07              je          0000000140C0DBD0
  0000000140C0DBC9: C6 44 24 60 01     mov         byte ptr [rsp+60h],1
  0000000140C0DBCE: EB 05              jmp         0000000140C0DBD5
  0000000140C0DBD0: E9 7D FD FF FF     jmp         0000000140C0D952
  0000000140C0DBD5: E9 8C FC FF FF     jmp         0000000140C0D866
  0000000140C0DBDA: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140C0DBDF: E8 3C 39 04 00     call        0000000140C51520
  0000000140C0DBE4: 0F B6 44 24 60     movzx       eax,byte ptr [rsp+60h]
  0000000140C0DBE9: 85 C0              test        eax,eax
  0000000140C0DBEB: 0F 84 FE 01 00 00  je          0000000140C0DDEF
  0000000140C0DBF1: 45 33 C0           xor         r8d,r8d
  0000000140C0DBF4: 33 D2              xor         edx,edx
  0000000140C0DBF6: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140C0DBFE: FF 15 64 65 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0DC04: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  0000000140C0DC0C: 48 8B F0           mov         rsi,rax
  0000000140C0DC0F: B9 10 00 00 00     mov         ecx,10h
  0000000140C0DC14: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0DC16: 45 33 C0           xor         r8d,r8d
  0000000140C0DC19: 33 D2              xor         edx,edx
  0000000140C0DC1B: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140C0DC23: FF 15 3F 65 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0DC29: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  0000000140C0DC31: 48 8B F0           mov         rsi,rax
  0000000140C0DC34: B9 10 00 00 00     mov         ecx,10h
  0000000140C0DC39: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0DC3B: 45 33 C0           xor         r8d,r8d
  0000000140C0DC3E: 33 D2              xor         edx,edx
  0000000140C0DC40: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000140C0DC48: FF 15 1A 65 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0DC4E: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  0000000140C0DC56: 48 8B F0           mov         rsi,rax
  0000000140C0DC59: B9 10 00 00 00     mov         ecx,10h
  0000000140C0DC5E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0DC60: 45 33 C0           xor         r8d,r8d
  0000000140C0DC63: 33 D2              xor         edx,edx
  0000000140C0DC65: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140C0DC6D: FF 15 F5 64 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0DC73: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  0000000140C0DC7B: 48 8B F0           mov         rsi,rax
  0000000140C0DC7E: B9 10 00 00 00     mov         ecx,10h
  0000000140C0DC83: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0DC85: 45 33 C0           xor         r8d,r8d
  0000000140C0DC88: 33 D2              xor         edx,edx
  0000000140C0DC8A: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000140C0DC92: FF 15 D0 64 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0DC98: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  0000000140C0DCA0: 48 8B F0           mov         rsi,rax
  0000000140C0DCA3: B9 10 00 00 00     mov         ecx,10h
  0000000140C0DCA8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0DCAA: 45 33 C0           xor         r8d,r8d
  0000000140C0DCAD: 33 D2              xor         edx,edx
  0000000140C0DCAF: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  0000000140C0DCB7: FF 15 AB 64 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0DCBD: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  0000000140C0DCC5: 48 8B F0           mov         rsi,rax
  0000000140C0DCC8: B9 10 00 00 00     mov         ecx,10h
  0000000140C0DCCD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0DCCF: 45 33 C0           xor         r8d,r8d
  0000000140C0DCD2: 33 D2              xor         edx,edx
  0000000140C0DCD4: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  0000000140C0DCDC: FF 15 86 64 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0DCE2: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  0000000140C0DCEA: 48 8B F0           mov         rsi,rax
  0000000140C0DCED: B9 10 00 00 00     mov         ecx,10h
  0000000140C0DCF2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0DCF4: 45 33 C0           xor         r8d,r8d
  0000000140C0DCF7: 33 D2              xor         edx,edx
  0000000140C0DCF9: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000140C0DD01: FF 15 61 64 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0DD07: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140C0DD0F: 48 8B F0           mov         rsi,rax
  0000000140C0DD12: B9 10 00 00 00     mov         ecx,10h
  0000000140C0DD17: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0DD19: 45 33 C0           xor         r8d,r8d
  0000000140C0DD1C: 33 D2              xor         edx,edx
  0000000140C0DD1E: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  0000000140C0DD26: FF 15 3C 64 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0DD2C: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140C0DD34: 48 8B F0           mov         rsi,rax
  0000000140C0DD37: B9 10 00 00 00     mov         ecx,10h
  0000000140C0DD3C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0DD3E: 45 33 C0           xor         r8d,r8d
  0000000140C0DD41: 33 D2              xor         edx,edx
  0000000140C0DD43: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  0000000140C0DD4B: FF 15 17 64 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0DD51: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140C0DD59: 48 8B F0           mov         rsi,rax
  0000000140C0DD5C: B9 10 00 00 00     mov         ecx,10h
  0000000140C0DD61: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0DD63: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  0000000140C0DD6B: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C0DD70: 48 8D 84 24 F0 01  lea         rax,[rsp+00000000000001F0h]
                    00 00
  0000000140C0DD78: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C0DD7D: 48 8D 84 24 E0 01  lea         rax,[rsp+00000000000001E0h]
                    00 00
  0000000140C0DD85: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140C0DD8A: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  0000000140C0DD92: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C0DD97: 48 8D 84 24 C0 01  lea         rax,[rsp+00000000000001C0h]
                    00 00
  0000000140C0DD9F: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C0DDA4: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  0000000140C0DDAC: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C0DDB1: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140C0DDB9: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C0DDBE: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140C0DDC6: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C0DDCB: 4C 8D 8C 24 80 01  lea         r9,[rsp+0000000000000180h]
                    00 00
  0000000140C0DDD3: 4C 8D 84 24 70 01  lea         r8,[rsp+0000000000000170h]
                    00 00
  0000000140C0DDDB: 48 8D 15 3E E3 C0  lea         rdx,[000000014281C120h]
                    01
  0000000140C0DDE2: 48 8D 0D 17 89 16  lea         rcx,[0000000143D76700h]
                    03
  0000000140C0DDE9: E8 D2 15 6C FF     call        00000001402CF3C0
  0000000140C0DDEE: 90                 nop
  0000000140C0DDEF: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140C0DDF4: E8 A7 A3 5F FF     call        00000001402081A0
  0000000140C0DDF9: 90                 nop
  0000000140C0DDFA: 48 8B 8C 24 78 02  mov         rcx,qword ptr [rsp+0000000000000278h]
                    00 00
  0000000140C0DE02: FF 15 D8 7B 1F 01  call        qword ptr [0000000141E059E0h]
  0000000140C0DE08: 48 81 C4 58 02 00  add         rsp,258h
                    00
  0000000140C0DE0F: 5F                 pop         rdi
  0000000140C0DE10: 5E                 pop         rsi
  0000000140C0DE11: C3                 ret

  Summary

     1E02000 .text
