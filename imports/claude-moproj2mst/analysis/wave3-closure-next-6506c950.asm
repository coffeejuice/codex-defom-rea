Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006506C950: 40 53              push        rbx
  000000006506C952: 55                 push        rbp
  000000006506C953: 56                 push        rsi
  000000006506C954: 57                 push        rdi
  000000006506C955: 41 54              push        r12
  000000006506C957: 48 81 EC 50 01 00  sub         rsp,150h
                    00
  000000006506C95E: 48 C7 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000006506C96A: 48 8B F2           mov         rsi,rdx
  000000006506C96D: 48 8B F9           mov         rdi,rcx
  000000006506C970: 48 8B 69 08        mov         rbp,qword ptr [rcx+8]
  000000006506C974: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006506C978: 44 0F B6 40 08     movzx       r8d,byte ptr [rax+8]
  000000006506C97D: 48 8D 1D 7C 36 F9  lea         rbx,[0000000065000000h]
                    FF
  000000006506C984: 41 80 E0 01        and         r8b,1
  000000006506C988: 74 30              je          000000006506C9BA
  000000006506C98A: 48 8B CA           mov         rcx,rdx
  000000006506C98D: FF 15 D5 A0 62 00  call        qword ptr [0000000065696A68h]
  000000006506C993: 83 C0 FE           add         eax,0FFFFFFFEh
  000000006506C996: 3D C2 00 00 00     cmp         eax,0C2h
  000000006506C99B: 77 1D              ja          000000006506C9BA
  000000006506C99D: 48 98              cdqe
  000000006506C99F: 0F B6 84 03 8C D7  movzx       eax,byte ptr [rbx+rax+000000000006D78Ch]
                    06 00
  000000006506C9A7: 8B 8C 83 84 D7 06  mov         ecx,dword ptr [rbx+rax*4+000000000006D784h]
                    00
  000000006506C9AE: 48 03 CB           add         rcx,rbx
  000000006506C9B1: FF E1              jmp         rcx
  000000006506C9B3: 32 C0              xor         al,al
  000000006506C9B5: E9 B9 0D 00 00     jmp         000000006506D773
  000000006506C9BA: 48 8B CE           mov         rcx,rsi
  000000006506C9BD: FF 15 A5 A0 62 00  call        qword ptr [0000000065696A68h]
  000000006506C9C3: 83 C0 FE           add         eax,0FFFFFFFEh
  000000006506C9C6: 3D C4 00 00 00     cmp         eax,0C4h
  000000006506C9CB: 0F 87 96 0D 00 00  ja          000000006506D767
  000000006506C9D1: 48 98              cdqe
  000000006506C9D3: 0F B6 84 03 24 D9  movzx       eax,byte ptr [rbx+rax+000000000006D924h]
                    06 00
  000000006506C9DB: 8B 8C 83 50 D8 06  mov         ecx,dword ptr [rbx+rax*4+000000000006D850h]
                    00
  000000006506C9E2: 48 03 CB           add         rcx,rbx
  000000006506C9E5: FF E1              jmp         rcx
  000000006506C9E7: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506C9EA: 48 8B D6           mov         rdx,rsi
  000000006506C9ED: 48 8B CF           mov         rcx,rdi
  000000006506C9F0: FF 90 90 00 00 00  call        qword ptr [rax+0000000000000090h]
  000000006506C9F6: E9 66 0C 00 00     jmp         000000006506D661
  000000006506C9FB: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506C9FE: 48 8B D6           mov         rdx,rsi
  000000006506CA01: 48 8B CF           mov         rcx,rdi
  000000006506CA04: FF 50 78           call        qword ptr [rax+78h]
  000000006506CA07: E9 55 0C 00 00     jmp         000000006506D661
  000000006506CA0C: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CA0F: 48 8B D6           mov         rdx,rsi
  000000006506CA12: 48 8B CF           mov         rcx,rdi
  000000006506CA15: FF 90 80 00 00 00  call        qword ptr [rax+0000000000000080h]
  000000006506CA1B: E9 41 0C 00 00     jmp         000000006506D661
  000000006506CA20: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CA23: 48 8B D6           mov         rdx,rsi
  000000006506CA26: 48 8B CF           mov         rcx,rdi
  000000006506CA29: FF 90 88 00 00 00  call        qword ptr [rax+0000000000000088h]
  000000006506CA2F: E9 2D 0C 00 00     jmp         000000006506D661
  000000006506CA34: E8 E7 3B FB FF     call        0000000065020620
  000000006506CA39: 48 8B D8           mov         rbx,rax
  000000006506CA3C: 48 85 C0           test        rax,rax
  000000006506CA3F: 0F 84 1C 0C 00 00  je          000000006506D661
  000000006506CA45: 48 3B DF           cmp         rbx,rdi
  000000006506CA48: 0F 84 13 0C 00 00  je          000000006506D661
  000000006506CA4E: BA 01 00 00 00     mov         edx,1
  000000006506CA53: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006506CA57: E8 74 FB FF FF     call        000000006506C5D0
  000000006506CA5C: E8 BF 3B FB FF     call        0000000065020620
  000000006506CA61: 48 3B C3           cmp         rax,rbx
  000000006506CA64: 75 0B              jne         000000006506CA71
  000000006506CA66: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006506CA69: 33 D2              xor         edx,edx
  000000006506CA6B: 48 8B CB           mov         rcx,rbx
  000000006506CA6E: FF 50 58           call        qword ptr [rax+58h]
  000000006506CA71: E8 AA 3B FB FF     call        0000000065020620
  000000006506CA76: 48 8B D8           mov         rbx,rax
  000000006506CA79: 48 85 C0           test        rax,rax
  000000006506CA7C: 75 C7              jne         000000006506CA45
  000000006506CA7E: E9 DE 0B 00 00     jmp         000000006506D661
  000000006506CA83: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CA86: 48 8B D6           mov         rdx,rsi
  000000006506CA89: 48 8B CF           mov         rcx,rdi
  000000006506CA8C: FF 90 98 00 00 00  call        qword ptr [rax+0000000000000098h]
  000000006506CA92: E9 CA 0B 00 00     jmp         000000006506D661
  000000006506CA97: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CA9A: 48 8B D6           mov         rdx,rsi
  000000006506CA9D: 48 8B CF           mov         rcx,rdi
  000000006506CAA0: FF 90 F8 00 00 00  call        qword ptr [rax+00000000000000F8h]
  000000006506CAA6: E9 B6 0B 00 00     jmp         000000006506D661
  000000006506CAAB: 48 8B CE           mov         rcx,rsi
  000000006506CAAE: FF 15 E4 A2 62 00  call        qword ptr [0000000065696D98h]
  000000006506CAB4: 32 C0              xor         al,al
  000000006506CAB6: E9 B8 0C 00 00     jmp         000000006506D773
  000000006506CABB: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000006506CAC3: 48 8B CE           mov         rcx,rsi
  000000006506CAC6: E8 75 7F FC FF     call        0000000065034A40
  000000006506CACB: 8B 08              mov         ecx,dword ptr [rax]
  000000006506CACD: 81 E1 00 00 00 0C  and         ecx,0C000000h
  000000006506CAD3: 75 57              jne         000000006506CB2C
  000000006506CAD5: 8B 46 28           mov         eax,dword ptr [rsi+28h]
  000000006506CAD8: 3D 02 00 00 01     cmp         eax,1000002h
  000000006506CADD: 74 37              je          000000006506CB16
  000000006506CADF: 3D 01 00 00 01     cmp         eax,1000001h
  000000006506CAE4: 75 17              jne         000000006506CAFD
  000000006506CAE6: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  000000006506CAEB: 48 8B CE           mov         rcx,rsi
  000000006506CAEE: E8 4D 7F FC FF     call        0000000065034A40
  000000006506CAF3: 8B 08              mov         ecx,dword ptr [rax]
  000000006506CAF5: 81 E1 00 00 00 02  and         ecx,2000000h
  000000006506CAFB: 75 19              jne         000000006506CB16
  000000006506CAFD: 81 7E 28 01 00 00  cmp         dword ptr [rsi+28h],1000001h
                    01
  000000006506CB04: 75 26              jne         000000006506CB2C
  000000006506CB06: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CB09: B2 01              mov         dl,1
  000000006506CB0B: 48 8B CF           mov         rcx,rdi
  000000006506CB0E: FF 90 58 01 00 00  call        qword ptr [rax+0000000000000158h]
  000000006506CB14: EB 0E              jmp         000000006506CB24
  000000006506CB16: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CB19: 33 D2              xor         edx,edx
  000000006506CB1B: 48 8B CF           mov         rcx,rdi
  000000006506CB1E: FF 90 58 01 00 00  call        qword ptr [rax+0000000000000158h]
  000000006506CB24: 84 C0              test        al,al
  000000006506CB26: 0F 85 35 0B 00 00  jne         000000006506D661
  000000006506CB2C: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CB2F: 48 8B D6           mov         rdx,rsi
  000000006506CB32: 48 8B CF           mov         rcx,rdi
  000000006506CB35: FF 90 A0 00 00 00  call        qword ptr [rax+00000000000000A0h]
  000000006506CB3B: 48 8B CE           mov         rcx,rsi
  000000006506CB3E: FF 15 4C A2 62 00  call        qword ptr [0000000065696D90h]
  000000006506CB44: 84 C0              test        al,al
  000000006506CB46: 0F 85 15 0B 00 00  jne         000000006506D661
  000000006506CB4C: 48 8D 54 24 7C     lea         rdx,[rsp+7Ch]
  000000006506CB51: 48 8B CE           mov         rcx,rsi
  000000006506CB54: E8 E7 7E FC FF     call        0000000065034A40
  000000006506CB59: 8B 08              mov         ecx,dword ptr [rax]
  000000006506CB5B: 81 E1 00 00 00 02  and         ecx,2000000h
  000000006506CB61: 0F 84 FA 0A 00 00  je          000000006506D661
  000000006506CB67: 81 7E 28 30 00 00  cmp         dword ptr [rsi+28h],1000030h
                    01
  000000006506CB6E: 0F 85 ED 0A 00 00  jne         000000006506D661
  000000006506CB74: 48 8D 8D 10 01 00  lea         rcx,[rbp+0000000000000110h]
                    00
  000000006506CB7B: FF 15 0F 9D 62 00  call        qword ptr [0000000065696890h]
  000000006506CB81: 85 C0              test        eax,eax
  000000006506CB83: 0F 84 D8 0A 00 00  je          000000006506D661
  000000006506CB89: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CB8C: 45 33 C0           xor         r8d,r8d
  000000006506CB8F: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  000000006506CB97: 48 8B CF           mov         rcx,rdi
  000000006506CB9A: FF 90 50 01 00 00  call        qword ptr [rax+0000000000000150h]
  000000006506CBA0: 90                 nop
  000000006506CBA1: 48 8D 94 24 18 01  lea         rdx,[rsp+0000000000000118h]
                    00 00
  000000006506CBA9: 48 8B C8           mov         rcx,rax
  000000006506CBAC: FF 15 06 A9 62 00  call        qword ptr [00000000656974B8h]
  000000006506CBB2: 48 8B C8           mov         rcx,rax
  000000006506CBB5: 48 8D 94 24 08 01  lea         rdx,[rsp+0000000000000108h]
                    00 00
  000000006506CBBD: FF 15 ED A8 62 00  call        qword ptr [00000000656974B0h]
  000000006506CBC3: 4C 8B C0           mov         r8,rax
  000000006506CBC6: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  000000006506CBCE: 48 8B CF           mov         rcx,rdi
  000000006506CBD1: E8 8A B2 02 00     call        0000000065097E60
  000000006506CBD6: 48 8B C8           mov         rcx,rax
  000000006506CBD9: 4C 8B C7           mov         r8,rdi
  000000006506CBDC: 48 8D 95 10 01 00  lea         rdx,[rbp+0000000000000110h]
                    00
  000000006506CBE3: E8 58 E0 FE FF     call        000000006505AC40
  000000006506CBE8: 90                 nop
  000000006506CBE9: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  000000006506CBF1: FF 15 E9 9A 62 00  call        qword ptr [00000000656966E0h]
  000000006506CBF7: 48 8B CE           mov         rcx,rsi
  000000006506CBFA: FF 15 A0 A1 62 00  call        qword ptr [0000000065696DA0h]
  000000006506CC00: E9 5C 0A 00 00     jmp         000000006506D661
  000000006506CC05: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CC08: 48 8B D6           mov         rdx,rsi
  000000006506CC0B: 48 8B CF           mov         rcx,rdi
  000000006506CC0E: FF 90 A8 00 00 00  call        qword ptr [rax+00000000000000A8h]
  000000006506CC14: E9 48 0A 00 00     jmp         000000006506D661
  000000006506CC19: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CC1C: 48 8B D6           mov         rdx,rsi
  000000006506CC1F: 48 8B CF           mov         rcx,rdi
  000000006506CC22: FF 90 48 01 00 00  call        qword ptr [rax+0000000000000148h]
  000000006506CC28: E9 34 0A 00 00     jmp         000000006506D661
  000000006506CC2D: 48 8B CF           mov         rcx,rdi
  000000006506CC30: E8 1B 83 FF FF     call        0000000065064F50
  000000006506CC35: E9 27 0A 00 00     jmp         000000006506D661
  000000006506CC3A: 48 8B CF           mov         rcx,rdi
  000000006506CC3D: E8 1E 0A FF FF     call        000000006505D660
  000000006506CC42: 4C 8B 00           mov         r8,qword ptr [rax]
  000000006506CC45: 48 8B D7           mov         rdx,rdi
  000000006506CC48: 48 8B C8           mov         rcx,rax
  000000006506CC4B: 41 FF 50 68        call        qword ptr [r8+68h]
  000000006506CC4F: 41 B0 01           mov         r8b,1
  000000006506CC52: BA 42 00 00 00     mov         edx,42h
  000000006506CC57: 48 8B CF           mov         rcx,rdi
  000000006506CC5A: E8 81 C3 FF FF     call        0000000065068FE0
  000000006506CC5F: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006506CC67: E8 C4 E6 FA FF     call        000000006501B330
  000000006506CC6C: 48 8B D8           mov         rbx,rax
  000000006506CC6F: 48 8B D7           mov         rdx,rdi
  000000006506CC72: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000006506CC7A: E8 51 48 FB FF     call        00000000650214D0
  000000006506CC7F: 90                 nop
  000000006506CC80: 48 8B D3           mov         rdx,rbx
  000000006506CC83: 48 8B C8           mov         rcx,rax
  000000006506CC86: E8 C5 8B 19 00     call        0000000065205850
  000000006506CC8B: 84 C0              test        al,al
  000000006506CC8D: 0F 94 C3           sete        bl
  000000006506CC90: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000006506CC98: E8 83 9E 19 00     call        0000000065206B20
  000000006506CC9D: 90                 nop
  000000006506CC9E: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006506CCA6: E8 75 9E 19 00     call        0000000065206B20
  000000006506CCAB: 84 DB              test        bl,bl
  000000006506CCAD: 74 08              je          000000006506CCB7
  000000006506CCAF: 48 8B CD           mov         rcx,rbp
  000000006506CCB2: E8 19 45 FF FF     call        00000000650611D0
  000000006506CCB7: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  000000006506CCBF: E8 AC A2 FA FF     call        0000000065016F70
  000000006506CCC4: 48 8B D8           mov         rbx,rax
  000000006506CCC7: 48 8B D7           mov         rdx,rdi
  000000006506CCCA: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000006506CCD2: E8 09 3F FB FF     call        0000000065020BE0
  000000006506CCD7: 90                 nop
  000000006506CCD8: 48 8B D3           mov         rdx,rbx
  000000006506CCDB: 48 8B C8           mov         rcx,rax
  000000006506CCDE: E8 6D 8B 19 00     call        0000000065205850
  000000006506CCE3: 84 C0              test        al,al
  000000006506CCE5: 0F 94 C3           sete        bl
  000000006506CCE8: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000006506CCF0: E8 DB D4 FD FF     call        000000006504A1D0
  000000006506CCF5: 90                 nop
  000000006506CCF6: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  000000006506CCFE: E8 CD D4 FD FF     call        000000006504A1D0
  000000006506CD03: 84 DB              test        bl,bl
  000000006506CD05: 74 08              je          000000006506CD0F
  000000006506CD07: 48 8B CD           mov         rcx,rbp
  000000006506CD0A: E8 71 B3 FF FF     call        0000000065068080
  000000006506CD0F: F6 45 20 20        test        byte ptr [rbp+20h],20h
  000000006506CD13: 0F 84 48 09 00 00  je          000000006506D661
  000000006506CD19: BA 46 00 00 00     mov         edx,46h
  000000006506CD1E: 48 8B CF           mov         rcx,rdi
  000000006506CD21: FF 15 19 9B 62 00  call        qword ptr [0000000065696840h]
  000000006506CD27: E9 35 09 00 00     jmp         000000006506D661
  000000006506CD2C: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006506CD30: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  000000006506CD34: 0F 84 27 09 00 00  je          000000006506D661
  000000006506CD3A: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006506CD3E: 0F BA A0 C8 01 00  bt          dword ptr [rax+00000000000001C8h],15h
                    00 15
  000000006506CD46: 0F 82 15 09 00 00  jb          000000006506D661
  000000006506CD4C: 33 D2              xor         edx,edx
  000000006506CD4E: 48 8B CD           mov         rcx,rbp
  000000006506CD51: E8 FA A8 02 00     call        0000000065097650
  000000006506CD56: 48 8B CD           mov         rcx,rbp
  000000006506CD59: E8 52 FE FE FF     call        000000006505CBB0
  000000006506CD5E: E9 FE 08 00 00     jmp         000000006506D661
  000000006506CD63: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CD66: 48 8B D6           mov         rdx,rsi
  000000006506CD69: 48 8B CF           mov         rcx,rdi
  000000006506CD6C: FF 90 B0 00 00 00  call        qword ptr [rax+00000000000000B0h]
  000000006506CD72: E9 EA 08 00 00     jmp         000000006506D661
  000000006506CD77: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CD7A: 48 8B D6           mov         rdx,rsi
  000000006506CD7D: 48 8B CF           mov         rcx,rdi
  000000006506CD80: FF 90 B8 00 00 00  call        qword ptr [rax+00000000000000B8h]
  000000006506CD86: E9 D6 08 00 00     jmp         000000006506D661
  000000006506CD8B: 48 8D 8D 08 01 00  lea         rcx,[rbp+0000000000000108h]
                    00
  000000006506CD92: FF 15 F8 9A 62 00  call        qword ptr [0000000065696890h]
  000000006506CD98: 85 C0              test        eax,eax
  000000006506CD9A: 74 2B              je          000000006506CDC7
  000000006506CD9C: 48 8D 95 08 01 00  lea         rdx,[rbp+0000000000000108h]
                    00
  000000006506CDA3: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506CDA8: E8 F3 56 FC FF     call        00000000650324A0
  000000006506CDAD: 90                 nop
  000000006506CDAE: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006506CDB3: 48 8B CF           mov         rcx,rdi
  000000006506CDB6: FF 15 54 9A 62 00  call        qword ptr [0000000065696810h]
  000000006506CDBC: 90                 nop
  000000006506CDBD: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506CDC2: E8 29 57 FC FF     call        00000000650324F0
  000000006506CDC7: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CDCA: 48 8B D6           mov         rdx,rsi
  000000006506CDCD: 48 8B CF           mov         rcx,rdi
  000000006506CDD0: FF 90 C0 00 00 00  call        qword ptr [rax+00000000000000C0h]
  000000006506CDD6: E9 86 08 00 00     jmp         000000006506D661
  000000006506CDDB: 48 8D 8D 08 01 00  lea         rcx,[rbp+0000000000000108h]
                    00
  000000006506CDE2: FF 15 A8 9A 62 00  call        qword ptr [0000000065696890h]
  000000006506CDE8: 85 C0              test        eax,eax
  000000006506CDEA: 74 4A              je          000000006506CE36
  000000006506CDEC: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506CDF4: FF 15 6E 99 62 00  call        qword ptr [0000000065696768h]
  000000006506CDFA: 90                 nop
  000000006506CDFB: 48 8D 94 24 80 01  lea         rdx,[rsp+0000000000000180h]
                    00 00
  000000006506CE03: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506CE08: E8 93 56 FC FF     call        00000000650324A0
  000000006506CE0D: 90                 nop
  000000006506CE0E: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006506CE13: 48 8B CF           mov         rcx,rdi
  000000006506CE16: FF 15 F4 99 62 00  call        qword ptr [0000000065696810h]
  000000006506CE1C: 90                 nop
  000000006506CE1D: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506CE22: E8 C9 56 FC FF     call        00000000650324F0
  000000006506CE27: 90                 nop
  000000006506CE28: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506CE30: FF 15 2A 99 62 00  call        qword ptr [0000000065696760h]
  000000006506CE36: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CE39: 48 8B D6           mov         rdx,rsi
  000000006506CE3C: 48 8B CF           mov         rcx,rdi
  000000006506CE3F: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  000000006506CE45: E9 17 08 00 00     jmp         000000006506D661
  000000006506CE4A: 48 8B CF           mov         rcx,rdi
  000000006506CE4D: E8 5E 84 FF FF     call        00000000650652B0
  000000006506CE52: E9 0A 08 00 00     jmp         000000006506D661
  000000006506CE57: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CE5A: 48 8B D6           mov         rdx,rsi
  000000006506CE5D: 48 8B CF           mov         rcx,rdi
  000000006506CE60: FF 90 D0 00 00 00  call        qword ptr [rax+00000000000000D0h]
  000000006506CE66: E9 F6 07 00 00     jmp         000000006506D661
  000000006506CE6B: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CE6E: 48 8B D6           mov         rdx,rsi
  000000006506CE71: 48 8B CF           mov         rcx,rdi
  000000006506CE74: FF 90 D8 00 00 00  call        qword ptr [rax+00000000000000D8h]
  000000006506CE7A: E9 E2 07 00 00     jmp         000000006506D661
  000000006506CE7F: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CE82: 48 8B D6           mov         rdx,rsi
  000000006506CE85: 48 8B CF           mov         rcx,rdi
  000000006506CE88: FF 90 E0 00 00 00  call        qword ptr [rax+00000000000000E0h]
  000000006506CE8E: E9 CE 07 00 00     jmp         000000006506D661
  000000006506CE93: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CE96: 48 8B D6           mov         rdx,rsi
  000000006506CE99: 48 8B CF           mov         rcx,rdi
  000000006506CE9C: FF 90 E8 00 00 00  call        qword ptr [rax+00000000000000E8h]
  000000006506CEA2: E9 BA 07 00 00     jmp         000000006506D661
  000000006506CEA7: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006506CEAB: 8B 48 10           mov         ecx,dword ptr [rax+10h]
  000000006506CEAE: C1 E9 0D           shr         ecx,0Dh
  000000006506CEB1: 83 E1 07           and         ecx,7
  000000006506CEB4: 83 E9 01           sub         ecx,1
  000000006506CEB7: 0F 84 8C 00 00 00  je          000000006506CF49
  000000006506CEBD: 83 E9 01           sub         ecx,1
  000000006506CEC0: 74 3D              je          000000006506CEFF
  000000006506CEC2: 83 E9 01           sub         ecx,1
  000000006506CEC5: 74 0B              je          000000006506CED2
  000000006506CEC7: 83 F9 01           cmp         ecx,1
  000000006506CECA: 0F 84 91 07 00 00  je          000000006506D661
  000000006506CED0: EB 69              jmp         000000006506CF3B
  000000006506CED2: 33 DB              xor         ebx,ebx
  000000006506CED4: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006506CED9: 48 8D 46 20        lea         rax,[rsi+20h]
  000000006506CEDD: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000006506CEE2: 4C 8D 4C 24 40     lea         r9,[rsp+40h]
  000000006506CEE7: 45 33 C0           xor         r8d,r8d
  000000006506CEEA: 48 8D 15 5F 08 95  lea         rdx,[00000000659BD750h]
                    00
  000000006506CEF1: 48 8B CF           mov         rcx,rdi
  000000006506CEF4: FF 15 B6 99 62 00  call        qword ptr [00000000656968B0h]
  000000006506CEFA: E9 62 07 00 00     jmp         000000006506D661
  000000006506CEFF: 48 8D 8D B8 01 00  lea         rcx,[rbp+00000000000001B8h]
                    00
  000000006506CF06: FF 15 AC 99 62 00  call        qword ptr [00000000656968B8h]
  000000006506CF0C: 85 C0              test        eax,eax
  000000006506CF0E: 74 2B              je          000000006506CF3B
  000000006506CF10: 48 8D 95 B8 01 00  lea         rdx,[rbp+00000000000001B8h]
                    00
  000000006506CF17: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  000000006506CF1F: E8 FC 6D 00 00     call        0000000065073D20
  000000006506CF24: 48 8D 56 28        lea         rdx,[rsi+28h]
  000000006506CF28: 4C 8B CF           mov         r9,rdi
  000000006506CF2B: 45 33 C0           xor         r8d,r8d
  000000006506CF2E: 48 8B C8           mov         rcx,rax
  000000006506CF31: E8 3A EC 37 00     call        00000000653EBB70
  000000006506CF36: E9 26 07 00 00     jmp         000000006506D661
  000000006506CF3B: 48 8B CE           mov         rcx,rsi
  000000006506CF3E: FF 15 54 9E 62 00  call        qword ptr [0000000065696D98h]
  000000006506CF44: E9 18 07 00 00     jmp         000000006506D661
  000000006506CF49: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CF4C: 48 8B D6           mov         rdx,rsi
  000000006506CF4F: 48 8B CF           mov         rcx,rdi
  000000006506CF52: FF 90 F0 00 00 00  call        qword ptr [rax+00000000000000F0h]
  000000006506CF58: E9 04 07 00 00     jmp         000000006506D661
  000000006506CF5D: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CF60: 48 8B D6           mov         rdx,rsi
  000000006506CF63: 48 8B CF           mov         rcx,rdi
  000000006506CF66: FF 90 20 01 00 00  call        qword ptr [rax+0000000000000120h]
  000000006506CF6C: E9 F0 06 00 00     jmp         000000006506D661
  000000006506CF71: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CF74: 48 8B D6           mov         rdx,rsi
  000000006506CF77: 48 8B CF           mov         rcx,rdi
  000000006506CF7A: FF 90 08 01 00 00  call        qword ptr [rax+0000000000000108h]
  000000006506CF80: E9 DC 06 00 00     jmp         000000006506D661
  000000006506CF85: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CF88: 48 8B D6           mov         rdx,rsi
  000000006506CF8B: 48 8B CF           mov         rcx,rdi
  000000006506CF8E: FF 90 10 01 00 00  call        qword ptr [rax+0000000000000110h]
  000000006506CF94: E9 C8 06 00 00     jmp         000000006506D661
  000000006506CF99: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CF9C: 48 8B D6           mov         rdx,rsi
  000000006506CF9F: 48 8B CF           mov         rcx,rdi
  000000006506CFA2: FF 90 18 01 00 00  call        qword ptr [rax+0000000000000118h]
  000000006506CFA8: E9 B4 06 00 00     jmp         000000006506D661
  000000006506CFAD: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CFB0: 48 8B D6           mov         rdx,rsi
  000000006506CFB3: 48 8B CF           mov         rcx,rdi
  000000006506CFB6: FF 90 28 01 00 00  call        qword ptr [rax+0000000000000128h]
  000000006506CFBC: E9 A0 06 00 00     jmp         000000006506D661
  000000006506CFC1: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506CFC4: 48 8B D6           mov         rdx,rsi
  000000006506CFC7: 48 8B CF           mov         rcx,rdi
  000000006506CFCA: FF 90 30 01 00 00  call        qword ptr [rax+0000000000000130h]
  000000006506CFD0: E9 8C 06 00 00     jmp         000000006506D661
  000000006506CFD5: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006506CFD9: F6 40 0A 01        test        byte ptr [rax+0Ah],1
  000000006506CFDD: 0F 85 7E 06 00 00  jne         000000006506D661
  000000006506CFE3: 48 8B CD           mov         rcx,rbp
  000000006506CFE6: E8 05 CB 02 00     call        0000000065099AF0
  000000006506CFEB: E9 71 06 00 00     jmp         000000006506D661
  000000006506CFF0: 48 8B CD           mov         rcx,rbp
  000000006506CFF3: E8 D8 41 FF FF     call        00000000650611D0
  000000006506CFF8: E9 64 06 00 00     jmp         000000006506D661
  000000006506CFFD: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006506D001: 80 78 0C 11        cmp         byte ptr [rax+0Ch],11h
  000000006506D005: 0F 84 56 06 00 00  je          000000006506D661
  000000006506D00B: 48 8B CD           mov         rcx,rbp
  000000006506D00E: E8 6D B0 FF FF     call        0000000065068080
  000000006506D013: E9 49 06 00 00     jmp         000000006506D661
  000000006506D018: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506D01B: 48 8B D6           mov         rdx,rsi
  000000006506D01E: 48 8B CF           mov         rcx,rdi
  000000006506D021: FF 90 40 01 00 00  call        qword ptr [rax+0000000000000140h]
  000000006506D027: E9 35 06 00 00     jmp         000000006506D661
  000000006506D02C: 48 8B 1F           mov         rbx,qword ptr [rdi]
  000000006506D02F: 48 8B CE           mov         rcx,rsi
  000000006506D032: FF 15 30 9A 62 00  call        qword ptr [0000000065696A68h]
  000000006506D038: 83 F8 18           cmp         eax,18h
  000000006506D03B: 0F 95 C2           setne       dl
  000000006506D03E: 48 8B CF           mov         rcx,rdi
  000000006506D041: FF 93 80 01 00 00  call        qword ptr [rbx+0000000000000180h]
  000000006506D047: 4C 8B 5F 20        mov         r11,qword ptr [rdi+20h]
  000000006506D04B: 41 8B 43 08        mov         eax,dword ptr [r11+8]
  000000006506D04F: C1 E8 0F           shr         eax,0Fh
  000000006506D052: 24 01              and         al,1
  000000006506D054: 74 22              je          000000006506D078
  000000006506D056: 48 8B CF           mov         rcx,rdi
  000000006506D059: E8 B2 73 FF FF     call        0000000065064410
  000000006506D05E: 48 8B C8           mov         rcx,rax
  000000006506D061: 33 D2              xor         edx,edx
  000000006506D063: 44 8D 42 02        lea         r8d,[rdx+2]
  000000006506D067: E8 14 CE FD FF     call        0000000065049E80
  000000006506D06C: 84 C0              test        al,al
  000000006506D06E: 75 08              jne         000000006506D078
  000000006506D070: 48 8B CF           mov         rcx,rdi
  000000006506D073: E8 38 82 FF FF     call        00000000650652B0
  000000006506D078: 48 8B 4D 18        mov         rcx,qword ptr [rbp+18h]
  000000006506D07C: 48 89 4C 24 40     mov         qword ptr [rsp+40h],rcx
  000000006506D081: FF 15 39 98 62 00  call        qword ptr [00000000656968C0h]
  000000006506D087: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  000000006506D08C: 41 F6 43 10 01     test        byte ptr [r11+10h],1
  000000006506D091: 75 0F              jne         000000006506D0A2
  000000006506D093: 41 8B 53 04        mov         edx,dword ptr [r11+4]
  000000006506D097: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506D09C: E8 CF 23 4A 00     call        000000006550F470
  000000006506D0A1: 90                 nop
  000000006506D0A2: 33 DB              xor         ebx,ebx
  000000006506D0A4: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506D0A9: FF 15 09 98 62 00  call        qword ptr [00000000656968B8h]
  000000006506D0AF: 85 C0              test        eax,eax
  000000006506D0B1: 7E 4F              jle         000000006506D102
  000000006506D0B3: 8B D3              mov         edx,ebx
  000000006506D0B5: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506D0BA: FF 15 80 98 62 00  call        qword ptr [0000000065696940h]
  000000006506D0C0: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006506D0C3: E8 98 BF F9 FF     call        0000000065009060
  000000006506D0C8: 48 85 C0           test        rax,rax
  000000006506D0CB: 74 24              je          000000006506D0F1
  000000006506D0CD: 48 8B 50 20        mov         rdx,qword ptr [rax+20h]
  000000006506D0D1: 8B 4A 08           mov         ecx,dword ptr [rdx+8]
  000000006506D0D4: C1 E9 0F           shr         ecx,0Fh
  000000006506D0D7: 80 E1 01           and         cl,1
  000000006506D0DA: 74 15              je          000000006506D0F1
  000000006506D0DC: 0F B6 4A 0C        movzx       ecx,byte ptr [rdx+0Ch]
  000000006506D0E0: F6 C1 01           test        cl,1
  000000006506D0E3: 75 0C              jne         000000006506D0F1
  000000006506D0E5: 48 8B D6           mov         rdx,rsi
  000000006506D0E8: 48 8B C8           mov         rcx,rax
  000000006506D0EB: FF 15 1F 97 62 00  call        qword ptr [0000000065696810h]
  000000006506D0F1: FF C3              inc         ebx
  000000006506D0F3: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506D0F8: FF 15 BA 97 62 00  call        qword ptr [00000000656968B8h]
  000000006506D0FE: 3B D8              cmp         ebx,eax
  000000006506D100: 7C B1              jl          000000006506D0B3
  000000006506D102: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006506D107: FF 15 6B 98 62 00  call        qword ptr [0000000065696978h]
  000000006506D10D: 84 C0              test        al,al
  000000006506D10F: 0F 85 4C 05 00 00  jne         000000006506D661
  000000006506D115: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006506D11A: FF 15 40 98 62 00  call        qword ptr [0000000065696960h]
  000000006506D120: E9 3C 05 00 00     jmp         000000006506D661
  000000006506D125: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506D128: 48 8B CF           mov         rcx,rdi
  000000006506D12B: FF 90 88 01 00 00  call        qword ptr [rax+0000000000000188h]
  000000006506D131: 4C 8B 1F           mov         r11,qword ptr [rdi]
  000000006506D134: 48 8B D6           mov         rdx,rsi
  000000006506D137: 48 8B CF           mov         rcx,rdi
  000000006506D13A: 41 FF 93 40 01 00  call        qword ptr [r11+0000000000000140h]
                    00
  000000006506D141: 48 8B 4D 18        mov         rcx,qword ptr [rbp+18h]
  000000006506D145: 48 89 8C 24 80 01  mov         qword ptr [rsp+0000000000000180h],rcx
                    00 00
  000000006506D14D: FF 15 6D 97 62 00  call        qword ptr [00000000656968C0h]
  000000006506D153: 4C 8B 9C 24 80 01  mov         r11,qword ptr [rsp+0000000000000180h]
                    00 00
  000000006506D15B: 41 F6 43 10 01     test        byte ptr [r11+10h],1
  000000006506D160: 75 12              jne         000000006506D174
  000000006506D162: 41 8B 53 04        mov         edx,dword ptr [r11+4]
  000000006506D166: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D16E: E8 FD 22 4A 00     call        000000006550F470
  000000006506D173: 90                 nop
  000000006506D174: 33 DB              xor         ebx,ebx
  000000006506D176: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D17E: FF 15 34 97 62 00  call        qword ptr [00000000656968B8h]
  000000006506D184: 85 C0              test        eax,eax
  000000006506D186: 7E 3D              jle         000000006506D1C5
  000000006506D188: 0F 1F 84 00 00 00  nop         dword ptr [rax+rax+0000000000000000h]
                    00 00
  000000006506D190: 8B D3              mov         edx,ebx
  000000006506D192: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D19A: FF 15 A0 97 62 00  call        qword ptr [0000000065696940h]
  000000006506D1A0: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006506D1A3: 48 85 C9           test        rcx,rcx
  000000006506D1A6: 74 09              je          000000006506D1B1
  000000006506D1A8: 48 8B D6           mov         rdx,rsi
  000000006506D1AB: FF 15 5F 96 62 00  call        qword ptr [0000000065696810h]
  000000006506D1B1: FF C3              inc         ebx
  000000006506D1B3: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D1BB: FF 15 F7 96 62 00  call        qword ptr [00000000656968B8h]
  000000006506D1C1: 3B D8              cmp         ebx,eax
  000000006506D1C3: 7C CB              jl          000000006506D190
  000000006506D1C5: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000006506D1CD: FF 15 A5 97 62 00  call        qword ptr [0000000065696978h]
  000000006506D1D3: 84 C0              test        al,al
  000000006506D1D5: 75 0E              jne         000000006506D1E5
  000000006506D1D7: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000006506D1DF: FF 15 7B 97 62 00  call        qword ptr [0000000065696960h]
  000000006506D1E5: 48 8B CF           mov         rcx,rdi
  000000006506D1E8: E8 C3 80 FF FF     call        00000000650652B0
  000000006506D1ED: E9 6F 04 00 00     jmp         000000006506D661
  000000006506D1F2: 48 8B CD           mov         rcx,rbp
  000000006506D1F5: E8 26 D4 FF FF     call        000000006506A620
  000000006506D1FA: E9 62 04 00 00     jmp         000000006506D661
  000000006506D1FF: 48 83 BD A8 00 00  cmp         qword ptr [rbp+00000000000000A8h],0
                    00 00
  000000006506D207: 74 11              je          000000006506D21A
  000000006506D209: 48 8B 8D A8 00 00  mov         rcx,qword ptr [rbp+00000000000000A8h]
                    00
  000000006506D210: 48 83 C1 10        add         rcx,10h
  000000006506D214: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006506D217: FF 50 58           call        qword ptr [rax+58h]
  000000006506D21A: 48 8B CF           mov         rcx,rdi
  000000006506D21D: E8 8E 80 FF FF     call        00000000650652B0
  000000006506D222: 4C 8B 1F           mov         r11,qword ptr [rdi]
  000000006506D225: 48 8B D6           mov         rdx,rsi
  000000006506D228: 48 8B CF           mov         rcx,rdi
  000000006506D22B: 41 FF 93 40 01 00  call        qword ptr [r11+0000000000000140h]
                    00
  000000006506D232: E9 2A 04 00 00     jmp         000000006506D661
  000000006506D237: 48 8B CD           mov         rcx,rbp
  000000006506D23A: E8 A1 36 FF FF     call        00000000650608E0
  000000006506D23F: E9 1D 04 00 00     jmp         000000006506D661
  000000006506D244: 48 8D 56 18        lea         rdx,[rsi+18h]
  000000006506D248: 48 8B CF           mov         rcx,rdi
  000000006506D24B: E8 70 54 FF FF     call        00000000650626C0
  000000006506D250: E9 0C 04 00 00     jmp         000000006506D661
  000000006506D255: 48 8B 4D 18        mov         rcx,qword ptr [rbp+18h]
  000000006506D259: 48 89 8C 24 80 01  mov         qword ptr [rsp+0000000000000180h],rcx
                    00 00
  000000006506D261: FF 15 59 96 62 00  call        qword ptr [00000000656968C0h]
  000000006506D267: 4C 8B 9C 24 80 01  mov         r11,qword ptr [rsp+0000000000000180h]
                    00 00
  000000006506D26F: 41 F6 43 10 01     test        byte ptr [r11+10h],1
  000000006506D274: 75 12              jne         000000006506D288
  000000006506D276: 41 8B 53 04        mov         edx,dword ptr [r11+4]
  000000006506D27A: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D282: E8 E9 21 4A 00     call        000000006550F470
  000000006506D287: 90                 nop
  000000006506D288: 33 DB              xor         ebx,ebx
  000000006506D28A: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D292: FF 15 20 96 62 00  call        qword ptr [00000000656968B8h]
  000000006506D298: 85 C0              test        eax,eax
  000000006506D29A: 7E 5F              jle         000000006506D2FB
  000000006506D29C: 0F 1F 40 00        nop         dword ptr [rax]
  000000006506D2A0: 8B D3              mov         edx,ebx
  000000006506D2A2: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D2AA: FF 15 90 96 62 00  call        qword ptr [0000000065696940h]
  000000006506D2B0: 48 8B 28           mov         rbp,qword ptr [rax]
  000000006506D2B3: 48 85 ED           test        rbp,rbp
  000000006506D2B6: 74 2F              je          000000006506D2E7
  000000006506D2B8: E8 D3 33 FB FF     call        0000000065020690
  000000006506D2BD: 48 3B E8           cmp         rbp,rax
  000000006506D2C0: 74 25              je          000000006506D2E7
  000000006506D2C2: 48 8B CD           mov         rcx,rbp
  000000006506D2C5: E8 96 BD F9 FF     call        0000000065009060
  000000006506D2CA: 48 85 C0           test        rax,rax
  000000006506D2CD: 74 0C              je          000000006506D2DB
  000000006506D2CF: 48 8B 45 20        mov         rax,qword ptr [rbp+20h]
  000000006506D2D3: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506D2D7: A8 01              test        al,1
  000000006506D2D9: 75 0C              jne         000000006506D2E7
  000000006506D2DB: 48 8B D6           mov         rdx,rsi
  000000006506D2DE: 48 8B CD           mov         rcx,rbp
  000000006506D2E1: FF 15 29 95 62 00  call        qword ptr [0000000065696810h]
  000000006506D2E7: FF C3              inc         ebx
  000000006506D2E9: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D2F1: FF 15 C1 95 62 00  call        qword ptr [00000000656968B8h]
  000000006506D2F7: 3B D8              cmp         ebx,eax
  000000006506D2F9: 7C A5              jl          000000006506D2A0
  000000006506D2FB: 48 8B CE           mov         rcx,rsi
  000000006506D2FE: FF 15 64 97 62 00  call        qword ptr [0000000065696A68h]
  000000006506D304: 83 F8 67           cmp         eax,67h
  000000006506D307: 0F 94 C2           sete        dl
  000000006506D30A: 48 8B CF           mov         rcx,rdi
  000000006506D30D: E8 AE F5 FF FF     call        000000006506C8C0
  000000006506D312: 90                 nop
  000000006506D313: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000006506D31B: FF 15 57 96 62 00  call        qword ptr [0000000065696978h]
  000000006506D321: 84 C0              test        al,al
  000000006506D323: 0F 85 38 03 00 00  jne         000000006506D661
  000000006506D329: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000006506D331: FF 15 29 96 62 00  call        qword ptr [0000000065696960h]
  000000006506D337: E9 25 03 00 00     jmp         000000006506D661
  000000006506D33C: 48 8D 8D 00 01 00  lea         rcx,[rbp+0000000000000100h]
                    00
  000000006506D343: FF 15 9F 96 62 00  call        qword ptr [00000000656969E8h]
  000000006506D349: 84 C0              test        al,al
  000000006506D34B: 75 18              jne         000000006506D365
  000000006506D34D: 48 8D 4E 20        lea         rcx,[rsi+20h]
  000000006506D351: 4C 8B C7           mov         r8,rdi
  000000006506D354: 48 8D 95 00 01 00  lea         rdx,[rbp+0000000000000100h]
                    00
  000000006506D35B: E8 D0 51 FE FF     call        0000000065052530
  000000006506D360: E9 FC 02 00 00     jmp         000000006506D661
  000000006506D365: 48 8B CE           mov         rcx,rsi
  000000006506D368: FF 15 2A 9A 62 00  call        qword ptr [0000000065696D98h]
  000000006506D36E: E9 EE 02 00 00     jmp         000000006506D661
  000000006506D373: 48 8D 8D 10 01 00  lea         rcx,[rbp+0000000000000110h]
                    00
  000000006506D37A: FF 15 10 95 62 00  call        qword ptr [0000000065696890h]
  000000006506D380: 85 C0              test        eax,eax
  000000006506D382: 74 18              je          000000006506D39C
  000000006506D384: 48 8D 4E 20        lea         rcx,[rsi+20h]
  000000006506D388: 4C 8B C7           mov         r8,rdi
  000000006506D38B: 48 8D 95 10 01 00  lea         rdx,[rbp+0000000000000110h]
                    00
  000000006506D392: E8 A9 D8 FE FF     call        000000006505AC40
  000000006506D397: E9 C5 02 00 00     jmp         000000006506D661
  000000006506D39C: 48 8B CE           mov         rcx,rsi
  000000006506D39F: FF 15 F3 99 62 00  call        qword ptr [0000000065696D98h]
  000000006506D3A5: E9 B7 02 00 00     jmp         000000006506D661
  000000006506D3AA: 48 8D 8D 10 01 00  lea         rcx,[rbp+0000000000000110h]
                    00
  000000006506D3B1: FF 15 31 96 62 00  call        qword ptr [00000000656969E8h]
  000000006506D3B7: 84 C0              test        al,al
  000000006506D3B9: 0F 84 A2 02 00 00  je          000000006506D661
  000000006506D3BF: 48 8B CE           mov         rcx,rsi
  000000006506D3C2: FF 15 D0 99 62 00  call        qword ptr [0000000065696D98h]
  000000006506D3C8: E9 94 02 00 00     jmp         000000006506D661
  000000006506D3CD: 83 7E 18 00        cmp         dword ptr [rsi+18h],0
  000000006506D3D1: 74 07              je          000000006506D3DA
  000000006506D3D3: 32 C0              xor         al,al
  000000006506D3D5: E9 99 03 00 00     jmp         000000006506D773
  000000006506D3DA: 48 8B CE           mov         rcx,rsi
  000000006506D3DD: FF 15 85 96 62 00  call        qword ptr [0000000065696A68h]
  000000006506D3E3: 83 E8 77           sub         eax,77h
  000000006506D3E6: 74 22              je          000000006506D40A
  000000006506D3E8: 83 E8 0B           sub         eax,0Bh
  000000006506D3EB: 74 07              je          000000006506D3F4
  000000006506D3ED: 32 C0              xor         al,al
  000000006506D3EF: E9 7F 03 00 00     jmp         000000006506D773
  000000006506D3F4: 48 8D 95 00 01 00  lea         rdx,[rbp+0000000000000100h]
                    00
  000000006506D3FB: 48 8D 4E 20        lea         rcx,[rsi+20h]
  000000006506D3FF: FF 15 7B 93 62 00  call        qword ptr [0000000065696780h]
  000000006506D405: E9 57 02 00 00     jmp         000000006506D661
  000000006506D40A: 48 8D 95 10 01 00  lea         rdx,[rbp+0000000000000110h]
                    00
  000000006506D411: 48 8D 4E 20        lea         rcx,[rsi+20h]
  000000006506D415: FF 15 65 93 62 00  call        qword ptr [0000000065696780h]
  000000006506D41B: E9 41 02 00 00     jmp         000000006506D661
  000000006506D420: 48 8B CD           mov         rcx,rbp
  000000006506D423: E8 58 86 FF FF     call        0000000065065A80
  000000006506D428: 48 8B 85 88 00 00  mov         rax,qword ptr [rbp+0000000000000088h]
                    00
  000000006506D42F: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006506D433: 48 83 C1 58        add         rcx,58h
  000000006506D437: 33 DB              xor         ebx,ebx
  000000006506D439: 89 5C 24 20        mov         dword ptr [rsp+20h],ebx
  000000006506D43D: 45 33 C9           xor         r9d,r9d
  000000006506D440: 45 33 C0           xor         r8d,r8d
  000000006506D443: 33 D2              xor         edx,edx
  000000006506D445: FF 15 85 9F 62 00  call        qword ptr [00000000656973D0h]
  000000006506D44B: 4C 8B 5F 20        mov         r11,qword ptr [rdi+20h]
  000000006506D44F: 41 81 63 10 FF EF  and         dword ptr [r11+10h],0FFFFEFFFh
                    FF FF
  000000006506D457: 48 8B CD           mov         rcx,rbp
  000000006506D45A: E8 21 86 FF FF     call        0000000065065A80
  000000006506D45F: 4C 8B 9D 88 00 00  mov         r11,qword ptr [rbp+0000000000000088h]
                    00
  000000006506D466: 49 8B 43 08        mov         rax,qword ptr [r11+8]
  000000006506D46A: 81 48 7C 00 10 00  or          dword ptr [rax+7Ch],1000h
                    00
  000000006506D471: E9 EB 01 00 00     jmp         000000006506D661
  000000006506D476: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506D479: 48 8B D6           mov         rdx,rsi
  000000006506D47C: 48 8B CF           mov         rcx,rdi
  000000006506D47F: FF 90 00 01 00 00  call        qword ptr [rax+0000000000000100h]
  000000006506D485: E9 D7 01 00 00     jmp         000000006506D661
  000000006506D48A: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006506D48D: 48 8B D6           mov         rdx,rsi
  000000006506D490: 48 8B CF           mov         rcx,rdi
  000000006506D493: FF 90 40 01 00 00  call        qword ptr [rax+0000000000000140h]
  000000006506D499: 48 8B 4D 18        mov         rcx,qword ptr [rbp+18h]
  000000006506D49D: 48 89 4C 24 40     mov         qword ptr [rsp+40h],rcx
  000000006506D4A2: FF 15 18 94 62 00  call        qword ptr [00000000656968C0h]
  000000006506D4A8: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  000000006506D4AD: 41 F6 43 10 01     test        byte ptr [r11+10h],1
  000000006506D4B2: 75 0F              jne         000000006506D4C3
  000000006506D4B4: 41 8B 53 04        mov         edx,dword ptr [r11+4]
  000000006506D4B8: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506D4BD: E8 AE 1F 4A 00     call        000000006550F470
  000000006506D4C2: 90                 nop
  000000006506D4C3: 33 DB              xor         ebx,ebx
  000000006506D4C5: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506D4CA: FF 15 E8 93 62 00  call        qword ptr [00000000656968B8h]
  000000006506D4D0: 85 C0              test        eax,eax
  000000006506D4D2: 7E 5B              jle         000000006506D52F
  000000006506D4D4: 66 66 66 66 0F 1F  nop         word ptr [rax+rax+0000000000000000h]
                    84 00 00 00 00 00
  000000006506D4E0: 8B D3              mov         edx,ebx
  000000006506D4E2: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506D4E7: FF 15 53 94 62 00  call        qword ptr [0000000065696940h]
  000000006506D4ED: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006506D4F0: E8 6B BB F9 FF     call        0000000065009060
  000000006506D4F5: 48 85 C0           test        rax,rax
  000000006506D4F8: 74 24              je          000000006506D51E
  000000006506D4FA: 48 8B 50 20        mov         rdx,qword ptr [rax+20h]
  000000006506D4FE: 8B 4A 08           mov         ecx,dword ptr [rdx+8]
  000000006506D501: C1 E9 0F           shr         ecx,0Fh
  000000006506D504: 80 E1 01           and         cl,1
  000000006506D507: 74 15              je          000000006506D51E
  000000006506D509: 0F B6 4A 0C        movzx       ecx,byte ptr [rdx+0Ch]
  000000006506D50D: F6 C1 01           test        cl,1
  000000006506D510: 75 0C              jne         000000006506D51E
  000000006506D512: 48 8B D6           mov         rdx,rsi
  000000006506D515: 48 8B C8           mov         rcx,rax
  000000006506D518: FF 15 F2 92 62 00  call        qword ptr [0000000065696810h]
  000000006506D51E: FF C3              inc         ebx
  000000006506D520: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506D525: FF 15 8D 93 62 00  call        qword ptr [00000000656968B8h]
  000000006506D52B: 3B D8              cmp         ebx,eax
  000000006506D52D: 7C B1              jl          000000006506D4E0
  000000006506D52F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006506D534: FF 15 3E 94 62 00  call        qword ptr [0000000065696978h]
  000000006506D53A: 84 C0              test        al,al
  000000006506D53C: 0F 85 1F 01 00 00  jne         000000006506D661
  000000006506D542: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006506D547: FF 15 13 94 62 00  call        qword ptr [0000000065696960h]
  000000006506D54D: E9 0F 01 00 00     jmp         000000006506D661
  000000006506D552: 48 8D 4E 30        lea         rcx,[rsi+30h]
  000000006506D556: 33 D2              xor         edx,edx
  000000006506D558: FF 15 E2 93 62 00  call        qword ptr [0000000065696940h]
  000000006506D55E: 48 8B 28           mov         rbp,qword ptr [rax]
  000000006506D561: 48 8B CD           mov         rcx,rbp
  000000006506D564: E8 37 83 FC FF     call        00000000650358A0
  000000006506D569: 84 C0              test        al,al
  000000006506D56B: 0F 85 F0 00 00 00  jne         000000006506D661
  000000006506D571: 83 7E 28 01        cmp         dword ptr [rsi+28h],1
  000000006506D575: 0F 84 E6 00 00 00  je          000000006506D661
  000000006506D57B: 48 8B CE           mov         rcx,rsi
  000000006506D57E: FF 15 E4 94 62 00  call        qword ptr [0000000065696A68h]
  000000006506D584: 2D C2 00 00 00     sub         eax,0C2h
  000000006506D589: 74 1C              je          000000006506D5A7
  000000006506D58B: 83 E8 01           sub         eax,1
  000000006506D58E: 74 0F              je          000000006506D59F
  000000006506D590: 83 E8 01           sub         eax,1
  000000006506D593: 0F 85 C8 00 00 00  jne         000000006506D661
  000000006506D599: 44 8D 60 03        lea         r12d,[rax+3]
  000000006506D59D: EB 0E              jmp         000000006506D5AD
  000000006506D59F: 41 BC 05 00 00 00  mov         r12d,5
  000000006506D5A5: EB 06              jmp         000000006506D5AD
  000000006506D5A7: 41 BC 02 00 00 00  mov         r12d,2
  000000006506D5AD: 8B 46 18           mov         eax,dword ptr [rsi+18h]
  000000006506D5B0: 89 84 24 90 01 00  mov         dword ptr [rsp+0000000000000190h],eax
                    00
  000000006506D5B7: C7 84 24 98 01 00  mov         dword ptr [rsp+0000000000000198h],1
                    00 01 00 00 00
  000000006506D5C2: 48 8D 94 24 28 01  lea         rdx,[rsp+0000000000000128h]
                    00 00
  000000006506D5CA: 48 8B CD           mov         rcx,rbp
  000000006506D5CD: E8 CE 54 FC FF     call        0000000065032AA0
  000000006506D5D2: 48 8B C8           mov         rcx,rax
  000000006506D5D5: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  000000006506D5DD: FF 15 9D 96 62 00  call        qword ptr [0000000065696C80h]
  000000006506D5E3: 48 8B D8           mov         rbx,rax
  000000006506D5E6: 48 8D 94 24 38 01  lea         rdx,[rsp+0000000000000138h]
                    00 00
  000000006506D5EE: 48 8B CD           mov         rcx,rbp
  000000006506D5F1: E8 6A 54 FC FF     call        0000000065032A60
  000000006506D5F6: 48 8B C8           mov         rcx,rax
  000000006506D5F9: 48 8D 94 24 84 00  lea         rdx,[rsp+0000000000000084h]
                    00 00
  000000006506D601: FF 15 79 96 62 00  call        qword ptr [0000000065696C80h]
  000000006506D607: 4C 8B C0           mov         r8,rax
  000000006506D60A: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  000000006506D612: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006506D617: 48 8D 84 24 98 01  lea         rax,[rsp+0000000000000198h]
                    00 00
  000000006506D61F: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000006506D624: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  000000006506D62C: 4C 8B CB           mov         r9,rbx
  000000006506D62F: 41 8B D4           mov         edx,r12d
  000000006506D632: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506D637: E8 D4 66 FC FF     call        0000000065033D10
  000000006506D63C: 90                 nop
  000000006506D63D: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006506D642: 48 8B CF           mov         rcx,rdi
  000000006506D645: FF 15 C5 91 62 00  call        qword ptr [0000000065696810h]
  000000006506D64B: 90                 nop
  000000006506D64C: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506D651: E8 1A 4C FC FF     call        0000000065032270
  000000006506D656: EB 09              jmp         000000006506D661
  000000006506D658: 48 8B CE           mov         rcx,rsi
  000000006506D65B: FF 15 37 97 62 00  call        qword ptr [0000000065696D98h]
  000000006506D661: B0 01              mov         al,1
  000000006506D663: E9 0B 01 00 00     jmp         000000006506D773
  000000006506D668: 48 8D 94 24 80 01  lea         rdx,[rsp+0000000000000180h]
                    00 00
  000000006506D670: 48 8B CE           mov         rcx,rsi
  000000006506D673: FF 15 2F 9E 62 00  call        qword ptr [00000000656974A8h]
  000000006506D679: 90                 nop
  000000006506D67A: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D682: FF 15 70 96 62 00  call        qword ptr [0000000065696CF8h]
  000000006506D688: 48 85 C0           test        rax,rax
  000000006506D68B: 0F 84 C8 00 00 00  je          000000006506D759
  000000006506D691: 41 B8 0C 00 00 00  mov         r8d,0Ch
  000000006506D697: 48 8D 15 4A E0 63  lea         rdx,[00000000656AB6E8h]
                    00
  000000006506D69E: 48 8B C8           mov         rcx,rax
  000000006506D6A1: FF 15 B1 8F 62 00  call        qword ptr [0000000065696658h]
  000000006506D6A7: 85 C0              test        eax,eax
  000000006506D6A9: 0F 85 AA 00 00 00  jne         000000006506D759
  000000006506D6AF: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D6B7: FF 15 53 96 62 00  call        qword ptr [0000000065696D10h]
  000000006506D6BD: 83 F8 0D           cmp         eax,0Dh
  000000006506D6C0: 0F 85 93 00 00 00  jne         000000006506D759
  000000006506D6C6: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D6CE: FF 15 84 93 62 00  call        qword ptr [0000000065696A58h]
  000000006506D6D4: 4C 8B C0           mov         r8,rax
  000000006506D6D7: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  000000006506D6DF: 48 8B CF           mov         rcx,rdi
  000000006506D6E2: FF 15 08 9C 62 00  call        qword ptr [00000000656972F0h]
  000000006506D6E8: 90                 nop
  000000006506D6E9: 33 D2              xor         edx,edx
  000000006506D6EB: 48 8B C8           mov         rcx,rax
  000000006506D6EE: FF 15 AC 9D 62 00  call        qword ptr [00000000656974A0h]
  000000006506D6F4: 8B D8              mov         ebx,eax
  000000006506D6F6: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000006506D6FE: FF 15 DC 8F 62 00  call        qword ptr [00000000656966E0h]
  000000006506D704: 48 83 BD 88 00 00  cmp         qword ptr [rbp+0000000000000088h],0
                    00 00
  000000006506D70C: 75 08              jne         000000006506D716
  000000006506D70E: 48 8B CD           mov         rcx,rbp
  000000006506D711: E8 EA 57 FF FF     call        0000000065062F00
  000000006506D716: BA 0C 00 00 00     mov         edx,0Ch
  000000006506D71B: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D723: FF 15 6F 9D 62 00  call        qword ptr [0000000065697498h]
  000000006506D729: 3C 58              cmp         al,58h
  000000006506D72B: 75 0D              jne         000000006506D73A
  000000006506D72D: 48 8B 85 88 00 00  mov         rax,qword ptr [rbp+0000000000000088h]
                    00
  000000006506D734: 66 89 58 50        mov         word ptr [rax+50h],bx
  000000006506D738: EB 0F              jmp         000000006506D749
  000000006506D73A: 3C 59              cmp         al,59h
  000000006506D73C: 75 0B              jne         000000006506D749
  000000006506D73E: 48 8B 85 88 00 00  mov         rax,qword ptr [rbp+0000000000000088h]
                    00
  000000006506D745: 66 89 58 52        mov         word ptr [rax+52h],bx
  000000006506D749: 48 8D 95 80 01 00  lea         rdx,[rbp+0000000000000180h]
                    00
  000000006506D750: 48 8B CD           mov         rcx,rbp
  000000006506D753: E8 B8 1D FF FF     call        000000006505F510
  000000006506D758: 90                 nop
  000000006506D759: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000006506D761: FF 15 E9 92 62 00  call        qword ptr [0000000065696A50h]
  000000006506D767: 48 8B D6           mov         rdx,rsi
  000000006506D76A: 48 8B CF           mov         rcx,rdi
  000000006506D76D: FF 15 D5 92 62 00  call        qword ptr [0000000065696A48h]
  000000006506D773: 48 81 C4 50 01 00  add         rsp,150h
                    00
  000000006506D77A: 41 5C              pop         r12
  000000006506D77C: 5F                 pop         rdi
  000000006506D77D: 5E                 pop         rsi
  000000006506D77E: 5D                 pop         rbp
  000000006506D77F: 5B                 pop         rbx
  000000006506D780: C3                 ret
  000000006506D781: 0F 1F 00           nop         dword ptr [rax]
  000000006506D784: B3 C9              mov         bl,0C9h
  000000006506D786: 06
  000000006506D787: 00 BA C9 06 00 00  add         byte ptr [rdx+00000000000006C9h],bh
  000000006506D78D: 00 00              add         byte ptr [rax],al
  000000006506D78F: 00 01              add         byte ptr [rcx],al
  000000006506D791: 01 01              add         dword ptr [rcx],eax
  000000006506D793: 01 01              add         dword ptr [rcx],eax
  000000006506D795: 01 01              add         dword ptr [rcx],eax
  000000006506D797: 01 01              add         dword ptr [rcx],eax
  000000006506D799: 01 01              add         dword ptr [rcx],eax
  000000006506D79B: 01 01              add         dword ptr [rcx],eax
  000000006506D79D: 01 01              add         dword ptr [rcx],eax
  000000006506D79F: 01 01              add         dword ptr [rcx],eax
  000000006506D7A1: 01 01              add         dword ptr [rcx],eax
  000000006506D7A3: 01 01              add         dword ptr [rcx],eax
  000000006506D7A5: 01 01              add         dword ptr [rcx],eax
  000000006506D7A7: 01 01              add         dword ptr [rcx],eax
  000000006506D7A9: 00 01              add         byte ptr [rcx],al
  000000006506D7AB: 01 01              add         dword ptr [rcx],eax
  000000006506D7AD: 01 01              add         dword ptr [rcx],eax
  000000006506D7AF: 01 01              add         dword ptr [rcx],eax
  000000006506D7B1: 01 01              add         dword ptr [rcx],eax
  000000006506D7B3: 01 01              add         dword ptr [rcx],eax
  000000006506D7B5: 01 01              add         dword ptr [rcx],eax
  000000006506D7B7: 01 01              add         dword ptr [rcx],eax
  000000006506D7B9: 01 01              add         dword ptr [rcx],eax
  000000006506D7BB: 01 01              add         dword ptr [rcx],eax
  000000006506D7BD: 01 01              add         dword ptr [rcx],eax
  000000006506D7BF: 01 01              add         dword ptr [rcx],eax
  000000006506D7C1: 01 01              add         dword ptr [rcx],eax
  000000006506D7C3: 01 01              add         dword ptr [rcx],eax
  000000006506D7C5: 01 01              add         dword ptr [rcx],eax
  000000006506D7C7: 01 01              add         dword ptr [rcx],eax
  000000006506D7C9: 01 01              add         dword ptr [rcx],eax
  000000006506D7CB: 01 01              add         dword ptr [rcx],eax
  000000006506D7CD: 01 01              add         dword ptr [rcx],eax
  000000006506D7CF: 01 01              add         dword ptr [rcx],eax
  000000006506D7D1: 01 01              add         dword ptr [rcx],eax
  000000006506D7D3: 01 01              add         dword ptr [rcx],eax
  000000006506D7D5: 01 01              add         dword ptr [rcx],eax
  000000006506D7D7: 01 01              add         dword ptr [rcx],eax
  000000006506D7D9: 01 01              add         dword ptr [rcx],eax
  000000006506D7DB: 01 00              add         dword ptr [rax],eax
  000000006506D7DD: 01 01              add         dword ptr [rcx],eax
  000000006506D7DF: 01 01              add         dword ptr [rcx],eax
  000000006506D7E1: 00 01              add         byte ptr [rcx],al
  000000006506D7E3: 01 01              add         dword ptr [rcx],eax
  000000006506D7E5: 01 00              add         dword ptr [rax],eax
  000000006506D7E7: 00 01              add         byte ptr [rcx],al
  000000006506D7E9: 01 01              add         dword ptr [rcx],eax
  000000006506D7EB: 01 01              add         dword ptr [rcx],eax
  000000006506D7ED: 01 01              add         dword ptr [rcx],eax
  000000006506D7EF: 01 01              add         dword ptr [rcx],eax
  000000006506D7F1: 01 01              add         dword ptr [rcx],eax
  000000006506D7F3: 01 01              add         dword ptr [rcx],eax
  000000006506D7F5: 01 01              add         dword ptr [rcx],eax
  000000006506D7F7: 01 01              add         dword ptr [rcx],eax
  000000006506D7F9: 01 01              add         dword ptr [rcx],eax
  000000006506D7FB: 01 01              add         dword ptr [rcx],eax
  000000006506D7FD: 01 01              add         dword ptr [rcx],eax
  000000006506D7FF: 01 01              add         dword ptr [rcx],eax
  000000006506D801: 01 01              add         dword ptr [rcx],eax
  000000006506D803: 01 01              add         dword ptr [rcx],eax
  000000006506D805: 01 01              add         dword ptr [rcx],eax
  000000006506D807: 01 01              add         dword ptr [rcx],eax
  000000006506D809: 01 01              add         dword ptr [rcx],eax
  000000006506D80B: 01 01              add         dword ptr [rcx],eax
  000000006506D80D: 01 01              add         dword ptr [rcx],eax
  000000006506D80F: 01 01              add         dword ptr [rcx],eax
  000000006506D811: 01 01              add         dword ptr [rcx],eax
  000000006506D813: 01 01              add         dword ptr [rcx],eax
  000000006506D815: 01 01              add         dword ptr [rcx],eax
  000000006506D817: 01 01              add         dword ptr [rcx],eax
  000000006506D819: 01 01              add         dword ptr [rcx],eax
  000000006506D81B: 01 01              add         dword ptr [rcx],eax
  000000006506D81D: 01 01              add         dword ptr [rcx],eax
  000000006506D81F: 01 01              add         dword ptr [rcx],eax
  000000006506D821: 01 01              add         dword ptr [rcx],eax
  000000006506D823: 01 01              add         dword ptr [rcx],eax
  000000006506D825: 01 01              add         dword ptr [rcx],eax
  000000006506D827: 01 01              add         dword ptr [rcx],eax
  000000006506D829: 01 01              add         dword ptr [rcx],eax
  000000006506D82B: 01 01              add         dword ptr [rcx],eax
  000000006506D82D: 01 01              add         dword ptr [rcx],eax
  000000006506D82F: 01 01              add         dword ptr [rcx],eax
  000000006506D831: 01 01              add         dword ptr [rcx],eax
  000000006506D833: 01 01              add         dword ptr [rcx],eax
  000000006506D835: 01 01              add         dword ptr [rcx],eax
  000000006506D837: 01 01              add         dword ptr [rcx],eax
  000000006506D839: 01 01              add         dword ptr [rcx],eax
  000000006506D83B: 01 01              add         dword ptr [rcx],eax
  000000006506D83D: 01 01              add         dword ptr [rcx],eax
  000000006506D83F: 01 01              add         dword ptr [rcx],eax
  000000006506D841: 01 01              add         dword ptr [rcx],eax
  000000006506D843: 01 01              add         dword ptr [rcx],eax
  000000006506D845: 01 01              add         dword ptr [rcx],eax
  000000006506D847: 01 01              add         dword ptr [rcx],eax
  000000006506D849: 01 01              add         dword ptr [rcx],eax
  000000006506D84B: 01 00              add         dword ptr [rax],eax
  000000006506D84D: 00 00              add         byte ptr [rax],al
  000000006506D84F: 90                 nop
  000000006506D850: FB                 sti
  000000006506D851: C9                 leave
  000000006506D852: 06
  000000006506D853: 00 0C CA           add         byte ptr [rdx+rcx*8],cl
  000000006506D856: 06
  000000006506D857: 00 20              add         byte ptr [rax],ah
  000000006506D859: CA 06 00           retf        6
  000000006506D85C: E7 C9              out         0C9h,eax
  000000006506D85E: 06
  000000006506D85F: 00 BB CA 06 00 05  add         byte ptr [rbx+00000000050006CAh],bh
  000000006506D865: CC                 int         3
  000000006506D866: 06
  000000006506D867: 00 63 CD           add         byte ptr [rbx-33h],ah
  000000006506D86A: 06
  000000006506D86B: 00 77 CD           add         byte ptr [rdi-33h],dh
  000000006506D86E: 06
  000000006506D86F: 00 8B CD 06 00 DB  add         byte ptr [rbx+FFFFFFFFDB0006CDh],cl
  000000006506D875: CD 06              int         6
  000000006506D877: 00 57 CE           add         byte ptr [rdi-32h],dl
  000000006506D87A: 06
  000000006506D87B: 00 6B CE           add         byte ptr [rbx-32h],ch
  000000006506D87E: 06
  000000006506D87F: 00 7F CE           add         byte ptr [rdi-32h],bh
  000000006506D882: 06
  000000006506D883: 00 AD CF 06 00 C1  add         byte ptr [rbp+FFFFFFFFC10006CFh],ch
  000000006506D889: CF                 iretd
  000000006506D88A: 06
  000000006506D88B: 00 93 CE 06 00 18  add         byte ptr [rbx+00000000180006CEh],dl
  000000006506D891: D0 06              rol         byte ptr [rsi],1
  000000006506D893: 00 2C D0           add         byte ptr [rax+rdx*8],ch
  000000006506D896: 06
  000000006506D897: 00 AB CA 06 00 83  add         byte ptr [rbx+FFFFFFFF830006CAh],ch
  000000006506D89D: CA 06 00           retf        6
  000000006506D8A0: 2C CD              sub         al,0CDh
  000000006506D8A2: 06
  000000006506D8A3: 00 F0              add         al,dh
  000000006506D8A5: CF                 iretd
  000000006506D8A6: 06
  000000006506D8A7: 00 F2              add         dl,dh
  000000006506D8A9: D1 06              rol         dword ptr [rsi],1
  000000006506D8AB: 00 FD              add         ch,bh
  000000006506D8AD: CF                 iretd
  000000006506D8AE: 06
  000000006506D8AF: 00 61 D6           add         byte ptr [rcx-2Ah],ah
  000000006506D8B2: 06
  000000006506D8B3: 00 71 CF           add         byte ptr [rcx-31h],dh
  000000006506D8B6: 06
  000000006506D8B7: 00 85 CF 06 00 99  add         byte ptr [rbp+FFFFFFFF990006CFh],al
  000000006506D8BD: CF                 iretd
  000000006506D8BE: 06
  000000006506D8BF: 00 5D CF           add         byte ptr [rbp-31h],bl
  000000006506D8C2: 06
  000000006506D8C3: 00 D5              add         ch,dl
  000000006506D8C5: CF                 iretd
  000000006506D8C6: 06
  000000006506D8C7: 00 2D CC 06 00 3A  add         byte ptr [000000009F06DF99h],ch
  000000006506D8CD: CC                 int         3
  000000006506D8CE: 06
  000000006506D8CF: 00 37              add         byte ptr [rdi],dh
  000000006506D8D1: D2 06              rol         byte ptr [rsi],cl
  000000006506D8D3: 00 44 D2 06        add         byte ptr [rdx+rdx*8+6],al
  000000006506D8D7: 00 20              add         byte ptr [rax],ah
  000000006506D8D9: D4
  000000006506D8DA: 06
  000000006506D8DB: 00 A7 CE 06 00 19  add         byte ptr [rdi+00000000190006CEh],ah
  000000006506D8E1: CC                 int         3
  000000006506D8E2: 06
  000000006506D8E3: 00 97 CA 06 00 25  add         byte ptr [rdi+00000000250006CAh],dl
  000000006506D8E9: D1 06              rol         dword ptr [rsi],1
  000000006506D8EB: 00 FF              add         bh,bh
  000000006506D8ED: D1 06              rol         dword ptr [rsi],1
  000000006506D8EF: 00 55 D2           add         byte ptr [rbp-2Eh],dl
  000000006506D8F2: 06
  000000006506D8F3: 00 3C D3           add         byte ptr [rbx+rdx*8],bh
  000000006506D8F6: 06
  000000006506D8F7: 00 73 D3           add         byte ptr [rbx-2Dh],dh
  000000006506D8FA: 06
  000000006506D8FB: 00 76 D4           add         byte ptr [rsi-2Ch],dh
  000000006506D8FE: 06
  000000006506D8FF: 00 CD              add         ch,cl
  000000006506D901: D3 06              rol         dword ptr [rsi],cl
  000000006506D903: 00 AA D3 06 00 4A  add         byte ptr [rdx+000000004A0006D3h],ch
  000000006506D909: CE
  000000006506D90A: 06
  000000006506D90B: 00 8A D4 06 00 68  add         byte ptr [rdx+00000000680006D4h],cl
  000000006506D911: D6
  000000006506D912: 06
  000000006506D913: 00 34 CA           add         byte ptr [rdx+rcx*8],dh
  000000006506D916: 06
  000000006506D917: 00 52 D5           add         byte ptr [rdx-2Bh],dl
  000000006506D91A: 06
  000000006506D91B: 00 58 D6           add         byte ptr [rax-2Ah],bl
  000000006506D91E: 06
  000000006506D91F: 00 67 D7           add         byte ptr [rdi-29h],ah
  000000006506D922: 06
  000000006506D923: 00 00              add         byte ptr [rax],al
  000000006506D925: 01 02              add         dword ptr [rdx],eax
  000000006506D927: 03 04 05 06 07 08  add         eax,dword ptr [rax+0000000009080706h]
                    09
  000000006506D92E: 0A 0B              or          cl,byte ptr [rbx]
  000000006506D930: 0C 34              or          al,34h
  000000006506D932: 34 0D              xor         al,0Dh
  000000006506D934: 0E
  000000006506D935: 0F 34              sysenter
  000000006506D937: 10 34 34           adc         byte ptr [rsp+rsi],dh
  000000006506D93A: 11 11              adc         dword ptr [rcx],edx
  000000006506D93C: 34 34              xor         al,34h
  000000006506D93E: 34 34              xor         al,34h
  000000006506D940: 12 13              adc         dl,byte ptr [rbx]
  000000006506D942: 34 10              xor         al,10h
  000000006506D944: 34 14              xor         al,14h
  000000006506D946: 15 16 17 10 34     adc         eax,34101716h
  000000006506D94B: 34 34              xor         al,34h
  000000006506D94D: 34 34              xor         al,34h
  000000006506D94F: 34 34              xor         al,34h
  000000006506D951: 34 34              xor         al,34h
  000000006506D953: 34 34              xor         al,34h
  000000006506D955: 18 34 34           sbb         byte ptr [rsp+rsi],dh
  000000006506D958: 34 34              xor         al,34h
  000000006506D95A: 34 34              xor         al,34h
  000000006506D95C: 34 34              xor         al,34h
  000000006506D95E: 19 1A              sbb         dword ptr [rdx],ebx
  000000006506D960: 1B 1C 34           sbb         ebx,dword ptr [rsp+rsi]
  000000006506D963: 34 34              xor         al,34h
  000000006506D965: 34 34              xor         al,34h
  000000006506D967: 34 34              xor         al,34h
  000000006506D969: 34 34              xor         al,34h
  000000006506D96B: 1D 1E 1F 34 20     sbb         eax,20341F1Eh
  000000006506D970: 21 22              and         dword ptr [rdx],esp
  000000006506D972: 34 34              xor         al,34h
  000000006506D974: 23 24 34           and         esp,dword ptr [rsp+rsi]
  000000006506D977: 34 34              xor         al,34h
  000000006506D979: 25 10 26 27 34     and         eax,34272610h
  000000006506D97E: 25 25 34 34 34     and         eax,34343425h
  000000006506D983: 10 10              adc         byte ptr [rax],dl
  000000006506D985: 10 10              adc         byte ptr [rax],dl
  000000006506D987: 10 10              adc         byte ptr [rax],dl
  000000006506D989: 28 28              sub         byte ptr [rax],ch
  000000006506D98B: 10 34 34           adc         byte ptr [rsp+rsi],dh
  000000006506D98E: 34 10              xor         al,10h
  000000006506D990: 29 2A              sub         dword ptr [rdx],ebp
  000000006506D992: 34 2B              xor         al,2Bh
  000000006506D994: 2B 2B              sub         ebp,dword ptr [rbx]
  000000006506D996: 34 34              xor         al,34h
  000000006506D998: 34 2C              xor         al,2Ch
  000000006506D99A: 10 34 34           adc         byte ptr [rsp+rsi],dh
  000000006506D99D: 2D 34 34 34 2E     sub         eax,2E343434h
  000000006506D9A2: 2E 34 2C           xor         al,2Ch
  000000006506D9A5: 34 34              xor         al,34h
  000000006506D9A7: 34 34              xor         al,34h
  000000006506D9A9: 34 34              xor         al,34h
  000000006506D9AB: 34 34              xor         al,34h
  000000006506D9AD: 34 34              xor         al,34h
  000000006506D9AF: 34 34              xor         al,34h
  000000006506D9B1: 34 34              xor         al,34h
  000000006506D9B3: 34 34              xor         al,34h
  000000006506D9B5: 34 34              xor         al,34h
  000000006506D9B7: 34 34              xor         al,34h
  000000006506D9B9: 34 34              xor         al,34h
  000000006506D9BB: 34 34              xor         al,34h
  000000006506D9BD: 34 34              xor         al,34h
  000000006506D9BF: 34 34              xor         al,34h
  000000006506D9C1: 34 34              xor         al,34h
  000000006506D9C3: 34 34              xor         al,34h
  000000006506D9C5: 34 34              xor         al,34h
  000000006506D9C7: 34 34              xor         al,34h
  000000006506D9C9: 34 34              xor         al,34h
  000000006506D9CB: 2F
  000000006506D9CC: 30 34 34           xor         byte ptr [rsp+rsi],dh
  000000006506D9CF: 34 31              xor         al,31h
  000000006506D9D1: 34 34              xor         al,34h
  000000006506D9D3: 10 10              adc         byte ptr [rax],dl
  000000006506D9D5: 34 34              xor         al,34h
  000000006506D9D7: 34 34              xor         al,34h
  000000006506D9D9: 34 34              xor         al,34h
  000000006506D9DB: 34 34              xor         al,34h
  000000006506D9DD: 34 34              xor         al,34h
  000000006506D9DF: 34 34              xor         al,34h
  000000006506D9E1: 34 34              xor         al,34h
  000000006506D9E3: 34 32              xor         al,32h
  000000006506D9E5: 32 32              xor         dh,byte ptr [rdx]
  000000006506D9E7: 34 33              xor         al,33h

  Summary

      695000 .text
