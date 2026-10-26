Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D2B60: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D2B65: 53                 push        rbx
  00000001400D2B66: 48 81 EC C0 01 00  sub         rsp,1C0h
                    00
  00000001400D2B6D: 48 C7 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400D2B79: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  00000001400D2B81: 48 83 C1 38        add         rcx,38h
  00000001400D2B85: E8 86 48 23 00     call        0000000140307410
  00000001400D2B8A: 0F B6 C0           movzx       eax,al
  00000001400D2B8D: 85 C0              test        eax,eax
  00000001400D2B8F: 0F 85 55 06 00 00  jne         00000001400D31EA
  00000001400D2B95: 48 8B 94 24 D0 01  mov         rdx,qword ptr [rsp+00000000000001D0h]
                    00 00
  00000001400D2B9D: 48 83 C2 38        add         rdx,38h
  00000001400D2BA1: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D2BA6: E8 25 6F 18 00     call        0000000140259AD0
  00000001400D2BAB: 90                 nop
  00000001400D2BAC: 48 8D 0D 05 3B CA  lea         rcx,[0000000143D766B8h]
                    03
  00000001400D2BB3: E8 D8 54 1F 00     call        00000001402C8090
  00000001400D2BB8: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400D2BBD: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  00000001400D2BC3: 0F 84 17 06 00 00  je          00000001400D31E0
  00000001400D2BC9: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400D2BD1: 4C 8D 0D 80 66 49  lea         r9,[0000000143569258h]
                    03
  00000001400D2BD8: 4C 8D 05 A1 66 49  lea         r8,[0000000143569280h]
                    03
  00000001400D2BDF: 33 D2              xor         edx,edx
  00000001400D2BE1: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400D2BE6: E8 11 CD 39 01     call        000000014146F8FC
  00000001400D2BEB: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001400D2BF0: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  00000001400D2BF6: 0F 84 E4 05 00 00  je          00000001400D31E0
  00000001400D2BFC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400D2C01: E8 9A 07 F4 FF     call        00000001400133A0
  00000001400D2C06: 48 8B C8           mov         rcx,rax
  00000001400D2C09: E8 72 2B F7 FF     call        0000000140045780
  00000001400D2C0E: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001400D2C13: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  00000001400D2C19: 0F 84 C1 05 00 00  je          00000001400D31E0
  00000001400D2C1F: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400D2C24: E8 87 2C FD FF     call        00000001400A58B0
  00000001400D2C29: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001400D2C2E: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  00000001400D2C34: 0F 84 A6 05 00 00  je          00000001400D31E0
  00000001400D2C3A: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],3
                    00 03 00 00 00
  00000001400D2C45: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D2C4A: E8 91 1E F3 FF     call        0000000140004AE0
  00000001400D2C4F: 89 44 24 7C        mov         dword ptr [rsp+7Ch],eax
  00000001400D2C53: C7 44 24 78 00 00  mov         dword ptr [rsp+78h],0
                    00 00
  00000001400D2C5B: EB 0B              jmp         00000001400D2C68
  00000001400D2C5D: 8B 44 24 78        mov         eax,dword ptr [rsp+78h]
  00000001400D2C61: 83 C0 01           add         eax,1
  00000001400D2C64: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  00000001400D2C68: 8B 44 24 7C        mov         eax,dword ptr [rsp+7Ch]
  00000001400D2C6C: 39 44 24 78        cmp         dword ptr [rsp+78h],eax
  00000001400D2C70: 0F 8D 6A 05 00 00  jge         00000001400D31E0
  00000001400D2C76: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  00000001400D2C7A: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D2C7F: E8 8C DF 08 00     call        0000000140160C10
  00000001400D2C84: 48 83 38 00        cmp         qword ptr [rax],0
  00000001400D2C88: 0F 84 4D 05 00 00  je          00000001400D31DB
  00000001400D2C8E: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  00000001400D2C92: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D2C97: E8 74 DF 08 00     call        0000000140160C10
  00000001400D2C9C: 33 D2              xor         edx,edx
  00000001400D2C9E: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D2CA1: E8 3A 3E 74 00     call        0000000140816AE0
  00000001400D2CA6: 89 84 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],eax
                    00
  00000001400D2CAD: 48 8D 94 24 84 00  lea         rdx,[rsp+0000000000000084h]
                    00 00
  00000001400D2CB5: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400D2CBA: E8 01 51 F7 FF     call        0000000140047DC0
  00000001400D2CBF: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001400D2CC7: 48 83 BC 24 88 00  cmp         qword ptr [rsp+0000000000000088h],0
                    00 00 00
  00000001400D2CD0: 0F 84 05 05 00 00  je          00000001400D31DB
  00000001400D2CD6: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001400D2CDE: E8 0D C4 F8 FF     call        000000014005F0F0
  00000001400D2CE3: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  00000001400D2CEA: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  00000001400D2CEE: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D2CF3: E8 18 DF 08 00     call        0000000140160C10
  00000001400D2CF8: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D2CFB: E8 50 B2 B3 00     call        0000000140C0DF50
  00000001400D2D00: 3B 84 24 80 00 00  cmp         eax,dword ptr [rsp+0000000000000080h]
                    00
  00000001400D2D07: 0F 84 CE 04 00 00  je          00000001400D31DB
  00000001400D2D0D: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  00000001400D2D11: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D2D16: E8 F5 DE 08 00     call        0000000140160C10
  00000001400D2D1B: 8B 94 24 80 00 00  mov         edx,dword ptr [rsp+0000000000000080h]
                    00
  00000001400D2D22: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D2D25: E8 16 53 00 00     call        00000001400D8040
  00000001400D2D2A: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  00000001400D2D2E: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D2D33: E8 D8 DE 08 00     call        0000000140160C10
  00000001400D2D38: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  00000001400D2D40: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D2D43: E8 18 E1 FF FF     call        00000001400D0E60
  00000001400D2D48: 90                 nop
  00000001400D2D49: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400D2D51: E8 BA 46 23 00     call        0000000140307410
  00000001400D2D56: 0F B6 C0           movzx       eax,al
  00000001400D2D59: 85 C0              test        eax,eax
  00000001400D2D5B: 0F 85 6D 04 00 00  jne         00000001400D31CE
  00000001400D2D61: 33 D2              xor         edx,edx
  00000001400D2D63: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400D2D6B: E8 A0 DE 08 00     call        0000000140160C10
  00000001400D2D70: 48 83 38 00        cmp         qword ptr [rax],0
  00000001400D2D74: 0F 84 54 04 00 00  je          00000001400D31CE
  00000001400D2D7A: 33 D2              xor         edx,edx
  00000001400D2D7C: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400D2D84: E8 87 DE 08 00     call        0000000140160C10
  00000001400D2D89: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D2D8C: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  00000001400D2D94: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400D2D9C: FF 15 1E 17 D3 01  call        qword ptr [0000000141E044C0h]
  00000001400D2DA2: 90                 nop
  00000001400D2DA3: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001400D2DAB: FF 15 0F 17 D3 01  call        qword ptr [0000000141E044C0h]
  00000001400D2DB1: 90                 nop
  00000001400D2DB2: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  00000001400D2DBA: FF 15 00 17 D3 01  call        qword ptr [0000000141E044C0h]
  00000001400D2DC0: 90                 nop
  00000001400D2DC1: 48 8D 94 24 10 01  lea         rdx,[rsp+0000000000000110h]
                    00 00
  00000001400D2DC9: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D2DD1: E8 1A 27 F9 FF     call        00000001400654F0
  00000001400D2DD6: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  00000001400D2DDE: 48 8B 84 24 58 01  mov         rax,qword ptr [rsp+0000000000000158h]
                    00 00
  00000001400D2DE6: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  00000001400D2DEE: 48 8B 94 24 60 01  mov         rdx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001400D2DF6: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400D2DFE: FF 15 6C 16 D3 01  call        qword ptr [0000000141E04470h]
  00000001400D2E04: 90                 nop
  00000001400D2E05: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  00000001400D2E0D: FF 15 CD 2B D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D2E13: 48 8D 94 24 18 01  lea         rdx,[rsp+0000000000000118h]
                    00 00
  00000001400D2E1B: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D2E23: E8 98 51 00 00     call        00000001400D7FC0
  00000001400D2E28: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  00000001400D2E30: 48 8B 84 24 68 01  mov         rax,qword ptr [rsp+0000000000000168h]
                    00 00
  00000001400D2E38: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  00000001400D2E40: 48 8B 94 24 70 01  mov         rdx,qword ptr [rsp+0000000000000170h]
                    00 00
  00000001400D2E48: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001400D2E50: FF 15 1A 16 D3 01  call        qword ptr [0000000141E04470h]
  00000001400D2E56: 90                 nop
  00000001400D2E57: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  00000001400D2E5F: FF 15 7B 2B D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D2E65: 48 8D 94 24 20 01  lea         rdx,[rsp+0000000000000120h]
                    00 00
  00000001400D2E6D: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D2E75: E8 86 51 00 00     call        00000001400D8000
  00000001400D2E7A: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  00000001400D2E82: 48 8B 84 24 78 01  mov         rax,qword ptr [rsp+0000000000000178h]
                    00 00
  00000001400D2E8A: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  00000001400D2E92: 48 8B 94 24 80 01  mov         rdx,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001400D2E9A: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  00000001400D2EA2: FF 15 C8 15 D3 01  call        qword ptr [0000000141E04470h]
  00000001400D2EA8: 90                 nop
  00000001400D2EA9: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001400D2EB1: FF 15 29 2B D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D2EB7: 48 C7 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],0
                    00 00 00 00 00 00
  00000001400D2EC3: 48 C7 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],0
                    00 00 00 00 00 00
  00000001400D2ECF: 48 C7 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],0
                    00 00 00 00 00 00
  00000001400D2EDB: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  00000001400D2EE3: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D2EEB: E8 40 EA 84 00     call        0000000140921930
  00000001400D2EF0: 90                 nop
  00000001400D2EF1: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000001400D2EF9: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D2F01: E8 BA 18 23 00     call        00000001403047C0
  00000001400D2F06: 90                 nop
  00000001400D2F07: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  00000001400D2F0F: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D2F17: E8 04 DF FF FF     call        00000001400D0E20
  00000001400D2F1C: 90                 nop
  00000001400D2F1D: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000001400D2F25: E8 F6 58 9C 00     call        0000000140A98820
  00000001400D2F2A: 90                 nop
  00000001400D2F2B: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000001400D2F33: E8 A8 1B F3 FF     call        0000000140004AE0
  00000001400D2F38: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  00000001400D2F3F: C7 84 24 00 01 00  mov         dword ptr [rsp+0000000000000100h],0
                    00 00 00 00 00
  00000001400D2F4A: EB 11              jmp         00000001400D2F5D
  00000001400D2F4C: 8B 84 24 00 01 00  mov         eax,dword ptr [rsp+0000000000000100h]
                    00
  00000001400D2F53: 83 C0 01           add         eax,1
  00000001400D2F56: 89 84 24 00 01 00  mov         dword ptr [rsp+0000000000000100h],eax
                    00
  00000001400D2F5D: 8B 84 24 D8 00 00  mov         eax,dword ptr [rsp+00000000000000D8h]
                    00
  00000001400D2F64: 39 84 24 00 01 00  cmp         dword ptr [rsp+0000000000000100h],eax
                    00
  00000001400D2F6B: 7D 2E              jge         00000001400D2F9B
  00000001400D2F6D: 8B 94 24 00 01 00  mov         edx,dword ptr [rsp+0000000000000100h]
                    00
  00000001400D2F74: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000001400D2F7C: E8 3F 21 F5 FF     call        00000001400250C0
  00000001400D2F81: 4C 8B C0           mov         r8,rax
  00000001400D2F84: 48 8D 94 24 28 01  lea         rdx,[rsp+0000000000000128h]
                    00 00
  00000001400D2F8C: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000001400D2F94: E8 27 8D F9 FF     call        000000014006BCC0
  00000001400D2F99: EB B1              jmp         00000001400D2F4C
  00000001400D2F9B: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D2FA3: E8 F8 4F 00 00     call        00000001400D7FA0
  00000001400D2FA8: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  00000001400D2FAF: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  00000001400D2FB7: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  00000001400D2FBF: 48 8D 94 24 B8 00  lea         rdx,[rsp+00000000000000B8h]
                    00 00
  00000001400D2FC7: 48 8B 8C 24 38 01  mov         rcx,qword ptr [rsp+0000000000000138h]
                    00 00
  00000001400D2FCF: E8 2C 48 84 00     call        0000000140917800
  00000001400D2FD4: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  00000001400D2FDC: 48 8B 84 24 88 01  mov         rax,qword ptr [rsp+0000000000000188h]
                    00 00
  00000001400D2FE4: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  00000001400D2FEC: 48 8D 94 24 40 01  lea         rdx,[rsp+0000000000000140h]
                    00 00
  00000001400D2FF4: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000001400D2FFC: FF 15 4E 0F D3 01  call        qword ptr [0000000141E03F50h]
  00000001400D3002: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  00000001400D300A: 48 8B 84 24 98 01  mov         rax,qword ptr [rsp+0000000000000198h]
                    00 00
  00000001400D3012: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  00000001400D301A: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001400D3022: FF 15 00 12 D3 01  call        qword ptr [0000000141E04228h]
  00000001400D3028: 48 8B D8           mov         rbx,rax
  00000001400D302B: 48 8D 94 24 48 01  lea         rdx,[rsp+0000000000000148h]
                    00 00
  00000001400D3033: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400D303B: FF 15 0F 0F D3 01  call        qword ptr [0000000141E03F50h]
  00000001400D3041: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  00000001400D3049: 48 8B 84 24 A8 01  mov         rax,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001400D3051: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  00000001400D3059: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001400D3061: FF 15 C1 11 D3 01  call        qword ptr [0000000141E04228h]
  00000001400D3067: 48 8B C8           mov         rcx,rax
  00000001400D306A: E8 21 EE 84 00     call        0000000140921E90
  00000001400D306F: 8B C8              mov         ecx,eax
  00000001400D3071: 48 8D 84 24 F8 00  lea         rax,[rsp+00000000000000F8h]
                    00 00
  00000001400D3079: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400D307E: 48 8D 84 24 E0 00  lea         rax,[rsp+00000000000000E0h]
                    00 00
  00000001400D3086: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400D308B: 48 8D 84 24 F0 00  lea         rax,[rsp+00000000000000F0h]
                    00 00
  00000001400D3093: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400D3098: 48 8D 84 24 A8 00  lea         rax,[rsp+00000000000000A8h]
                    00 00
  00000001400D30A0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400D30A5: 48 8D 84 24 C8 00  lea         rax,[rsp+00000000000000C8h]
                    00 00
  00000001400D30AD: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400D30B2: 48 8D 84 24 A0 00  lea         rax,[rsp+00000000000000A0h]
                    00 00
  00000001400D30BA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400D30BF: 4C 8B 8C 24 90 01  mov         r9,qword ptr [rsp+0000000000000190h]
                    00 00
  00000001400D30C7: 44 8B 84 24 80 00  mov         r8d,dword ptr [rsp+0000000000000080h]
                    00 00
  00000001400D30CF: 48 8B D3           mov         rdx,rbx
  00000001400D30D2: E8 59 34 84 00     call        0000000140916530
  00000001400D30D7: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  00000001400D30DE: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  00000001400D30E6: FF 15 A4 11 D3 01  call        qword ptr [0000000141E04290h]
  00000001400D30EC: 90                 nop
  00000001400D30ED: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  00000001400D30F5: FF 15 95 11 D3 01  call        qword ptr [0000000141E04290h]
  00000001400D30FB: 48 83 BC 24 E0 00  cmp         qword ptr [rsp+00000000000000E0h],0
                    00 00 00
  00000001400D3104: 74 61              je          00000001400D3167
  00000001400D3106: 48 83 BC 24 F8 00  cmp         qword ptr [rsp+00000000000000F8h],0
                    00 00 00
  00000001400D310F: 74 56              je          00000001400D3167
  00000001400D3111: 48 83 BC 24 F0 00  cmp         qword ptr [rsp+00000000000000F0h],0
                    00 00 00
  00000001400D311A: 74 20              je          00000001400D313C
  00000001400D311C: 45 33 C9           xor         r9d,r9d
  00000001400D311F: 44 8B 84 24 98 00  mov         r8d,dword ptr [rsp+0000000000000098h]
                    00 00
  00000001400D3127: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400D312F: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D3137: E8 94 DF 84 00     call        00000001409210D0
  00000001400D313C: 48 8B 94 24 F8 00  mov         rdx,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001400D3144: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D314C: E8 9F E2 84 00     call        00000001409213F0
  00000001400D3151: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D3159: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D3161: E8 8A E1 84 00     call        00000001409212F0
  00000001400D3166: 90                 nop
  00000001400D3167: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000001400D316F: E8 7C 95 81 00     call        00000001408EC6F0
  00000001400D3174: 90                 nop
  00000001400D3175: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000001400D317D: E8 1E 50 13 00     call        00000001402081A0
  00000001400D3182: 90                 nop
  00000001400D3183: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000001400D318B: FF 15 4F 28 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D3191: 90                 nop
  00000001400D3192: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400D319A: FF 15 40 28 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D31A0: 90                 nop
  00000001400D31A1: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  00000001400D31A9: FF 15 31 28 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D31AF: 90                 nop
  00000001400D31B0: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001400D31B8: FF 15 22 28 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D31BE: 90                 nop
  00000001400D31BF: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400D31C7: FF 15 13 28 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D31CD: 90                 nop
  00000001400D31CE: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400D31D6: E8 C5 4F 13 00     call        00000001402081A0
  00000001400D31DB: E9 7D FA FF FF     jmp         00000001400D2C5D
  00000001400D31E0: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D31E5: E8 B6 4F 13 00     call        00000001402081A0
  00000001400D31EA: 48 81 C4 C0 01 00  add         rsp,1C0h
                    00
  00000001400D31F1: 5B                 pop         rbx
  00000001400D31F2: C3                 ret

  Summary

     1E02000 .text
