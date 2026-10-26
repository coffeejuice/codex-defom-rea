Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C90BD0: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140C90BD5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C90BDA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C90BDF: 48 81 EC 08 01 00  sub         rsp,108h
                    00
  0000000140C90BE6: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140C90BF2: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140C90BF7: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140C90BFF: FF 15 53 BA 17 01  call        qword ptr [0000000141E0C658h]
  0000000140C90C05: 90                 nop
  0000000140C90C06: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140C90C0E: EB 0B              jmp         0000000140C90C1B
  0000000140C90C10: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140C90C14: 83 C0 01           add         eax,1
  0000000140C90C17: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140C90C1B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140C90C20: FF 15 2A BA 17 01  call        qword ptr [0000000141E0C650h]
  0000000140C90C26: 39 44 24 28        cmp         dword ptr [rsp+28h],eax
  0000000140C90C2A: 0F 8D 93 02 00 00  jge         0000000140C90EC3
  0000000140C90C30: 44 8B 44 24 28     mov         r8d,dword ptr [rsp+28h]
  0000000140C90C35: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000140C90C3A: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140C90C3F: FF 15 03 BA 17 01  call        qword ptr [0000000141E0C648h]
  0000000140C90C45: 90                 nop
  0000000140C90C46: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140C90C4B: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140C90C50: FF 15 5A B9 17 01  call        qword ptr [0000000141E0C5B0h]
  0000000140C90C56: 90                 nop
  0000000140C90C57: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C90C5C: FF 15 D6 B8 17 01  call        qword ptr [0000000141E0C538h]
  0000000140C90C62: 0F B6 C0           movzx       eax,al
  0000000140C90C65: 85 C0              test        eax,eax
  0000000140C90C67: 0F 85 3A 02 00 00  jne         0000000140C90EA7
  0000000140C90C6D: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140C90C72: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C90C77: FF 15 2B B9 17 01  call        qword ptr [0000000141E0C5A8h]
  0000000140C90C7D: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140C90C85: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140C90C8D: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140C90C95: 48 8D 15 9C D6 C6  lea         rdx,[00000001428FE338h]
                    01
  0000000140C90C9C: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140C90CA4: FF 15 BE 37 17 01  call        qword ptr [0000000141E04468h]
  0000000140C90CAA: 88 44 24 44        mov         byte ptr [rsp+44h],al
  0000000140C90CAE: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140C90CB3: FF 15 27 4D 17 01  call        qword ptr [0000000141E059E0h]
  0000000140C90CB9: 44 0F B6 5C 24 44  movzx       r11d,byte ptr [rsp+44h]
  0000000140C90CBF: 45 85 DB           test        r11d,r11d
  0000000140C90CC2: 74 61              je          0000000140C90D25
  0000000140C90CC4: 0F B6 84 24 20 01  movzx       eax,byte ptr [rsp+0000000000000120h]
                    00 00
  0000000140C90CCC: 85 C0              test        eax,eax
  0000000140C90CCE: 74 50              je          0000000140C90D20
  0000000140C90CD0: 48 8D 44 24 50     lea         rax,[rsp+50h]
  0000000140C90CD5: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C90CDA: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140C90CDF: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C90CE4: FF 15 A6 B8 17 01  call        qword ptr [0000000141E0C590h]
  0000000140C90CEA: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140C90CF2: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C90CFA: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140C90D02: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140C90D0A: E8 A1 F8 FF FF     call        0000000140C905B0
  0000000140C90D0F: 90                 nop
  0000000140C90D10: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140C90D18: 48 8B C8           mov         rcx,rax
  0000000140C90D1B: E8 A0 1B 0A 00     call        0000000140D328C0
  0000000140C90D20: E9 82 01 00 00     jmp         0000000140C90EA7
  0000000140C90D25: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  0000000140C90D2A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C90D2F: FF 15 73 B8 17 01  call        qword ptr [0000000141E0C5A8h]
  0000000140C90D35: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140C90D3D: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140C90D45: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140C90D4D: 48 8D 15 F0 D5 C6  lea         rdx,[00000001428FE344h]
                    01
  0000000140C90D54: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140C90D5C: FF 15 06 37 17 01  call        qword ptr [0000000141E04468h]
  0000000140C90D62: 88 44 24 60        mov         byte ptr [rsp+60h],al
  0000000140C90D66: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140C90D6B: FF 15 6F 4C 17 01  call        qword ptr [0000000141E059E0h]
  0000000140C90D71: 44 0F B6 5C 24 60  movzx       r11d,byte ptr [rsp+60h]
  0000000140C90D77: 45 85 DB           test        r11d,r11d
  0000000140C90D7A: 0F 84 27 01 00 00  je          0000000140C90EA7
  0000000140C90D80: 48 8D 15 C5 D5 C6  lea         rdx,[00000001428FE34Ch]
                    01
  0000000140C90D87: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140C90D8C: FF 15 EE 36 17 01  call        qword ptr [0000000141E04480h]
  0000000140C90D92: 90                 nop
  0000000140C90D93: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140C90D98: FF 15 22 37 17 01  call        qword ptr [0000000141E044C0h]
  0000000140C90D9E: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000140C90DA6: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C90DAE: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000140C90DB6: 4C 8B 8C 24 D8 00  mov         r9,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140C90DBE: 4C 8D 44 24 78     lea         r8,[rsp+78h]
  0000000140C90DC3: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000140C90DCB: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C90DD0: FF 15 92 B8 17 01  call        qword ptr [0000000141E0C668h]
  0000000140C90DD6: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140C90DDE: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140C90DE6: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000140C90DEE: 41 B8 0A 00 00 00  mov         r8d,0Ah
  0000000140C90DF4: 33 D2              xor         edx,edx
  0000000140C90DF6: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140C90DFE: FF 15 8C 32 17 01  call        qword ptr [0000000141E04090h]
  0000000140C90E04: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140C90E08: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140C90E10: FF 15 CA 4B 17 01  call        qword ptr [0000000141E059E0h]
  0000000140C90E16: 90                 nop
  0000000140C90E17: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140C90E1C: FF 15 BE 4B 17 01  call        qword ptr [0000000141E059E0h]
  0000000140C90E22: 90                 nop
  0000000140C90E23: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140C90E28: FF 15 B2 4B 17 01  call        qword ptr [0000000141E059E0h]
  0000000140C90E2E: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140C90E36: 48 83 C1 18        add         rcx,18h
  0000000140C90E3A: E8 A1 3C 37 FF     call        0000000140004AE0
  0000000140C90E3F: 3B 44 24 40        cmp         eax,dword ptr [rsp+40h]
  0000000140C90E43: 7E 62              jle         0000000140C90EA7
  0000000140C90E45: 48 8D 84 24 88 00  lea         rax,[rsp+0000000000000088h]
                    00 00
  0000000140C90E4D: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140C90E55: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140C90E5A: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140C90E62: FF 15 28 B7 17 01  call        qword ptr [0000000141E0C590h]
  0000000140C90E68: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140C90E70: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140C90E78: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  0000000140C90E80: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140C90E88: 48 83 C1 18        add         rcx,18h
  0000000140C90E8C: 8B 54 24 40        mov         edx,dword ptr [rsp+40h]
  0000000140C90E90: E8 1B A7 4B FF     call        000000014014B5B0
  0000000140C90E95: 90                 nop
  0000000140C90E96: 48 8B 94 24 F8 00  mov         rdx,qword ptr [rsp+00000000000000F8h]
                    00 00
  0000000140C90E9E: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140C90EA1: E8 AA FF 56 00     call        0000000141200E50
  0000000140C90EA6: 90                 nop
  0000000140C90EA7: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C90EAC: FF 15 FE B7 17 01  call        qword ptr [0000000141E0C6B0h]
  0000000140C90EB2: 90                 nop
  0000000140C90EB3: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140C90EB8: FF 15 EA B7 17 01  call        qword ptr [0000000141E0C6A8h]
  0000000140C90EBE: E9 4D FD FF FF     jmp         0000000140C90C10
  0000000140C90EC3: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140C90ECB: E8 40 F3 FF FF     call        0000000140C90210
  0000000140C90ED0: 48 8B C8           mov         rcx,rax
  0000000140C90ED3: E8 88 B3 0E 00     call        0000000140D7C260
  0000000140C90ED8: 90                 nop
  0000000140C90ED9: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140C90EDE: FF 15 5C B7 17 01  call        qword ptr [0000000141E0C640h]
  0000000140C90EE4: 90                 nop
  0000000140C90EE5: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140C90EED: FF 15 B5 B7 17 01  call        qword ptr [0000000141E0C6A8h]
  0000000140C90EF3: 48 81 C4 08 01 00  add         rsp,108h
                    00
  0000000140C90EFA: C3                 ret

  Summary

     1E02000 .text
