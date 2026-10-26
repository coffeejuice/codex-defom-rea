Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D35A80: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D35A85: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D35A8A: 48 81 EC A8 01 00  sub         rsp,1A8h
                    00
  0000000140D35A91: 48 C7 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D35A9D: C7 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],0
                    00 00 00 00 00
  0000000140D35AA8: 48 8B 94 24 B0 01  mov         rdx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D35AB0: 48 83 C2 30        add         rdx,30h
  0000000140D35AB4: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140D35ABC: FF 15 BE F1 0C 01  call        qword ptr [0000000141E04C80h]
  0000000140D35AC2: 0F B6 C0           movzx       eax,al
  0000000140D35AC5: 85 C0              test        eax,eax
  0000000140D35AC7: 74 05              je          0000000140D35ACE
  0000000140D35AC9: E9 86 06 00 00     jmp         0000000140D36154
  0000000140D35ACE: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D35AD6: E8 45 EE FF FF     call        0000000140D34920
  0000000140D35ADB: 48 8D 15 7E 0C CD  lea         rdx,[0000000142A06760h]
                    01
  0000000140D35AE2: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140D35AE7: FF 15 23 E6 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D35AED: 48 8B D0           mov         rdx,rax
  0000000140D35AF0: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140D35AF8: E8 63 C9 40 FF     call        0000000140142460
  0000000140D35AFD: 0F B6 C0           movzx       eax,al
  0000000140D35B00: 85 C0              test        eax,eax
  0000000140D35B02: 74 17              je          0000000140D35B1B
  0000000140D35B04: BA 01 00 00 00     mov         edx,1
  0000000140D35B09: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D35B11: E8 8A EE FF FF     call        0000000140D349A0
  0000000140D35B16: E9 1F 06 00 00     jmp         0000000140D3613A
  0000000140D35B1B: 48 8D 15 4E 0C CD  lea         rdx,[0000000142A06770h]
                    01
  0000000140D35B22: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140D35B27: FF 15 E3 E5 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D35B2D: 48 8B D0           mov         rdx,rax
  0000000140D35B30: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140D35B38: E8 23 C9 40 FF     call        0000000140142460
  0000000140D35B3D: 0F B6 C0           movzx       eax,al
  0000000140D35B40: 85 C0              test        eax,eax
  0000000140D35B42: 0F 84 DC 02 00 00  je          0000000140D35E24
  0000000140D35B48: B9 30 00 00 00     mov         ecx,30h
  0000000140D35B4D: E8 6A 8F 73 00     call        000000014146EABC
  0000000140D35B52: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140D35B57: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  0000000140D35B5D: 74 5D              je          0000000140D35BBC
  0000000140D35B5F: 48 8D 15 1A 0C CD  lea         rdx,[0000000142A06780h]
                    01
  0000000140D35B66: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140D35B6E: FF 15 9C E5 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D35B74: 48 8B D0           mov         rdx,rax
  0000000140D35B77: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140D35B7F: FF 15 93 E5 0C 01  call        qword ptr [0000000141E04118h]
  0000000140D35B85: 90                 nop
  0000000140D35B86: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D35B8D: 83 C8 01           or          eax,1
  0000000140D35B90: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  0000000140D35B97: 45 33 C9           xor         r9d,r9d
  0000000140D35B9A: 41 B8 02 00 00 00  mov         r8d,2
  0000000140D35BA0: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000140D35BA8: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D35BAD: E8 CE 5B 07 00     call        0000000140DAB780
  0000000140D35BB2: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  0000000140D35BBA: EB 0C              jmp         0000000140D35BC8
  0000000140D35BBC: 48 C7 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],0
                    00 00 00 00 00 00
  0000000140D35BC8: 48 8B 84 24 68 01  mov         rax,qword ptr [rsp+0000000000000168h]
                    00 00
  0000000140D35BD0: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140D35BD5: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140D35BDA: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D35BDF: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D35BE6: 83 E0 01           and         eax,1
  0000000140D35BE9: 85 C0              test        eax,eax
  0000000140D35BEB: 74 16              je          0000000140D35C03
  0000000140D35BED: 83 A4 24 58 01 00  and         dword ptr [rsp+0000000000000158h],0FFFFFFFEh
                    00 FE
  0000000140D35BF5: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140D35BFD: FF 15 DD FD 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D35C03: BA 01 00 00 00     mov         edx,1
  0000000140D35C08: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D35C10: E8 8B ED FF FF     call        0000000140D349A0
  0000000140D35C15: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D35C1A: 4C 8B 44 24 28     mov         r8,qword ptr [rsp+28h]
  0000000140D35C1F: BA 01 00 00 00     mov         edx,1
  0000000140D35C24: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D35C29: E8 A2 5F 07 00     call        0000000140DABBD0
  0000000140D35C2E: B9 40 00 00 00     mov         ecx,40h
  0000000140D35C33: E8 84 8E 73 00     call        000000014146EABC
  0000000140D35C38: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140D35C40: 48 83 BC 24 98 00  cmp         qword ptr [rsp+0000000000000098h],0
                    00 00 00
  0000000140D35C49: 74 5A              je          0000000140D35CA5
  0000000140D35C4B: 48 8D 15 3E 0B CD  lea         rdx,[0000000142A06790h]
                    01
  0000000140D35C52: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140D35C5A: FF 15 B0 E4 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D35C60: 48 8B D0           mov         rdx,rax
  0000000140D35C63: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140D35C6B: FF 15 A7 E4 0C 01  call        qword ptr [0000000141E04118h]
  0000000140D35C71: 90                 nop
  0000000140D35C72: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D35C79: 83 C8 02           or          eax,2
  0000000140D35C7C: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  0000000140D35C83: 45 33 C0           xor         r8d,r8d
  0000000140D35C86: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  0000000140D35C8E: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140D35C96: E8 35 0A 0A 00     call        0000000140DD66D0
  0000000140D35C9B: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  0000000140D35CA3: EB 0C              jmp         0000000140D35CB1
  0000000140D35CA5: 48 C7 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],0
                    00 00 00 00 00 00
  0000000140D35CB1: 48 8B 84 24 70 01  mov         rax,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140D35CB9: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D35CC1: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D35CC9: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D35CCE: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D35CD5: 83 E0 02           and         eax,2
  0000000140D35CD8: 85 C0              test        eax,eax
  0000000140D35CDA: 74 16              je          0000000140D35CF2
  0000000140D35CDC: 83 A4 24 58 01 00  and         dword ptr [rsp+0000000000000158h],0FFFFFFFDh
                    00 FD
  0000000140D35CE4: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140D35CEC: FF 15 EE FC 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D35CF2: 48 8D 15 A7 0A CD  lea         rdx,[0000000142A067A0h]
                    01
  0000000140D35CF9: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140D35D01: FF 15 09 E4 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D35D07: 48 8B D0           mov         rdx,rax
  0000000140D35D0A: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140D35D0F: E8 FC 0E 0A 00     call        0000000140DD6C10
  0000000140D35D14: B9 40 00 00 00     mov         ecx,40h
  0000000140D35D19: E8 9E 8D 73 00     call        000000014146EABC
  0000000140D35D1E: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140D35D26: 48 83 BC 24 C0 00  cmp         qword ptr [rsp+00000000000000C0h],0
                    00 00 00
  0000000140D35D2F: 74 5A              je          0000000140D35D8B
  0000000140D35D31: 48 8D 15 70 0A CD  lea         rdx,[0000000142A067A8h]
                    01
  0000000140D35D38: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140D35D40: FF 15 CA E3 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D35D46: 48 8B D0           mov         rdx,rax
  0000000140D35D49: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140D35D51: FF 15 C1 E3 0C 01  call        qword ptr [0000000141E04118h]
  0000000140D35D57: 90                 nop
  0000000140D35D58: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D35D5F: 83 C8 04           or          eax,4
  0000000140D35D62: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  0000000140D35D69: 45 33 C0           xor         r8d,r8d
  0000000140D35D6C: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  0000000140D35D74: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D35D7C: E8 4F 09 0A 00     call        0000000140DD66D0
  0000000140D35D81: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  0000000140D35D89: EB 0C              jmp         0000000140D35D97
  0000000140D35D8B: 48 C7 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],0
                    00 00 00 00 00 00
  0000000140D35D97: 48 8B 84 24 78 01  mov         rax,qword ptr [rsp+0000000000000178h]
                    00 00
  0000000140D35D9F: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140D35DA7: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D35DAF: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D35DB4: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D35DBB: 83 E0 04           and         eax,4
  0000000140D35DBE: 85 C0              test        eax,eax
  0000000140D35DC0: 74 16              je          0000000140D35DD8
  0000000140D35DC2: 83 A4 24 58 01 00  and         dword ptr [rsp+0000000000000158h],0FFFFFFFBh
                    00 FB
  0000000140D35DCA: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140D35DD2: FF 15 08 FC 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D35DD8: 48 8D 15 E1 09 CD  lea         rdx,[0000000142A067C0h]
                    01
  0000000140D35DDF: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000140D35DE7: FF 15 23 E3 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D35DED: 48 8B D0           mov         rdx,rax
  0000000140D35DF0: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D35DF5: E8 16 0E 0A 00     call        0000000140DD6C10
  0000000140D35DFA: 4C 8B 44 24 38     mov         r8,qword ptr [rsp+38h]
  0000000140D35DFF: 33 D2              xor         edx,edx
  0000000140D35E01: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140D35E06: E8 C5 5D 07 00     call        0000000140DABBD0
  0000000140D35E0B: 4C 8B 44 24 20     mov         r8,qword ptr [rsp+20h]
  0000000140D35E10: BA 01 00 00 00     mov         edx,1
  0000000140D35E15: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140D35E1A: E8 B1 5D 07 00     call        0000000140DABBD0
  0000000140D35E1F: E9 16 03 00 00     jmp         0000000140D3613A
  0000000140D35E24: 48 8D 15 9D 09 CD  lea         rdx,[0000000142A067C8h]
                    01
  0000000140D35E2B: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140D35E33: FF 15 D7 E2 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D35E39: 48 8B D0           mov         rdx,rax
  0000000140D35E3C: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140D35E44: E8 17 C6 40 FF     call        0000000140142460
  0000000140D35E49: 0F B6 C0           movzx       eax,al
  0000000140D35E4C: 85 C0              test        eax,eax
  0000000140D35E4E: 0F 84 E6 02 00 00  je          0000000140D3613A
  0000000140D35E54: B9 30 00 00 00     mov         ecx,30h
  0000000140D35E59: E8 5E 8C 73 00     call        000000014146EABC
  0000000140D35E5E: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140D35E66: 48 83 BC 24 F0 00  cmp         qword ptr [rsp+00000000000000F0h],0
                    00 00 00
  0000000140D35E6F: 74 60              je          0000000140D35ED1
  0000000140D35E71: 48 8D 15 60 09 CD  lea         rdx,[0000000142A067D8h]
                    01
  0000000140D35E78: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140D35E80: FF 15 8A E2 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D35E86: 48 8B D0           mov         rdx,rax
  0000000140D35E89: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140D35E91: FF 15 81 E2 0C 01  call        qword ptr [0000000141E04118h]
  0000000140D35E97: 90                 nop
  0000000140D35E98: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D35E9F: 83 C8 08           or          eax,8
  0000000140D35EA2: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  0000000140D35EA9: 45 33 C9           xor         r9d,r9d
  0000000140D35EAC: 41 B8 02 00 00 00  mov         r8d,2
  0000000140D35EB2: 48 8D 94 24 F8 00  lea         rdx,[rsp+00000000000000F8h]
                    00 00
  0000000140D35EBA: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D35EC2: E8 B9 58 07 00     call        0000000140DAB780
  0000000140D35EC7: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  0000000140D35ECF: EB 0C              jmp         0000000140D35EDD
  0000000140D35ED1: 48 C7 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],0
                    00 00 00 00 00 00
  0000000140D35EDD: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  0000000140D35EE5: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000140D35EED: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140D35EF5: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D35EFA: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D35F01: 83 E0 08           and         eax,8
  0000000140D35F04: 85 C0              test        eax,eax
  0000000140D35F06: 74 16              je          0000000140D35F1E
  0000000140D35F08: 83 A4 24 58 01 00  and         dword ptr [rsp+0000000000000158h],0FFFFFFF7h
                    00 F7
  0000000140D35F10: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140D35F18: FF 15 C2 FA 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D35F1E: BA 01 00 00 00     mov         edx,1
  0000000140D35F23: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D35F2B: E8 70 EA FF FF     call        0000000140D349A0
  0000000140D35F30: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140D35F35: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000140D35F3A: BA 01 00 00 00     mov         edx,1
  0000000140D35F3F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D35F44: E8 87 5C 07 00     call        0000000140DABBD0
  0000000140D35F49: B9 40 00 00 00     mov         ecx,40h
  0000000140D35F4E: E8 69 8B 73 00     call        000000014146EABC
  0000000140D35F53: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  0000000140D35F5B: 48 83 BC 24 10 01  cmp         qword ptr [rsp+0000000000000110h],0
                    00 00 00
  0000000140D35F64: 74 5A              je          0000000140D35FC0
  0000000140D35F66: 48 8D 15 7B 08 CD  lea         rdx,[0000000142A067E8h]
                    01
  0000000140D35F6D: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  0000000140D35F75: FF 15 95 E1 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D35F7B: 48 8B D0           mov         rdx,rax
  0000000140D35F7E: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000140D35F86: FF 15 8C E1 0C 01  call        qword ptr [0000000141E04118h]
  0000000140D35F8C: 90                 nop
  0000000140D35F8D: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D35F94: 83 C8 10           or          eax,10h
  0000000140D35F97: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  0000000140D35F9E: 45 33 C0           xor         r8d,r8d
  0000000140D35FA1: 48 8D 94 24 18 01  lea         rdx,[rsp+0000000000000118h]
                    00 00
  0000000140D35FA9: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D35FB1: E8 1A 07 0A 00     call        0000000140DD66D0
  0000000140D35FB6: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  0000000140D35FBE: EB 0C              jmp         0000000140D35FCC
  0000000140D35FC0: 48 C7 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],0
                    00 00 00 00 00 00
  0000000140D35FCC: 48 8B 84 24 88 01  mov         rax,qword ptr [rsp+0000000000000188h]
                    00 00
  0000000140D35FD4: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  0000000140D35FDC: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140D35FE4: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140D35FE9: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D35FF0: 83 E0 10           and         eax,10h
  0000000140D35FF3: 85 C0              test        eax,eax
  0000000140D35FF5: 74 16              je          0000000140D3600D
  0000000140D35FF7: 83 A4 24 58 01 00  and         dword ptr [rsp+0000000000000158h],0FFFFFFEFh
                    00 EF
  0000000140D35FFF: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000140D36007: FF 15 D3 F9 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D3600D: 48 8D 15 E4 07 CD  lea         rdx,[0000000142A067F8h]
                    01
  0000000140D36014: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  0000000140D3601C: FF 15 EE E0 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D36022: 48 8B D0           mov         rdx,rax
  0000000140D36025: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D3602A: E8 E1 0B 0A 00     call        0000000140DD6C10
  0000000140D3602F: B9 40 00 00 00     mov         ecx,40h
  0000000140D36034: E8 83 8A 73 00     call        000000014146EABC
  0000000140D36039: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  0000000140D36041: 48 83 BC 24 38 01  cmp         qword ptr [rsp+0000000000000138h],0
                    00 00 00
  0000000140D3604A: 74 5A              je          0000000140D360A6
  0000000140D3604C: 48 8D 15 AD 07 CD  lea         rdx,[0000000142A06800h]
                    01
  0000000140D36053: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  0000000140D3605B: FF 15 AF E0 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D36061: 48 8B D0           mov         rdx,rax
  0000000140D36064: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000140D3606C: FF 15 A6 E0 0C 01  call        qword ptr [0000000141E04118h]
  0000000140D36072: 90                 nop
  0000000140D36073: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D3607A: 83 C8 20           or          eax,20h
  0000000140D3607D: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  0000000140D36084: 45 33 C0           xor         r8d,r8d
  0000000140D36087: 48 8D 94 24 40 01  lea         rdx,[rsp+0000000000000140h]
                    00 00
  0000000140D3608F: 48 8B 8C 24 38 01  mov         rcx,qword ptr [rsp+0000000000000138h]
                    00 00
  0000000140D36097: E8 34 06 0A 00     call        0000000140DD66D0
  0000000140D3609C: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  0000000140D360A4: EB 0C              jmp         0000000140D360B2
  0000000140D360A6: 48 C7 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],0
                    00 00 00 00 00 00
  0000000140D360B2: 48 8B 84 24 90 01  mov         rax,qword ptr [rsp+0000000000000190h]
                    00 00
  0000000140D360BA: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  0000000140D360C2: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140D360CA: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D360CF: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  0000000140D360D6: 83 E0 20           and         eax,20h
  0000000140D360D9: 85 C0              test        eax,eax
  0000000140D360DB: 74 16              je          0000000140D360F3
  0000000140D360DD: 83 A4 24 58 01 00  and         dword ptr [rsp+0000000000000158h],0FFFFFFDFh
                    00 DF
  0000000140D360E5: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000140D360ED: FF 15 ED F8 0C 01  call        qword ptr [0000000141E059E0h]
  0000000140D360F3: 48 8D 15 1E 07 CD  lea         rdx,[0000000142A06818h]
                    01
  0000000140D360FA: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  0000000140D36102: FF 15 08 E0 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D36108: 48 8B D0           mov         rdx,rax
  0000000140D3610B: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D36110: E8 FB 0A 0A 00     call        0000000140DD6C10
  0000000140D36115: 4C 8B 44 24 58     mov         r8,qword ptr [rsp+58h]
  0000000140D3611A: 33 D2              xor         edx,edx
  0000000140D3611C: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D36121: E8 AA 5A 07 00     call        0000000140DABBD0
  0000000140D36126: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  0000000140D3612B: BA 01 00 00 00     mov         edx,1
  0000000140D36130: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D36135: E8 96 5A 07 00     call        0000000140DABBD0
  0000000140D3613A: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D36142: 48 83 C1 30        add         rcx,30h
  0000000140D36146: 48 8B 94 24 B8 01  mov         rdx,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140D3614E: FF 15 84 F7 0C 01  call        qword ptr [0000000141E058D8h]
  0000000140D36154: 48 81 C4 A8 01 00  add         rsp,1A8h
                    00
  0000000140D3615B: C3                 ret

  Summary

     1E02000 .text
