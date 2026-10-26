Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C36C00: 8B 8C 24 28 01 00  mov         ecx,dword ptr [rsp+0000000000000128h]
                    00
  0000000140C36C07: FF 15 5B D8 1C 01  call        qword ptr [0000000141E04468h]
  0000000140C36C0D: 0F B6 C0           movzx       eax,al
  0000000140C36C10: 85 C0              test        eax,eax
  0000000140C36C12: 0F 84 9F 00 00 00  je          0000000140C36CB7
  0000000140C36C18: B9 60 01 00 00     mov         ecx,160h
  0000000140C36C1D: E8 9A 7E 83 00     call        000000014146EABC
  0000000140C36C22: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140C36C27: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140C36C2D: 74 60              je          0000000140C36C8F
  0000000140C36C2F: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36C37: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C36C3B: E8 E0 F4 FD FF     call        0000000140C16120
  0000000140C36C40: C7 44 24 38 64 00  mov         dword ptr [rsp+38h],64h
                    00 00
  0000000140C36C48: C7 44 24 30 64 00  mov         dword ptr [rsp+30h],64h
                    00 00
  0000000140C36C50: C7 44 24 28 2C 01  mov         dword ptr [rsp+28h],12Ch
                    00 00
  0000000140C36C58: C7 44 24 20 F4 01  mov         dword ptr [rsp+20h],1F4h
                    00 00
  0000000140C36C60: 4C 8B 8C 24 20 01  mov         r9,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36C68: 4D 8B 49 10        mov         r9,qword ptr [r9+10h]
  0000000140C36C6C: 4C 8B C0           mov         r8,rax
  0000000140C36C6F: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36C77: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  0000000140C36C7B: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140C36C80: E8 CB AB 01 00     call        0000000140C51850
  0000000140C36C85: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140C36C8D: EB 0C              jmp         0000000140C36C9B
  0000000140C36C8F: 48 C7 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],0
                    00 00 00 00 00 00
  0000000140C36C9B: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140C36CA3: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140C36CA8: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140C36CAD: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C36CB2: E9 68 04 00 00     jmp         0000000140C3711F
  0000000140C36CB7: 48 8D 15 7A 61 C1  lea         rdx,[000000014284CE38h]
                    01
  0000000140C36CBE: 48 8B 8C 24 28 01  mov         rcx,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140C36CC6: FF 15 9C D7 1C 01  call        qword ptr [0000000141E04468h]
  0000000140C36CCC: 0F B6 C0           movzx       eax,al
  0000000140C36CCF: 85 C0              test        eax,eax
  0000000140C36CD1: 0F 84 BB 01 00 00  je          0000000140C36E92
  0000000140C36CD7: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C36CDF: 4C 8D 0D 4A F6 9F  lea         r9,[0000000143636330h]
                    02
  0000000140C36CE6: 4C 8D 05 1B F6 9F  lea         r8,[0000000143636308h]
                    02
  0000000140C36CED: 33 D2              xor         edx,edx
  0000000140C36CEF: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36CF7: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C36CFB: E8 FC 8B 83 00     call        000000014146F8FC
  0000000140C36D00: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C36D05: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  0000000140C36D0B: 0F 84 C3 00 00 00  je          0000000140C36DD4
  0000000140C36D11: B9 30 01 00 00     mov         ecx,130h
  0000000140C36D16: E8 A1 7D 83 00     call        000000014146EABC
  0000000140C36D1B: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140C36D20: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  0000000140C36D26: 0F 84 80 00 00 00  je          0000000140C36DAC
  0000000140C36D2C: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C36D31: E8 6A 71 FD FF     call        0000000140C0DEA0
  0000000140C36D36: 8B F8              mov         edi,eax
  0000000140C36D38: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C36D3D: E8 3E 71 FD FF     call        0000000140C0DE80
  0000000140C36D42: 8B D8              mov         ebx,eax
  0000000140C36D44: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36D4C: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C36D50: E8 CB F3 FD FF     call        0000000140C16120
  0000000140C36D55: C7 44 24 48 64 00  mov         dword ptr [rsp+48h],64h
                    00 00
  0000000140C36D5D: C7 44 24 40 64 00  mov         dword ptr [rsp+40h],64h
                    00 00
  0000000140C36D65: C7 44 24 38 2C 01  mov         dword ptr [rsp+38h],12Ch
                    00 00
  0000000140C36D6D: C7 44 24 30 F4 01  mov         dword ptr [rsp+30h],1F4h
                    00 00
  0000000140C36D75: 89 7C 24 28        mov         dword ptr [rsp+28h],edi
  0000000140C36D79: 89 5C 24 20        mov         dword ptr [rsp+20h],ebx
  0000000140C36D7D: 4C 8B 8C 24 20 01  mov         r9,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36D85: 4D 8B 49 10        mov         r9,qword ptr [r9+10h]
  0000000140C36D89: 4C 8B C0           mov         r8,rax
  0000000140C36D8C: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36D94: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  0000000140C36D98: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140C36D9D: E8 0E 49 FF FF     call        0000000140C2B6B0
  0000000140C36DA2: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000140C36DAA: EB 0C              jmp         0000000140C36DB8
  0000000140C36DAC: 48 C7 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],0
                    00 00 00 00 00 00
  0000000140C36DB8: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C36DC0: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140C36DC5: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140C36DCA: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C36DCF: E9 B9 00 00 00     jmp         0000000140C36E8D
  0000000140C36DD4: B9 30 01 00 00     mov         ecx,130h
  0000000140C36DD9: E8 DE 7C 83 00     call        000000014146EABC
  0000000140C36DDE: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140C36DE6: 48 83 BC 24 88 00  cmp         qword ptr [rsp+0000000000000088h],0
                    00 00 00
  0000000140C36DEF: 74 73              je          0000000140C36E64
  0000000140C36DF1: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36DF9: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C36DFD: E8 1E F3 FD FF     call        0000000140C16120
  0000000140C36E02: C7 44 24 48 64 00  mov         dword ptr [rsp+48h],64h
                    00 00
  0000000140C36E0A: C7 44 24 40 64 00  mov         dword ptr [rsp+40h],64h
                    00 00
  0000000140C36E12: C7 44 24 38 2C 01  mov         dword ptr [rsp+38h],12Ch
                    00 00
  0000000140C36E1A: C7 44 24 30 F4 01  mov         dword ptr [rsp+30h],1F4h
                    00 00
  0000000140C36E22: C7 44 24 28 0A 00  mov         dword ptr [rsp+28h],0Ah
                    00 00
  0000000140C36E2A: C7 44 24 20 0A 00  mov         dword ptr [rsp+20h],0Ah
                    00 00
  0000000140C36E32: 4C 8B 8C 24 20 01  mov         r9,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36E3A: 4D 8B 49 10        mov         r9,qword ptr [r9+10h]
  0000000140C36E3E: 4C 8B C0           mov         r8,rax
  0000000140C36E41: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36E49: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  0000000140C36E4D: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140C36E55: E8 56 48 FF FF     call        0000000140C2B6B0
  0000000140C36E5A: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000140C36E62: EB 0C              jmp         0000000140C36E70
  0000000140C36E64: 48 C7 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],0
                    00 00 00 00 00 00
  0000000140C36E70: 48 8B 84 24 D8 00  mov         rax,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140C36E78: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140C36E80: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C36E88: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C36E8D: E9 8D 02 00 00     jmp         0000000140C3711F
  0000000140C36E92: 48 8D 15 B7 5F C1  lea         rdx,[000000014284CE50h]
                    01
  0000000140C36E99: 48 8B 8C 24 28 01  mov         rcx,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140C36EA1: FF 15 C1 D5 1C 01  call        qword ptr [0000000141E04468h]
  0000000140C36EA7: 0F B6 C0           movzx       eax,al
  0000000140C36EAA: 85 C0              test        eax,eax
  0000000140C36EAC: 0F 84 AE 00 00 00  je          0000000140C36F60
  0000000140C36EB2: B9 08 01 00 00     mov         ecx,108h
  0000000140C36EB7: E8 00 7C 83 00     call        000000014146EABC
  0000000140C36EBC: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140C36EC4: 48 83 BC 24 98 00  cmp         qword ptr [rsp+0000000000000098h],0
                    00 00 00
  0000000140C36ECD: 74 63              je          0000000140C36F32
  0000000140C36ECF: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36ED7: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C36EDB: E8 40 F2 FD FF     call        0000000140C16120
  0000000140C36EE0: C7 44 24 38 64 00  mov         dword ptr [rsp+38h],64h
                    00 00
  0000000140C36EE8: C7 44 24 30 64 00  mov         dword ptr [rsp+30h],64h
                    00 00
  0000000140C36EF0: C7 44 24 28 2C 01  mov         dword ptr [rsp+28h],12Ch
                    00 00
  0000000140C36EF8: C7 44 24 20 F4 01  mov         dword ptr [rsp+20h],1F4h
                    00 00
  0000000140C36F00: 4C 8B 8C 24 20 01  mov         r9,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36F08: 4D 8B 49 10        mov         r9,qword ptr [r9+10h]
  0000000140C36F0C: 4C 8B C0           mov         r8,rax
  0000000140C36F0F: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36F17: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  0000000140C36F1B: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140C36F23: E8 08 6B 01 00     call        0000000140C4DA30
  0000000140C36F28: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140C36F30: EB 0C              jmp         0000000140C36F3E
  0000000140C36F32: 48 C7 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],0
                    00 00 00 00 00 00
  0000000140C36F3E: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140C36F46: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140C36F4E: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140C36F56: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C36F5B: E9 BF 01 00 00     jmp         0000000140C3711F
  0000000140C36F60: 48 8D 15 09 5F C1  lea         rdx,[000000014284CE70h]
                    01
  0000000140C36F67: 48 8B 8C 24 28 01  mov         rcx,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140C36F6F: FF 15 F3 D4 1C 01  call        qword ptr [0000000141E04468h]
  0000000140C36F75: 0F B6 C0           movzx       eax,al
  0000000140C36F78: 85 C0              test        eax,eax
  0000000140C36F7A: 0F 84 AE 00 00 00  je          0000000140C3702E
  0000000140C36F80: B9 58 01 00 00     mov         ecx,158h
  0000000140C36F85: E8 32 7B 83 00     call        000000014146EABC
  0000000140C36F8A: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140C36F92: 48 83 BC 24 A8 00  cmp         qword ptr [rsp+00000000000000A8h],0
                    00 00 00
  0000000140C36F9B: 74 63              je          0000000140C37000
  0000000140C36F9D: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36FA5: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C36FA9: E8 72 F1 FD FF     call        0000000140C16120
  0000000140C36FAE: C7 44 24 38 64 00  mov         dword ptr [rsp+38h],64h
                    00 00
  0000000140C36FB6: C7 44 24 30 64 00  mov         dword ptr [rsp+30h],64h
                    00 00
  0000000140C36FBE: C7 44 24 28 2C 01  mov         dword ptr [rsp+28h],12Ch
                    00 00
  0000000140C36FC6: C7 44 24 20 F4 01  mov         dword ptr [rsp+20h],1F4h
                    00 00
  0000000140C36FCE: 4C 8B 8C 24 20 01  mov         r9,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36FD6: 4D 8B 49 10        mov         r9,qword ptr [r9+10h]
  0000000140C36FDA: 4C 8B C0           mov         r8,rax
  0000000140C36FDD: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C36FE5: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  0000000140C36FE9: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140C36FF1: E8 CA 40 01 00     call        0000000140C4B0C0
  0000000140C36FF6: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000140C36FFE: EB 0C              jmp         0000000140C3700C
  0000000140C37000: 48 C7 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],0
                    00 00 00 00 00 00
  0000000140C3700C: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140C37014: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140C3701C: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140C37024: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C37029: E9 F1 00 00 00     jmp         0000000140C3711F
  0000000140C3702E: 48 8D 15 53 5E C1  lea         rdx,[000000014284CE88h]
                    01
  0000000140C37035: 48 8B 8C 24 28 01  mov         rcx,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140C3703D: FF 15 25 D4 1C 01  call        qword ptr [0000000141E04468h]
  0000000140C37043: 0F B6 C0           movzx       eax,al
  0000000140C37046: 85 C0              test        eax,eax
  0000000140C37048: 0F 84 B7 00 00 00  je          0000000140C37105
  0000000140C3704E: B9 50 01 00 00     mov         ecx,150h
  0000000140C37053: E8 64 7A 83 00     call        000000014146EABC
  0000000140C37058: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140C37060: 48 83 BC 24 B8 00  cmp         qword ptr [rsp+00000000000000B8h],0
                    00 00 00
  0000000140C37069: 74 63              je          0000000140C370CE
  0000000140C3706B: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C37073: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C37077: E8 A4 F0 FD FF     call        0000000140C16120
  0000000140C3707C: C7 44 24 38 64 00  mov         dword ptr [rsp+38h],64h
                    00 00
  0000000140C37084: C7 44 24 30 64 00  mov         dword ptr [rsp+30h],64h
                    00 00
  0000000140C3708C: C7 44 24 28 2C 01  mov         dword ptr [rsp+28h],12Ch
                    00 00
  0000000140C37094: C7 44 24 20 F4 01  mov         dword ptr [rsp+20h],1F4h
                    00 00
  0000000140C3709C: 4C 8B 8C 24 20 01  mov         r9,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C370A4: 4D 8B 49 10        mov         r9,qword ptr [r9+10h]
  0000000140C370A8: 4C 8B C0           mov         r8,rax
  0000000140C370AB: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C370B3: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  0000000140C370B7: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140C370BF: E8 2C 26 01 00     call        0000000140C496F0
  0000000140C370C4: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140C370CC: EB 0C              jmp         0000000140C370DA
  0000000140C370CE: 48 C7 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],0
                    00 00 00 00 00 00
  0000000140C370DA: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140C370E2: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140C370EA: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C370F2: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C370F7: 33 D2              xor         edx,edx
  0000000140C370F9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C370FE: E8 2D D3 7D FF     call        0000000140414430
  0000000140C37103: EB 1A              jmp         0000000140C3711F
  0000000140C37105: 48 8B 94 24 28 01  mov         rdx,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140C3710D: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C37115: E8 96 1A 00 00     call        0000000140C38BB0
  0000000140C3711A: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C3711F: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C37124: 48 81 C4 08 01 00  add         rsp,108h
                    00
  0000000140C3712B: 5F                 pop         rdi
  0000000140C3712C: 5B                 pop         rbx
  0000000140C3712D: C3                 ret
  0000000140C3712E: CC                 int         3
  0000000140C3712F: CC                 int         3
  0000000140C37130: CC                 int         3
  0000000140C37131: CC                 int         3
  0000000140C37132: CC                 int         3
  0000000140C37133: CC                 int         3
  0000000140C37134: CC                 int         3
  0000000140C37135: CC                 int         3
  0000000140C37136: CC                 int         3
  0000000140C37137: CC                 int         3
  0000000140C37138: CC                 int         3
  0000000140C37139: CC                 int         3
  0000000140C3713A: CC                 int         3
  0000000140C3713B: CC                 int         3
  0000000140C3713C: CC                 int         3
  0000000140C3713D: CC                 int         3
  0000000140C3713E: CC                 int         3
  0000000140C3713F: CC                 int         3
  0000000140C37140: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140C37145: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C3714A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C3714F: 48 83 EC 68        sub         rsp,68h
  0000000140C37153: 48 C7 44 24 50 FE  mov         qword ptr [rsp+50h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C3715C: C7 44 24 48 00 00  mov         dword ptr [rsp+48h],0
                    00 00
  0000000140C37164: 48 8D 0D 4D F5 13  lea         rcx,[0000000143D766B8h]
                    03
  0000000140C3716B: E8 20 0F 69 FF     call        00000001402C8090
  0000000140C37170: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C37175: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C3717A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C3717D: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140C37182: FF 90 68 02 00 00  call        qword ptr [rax+0000000000000268h]
  0000000140C37188: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C3718D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C37192: E8 49 88 55 FF     call        000000014018F9E0
  0000000140C37197: 90                 nop
  0000000140C37198: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140C3719E: 0F 84 8C 00 00 00  je          0000000140C37230
  0000000140C371A4: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C371A9: E8 F2 4F 04 00     call        0000000140C7C1A0
  0000000140C371AE: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140C371B2: C7 44 24 3C 00 00  mov         dword ptr [rsp+3Ch],0
                    00 00
  0000000140C371BA: EB 0B              jmp         0000000140C371C7
  0000000140C371BC: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140C371C0: 83 C0 01           add         eax,1
  0000000140C371C3: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000140C371C7: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140C371CB: 39 44 24 3C        cmp         dword ptr [rsp+3Ch],eax
  0000000140C371CF: 7D 5F              jge         0000000140C37230
  0000000140C371D1: 8B 54 24 3C        mov         edx,dword ptr [rsp+3Ch]
  0000000140C371D5: 83 C2 01           add         edx,1
  0000000140C371D8: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C371DD: E8 EE 3C 04 00     call        0000000140C7AED0
  0000000140C371E2: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C371E7: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140C371ED: 74 3F              je          0000000140C3722E
  0000000140C371EF: 0F B6 84 24 80 00  movzx       eax,byte ptr [rsp+0000000000000080h]
                    00 00
  0000000140C371F7: 85 C0              test        eax,eax
  0000000140C371F9: 74 20              je          0000000140C3721B
  0000000140C371FB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C37200: 83

  Summary

     1E02000 .text
