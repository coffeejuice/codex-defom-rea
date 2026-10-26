Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400569D0: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  00000001400569D4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400569D9: 53                 push        rbx
  00000001400569DA: 48 81 EC A0 01 00  sub         rsp,1A0h
                    00
  00000001400569E1: 48 C7 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400569ED: C7 84 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],0
                    00 00 00 00 00
  00000001400569F8: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001400569FD: FF 15 BD DA DA 01  call        qword ptr [0000000141E044C0h]
  0000000140056A03: 90                 nop
  0000000140056A04: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056A0C: E8 5F 8D 00 00     call        000000014005F770
  0000000140056A11: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140056A16: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140056A1C: 75 24              jne         0000000140056A42
  0000000140056A1E: 48 C7 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],0
                    00 00 00 00 00 00
  0000000140056A2A: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140056A2F: FF 15 AB EF DA 01  call        qword ptr [0000000141E059E0h]
  0000000140056A35: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140056A3D: E9 D3 07 00 00     jmp         0000000140057215
  0000000140056A42: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056A4A: E8 01 E6 FF FF     call        0000000140055050
  0000000140056A4F: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140056A54: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140056A5A: 75 24              jne         0000000140056A80
  0000000140056A5C: 48 C7 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],0
                    00 00 00 00 00 00
  0000000140056A68: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140056A6D: FF 15 6D EF DA 01  call        qword ptr [0000000141E059E0h]
  0000000140056A73: 48 8B 84 24 38 01  mov         rax,qword ptr [rsp+0000000000000138h]
                    00 00
  0000000140056A7B: E9 95 07 00 00     jmp         0000000140057215
  0000000140056A80: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140056A85: E8 46 32 06 00     call        00000001400B9CD0
  0000000140056A8A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140056A8F: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140056A95: 75 24              jne         0000000140056ABB
  0000000140056A97: 48 C7 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],0
                    00 00 00 00 00 00
  0000000140056AA3: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140056AA8: FF 15 32 EF DA 01  call        qword ptr [0000000141E059E0h]
  0000000140056AAE: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140056AB6: E9 5A 07 00 00     jmp         0000000140057215
  0000000140056ABB: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056AC3: E8 58 A0 FF FF     call        0000000140050B20
  0000000140056AC8: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  0000000140056ACF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140056AD4: E8 F7 47 C2 00     call        0000000140C7B2D0
  0000000140056AD9: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140056ADE: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056AE6: E8 B5 37 00 00     call        000000014005A2A0
  0000000140056AEB: 0F B6 C0           movzx       eax,al
  0000000140056AEE: 85 C0              test        eax,eax
  0000000140056AF0: 74 24              je          0000000140056B16
  0000000140056AF2: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140056AF8: 74 1C              je          0000000140056B16
  0000000140056AFA: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056B02: E8 D9 37 00 00     call        000000014005A2E0
  0000000140056B07: 44 8B D8           mov         r11d,eax
  0000000140056B0A: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140056B0F: 44 89 98 60 01 00  mov         dword ptr [rax+0000000000000160h],r11d
                    00
  0000000140056B16: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056B1E: E8 AD 29 00 00     call        00000001400594D0
  0000000140056B23: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140056B28: E8 73 4A C2 00     call        0000000140C7B5A0
  0000000140056B2D: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  0000000140056B31: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056B39: E8 72 3C 00 00     call        000000014005A7B0
  0000000140056B3E: C7 44 24 58 FF FF  mov         dword ptr [rsp+58h],0FFFFFFFFh
                    FF FF
  0000000140056B46: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056B4E: E8 BD E5 FF FF     call        0000000140055110
  0000000140056B53: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140056B58: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140056B5D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140056B60: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140056B65: FF 90 B8 01 00 00  call        qword ptr [rax+00000000000001B8h]
  0000000140056B6B: 89 44 24 54        mov         dword ptr [rsp+54h],eax
  0000000140056B6F: C7 84 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],0
                    00 00 00 00 00
  0000000140056B7A: EB 11              jmp         0000000140056B8D
  0000000140056B7C: 8B 84 24 84 00 00  mov         eax,dword ptr [rsp+0000000000000084h]
                    00
  0000000140056B83: 83 C0 01           add         eax,1
  0000000140056B86: 89 84 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],eax
                    00
  0000000140056B8D: 8B 44 24 54        mov         eax,dword ptr [rsp+54h]
  0000000140056B91: 39 84 24 84 00 00  cmp         dword ptr [rsp+0000000000000084h],eax
                    00
  0000000140056B98: 0F 8D 53 02 00 00  jge         0000000140056DF1
  0000000140056B9E: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140056BA3: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140056BA6: 48 8D 94 24 84 00  lea         rdx,[rsp+0000000000000084h]
                    00 00
  0000000140056BAE: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140056BB3: FF 90 C0 01 00 00  call        qword ptr [rax+00000000000001C0h]
  0000000140056BB9: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140056BC1: 48 83 BC 24 A0 00  cmp         qword ptr [rsp+00000000000000A0h],0
                    00 00 00
  0000000140056BCA: 75 02              jne         0000000140056BCE
  0000000140056BCC: EB AE              jmp         0000000140056B7C
  0000000140056BCE: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140056BD6: E8 B5 BE FE FF     call        0000000140042A90
  0000000140056BDB: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140056BE3: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140056BEB: E8 40 B2 FE FF     call        0000000140041E30
  0000000140056BF0: 0F B6 C0           movzx       eax,al
  0000000140056BF3: 85 C0              test        eax,eax
  0000000140056BF5: 75 19              jne         0000000140056C10
  0000000140056BF7: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140056BFF: E8 6C B2 FE FF     call        0000000140041E70
  0000000140056C04: 0F B6 C0           movzx       eax,al
  0000000140056C07: 85 C0              test        eax,eax
  0000000140056C09: 75 05              jne         0000000140056C10
  0000000140056C0B: E9 6C FF FF FF     jmp         0000000140056B7C
  0000000140056C10: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140056C18: E8 83 4C 01 00     call        000000014006B8A0
  0000000140056C1D: 89 44 24 58        mov         dword ptr [rsp+58h],eax
  0000000140056C21: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140056C26: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140056C29: 4C 8D 44 24 58     lea         r8,[rsp+58h]
  0000000140056C2E: 48 8D 94 24 48 01  lea         rdx,[rsp+0000000000000148h]
                    00 00
  0000000140056C36: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140056C3B: FF 90 08 02 00 00  call        qword ptr [rax+0000000000000208h]
  0000000140056C41: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  0000000140056C49: 48 8B 84 24 70 01  mov         rax,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140056C51: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  0000000140056C59: 48 8B 94 24 78 01  mov         rdx,qword ptr [rsp+0000000000000178h]
                    00 00
  0000000140056C61: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140056C66: FF 15 04 D8 DA 01  call        qword ptr [0000000141E04470h]
  0000000140056C6C: 90                 nop
  0000000140056C6D: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  0000000140056C75: FF 15 65 ED DA 01  call        qword ptr [0000000141E059E0h]
  0000000140056C7B: 8B 44 24 58        mov         eax,dword ptr [rsp+58h]
  0000000140056C7F: 89 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],eax
                    00
  0000000140056C86: 8B 54 24 58        mov         edx,dword ptr [rsp+58h]
  0000000140056C8A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140056C8F: E8 8C 5E C2 00     call        0000000140C7CB20
  0000000140056C94: 48 85 C0           test        rax,rax
  0000000140056C97: 74 0B              je          0000000140056CA4
  0000000140056C99: C7 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],0FFFFFFFFh
                    00 FF FF FF FF
  0000000140056CA4: 4C 8D 84 24 90 00  lea         r8,[rsp+0000000000000090h]
                    00 00
  0000000140056CAC: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140056CB1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140056CB6: E8 C5 62 C2 00     call        0000000140C7CF80
  0000000140056CBB: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140056CC3: 48 83 BC 24 88 00  cmp         qword ptr [rsp+0000000000000088h],0
                    00 00 00
  0000000140056CCC: 0F 84 1A 01 00 00  je          0000000140056DEC
  0000000140056CD2: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140056CDA: 8B 44 24 58        mov         eax,dword ptr [rsp+58h]
  0000000140056CDE: 39 01              cmp         dword ptr [rcx],eax
  0000000140056CE0: 0F 84 06 01 00 00  je          0000000140056DEC
  0000000140056CE6: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140056CEE: 8B 00              mov         eax,dword ptr [rax]
  0000000140056CF0: 89 84 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],eax
                    00
  0000000140056CF7: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140056CFF: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140056D02: 8B 94 24 B0 00 00  mov         edx,dword ptr [rsp+00000000000000B0h]
                    00
  0000000140056D09: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140056D11: FF 50 50           call        qword ptr [rax+50h]
  0000000140056D14: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140056D19: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140056D1C: 8B 94 24 B0 00 00  mov         edx,dword ptr [rsp+00000000000000B0h]
                    00
  0000000140056D23: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140056D28: FF 90 B0 01 00 00  call        qword ptr [rax+00000000000001B0h]
  0000000140056D2E: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140056D33: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140056D36: 44 8B 84 24 B0 00  mov         r8d,dword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140056D3E: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  0000000140056D46: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140056D4B: FF 90 78 01 00 00  call        qword ptr [rax+0000000000000178h]
  0000000140056D51: 90                 nop
  0000000140056D52: 48 8D 94 24 50 01  lea         rdx,[rsp+0000000000000150h]
                    00 00
  0000000140056D5A: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140056D62: FF 15 E8 D1 DA 01  call        qword ptr [0000000141E03F50h]
  0000000140056D68: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  0000000140056D70: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  0000000140056D78: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  0000000140056D80: 48 8B 8C 24 88 01  mov         rcx,qword ptr [rsp+0000000000000188h]
                    00 00
  0000000140056D88: FF 15 9A D4 DA 01  call        qword ptr [0000000141E04228h]
  0000000140056D8E: 48 8B D8           mov         rbx,rax
  0000000140056D91: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056D99: E8 D2 89 00 00     call        000000014005F770
  0000000140056D9E: 48 8B C8           mov         rcx,rax
  0000000140056DA1: E8 2A 3F C2 00     call        0000000140C7ACD0
  0000000140056DA6: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140056DAF: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140056DB8: 45 33 C9           xor         r9d,r9d
  0000000140056DBB: 44 8B 84 24 B0 00  mov         r8d,dword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140056DC3: 48 8B D3           mov         rdx,rbx
  0000000140056DC6: 48 8B C8           mov         rcx,rax
  0000000140056DC9: E8 12 EC 83 00     call        00000001408959E0
  0000000140056DCE: 90                 nop
  0000000140056DCF: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  0000000140056DD7: FF 15 B3 D4 DA 01  call        qword ptr [0000000141E04290h]
  0000000140056DDD: 90                 nop
  0000000140056DDE: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140056DE6: FF 15 F4 EB DA 01  call        qword ptr [0000000141E059E0h]
  0000000140056DEC: E9 8B FD FF FF     jmp         0000000140056B7C
  0000000140056DF1: 0F B6 84 24 B8 01  movzx       eax,byte ptr [rsp+00000000000001B8h]
                    00 00
  0000000140056DF9: 85 C0              test        eax,eax
  0000000140056DFB: 0F 84 51 02 00 00  je          0000000140057052
  0000000140056E01: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056E09: E8 22 E2 FF FF     call        0000000140055030
  0000000140056E0E: 48 8B C8           mov         rcx,rax
  0000000140056E11: E8 2A C7 04 00     call        00000001400A3540
  0000000140056E16: 48 C7 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],0
                    00 00 00 00 00 00
  0000000140056E22: 48 C7 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],0
                    00 00 00 00 00 00
  0000000140056E2E: 48 C7 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],0
                    00 00 00 00 00 00
  0000000140056E3A: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056E42: E8 E9 E1 FF FF     call        0000000140055030
  0000000140056E47: 48 8B C8           mov         rcx,rax
  0000000140056E4A: E8 A1 EA 04 00     call        00000001400A58F0
  0000000140056E4F: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140056E57: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056E5F: E8 1C 8C 00 00     call        000000014005FA80
  0000000140056E64: 89 84 24 B4 00 00  mov         dword ptr [rsp+00000000000000B4h],eax
                    00
  0000000140056E6B: C7 84 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],0
                    00 00 00 00 00
  0000000140056E76: EB 11              jmp         0000000140056E89
  0000000140056E78: 8B 84 24 84 00 00  mov         eax,dword ptr [rsp+0000000000000084h]
                    00
  0000000140056E7F: 83 C0 01           add         eax,1
  0000000140056E82: 89 84 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],eax
                    00
  0000000140056E89: 8B 84 24 B4 00 00  mov         eax,dword ptr [rsp+00000000000000B4h]
                    00
  0000000140056E90: 39 84 24 84 00 00  cmp         dword ptr [rsp+0000000000000084h],eax
                    00
  0000000140056E97: 0F 8D B5 01 00 00  jge         0000000140057052
  0000000140056E9D: 8B 94 24 84 00 00  mov         edx,dword ptr [rsp+0000000000000084h]
                    00
  0000000140056EA4: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056EAC: E8 7F 8C 00 00     call        000000014005FB30
  0000000140056EB1: 89 84 24 DC 00 00  mov         dword ptr [rsp+00000000000000DCh],eax
                    00
  0000000140056EB8: 8B 84 24 84 00 00  mov         eax,dword ptr [rsp+0000000000000084h]
                    00
  0000000140056EBF: 83 C0 01           add         eax,1
  0000000140056EC2: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  0000000140056EC9: 83 BC 24 DC 00 00  cmp         dword ptr [rsp+00000000000000DCh],0FFFFFFFFh
                    00 FF
  0000000140056ED1: 75 35              jne         0000000140056F08
  0000000140056ED3: 41 B1 01           mov         r9b,1
  0000000140056ED6: 45 33 C0           xor         r8d,r8d
  0000000140056ED9: 8B 94 24 E8 00 00  mov         edx,dword ptr [rsp+00000000000000E8h]
                    00
  0000000140056EE0: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140056EE5: E8 66 5A C2 00     call        0000000140C7C950
  0000000140056EEA: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140056EF2: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140056EFA: C7 40 0C 07 00 00  mov         dword ptr [rax+0Ch],7
                    00
  0000000140056F01: E9 72 FF FF FF     jmp         0000000140056E78
  0000000140056F06: EB 52              jmp         0000000140056F5A
  0000000140056F08: 8B 8C 24 DC 00 00  mov         ecx,dword ptr [rsp+00000000000000DCh]
                    00
  0000000140056F0F: E8 AC 97 00 00     call        00000001400606C0
  0000000140056F14: 0F B6 C0           movzx       eax,al
  0000000140056F17: 85 C0              test        eax,eax
  0000000140056F19: 74 30              je          0000000140056F4B
  0000000140056F1B: 41 B1 01           mov         r9b,1
  0000000140056F1E: 45 33 C0           xor         r8d,r8d
  0000000140056F21: 8B 94 24 E8 00 00  mov         edx,dword ptr [rsp+00000000000000E8h]
                    00
  0000000140056F28: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140056F2D: E8 1E 5A C2 00     call        0000000140C7C950
  0000000140056F32: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  0000000140056F3A: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  0000000140056F42: C7 40 0C 05 00 00  mov         dword ptr [rax+0Ch],5
                    00
  0000000140056F49: EB 0F              jmp         0000000140056F5A
  0000000140056F4B: 83 BC 24 DC 00 00  cmp         dword ptr [rsp+00000000000000DCh],0FFFFFF9Dh
                    00 9D
  0000000140056F53: 75 05              jne         0000000140056F5A
  0000000140056F55: E9 1E FF FF FF     jmp         0000000140056E78
  0000000140056F5A: C6 84 24 D8 00 00  mov         byte ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140056F62: 44 0F B6 8C 24 D8  movzx       r9d,byte ptr [rsp+00000000000000D8h]
                    00 00 00
  0000000140056F6B: 44 8B 84 24 84 00  mov         r8d,dword ptr [rsp+0000000000000084h]
                    00 00
  0000000140056F73: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140056F78: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056F80: E8 3B 3F 00 00     call        000000014005AEC0
  0000000140056F85: 48 85 C0           test        rax,rax
  0000000140056F88: 75 05              jne         0000000140056F8F
  0000000140056F8A: E9 E9 FE FF FF     jmp         0000000140056E78
  0000000140056F8F: 8B 94 24 84 00 00  mov         edx,dword ptr [rsp+0000000000000084h]
                    00
  0000000140056F96: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056F9E: E8 AD A2 00 00     call        0000000140061250
  0000000140056FA3: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140056FAB: 48 83 BC 24 B8 00  cmp         qword ptr [rsp+00000000000000B8h],0
                    00 00 00
  0000000140056FB4: 74 4E              je          0000000140057004
  0000000140056FB6: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140056FBE: E8 8D 6B FF FF     call        000000014004DB50
  0000000140056FC3: 85 C0              test        eax,eax
  0000000140056FC5: 75 3D              jne         0000000140057004
  0000000140056FC7: C7 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],0
                    00 00 00 00 00
  0000000140056FD2: 48 8D 94 24 58 01  lea         rdx,[rsp+0000000000000158h]
                    00 00
  0000000140056FDA: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140056FE2: E8 99 6B FF FF     call        000000014004DB80
  0000000140056FE7: 89 44 24 58        mov         dword ptr [rsp+58h],eax
  0000000140056FEB: 44 8B 44 24 58     mov         r8d,dword ptr [rsp+58h]
  0000000140056FF0: 8B 94 24 84 00 00  mov         edx,dword ptr [rsp+0000000000000084h]
                    00
  0000000140056FF7: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140056FFF: E8 DC 9B 00 00     call        0000000140060BE0
  0000000140057004: 41 B1 01           mov         r9b,1
  0000000140057007: 45 33 C0           xor         r8d,r8d
  000000014005700A: 8B 94 24 E8 00 00  mov         edx,dword ptr [rsp+00000000000000E8h]
                    00
  0000000140057011: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140057016: E8 35 59 C2 00     call        0000000140C7C950
  000000014005701B: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140057023: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014005702B: E8 10 71 FF FF     call        000000014004E140
  0000000140057030: 88 84 24 D9 00 00  mov         byte ptr [rsp+00000000000000D9h],al
                    00
  0000000140057037: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014005703F: 0F B6 84 24 D9 00  movzx       eax,byte ptr [rsp+00000000000000D9h]
                    00 00
  0000000140057047: 88 81 D8 2B 00 00  mov         byte ptr [rcx+0000000000002BD8h],al
  000000014005704D: E9 26 FE FF FF     jmp         0000000140056E78
  0000000140057052: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  0000000140057056: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014005705B: E8 80 45 C2 00     call        0000000140C7B5E0
  0000000140057060: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140057068: E8 13 8A 00 00     call        000000014005FA80
  000000014005706D: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140057071: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  0000000140057079: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  0000000140057081: EB 0B              jmp         000000014005708E
  0000000140057083: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  0000000140057087: 83 C0 01           add         eax,1
  000000014005708A: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  000000014005708E: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140057092: 39 44 24 50        cmp         dword ptr [rsp+50h],eax
  0000000140057096: 0F 8D 59 01 00 00  jge         00000001400571F5
  000000014005709C: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001400570A0: 83 C0 01           add         eax,1
  00000001400570A3: 89 84 24 10 01 00  mov         dword ptr [rsp+0000000000000110h],eax
                    00
  00000001400570AA: 8B 94 24 10 01 00  mov         edx,dword ptr [rsp+0000000000000110h]
                    00
  00000001400570B1: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001400570B9: E8 52 06 00 00     call        0000000140057710
  00000001400570BE: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  00000001400570C6: 8B 54 24 50        mov         edx,dword ptr [rsp+50h]
  00000001400570CA: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001400570D2: E8 79 A1 00 00     call        0000000140061250
  00000001400570D7: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  00000001400570DF: 48 83 BC 24 00 01  cmp         qword ptr [rsp+0000000000000100h],0
                    00 00 00
  00000001400570E8: 0F 84 02 01 00 00  je          00000001400571F0
  00000001400570EE: 48 83 BC 24 08 01  cmp         qword ptr [rsp+0000000000000108h],0
                    00 00 00
  00000001400570F7: 0F 84 F3 00 00 00  je          00000001400571F0
  00000001400570FD: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140057105: 8B 84 24 10 01 00  mov         eax,dword ptr [rsp+0000000000000110h]
                    00
  000000014005710C: 39 41 08           cmp         dword ptr [rcx+8],eax
  000000014005710F: 74 05              je          0000000140057116
  0000000140057111: E9 6D FF FF FF     jmp         0000000140057083
  0000000140057116: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  000000014005711E: E8 6D 6D FF FF     call        000000014004DE90
  0000000140057123: 89 84 24 14 01 00  mov         dword ptr [rsp+0000000000000114h],eax
                    00
  000000014005712A: 83 BC 24 14 01 00  cmp         dword ptr [rsp+0000000000000114h],0
                    00 00
  0000000140057132: 0F 8E B8 00 00 00  jle         00000001400571F0
  0000000140057138: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140057140: E8 EB DE FF FF     call        0000000140055030
  0000000140057145: 48 8B C8           mov         rcx,rax
  0000000140057148: E8 23 E8 04 00     call        00000001400A5970
  000000014005714D: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  0000000140057155: 48 8B 84 24 90 01  mov         rax,qword ptr [rsp+0000000000000190h]
                    00 00
  000000014005715D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140057160: 4C 8D 84 24 14 01  lea         r8,[rsp+0000000000000114h]
                    00 00
  0000000140057168: 48 8D 94 24 20 01  lea         rdx,[rsp+0000000000000120h]
                    00 00
  0000000140057170: 48 8B 8C 24 90 01  mov         rcx,qword ptr [rsp+0000000000000190h]
                    00 00
  0000000140057178: FF 90 08 02 00 00  call        qword ptr [rax+0000000000000208h]
  000000014005717E: 90                 nop
  000000014005717F: 48 C7 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],0
                    00 00 00 00 00 00
  000000014005718B: C6 84 24 1C 01 00  mov         byte ptr [rsp+000000000000011Ch],1
                    00 01
  0000000140057193: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  000000014005719B: 8B 40 08           mov         eax,dword ptr [rax+8]
  000000014005719E: 89 84 24 18 01 00  mov         dword ptr [rsp+0000000000000118h],eax
                    00
  00000001400571A5: 48 8D 84 24 18 01  lea         rax,[rsp+0000000000000118h]
                    00 00
  00000001400571AD: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400571B2: 48 8D 84 24 1C 01  lea         rax,[rsp+000000000000011Ch]
                    00 00
  00000001400571BA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400571BF: 4C 8B 8C 24 28 01  mov         r9,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001400571C7: 4C 8B 84 24 08 01  mov         r8,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400571CF: 48 8D 94 24 20 01  lea         rdx,[rsp+0000000000000120h]
                    00 00
  00000001400571D7: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400571DC: E8 BF F6 2F 00     call        00000001403568A0
  00000001400571E1: 90                 nop
  00000001400571E2: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001400571EA: FF 15 F0 E7 DA 01  call        qword ptr [0000000141E059E0h]
  00000001400571F0: E9 8E FE FF FF     jmp         0000000140057083
  00000001400571F5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400571FA: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  0000000140057202: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140057207: FF 15 D3 E7 DA 01  call        qword ptr [0000000141E059E0h]
  000000014005720D: 48 8B 84 24 60 01  mov         rax,qword ptr [rsp+0000000000000160h]
                    00 00
  0000000140057215: 48 81 C4 A0 01 00  add         rsp,1A0h
                    00
  000000014005721C: 5B                 pop         rbx
  000000014005721D: C3                 ret

  Summary

     1E02000 .text
