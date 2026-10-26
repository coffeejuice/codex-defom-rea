Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

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
  0000000140C36F00: 4C

  Summary

     1E02000 .text
