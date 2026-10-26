Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140394A30: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140394A34: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140394A39: 56                 push        rsi
  0000000140394A3A: 57                 push        rdi
  0000000140394A3B: 48 81 EC E8 03 00  sub         rsp,3E8h
                    00
  0000000140394A42: 48 C7 84 24 D0 03  mov         qword ptr [rsp+00000000000003D0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140394A4E: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394A56: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140394A59: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394A61: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140394A67: 48 85 C0           test        rax,rax
  0000000140394A6A: 0F 84 BF 07 00 00  je          000000014039522F
  0000000140394A70: C6 44 24 69 00     mov         byte ptr [rsp+69h],0
  0000000140394A75: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394A7D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140394A80: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394A88: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140394A8E: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140394A93: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394A9B: 83 78 48 02        cmp         dword ptr [rax+48h],2
  0000000140394A9F: 75 16              jne         0000000140394AB7
  0000000140394AA1: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140394AA6: E8 F5 75 8E 00     call        0000000140C7C0A0
  0000000140394AAB: 0F B6 C0           movzx       eax,al
  0000000140394AAE: 85 C0              test        eax,eax
  0000000140394AB0: 74 05              je          0000000140394AB7
  0000000140394AB2: C6 44 24 69 01     mov         byte ptr [rsp+69h],1
  0000000140394AB7: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394ABF: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140394AC2: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394ACA: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140394AD0: 44 0F B6 8C 24 08  movzx       r9d,byte ptr [rsp+0000000000000408h]
                    04 00 00
  0000000140394AD9: 45 33 C0           xor         r8d,r8d
  0000000140394ADC: 0F B6 54 24 69     movzx       edx,byte ptr [rsp+69h]
  0000000140394AE1: 48 8B C8           mov         rcx,rax
  0000000140394AE4: E8 D7 DE FC FF     call        00000001403629C0
  0000000140394AE9: 88 44 24 68        mov         byte ptr [rsp+68h],al
  0000000140394AED: 0F B6 44 24 68     movzx       eax,byte ptr [rsp+68h]
  0000000140394AF2: 85 C0              test        eax,eax
  0000000140394AF4: 0F 84 35 07 00 00  je          000000014039522F
  0000000140394AFA: B2 01              mov         dl,1
  0000000140394AFC: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394B04: E8 47 43 FE FF     call        0000000140378E50
  0000000140394B09: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394B11: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140394B14: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394B1C: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  0000000140394B22: 89 84 24 20 01 00  mov         dword ptr [rsp+0000000000000120h],eax
                    00
  0000000140394B29: C7 84 24 48 01 00  mov         dword ptr [rsp+0000000000000148h],3F3h
                    00 F3 03 00 00
  0000000140394B34: 4C 8D 84 24 20 01  lea         r8,[rsp+0000000000000120h]
                    00 00
  0000000140394B3C: 48 8D 15 B9 D7 F9  lea         rdx,[00000001423322FCh]
                    01
  0000000140394B43: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000140394B4B: E8 30 05 C9 FF     call        0000000140025080
  0000000140394B50: 48 8D BC 24 C0 03  lea         rdi,[rsp+00000000000003C0h]
                    00 00
  0000000140394B58: 48 8B F0           mov         rsi,rax
  0000000140394B5B: B9 10 00 00 00     mov         ecx,10h
  0000000140394B60: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394B62: 48 8D BC 24 00 01  lea         rdi,[rsp+0000000000000100h]
                    00 00
  0000000140394B6A: 48 8D B4 24 C0 03  lea         rsi,[rsp+00000000000003C0h]
                    00 00
  0000000140394B72: B9 10 00 00 00     mov         ecx,10h
  0000000140394B77: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394B79: 4C 8D 84 24 48 01  lea         r8,[rsp+0000000000000148h]
                    00 00
  0000000140394B81: 48 8D 15 48 0E FA  lea         rdx,[00000001423359D0h]
                    01
  0000000140394B88: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  0000000140394B90: E8 EB 04 C9 FF     call        0000000140025080
  0000000140394B95: 48 8D BC 24 B0 03  lea         rdi,[rsp+00000000000003B0h]
                    00 00
  0000000140394B9D: 48 8B F0           mov         rsi,rax
  0000000140394BA0: B9 10 00 00 00     mov         ecx,10h
  0000000140394BA5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394BA7: 48 8D BC 24 28 01  lea         rdi,[rsp+0000000000000128h]
                    00 00
  0000000140394BAF: 48 8D B4 24 B0 03  lea         rsi,[rsp+00000000000003B0h]
                    00 00
  0000000140394BB7: B9 10 00 00 00     mov         ecx,10h
  0000000140394BBC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394BBE: 45 33 C0           xor         r8d,r8d
  0000000140394BC1: 33 D2              xor         edx,edx
  0000000140394BC3: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140394BCB: FF 15 97 F5 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394BD1: 48 8D BC 24 E0 02  lea         rdi,[rsp+00000000000002E0h]
                    00 00
  0000000140394BD9: 48 8B F0           mov         rsi,rax
  0000000140394BDC: B9 10 00 00 00     mov         ecx,10h
  0000000140394BE1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394BE3: 45 33 C0           xor         r8d,r8d
  0000000140394BE6: 33 D2              xor         edx,edx
  0000000140394BE8: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140394BF0: FF 15 72 F5 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394BF6: 48 8D BC 24 D0 02  lea         rdi,[rsp+00000000000002D0h]
                    00 00
  0000000140394BFE: 48 8B F0           mov         rsi,rax
  0000000140394C01: B9 10 00 00 00     mov         ecx,10h
  0000000140394C06: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394C08: 45 33 C0           xor         r8d,r8d
  0000000140394C0B: 33 D2              xor         edx,edx
  0000000140394C0D: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140394C15: FF 15 4D F5 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394C1B: 48 8D BC 24 C0 02  lea         rdi,[rsp+00000000000002C0h]
                    00 00
  0000000140394C23: 48 8B F0           mov         rsi,rax
  0000000140394C26: B9 10 00 00 00     mov         ecx,10h
  0000000140394C2B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394C2D: 45 33 C0           xor         r8d,r8d
  0000000140394C30: 33 D2              xor         edx,edx
  0000000140394C32: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140394C3A: FF 15 28 F5 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394C40: 48 8D BC 24 B0 02  lea         rdi,[rsp+00000000000002B0h]
                    00 00
  0000000140394C48: 48 8B F0           mov         rsi,rax
  0000000140394C4B: B9 10 00 00 00     mov         ecx,10h
  0000000140394C50: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394C52: 45 33 C0           xor         r8d,r8d
  0000000140394C55: 33 D2              xor         edx,edx
  0000000140394C57: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140394C5F: FF 15 03 F5 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394C65: 48 8D BC 24 A0 02  lea         rdi,[rsp+00000000000002A0h]
                    00 00
  0000000140394C6D: 48 8B F0           mov         rsi,rax
  0000000140394C70: B9 10 00 00 00     mov         ecx,10h
  0000000140394C75: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394C77: 45 33 C0           xor         r8d,r8d
  0000000140394C7A: 33 D2              xor         edx,edx
  0000000140394C7C: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140394C84: FF 15 DE F4 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394C8A: 48 8D BC 24 90 02  lea         rdi,[rsp+0000000000000290h]
                    00 00
  0000000140394C92: 48 8B F0           mov         rsi,rax
  0000000140394C95: B9 10 00 00 00     mov         ecx,10h
  0000000140394C9A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394C9C: 45 33 C0           xor         r8d,r8d
  0000000140394C9F: 33 D2              xor         edx,edx
  0000000140394CA1: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140394CA9: FF 15 B9 F4 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394CAF: 48 8D BC 24 80 02  lea         rdi,[rsp+0000000000000280h]
                    00 00
  0000000140394CB7: 48 8B F0           mov         rsi,rax
  0000000140394CBA: B9 10 00 00 00     mov         ecx,10h
  0000000140394CBF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394CC1: 45 33 C0           xor         r8d,r8d
  0000000140394CC4: 33 D2              xor         edx,edx
  0000000140394CC6: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000140394CCE: FF 15 94 F4 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394CD4: 48 8D BC 24 70 02  lea         rdi,[rsp+0000000000000270h]
                    00 00
  0000000140394CDC: 48 8B F0           mov         rsi,rax
  0000000140394CDF: B9 10 00 00 00     mov         ecx,10h
  0000000140394CE4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394CE6: 48 8D BC 24 60 02  lea         rdi,[rsp+0000000000000260h]
                    00 00
  0000000140394CEE: 48 8D B4 24 00 01  lea         rsi,[rsp+0000000000000100h]
                    00 00
  0000000140394CF6: B9 10 00 00 00     mov         ecx,10h
  0000000140394CFB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394CFD: 48 8D BC 24 50 02  lea         rdi,[rsp+0000000000000250h]
                    00 00
  0000000140394D05: 48 8D B4 24 28 01  lea         rsi,[rsp+0000000000000128h]
                    00 00
  0000000140394D0D: B9 10 00 00 00     mov         ecx,10h
  0000000140394D12: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394D14: 48 8D 84 24 E0 02  lea         rax,[rsp+00000000000002E0h]
                    00 00
  0000000140394D1C: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140394D21: 48 8D 84 24 D0 02  lea         rax,[rsp+00000000000002D0h]
                    00 00
  0000000140394D29: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140394D2E: 48 8D 84 24 C0 02  lea         rax,[rsp+00000000000002C0h]
                    00 00
  0000000140394D36: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140394D3B: 48 8D 84 24 B0 02  lea         rax,[rsp+00000000000002B0h]
                    00 00
  0000000140394D43: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140394D48: 48 8D 84 24 A0 02  lea         rax,[rsp+00000000000002A0h]
                    00 00
  0000000140394D50: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140394D55: 48 8D 84 24 90 02  lea         rax,[rsp+0000000000000290h]
                    00 00
  0000000140394D5D: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140394D62: 48 8D 84 24 80 02  lea         rax,[rsp+0000000000000280h]
                    00 00
  0000000140394D6A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140394D6F: 48 8D 84 24 70 02  lea         rax,[rsp+0000000000000270h]
                    00 00
  0000000140394D77: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140394D7C: 4C 8D 8C 24 60 02  lea         r9,[rsp+0000000000000260h]
                    00 00
  0000000140394D84: 4C 8D 84 24 50 02  lea         r8,[rsp+0000000000000250h]
                    00 00
  0000000140394D8C: 48 8D 15 4D 0C FA  lea         rdx,[00000001423359E0h]
                    01
  0000000140394D93: 48 8D 0D 66 19 9E  lea         rcx,[0000000143D76700h]
                    03
  0000000140394D9A: E8 21 A6 F3 FF     call        00000001402CF3C0
  0000000140394D9F: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394DA7: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140394DAA: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140394DB2: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  0000000140394DB8: 89 84 24 F0 01 00  mov         dword ptr [rsp+00000000000001F0h],eax
                    00
  0000000140394DBF: C7 84 24 18 02 00  mov         dword ptr [rsp+0000000000000218h],3F5h
                    00 F5 03 00 00
  0000000140394DCA: 4C 8D 84 24 F0 01  lea         r8,[rsp+00000000000001F0h]
                    00 00
  0000000140394DD2: 48 8D 15 53 D5 F9  lea         rdx,[000000014233232Ch]
                    01
  0000000140394DD9: 48 8D 8C 24 E0 01  lea         rcx,[rsp+00000000000001E0h]
                    00 00
  0000000140394DE1: E8 9A 02 C9 FF     call        0000000140025080
  0000000140394DE6: 48 8D BC 24 A0 03  lea         rdi,[rsp+00000000000003A0h]
                    00 00
  0000000140394DEE: 48 8B F0           mov         rsi,rax
  0000000140394DF1: B9 10 00 00 00     mov         ecx,10h
  0000000140394DF6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394DF8: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  0000000140394E00: 48 8D B4 24 A0 03  lea         rsi,[rsp+00000000000003A0h]
                    00 00
  0000000140394E08: B9 10 00 00 00     mov         ecx,10h
  0000000140394E0D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394E0F: 4C 8D 84 24 18 02  lea         r8,[rsp+0000000000000218h]
                    00 00
  0000000140394E17: 48 8D 15 DA 0B FA  lea         rdx,[00000001423359F8h]
                    01
  0000000140394E1E: 48 8D 8C 24 08 02  lea         rcx,[rsp+0000000000000208h]
                    00 00
  0000000140394E26: E8 55 02 C9 FF     call        0000000140025080
  0000000140394E2B: 48 8D BC 24 90 03  lea         rdi,[rsp+0000000000000390h]
                    00 00
  0000000140394E33: 48 8B F0           mov         rsi,rax
  0000000140394E36: B9 10 00 00 00     mov         ecx,10h
  0000000140394E3B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394E3D: 48 8D BC 24 F8 01  lea         rdi,[rsp+00000000000001F8h]
                    00 00
  0000000140394E45: 48 8D B4 24 90 03  lea         rsi,[rsp+0000000000000390h]
                    00 00
  0000000140394E4D: B9 10 00 00 00     mov         ecx,10h
  0000000140394E52: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394E54: 45 33 C0           xor         r8d,r8d
  0000000140394E57: 33 D2              xor         edx,edx
  0000000140394E59: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  0000000140394E61: FF 15 01 F3 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394E67: 48 8D BC 24 80 03  lea         rdi,[rsp+0000000000000380h]
                    00 00
  0000000140394E6F: 48 8B F0           mov         rsi,rax
  0000000140394E72: B9 10 00 00 00     mov         ecx,10h
  0000000140394E77: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394E79: 45 33 C0           xor         r8d,r8d
  0000000140394E7C: 33 D2              xor         edx,edx
  0000000140394E7E: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  0000000140394E86: FF 15 DC F2 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394E8C: 48 8D BC 24 70 03  lea         rdi,[rsp+0000000000000370h]
                    00 00
  0000000140394E94: 48 8B F0           mov         rsi,rax
  0000000140394E97: B9 10 00 00 00     mov         ecx,10h
  0000000140394E9C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394E9E: 45 33 C0           xor         r8d,r8d
  0000000140394EA1: 33 D2              xor         edx,edx
  0000000140394EA3: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  0000000140394EAB: FF 15 B7 F2 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394EB1: 48 8D BC 24 60 03  lea         rdi,[rsp+0000000000000360h]
                    00 00
  0000000140394EB9: 48 8B F0           mov         rsi,rax
  0000000140394EBC: B9 10 00 00 00     mov         ecx,10h
  0000000140394EC1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394EC3: 45 33 C0           xor         r8d,r8d
  0000000140394EC6: 33 D2              xor         edx,edx
  0000000140394EC8: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  0000000140394ED0: FF 15 92 F2 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394ED6: 48 8D BC 24 50 03  lea         rdi,[rsp+0000000000000350h]
                    00 00
  0000000140394EDE: 48 8B F0           mov         rsi,rax
  0000000140394EE1: B9 10 00 00 00     mov         ecx,10h
  0000000140394EE6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394EE8: 45 33 C0           xor         r8d,r8d
  0000000140394EEB: 33 D2              xor         edx,edx
  0000000140394EED: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  0000000140394EF5: FF 15 6D F2 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394EFB: 48 8D BC 24 40 03  lea         rdi,[rsp+0000000000000340h]
                    00 00
  0000000140394F03: 48 8B F0           mov         rsi,rax
  0000000140394F06: B9 10 00 00 00     mov         ecx,10h
  0000000140394F0B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394F0D: 45 33 C0           xor         r8d,r8d
  0000000140394F10: 33 D2              xor         edx,edx
  0000000140394F12: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  0000000140394F1A: FF 15 48 F2 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394F20: 48 8D BC 24 30 03  lea         rdi,[rsp+0000000000000330h]
                    00 00
  0000000140394F28: 48 8B F0           mov         rsi,rax
  0000000140394F2B: B9 10 00 00 00     mov         ecx,10h
  0000000140394F30: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394F32: 45 33 C0           xor         r8d,r8d
  0000000140394F35: 33 D2              xor         edx,edx
  0000000140394F37: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  0000000140394F3F: FF 15 23 F2 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394F45: 48 8D BC 24 20 03  lea         rdi,[rsp+0000000000000320h]
                    00 00
  0000000140394F4D: 48 8B F0           mov         rsi,rax
  0000000140394F50: B9 10 00 00 00     mov         ecx,10h
  0000000140394F55: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394F57: 45 33 C0           xor         r8d,r8d
  0000000140394F5A: 33 D2              xor         edx,edx
  0000000140394F5C: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  0000000140394F64: FF 15 FE F1 A6 01  call        qword ptr [0000000141E04168h]
  0000000140394F6A: 48 8D BC 24 10 03  lea         rdi,[rsp+0000000000000310h]
                    00 00
  0000000140394F72: 48 8B F0           mov         rsi,rax
  0000000140394F75: B9 10 00 00 00     mov         ecx,10h
  0000000140394F7A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394F7C: 48 8D BC 24 00 03  lea         rdi,[rsp+0000000000000300h]
                    00 00
  0000000140394F84: 48 8D B4 24 D0 01  lea         rsi,[rsp+00000000000001D0h]
                    00 00
  0000000140394F8C: B9 10 00 00 00     mov         ecx,10h
  0000000140394F91: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394F93: 48 8D BC 24 F0 02  lea         rdi,[rsp+00000000000002F0h]
                    00 00
  0000000140394F9B: 48 8D B4 24 F8 01  lea         rsi,[rsp+00000000000001F8h]
                    00 00
  0000000140394FA3: B9 10 00 00 00     mov         ecx,10h
  0000000140394FA8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140394FAA: 48 8D 84 24 80 03  lea         rax,[rsp+0000000000000380h]
                    00 00
  0000000140394FB2: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140394FB7: 48 8D 84 24 70 03  lea         rax,[rsp+0000000000000370h]
                    00 00
  0000000140394FBF: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140394FC4: 48 8D 84 24 60 03  lea         rax,[rsp+0000000000000360h]
                    00 00
  0000000140394FCC: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140394FD1: 48 8D 84 24 50 03  lea         rax,[rsp+0000000000000350h]
                    00 00
  0000000140394FD9: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140394FDE: 48 8D 84 24 40 03  lea         rax,[rsp+0000000000000340h]
                    00 00
  0000000140394FE6: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140394FEB: 48 8D 84 24 30 03  lea         rax,[rsp+0000000000000330h]
                    00 00
  0000000140394FF3: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140394FF8: 48 8D 84 24 20 03  lea         rax,[rsp+0000000000000320h]
                    00 00
  0000000140395000: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140395005: 48 8D 84 24 10 03  lea         rax,[rsp+0000000000000310h]
                    00 00
  000000014039500D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140395012: 4C 8D 8C 24 00 03  lea         r9,[rsp+0000000000000300h]
                    00 00
  000000014039501A: 4C 8D 84 24 F0 02  lea         r8,[rsp+00000000000002F0h]
                    00 00
  0000000140395022: 48 8D 15 DF 09 FA  lea         rdx,[0000000142335A08h]
                    01
  0000000140395029: 48 8D 0D D0 16 9E  lea         rcx,[0000000143D76700h]
                    03
  0000000140395030: E8 8B A3 F3 FF     call        00000001402CF3C0
  0000000140395035: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  000000014039503D: E8 EE 47 FF FF     call        0000000140389830
  0000000140395042: 0F B6 44 24 69     movzx       eax,byte ptr [rsp+69h]
  0000000140395047: 85 C0              test        eax,eax
  0000000140395049: 0F 84 AB 01 00 00  je          00000001403951FA
  000000014039504F: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140395057: 48 8B 00           mov         rax,qword ptr [rax]
  000000014039505A: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140395062: FF 90 A8 01 00 00  call        qword ptr [rax+00000000000001A8h]
  0000000140395068: 48 8B D0           mov         rdx,rax
  000000014039506B: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140395070: E8 CB DD 8E 00     call        0000000140C82E40
  0000000140395075: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000014039507A: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140395080: 0F 84 72 01 00 00  je          00000001403951F8
  0000000140395086: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014039508B: E8 80 C9 E5 00     call        00000001411F1A10
  0000000140395090: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140395095: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  000000014039509B: 0F 84 57 01 00 00  je          00000001403951F8
  00000001403950A1: 41 B1 01           mov         r9b,1
  00000001403950A4: 45 33 C0           xor         r8d,r8d
  00000001403950A7: BA 03 00 00 00     mov         edx,3
  00000001403950AC: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001403950B1: E8 3A 9A E5 00     call        00000001411EEAF0
  00000001403950B6: 41 B1 01           mov         r9b,1
  00000001403950B9: 45 33 C0           xor         r8d,r8d
  00000001403950BC: BA 02 00 00 00     mov         edx,2
  00000001403950C1: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001403950C6: E8 25 9A E5 00     call        00000001411EEAF0
  00000001403950CB: 41 B1 01           mov         r9b,1
  00000001403950CE: 41 B0 01           mov         r8b,1
  00000001403950D1: BA 2B 00 00 00     mov         edx,2Bh
  00000001403950D6: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001403950DB: E8 10 9A E5 00     call        00000001411EEAF0
  00000001403950E0: 41 B1 01           mov         r9b,1
  00000001403950E3: 45 33 C0           xor         r8d,r8d
  00000001403950E6: BA 2C 00 00 00     mov         edx,2Ch
  00000001403950EB: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001403950F0: E8 FB 99 E5 00     call        00000001411EEAF0
  00000001403950F5: 41 B1 01           mov         r9b,1
  00000001403950F8: 45 33 C0           xor         r8d,r8d
  00000001403950FB: BA 2D 00 00 00     mov         edx,2Dh
  0000000140395100: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140395105: E8 E6 99 E5 00     call        00000001411EEAF0
  000000014039510A: 48 8B 15 7F F0 A6  mov         rdx,qword ptr [0000000141E04190h]
                    01
  0000000140395111: 48 8D 8C 24 20 02  lea         rcx,[rsp+0000000000000220h]
                    00 00
  0000000140395119: FF 15 79 F0 A6 01  call        qword ptr [0000000141E04198h]
  000000014039511F: 90                 nop
  0000000140395120: 48 8D 15 F9 08 FA  lea         rdx,[0000000142335A20h]
                    01
  0000000140395127: 48 8D 8C 24 30 02  lea         rcx,[rsp+0000000000000230h]
                    00 00
  000000014039512F: FF 15 DB EF A6 01  call        qword ptr [0000000141E04110h]
  0000000140395135: 48 8B D0           mov         rdx,rax
  0000000140395138: 48 8D 8C 24 28 02  lea         rcx,[rsp+0000000000000228h]
                    00 00
  0000000140395140: FF 15 D2 EF A6 01  call        qword ptr [0000000141E04118h]
  0000000140395146: 90                 nop
  0000000140395147: 4C 8D 84 24 20 02  lea         r8,[rsp+0000000000000220h]
                    00 00
  000000014039514F: 48 8D 94 24 28 02  lea         rdx,[rsp+0000000000000228h]
                    00 00
  0000000140395157: 48 8D 0D A2 15 9E  lea         rcx,[0000000143D76700h]
                    03
  000000014039515E: E8 3D 97 F3 FF     call        00000001402CE8A0
  0000000140395163: 90                 nop
  0000000140395164: 48 8D 8C 24 28 02  lea         rcx,[rsp+0000000000000228h]
                    00 00
  000000014039516C: FF 15 6E 08 A7 01  call        qword ptr [0000000141E059E0h]
  0000000140395172: 90                 nop
  0000000140395173: 48 8D 8C 24 20 02  lea         rcx,[rsp+0000000000000220h]
                    00 00
  000000014039517B: FF 15 5F 08 A7 01  call        qword ptr [0000000141E059E0h]
  0000000140395181: 48 8B 15 08 F0 A6  mov         rdx,qword ptr [0000000141E04190h]
                    01
  0000000140395188: 48 8D 8C 24 38 02  lea         rcx,[rsp+0000000000000238h]
                    00 00
  0000000140395190: FF 15 02 F0 A6 01  call        qword ptr [0000000141E04198h]
  0000000140395196: 90                 nop
  0000000140395197: 48 8D 15 92 08 FA  lea         rdx,[0000000142335A30h]
                    01
  000000014039519E: 48 8D 8C 24 48 02  lea         rcx,[rsp+0000000000000248h]
                    00 00
  00000001403951A6: FF 15 64 EF A6 01  call        qword ptr [0000000141E04110h]
  00000001403951AC: 48 8B D0           mov         rdx,rax
  00000001403951AF: 48 8D 8C 24 40 02  lea         rcx,[rsp+0000000000000240h]
                    00 00
  00000001403951B7: FF 15 5B EF A6 01  call        qword ptr [0000000141E04118h]
  00000001403951BD: 90                 nop
  00000001403951BE: 4C 8D 84 24 38 02  lea         r8,[rsp+0000000000000238h]
                    00 00
  00000001403951C6: 48 8D 94 24 40 02  lea         rdx,[rsp+0000000000000240h]
                    00 00
  00000001403951CE: 48 8D 0D 2B 15 9E  lea         rcx,[0000000143D76700h]
                    03
  00000001403951D5: E8 C6 96 F3 FF     call        00000001402CE8A0
  00000001403951DA: 90                 nop
  00000001403951DB: 48 8D 8C 24 40 02  lea         rcx,[rsp+0000000000000240h]
                    00 00
  00000001403951E3: FF 15 F7 07 A7 01  call        qword ptr [0000000141E059E0h]
  00000001403951E9: 90                 nop
  00000001403951EA: 48 8D 8C 24 38 02  lea         rcx,[rsp+0000000000000238h]
                    00 00
  00000001403951F2: FF 15 E8 07 A7 01  call        qword ptr [0000000141E059E0h]
  00000001403951F8: EB 29              jmp         0000000140395223
  00000001403951FA: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140395202: 83 78 48 03        cmp         dword ptr [rax+48h],3
  0000000140395206: 75 1B              jne         0000000140395223
  0000000140395208: 48 8B 84 24 00 04  mov         rax,qword ptr [rsp+0000000000000400h]
                    00 00
  0000000140395210: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140395213: 33 D2              xor         edx,edx
  0000000140395215: 48 8B 8C 24 00 04  mov         rcx,qword ptr [rsp+0000000000000400h]
                    00 00
  000000014039521D: FF 90 F0 01 00 00  call        qword ptr [rax+00000000000001F0h]
  0000000140395223: 48 8D 0D 8E 14 9E  lea         rcx,[0000000143D766B8h]
                    03
  000000014039522A: E8 61 56 F3 FF     call        00000001402CA890
  000000014039522F: 48 81 C4 E8 03 00  add         rsp,3E8h
                    00
  0000000140395236: 5F                 pop         rdi
  0000000140395237: 5E                 pop         rsi
  0000000140395238: C3                 ret

  Summary

     1E02000 .text
