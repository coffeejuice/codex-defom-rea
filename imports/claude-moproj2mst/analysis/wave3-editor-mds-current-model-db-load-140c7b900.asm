Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C7B900: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140C7B905: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C7B90A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C7B90F: 56                 push        rsi
  0000000140C7B910: 57                 push        rdi
  0000000140C7B911: 48 81 EC 28 03 00  sub         rsp,328h
                    00
  0000000140C7B918: 48 8B 84 24 40 03  mov         rax,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7B920: 48 83 B8 D8 00 00  cmp         qword ptr [rax+00000000000000D8h],0
                    00 00
  0000000140C7B928: 74 12              je          0000000140C7B93C
  0000000140C7B92A: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7B932: E8 99 F3 FF FF     call        0000000140C7ACD0
  0000000140C7B937: 48 85 C0           test        rax,rax
  0000000140C7B93A: 75 07              jne         0000000140C7B943
  0000000140C7B93C: 32 C0              xor         al,al
  0000000140C7B93E: E9 4B 07 00 00     jmp         0000000140C7C08E
  0000000140C7B943: 0F B6 84 24 50 03  movzx       eax,byte ptr [rsp+0000000000000350h]
                    00 00
  0000000140C7B94B: 85 C0              test        eax,eax
  0000000140C7B94D: 74 0D              je          0000000140C7B95C
  0000000140C7B94F: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7B957: E8 14 73 00 00     call        0000000140C82C70
  0000000140C7B95C: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7B964: E8 17 01 01 00     call        0000000140C8BA80
  0000000140C7B969: 48 8D 0D 48 AD 0F  lea         rcx,[0000000143D766B8h]
                    03
  0000000140C7B970: E8 2B FE 64 FF     call        00000001402CB7A0
  0000000140C7B975: 83 F8 03           cmp         eax,3
  0000000140C7B978: 75 21              jne         0000000140C7B99B
  0000000140C7B97A: C6 44 24 78 00     mov         byte ptr [rsp+78h],0
  0000000140C7B97F: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7B987: E8 44 F3 FF FF     call        0000000140C7ACD0
  0000000140C7B98C: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140C7B991: 48 8B C8           mov         rcx,rax
  0000000140C7B994: E8 A7 AF C1 FF     call        0000000140896940
  0000000140C7B999: EB 1F              jmp         0000000140C7B9BA
  0000000140C7B99B: C6 44 24 79 01     mov         byte ptr [rsp+79h],1
  0000000140C7B9A0: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7B9A8: E8 23 F3 FF FF     call        0000000140C7ACD0
  0000000140C7B9AD: 48 8D 54 24 79     lea         rdx,[rsp+79h]
  0000000140C7B9B2: 48 8B C8           mov         rcx,rax
  0000000140C7B9B5: E8 86 AF C1 FF     call        0000000140896940
  0000000140C7B9BA: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7B9C2: E8 89 F2 FF FF     call        0000000140C7AC50
  0000000140C7B9C7: 48 8B F8           mov         rdi,rax
  0000000140C7B9CA: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7B9D2: E8 F9 F2 FF FF     call        0000000140C7ACD0
  0000000140C7B9D7: 48 8B C8           mov         rcx,rax
  0000000140C7B9DA: 45 33 C9           xor         r9d,r9d
  0000000140C7B9DD: 48 8B 84 24 48 03  mov         rax,qword ptr [rsp+0000000000000348h]
                    00 00
  0000000140C7B9E5: 44 8B 00           mov         r8d,dword ptr [rax]
  0000000140C7B9E8: 48 8B D7           mov         rdx,rdi
  0000000140C7B9EB: E8 D0 9C BA FF     call        00000001408256C0
  0000000140C7B9F0: 89 44 24 64        mov         dword ptr [rsp+64h],eax
  0000000140C7B9F4: 83 7C 24 64 00     cmp         dword ptr [rsp+64h],0
  0000000140C7B9F9: 74 07              je          0000000140C7BA02
  0000000140C7B9FB: 32 C0              xor         al,al
  0000000140C7B9FD: E9 8C 06 00 00     jmp         0000000140C7C08E
  0000000140C7BA02: 48 8D 0D AF AC 0F  lea         rcx,[0000000143D766B8h]
                    03
  0000000140C7BA09: E8 92 FD 64 FF     call        00000001402CB7A0
  0000000140C7BA0E: 83 F8 01           cmp         eax,1
  0000000140C7BA11: 75 43              jne         0000000140C7BA56
  0000000140C7BA13: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BA1B: E8 B0 F8 FF FF     call        0000000140C7B2D0
  0000000140C7BA20: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140C7BA25: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140C7BA2B: 74 29              je          0000000140C7BA56
  0000000140C7BA2D: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140C7BA32: 83 B8 60 01 00 00  cmp         dword ptr [rax+0000000000000160h],0
                    00
  0000000140C7BA39: 7C 1B              jl          0000000140C7BA56
  0000000140C7BA3B: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140C7BA40: 8B 88 60 01 00 00  mov         ecx,dword ptr [rax+0000000000000160h]
  0000000140C7BA46: 83 C1 01           add         ecx,1
  0000000140C7BA49: F7 D9              neg         ecx
  0000000140C7BA4B: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140C7BA50: 89 88 60 01 00 00  mov         dword ptr [rax+0000000000000160h],ecx
  0000000140C7BA56: C6 44 24 7A 00     mov         byte ptr [rsp+7Ah],0
  0000000140C7BA5B: 48 8D 54 24 7A     lea         rdx,[rsp+7Ah]
  0000000140C7BA60: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BA68: 48 8B 89 D8 00 00  mov         rcx,qword ptr [rcx+00000000000000D8h]
                    00
  0000000140C7BA6F: E8 5C 02 06 00     call        0000000140CDBCD0
  0000000140C7BA74: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BA7C: E8 1F 19 00 00     call        0000000140C7D3A0
  0000000140C7BA81: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BA89: E8 52 31 00 00     call        0000000140C7EBE0
  0000000140C7BA8E: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BA96: E8 B5 F1 FF FF     call        0000000140C7AC50
  0000000140C7BA9B: 48 8B C8           mov         rcx,rax
  0000000140C7BA9E: 48 8B 84 24 48 03  mov         rax,qword ptr [rsp+0000000000000348h]
                    00 00
  0000000140C7BAA6: 8B 10              mov         edx,dword ptr [rax]
  0000000140C7BAA8: E8 53 2A B9 FF     call        000000014080E500
  0000000140C7BAAD: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  0000000140C7BAB1: C6 44 24 68 00     mov         byte ptr [rsp+68h],0
  0000000140C7BAB6: 44 0F B6 44 24 68  movzx       r8d,byte ptr [rsp+68h]
  0000000140C7BABC: 0F B6 94 24 50 03  movzx       edx,byte ptr [rsp+0000000000000350h]
                    00 00
  0000000140C7BAC4: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BACC: E8 AF 44 00 00     call        0000000140C7FF80
  0000000140C7BAD1: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BAD9: E8 22 F3 FF FF     call        0000000140C7AE00
  0000000140C7BADE: 48 85 C0           test        rax,rax
  0000000140C7BAE1: 74 19              je          0000000140C7BAFC
  0000000140C7BAE3: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BAEB: E8 10 F3 FF FF     call        0000000140C7AE00
  0000000140C7BAF0: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  0000000140C7BAF4: 48 8B C8           mov         rcx,rax
  0000000140C7BAF7: E8 04 11 09 00     call        0000000140D0CC00
  0000000140C7BAFC: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BB04: E8 B7 56 01 00     call        0000000140C911C0
  0000000140C7BB09: 0F B6 C0           movzx       eax,al
  0000000140C7BB0C: 85 C0              test        eax,eax
  0000000140C7BB0E: 75 21              jne         0000000140C7BB31
  0000000140C7BB10: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BB18: E8 93 4A 01 00     call        0000000140C905B0
  0000000140C7BB1D: 48 8B C8           mov         rcx,rax
  0000000140C7BB20: E8 1B A0 09 00     call        0000000140D15B40
  0000000140C7BB25: 48 8D 0D 8C AB 0F  lea         rcx,[0000000143D766B8h]
                    03
  0000000140C7BB2C: E8 5F ED 64 FF     call        00000001402CA890
  0000000140C7BB31: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BB39: E8 12 46 01 00     call        0000000140C90150
  0000000140C7BB3E: C7 44 24 7C 01 00  mov         dword ptr [rsp+7Ch],1
                    00 00
  0000000140C7BB46: 48 8D 54 24 7C     lea         rdx,[rsp+7Ch]
  0000000140C7BB4B: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BB53: E8 68 78 00 00     call        0000000140C833C0
  0000000140C7BB58: 48 8B 94 24 40 03  mov         rdx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BB60: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BB68: E8 A3 BA 0F 00     call        0000000140D77610
  0000000140C7BB6D: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  0000000140C7BB72: 48 8D 15 8B ED C7  lea         rdx,[00000001428FA904h]
                    01
  0000000140C7BB79: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  0000000140C7BB81: E8 FA 94 3A FF     call        0000000140025080
  0000000140C7BB86: 48 8D BC 24 10 03  lea         rdi,[rsp+0000000000000310h]
                    00 00
  0000000140C7BB8E: 48 8B F0           mov         rsi,rax
  0000000140C7BB91: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BB96: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BB98: 48 8D BC 24 10 01  lea         rdi,[rsp+0000000000000110h]
                    00 00
  0000000140C7BBA0: 48 8D B4 24 10 03  lea         rsi,[rsp+0000000000000310h]
                    00 00
  0000000140C7BBA8: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BBAD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BBAF: 45 33 C0           xor         r8d,r8d
  0000000140C7BBB2: 33 D2              xor         edx,edx
  0000000140C7BBB4: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140C7BBBC: FF 15 A6 85 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BBC2: 48 8D BC 24 60 02  lea         rdi,[rsp+0000000000000260h]
                    00 00
  0000000140C7BBCA: 48 8B F0           mov         rsi,rax
  0000000140C7BBCD: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BBD2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BBD4: 45 33 C0           xor         r8d,r8d
  0000000140C7BBD7: 33 D2              xor         edx,edx
  0000000140C7BBD9: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140C7BBE1: FF 15 81 85 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BBE7: 48 8D BC 24 50 02  lea         rdi,[rsp+0000000000000250h]
                    00 00
  0000000140C7BBEF: 48 8B F0           mov         rsi,rax
  0000000140C7BBF2: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BBF7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BBF9: 45 33 C0           xor         r8d,r8d
  0000000140C7BBFC: 33 D2              xor         edx,edx
  0000000140C7BBFE: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140C7BC06: FF 15 5C 85 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BC0C: 48 8D BC 24 40 02  lea         rdi,[rsp+0000000000000240h]
                    00 00
  0000000140C7BC14: 48 8B F0           mov         rsi,rax
  0000000140C7BC17: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BC1C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BC1E: 45 33 C0           xor         r8d,r8d
  0000000140C7BC21: 33 D2              xor         edx,edx
  0000000140C7BC23: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140C7BC2B: FF 15 37 85 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BC31: 48 8D BC 24 30 02  lea         rdi,[rsp+0000000000000230h]
                    00 00
  0000000140C7BC39: 48 8B F0           mov         rsi,rax
  0000000140C7BC3C: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BC41: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BC43: 45 33 C0           xor         r8d,r8d
  0000000140C7BC46: 33 D2              xor         edx,edx
  0000000140C7BC48: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140C7BC50: FF 15 12 85 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BC56: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  0000000140C7BC5E: 48 8B F0           mov         rsi,rax
  0000000140C7BC61: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BC66: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BC68: 45 33 C0           xor         r8d,r8d
  0000000140C7BC6B: 33 D2              xor         edx,edx
  0000000140C7BC6D: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140C7BC75: FF 15 ED 84 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BC7B: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  0000000140C7BC83: 48 8B F0           mov         rsi,rax
  0000000140C7BC86: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BC8B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BC8D: 45 33 C0           xor         r8d,r8d
  0000000140C7BC90: 33 D2              xor         edx,edx
  0000000140C7BC92: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140C7BC9A: FF 15 C8 84 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BCA0: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  0000000140C7BCA8: 48 8B F0           mov         rsi,rax
  0000000140C7BCAB: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BCB0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BCB2: 45 33 C0           xor         r8d,r8d
  0000000140C7BCB5: 33 D2              xor         edx,edx
  0000000140C7BCB7: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000140C7BCBF: FF 15 A3 84 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BCC5: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  0000000140C7BCCD: 48 8B F0           mov         rsi,rax
  0000000140C7BCD0: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BCD5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BCD7: 45 33 C0           xor         r8d,r8d
  0000000140C7BCDA: 33 D2              xor         edx,edx
  0000000140C7BCDC: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140C7BCE4: FF 15 7E 84 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BCEA: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  0000000140C7BCF2: 48 8B F0           mov         rsi,rax
  0000000140C7BCF5: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BCFA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BCFC: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  0000000140C7BD04: 48 8D B4 24 10 01  lea         rsi,[rsp+0000000000000110h]
                    00 00
  0000000140C7BD0C: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BD11: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BD13: 48 8D 84 24 60 02  lea         rax,[rsp+0000000000000260h]
                    00 00
  0000000140C7BD1B: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C7BD20: 48 8D 84 24 50 02  lea         rax,[rsp+0000000000000250h]
                    00 00
  0000000140C7BD28: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C7BD2D: 48 8D 84 24 40 02  lea         rax,[rsp+0000000000000240h]
                    00 00
  0000000140C7BD35: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140C7BD3A: 48 8D 84 24 30 02  lea         rax,[rsp+0000000000000230h]
                    00 00
  0000000140C7BD42: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C7BD47: 48 8D 84 24 20 02  lea         rax,[rsp+0000000000000220h]
                    00 00
  0000000140C7BD4F: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C7BD54: 48 8D 84 24 10 02  lea         rax,[rsp+0000000000000210h]
                    00 00
  0000000140C7BD5C: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C7BD61: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  0000000140C7BD69: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C7BD6E: 48 8D 84 24 F0 01  lea         rax,[rsp+00000000000001F0h]
                    00 00
  0000000140C7BD76: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C7BD7B: 4C 8D 8C 24 E0 01  lea         r9,[rsp+00000000000001E0h]
                    00 00
  0000000140C7BD83: 4C 8D 84 24 D0 01  lea         r8,[rsp+00000000000001D0h]
                    00 00
  0000000140C7BD8B: 48 8D 15 46 21 C8  lea         rdx,[00000001428FDED8h]
                    01
  0000000140C7BD92: 48 8D 0D 67 A9 0F  lea         rcx,[0000000143D76700h]
                    03
  0000000140C7BD99: E8 22 36 65 FF     call        00000001402CF3C0
  0000000140C7BD9E: 45 33 C0           xor         r8d,r8d
  0000000140C7BDA1: 33 D2              xor         edx,edx
  0000000140C7BDA3: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  0000000140C7BDAB: FF 15 B7 83 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BDB1: 48 8D BC 24 00 03  lea         rdi,[rsp+0000000000000300h]
                    00 00
  0000000140C7BDB9: 48 8B F0           mov         rsi,rax
  0000000140C7BDBC: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BDC1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BDC3: 45 33 C0           xor         r8d,r8d
  0000000140C7BDC6: 33 D2              xor         edx,edx
  0000000140C7BDC8: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000140C7BDD0: FF 15 92 83 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BDD6: 48 8D BC 24 F0 02  lea         rdi,[rsp+00000000000002F0h]
                    00 00
  0000000140C7BDDE: 48 8B F0           mov         rsi,rax
  0000000140C7BDE1: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BDE6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BDE8: 45 33 C0           xor         r8d,r8d
  0000000140C7BDEB: 33 D2              xor         edx,edx
  0000000140C7BDED: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  0000000140C7BDF5: FF 15 6D 83 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BDFB: 48 8D BC 24 E0 02  lea         rdi,[rsp+00000000000002E0h]
                    00 00
  0000000140C7BE03: 48 8B F0           mov         rsi,rax
  0000000140C7BE06: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BE0B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BE0D: 45 33 C0           xor         r8d,r8d
  0000000140C7BE10: 33 D2              xor         edx,edx
  0000000140C7BE12: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  0000000140C7BE1A: FF 15 48 83 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BE20: 48 8D BC 24 D0 02  lea         rdi,[rsp+00000000000002D0h]
                    00 00
  0000000140C7BE28: 48 8B F0           mov         rsi,rax
  0000000140C7BE2B: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BE30: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BE32: 45 33 C0           xor         r8d,r8d
  0000000140C7BE35: 33 D2              xor         edx,edx
  0000000140C7BE37: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  0000000140C7BE3F: FF 15 23 83 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BE45: 48 8D BC 24 C0 02  lea         rdi,[rsp+00000000000002C0h]
                    00 00
  0000000140C7BE4D: 48 8B F0           mov         rsi,rax
  0000000140C7BE50: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BE55: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BE57: 45 33 C0           xor         r8d,r8d
  0000000140C7BE5A: 33 D2              xor         edx,edx
  0000000140C7BE5C: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  0000000140C7BE64: FF 15 FE 82 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BE6A: 48 8D BC 24 B0 02  lea         rdi,[rsp+00000000000002B0h]
                    00 00
  0000000140C7BE72: 48 8B F0           mov         rsi,rax
  0000000140C7BE75: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BE7A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BE7C: 45 33 C0           xor         r8d,r8d
  0000000140C7BE7F: 33 D2              xor         edx,edx
  0000000140C7BE81: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  0000000140C7BE89: FF 15 D9 82 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BE8F: 48 8D BC 24 A0 02  lea         rdi,[rsp+00000000000002A0h]
                    00 00
  0000000140C7BE97: 48 8B F0           mov         rsi,rax
  0000000140C7BE9A: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BE9F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BEA1: 45 33 C0           xor         r8d,r8d
  0000000140C7BEA4: 33 D2              xor         edx,edx
  0000000140C7BEA6: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  0000000140C7BEAE: FF 15 B4 82 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BEB4: 48 8D BC 24 90 02  lea         rdi,[rsp+0000000000000290h]
                    00 00
  0000000140C7BEBC: 48 8B F0           mov         rsi,rax
  0000000140C7BEBF: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BEC4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BEC6: 45 33 C0           xor         r8d,r8d
  0000000140C7BEC9: 33 D2              xor         edx,edx
  0000000140C7BECB: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  0000000140C7BED3: FF 15 8F 82 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BED9: 48 8D BC 24 80 02  lea         rdi,[rsp+0000000000000280h]
                    00 00
  0000000140C7BEE1: 48 8B F0           mov         rsi,rax
  0000000140C7BEE4: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BEE9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BEEB: 45 33 C0           xor         r8d,r8d
  0000000140C7BEEE: 33 D2              xor         edx,edx
  0000000140C7BEF0: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  0000000140C7BEF8: FF 15 6A 82 18 01  call        qword ptr [0000000141E04168h]
  0000000140C7BEFE: 48 8D BC 24 70 02  lea         rdi,[rsp+0000000000000270h]
                    00 00
  0000000140C7BF06: 48 8B F0           mov         rsi,rax
  0000000140C7BF09: B9 10 00 00 00     mov         ecx,10h
  0000000140C7BF0E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C7BF10: 48 8D 84 24 00 03  lea         rax,[rsp+0000000000000300h]
                    00 00
  0000000140C7BF18: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C7BF1D: 48 8D 84 24 F0 02  lea         rax,[rsp+00000000000002F0h]
                    00 00
  0000000140C7BF25: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C7BF2A: 48 8D 84 24 E0 02  lea         rax,[rsp+00000000000002E0h]
                    00 00
  0000000140C7BF32: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140C7BF37: 48 8D 84 24 D0 02  lea         rax,[rsp+00000000000002D0h]
                    00 00
  0000000140C7BF3F: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C7BF44: 48 8D 84 24 C0 02  lea         rax,[rsp+00000000000002C0h]
                    00 00
  0000000140C7BF4C: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C7BF51: 48 8D 84 24 B0 02  lea         rax,[rsp+00000000000002B0h]
                    00 00
  0000000140C7BF59: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C7BF5E: 48 8D 84 24 A0 02  lea         rax,[rsp+00000000000002A0h]
                    00 00
  0000000140C7BF66: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C7BF6B: 48 8D 84 24 90 02  lea         rax,[rsp+0000000000000290h]
                    00 00
  0000000140C7BF73: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C7BF78: 4C 8D 8C 24 80 02  lea         r9,[rsp+0000000000000280h]
                    00 00
  0000000140C7BF80: 4C 8D 84 24 70 02  lea         r8,[rsp+0000000000000270h]
                    00 00
  0000000140C7BF88: 48 8D 15 69 1F C8  lea         rdx,[00000001428FDEF8h]
                    01
  0000000140C7BF8F: 48 8D 0D 6A A7 0F  lea         rcx,[0000000143D76700h]
                    03
  0000000140C7BF96: E8 25 34 65 FF     call        00000001402CF3C0
  0000000140C7BF9B: 48 8B 84 24 40 03  mov         rax,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BFA3: 48 83 B8 D8 00 00  cmp         qword ptr [rax+00000000000000D8h],0
                    00 00
  0000000140C7BFAB: 74 2C              je          0000000140C7BFD9
  0000000140C7BFAD: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BFB5: 48 8B 89 D8 00 00  mov         rcx,qword ptr [rcx+00000000000000D8h]
                    00
  0000000140C7BFBC: 48 83 C1 18        add         rcx,18h
  0000000140C7BFC0: E8 4B B4 68 FF     call        0000000140307410
  0000000140C7BFC5: 0F B6 C0           movzx       eax,al
  0000000140C7BFC8: 85 C0              test        eax,eax
  0000000140C7BFCA: 75 0D              jne         0000000140C7BFD9
  0000000140C7BFCC: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BFD4: E8 57 09 01 00     call        0000000140C8C930
  0000000140C7BFD9: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BFE1: E8 1A EE FF FF     call        0000000140C7AE00
  0000000140C7BFE6: 48 85 C0           test        rax,rax
  0000000140C7BFE9: 74 3C              je          0000000140C7C027
  0000000140C7BFEB: 0F B6 84 24 50 03  movzx       eax,byte ptr [rsp+0000000000000350h]
                    00 00
  0000000140C7BFF3: 85 C0              test        eax,eax
  0000000140C7BFF5: 74 17              je          0000000140C7C00E
  0000000140C7BFF7: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7BFFF: E8 FC ED FF FF     call        0000000140C7AE00
  0000000140C7C004: 33 D2              xor         edx,edx
  0000000140C7C006: 48 8B C8           mov         rcx,rax
  0000000140C7C009: E8 E2 D7 07 00     call        0000000140CF97F0
  0000000140C7C00E: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7C016: E8 E5 ED FF FF     call        0000000140C7AE00
  0000000140C7C01B: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  0000000140C7C01F: 48 8B C8           mov         rcx,rax
  0000000140C7C022: E8 49 33 08 00     call        0000000140CFF370
  0000000140C7C027: 0F B6 84 24 50 03  movzx       eax,byte ptr [rsp+0000000000000350h]
                    00 00
  0000000140C7C02F: 85 C0              test        eax,eax
  0000000140C7C031: 74 59              je          0000000140C7C08C
  0000000140C7C033: 48 8B 84 24 40 03  mov         rax,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7C03B: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  0000000140C7C040: 74 4A              je          0000000140C7C08C
  0000000140C7C042: 48 8B 84 24 40 03  mov         rax,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7C04A: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C7C04E: 83 B8 04 02 00 00  cmp         dword ptr [rax+0000000000000204h],1
                    01
  0000000140C7C055: 75 11              jne         0000000140C7C068
  0000000140C7C057: 33 D2              xor         edx,edx
  0000000140C7C059: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7C061: E8 BA 49 00 00     call        0000000140C80A20
  0000000140C7C066: EB 24              jmp         0000000140C7C08C
  0000000140C7C068: 48 8B 84 24 40 03  mov         rax,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7C070: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C7C074: 83 B8 04 02 00 00  cmp         dword ptr [rax+0000000000000204h],2
                    02
  0000000140C7C07B: 75 0F              jne         0000000140C7C08C
  0000000140C7C07D: 33 D2              xor         edx,edx
  0000000140C7C07F: 48 8B 8C 24 40 03  mov         rcx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140C7C087: E8 74 4A 00 00     call        0000000140C80B00
  0000000140C7C08C: B0 01              mov         al,1
  0000000140C7C08E: 48 81 C4 28 03 00  add         rsp,328h
                    00
  0000000140C7C095: 5F                 pop         rdi
  0000000140C7C096: 5E                 pop         rsi
  0000000140C7C097: C3                 ret

  Summary

     1E02000 .text
