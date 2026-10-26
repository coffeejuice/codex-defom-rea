
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014001E910: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000014001E915: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014001E919: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014001E91E: 56                 push        rsi
  000000014001E91F: 57                 push        rdi
  000000014001E920: 48 81 EC 38 08 00  sub         rsp,838h
                    00
  000000014001E927: 48 C7 84 24 30 05  mov         qword ptr [rsp+0000000000000530h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014001E933: 8B 84 24 58 08 00  mov         eax,dword ptr [rsp+0000000000000858h]
                    00
  000000014001E93A: 89 84 24 38 05 00  mov         dword ptr [rsp+0000000000000538h],eax
                    00
  000000014001E941: 8B 84 24 38 05 00  mov         eax,dword ptr [rsp+0000000000000538h]
                    00
  000000014001E948: 83 C0 01           add         eax,1
  000000014001E94B: 89 84 24 38 05 00  mov         dword ptr [rsp+0000000000000538h],eax
                    00
  000000014001E952: 81 BC 24 38 05 00  cmp         dword ptr [rsp+0000000000000538h],162h
                    00 62 01 00 00
  000000014001E95D: 0F 87 5A 5C 00 00  ja          00000001400245BD
  000000014001E963: 48 63 84 24 38 05  movsxd      rax,dword ptr [rsp+0000000000000538h]
                    00 00
  000000014001E96B: 48 8D 0D 8E 16 FE  lea         rcx,[0000000140000000h]
                    FF
  000000014001E972: 8B 84 81 CC 45 02  mov         eax,dword ptr [rcx+rax*4+00000000000245CCh]
                    00
  000000014001E979: 48 03 C1           add         rax,rcx
  000000014001E97C: FF E0              jmp         rax
  000000014001E97E: B8 01 00 00 00     mov         eax,1
  000000014001E983: E9 3A 5C 00 00     jmp         00000001400245C2
  000000014001E988: E9 35 5C 00 00     jmp         00000001400245C2
  000000014001E98D: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001E995: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001E998: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001E9A0: FF 10              call        qword ptr [rax]
  000000014001E9A2: 33 C0              xor         eax,eax
  000000014001E9A4: E9 19 5C 00 00     jmp         00000001400245C2
  000000014001E9A9: E9 14 5C 00 00     jmp         00000001400245C2
  000000014001E9AE: 33 C0              xor         eax,eax
  000000014001E9B0: 85 C0              test        eax,eax
  000000014001E9B2: 74 2D              je          000000014001E9E1
  000000014001E9B4: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001E9BC: 48 83 C0 08        add         rax,8
  000000014001E9C0: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001E9C8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001E9D0: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001E9D4: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001E9D7: 48 89 84 24 40 05  mov         qword ptr [rsp+0000000000000540h],rax
                    00 00
  000000014001E9DF: EB 28              jmp         000000014001EA09
  000000014001E9E1: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001E9E9: 48 83 C0 08        add         rax,8
  000000014001E9ED: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001E9F5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001E9FD: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EA01: 48 89 84 24 40 05  mov         qword ptr [rsp+0000000000000540h],rax
                    00 00
  000000014001EA09: 48 8B 84 24 40 05  mov         rax,qword ptr [rsp+0000000000000540h]
                    00 00
  000000014001EA11: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014001EA16: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001EA1E: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001EA21: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  000000014001EA26: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001EA2E: FF 50 08           call        qword ptr [rax+8]
  000000014001EA31: 33 C0              xor         eax,eax
  000000014001EA33: E9 8A 5B 00 00     jmp         00000001400245C2
  000000014001EA38: E9 85 5B 00 00     jmp         00000001400245C2
  000000014001EA3D: 33 C0              xor         eax,eax
  000000014001EA3F: 85 C0              test        eax,eax
  000000014001EA41: 74 2D              je          000000014001EA70
  000000014001EA43: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EA4B: 48 83 C0 08        add         rax,8
  000000014001EA4F: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EA57: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EA5F: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EA63: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001EA66: 48 89 84 24 48 05  mov         qword ptr [rsp+0000000000000548h],rax
                    00 00
  000000014001EA6E: EB 28              jmp         000000014001EA98
  000000014001EA70: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EA78: 48 83 C0 08        add         rax,8
  000000014001EA7C: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EA84: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EA8C: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EA90: 48 89 84 24 48 05  mov         qword ptr [rsp+0000000000000548h],rax
                    00 00
  000000014001EA98: 48 8B 84 24 48 05  mov         rax,qword ptr [rsp+0000000000000548h]
                    00 00
  000000014001EAA0: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014001EAA5: 33 C0              xor         eax,eax
  000000014001EAA7: 85 C0              test        eax,eax
  000000014001EAA9: 74 2D              je          000000014001EAD8
  000000014001EAAB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EAB3: 48 83 C0 08        add         rax,8
  000000014001EAB7: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EABF: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EAC7: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EACB: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001EACE: 48 89 84 24 50 05  mov         qword ptr [rsp+0000000000000550h],rax
                    00 00
  000000014001EAD6: EB 28              jmp         000000014001EB00
  000000014001EAD8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EAE0: 48 83 C0 08        add         rax,8
  000000014001EAE4: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EAEC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EAF4: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EAF8: 48 89 84 24 50 05  mov         qword ptr [rsp+0000000000000550h],rax
                    00 00
  000000014001EB00: 48 8B 84 24 50 05  mov         rax,qword ptr [rsp+0000000000000550h]
                    00 00
  000000014001EB08: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014001EB0D: 33 C0              xor         eax,eax
  000000014001EB0F: 85 C0              test        eax,eax
  000000014001EB11: 74 2B              je          000000014001EB3E
  000000014001EB13: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EB1B: 48 83 C0 08        add         rax,8
  000000014001EB1F: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EB27: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EB2F: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EB33: 8B 00              mov         eax,dword ptr [rax]
  000000014001EB35: 89 84 24 58 05 00  mov         dword ptr [rsp+0000000000000558h],eax
                    00
  000000014001EB3C: EB 26              jmp         000000014001EB64
  000000014001EB3E: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EB46: 48 83 C0 08        add         rax,8
  000000014001EB4A: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EB52: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EB5A: 8B 40 F8           mov         eax,dword ptr [rax-8]
  000000014001EB5D: 89 84 24 58 05 00  mov         dword ptr [rsp+0000000000000558h],eax
                    00
  000000014001EB64: 8B 84 24 58 05 00  mov         eax,dword ptr [rsp+0000000000000558h]
                    00
  000000014001EB6B: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000014001EB6F: 44 8B 4C 24 40     mov         r9d,dword ptr [rsp+40h]
  000000014001EB74: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  000000014001EB79: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  000000014001EB7E: 48 8D 8C 24 D8 02  lea         rcx,[rsp+00000000000002D8h]
                    00 00
  000000014001EB86: E8 75 60 00 00     call        0000000140024C00
  000000014001EB8B: 48 8D 8C 24 D8 02  lea         rcx,[rsp+00000000000002D8h]
                    00 00
  000000014001EB93: FF 15 47 6E DE 01  call        qword ptr [0000000141E059E0h]
  000000014001EB99: 33 C0              xor         eax,eax
  000000014001EB9B: E9 22 5A 00 00     jmp         00000001400245C2
  000000014001EBA0: E9 1D 5A 00 00     jmp         00000001400245C2
  000000014001EBA5: 33 C0              xor         eax,eax
  000000014001EBA7: 85 C0              test        eax,eax
  000000014001EBA9: 74 2D              je          000000014001EBD8
  000000014001EBAB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EBB3: 48 83 C0 08        add         rax,8
  000000014001EBB7: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EBBF: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EBC7: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EBCB: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001EBCE: 48 89 84 24 60 05  mov         qword ptr [rsp+0000000000000560h],rax
                    00 00
  000000014001EBD6: EB 28              jmp         000000014001EC00
  000000014001EBD8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EBE0: 48 83 C0 08        add         rax,8
  000000014001EBE4: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EBEC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EBF4: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EBF8: 48 89 84 24 60 05  mov         qword ptr [rsp+0000000000000560h],rax
                    00 00
  000000014001EC00: 48 8B 84 24 60 05  mov         rax,qword ptr [rsp+0000000000000560h]
                    00 00
  000000014001EC08: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014001EC0D: 33 C0              xor         eax,eax
  000000014001EC0F: 85 C0              test        eax,eax
  000000014001EC11: 74 2D              je          000000014001EC40
  000000014001EC13: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EC1B: 48 83 C0 08        add         rax,8
  000000014001EC1F: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EC27: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EC2F: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EC33: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001EC36: 48 89 84 24 68 05  mov         qword ptr [rsp+0000000000000568h],rax
                    00 00
  000000014001EC3E: EB 28              jmp         000000014001EC68
  000000014001EC40: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EC48: 48 83 C0 08        add         rax,8
  000000014001EC4C: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EC54: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EC5C: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EC60: 48 89 84 24 68 05  mov         qword ptr [rsp+0000000000000568h],rax
                    00 00
  000000014001EC68: 48 8B 84 24 68 05  mov         rax,qword ptr [rsp+0000000000000568h]
                    00 00
  000000014001EC70: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014001EC75: 4C 8B 44 24 58     mov         r8,qword ptr [rsp+58h]
  000000014001EC7A: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  000000014001EC7F: 48 8D 8C 24 E0 02  lea         rcx,[rsp+00000000000002E0h]
                    00 00
  000000014001EC87: E8 D4 5E 00 00     call        0000000140024B60
  000000014001EC8C: 48 8D 8C 24 E0 02  lea         rcx,[rsp+00000000000002E0h]
                    00 00
  000000014001EC94: FF 15 46 6D DE 01  call        qword ptr [0000000141E059E0h]
  000000014001EC9A: 33 C0              xor         eax,eax
  000000014001EC9C: E9 21 59 00 00     jmp         00000001400245C2
  000000014001ECA1: E9 1C 59 00 00     jmp         00000001400245C2
  000000014001ECA6: 33 C0              xor         eax,eax
  000000014001ECA8: 85 C0              test        eax,eax
  000000014001ECAA: 74 2D              je          000000014001ECD9
  000000014001ECAC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001ECB4: 48 83 C0 08        add         rax,8
  000000014001ECB8: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001ECC0: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001ECC8: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001ECCC: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001ECCF: 48 89 84 24 70 05  mov         qword ptr [rsp+0000000000000570h],rax
                    00 00
  000000014001ECD7: EB 28              jmp         000000014001ED01
  000000014001ECD9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001ECE1: 48 83 C0 08        add         rax,8
  000000014001ECE5: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001ECED: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001ECF5: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001ECF9: 48 89 84 24 70 05  mov         qword ptr [rsp+0000000000000570h],rax
                    00 00
  000000014001ED01: 48 8B 84 24 70 05  mov         rax,qword ptr [rsp+0000000000000570h]
                    00 00
  000000014001ED09: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014001ED0E: 33 C0              xor         eax,eax
  000000014001ED10: 85 C0              test        eax,eax
  000000014001ED12: 74 2D              je          000000014001ED41
  000000014001ED14: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001ED1C: 48 83 C0 08        add         rax,8
  000000014001ED20: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001ED28: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001ED30: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001ED34: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001ED37: 48 89 84 24 78 05  mov         qword ptr [rsp+0000000000000578h],rax
                    00 00
  000000014001ED3F: EB 28              jmp         000000014001ED69
  000000014001ED41: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001ED49: 48 83 C0 08        add         rax,8
  000000014001ED4D: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001ED55: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001ED5D: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001ED61: 48 89 84 24 78 05  mov         qword ptr [rsp+0000000000000578h],rax
                    00 00
  000000014001ED69: 48 8B 84 24 78 05  mov         rax,qword ptr [rsp+0000000000000578h]
                    00 00
  000000014001ED71: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000014001ED76: 33 C0              xor         eax,eax
  000000014001ED78: 85 C0              test        eax,eax
  000000014001ED7A: 74 2B              je          000000014001EDA7
  000000014001ED7C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001ED84: 48 83 C0 08        add         rax,8
  000000014001ED88: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001ED90: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001ED98: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001ED9C: 8B 00              mov         eax,dword ptr [rax]
  000000014001ED9E: 89 84 24 80 05 00  mov         dword ptr [rsp+0000000000000580h],eax
                    00
  000000014001EDA5: EB 26              jmp         000000014001EDCD
  000000014001EDA7: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EDAF: 48 83 C0 08        add         rax,8
  000000014001EDB3: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EDBB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EDC3: 8B 40 F8           mov         eax,dword ptr [rax-8]
  000000014001EDC6: 89 84 24 80 05 00  mov         dword ptr [rsp+0000000000000580h],eax
                    00
  000000014001EDCD: 8B 84 24 80 05 00  mov         eax,dword ptr [rsp+0000000000000580h]
                    00
  000000014001EDD4: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  000000014001EDD8: 44 8B 4C 24 68     mov         r9d,dword ptr [rsp+68h]
  000000014001EDDD: 4C 8B 44 24 70     mov         r8,qword ptr [rsp+70h]
  000000014001EDE2: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  000000014001EDE7: 48 8D 8C 24 E8 02  lea         rcx,[rsp+00000000000002E8h]
                    00 00
  000000014001EDEF: E8 6C 5E 00 00     call        0000000140024C60
  000000014001EDF4: 48 8D 8C 24 E8 02  lea         rcx,[rsp+00000000000002E8h]
                    00 00
  000000014001EDFC: FF 15 DE 6B DE 01  call        qword ptr [0000000141E059E0h]
  000000014001EE02: 33 C0              xor         eax,eax
  000000014001EE04: E9 B9 57 00 00     jmp         00000001400245C2
  000000014001EE09: E9 B4 57 00 00     jmp         00000001400245C2
  000000014001EE0E: 33 C0              xor         eax,eax
  000000014001EE10: 85 C0              test        eax,eax
  000000014001EE12: 74 2D              je          000000014001EE41
  000000014001EE14: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EE1C: 48 83 C0 08        add         rax,8
  000000014001EE20: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EE28: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EE30: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EE34: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001EE37: 48 89 84 24 88 05  mov         qword ptr [rsp+0000000000000588h],rax
                    00 00
  000000014001EE3F: EB 28              jmp         000000014001EE69
  000000014001EE41: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EE49: 48 83 C0 08        add         rax,8
  000000014001EE4D: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EE55: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EE5D: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EE61: 48 89 84 24 88 05  mov         qword ptr [rsp+0000000000000588h],rax
                    00 00
  000000014001EE69: 48 8B 84 24 88 05  mov         rax,qword ptr [rsp+0000000000000588h]
                    00 00
  000000014001EE71: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000014001EE76: 33 C0              xor         eax,eax
  000000014001EE78: 85 C0              test        eax,eax
  000000014001EE7A: 74 2D              je          000000014001EEA9
  000000014001EE7C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EE84: 48 83 C0 08        add         rax,8
  000000014001EE88: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EE90: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EE98: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EE9C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001EE9F: 48 89 84 24 90 05  mov         qword ptr [rsp+0000000000000590h],rax
                    00 00
  000000014001EEA7: EB 28              jmp         000000014001EED1
  000000014001EEA9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EEB1: 48 83 C0 08        add         rax,8
  000000014001EEB5: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EEBD: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EEC5: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EEC9: 48 89 84 24 90 05  mov         qword ptr [rsp+0000000000000590h],rax
                    00 00
  000000014001EED1: 48 8B 84 24 90 05  mov         rax,qword ptr [rsp+0000000000000590h]
                    00 00
  000000014001EED9: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014001EEE1: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014001EEE9: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  000000014001EEEE: 48 8D 8C 24 F0 02  lea         rcx,[rsp+00000000000002F0h]
                    00 00
  000000014001EEF6: E8 B5 5C 00 00     call        0000000140024BB0
  000000014001EEFB: 48 8D 8C 24 F0 02  lea         rcx,[rsp+00000000000002F0h]
                    00 00
  000000014001EF03: FF 15 D7 6A DE 01  call        qword ptr [0000000141E059E0h]
  000000014001EF09: 33 C0              xor         eax,eax
  000000014001EF0B: E9 B2 56 00 00     jmp         00000001400245C2
  000000014001EF10: E9 AD 56 00 00     jmp         00000001400245C2
  000000014001EF15: 33 C0              xor         eax,eax
  000000014001EF17: 85 C0              test        eax,eax
  000000014001EF19: 74 2B              je          000000014001EF46
  000000014001EF1B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EF23: 48 83 C0 08        add         rax,8
  000000014001EF27: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EF2F: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EF37: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EF3B: 8B 00              mov         eax,dword ptr [rax]
  000000014001EF3D: 89 84 24 98 05 00  mov         dword ptr [rsp+0000000000000598h],eax
                    00
  000000014001EF44: EB 26              jmp         000000014001EF6C
  000000014001EF46: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EF4E: 48 83 C0 08        add         rax,8
  000000014001EF52: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EF5A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EF62: 8B 40 F8           mov         eax,dword ptr [rax-8]
  000000014001EF65: 89 84 24 98 05 00  mov         dword ptr [rsp+0000000000000598h],eax
                    00
  000000014001EF6C: 8B 84 24 98 05 00  mov         eax,dword ptr [rsp+0000000000000598h]
                    00
  000000014001EF73: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  000000014001EF7A: 33 C0              xor         eax,eax
  000000014001EF7C: 85 C0              test        eax,eax
  000000014001EF7E: 74 2B              je          000000014001EFAB
  000000014001EF80: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EF88: 48 83 C0 08        add         rax,8
  000000014001EF8C: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EF94: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EF9C: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001EFA0: 8B 00              mov         eax,dword ptr [rax]
  000000014001EFA2: 89 84 24 9C 05 00  mov         dword ptr [rsp+000000000000059Ch],eax
                    00
  000000014001EFA9: EB 26              jmp         000000014001EFD1
  000000014001EFAB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EFB3: 48 83 C0 08        add         rax,8
  000000014001EFB7: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EFBF: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EFC7: 8B 40 F8           mov         eax,dword ptr [rax-8]
  000000014001EFCA: 89 84 24 9C 05 00  mov         dword ptr [rsp+000000000000059Ch],eax
                    00
  000000014001EFD1: 8B 84 24 9C 05 00  mov         eax,dword ptr [rsp+000000000000059Ch]
                    00
  000000014001EFD8: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  000000014001EFDF: 33 C0              xor         eax,eax
  000000014001EFE1: 85 C0              test        eax,eax
  000000014001EFE3: 74 2D              je          000000014001F012
  000000014001EFE5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001EFED: 48 83 C0 08        add         rax,8
  000000014001EFF1: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001EFF9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F001: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F005: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F008: 48 89 84 24 A0 05  mov         qword ptr [rsp+00000000000005A0h],rax
                    00 00
  000000014001F010: EB 28              jmp         000000014001F03A
  000000014001F012: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F01A: 48 83 C0 08        add         rax,8
  000000014001F01E: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F026: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F02E: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F032: 48 89 84 24 A0 05  mov         qword ptr [rsp+00000000000005A0h],rax
                    00 00
  000000014001F03A: 48 8B 84 24 A0 05  mov         rax,qword ptr [rsp+00000000000005A0h]
                    00 00
  000000014001F042: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  000000014001F04A: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F052: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F055: 4C 8B 8C 24 90 00  mov         r9,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014001F05D: 44 8B 84 24 98 00  mov         r8d,dword ptr [rsp+0000000000000098h]
                    00 00
  000000014001F065: 8B 94 24 88 00 00  mov         edx,dword ptr [rsp+0000000000000088h]
                    00
  000000014001F06C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F074: FF 50 10           call        qword ptr [rax+10h]
  000000014001F077: E9 46 55 00 00     jmp         00000001400245C2
  000000014001F07C: E9 41 55 00 00     jmp         00000001400245C2
  000000014001F081: 33 C0              xor         eax,eax
  000000014001F083: 85 C0              test        eax,eax
  000000014001F085: 74 2D              je          000000014001F0B4
  000000014001F087: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F08F: 48 83 C0 08        add         rax,8
  000000014001F093: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F09B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F0A3: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F0A7: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F0AA: 48 89 84 24 A8 05  mov         qword ptr [rsp+00000000000005A8h],rax
                    00 00
  000000014001F0B2: EB 28              jmp         000000014001F0DC
  000000014001F0B4: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F0BC: 48 83 C0 08        add         rax,8
  000000014001F0C0: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F0C8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F0D0: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F0D4: 48 89 84 24 A8 05  mov         qword ptr [rsp+00000000000005A8h],rax
                    00 00
  000000014001F0DC: 48 8B 84 24 A8 05  mov         rax,qword ptr [rsp+00000000000005A8h]
                    00 00
  000000014001F0E4: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  000000014001F0EC: 33 C0              xor         eax,eax
  000000014001F0EE: 85 C0              test        eax,eax
  000000014001F0F0: 74 2B              je          000000014001F11D
  000000014001F0F2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F0FA: 48 83 C0 08        add         rax,8
  000000014001F0FE: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F106: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F10E: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F112: 8B 00              mov         eax,dword ptr [rax]
  000000014001F114: 89 84 24 B0 05 00  mov         dword ptr [rsp+00000000000005B0h],eax
                    00
  000000014001F11B: EB 26              jmp         000000014001F143
  000000014001F11D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F125: 48 83 C0 08        add         rax,8
  000000014001F129: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F131: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F139: 8B 40 F8           mov         eax,dword ptr [rax-8]
  000000014001F13C: 89 84 24 B0 05 00  mov         dword ptr [rsp+00000000000005B0h],eax
                    00
  000000014001F143: 8B 84 24 B0 05 00  mov         eax,dword ptr [rsp+00000000000005B0h]
                    00
  000000014001F14A: 89 84 24 B8 00 00  mov         dword ptr [rsp+00000000000000B8h],eax
                    00
  000000014001F151: 33 C0              xor         eax,eax
  000000014001F153: 85 C0              test        eax,eax
  000000014001F155: 74 2B              je          000000014001F182
  000000014001F157: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F15F: 48 83 C0 08        add         rax,8
  000000014001F163: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F16B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F173: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F177: 8B 00              mov         eax,dword ptr [rax]
  000000014001F179: 89 84 24 B4 05 00  mov         dword ptr [rsp+00000000000005B4h],eax
                    00
  000000014001F180: EB 26              jmp         000000014001F1A8
  000000014001F182: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F18A: 48 83 C0 08        add         rax,8
  000000014001F18E: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F196: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F19E: 8B 40 F8           mov         eax,dword ptr [rax-8]
  000000014001F1A1: 89 84 24 B4 05 00  mov         dword ptr [rsp+00000000000005B4h],eax
                    00
  000000014001F1A8: 8B 84 24 B4 05 00  mov         eax,dword ptr [rsp+00000000000005B4h]
                    00
  000000014001F1AF: 89 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],eax
                    00
  000000014001F1B6: 33 C0              xor         eax,eax
  000000014001F1B8: 85 C0              test        eax,eax
  000000014001F1BA: 74 2D              je          000000014001F1E9
  000000014001F1BC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F1C4: 48 83 C0 08        add         rax,8
  000000014001F1C8: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F1D0: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F1D8: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F1DC: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F1DF: 48 89 84 24 B8 05  mov         qword ptr [rsp+00000000000005B8h],rax
                    00 00
  000000014001F1E7: EB 28              jmp         000000014001F211
  000000014001F1E9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F1F1: 48 83 C0 08        add         rax,8
  000000014001F1F5: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F1FD: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F205: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F209: 48 89 84 24 B8 05  mov         qword ptr [rsp+00000000000005B8h],rax
                    00 00
  000000014001F211: 48 8B 84 24 B8 05  mov         rax,qword ptr [rsp+00000000000005B8h]
                    00 00
  000000014001F219: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  000000014001F221: 4C 8B 8C 24 B0 00  mov         r9,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014001F229: 44 8B 84 24 A8 00  mov         r8d,dword ptr [rsp+00000000000000A8h]
                    00 00
  000000014001F231: 8B 94 24 B8 00 00  mov         edx,dword ptr [rsp+00000000000000B8h]
                    00
  000000014001F238: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014001F240: E8 2B 67 CA 00     call        0000000140CC5970
  000000014001F245: 33 C0              xor         eax,eax
  000000014001F247: E9 76 53 00 00     jmp         00000001400245C2
  000000014001F24C: E9 71 53 00 00     jmp         00000001400245C2
  000000014001F251: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F259: E8 82 A7 C7 00     call        0000000140C999E0
  000000014001F25E: 33 C0              xor         eax,eax
  000000014001F260: E9 5D 53 00 00     jmp         00000001400245C2
  000000014001F265: E9 58 53 00 00     jmp         00000001400245C2
  000000014001F26A: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F272: E8 C9 A7 C7 00     call        0000000140C99A40
  000000014001F277: 33 C0              xor         eax,eax
  000000014001F279: E9 44 53 00 00     jmp         00000001400245C2
  000000014001F27E: E9 3F 53 00 00     jmp         00000001400245C2
  000000014001F283: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F28B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F28E: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F296: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  000000014001F29C: 33 C0              xor         eax,eax
  000000014001F29E: E9 1F 53 00 00     jmp         00000001400245C2
  000000014001F2A3: E9 1A 53 00 00     jmp         00000001400245C2
  000000014001F2A8: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F2B0: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F2B3: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F2BB: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  000000014001F2C1: 33 C0              xor         eax,eax
  000000014001F2C3: E9 FA 52 00 00     jmp         00000001400245C2
  000000014001F2C8: E9 F5 52 00 00     jmp         00000001400245C2
  000000014001F2CD: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F2D5: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F2D8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F2E0: FF 90 A8 01 00 00  call        qword ptr [rax+00000000000001A8h]
  000000014001F2E6: 33 C0              xor         eax,eax
  000000014001F2E8: E9 D5 52 00 00     jmp         00000001400245C2
  000000014001F2ED: E9 D0 52 00 00     jmp         00000001400245C2
  000000014001F2F2: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F2FA: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F2FD: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F305: FF 90 B0 01 00 00  call        qword ptr [rax+00000000000001B0h]
  000000014001F30B: 33 C0              xor         eax,eax
  000000014001F30D: E9 B0 52 00 00     jmp         00000001400245C2
  000000014001F312: E9 AB 52 00 00     jmp         00000001400245C2
  000000014001F317: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F31F: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F322: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F32A: FF 90 B8 01 00 00  call        qword ptr [rax+00000000000001B8h]
  000000014001F330: 33 C0              xor         eax,eax
  000000014001F332: E9 8B 52 00 00     jmp         00000001400245C2
  000000014001F337: E9 86 52 00 00     jmp         00000001400245C2
  000000014001F33C: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F344: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F347: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F34F: FF 90 C0 01 00 00  call        qword ptr [rax+00000000000001C0h]
  000000014001F355: 33 C0              xor         eax,eax
  000000014001F357: E9 66 52 00 00     jmp         00000001400245C2
  000000014001F35C: E9 61 52 00 00     jmp         00000001400245C2
  000000014001F361: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F369: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F36C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F374: FF 90 C8 01 00 00  call        qword ptr [rax+00000000000001C8h]
  000000014001F37A: 33 C0              xor         eax,eax
  000000014001F37C: E9 41 52 00 00     jmp         00000001400245C2
  000000014001F381: E9 3C 52 00 00     jmp         00000001400245C2
  000000014001F386: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F38E: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F391: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F399: FF 90 D0 01 00 00  call        qword ptr [rax+00000000000001D0h]
  000000014001F39F: 33 C0              xor         eax,eax
  000000014001F3A1: E9 1C 52 00 00     jmp         00000001400245C2
  000000014001F3A6: E9 17 52 00 00     jmp         00000001400245C2
  000000014001F3AB: 33 C0              xor         eax,eax
  000000014001F3AD: 85 C0              test        eax,eax
  000000014001F3AF: 74 2D              je          000000014001F3DE
  000000014001F3B1: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F3B9: 48 83 C0 08        add         rax,8
  000000014001F3BD: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F3C5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F3CD: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F3D1: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F3D4: 48 89 84 24 C0 05  mov         qword ptr [rsp+00000000000005C0h],rax
                    00 00
  000000014001F3DC: EB 28              jmp         000000014001F406
  000000014001F3DE: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F3E6: 48 83 C0 08        add         rax,8
  000000014001F3EA: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F3F2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F3FA: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F3FE: 48 89 84 24 C0 05  mov         qword ptr [rsp+00000000000005C0h],rax
                    00 00
  000000014001F406: 48 8B 84 24 C0 05  mov         rax,qword ptr [rsp+00000000000005C0h]
                    00 00
  000000014001F40E: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  000000014001F416: 33 C0              xor         eax,eax
  000000014001F418: 85 C0              test        eax,eax
  000000014001F41A: 74 2D              je          000000014001F449
  000000014001F41C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F424: 48 83 C0 08        add         rax,8
  000000014001F428: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F430: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F438: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F43C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F43F: 48 89 84 24 C8 05  mov         qword ptr [rsp+00000000000005C8h],rax
                    00 00
  000000014001F447: EB 28              jmp         000000014001F471
  000000014001F449: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F451: 48 83 C0 08        add         rax,8
  000000014001F455: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F45D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F465: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F469: 48 89 84 24 C8 05  mov         qword ptr [rsp+00000000000005C8h],rax
                    00 00
  000000014001F471: 48 8B 84 24 C8 05  mov         rax,qword ptr [rsp+00000000000005C8h]
                    00 00
  000000014001F479: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  000000014001F481: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F489: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F48C: 4C 8B 84 24 C8 00  mov         r8,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014001F494: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014001F49C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F4A4: FF 50 28           call        qword ptr [rax+28h]
  000000014001F4A7: 0F B6 C0           movzx       eax,al
  000000014001F4AA: E9 13 51 00 00     jmp         00000001400245C2
  000000014001F4AF: E9 0E 51 00 00     jmp         00000001400245C2
  000000014001F4B4: 33 C0              xor         eax,eax
  000000014001F4B6: 85 C0              test        eax,eax
  000000014001F4B8: 74 2C              je          000000014001F4E6
  000000014001F4BA: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F4C2: 48 83 C0 08        add         rax,8
  000000014001F4C6: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F4CE: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F4D6: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F4DA: 0F B6 00           movzx       eax,byte ptr [rax]
  000000014001F4DD: 88 84 24 D0 05 00  mov         byte ptr [rsp+00000000000005D0h],al
                    00
  000000014001F4E4: EB 27              jmp         000000014001F50D
  000000014001F4E6: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F4EE: 48 83 C0 08        add         rax,8
  000000014001F4F2: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F4FA: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F502: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  000000014001F506: 88 84 24 D0 05 00  mov         byte ptr [rsp+00000000000005D0h],al
                    00
  000000014001F50D: 0F B6 84 24 D0 05  movzx       eax,byte ptr [rsp+00000000000005D0h]
                    00 00
  000000014001F515: 88 84 24 D0 00 00  mov         byte ptr [rsp+00000000000000D0h],al
                    00
  000000014001F51C: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F524: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F527: 0F B6 94 24 D0 00  movzx       edx,byte ptr [rsp+00000000000000D0h]
                    00 00
  000000014001F52F: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F537: FF 90 D8 01 00 00  call        qword ptr [rax+00000000000001D8h]
  000000014001F53D: 33 C0              xor         eax,eax
  000000014001F53F: E9 7E 50 00 00     jmp         00000001400245C2
  000000014001F544: E9 79 50 00 00     jmp         00000001400245C2
  000000014001F549: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F551: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F554: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F55C: FF 90 E0 01 00 00  call        qword ptr [rax+00000000000001E0h]
  000000014001F562: 33 C0              xor         eax,eax
  000000014001F564: E9 59 50 00 00     jmp         00000001400245C2
  000000014001F569: E9 54 50 00 00     jmp         00000001400245C2
  000000014001F56E: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F576: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F579: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F581: FF 90 E8 01 00 00  call        qword ptr [rax+00000000000001E8h]
  000000014001F587: 33 C0              xor         eax,eax
  000000014001F589: E9 34 50 00 00     jmp         00000001400245C2
  000000014001F58E: E9 2F 50 00 00     jmp         00000001400245C2
  000000014001F593: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F59B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F59E: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F5A6: FF 90 F0 01 00 00  call        qword ptr [rax+00000000000001F0h]
  000000014001F5AC: 33 C0              xor         eax,eax
  000000014001F5AE: E9 0F 50 00 00     jmp         00000001400245C2
  000000014001F5B3: E9 0A 50 00 00     jmp         00000001400245C2
  000000014001F5B8: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F5C0: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F5C3: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F5CB: FF 90 F8 01 00 00  call        qword ptr [rax+00000000000001F8h]
  000000014001F5D1: 33 C0              xor         eax,eax
  000000014001F5D3: E9 EA 4F 00 00     jmp         00000001400245C2
  000000014001F5D8: E9 E5 4F 00 00     jmp         00000001400245C2
  000000014001F5DD: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F5E5: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F5E8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F5F0: FF 90 00 02 00 00  call        qword ptr [rax+0000000000000200h]
  000000014001F5F6: 33 C0              xor         eax,eax
  000000014001F5F8: E9 C5 4F 00 00     jmp         00000001400245C2
  000000014001F5FD: E9 C0 4F 00 00     jmp         00000001400245C2
  000000014001F602: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F60A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F60D: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F615: FF 90 08 02 00 00  call        qword ptr [rax+0000000000000208h]
  000000014001F61B: 33 C0              xor         eax,eax
  000000014001F61D: E9 A0 4F 00 00     jmp         00000001400245C2
  000000014001F622: E9 9B 4F 00 00     jmp         00000001400245C2
  000000014001F627: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F62F: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F632: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F63A: FF 90 10 02 00 00  call        qword ptr [rax+0000000000000210h]
  000000014001F640: 33 C0              xor         eax,eax
  000000014001F642: E9 7B 4F 00 00     jmp         00000001400245C2
  000000014001F647: E9 76 4F 00 00     jmp         00000001400245C2
  000000014001F64C: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F654: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F657: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F65F: FF 90 18 02 00 00  call        qword ptr [rax+0000000000000218h]
  000000014001F665: 33 C0              xor         eax,eax
  000000014001F667: E9 56 4F 00 00     jmp         00000001400245C2
  000000014001F66C: E9 51 4F 00 00     jmp         00000001400245C2
  000000014001F671: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F679: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F67C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F684: FF 90 20 02 00 00  call        qword ptr [rax+0000000000000220h]
  000000014001F68A: 33 C0              xor         eax,eax
  000000014001F68C: E9 31 4F 00 00     jmp         00000001400245C2
  000000014001F691: E9 2C 4F 00 00     jmp         00000001400245C2
  000000014001F696: 33 C0              xor         eax,eax
  000000014001F698: 85 C0              test        eax,eax
  000000014001F69A: 74 2D              je          000000014001F6C9
  000000014001F69C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F6A4: 48 83 C0 08        add         rax,8
  000000014001F6A8: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F6B0: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F6B8: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F6BC: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F6BF: 48 89 84 24 D8 05  mov         qword ptr [rsp+00000000000005D8h],rax
                    00 00
  000000014001F6C7: EB 28              jmp         000000014001F6F1
  000000014001F6C9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F6D1: 48 83 C0 08        add         rax,8
  000000014001F6D5: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F6DD: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F6E5: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F6E9: 48 89 84 24 D8 05  mov         qword ptr [rsp+00000000000005D8h],rax
                    00 00
  000000014001F6F1: 48 8B 84 24 D8 05  mov         rax,qword ptr [rsp+00000000000005D8h]
                    00 00
  000000014001F6F9: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  000000014001F701: 33 C0              xor         eax,eax
  000000014001F703: 85 C0              test        eax,eax
  000000014001F705: 74 2A              je          000000014001F731
  000000014001F707: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F70F: 48 83 C0 08        add         rax,8
  000000014001F713: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F71B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F723: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F727: 48 89 84 24 E0 05  mov         qword ptr [rsp+00000000000005E0h],rax
                    00 00
  000000014001F72F: EB 28              jmp         000000014001F759
  000000014001F731: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F739: 48 83 C0 08        add         rax,8
  000000014001F73D: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F745: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F74D: 48 83 E8 08        sub         rax,8
  000000014001F751: 48 89 84 24 E0 05  mov         qword ptr [rsp+00000000000005E0h],rax
                    00 00
  000000014001F759: 48 8B 84 24 E0 05  mov         rax,qword ptr [rsp+00000000000005E0h]
                    00 00
  000000014001F761: 48 89 84 24 F8 02  mov         qword ptr [rsp+00000000000002F8h],rax
                    00 00
  000000014001F769: 48 8B 94 24 F8 02  mov         rdx,qword ptr [rsp+00000000000002F8h]
                    00 00
  000000014001F771: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  000000014001F779: E8 72 52 FE FF     call        00000001400049F0
  000000014001F77E: 90                 nop
  000000014001F77F: 48 8D 84 24 00 03  lea         rax,[rsp+0000000000000300h]
                    00 00
  000000014001F787: 48 89 84 24 08 03  mov         qword ptr [rsp+0000000000000308h],rax
                    00 00
  000000014001F78F: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  000000014001F797: 48 8B 8C 24 08 03  mov         rcx,qword ptr [rsp+0000000000000308h]
                    00 00
  000000014001F79F: E8 4C 52 FE FF     call        00000001400049F0
  000000014001F7A4: 48 89 84 24 E8 05  mov         qword ptr [rsp+00000000000005E8h],rax
                    00 00
  000000014001F7AC: 4C 8B 84 24 E8 05  mov         r8,qword ptr [rsp+00000000000005E8h]
                    00 00
  000000014001F7B4: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  000000014001F7BC: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F7C4: E8 C7 B9 C8 00     call        0000000140CAB190
  000000014001F7C9: C7 84 24 10 03 00  mov         dword ptr [rsp+0000000000000310h],0
                    00 00 00 00 00
  000000014001F7D4: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  000000014001F7DC: E8 EF 50 FE FF     call        00000001400048D0
  000000014001F7E1: 8B 84 24 10 03 00  mov         eax,dword ptr [rsp+0000000000000310h]
                    00
  000000014001F7E8: E9 D5 4D 00 00     jmp         00000001400245C2
  000000014001F7ED: E9 D0 4D 00 00     jmp         00000001400245C2
  000000014001F7F2: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F7FA: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F7FD: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F805: FF 90 28 02 00 00  call        qword ptr [rax+0000000000000228h]
  000000014001F80B: 33 C0              xor         eax,eax
  000000014001F80D: E9 B0 4D 00 00     jmp         00000001400245C2
  000000014001F812: E9 AB 4D 00 00     jmp         00000001400245C2
  000000014001F817: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F81F: E8 8C C4 C8 00     call        0000000140CABCB0
  000000014001F824: 33 C0              xor         eax,eax
  000000014001F826: E9 97 4D 00 00     jmp         00000001400245C2
  000000014001F82B: E9 92 4D 00 00     jmp         00000001400245C2
  000000014001F830: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F838: E8 53 C5 C8 00     call        0000000140CABD90
  000000014001F83D: 33 C0              xor         eax,eax
  000000014001F83F: E9 7E 4D 00 00     jmp         00000001400245C2
  000000014001F844: E9 79 4D 00 00     jmp         00000001400245C2
  000000014001F849: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F851: E8 5A C5 C8 00     call        0000000140CABDB0
  000000014001F856: 33 C0              xor         eax,eax
  000000014001F858: E9 65 4D 00 00     jmp         00000001400245C2
  000000014001F85D: E9 60 4D 00 00     jmp         00000001400245C2
  000000014001F862: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F86A: E8 F1 C5 C8 00     call        0000000140CABE60
  000000014001F86F: 33 C0              xor         eax,eax
  000000014001F871: E9 4C 4D 00 00     jmp         00000001400245C2
  000000014001F876: E9 47 4D 00 00     jmp         00000001400245C2
  000000014001F87B: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F883: E8 88 C6 C8 00     call        0000000140CABF10
  000000014001F888: 33 C0              xor         eax,eax
  000000014001F88A: E9 33 4D 00 00     jmp         00000001400245C2
  000000014001F88F: E9 2E 4D 00 00     jmp         00000001400245C2
  000000014001F894: 33 C0              xor         eax,eax
  000000014001F896: 85 C0              test        eax,eax
  000000014001F898: 74 2D              je          000000014001F8C7
  000000014001F89A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F8A2: 48 83 C0 08        add         rax,8
  000000014001F8A6: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F8AE: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F8B6: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F8BA: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F8BD: 48 89 84 24 F0 05  mov         qword ptr [rsp+00000000000005F0h],rax
                    00 00
  000000014001F8C5: EB 28              jmp         000000014001F8EF
  000000014001F8C7: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F8CF: 48 83 C0 08        add         rax,8
  000000014001F8D3: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001F8DB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001F8E3: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001F8E7: 48 89 84 24 F0 05  mov         qword ptr [rsp+00000000000005F0h],rax
                    00 00
  000000014001F8EF: 48 8B 84 24 F0 05  mov         rax,qword ptr [rsp+00000000000005F0h]
                    00 00
  000000014001F8F7: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  000000014001F8FF: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F907: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F90A: 48 8B 94 24 E8 00  mov         rdx,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000014001F912: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F91A: FF 90 30 02 00 00  call        qword ptr [rax+0000000000000230h]
  000000014001F920: 33 C0              xor         eax,eax
  000000014001F922: E9 9B 4C 00 00     jmp         00000001400245C2
  000000014001F927: E9 96 4C 00 00     jmp         00000001400245C2
  000000014001F92C: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F934: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F937: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F93F: FF 90 38 02 00 00  call        qword ptr [rax+0000000000000238h]
  000000014001F945: 33 C0              xor         eax,eax
  000000014001F947: E9 76 4C 00 00     jmp         00000001400245C2
  000000014001F94C: E9 71 4C 00 00     jmp         00000001400245C2
  000000014001F951: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F959: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F95C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F964: FF 90 40 02 00 00  call        qword ptr [rax+0000000000000240h]
  000000014001F96A: E9 53 4C 00 00     jmp         00000001400245C2
  000000014001F96F: E9 4E 4C 00 00     jmp         00000001400245C2
  000000014001F974: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F97C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F97F: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F987: FF 90 48 02 00 00  call        qword ptr [rax+0000000000000248h]
  000000014001F98D: E9 30 4C 00 00     jmp         00000001400245C2
  000000014001F992: E9 2B 4C 00 00     jmp         00000001400245C2
  000000014001F997: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F99F: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F9A2: 48 8D 94 24 18 03  lea         rdx,[rsp+0000000000000318h]
                    00 00
  000000014001F9AA: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F9B2: FF 90 50 02 00 00  call        qword ptr [rax+0000000000000250h]
  000000014001F9B8: 48 8D 8C 24 18 03  lea         rcx,[rsp+0000000000000318h]
                    00 00
  000000014001F9C0: FF 15 1A 60 DE 01  call        qword ptr [0000000141E059E0h]
  000000014001F9C6: 33 C0              xor         eax,eax
  000000014001F9C8: E9 F5 4B 00 00     jmp         00000001400245C2
  000000014001F9CD: E9 F0 4B 00 00     jmp         00000001400245C2
  000000014001F9D2: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F9DA: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001F9DD: 48 8D 94 24 20 03  lea         rdx,[rsp+0000000000000320h]
                    00 00
  000000014001F9E5: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001F9ED: FF 90 58 02 00 00  call        qword ptr [rax+0000000000000258h]
  000000014001F9F3: 48 8D 8C 24 20 03  lea         rcx,[rsp+0000000000000320h]
                    00 00
  000000014001F9FB: FF 15 DF 5F DE 01  call        qword ptr [0000000141E059E0h]
  000000014001FA01: 33 C0              xor         eax,eax
  000000014001FA03: E9 BA 4B 00 00     jmp         00000001400245C2
  000000014001FA08: E9 B5 4B 00 00     jmp         00000001400245C2
  000000014001FA0D: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FA15: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FA18: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FA20: FF 90 60 02 00 00  call        qword ptr [rax+0000000000000260h]
  000000014001FA26: E9 97 4B 00 00     jmp         00000001400245C2
  000000014001FA2B: E9 92 4B 00 00     jmp         00000001400245C2
  000000014001FA30: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FA38: E8 23 B9 C8 00     call        0000000140CAB360
  000000014001FA3D: 33 C0              xor         eax,eax
  000000014001FA3F: E9 7E 4B 00 00     jmp         00000001400245C2
  000000014001FA44: E9 79 4B 00 00     jmp         00000001400245C2
  000000014001FA49: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FA51: E8 FA B9 C8 00     call        0000000140CAB450
  000000014001FA56: 33 C0              xor         eax,eax
  000000014001FA58: E9 65 4B 00 00     jmp         00000001400245C2
  000000014001FA5D: E9 60 4B 00 00     jmp         00000001400245C2
  000000014001FA62: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FA6A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FA6D: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FA75: FF 90 68 02 00 00  call        qword ptr [rax+0000000000000268h]
  000000014001FA7B: 33 C0              xor         eax,eax
  000000014001FA7D: E9 40 4B 00 00     jmp         00000001400245C2
  000000014001FA82: E9 3B 4B 00 00     jmp         00000001400245C2
  000000014001FA87: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FA8F: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FA92: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FA9A: FF 90 70 02 00 00  call        qword ptr [rax+0000000000000270h]
  000000014001FAA0: 33 C0              xor         eax,eax
  000000014001FAA2: E9 1B 4B 00 00     jmp         00000001400245C2
  000000014001FAA7: E9 16 4B 00 00     jmp         00000001400245C2
  000000014001FAAC: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FAB4: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FAB7: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FABF: FF 90 78 02 00 00  call        qword ptr [rax+0000000000000278h]
  000000014001FAC5: 33 C0              xor         eax,eax
  000000014001FAC7: E9 F6 4A 00 00     jmp         00000001400245C2
  000000014001FACC: E9 F1 4A 00 00     jmp         00000001400245C2
  000000014001FAD1: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FAD9: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FADC: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FAE4: FF 90 80 02 00 00  call        qword ptr [rax+0000000000000280h]
  000000014001FAEA: 33 C0              xor         eax,eax
  000000014001FAEC: E9 D1 4A 00 00     jmp         00000001400245C2
  000000014001FAF1: E9 CC 4A 00 00     jmp         00000001400245C2
  000000014001FAF6: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FAFE: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FB01: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FB09: FF 90 88 02 00 00  call        qword ptr [rax+0000000000000288h]
  000000014001FB0F: 33 C0              xor         eax,eax
  000000014001FB11: E9 AC 4A 00 00     jmp         00000001400245C2
  000000014001FB16: E9 A7 4A 00 00     jmp         00000001400245C2
  000000014001FB1B: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FB23: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FB26: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FB2E: FF 90 90 02 00 00  call        qword ptr [rax+0000000000000290h]
  000000014001FB34: 33 C0              xor         eax,eax
  000000014001FB36: E9 87 4A 00 00     jmp         00000001400245C2
  000000014001FB3B: E9 82 4A 00 00     jmp         00000001400245C2
  000000014001FB40: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FB48: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FB4B: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FB53: FF 90 98 02 00 00  call        qword ptr [rax+0000000000000298h]
  000000014001FB59: 33 C0              xor         eax,eax
  000000014001FB5B: E9 62 4A 00 00     jmp         00000001400245C2
  000000014001FB60: E9 5D 4A 00 00     jmp         00000001400245C2
  000000014001FB65: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FB6D: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FB70: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FB78: FF 90 A0 02 00 00  call        qword ptr [rax+00000000000002A0h]
  000000014001FB7E: 33 C0              xor         eax,eax
  000000014001FB80: E9 3D 4A 00 00     jmp         00000001400245C2
  000000014001FB85: E9 38 4A 00 00     jmp         00000001400245C2
  000000014001FB8A: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FB92: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FB95: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FB9D: FF 90 A8 02 00 00  call        qword ptr [rax+00000000000002A8h]
  000000014001FBA3: 33 C0              xor         eax,eax
  000000014001FBA5: E9 18 4A 00 00     jmp         00000001400245C2
  000000014001FBAA: E9 13 4A 00 00     jmp         00000001400245C2
  000000014001FBAF: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FBB7: E8 64 C8 C7 00     call        0000000140C9C420
  000000014001FBBC: 33 C0              xor         eax,eax
  000000014001FBBE: E9 FF 49 00 00     jmp         00000001400245C2
  000000014001FBC3: E9 FA 49 00 00     jmp         00000001400245C2
  000000014001FBC8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FBD0: E8 BB C8 C7 00     call        0000000140C9C490
  000000014001FBD5: 33 C0              xor         eax,eax
  000000014001FBD7: E9 E6 49 00 00     jmp         00000001400245C2
  000000014001FBDC: E9 E1 49 00 00     jmp         00000001400245C2
  000000014001FBE1: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FBE9: E8 52 C9 C7 00     call        0000000140C9C540
  000000014001FBEE: 33 C0              xor         eax,eax
  000000014001FBF0: E9 CD 49 00 00     jmp         00000001400245C2
  000000014001FBF5: E9 C8 49 00 00     jmp         00000001400245C2
  000000014001FBFA: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FC02: E8 59 C9 C7 00     call        0000000140C9C560
  000000014001FC07: 33 C0              xor         eax,eax
  000000014001FC09: E9 B4 49 00 00     jmp         00000001400245C2
  000000014001FC0E: E9 AF 49 00 00     jmp         00000001400245C2
  000000014001FC13: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FC1B: E8 10 99 C7 00     call        0000000140C99530
  000000014001FC20: 33 C0              xor         eax,eax
  000000014001FC22: E9 9B 49 00 00     jmp         00000001400245C2
  000000014001FC27: E9 96 49 00 00     jmp         00000001400245C2
  000000014001FC2C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FC34: E8 57 9C C7 00     call        0000000140C99890
  000000014001FC39: 33 C0              xor         eax,eax
  000000014001FC3B: E9 82 49 00 00     jmp         00000001400245C2
  000000014001FC40: E9 7D 49 00 00     jmp         00000001400245C2
  000000014001FC45: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FC4D: E8 BE D9 C7 00     call        0000000140C9D610
  000000014001FC52: 33 C0              xor         eax,eax
  000000014001FC54: E9 69 49 00 00     jmp         00000001400245C2
  000000014001FC59: E9 64 49 00 00     jmp         00000001400245C2
  000000014001FC5E: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FC66: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FC69: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FC71: FF 90 B0 02 00 00  call        qword ptr [rax+00000000000002B0h]
  000000014001FC77: 33 C0              xor         eax,eax
  000000014001FC79: E9 44 49 00 00     jmp         00000001400245C2
  000000014001FC7E: E9 3F 49 00 00     jmp         00000001400245C2
  000000014001FC83: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FC8B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FC8E: 48 8D 94 24 28 03  lea         rdx,[rsp+0000000000000328h]
                    00 00
  000000014001FC96: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FC9E: FF 90 B8 02 00 00  call        qword ptr [rax+00000000000002B8h]
  000000014001FCA4: 48 8D 8C 24 28 03  lea         rcx,[rsp+0000000000000328h]
                    00 00
  000000014001FCAC: FF 15 2E 5D DE 01  call        qword ptr [0000000141E059E0h]
  000000014001FCB2: 33 C0              xor         eax,eax
  000000014001FCB4: E9 09 49 00 00     jmp         00000001400245C2
  000000014001FCB9: E9 04 49 00 00     jmp         00000001400245C2
  000000014001FCBE: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FCC6: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FCC9: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FCD1: FF 90 C0 02 00 00  call        qword ptr [rax+00000000000002C0h]
  000000014001FCD7: 33 C0              xor         eax,eax
  000000014001FCD9: E9 E4 48 00 00     jmp         00000001400245C2
  000000014001FCDE: E9 DF 48 00 00     jmp         00000001400245C2
  000000014001FCE3: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FCEB: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FCEE: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FCF6: FF 90 C8 02 00 00  call        qword ptr [rax+00000000000002C8h]
  000000014001FCFC: 33 C0              xor         eax,eax
  000000014001FCFE: E9 BF 48 00 00     jmp         00000001400245C2
  000000014001FD03: E9 BA 48 00 00     jmp         00000001400245C2
  000000014001FD08: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FD10: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FD13: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FD1B: FF 90 D0 02 00 00  call        qword ptr [rax+00000000000002D0h]
  000000014001FD21: 33 C0              xor         eax,eax
  000000014001FD23: E9 9A 48 00 00     jmp         00000001400245C2
  000000014001FD28: E9 95 48 00 00     jmp         00000001400245C2
  000000014001FD2D: 33 C0              xor         eax,eax
  000000014001FD2F: 85 C0              test        eax,eax
  000000014001FD31: 74 2D              je          000000014001FD60
  000000014001FD33: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001FD3B: 48 83 C0 08        add         rax,8
  000000014001FD3F: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001FD47: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001FD4F: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001FD53: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FD56: 48 89 84 24 F8 05  mov         qword ptr [rsp+00000000000005F8h],rax
                    00 00
  000000014001FD5E: EB 28              jmp         000000014001FD88
  000000014001FD60: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001FD68: 48 83 C0 08        add         rax,8
  000000014001FD6C: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001FD74: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001FD7C: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001FD80: 48 89 84 24 F8 05  mov         qword ptr [rsp+00000000000005F8h],rax
                    00 00
  000000014001FD88: 48 8B 84 24 F8 05  mov         rax,qword ptr [rsp+00000000000005F8h]
                    00 00
  000000014001FD90: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  000000014001FD98: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FDA0: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FDA3: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  000000014001FDAB: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FDB3: FF 90 E8 00 00 00  call        qword ptr [rax+00000000000000E8h]
  000000014001FDB9: 33 C0              xor         eax,eax
  000000014001FDBB: E9 02 48 00 00     jmp         00000001400245C2
  000000014001FDC0: E9 FD 47 00 00     jmp         00000001400245C2
  000000014001FDC5: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FDCD: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FDD0: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FDD8: FF 90 D8 02 00 00  call        qword ptr [rax+00000000000002D8h]
  000000014001FDDE: 0F B6 C0           movzx       eax,al
  000000014001FDE1: E9 DC 47 00 00     jmp         00000001400245C2
  000000014001FDE6: E9 D7 47 00 00     jmp         00000001400245C2
  000000014001FDEB: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FDF3: E8 C8 B9 C7 00     call        0000000140C9B7C0
  000000014001FDF8: 33 C0              xor         eax,eax
  000000014001FDFA: E9 C3 47 00 00     jmp         00000001400245C2
  000000014001FDFF: E9 BE 47 00 00     jmp         00000001400245C2
  000000014001FE04: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FE0C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FE0F: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FE17: FF 90 E0 02 00 00  call        qword ptr [rax+00000000000002E0h]
  000000014001FE1D: 33 C0              xor         eax,eax
  000000014001FE1F: E9 9E 47 00 00     jmp         00000001400245C2
  000000014001FE24: E9 99 47 00 00     jmp         00000001400245C2
  000000014001FE29: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FE31: E8 AA B9 C7 00     call        0000000140C9B7E0
  000000014001FE36: 33 C0              xor         eax,eax
  000000014001FE38: E9 85 47 00 00     jmp         00000001400245C2
  000000014001FE3D: E9 80 47 00 00     jmp         00000001400245C2
  000000014001FE42: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FE4A: E8 11 BA C7 00     call        0000000140C9B860
  000000014001FE4F: 0F B6 C0           movzx       eax,al
  000000014001FE52: E9 6B 47 00 00     jmp         00000001400245C2
  000000014001FE57: E9 66 47 00 00     jmp         00000001400245C2
  000000014001FE5C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FE64: E8 57 BA C7 00     call        0000000140C9B8C0
  000000014001FE69: 0F B6 C0           movzx       eax,al
  000000014001FE6C: E9 51 47 00 00     jmp         00000001400245C2
  000000014001FE71: E9 4C 47 00 00     jmp         00000001400245C2
  000000014001FE76: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FE7E: E8 8D BA C7 00     call        0000000140C9B910
  000000014001FE83: 33 C0              xor         eax,eax
  000000014001FE85: E9 38 47 00 00     jmp         00000001400245C2
  000000014001FE8A: E9 33 47 00 00     jmp         00000001400245C2
  000000014001FE8F: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FE97: E8 F4 BA C7 00     call        0000000140C9B990
  000000014001FE9C: 33 C0              xor         eax,eax
  000000014001FE9E: E9 1F 47 00 00     jmp         00000001400245C2
  000000014001FEA3: E9 1A 47 00 00     jmp         00000001400245C2
  000000014001FEA8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FEB0: E8 FB BA C7 00     call        0000000140C9B9B0
  000000014001FEB5: 33 C0              xor         eax,eax
  000000014001FEB7: E9 06 47 00 00     jmp         00000001400245C2
  000000014001FEBC: E9 01 47 00 00     jmp         00000001400245C2
  000000014001FEC1: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FEC9: E8 52 BB C7 00     call        0000000140C9BA20
  000000014001FECE: 33 C0              xor         eax,eax
  000000014001FED0: E9 ED 46 00 00     jmp         00000001400245C2
  000000014001FED5: E9 E8 46 00 00     jmp         00000001400245C2
  000000014001FEDA: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FEE2: E8 B9 BB C7 00     call        0000000140C9BAA0
  000000014001FEE7: 33 C0              xor         eax,eax
  000000014001FEE9: E9 D4 46 00 00     jmp         00000001400245C2
  000000014001FEEE: E9 CF 46 00 00     jmp         00000001400245C2
  000000014001FEF3: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FEFB: E8 C0 BB C7 00     call        0000000140C9BAC0
  000000014001FF00: 33 C0              xor         eax,eax
  000000014001FF02: E9 BB 46 00 00     jmp         00000001400245C2
  000000014001FF07: E9 B6 46 00 00     jmp         00000001400245C2
  000000014001FF0C: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FF14: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001FF17: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FF1F: FF 90 E8 02 00 00  call        qword ptr [rax+00000000000002E8h]
  000000014001FF25: 33 C0              xor         eax,eax
  000000014001FF27: E9 96 46 00 00     jmp         00000001400245C2
  000000014001FF2C: E9 91 46 00 00     jmp         00000001400245C2
  000000014001FF31: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FF39: E8 B2 75 C8 00     call        0000000140CA74F0
  000000014001FF3E: 33 C0              xor         eax,eax
  000000014001FF40: E9 7D 46 00 00     jmp         00000001400245C2
  000000014001FF45: E9 78 46 00 00     jmp         00000001400245C2
  000000014001FF4A: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014001FF52: E8 89 77 C8 00     call        0000000140CA76E0
  000000014001FF57: 33 C0              xor         eax,eax
  000000014001FF59: E9 64 46 00 00     jmp         00000001400245C2
  000000014001FF5E: E9 5F 46 00 00     jmp         00000001400245C2
  000000014001FF63: 33 C0              xor         eax,eax
  000000014001FF65: 85 C0              test        eax,eax
  000000014001FF67: 74 2A              je          000000014001FF93
  000000014001FF69: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001FF71: 48 83 C0 08        add         rax,8
  000000014001FF75: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001FF7D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001FF85: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014001FF89: 48 89 84 24 00 06  mov         qword ptr [rsp+0000000000000600h],rax
                    00 00
  000000014001FF91: EB 28              jmp         000000014001FFBB
  000000014001FF93: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001FF9B: 48 83 C0 08        add         rax,8
  000000014001FF9F: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001FFA7: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001FFAF: 48 83 E8 08        sub         rax,8
  000000014001FFB3: 48 89 84 24 00 06  mov         qword ptr [rsp+0000000000000600h],rax
                    00 00
  000000014001FFBB: 48 8B 84 24 00 06  mov         rax,qword ptr [rsp+0000000000000600h]
                    00 00
  000000014001FFC3: 48 89 84 24 30 03  mov         qword ptr [rsp+0000000000000330h],rax
                    00 00
  000000014001FFCB: 48 8B 94 24 30 03  mov         rdx,qword ptr [rsp+0000000000000330h]
                    00 00
  000000014001FFD3: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014001FFDB: FF 15 F7 59 DE 01  call        qword ptr [0000000141E059D8h]
  000000014001FFE1: 90                 nop
  000000014001FFE2: 33 C0              xor         eax,eax
  000000014001FFE4: 85 C0              test        eax,eax
  000000014001FFE6: 74 2A              je          0000000140020012
  000000014001FFE8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014001FFF0: 48 83 C0 08        add         rax,8
  000000014001FFF4: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014001FFFC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020004: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020008: 48 89 84 24 08 06  mov         qword ptr [rsp+0000000000000608h],rax
                    00 00
  0000000140020010: EB 28              jmp         000000014002003A
  0000000140020012: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002001A: 48 83 C0 08        add         rax,8
  000000014002001E: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020026: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002002E: 48 83 E8 08        sub         rax,8
  0000000140020032: 48 89 84 24 08 06  mov         qword ptr [rsp+0000000000000608h],rax
                    00 00
  000000014002003A: 48 8B 84 24 08 06  mov         rax,qword ptr [rsp+0000000000000608h]
                    00 00
  0000000140020042: 48 89 84 24 38 03  mov         qword ptr [rsp+0000000000000338h],rax
                    00 00
  000000014002004A: 48 8B 94 24 38 03  mov         rdx,qword ptr [rsp+0000000000000338h]
                    00 00
  0000000140020052: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000014002005A: E8 91 49 FE FF     call        00000001400049F0
  000000014002005F: 90                 nop
  0000000140020060: 33 C0              xor         eax,eax
  0000000140020062: 85 C0              test        eax,eax
  0000000140020064: 74 2B              je          0000000140020091
  0000000140020066: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002006E: 48 83 C0 08        add         rax,8
  0000000140020072: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002007A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020082: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020086: 8B 00              mov         eax,dword ptr [rax]
  0000000140020088: 89 84 24 10 06 00  mov         dword ptr [rsp+0000000000000610h],eax
                    00
  000000014002008F: EB 26              jmp         00000001400200B7
  0000000140020091: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020099: 48 83 C0 08        add         rax,8
  000000014002009D: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400200A5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400200AD: 8B 40 F8           mov         eax,dword ptr [rax-8]
  00000001400200B0: 89 84 24 10 06 00  mov         dword ptr [rsp+0000000000000610h],eax
                    00
  00000001400200B7: 8B 84 24 10 06 00  mov         eax,dword ptr [rsp+0000000000000610h]
                    00
  00000001400200BE: 89 84 24 04 01 00  mov         dword ptr [rsp+0000000000000104h],eax
                    00
  00000001400200C5: 33 C0              xor         eax,eax
  00000001400200C7: 85 C0              test        eax,eax
  00000001400200C9: 74 2B              je          00000001400200F6
  00000001400200CB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400200D3: 48 83 C0 08        add         rax,8
  00000001400200D7: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400200DF: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400200E7: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400200EB: 8B 00              mov         eax,dword ptr [rax]
  00000001400200ED: 89 84 24 14 06 00  mov         dword ptr [rsp+0000000000000614h],eax
                    00
  00000001400200F4: EB 26              jmp         000000014002011C
  00000001400200F6: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400200FE: 48 83 C0 08        add         rax,8
  0000000140020102: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002010A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020112: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140020115: 89 84 24 14 06 00  mov         dword ptr [rsp+0000000000000614h],eax
                    00
  000000014002011C: 8B 84 24 14 06 00  mov         eax,dword ptr [rsp+0000000000000614h]
                    00
  0000000140020123: 89 84 24 08 01 00  mov         dword ptr [rsp+0000000000000108h],eax
                    00
  000000014002012A: 33 C0              xor         eax,eax
  000000014002012C: 85 C0              test        eax,eax
  000000014002012E: 74 2C              je          000000014002015C
  0000000140020130: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020138: 48 83 C0 08        add         rax,8
  000000014002013C: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020144: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002014C: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020150: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000140020153: 88 84 24 18 06 00  mov         byte ptr [rsp+0000000000000618h],al
                    00
  000000014002015A: EB 27              jmp         0000000140020183
  000000014002015C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020164: 48 83 C0 08        add         rax,8
  0000000140020168: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020170: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020178: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  000000014002017C: 88 84 24 18 06 00  mov         byte ptr [rsp+0000000000000618h],al
                    00
  0000000140020183: 0F B6 84 24 18 06  movzx       eax,byte ptr [rsp+0000000000000618h]
                    00 00
  000000014002018B: 88 84 24 00 01 00  mov         byte ptr [rsp+0000000000000100h],al
                    00
  0000000140020192: 48 8D 84 24 40 03  lea         rax,[rsp+0000000000000340h]
                    00 00
  000000014002019A: 48 89 84 24 48 03  mov         qword ptr [rsp+0000000000000348h],rax
                    00 00
  00000001400201A2: 48 8D 84 24 50 03  lea         rax,[rsp+0000000000000350h]
                    00 00
  00000001400201AA: 48 89 84 24 58 03  mov         qword ptr [rsp+0000000000000358h],rax
                    00 00
  00000001400201B2: 48 8D 94 24 10 01  lea         rdx,[rsp+0000000000000110h]
                    00 00
  00000001400201BA: 48 8B 8C 24 48 03  mov         rcx,qword ptr [rsp+0000000000000348h]
                    00 00
  00000001400201C2: E8 29 48 FE FF     call        00000001400049F0
  00000001400201C7: 48 89 84 24 20 06  mov         qword ptr [rsp+0000000000000620h],rax
                    00 00
  00000001400201CF: 48 8B 84 24 20 06  mov         rax,qword ptr [rsp+0000000000000620h]
                    00 00
  00000001400201D7: 48 89 84 24 28 06  mov         qword ptr [rsp+0000000000000628h],rax
                    00 00
  00000001400201DF: 48 8D 94 24 F8 00  lea         rdx,[rsp+00000000000000F8h]
                    00 00
  00000001400201E7: 48 8B 8C 24 58 03  mov         rcx,qword ptr [rsp+0000000000000358h]
                    00 00
  00000001400201EF: FF 15 E3 57 DE 01  call        qword ptr [0000000141E059D8h]
  00000001400201F5: 48 89 84 24 30 06  mov         qword ptr [rsp+0000000000000630h],rax
                    00 00
  00000001400201FD: 0F B6 84 24 00 01  movzx       eax,byte ptr [rsp+0000000000000100h]
                    00 00
  0000000140020205: 88 44 24 28        mov         byte ptr [rsp+28h],al
  0000000140020209: 8B 84 24 08 01 00  mov         eax,dword ptr [rsp+0000000000000108h]
                    00
  0000000140020210: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140020214: 44 8B 8C 24 04 01  mov         r9d,dword ptr [rsp+0000000000000104h]
                    00 00
  000000014002021C: 4C 8B 84 24 28 06  mov         r8,qword ptr [rsp+0000000000000628h]
                    00 00
  0000000140020224: 48 8B 94 24 30 06  mov         rdx,qword ptr [rsp+0000000000000630h]
                    00 00
  000000014002022C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020234: E8 D7 75 C8 00     call        0000000140CA7810
  0000000140020239: C7 84 24 60 03 00  mov         dword ptr [rsp+0000000000000360h],0
                    00 00 00 00 00
  0000000140020244: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000014002024C: E8 7F 46 FE FF     call        00000001400048D0
  0000000140020251: 90                 nop
  0000000140020252: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014002025A: FF 15 80 57 DE 01  call        qword ptr [0000000141E059E0h]
  0000000140020260: 8B 84 24 60 03 00  mov         eax,dword ptr [rsp+0000000000000360h]
                    00
  0000000140020267: E9 56 43 00 00     jmp         00000001400245C2
  000000014002026C: E9 51 43 00 00     jmp         00000001400245C2
  0000000140020271: 33 C0              xor         eax,eax
  0000000140020273: 85 C0              test        eax,eax
  0000000140020275: 74 2A              je          00000001400202A1
  0000000140020277: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002027F: 48 83 C0 08        add         rax,8
  0000000140020283: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002028B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020293: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020297: 48 89 84 24 38 06  mov         qword ptr [rsp+0000000000000638h],rax
                    00 00
  000000014002029F: EB 28              jmp         00000001400202C9
  00000001400202A1: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400202A9: 48 83 C0 08        add         rax,8
  00000001400202AD: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400202B5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400202BD: 48 83 E8 08        sub         rax,8
  00000001400202C1: 48 89 84 24 38 06  mov         qword ptr [rsp+0000000000000638h],rax
                    00 00
  00000001400202C9: 48 8B 84 24 38 06  mov         rax,qword ptr [rsp+0000000000000638h]
                    00 00
  00000001400202D1: 48 89 84 24 68 03  mov         qword ptr [rsp+0000000000000368h],rax
                    00 00
  00000001400202D9: 48 8B 94 24 68 03  mov         rdx,qword ptr [rsp+0000000000000368h]
                    00 00
  00000001400202E1: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  00000001400202E9: E8 02 47 FE FF     call        00000001400049F0
  00000001400202EE: 90                 nop
  00000001400202EF: 48 8D 84 24 70 03  lea         rax,[rsp+0000000000000370h]
                    00 00
  00000001400202F7: 48 89 84 24 78 03  mov         qword ptr [rsp+0000000000000378h],rax
                    00 00
  00000001400202FF: 48 8D 94 24 18 01  lea         rdx,[rsp+0000000000000118h]
                    00 00
  0000000140020307: 48 8B 8C 24 78 03  mov         rcx,qword ptr [rsp+0000000000000378h]
                    00 00
  000000014002030F: E8 DC 46 FE FF     call        00000001400049F0
  0000000140020314: 48 89 84 24 40 06  mov         qword ptr [rsp+0000000000000640h],rax
                    00 00
  000000014002031C: 48 8B 94 24 40 06  mov         rdx,qword ptr [rsp+0000000000000640h]
                    00 00
  0000000140020324: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002032C: E8 7F 89 C8 00     call        0000000140CA8CB0
  0000000140020331: C7 84 24 80 03 00  mov         dword ptr [rsp+0000000000000380h],0
                    00 00 00 00 00
  000000014002033C: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000140020344: E8 87 45 FE FF     call        00000001400048D0
  0000000140020349: 8B 84 24 80 03 00  mov         eax,dword ptr [rsp+0000000000000380h]
                    00
  0000000140020350: E9 6D 42 00 00     jmp         00000001400245C2
  0000000140020355: E9 68 42 00 00     jmp         00000001400245C2
  000000014002035A: 33 C0              xor         eax,eax
  000000014002035C: 85 C0              test        eax,eax
  000000014002035E: 74 2A              je          000000014002038A
  0000000140020360: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020368: 48 83 C0 08        add         rax,8
  000000014002036C: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020374: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002037C: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020380: 48 89 84 24 48 06  mov         qword ptr [rsp+0000000000000648h],rax
                    00 00
  0000000140020388: EB 28              jmp         00000001400203B2
  000000014002038A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020392: 48 83 C0 08        add         rax,8
  0000000140020396: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002039E: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400203A6: 48 83 E8 08        sub         rax,8
  00000001400203AA: 48 89 84 24 48 06  mov         qword ptr [rsp+0000000000000648h],rax
                    00 00
  00000001400203B2: 48 8B 84 24 48 06  mov         rax,qword ptr [rsp+0000000000000648h]
                    00 00
  00000001400203BA: 48 89 84 24 88 03  mov         qword ptr [rsp+0000000000000388h],rax
                    00 00
  00000001400203C2: 48 8B 94 24 88 03  mov         rdx,qword ptr [rsp+0000000000000388h]
                    00 00
  00000001400203CA: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001400203D2: FF 15 00 56 DE 01  call        qword ptr [0000000141E059D8h]
  00000001400203D8: 90                 nop
  00000001400203D9: 33 C0              xor         eax,eax
  00000001400203DB: 85 C0              test        eax,eax
  00000001400203DD: 74 2B              je          000000014002040A
  00000001400203DF: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400203E7: 48 83 C0 08        add         rax,8
  00000001400203EB: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400203F3: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400203FB: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400203FF: 8B 00              mov         eax,dword ptr [rax]
  0000000140020401: 89 84 24 50 06 00  mov         dword ptr [rsp+0000000000000650h],eax
                    00
  0000000140020408: EB 26              jmp         0000000140020430
  000000014002040A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020412: 48 83 C0 08        add         rax,8
  0000000140020416: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002041E: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020426: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140020429: 89 84 24 50 06 00  mov         dword ptr [rsp+0000000000000650h],eax
                    00
  0000000140020430: 8B 84 24 50 06 00  mov         eax,dword ptr [rsp+0000000000000650h]
                    00
  0000000140020437: 89 84 24 28 01 00  mov         dword ptr [rsp+0000000000000128h],eax
                    00
  000000014002043E: 48 8D 84 24 90 03  lea         rax,[rsp+0000000000000390h]
                    00 00
  0000000140020446: 48 89 84 24 98 03  mov         qword ptr [rsp+0000000000000398h],rax
                    00 00
  000000014002044E: 48 8D 94 24 20 01  lea         rdx,[rsp+0000000000000120h]
                    00 00
  0000000140020456: 48 8B 8C 24 98 03  mov         rcx,qword ptr [rsp+0000000000000398h]
                    00 00
  000000014002045E: FF 15 74 55 DE 01  call        qword ptr [0000000141E059D8h]
  0000000140020464: 48 89 84 24 58 06  mov         qword ptr [rsp+0000000000000658h],rax
                    00 00
  000000014002046C: 44 8B 84 24 28 01  mov         r8d,dword ptr [rsp+0000000000000128h]
                    00 00
  0000000140020474: 48 8B 94 24 58 06  mov         rdx,qword ptr [rsp+0000000000000658h]
                    00 00
  000000014002047C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020484: E8 D7 92 C8 00     call        0000000140CA9760
  0000000140020489: C7 84 24 A0 03 00  mov         dword ptr [rsp+00000000000003A0h],0
                    00 00 00 00 00
  0000000140020494: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  000000014002049C: FF 15 3E 55 DE 01  call        qword ptr [0000000141E059E0h]
  00000001400204A2: 8B 84 24 A0 03 00  mov         eax,dword ptr [rsp+00000000000003A0h]
                    00
  00000001400204A9: E9 14 41 00 00     jmp         00000001400245C2
  00000001400204AE: E9 0F 41 00 00     jmp         00000001400245C2
  00000001400204B3: 33 C0              xor         eax,eax
  00000001400204B5: 85 C0              test        eax,eax
  00000001400204B7: 74 2A              je          00000001400204E3
  00000001400204B9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400204C1: 48 83 C0 08        add         rax,8
  00000001400204C5: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400204CD: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400204D5: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400204D9: 48 89 84 24 60 06  mov         qword ptr [rsp+0000000000000660h],rax
                    00 00
  00000001400204E1: EB 28              jmp         000000014002050B
  00000001400204E3: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400204EB: 48 83 C0 08        add         rax,8
  00000001400204EF: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400204F7: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400204FF: 48 83 E8 08        sub         rax,8
  0000000140020503: 48 89 84 24 60 06  mov         qword ptr [rsp+0000000000000660h],rax
                    00 00
  000000014002050B: 48 8B 84 24 60 06  mov         rax,qword ptr [rsp+0000000000000660h]
                    00 00
  0000000140020513: 48 89 84 24 A8 03  mov         qword ptr [rsp+00000000000003A8h],rax
                    00 00
  000000014002051B: 48 8B 94 24 A8 03  mov         rdx,qword ptr [rsp+00000000000003A8h]
                    00 00
  0000000140020523: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  000000014002052B: FF 15 A7 54 DE 01  call        qword ptr [0000000141E059D8h]
  0000000140020531: 90                 nop
  0000000140020532: 48 8D 84 24 B0 03  lea         rax,[rsp+00000000000003B0h]
                    00 00
  000000014002053A: 48 89 84 24 B8 03  mov         qword ptr [rsp+00000000000003B8h],rax
                    00 00
  0000000140020542: 48 8D 94 24 30 01  lea         rdx,[rsp+0000000000000130h]
                    00 00
  000000014002054A: 48 8B 8C 24 B8 03  mov         rcx,qword ptr [rsp+00000000000003B8h]
                    00 00
  0000000140020552: FF 15 80 54 DE 01  call        qword ptr [0000000141E059D8h]
  0000000140020558: 48 89 84 24 68 06  mov         qword ptr [rsp+0000000000000668h],rax
                    00 00
  0000000140020560: 48 8B 94 24 68 06  mov         rdx,qword ptr [rsp+0000000000000668h]
                    00 00
  0000000140020568: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020570: E8 EB 90 C8 00     call        0000000140CA9660
  0000000140020575: C7 84 24 C0 03 00  mov         dword ptr [rsp+00000000000003C0h],0
                    00 00 00 00 00
  0000000140020580: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  0000000140020588: FF 15 52 54 DE 01  call        qword ptr [0000000141E059E0h]
  000000014002058E: 8B 84 24 C0 03 00  mov         eax,dword ptr [rsp+00000000000003C0h]
                    00
  0000000140020595: E9 28 40 00 00     jmp         00000001400245C2
  000000014002059A: E9 23 40 00 00     jmp         00000001400245C2
  000000014002059F: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400205A7: E8 B4 9A C8 00     call        0000000140CAA060
  00000001400205AC: 33 C0              xor         eax,eax
  00000001400205AE: E9 0F 40 00 00     jmp         00000001400245C2
  00000001400205B3: E9 0A 40 00 00     jmp         00000001400245C2
  00000001400205B8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400205C0: E8 3B 87 C8 00     call        0000000140CA8D00
  00000001400205C5: 33 C0              xor         eax,eax
  00000001400205C7: E9 F6 3F 00 00     jmp         00000001400245C2
  00000001400205CC: E9 F1 3F 00 00     jmp         00000001400245C2
  00000001400205D1: 33 C0              xor         eax,eax
  00000001400205D3: 85 C0              test        eax,eax
  00000001400205D5: 74 2B              je          0000000140020602
  00000001400205D7: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400205DF: 48 83 C0 08        add         rax,8
  00000001400205E3: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400205EB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400205F3: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400205F7: 8B 00              mov         eax,dword ptr [rax]
  00000001400205F9: 89 84 24 70 06 00  mov         dword ptr [rsp+0000000000000670h],eax
                    00
  0000000140020600: EB 26              jmp         0000000140020628
  0000000140020602: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002060A: 48 83 C0 08        add         rax,8
  000000014002060E: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020616: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002061E: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140020621: 89 84 24 70 06 00  mov         dword ptr [rsp+0000000000000670h],eax
                    00
  0000000140020628: 8B 84 24 70 06 00  mov         eax,dword ptr [rsp+0000000000000670h]
                    00
  000000014002062F: 89 84 24 38 01 00  mov         dword ptr [rsp+0000000000000138h],eax
                    00
  0000000140020636: 33 C0              xor         eax,eax
  0000000140020638: 85 C0              test        eax,eax
  000000014002063A: 74 2B              je          0000000140020667
  000000014002063C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020644: 48 83 C0 08        add         rax,8
  0000000140020648: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020650: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020658: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002065C: 8B 00              mov         eax,dword ptr [rax]
  000000014002065E: 89 84 24 74 06 00  mov         dword ptr [rsp+0000000000000674h],eax
                    00
  0000000140020665: EB 26              jmp         000000014002068D
  0000000140020667: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002066F: 48 83 C0 08        add         rax,8
  0000000140020673: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002067B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020683: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140020686: 89 84 24 74 06 00  mov         dword ptr [rsp+0000000000000674h],eax
                    00
  000000014002068D: 8B 84 24 74 06 00  mov         eax,dword ptr [rsp+0000000000000674h]
                    00
  0000000140020694: 89 84 24 3C 01 00  mov         dword ptr [rsp+000000000000013Ch],eax
                    00
  000000014002069B: 44 8B 84 24 3C 01  mov         r8d,dword ptr [rsp+000000000000013Ch]
                    00 00
  00000001400206A3: 8B 94 24 38 01 00  mov         edx,dword ptr [rsp+0000000000000138h]
                    00
  00000001400206AA: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400206B2: E8 09 87 C8 00     call        0000000140CA8DC0
  00000001400206B7: 33 C0              xor         eax,eax
  00000001400206B9: E9 04 3F 00 00     jmp         00000001400245C2
  00000001400206BE: E9 FF 3E 00 00     jmp         00000001400245C2
  00000001400206C3: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400206CB: E8 C0 89 C8 00     call        0000000140CA9090
  00000001400206D0: 33 C0              xor         eax,eax
  00000001400206D2: E9 EB 3E 00 00     jmp         00000001400245C2
  00000001400206D7: E9 E6 3E 00 00     jmp         00000001400245C2
  00000001400206DC: 33 C0              xor         eax,eax
  00000001400206DE: 85 C0              test        eax,eax
  00000001400206E0: 74 2B              je          000000014002070D
  00000001400206E2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400206EA: 48 83 C0 08        add         rax,8
  00000001400206EE: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400206F6: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400206FE: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020702: 8B 00              mov         eax,dword ptr [rax]
  0000000140020704: 89 84 24 78 06 00  mov         dword ptr [rsp+0000000000000678h],eax
                    00
  000000014002070B: EB 26              jmp         0000000140020733
  000000014002070D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020715: 48 83 C0 08        add         rax,8
  0000000140020719: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020721: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020729: 8B 40 F8           mov         eax,dword ptr [rax-8]
  000000014002072C: 89 84 24 78 06 00  mov         dword ptr [rsp+0000000000000678h],eax
                    00
  0000000140020733: 8B 84 24 78 06 00  mov         eax,dword ptr [rsp+0000000000000678h]
                    00
  000000014002073A: 89 84 24 40 01 00  mov         dword ptr [rsp+0000000000000140h],eax
                    00
  0000000140020741: 8B 94 24 40 01 00  mov         edx,dword ptr [rsp+0000000000000140h]
                    00
  0000000140020748: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020750: E8 DB 88 C8 00     call        0000000140CA9030
  0000000140020755: 33 C0              xor         eax,eax
  0000000140020757: E9 66 3E 00 00     jmp         00000001400245C2
  000000014002075C: E9 61 3E 00 00     jmp         00000001400245C2
  0000000140020761: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020769: E8 B2 8D C8 00     call        0000000140CA9520
  000000014002076E: 33 C0              xor         eax,eax
  0000000140020770: E9 4D 3E 00 00     jmp         00000001400245C2
  0000000140020775: E9 48 3E 00 00     jmp         00000001400245C2
  000000014002077A: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020782: E8 89 FD 8B 00     call        00000001408E0510
  0000000140020787: E9 36 3E 00 00     jmp         00000001400245C2
  000000014002078C: E9 31 3E 00 00     jmp         00000001400245C2
  0000000140020791: 33 C0              xor         eax,eax
  0000000140020793: 85 C0              test        eax,eax
  0000000140020795: 74 2B              je          00000001400207C2
  0000000140020797: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002079F: 48 83 C0 08        add         rax,8
  00000001400207A3: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400207AB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400207B3: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400207B7: 8B 00              mov         eax,dword ptr [rax]
  00000001400207B9: 89 84 24 7C 06 00  mov         dword ptr [rsp+000000000000067Ch],eax
                    00
  00000001400207C0: EB 26              jmp         00000001400207E8
  00000001400207C2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400207CA: 48 83 C0 08        add         rax,8
  00000001400207CE: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400207D6: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400207DE: 8B 40 F8           mov         eax,dword ptr [rax-8]
  00000001400207E1: 89 84 24 7C 06 00  mov         dword ptr [rsp+000000000000067Ch],eax
                    00
  00000001400207E8: 8B 84 24 7C 06 00  mov         eax,dword ptr [rsp+000000000000067Ch]
                    00
  00000001400207EF: 89 84 24 44 01 00  mov         dword ptr [rsp+0000000000000144h],eax
                    00
  00000001400207F6: 8B 94 24 44 01 00  mov         edx,dword ptr [rsp+0000000000000144h]
                    00
  00000001400207FD: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020805: E8 A6 E4 C7 00     call        0000000140C9ECB0
  000000014002080A: 33 C0              xor         eax,eax
  000000014002080C: E9 B1 3D 00 00     jmp         00000001400245C2
  0000000140020811: E9 AC 3D 00 00     jmp         00000001400245C2
  0000000140020816: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002081E: E8 2D 6E C7 00     call        0000000140C97650
  0000000140020823: 0F B6 C0           movzx       eax,al
  0000000140020826: E9 97 3D 00 00     jmp         00000001400245C2
  000000014002082B: E9 92 3D 00 00     jmp         00000001400245C2
  0000000140020830: 33 C0              xor         eax,eax
  0000000140020832: 85 C0              test        eax,eax
  0000000140020834: 74 2D              je          0000000140020863
  0000000140020836: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002083E: 48 83 C0 08        add         rax,8
  0000000140020842: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002084A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020852: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020856: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140020859: 48 89 84 24 80 06  mov         qword ptr [rsp+0000000000000680h],rax
                    00 00
  0000000140020861: EB 28              jmp         000000014002088B
  0000000140020863: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002086B: 48 83 C0 08        add         rax,8
  000000014002086F: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020877: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002087F: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020883: 48 89 84 24 80 06  mov         qword ptr [rsp+0000000000000680h],rax
                    00 00
  000000014002088B: 48 8B 84 24 80 06  mov         rax,qword ptr [rsp+0000000000000680h]
                    00 00
  0000000140020893: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  000000014002089B: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400208A3: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400208A6: 48 8B 94 24 48 01  mov         rdx,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001400208AE: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400208B6: FF 90 28 01 00 00  call        qword ptr [rax+0000000000000128h]
  00000001400208BC: 33 C0              xor         eax,eax
  00000001400208BE: E9 FF 3C 00 00     jmp         00000001400245C2
  00000001400208C3: E9 FA 3C 00 00     jmp         00000001400245C2
  00000001400208C8: 33 C0              xor         eax,eax
  00000001400208CA: 85 C0              test        eax,eax
  00000001400208CC: 74 2D              je          00000001400208FB
  00000001400208CE: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400208D6: 48 83 C0 08        add         rax,8
  00000001400208DA: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400208E2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400208EA: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400208EE: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400208F1: 48 89 84 24 88 06  mov         qword ptr [rsp+0000000000000688h],rax
                    00 00
  00000001400208F9: EB 28              jmp         0000000140020923
  00000001400208FB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020903: 48 83 C0 08        add         rax,8
  0000000140020907: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002090F: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020917: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002091B: 48 89 84 24 88 06  mov         qword ptr [rsp+0000000000000688h],rax
                    00 00
  0000000140020923: 48 8B 84 24 88 06  mov         rax,qword ptr [rsp+0000000000000688h]
                    00 00
  000000014002092B: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  0000000140020933: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002093B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002093E: 48 8B 94 24 50 01  mov         rdx,qword ptr [rsp+0000000000000150h]
                    00 00
  0000000140020946: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002094E: FF 90 C0 00 00 00  call        qword ptr [rax+00000000000000C0h]
  0000000140020954: 33 C0              xor         eax,eax
  0000000140020956: E9 67 3C 00 00     jmp         00000001400245C2
  000000014002095B: E9 62 3C 00 00     jmp         00000001400245C2
  0000000140020960: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020968: E8 03 17 C7 00     call        0000000140C92070
  000000014002096D: 33 C0              xor         eax,eax
  000000014002096F: E9 4E 3C 00 00     jmp         00000001400245C2
  0000000140020974: E9 49 3C 00 00     jmp         00000001400245C2
  0000000140020979: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020981: E8 4A 05 C8 00     call        0000000140CA0ED0
  0000000140020986: 33 C0              xor         eax,eax
  0000000140020988: E9 35 3C 00 00     jmp         00000001400245C2
  000000014002098D: E9 30 3C 00 00     jmp         00000001400245C2
  0000000140020992: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002099A: E8 E1 0B C8 00     call        0000000140CA1580
  000000014002099F: 33 C0              xor         eax,eax
  00000001400209A1: E9 1C 3C 00 00     jmp         00000001400245C2
  00000001400209A6: E9 17 3C 00 00     jmp         00000001400245C2
  00000001400209AB: 33 C0              xor         eax,eax
  00000001400209AD: 85 C0              test        eax,eax
  00000001400209AF: 74 2B              je          00000001400209DC
  00000001400209B1: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400209B9: 48 83 C0 08        add         rax,8
  00000001400209BD: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400209C5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400209CD: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400209D1: 8B 00              mov         eax,dword ptr [rax]
  00000001400209D3: 89 84 24 90 06 00  mov         dword ptr [rsp+0000000000000690h],eax
                    00
  00000001400209DA: EB 26              jmp         0000000140020A02
  00000001400209DC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400209E4: 48 83 C0 08        add         rax,8
  00000001400209E8: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400209F0: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400209F8: 8B 40 F8           mov         eax,dword ptr [rax-8]
  00000001400209FB: 89 84 24 90 06 00  mov         dword ptr [rsp+0000000000000690h],eax
                    00
  0000000140020A02: 8B 84 24 90 06 00  mov         eax,dword ptr [rsp+0000000000000690h]
                    00
  0000000140020A09: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  0000000140020A10: 8B 94 24 58 01 00  mov         edx,dword ptr [rsp+0000000000000158h]
                    00
  0000000140020A17: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020A1F: E8 3C 23 C8 00     call        0000000140CA2D60
  0000000140020A24: 33 C0              xor         eax,eax
  0000000140020A26: E9 97 3B 00 00     jmp         00000001400245C2
  0000000140020A2B: E9 92 3B 00 00     jmp         00000001400245C2
  0000000140020A30: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020A38: E8 A3 24 C8 00     call        0000000140CA2EE0
  0000000140020A3D: 33 C0              xor         eax,eax
  0000000140020A3F: E9 7E 3B 00 00     jmp         00000001400245C2
  0000000140020A44: E9 79 3B 00 00     jmp         00000001400245C2
  0000000140020A49: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020A51: E8 1A 03 C8 00     call        0000000140CA0D70
  0000000140020A56: 33 C0              xor         eax,eax
  0000000140020A58: E9 65 3B 00 00     jmp         00000001400245C2
  0000000140020A5D: E9 60 3B 00 00     jmp         00000001400245C2
  0000000140020A62: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020A6A: E8 A1 31 C8 00     call        0000000140CA3C10
  0000000140020A6F: 33 C0              xor         eax,eax
  0000000140020A71: E9 4C 3B 00 00     jmp         00000001400245C2
  0000000140020A76: E9 47 3B 00 00     jmp         00000001400245C2
  0000000140020A7B: 33 C0              xor         eax,eax
  0000000140020A7D: 85 C0              test        eax,eax
  0000000140020A7F: 74 2D              je          0000000140020AAE
  0000000140020A81: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020A89: 48 83 C0 08        add         rax,8
  0000000140020A8D: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020A95: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020A9D: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020AA1: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140020AA4: 48 89 84 24 98 06  mov         qword ptr [rsp+0000000000000698h],rax
                    00 00
  0000000140020AAC: EB 28              jmp         0000000140020AD6
  0000000140020AAE: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020AB6: 48 83 C0 08        add         rax,8
  0000000140020ABA: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020AC2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020ACA: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020ACE: 48 89 84 24 98 06  mov         qword ptr [rsp+0000000000000698h],rax
                    00 00
  0000000140020AD6: 48 8B 84 24 98 06  mov         rax,qword ptr [rsp+0000000000000698h]
                    00 00
  0000000140020ADE: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  0000000140020AE6: 48 8B 94 24 60 01  mov         rdx,qword ptr [rsp+0000000000000160h]
                    00 00
  0000000140020AEE: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020AF6: E8 35 36 C8 00     call        0000000140CA4130
  0000000140020AFB: 33 C0              xor         eax,eax
  0000000140020AFD: E9 C0 3A 00 00     jmp         00000001400245C2
  0000000140020B02: E9 BB 3A 00 00     jmp         00000001400245C2
  0000000140020B07: 33 C0              xor         eax,eax
  0000000140020B09: 85 C0              test        eax,eax
  0000000140020B0B: 74 2A              je          0000000140020B37
  0000000140020B0D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020B15: 48 83 C0 08        add         rax,8
  0000000140020B19: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020B21: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020B29: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020B2D: 48 89 84 24 A0 06  mov         qword ptr [rsp+00000000000006A0h],rax
                    00 00
  0000000140020B35: EB 28              jmp         0000000140020B5F
  0000000140020B37: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020B3F: 48 83 C0 08        add         rax,8
  0000000140020B43: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020B4B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020B53: 48 83 E8 08        sub         rax,8
  0000000140020B57: 48 89 84 24 A0 06  mov         qword ptr [rsp+00000000000006A0h],rax
                    00 00
  0000000140020B5F: 48 8B 84 24 A0 06  mov         rax,qword ptr [rsp+00000000000006A0h]
                    00 00
  0000000140020B67: 48 89 84 24 C8 03  mov         qword ptr [rsp+00000000000003C8h],rax
                    00 00
  0000000140020B6F: 48 8B 94 24 C8 03  mov         rdx,qword ptr [rsp+00000000000003C8h]
                    00 00
  0000000140020B77: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  0000000140020B7F: FF 15 53 4E DE 01  call        qword ptr [0000000141E059D8h]
  0000000140020B85: 90                 nop
  0000000140020B86: 48 8D 84 24 D0 03  lea         rax,[rsp+00000000000003D0h]
                    00 00
  0000000140020B8E: 48 89 84 24 D8 03  mov         qword ptr [rsp+00000000000003D8h],rax
                    00 00
  0000000140020B96: 48 8D 94 24 68 01  lea         rdx,[rsp+0000000000000168h]
                    00 00
  0000000140020B9E: 48 8B 8C 24 D8 03  mov         rcx,qword ptr [rsp+00000000000003D8h]
                    00 00
  0000000140020BA6: FF 15 2C 4E DE 01  call        qword ptr [0000000141E059D8h]
  0000000140020BAC: 48 89 84 24 A8 06  mov         qword ptr [rsp+00000000000006A8h],rax
                    00 00
  0000000140020BB4: 48 8B 94 24 A8 06  mov         rdx,qword ptr [rsp+00000000000006A8h]
                    00 00
  0000000140020BBC: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020BC4: E8 57 69 C7 00     call        0000000140C97520
  0000000140020BC9: C7 84 24 E0 03 00  mov         dword ptr [rsp+00000000000003E0h],0
                    00 00 00 00 00
  0000000140020BD4: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  0000000140020BDC: FF 15 FE 4D DE 01  call        qword ptr [0000000141E059E0h]
  0000000140020BE2: 8B 84 24 E0 03 00  mov         eax,dword ptr [rsp+00000000000003E0h]
                    00
  0000000140020BE9: E9 D4 39 00 00     jmp         00000001400245C2
  0000000140020BEE: E9 CF 39 00 00     jmp         00000001400245C2
  0000000140020BF3: 33 C0              xor         eax,eax
  0000000140020BF5: 85 C0              test        eax,eax
  0000000140020BF7: 74 2A              je          0000000140020C23
  0000000140020BF9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020C01: 48 83 C0 08        add         rax,8
  0000000140020C05: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020C0D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020C15: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020C19: 48 89 84 24 B0 06  mov         qword ptr [rsp+00000000000006B0h],rax
                    00 00
  0000000140020C21: EB 28              jmp         0000000140020C4B
  0000000140020C23: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020C2B: 48 83 C0 08        add         rax,8
  0000000140020C2F: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020C37: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020C3F: 48 83 E8 08        sub         rax,8
  0000000140020C43: 48 89 84 24 B0 06  mov         qword ptr [rsp+00000000000006B0h],rax
                    00 00
  0000000140020C4B: 48 8B 84 24 B0 06  mov         rax,qword ptr [rsp+00000000000006B0h]
                    00 00
  0000000140020C53: 48 89 84 24 E8 03  mov         qword ptr [rsp+00000000000003E8h],rax
                    00 00
  0000000140020C5B: 48 8B 94 24 E8 03  mov         rdx,qword ptr [rsp+00000000000003E8h]
                    00 00
  0000000140020C63: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  0000000140020C6B: FF 15 67 4D DE 01  call        qword ptr [0000000141E059D8h]
  0000000140020C71: 90                 nop
  0000000140020C72: 48 8D 84 24 F0 03  lea         rax,[rsp+00000000000003F0h]
                    00 00
  0000000140020C7A: 48 89 84 24 F8 03  mov         qword ptr [rsp+00000000000003F8h],rax
                    00 00
  0000000140020C82: 48 8D 94 24 70 01  lea         rdx,[rsp+0000000000000170h]
                    00 00
  0000000140020C8A: 48 8B 8C 24 F8 03  mov         rcx,qword ptr [rsp+00000000000003F8h]
                    00 00
  0000000140020C92: FF 15 40 4D DE 01  call        qword ptr [0000000141E059D8h]
  0000000140020C98: 48 89 84 24 B8 06  mov         qword ptr [rsp+00000000000006B8h],rax
                    00 00
  0000000140020CA0: 48 8B 94 24 B8 06  mov         rdx,qword ptr [rsp+00000000000006B8h]
                    00 00
  0000000140020CA8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020CB0: E8 CB 68 C7 00     call        0000000140C97580
  0000000140020CB5: C7 84 24 00 04 00  mov         dword ptr [rsp+0000000000000400h],0
                    00 00 00 00 00
  0000000140020CC0: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  0000000140020CC8: FF 15 12 4D DE 01  call        qword ptr [0000000141E059E0h]
  0000000140020CCE: 8B 84 24 00 04 00  mov         eax,dword ptr [rsp+0000000000000400h]
                    00
  0000000140020CD5: E9 E8 38 00 00     jmp         00000001400245C2
  0000000140020CDA: E9 E3 38 00 00     jmp         00000001400245C2
  0000000140020CDF: 33 C0              xor         eax,eax
  0000000140020CE1: 85 C0              test        eax,eax
  0000000140020CE3: 74 2A              je          0000000140020D0F
  0000000140020CE5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020CED: 48 83 C0 08        add         rax,8
  0000000140020CF1: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020CF9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020D01: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020D05: 48 89 84 24 C0 06  mov         qword ptr [rsp+00000000000006C0h],rax
                    00 00
  0000000140020D0D: EB 28              jmp         0000000140020D37
  0000000140020D0F: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020D17: 48 83 C0 08        add         rax,8
  0000000140020D1B: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020D23: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020D2B: 48 83 E8 08        sub         rax,8
  0000000140020D2F: 48 89 84 24 C0 06  mov         qword ptr [rsp+00000000000006C0h],rax
                    00 00
  0000000140020D37: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140020D3F: 48 89 84 24 08 04  mov         qword ptr [rsp+0000000000000408h],rax
                    00 00
  0000000140020D47: 48 8B 94 24 08 04  mov         rdx,qword ptr [rsp+0000000000000408h]
                    00 00
  0000000140020D4F: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  0000000140020D57: FF 15 7B 4C DE 01  call        qword ptr [0000000141E059D8h]
  0000000140020D5D: 90                 nop
  0000000140020D5E: 48 8D 84 24 18 04  lea         rax,[rsp+0000000000000418h]
                    00 00
  0000000140020D66: 48 89 84 24 20 04  mov         qword ptr [rsp+0000000000000420h],rax
                    00 00
  0000000140020D6E: 48 8D 94 24 78 01  lea         rdx,[rsp+0000000000000178h]
                    00 00
  0000000140020D76: 48 8B 8C 24 20 04  mov         rcx,qword ptr [rsp+0000000000000420h]
                    00 00
  0000000140020D7E: FF 15 54 4C DE 01  call        qword ptr [0000000141E059D8h]
  0000000140020D84: 48 89 84 24 C8 06  mov         qword ptr [rsp+00000000000006C8h],rax
                    00 00
  0000000140020D8C: 48 8B 94 24 C8 06  mov         rdx,qword ptr [rsp+00000000000006C8h]
                    00 00
  0000000140020D94: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020D9C: E8 3F 68 C7 00     call        0000000140C975E0
  0000000140020DA1: 0F B6 C0           movzx       eax,al
  0000000140020DA4: 89 84 24 10 04 00  mov         dword ptr [rsp+0000000000000410h],eax
                    00
  0000000140020DAB: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  0000000140020DB3: FF 15 27 4C DE 01  call        qword ptr [0000000141E059E0h]
  0000000140020DB9: 8B 84 24 10 04 00  mov         eax,dword ptr [rsp+0000000000000410h]
                    00
  0000000140020DC0: E9 FD 37 00 00     jmp         00000001400245C2
  0000000140020DC5: E9 F8 37 00 00     jmp         00000001400245C2
  0000000140020DCA: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020DD2: E8 79 AD C7 00     call        0000000140C9BB50
  0000000140020DD7: 33 C0              xor         eax,eax
  0000000140020DD9: E9 E4 37 00 00     jmp         00000001400245C2
  0000000140020DDE: E9 DF 37 00 00     jmp         00000001400245C2
  0000000140020DE3: 33 C0              xor         eax,eax
  0000000140020DE5: 85 C0              test        eax,eax
  0000000140020DE7: 74 2B              je          0000000140020E14
  0000000140020DE9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020DF1: 48 83 C0 08        add         rax,8
  0000000140020DF5: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020DFD: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020E05: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020E09: 8B 00              mov         eax,dword ptr [rax]
  0000000140020E0B: 89 84 24 D0 06 00  mov         dword ptr [rsp+00000000000006D0h],eax
                    00
  0000000140020E12: EB 26              jmp         0000000140020E3A
  0000000140020E14: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020E1C: 48 83 C0 08        add         rax,8
  0000000140020E20: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020E28: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020E30: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140020E33: 89 84 24 D0 06 00  mov         dword ptr [rsp+00000000000006D0h],eax
                    00
  0000000140020E3A: 8B 84 24 D0 06 00  mov         eax,dword ptr [rsp+00000000000006D0h]
                    00
  0000000140020E41: 89 84 24 80 01 00  mov         dword ptr [rsp+0000000000000180h],eax
                    00
  0000000140020E48: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020E50: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140020E53: 8B 94 24 80 01 00  mov         edx,dword ptr [rsp+0000000000000180h]
                    00
  0000000140020E5A: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020E62: FF 90 F0 02 00 00  call        qword ptr [rax+00000000000002F0h]
  0000000140020E68: 33 C0              xor         eax,eax
  0000000140020E6A: E9 53 37 00 00     jmp         00000001400245C2
  0000000140020E6F: E9 4E 37 00 00     jmp         00000001400245C2
  0000000140020E74: 33 C0              xor         eax,eax
  0000000140020E76: 85 C0              test        eax,eax
  0000000140020E78: 74 2B              je          0000000140020EA5
  0000000140020E7A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020E82: 48 83 C0 08        add         rax,8
  0000000140020E86: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020E8E: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020E96: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020E9A: 8B 00              mov         eax,dword ptr [rax]
  0000000140020E9C: 89 84 24 D4 06 00  mov         dword ptr [rsp+00000000000006D4h],eax
                    00
  0000000140020EA3: EB 26              jmp         0000000140020ECB
  0000000140020EA5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020EAD: 48 83 C0 08        add         rax,8
  0000000140020EB1: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020EB9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020EC1: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140020EC4: 89 84 24 D4 06 00  mov         dword ptr [rsp+00000000000006D4h],eax
                    00
  0000000140020ECB: 8B 84 24 D4 06 00  mov         eax,dword ptr [rsp+00000000000006D4h]
                    00
  0000000140020ED2: 89 84 24 84 01 00  mov         dword ptr [rsp+0000000000000184h],eax
                    00
  0000000140020ED9: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020EE1: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140020EE4: 8B 94 24 84 01 00  mov         edx,dword ptr [rsp+0000000000000184h]
                    00
  0000000140020EEB: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020EF3: FF 90 F8 02 00 00  call        qword ptr [rax+00000000000002F8h]
  0000000140020EF9: 33 C0              xor         eax,eax
  0000000140020EFB: E9 C2 36 00 00     jmp         00000001400245C2
  0000000140020F00: E9 BD 36 00 00     jmp         00000001400245C2
  0000000140020F05: 33 C0              xor         eax,eax
  0000000140020F07: 85 C0              test        eax,eax
  0000000140020F09: 74 2B              je          0000000140020F36
  0000000140020F0B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020F13: 48 83 C0 08        add         rax,8
  0000000140020F17: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020F1F: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020F27: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020F2B: 8B 00              mov         eax,dword ptr [rax]
  0000000140020F2D: 89 84 24 D8 06 00  mov         dword ptr [rsp+00000000000006D8h],eax
                    00
  0000000140020F34: EB 26              jmp         0000000140020F5C
  0000000140020F36: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020F3E: 48 83 C0 08        add         rax,8
  0000000140020F42: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020F4A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020F52: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140020F55: 89 84 24 D8 06 00  mov         dword ptr [rsp+00000000000006D8h],eax
                    00
  0000000140020F5C: 8B 84 24 D8 06 00  mov         eax,dword ptr [rsp+00000000000006D8h]
                    00
  0000000140020F63: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140020F6A: 8B 94 24 88 01 00  mov         edx,dword ptr [rsp+0000000000000188h]
                    00
  0000000140020F71: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020F79: E8 B2 66 CA 00     call        0000000140CC7630
  0000000140020F7E: 33 C0              xor         eax,eax
  0000000140020F80: E9 3D 36 00 00     jmp         00000001400245C2
  0000000140020F85: E9 38 36 00 00     jmp         00000001400245C2
  0000000140020F8A: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020F92: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140020F95: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020F9D: FF 90 00 03 00 00  call        qword ptr [rax+0000000000000300h]
  0000000140020FA3: 0F B6 C0           movzx       eax,al
  0000000140020FA6: E9 17 36 00 00     jmp         00000001400245C2
  0000000140020FAB: E9 12 36 00 00     jmp         00000001400245C2
  0000000140020FB0: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020FB8: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140020FBB: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140020FC3: FF 90 08 03 00 00  call        qword ptr [rax+0000000000000308h]
  0000000140020FC9: 0F B6 C0           movzx       eax,al
  0000000140020FCC: E9 F1 35 00 00     jmp         00000001400245C2
  0000000140020FD1: E9 EC 35 00 00     jmp         00000001400245C2
  0000000140020FD6: 33 C0              xor         eax,eax
  0000000140020FD8: 85 C0              test        eax,eax
  0000000140020FDA: 74 2D              je          0000000140021009
  0000000140020FDC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020FE4: 48 83 C0 08        add         rax,8
  0000000140020FE8: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140020FF0: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140020FF8: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140020FFC: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140020FFF: 48 89 84 24 E0 06  mov         qword ptr [rsp+00000000000006E0h],rax
                    00 00
  0000000140021007: EB 28              jmp         0000000140021031
  0000000140021009: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021011: 48 83 C0 08        add         rax,8
  0000000140021015: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002101D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021025: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140021029: 48 89 84 24 E0 06  mov         qword ptr [rsp+00000000000006E0h],rax
                    00 00
  0000000140021031: 48 8B 84 24 E0 06  mov         rax,qword ptr [rsp+00000000000006E0h]
                    00 00
  0000000140021039: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  0000000140021041: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021049: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002104C: 48 8B 94 24 90 01  mov         rdx,qword ptr [rsp+0000000000000190h]
                    00 00
  0000000140021054: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002105C: FF 90 28 03 00 00  call        qword ptr [rax+0000000000000328h]
  0000000140021062: 33 C0              xor         eax,eax
  0000000140021064: E9 59 35 00 00     jmp         00000001400245C2
  0000000140021069: E9 54 35 00 00     jmp         00000001400245C2
  000000014002106E: 33 C0              xor         eax,eax
  0000000140021070: 85 C0              test        eax,eax
  0000000140021072: 74 2D              je          00000001400210A1
  0000000140021074: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002107C: 48 83 C0 08        add         rax,8
  0000000140021080: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140021088: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021090: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140021094: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021097: 48 89 84 24 E8 06  mov         qword ptr [rsp+00000000000006E8h],rax
                    00 00
  000000014002109F: EB 28              jmp         00000001400210C9
  00000001400210A1: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400210A9: 48 83 C0 08        add         rax,8
  00000001400210AD: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400210B5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400210BD: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400210C1: 48 89 84 24 E8 06  mov         qword ptr [rsp+00000000000006E8h],rax
                    00 00
  00000001400210C9: 48 8B 84 24 E8 06  mov         rax,qword ptr [rsp+00000000000006E8h]
                    00 00
  00000001400210D1: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  00000001400210D9: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400210E1: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400210E4: 48 8B 94 24 98 01  mov         rdx,qword ptr [rsp+0000000000000198h]
                    00 00
  00000001400210EC: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400210F4: FF 90 30 03 00 00  call        qword ptr [rax+0000000000000330h]
  00000001400210FA: 33 C0              xor         eax,eax
  00000001400210FC: E9 C1 34 00 00     jmp         00000001400245C2
  0000000140021101: E9 BC 34 00 00     jmp         00000001400245C2
  0000000140021106: 33 C0              xor         eax,eax
  0000000140021108: 85 C0              test        eax,eax
  000000014002110A: 74 2D              je          0000000140021139
  000000014002110C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021114: 48 83 C0 08        add         rax,8
  0000000140021118: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140021120: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021128: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002112C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002112F: 48 89 84 24 F0 06  mov         qword ptr [rsp+00000000000006F0h],rax
                    00 00
  0000000140021137: EB 28              jmp         0000000140021161
  0000000140021139: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021141: 48 83 C0 08        add         rax,8
  0000000140021145: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002114D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021155: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140021159: 48 89 84 24 F0 06  mov         qword ptr [rsp+00000000000006F0h],rax
                    00 00
  0000000140021161: 48 8B 84 24 F0 06  mov         rax,qword ptr [rsp+00000000000006F0h]
                    00 00
  0000000140021169: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  0000000140021171: 33 C0              xor         eax,eax
  0000000140021173: 85 C0              test        eax,eax
  0000000140021175: 74 2C              je          00000001400211A3
  0000000140021177: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002117F: 48 83 C0 08        add         rax,8
  0000000140021183: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002118B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021193: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140021197: 0F B6 00           movzx       eax,byte ptr [rax]
  000000014002119A: 88 84 24 F8 06 00  mov         byte ptr [rsp+00000000000006F8h],al
                    00
  00000001400211A1: EB 27              jmp         00000001400211CA
  00000001400211A3: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400211AB: 48 83 C0 08        add         rax,8
  00000001400211AF: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400211B7: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400211BF: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  00000001400211C3: 88 84 24 F8 06 00  mov         byte ptr [rsp+00000000000006F8h],al
                    00
  00000001400211CA: 0F B6 84 24 F8 06  movzx       eax,byte ptr [rsp+00000000000006F8h]
                    00 00
  00000001400211D2: 88 84 24 A8 01 00  mov         byte ptr [rsp+00000000000001A8h],al
                    00
  00000001400211D9: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400211E1: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400211E4: 44 0F B6 84 24 A8  movzx       r8d,byte ptr [rsp+00000000000001A8h]
                    01 00 00
  00000001400211ED: 48 8B 94 24 A0 01  mov         rdx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001400211F5: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400211FD: FF 90 38 03 00 00  call        qword ptr [rax+0000000000000338h]
  0000000140021203: 33 C0              xor         eax,eax
  0000000140021205: E9 B8 33 00 00     jmp         00000001400245C2
  000000014002120A: E9 B3 33 00 00     jmp         00000001400245C2
  000000014002120F: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021217: E8 84 29 CA 00     call        0000000140CC3BA0
  000000014002121C: 33 C0              xor         eax,eax
  000000014002121E: E9 9F 33 00 00     jmp         00000001400245C2
  0000000140021223: E9 9A 33 00 00     jmp         00000001400245C2
  0000000140021228: 33 C0              xor         eax,eax
  000000014002122A: 85 C0              test        eax,eax
  000000014002122C: 74 2D              je          000000014002125B
  000000014002122E: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021236: 48 83 C0 08        add         rax,8
  000000014002123A: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140021242: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002124A: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002124E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021251: 48 89 84 24 00 07  mov         qword ptr [rsp+0000000000000700h],rax
                    00 00
  0000000140021259: EB 28              jmp         0000000140021283
  000000014002125B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021263: 48 83 C0 08        add         rax,8
  0000000140021267: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002126F: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021277: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002127B: 48 89 84 24 00 07  mov         qword ptr [rsp+0000000000000700h],rax
                    00 00
  0000000140021283: 48 8B 84 24 00 07  mov         rax,qword ptr [rsp+0000000000000700h]
                    00 00
  000000014002128B: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  0000000140021293: 48 8B 94 24 B0 01  mov         rdx,qword ptr [rsp+00000000000001B0h]
                    00 00
  000000014002129B: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400212A3: E8 38 2F CA 00     call        0000000140CC41E0
  00000001400212A8: 33 C0              xor         eax,eax
  00000001400212AA: E9 13 33 00 00     jmp         00000001400245C2
  00000001400212AF: E9 0E 33 00 00     jmp         00000001400245C2
  00000001400212B4: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400212BC: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400212BF: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400212C7: FF 90 60 03 00 00  call        qword ptr [rax+0000000000000360h]
  00000001400212CD: 33 C0              xor         eax,eax
  00000001400212CF: E9 EE 32 00 00     jmp         00000001400245C2
  00000001400212D4: E9 E9 32 00 00     jmp         00000001400245C2
  00000001400212D9: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400212E1: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400212E4: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400212EC: FF 90 68 03 00 00  call        qword ptr [rax+0000000000000368h]
  00000001400212F2: 33 C0              xor         eax,eax
  00000001400212F4: E9 C9 32 00 00     jmp         00000001400245C2
  00000001400212F9: E9 C4 32 00 00     jmp         00000001400245C2
  00000001400212FE: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021306: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021309: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021311: FF 90 70 03 00 00  call        qword ptr [rax+0000000000000370h]
  0000000140021317: 33 C0              xor         eax,eax
  0000000140021319: E9 A4 32 00 00     jmp         00000001400245C2
  000000014002131E: E9 9F 32 00 00     jmp         00000001400245C2
  0000000140021323: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002132B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002132E: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021336: FF 90 78 03 00 00  call        qword ptr [rax+0000000000000378h]
  000000014002133C: 33 C0              xor         eax,eax
  000000014002133E: E9 7F 32 00 00     jmp         00000001400245C2
  0000000140021343: E9 7A 32 00 00     jmp         00000001400245C2
  0000000140021348: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021350: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021353: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002135B: FF 90 80 03 00 00  call        qword ptr [rax+0000000000000380h]
  0000000140021361: 0F B6 C0           movzx       eax,al
  0000000140021364: E9 59 32 00 00     jmp         00000001400245C2
  0000000140021369: E9 54 32 00 00     jmp         00000001400245C2
  000000014002136E: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021376: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021379: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021381: FF 90 88 03 00 00  call        qword ptr [rax+0000000000000388h]
  0000000140021387: 33 C0              xor         eax,eax
  0000000140021389: E9 34 32 00 00     jmp         00000001400245C2
  000000014002138E: E9 2F 32 00 00     jmp         00000001400245C2
  0000000140021393: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002139B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002139E: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400213A6: FF 90 90 03 00 00  call        qword ptr [rax+0000000000000390h]
  00000001400213AC: 33 C0              xor         eax,eax
  00000001400213AE: E9 0F 32 00 00     jmp         00000001400245C2
  00000001400213B3: E9 0A 32 00 00     jmp         00000001400245C2
  00000001400213B8: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400213C0: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400213C3: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400213CB: FF 90 98 03 00 00  call        qword ptr [rax+0000000000000398h]
  00000001400213D1: 33 C0              xor         eax,eax
  00000001400213D3: E9 EA 31 00 00     jmp         00000001400245C2
  00000001400213D8: E9 E5 31 00 00     jmp         00000001400245C2
  00000001400213DD: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400213E5: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400213E8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400213F0: FF 90 A0 03 00 00  call        qword ptr [rax+00000000000003A0h]
  00000001400213F6: 33 C0              xor         eax,eax
  00000001400213F8: E9 C5 31 00 00     jmp         00000001400245C2
  00000001400213FD: E9 C0 31 00 00     jmp         00000001400245C2
  0000000140021402: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002140A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002140D: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021415: FF 90 A8 03 00 00  call        qword ptr [rax+00000000000003A8h]
  000000014002141B: 0F B6 C0           movzx       eax,al
  000000014002141E: E9 9F 31 00 00     jmp         00000001400245C2
  0000000140021423: E9 9A 31 00 00     jmp         00000001400245C2
  0000000140021428: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021430: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021433: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002143B: FF 90 B0 03 00 00  call        qword ptr [rax+00000000000003B0h]
  0000000140021441: 33 C0              xor         eax,eax
  0000000140021443: E9 7A 31 00 00     jmp         00000001400245C2
  0000000140021448: E9 75 31 00 00     jmp         00000001400245C2
  000000014002144D: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021455: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021458: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021460: FF 90 B8 03 00 00  call        qword ptr [rax+00000000000003B8h]
  0000000140021466: 33 C0              xor         eax,eax
  0000000140021468: E9 55 31 00 00     jmp         00000001400245C2
  000000014002146D: E9 50 31 00 00     jmp         00000001400245C2
  0000000140021472: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002147A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002147D: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021485: FF 90 C0 03 00 00  call        qword ptr [rax+00000000000003C0h]
  000000014002148B: 33 C0              xor         eax,eax
  000000014002148D: E9 30 31 00 00     jmp         00000001400245C2
  0000000140021492: E9 2B 31 00 00     jmp         00000001400245C2
  0000000140021497: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002149F: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400214A2: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400214AA: FF 90 C8 03 00 00  call        qword ptr [rax+00000000000003C8h]
  00000001400214B0: 33 C0              xor         eax,eax
  00000001400214B2: E9 0B 31 00 00     jmp         00000001400245C2
  00000001400214B7: E9 06 31 00 00     jmp         00000001400245C2
  00000001400214BC: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400214C4: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400214C7: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400214CF: FF 90 D0 03 00 00  call        qword ptr [rax+00000000000003D0h]
  00000001400214D5: 33 C0              xor         eax,eax
  00000001400214D7: E9 E6 30 00 00     jmp         00000001400245C2
  00000001400214DC: E9 E1 30 00 00     jmp         00000001400245C2
  00000001400214E1: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400214E9: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400214EC: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400214F4: FF 90 D8 03 00 00  call        qword ptr [rax+00000000000003D8h]
  00000001400214FA: 33 C0              xor         eax,eax
  00000001400214FC: E9 C1 30 00 00     jmp         00000001400245C2
  0000000140021501: E9 BC 30 00 00     jmp         00000001400245C2
  0000000140021506: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002150E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021511: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021519: FF 90 E0 03 00 00  call        qword ptr [rax+00000000000003E0h]
  000000014002151F: 33 C0              xor         eax,eax
  0000000140021521: E9 9C 30 00 00     jmp         00000001400245C2
  0000000140021526: E9 97 30 00 00     jmp         00000001400245C2
  000000014002152B: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021533: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021536: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002153E: FF 90 E8 03 00 00  call        qword ptr [rax+00000000000003E8h]
  0000000140021544: 33 C0              xor         eax,eax
  0000000140021546: E9 77 30 00 00     jmp         00000001400245C2
  000000014002154B: E9 72 30 00 00     jmp         00000001400245C2
  0000000140021550: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021558: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002155B: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021563: FF 90 F0 03 00 00  call        qword ptr [rax+00000000000003F0h]
  0000000140021569: 33 C0              xor         eax,eax
  000000014002156B: E9 52 30 00 00     jmp         00000001400245C2
  0000000140021570: E9 4D 30 00 00     jmp         00000001400245C2
  0000000140021575: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002157D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021580: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021588: FF 90 F8 03 00 00  call        qword ptr [rax+00000000000003F8h]
  000000014002158E: 33 C0              xor         eax,eax
  0000000140021590: E9 2D 30 00 00     jmp         00000001400245C2
  0000000140021595: E9 28 30 00 00     jmp         00000001400245C2
  000000014002159A: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400215A2: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400215A5: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400215AD: FF 90 00 04 00 00  call        qword ptr [rax+0000000000000400h]
  00000001400215B3: 33 C0              xor         eax,eax
  00000001400215B5: E9 08 30 00 00     jmp         00000001400245C2
  00000001400215BA: E9 03 30 00 00     jmp         00000001400245C2
  00000001400215BF: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400215C7: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400215CA: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400215D2: FF 90 08 04 00 00  call        qword ptr [rax+0000000000000408h]
  00000001400215D8: 33 C0              xor         eax,eax
  00000001400215DA: E9 E3 2F 00 00     jmp         00000001400245C2
  00000001400215DF: E9 DE 2F 00 00     jmp         00000001400245C2
  00000001400215E4: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400215EC: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400215EF: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400215F7: FF 90 10 04 00 00  call        qword ptr [rax+0000000000000410h]
  00000001400215FD: 33 C0              xor         eax,eax
  00000001400215FF: E9 BE 2F 00 00     jmp         00000001400245C2
  0000000140021604: E9 B9 2F 00 00     jmp         00000001400245C2
  0000000140021609: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021611: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021614: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002161C: FF 90 18 04 00 00  call        qword ptr [rax+0000000000000418h]
  0000000140021622: 33 C0              xor         eax,eax
  0000000140021624: E9 99 2F 00 00     jmp         00000001400245C2
  0000000140021629: E9 94 2F 00 00     jmp         00000001400245C2
  000000014002162E: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021636: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021639: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021641: FF 90 20 04 00 00  call        qword ptr [rax+0000000000000420h]
  0000000140021647: 33 C0              xor         eax,eax
  0000000140021649: E9 74 2F 00 00     jmp         00000001400245C2
  000000014002164E: E9 6F 2F 00 00     jmp         00000001400245C2
  0000000140021653: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002165B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002165E: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021666: FF 90 28 04 00 00  call        qword ptr [rax+0000000000000428h]
  000000014002166C: 33 C0              xor         eax,eax
  000000014002166E: E9 4F 2F 00 00     jmp         00000001400245C2
  0000000140021673: E9 4A 2F 00 00     jmp         00000001400245C2
  0000000140021678: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021680: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021683: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002168B: FF 90 30 04 00 00  call        qword ptr [rax+0000000000000430h]
  0000000140021691: 33 C0              xor         eax,eax
  0000000140021693: E9 2A 2F 00 00     jmp         00000001400245C2
  0000000140021698: E9 25 2F 00 00     jmp         00000001400245C2
  000000014002169D: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400216A5: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400216A8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400216B0: FF 90 38 04 00 00  call        qword ptr [rax+0000000000000438h]
  00000001400216B6: 33 C0              xor         eax,eax
  00000001400216B8: E9 05 2F 00 00     jmp         00000001400245C2
  00000001400216BD: E9 00 2F 00 00     jmp         00000001400245C2
  00000001400216C2: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400216CA: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400216CD: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400216D5: FF 90 40 04 00 00  call        qword ptr [rax+0000000000000440h]
  00000001400216DB: 33 C0              xor         eax,eax
  00000001400216DD: E9 E0 2E 00 00     jmp         00000001400245C2
  00000001400216E2: E9 DB 2E 00 00     jmp         00000001400245C2
  00000001400216E7: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400216EF: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400216F2: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400216FA: FF 90 48 04 00 00  call        qword ptr [rax+0000000000000448h]
  0000000140021700: 33 C0              xor         eax,eax
  0000000140021702: E9 BB 2E 00 00     jmp         00000001400245C2
  0000000140021707: E9 B6 2E 00 00     jmp         00000001400245C2
  000000014002170C: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021714: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021717: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002171F: FF 90 50 04 00 00  call        qword ptr [rax+0000000000000450h]
  0000000140021725: 33 C0              xor         eax,eax
  0000000140021727: E9 96 2E 00 00     jmp         00000001400245C2
  000000014002172C: E9 91 2E 00 00     jmp         00000001400245C2
  0000000140021731: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021739: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002173C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021744: FF 90 58 04 00 00  call        qword ptr [rax+0000000000000458h]
  000000014002174A: 33 C0              xor         eax,eax
  000000014002174C: E9 71 2E 00 00     jmp         00000001400245C2
  0000000140021751: E9 6C 2E 00 00     jmp         00000001400245C2
  0000000140021756: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002175E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021761: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021769: FF 90 60 04 00 00  call        qword ptr [rax+0000000000000460h]
  000000014002176F: 33 C0              xor         eax,eax
  0000000140021771: E9 4C 2E 00 00     jmp         00000001400245C2
  0000000140021776: E9 47 2E 00 00     jmp         00000001400245C2
  000000014002177B: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021783: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021786: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002178E: FF 90 68 04 00 00  call        qword ptr [rax+0000000000000468h]
  0000000140021794: 33 C0              xor         eax,eax
  0000000140021796: E9 27 2E 00 00     jmp         00000001400245C2
  000000014002179B: E9 22 2E 00 00     jmp         00000001400245C2
  00000001400217A0: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400217A8: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400217AB: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400217B3: FF 90 70 04 00 00  call        qword ptr [rax+0000000000000470h]
  00000001400217B9: 33 C0              xor         eax,eax
  00000001400217BB: E9 02 2E 00 00     jmp         00000001400245C2
  00000001400217C0: E9 FD 2D 00 00     jmp         00000001400245C2
  00000001400217C5: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400217CD: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400217D0: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400217D8: FF 90 78 04 00 00  call        qword ptr [rax+0000000000000478h]
  00000001400217DE: 33 C0              xor         eax,eax
  00000001400217E0: E9 DD 2D 00 00     jmp         00000001400245C2
  00000001400217E5: E9 D8 2D 00 00     jmp         00000001400245C2
  00000001400217EA: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400217F2: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400217F5: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400217FD: FF 90 80 04 00 00  call        qword ptr [rax+0000000000000480h]
  0000000140021803: 33 C0              xor         eax,eax
  0000000140021805: E9 B8 2D 00 00     jmp         00000001400245C2
  000000014002180A: E9 B3 2D 00 00     jmp         00000001400245C2
  000000014002180F: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021817: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002181A: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021822: FF 90 88 04 00 00  call        qword ptr [rax+0000000000000488h]
  0000000140021828: 33 C0              xor         eax,eax
  000000014002182A: E9 93 2D 00 00     jmp         00000001400245C2
  000000014002182F: E9 8E 2D 00 00     jmp         00000001400245C2
  0000000140021834: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002183C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002183F: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021847: FF 90 90 04 00 00  call        qword ptr [rax+0000000000000490h]
  000000014002184D: 33 C0              xor         eax,eax
  000000014002184F: E9 6E 2D 00 00     jmp         00000001400245C2
  0000000140021854: E9 69 2D 00 00     jmp         00000001400245C2
  0000000140021859: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021861: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021864: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002186C: FF 90 98 04 00 00  call        qword ptr [rax+0000000000000498h]
  0000000140021872: 33 C0              xor         eax,eax
  0000000140021874: E9 49 2D 00 00     jmp         00000001400245C2
  0000000140021879: E9 44 2D 00 00     jmp         00000001400245C2
  000000014002187E: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021886: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021889: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021891: FF 90 A0 04 00 00  call        qword ptr [rax+00000000000004A0h]
  0000000140021897: 33 C0              xor         eax,eax
  0000000140021899: E9 24 2D 00 00     jmp         00000001400245C2
  000000014002189E: E9 1F 2D 00 00     jmp         00000001400245C2
  00000001400218A3: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400218AB: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400218AE: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400218B6: FF 90 A8 04 00 00  call        qword ptr [rax+00000000000004A8h]
  00000001400218BC: 33 C0              xor         eax,eax
  00000001400218BE: E9 FF 2C 00 00     jmp         00000001400245C2
  00000001400218C3: E9 FA 2C 00 00     jmp         00000001400245C2
  00000001400218C8: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400218D0: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400218D3: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400218DB: FF 90 B0 04 00 00  call        qword ptr [rax+00000000000004B0h]
  00000001400218E1: 33 C0              xor         eax,eax
  00000001400218E3: E9 DA 2C 00 00     jmp         00000001400245C2
  00000001400218E8: E9 D5 2C 00 00     jmp         00000001400245C2
  00000001400218ED: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400218F5: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400218F8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021900: FF 90 B8 04 00 00  call        qword ptr [rax+00000000000004B8h]
  0000000140021906: 33 C0              xor         eax,eax
  0000000140021908: E9 B5 2C 00 00     jmp         00000001400245C2
  000000014002190D: E9 B0 2C 00 00     jmp         00000001400245C2
  0000000140021912: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002191A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002191D: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021925: FF 90 C0 04 00 00  call        qword ptr [rax+00000000000004C0h]
  000000014002192B: 33 C0              xor         eax,eax
  000000014002192D: E9 90 2C 00 00     jmp         00000001400245C2
  0000000140021932: E9 8B 2C 00 00     jmp         00000001400245C2
  0000000140021937: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002193F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021942: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002194A: FF 90 C8 04 00 00  call        qword ptr [rax+00000000000004C8h]
  0000000140021950: 33 C0              xor         eax,eax
  0000000140021952: E9 6B 2C 00 00     jmp         00000001400245C2
  0000000140021957: E9 66 2C 00 00     jmp         00000001400245C2
  000000014002195C: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021964: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021967: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002196F: FF 90 D0 04 00 00  call        qword ptr [rax+00000000000004D0h]
  0000000140021975: 33 C0              xor         eax,eax
  0000000140021977: E9 46 2C 00 00     jmp         00000001400245C2
  000000014002197C: E9 41 2C 00 00     jmp         00000001400245C2
  0000000140021981: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021989: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002198C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021994: FF 90 D8 04 00 00  call        qword ptr [rax+00000000000004D8h]
  000000014002199A: 33 C0              xor         eax,eax
  000000014002199C: E9 21 2C 00 00     jmp         00000001400245C2
  00000001400219A1: E9 1C 2C 00 00     jmp         00000001400245C2
  00000001400219A6: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400219AE: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400219B1: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400219B9: FF 90 E0 04 00 00  call        qword ptr [rax+00000000000004E0h]
  00000001400219BF: 33 C0              xor         eax,eax
  00000001400219C1: E9 FC 2B 00 00     jmp         00000001400245C2
  00000001400219C6: E9 F7 2B 00 00     jmp         00000001400245C2
  00000001400219CB: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400219D3: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400219D6: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400219DE: FF 90 E8 04 00 00  call        qword ptr [rax+00000000000004E8h]
  00000001400219E4: 33 C0              xor         eax,eax
  00000001400219E6: E9 D7 2B 00 00     jmp         00000001400245C2
  00000001400219EB: E9 D2 2B 00 00     jmp         00000001400245C2
  00000001400219F0: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400219F8: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400219FB: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021A03: FF 90 F0 04 00 00  call        qword ptr [rax+00000000000004F0h]
  0000000140021A09: 33 C0              xor         eax,eax
  0000000140021A0B: E9 B2 2B 00 00     jmp         00000001400245C2
  0000000140021A10: E9 AD 2B 00 00     jmp         00000001400245C2
  0000000140021A15: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021A1D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021A20: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021A28: FF 90 F8 04 00 00  call        qword ptr [rax+00000000000004F8h]
  0000000140021A2E: 33 C0              xor         eax,eax
  0000000140021A30: E9 8D 2B 00 00     jmp         00000001400245C2
  0000000140021A35: E9 88 2B 00 00     jmp         00000001400245C2
  0000000140021A3A: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021A42: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021A45: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021A4D: FF 90 00 05 00 00  call        qword ptr [rax+0000000000000500h]
  0000000140021A53: 33 C0              xor         eax,eax
  0000000140021A55: E9 68 2B 00 00     jmp         00000001400245C2
  0000000140021A5A: E9 63 2B 00 00     jmp         00000001400245C2
  0000000140021A5F: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021A67: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021A6A: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021A72: FF 90 08 05 00 00  call        qword ptr [rax+0000000000000508h]
  0000000140021A78: 33 C0              xor         eax,eax
  0000000140021A7A: E9 43 2B 00 00     jmp         00000001400245C2
  0000000140021A7F: E9 3E 2B 00 00     jmp         00000001400245C2
  0000000140021A84: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021A8C: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021A8F: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021A97: FF 90 10 05 00 00  call        qword ptr [rax+0000000000000510h]
  0000000140021A9D: 33 C0              xor         eax,eax
  0000000140021A9F: E9 1E 2B 00 00     jmp         00000001400245C2
  0000000140021AA4: E9 19 2B 00 00     jmp         00000001400245C2
  0000000140021AA9: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021AB1: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021AB4: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021ABC: FF 90 18 05 00 00  call        qword ptr [rax+0000000000000518h]
  0000000140021AC2: 33 C0              xor         eax,eax
  0000000140021AC4: E9 F9 2A 00 00     jmp         00000001400245C2
  0000000140021AC9: E9 F4 2A 00 00     jmp         00000001400245C2
  0000000140021ACE: 33 C0              xor         eax,eax
  0000000140021AD0: 85 C0              test        eax,eax
  0000000140021AD2: 74 2B              je          0000000140021AFF
  0000000140021AD4: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021ADC: 48 83 C0 08        add         rax,8
  0000000140021AE0: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140021AE8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021AF0: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140021AF4: 8B 00              mov         eax,dword ptr [rax]
  0000000140021AF6: 89 84 24 08 07 00  mov         dword ptr [rsp+0000000000000708h],eax
                    00
  0000000140021AFD: EB 26              jmp         0000000140021B25
  0000000140021AFF: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021B07: 48 83 C0 08        add         rax,8
  0000000140021B0B: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140021B13: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021B1B: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140021B1E: 89 84 24 08 07 00  mov         dword ptr [rsp+0000000000000708h],eax
                    00
  0000000140021B25: 8B 84 24 08 07 00  mov         eax,dword ptr [rsp+0000000000000708h]
                    00
  0000000140021B2C: 89 84 24 B8 01 00  mov         dword ptr [rsp+00000000000001B8h],eax
                    00
  0000000140021B33: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021B3B: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021B3E: 8B 94 24 B8 01 00  mov         edx,dword ptr [rsp+00000000000001B8h]
                    00
  0000000140021B45: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021B4D: FF 90 20 05 00 00  call        qword ptr [rax+0000000000000520h]
  0000000140021B53: 33 C0              xor         eax,eax
  0000000140021B55: E9 68 2A 00 00     jmp         00000001400245C2
  0000000140021B5A: E9 63 2A 00 00     jmp         00000001400245C2
  0000000140021B5F: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021B67: E8 A4 35 C7 00     call        0000000140C95110
  0000000140021B6C: 33 C0              xor         eax,eax
  0000000140021B6E: E9 4F 2A 00 00     jmp         00000001400245C2
  0000000140021B73: E9 4A 2A 00 00     jmp         00000001400245C2
  0000000140021B78: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021B80: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021B83: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021B8B: FF 90 28 05 00 00  call        qword ptr [rax+0000000000000528h]
  0000000140021B91: 33 C0              xor         eax,eax
  0000000140021B93: E9 2A 2A 00 00     jmp         00000001400245C2
  0000000140021B98: E9 25 2A 00 00     jmp         00000001400245C2
  0000000140021B9D: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021BA5: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021BA8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021BB0: FF 90 30 05 00 00  call        qword ptr [rax+0000000000000530h]
  0000000140021BB6: 33 C0              xor         eax,eax
  0000000140021BB8: E9 05 2A 00 00     jmp         00000001400245C2
  0000000140021BBD: E9 00 2A 00 00     jmp         00000001400245C2
  0000000140021BC2: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021BCA: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021BCD: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021BD5: FF 90 38 05 00 00  call        qword ptr [rax+0000000000000538h]
  0000000140021BDB: 33 C0              xor         eax,eax
  0000000140021BDD: E9 E0 29 00 00     jmp         00000001400245C2
  0000000140021BE2: E9 DB 29 00 00     jmp         00000001400245C2
  0000000140021BE7: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021BEF: E8 6C 37 C7 00     call        0000000140C95360
  0000000140021BF4: 33 C0              xor         eax,eax
  0000000140021BF6: E9 C7 29 00 00     jmp         00000001400245C2
  0000000140021BFB: E9 C2 29 00 00     jmp         00000001400245C2
  0000000140021C00: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021C08: E8 53 38 C7 00     call        0000000140C95460
  0000000140021C0D: 33 C0              xor         eax,eax
  0000000140021C0F: E9 AE 29 00 00     jmp         00000001400245C2
  0000000140021C14: E9 A9 29 00 00     jmp         00000001400245C2
  0000000140021C19: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021C21: E8 8A 3A C7 00     call        0000000140C956B0
  0000000140021C26: 33 C0              xor         eax,eax
  0000000140021C28: E9 95 29 00 00     jmp         00000001400245C2
  0000000140021C2D: E9 90 29 00 00     jmp         00000001400245C2
  0000000140021C32: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021C3A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021C3D: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021C45: FF 90 40 05 00 00  call        qword ptr [rax+0000000000000540h]
  0000000140021C4B: 33 C0              xor         eax,eax
  0000000140021C4D: E9 70 29 00 00     jmp         00000001400245C2
  0000000140021C52: E9 6B 29 00 00     jmp         00000001400245C2
  0000000140021C57: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021C5F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021C62: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021C6A: FF 90 48 05 00 00  call        qword ptr [rax+0000000000000548h]
  0000000140021C70: 33 C0              xor         eax,eax
  0000000140021C72: E9 4B 29 00 00     jmp         00000001400245C2
  0000000140021C77: E9 46 29 00 00     jmp         00000001400245C2
  0000000140021C7C: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021C84: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021C87: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021C8F: FF 90 50 05 00 00  call        qword ptr [rax+0000000000000550h]
  0000000140021C95: 33 C0              xor         eax,eax
  0000000140021C97: E9 26 29 00 00     jmp         00000001400245C2
  0000000140021C9C: E9 21 29 00 00     jmp         00000001400245C2
  0000000140021CA1: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021CA9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021CAC: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021CB4: FF 90 58 05 00 00  call        qword ptr [rax+0000000000000558h]
  0000000140021CBA: 33 C0              xor         eax,eax
  0000000140021CBC: E9 01 29 00 00     jmp         00000001400245C2
  0000000140021CC1: E9 FC 28 00 00     jmp         00000001400245C2
  0000000140021CC6: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021CCE: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021CD1: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021CD9: FF 90 60 05 00 00  call        qword ptr [rax+0000000000000560h]
  0000000140021CDF: 33 C0              xor         eax,eax
  0000000140021CE1: E9 DC 28 00 00     jmp         00000001400245C2
  0000000140021CE6: E9 D7 28 00 00     jmp         00000001400245C2
  0000000140021CEB: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021CF3: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021CF6: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021CFE: FF 90 68 05 00 00  call        qword ptr [rax+0000000000000568h]
  0000000140021D04: 33 C0              xor         eax,eax
  0000000140021D06: E9 B7 28 00 00     jmp         00000001400245C2
  0000000140021D0B: E9 B2 28 00 00     jmp         00000001400245C2
  0000000140021D10: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021D18: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021D1B: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021D23: FF 90 70 05 00 00  call        qword ptr [rax+0000000000000570h]
  0000000140021D29: 33 C0              xor         eax,eax
  0000000140021D2B: E9 92 28 00 00     jmp         00000001400245C2
  0000000140021D30: E9 8D 28 00 00     jmp         00000001400245C2
  0000000140021D35: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021D3D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021D40: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021D48: FF 90 78 05 00 00  call        qword ptr [rax+0000000000000578h]
  0000000140021D4E: 33 C0              xor         eax,eax
  0000000140021D50: E9 6D 28 00 00     jmp         00000001400245C2
  0000000140021D55: E9 68 28 00 00     jmp         00000001400245C2
  0000000140021D5A: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021D62: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021D65: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021D6D: FF 90 80 05 00 00  call        qword ptr [rax+0000000000000580h]
  0000000140021D73: 33 C0              xor         eax,eax
  0000000140021D75: E9 48 28 00 00     jmp         00000001400245C2
  0000000140021D7A: E9 43 28 00 00     jmp         00000001400245C2
  0000000140021D7F: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021D87: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021D8A: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021D92: FF 90 88 05 00 00  call        qword ptr [rax+0000000000000588h]
  0000000140021D98: 33 C0              xor         eax,eax
  0000000140021D9A: E9 23 28 00 00     jmp         00000001400245C2
  0000000140021D9F: E9 1E 28 00 00     jmp         00000001400245C2
  0000000140021DA4: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021DAC: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021DAF: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021DB7: FF 90 90 05 00 00  call        qword ptr [rax+0000000000000590h]
  0000000140021DBD: 33 C0              xor         eax,eax
  0000000140021DBF: E9 FE 27 00 00     jmp         00000001400245C2
  0000000140021DC4: E9 F9 27 00 00     jmp         00000001400245C2
  0000000140021DC9: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021DD1: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021DD4: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021DDC: FF 90 98 05 00 00  call        qword ptr [rax+0000000000000598h]
  0000000140021DE2: 33 C0              xor         eax,eax
  0000000140021DE4: E9 D9 27 00 00     jmp         00000001400245C2
  0000000140021DE9: E9 D4 27 00 00     jmp         00000001400245C2
  0000000140021DEE: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021DF6: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021DF9: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021E01: FF 90 A0 05 00 00  call        qword ptr [rax+00000000000005A0h]
  0000000140021E07: 33 C0              xor         eax,eax
  0000000140021E09: E9 B4 27 00 00     jmp         00000001400245C2
  0000000140021E0E: E9 AF 27 00 00     jmp         00000001400245C2
  0000000140021E13: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021E1B: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021E1E: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021E26: FF 90 A8 05 00 00  call        qword ptr [rax+00000000000005A8h]
  0000000140021E2C: 33 C0              xor         eax,eax
  0000000140021E2E: E9 8F 27 00 00     jmp         00000001400245C2
  0000000140021E33: E9 8A 27 00 00     jmp         00000001400245C2
  0000000140021E38: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021E40: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021E43: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021E4B: FF 90 B0 05 00 00  call        qword ptr [rax+00000000000005B0h]
  0000000140021E51: 33 C0              xor         eax,eax
  0000000140021E53: E9 6A 27 00 00     jmp         00000001400245C2
  0000000140021E58: E9 65 27 00 00     jmp         00000001400245C2
  0000000140021E5D: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021E65: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021E68: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021E70: FF 90 B8 05 00 00  call        qword ptr [rax+00000000000005B8h]
  0000000140021E76: 33 C0              xor         eax,eax
  0000000140021E78: E9 45 27 00 00     jmp         00000001400245C2
  0000000140021E7D: E9 40 27 00 00     jmp         00000001400245C2
  0000000140021E82: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021E8A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021E8D: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021E95: FF 90 C0 05 00 00  call        qword ptr [rax+00000000000005C0h]
  0000000140021E9B: 33 C0              xor         eax,eax
  0000000140021E9D: E9 20 27 00 00     jmp         00000001400245C2
  0000000140021EA2: E9 1B 27 00 00     jmp         00000001400245C2
  0000000140021EA7: 33 C0              xor         eax,eax
  0000000140021EA9: 85 C0              test        eax,eax
  0000000140021EAB: 74 2B              je          0000000140021ED8
  0000000140021EAD: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021EB5: 48 83 C0 08        add         rax,8
  0000000140021EB9: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140021EC1: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021EC9: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140021ECD: 8B 00              mov         eax,dword ptr [rax]
  0000000140021ECF: 89 84 24 0C 07 00  mov         dword ptr [rsp+000000000000070Ch],eax
                    00
  0000000140021ED6: EB 26              jmp         0000000140021EFE
  0000000140021ED8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021EE0: 48 83 C0 08        add         rax,8
  0000000140021EE4: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140021EEC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140021EF4: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140021EF7: 89 84 24 0C 07 00  mov         dword ptr [rsp+000000000000070Ch],eax
                    00
  0000000140021EFE: 8B 84 24 0C 07 00  mov         eax,dword ptr [rsp+000000000000070Ch]
                    00
  0000000140021F05: 89 84 24 BC 01 00  mov         dword ptr [rsp+00000000000001BCh],eax
                    00
  0000000140021F0C: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021F14: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021F17: 8B 94 24 BC 01 00  mov         edx,dword ptr [rsp+00000000000001BCh]
                    00
  0000000140021F1E: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021F26: FF 90 C8 05 00 00  call        qword ptr [rax+00000000000005C8h]
  0000000140021F2C: 33 C0              xor         eax,eax
  0000000140021F2E: E9 8F 26 00 00     jmp         00000001400245C2
  0000000140021F33: E9 8A 26 00 00     jmp         00000001400245C2
  0000000140021F38: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021F40: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021F43: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021F4B: FF 90 D0 05 00 00  call        qword ptr [rax+00000000000005D0h]
  0000000140021F51: 33 C0              xor         eax,eax
  0000000140021F53: E9 6A 26 00 00     jmp         00000001400245C2
  0000000140021F58: E9 65 26 00 00     jmp         00000001400245C2
  0000000140021F5D: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021F65: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021F68: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021F70: FF 90 D8 05 00 00  call        qword ptr [rax+00000000000005D8h]
  0000000140021F76: 33 C0              xor         eax,eax
  0000000140021F78: E9 45 26 00 00     jmp         00000001400245C2
  0000000140021F7D: E9 40 26 00 00     jmp         00000001400245C2
  0000000140021F82: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021F8A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021F8D: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021F95: FF 90 E0 05 00 00  call        qword ptr [rax+00000000000005E0h]
  0000000140021F9B: 33 C0              xor         eax,eax
  0000000140021F9D: E9 20 26 00 00     jmp         00000001400245C2
  0000000140021FA2: E9 1B 26 00 00     jmp         00000001400245C2
  0000000140021FA7: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021FAF: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021FB2: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021FBA: FF 90 E8 05 00 00  call        qword ptr [rax+00000000000005E8h]
  0000000140021FC0: 33 C0              xor         eax,eax
  0000000140021FC2: E9 FB 25 00 00     jmp         00000001400245C2
  0000000140021FC7: E9 F6 25 00 00     jmp         00000001400245C2
  0000000140021FCC: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021FD4: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021FD7: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021FDF: FF 90 F0 05 00 00  call        qword ptr [rax+00000000000005F0h]
  0000000140021FE5: 33 C0              xor         eax,eax
  0000000140021FE7: E9 D6 25 00 00     jmp         00000001400245C2
  0000000140021FEC: E9 D1 25 00 00     jmp         00000001400245C2
  0000000140021FF1: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140021FF9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140021FFC: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022004: FF 90 F8 05 00 00  call        qword ptr [rax+00000000000005F8h]
  000000014002200A: 33 C0              xor         eax,eax
  000000014002200C: E9 B1 25 00 00     jmp         00000001400245C2
  0000000140022011: E9 AC 25 00 00     jmp         00000001400245C2
  0000000140022016: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002201E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022021: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022029: FF 90 00 06 00 00  call        qword ptr [rax+0000000000000600h]
  000000014002202F: 33 C0              xor         eax,eax
  0000000140022031: E9 8C 25 00 00     jmp         00000001400245C2
  0000000140022036: E9 87 25 00 00     jmp         00000001400245C2
  000000014002203B: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022043: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022046: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002204E: FF 90 08 06 00 00  call        qword ptr [rax+0000000000000608h]
  0000000140022054: 33 C0              xor         eax,eax
  0000000140022056: E9 67 25 00 00     jmp         00000001400245C2
  000000014002205B: E9 62 25 00 00     jmp         00000001400245C2
  0000000140022060: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022068: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002206B: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022073: FF 90 10 06 00 00  call        qword ptr [rax+0000000000000610h]
  0000000140022079: 33 C0              xor         eax,eax
  000000014002207B: E9 42 25 00 00     jmp         00000001400245C2
  0000000140022080: E9 3D 25 00 00     jmp         00000001400245C2
  0000000140022085: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002208D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022090: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022098: FF 90 18 06 00 00  call        qword ptr [rax+0000000000000618h]
  000000014002209E: 33 C0              xor         eax,eax
  00000001400220A0: E9 1D 25 00 00     jmp         00000001400245C2
  00000001400220A5: E9 18 25 00 00     jmp         00000001400245C2
  00000001400220AA: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400220B2: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400220B5: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400220BD: FF 90 20 06 00 00  call        qword ptr [rax+0000000000000620h]
  00000001400220C3: 33 C0              xor         eax,eax
  00000001400220C5: E9 F8 24 00 00     jmp         00000001400245C2
  00000001400220CA: E9 F3 24 00 00     jmp         00000001400245C2
  00000001400220CF: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400220D7: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400220DA: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400220E2: FF 90 28 06 00 00  call        qword ptr [rax+0000000000000628h]
  00000001400220E8: 33 C0              xor         eax,eax
  00000001400220EA: E9 D3 24 00 00     jmp         00000001400245C2
  00000001400220EF: E9 CE 24 00 00     jmp         00000001400245C2
  00000001400220F4: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400220FC: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400220FF: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022107: FF 90 30 06 00 00  call        qword ptr [rax+0000000000000630h]
  000000014002210D: 33 C0              xor         eax,eax
  000000014002210F: E9 AE 24 00 00     jmp         00000001400245C2
  0000000140022114: E9 A9 24 00 00     jmp         00000001400245C2
  0000000140022119: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022121: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022124: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002212C: FF 90 38 06 00 00  call        qword ptr [rax+0000000000000638h]
  0000000140022132: 33 C0              xor         eax,eax
  0000000140022134: E9 89 24 00 00     jmp         00000001400245C2
  0000000140022139: E9 84 24 00 00     jmp         00000001400245C2
  000000014002213E: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022146: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022149: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022151: FF 90 40 06 00 00  call        qword ptr [rax+0000000000000640h]
  0000000140022157: 33 C0              xor         eax,eax
  0000000140022159: E9 64 24 00 00     jmp         00000001400245C2
  000000014002215E: E9 5F 24 00 00     jmp         00000001400245C2
  0000000140022163: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002216B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002216E: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022176: FF 90 48 06 00 00  call        qword ptr [rax+0000000000000648h]
  000000014002217C: 33 C0              xor         eax,eax
  000000014002217E: E9 3F 24 00 00     jmp         00000001400245C2
  0000000140022183: E9 3A 24 00 00     jmp         00000001400245C2
  0000000140022188: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022190: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022193: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002219B: FF 90 50 06 00 00  call        qword ptr [rax+0000000000000650h]
  00000001400221A1: 33 C0              xor         eax,eax
  00000001400221A3: E9 1A 24 00 00     jmp         00000001400245C2
  00000001400221A8: E9 15 24 00 00     jmp         00000001400245C2
  00000001400221AD: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400221B5: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400221B8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400221C0: FF 90 58 06 00 00  call        qword ptr [rax+0000000000000658h]
  00000001400221C6: 33 C0              xor         eax,eax
  00000001400221C8: E9 F5 23 00 00     jmp         00000001400245C2
  00000001400221CD: E9 F0 23 00 00     jmp         00000001400245C2
  00000001400221D2: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400221DA: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400221DD: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400221E5: FF 90 60 06 00 00  call        qword ptr [rax+0000000000000660h]
  00000001400221EB: 33 C0              xor         eax,eax
  00000001400221ED: E9 D0 23 00 00     jmp         00000001400245C2
  00000001400221F2: E9 CB 23 00 00     jmp         00000001400245C2
  00000001400221F7: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400221FF: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022202: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002220A: FF 90 68 06 00 00  call        qword ptr [rax+0000000000000668h]
  0000000140022210: 33 C0              xor         eax,eax
  0000000140022212: E9 AB 23 00 00     jmp         00000001400245C2
  0000000140022217: E9 A6 23 00 00     jmp         00000001400245C2
  000000014002221C: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022224: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022227: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002222F: FF 90 70 06 00 00  call        qword ptr [rax+0000000000000670h]
  0000000140022235: 33 C0              xor         eax,eax
  0000000140022237: E9 86 23 00 00     jmp         00000001400245C2
  000000014002223C: E9 81 23 00 00     jmp         00000001400245C2
  0000000140022241: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022249: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002224C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022254: FF 90 78 06 00 00  call        qword ptr [rax+0000000000000678h]
  000000014002225A: 33 C0              xor         eax,eax
  000000014002225C: E9 61 23 00 00     jmp         00000001400245C2
  0000000140022261: E9 5C 23 00 00     jmp         00000001400245C2
  0000000140022266: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002226E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022271: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022279: FF 90 80 06 00 00  call        qword ptr [rax+0000000000000680h]
  000000014002227F: 33 C0              xor         eax,eax
  0000000140022281: E9 3C 23 00 00     jmp         00000001400245C2
  0000000140022286: E9 37 23 00 00     jmp         00000001400245C2
  000000014002228B: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022293: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022296: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002229E: FF 90 88 06 00 00  call        qword ptr [rax+0000000000000688h]
  00000001400222A4: 33 C0              xor         eax,eax
  00000001400222A6: E9 17 23 00 00     jmp         00000001400245C2
  00000001400222AB: E9 12 23 00 00     jmp         00000001400245C2
  00000001400222B0: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400222B8: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400222BB: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400222C3: FF 90 90 06 00 00  call        qword ptr [rax+0000000000000690h]
  00000001400222C9: 33 C0              xor         eax,eax
  00000001400222CB: E9 F2 22 00 00     jmp         00000001400245C2
  00000001400222D0: E9 ED 22 00 00     jmp         00000001400245C2
  00000001400222D5: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400222DD: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400222E0: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400222E8: FF 90 98 06 00 00  call        qword ptr [rax+0000000000000698h]
  00000001400222EE: 33 C0              xor         eax,eax
  00000001400222F0: E9 CD 22 00 00     jmp         00000001400245C2
  00000001400222F5: E9 C8 22 00 00     jmp         00000001400245C2
  00000001400222FA: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022302: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022305: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002230D: FF 90 A0 06 00 00  call        qword ptr [rax+00000000000006A0h]
  0000000140022313: 33 C0              xor         eax,eax
  0000000140022315: E9 A8 22 00 00     jmp         00000001400245C2
  000000014002231A: E9 A3 22 00 00     jmp         00000001400245C2
  000000014002231F: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022327: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002232A: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022332: FF 90 A8 06 00 00  call        qword ptr [rax+00000000000006A8h]
  0000000140022338: 33 C0              xor         eax,eax
  000000014002233A: E9 83 22 00 00     jmp         00000001400245C2
  000000014002233F: E9 7E 22 00 00     jmp         00000001400245C2
  0000000140022344: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002234C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002234F: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022357: FF 90 B0 06 00 00  call        qword ptr [rax+00000000000006B0h]
  000000014002235D: 33 C0              xor         eax,eax
  000000014002235F: E9 5E 22 00 00     jmp         00000001400245C2
  0000000140022364: E9 59 22 00 00     jmp         00000001400245C2
  0000000140022369: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022371: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022374: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002237C: FF 90 B8 06 00 00  call        qword ptr [rax+00000000000006B8h]
  0000000140022382: 33 C0              xor         eax,eax
  0000000140022384: E9 39 22 00 00     jmp         00000001400245C2
  0000000140022389: E9 34 22 00 00     jmp         00000001400245C2
  000000014002238E: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022396: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022399: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400223A1: FF 90 C0 06 00 00  call        qword ptr [rax+00000000000006C0h]
  00000001400223A7: 33 C0              xor         eax,eax
  00000001400223A9: E9 14 22 00 00     jmp         00000001400245C2
  00000001400223AE: E9 0F 22 00 00     jmp         00000001400245C2
  00000001400223B3: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400223BB: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400223BE: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400223C6: FF 90 C8 06 00 00  call        qword ptr [rax+00000000000006C8h]
  00000001400223CC: 33 C0              xor         eax,eax
  00000001400223CE: E9 EF 21 00 00     jmp         00000001400245C2
  00000001400223D3: E9 EA 21 00 00     jmp         00000001400245C2
  00000001400223D8: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400223E0: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400223E3: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400223EB: FF 90 D0 06 00 00  call        qword ptr [rax+00000000000006D0h]
  00000001400223F1: 33 C0              xor         eax,eax
  00000001400223F3: E9 CA 21 00 00     jmp         00000001400245C2
  00000001400223F8: E9 C5 21 00 00     jmp         00000001400245C2
  00000001400223FD: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022405: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022408: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022410: FF 90 D8 06 00 00  call        qword ptr [rax+00000000000006D8h]
  0000000140022416: 33 C0              xor         eax,eax
  0000000140022418: E9 A5 21 00 00     jmp         00000001400245C2
  000000014002241D: E9 A0 21 00 00     jmp         00000001400245C2
  0000000140022422: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002242A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002242D: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022435: FF 90 E0 06 00 00  call        qword ptr [rax+00000000000006E0h]
  000000014002243B: 33 C0              xor         eax,eax
  000000014002243D: E9 80 21 00 00     jmp         00000001400245C2
  0000000140022442: E9 7B 21 00 00     jmp         00000001400245C2
  0000000140022447: 33 C0              xor         eax,eax
  0000000140022449: 85 C0              test        eax,eax
  000000014002244B: 74 2C              je          0000000140022479
  000000014002244D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022455: 48 83 C0 08        add         rax,8
  0000000140022459: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022461: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022469: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002246D: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000140022470: 88 84 24 10 07 00  mov         byte ptr [rsp+0000000000000710h],al
                    00
  0000000140022477: EB 27              jmp         00000001400224A0
  0000000140022479: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022481: 48 83 C0 08        add         rax,8
  0000000140022485: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002248D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022495: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  0000000140022499: 88 84 24 10 07 00  mov         byte ptr [rsp+0000000000000710h],al
                    00
  00000001400224A0: 0F B6 84 24 10 07  movzx       eax,byte ptr [rsp+0000000000000710h]
                    00 00
  00000001400224A8: 88 84 24 C0 01 00  mov         byte ptr [rsp+00000000000001C0h],al
                    00
  00000001400224AF: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400224B7: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400224BA: 0F B6 94 24 C0 01  movzx       edx,byte ptr [rsp+00000000000001C0h]
                    00 00
  00000001400224C2: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400224CA: FF 90 E8 06 00 00  call        qword ptr [rax+00000000000006E8h]
  00000001400224D0: 33 C0              xor         eax,eax
  00000001400224D2: E9 EB 20 00 00     jmp         00000001400245C2
  00000001400224D7: E9 E6 20 00 00     jmp         00000001400245C2
  00000001400224DC: 33 C0              xor         eax,eax
  00000001400224DE: 85 C0              test        eax,eax
  00000001400224E0: 74 2C              je          000000014002250E
  00000001400224E2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400224EA: 48 83 C0 08        add         rax,8
  00000001400224EE: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400224F6: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400224FE: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022502: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000140022505: 88 84 24 11 07 00  mov         byte ptr [rsp+0000000000000711h],al
                    00
  000000014002250C: EB 27              jmp         0000000140022535
  000000014002250E: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022516: 48 83 C0 08        add         rax,8
  000000014002251A: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022522: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002252A: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  000000014002252E: 88 84 24 11 07 00  mov         byte ptr [rsp+0000000000000711h],al
                    00
  0000000140022535: 0F B6 84 24 11 07  movzx       eax,byte ptr [rsp+0000000000000711h]
                    00 00
  000000014002253D: 88 84 24 C1 01 00  mov         byte ptr [rsp+00000000000001C1h],al
                    00
  0000000140022544: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002254C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002254F: 0F B6 94 24 C1 01  movzx       edx,byte ptr [rsp+00000000000001C1h]
                    00 00
  0000000140022557: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002255F: FF 90 F0 06 00 00  call        qword ptr [rax+00000000000006F0h]
  0000000140022565: 33 C0              xor         eax,eax
  0000000140022567: E9 56 20 00 00     jmp         00000001400245C2
  000000014002256C: E9 51 20 00 00     jmp         00000001400245C2
  0000000140022571: 33 C0              xor         eax,eax
  0000000140022573: 85 C0              test        eax,eax
  0000000140022575: 74 2C              je          00000001400225A3
  0000000140022577: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002257F: 48 83 C0 08        add         rax,8
  0000000140022583: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002258B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022593: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022597: 0F B6 00           movzx       eax,byte ptr [rax]
  000000014002259A: 88 84 24 12 07 00  mov         byte ptr [rsp+0000000000000712h],al
                    00
  00000001400225A1: EB 27              jmp         00000001400225CA
  00000001400225A3: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400225AB: 48 83 C0 08        add         rax,8
  00000001400225AF: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400225B7: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400225BF: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  00000001400225C3: 88 84 24 12 07 00  mov         byte ptr [rsp+0000000000000712h],al
                    00
  00000001400225CA: 0F B6 84 24 12 07  movzx       eax,byte ptr [rsp+0000000000000712h]
                    00 00
  00000001400225D2: 88 84 24 C2 01 00  mov         byte ptr [rsp+00000000000001C2h],al
                    00
  00000001400225D9: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400225E1: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400225E4: 0F B6 94 24 C2 01  movzx       edx,byte ptr [rsp+00000000000001C2h]
                    00 00
  00000001400225EC: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400225F4: FF 90 F8 06 00 00  call        qword ptr [rax+00000000000006F8h]
  00000001400225FA: 33 C0              xor         eax,eax
  00000001400225FC: E9 C1 1F 00 00     jmp         00000001400245C2
  0000000140022601: E9 BC 1F 00 00     jmp         00000001400245C2
  0000000140022606: 33 C0              xor         eax,eax
  0000000140022608: 85 C0              test        eax,eax
  000000014002260A: 74 2C              je          0000000140022638
  000000014002260C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022614: 48 83 C0 08        add         rax,8
  0000000140022618: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022620: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022628: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002262C: 0F B6 00           movzx       eax,byte ptr [rax]
  000000014002262F: 88 84 24 13 07 00  mov         byte ptr [rsp+0000000000000713h],al
                    00
  0000000140022636: EB 27              jmp         000000014002265F
  0000000140022638: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022640: 48 83 C0 08        add         rax,8
  0000000140022644: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002264C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022654: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  0000000140022658: 88 84 24 13 07 00  mov         byte ptr [rsp+0000000000000713h],al
                    00
  000000014002265F: 0F B6 84 24 13 07  movzx       eax,byte ptr [rsp+0000000000000713h]
                    00 00
  0000000140022667: 88 84 24 C3 01 00  mov         byte ptr [rsp+00000000000001C3h],al
                    00
  000000014002266E: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022676: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022679: 0F B6 94 24 C3 01  movzx       edx,byte ptr [rsp+00000000000001C3h]
                    00 00
  0000000140022681: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022689: FF 90 00 07 00 00  call        qword ptr [rax+0000000000000700h]
  000000014002268F: 33 C0              xor         eax,eax
  0000000140022691: E9 2C 1F 00 00     jmp         00000001400245C2
  0000000140022696: E9 27 1F 00 00     jmp         00000001400245C2
  000000014002269B: 33 C0              xor         eax,eax
  000000014002269D: 85 C0              test        eax,eax
  000000014002269F: 74 2C              je          00000001400226CD
  00000001400226A1: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400226A9: 48 83 C0 08        add         rax,8
  00000001400226AD: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400226B5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400226BD: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400226C1: 0F B6 00           movzx       eax,byte ptr [rax]
  00000001400226C4: 88 84 24 14 07 00  mov         byte ptr [rsp+0000000000000714h],al
                    00
  00000001400226CB: EB 27              jmp         00000001400226F4
  00000001400226CD: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400226D5: 48 83 C0 08        add         rax,8
  00000001400226D9: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400226E1: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400226E9: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  00000001400226ED: 88 84 24 14 07 00  mov         byte ptr [rsp+0000000000000714h],al
                    00
  00000001400226F4: 0F B6 84 24 14 07  movzx       eax,byte ptr [rsp+0000000000000714h]
                    00 00
  00000001400226FC: 88 84 24 C4 01 00  mov         byte ptr [rsp+00000000000001C4h],al
                    00
  0000000140022703: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002270B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002270E: 0F B6 94 24 C4 01  movzx       edx,byte ptr [rsp+00000000000001C4h]
                    00 00
  0000000140022716: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002271E: FF 90 08 07 00 00  call        qword ptr [rax+0000000000000708h]
  0000000140022724: 33 C0              xor         eax,eax
  0000000140022726: E9 97 1E 00 00     jmp         00000001400245C2
  000000014002272B: E9 92 1E 00 00     jmp         00000001400245C2
  0000000140022730: 33 C0              xor         eax,eax
  0000000140022732: 85 C0              test        eax,eax
  0000000140022734: 74 2C              je          0000000140022762
  0000000140022736: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002273E: 48 83 C0 08        add         rax,8
  0000000140022742: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002274A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022752: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022756: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000140022759: 88 84 24 15 07 00  mov         byte ptr [rsp+0000000000000715h],al
                    00
  0000000140022760: EB 27              jmp         0000000140022789
  0000000140022762: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002276A: 48 83 C0 08        add         rax,8
  000000014002276E: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022776: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002277E: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  0000000140022782: 88 84 24 15 07 00  mov         byte ptr [rsp+0000000000000715h],al
                    00
  0000000140022789: 0F B6 84 24 15 07  movzx       eax,byte ptr [rsp+0000000000000715h]
                    00 00
  0000000140022791: 88 84 24 C5 01 00  mov         byte ptr [rsp+00000000000001C5h],al
                    00
  0000000140022798: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400227A0: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400227A3: 0F B6 94 24 C5 01  movzx       edx,byte ptr [rsp+00000000000001C5h]
                    00 00
  00000001400227AB: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400227B3: FF 90 10 07 00 00  call        qword ptr [rax+0000000000000710h]
  00000001400227B9: 33 C0              xor         eax,eax
  00000001400227BB: E9 02 1E 00 00     jmp         00000001400245C2
  00000001400227C0: E9 FD 1D 00 00     jmp         00000001400245C2
  00000001400227C5: 33 C0              xor         eax,eax
  00000001400227C7: 85 C0              test        eax,eax
  00000001400227C9: 74 2D              je          00000001400227F8
  00000001400227CB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400227D3: 48 83 C0 08        add         rax,8
  00000001400227D7: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400227DF: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400227E7: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400227EB: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400227EE: 48 89 84 24 18 07  mov         qword ptr [rsp+0000000000000718h],rax
                    00 00
  00000001400227F6: EB 28              jmp         0000000140022820
  00000001400227F8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022800: 48 83 C0 08        add         rax,8
  0000000140022804: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002280C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022814: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022818: 48 89 84 24 18 07  mov         qword ptr [rsp+0000000000000718h],rax
                    00 00
  0000000140022820: 48 8B 84 24 18 07  mov         rax,qword ptr [rsp+0000000000000718h]
                    00 00
  0000000140022828: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  0000000140022830: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022838: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002283B: 48 8B 94 24 C8 01  mov         rdx,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000140022843: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002284B: FF 90 18 07 00 00  call        qword ptr [rax+0000000000000718h]
  0000000140022851: 33 C0              xor         eax,eax
  0000000140022853: E9 6A 1D 00 00     jmp         00000001400245C2
  0000000140022858: E9 65 1D 00 00     jmp         00000001400245C2
  000000014002285D: 33 C0              xor         eax,eax
  000000014002285F: 85 C0              test        eax,eax
  0000000140022861: 74 2B              je          000000014002288E
  0000000140022863: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002286B: 48 83 C0 08        add         rax,8
  000000014002286F: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022877: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002287F: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022883: 8B 00              mov         eax,dword ptr [rax]
  0000000140022885: 89 84 24 20 07 00  mov         dword ptr [rsp+0000000000000720h],eax
                    00
  000000014002288C: EB 26              jmp         00000001400228B4
  000000014002288E: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022896: 48 83 C0 08        add         rax,8
  000000014002289A: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400228A2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400228AA: 8B 40 F8           mov         eax,dword ptr [rax-8]
  00000001400228AD: 89 84 24 20 07 00  mov         dword ptr [rsp+0000000000000720h],eax
                    00
  00000001400228B4: 8B 84 24 20 07 00  mov         eax,dword ptr [rsp+0000000000000720h]
                    00
  00000001400228BB: 89 84 24 D0 01 00  mov         dword ptr [rsp+00000000000001D0h],eax
                    00
  00000001400228C2: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400228CA: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400228CD: 8B 94 24 D0 01 00  mov         edx,dword ptr [rsp+00000000000001D0h]
                    00
  00000001400228D4: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400228DC: FF 90 20 07 00 00  call        qword ptr [rax+0000000000000720h]
  00000001400228E2: 33 C0              xor         eax,eax
  00000001400228E4: E9 D9 1C 00 00     jmp         00000001400245C2
  00000001400228E9: E9 D4 1C 00 00     jmp         00000001400245C2
  00000001400228EE: 33 C0              xor         eax,eax
  00000001400228F0: 85 C0              test        eax,eax
  00000001400228F2: 74 2D              je          0000000140022921
  00000001400228F4: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400228FC: 48 83 C0 08        add         rax,8
  0000000140022900: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022908: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022910: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022914: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022917: 48 89 84 24 28 07  mov         qword ptr [rsp+0000000000000728h],rax
                    00 00
  000000014002291F: EB 28              jmp         0000000140022949
  0000000140022921: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022929: 48 83 C0 08        add         rax,8
  000000014002292D: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022935: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002293D: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022941: 48 89 84 24 28 07  mov         qword ptr [rsp+0000000000000728h],rax
                    00 00
  0000000140022949: 48 8B 84 24 28 07  mov         rax,qword ptr [rsp+0000000000000728h]
                    00 00
  0000000140022951: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  0000000140022959: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022961: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022964: 48 8B 94 24 D8 01  mov         rdx,qword ptr [rsp+00000000000001D8h]
                    00 00
  000000014002296C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022974: FF 90 28 07 00 00  call        qword ptr [rax+0000000000000728h]
  000000014002297A: 33 C0              xor         eax,eax
  000000014002297C: E9 41 1C 00 00     jmp         00000001400245C2
  0000000140022981: E9 3C 1C 00 00     jmp         00000001400245C2
  0000000140022986: 33 C0              xor         eax,eax
  0000000140022988: 85 C0              test        eax,eax
  000000014002298A: 74 2D              je          00000001400229B9
  000000014002298C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022994: 48 83 C0 08        add         rax,8
  0000000140022998: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400229A0: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400229A8: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400229AC: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400229AF: 48 89 84 24 30 07  mov         qword ptr [rsp+0000000000000730h],rax
                    00 00
  00000001400229B7: EB 28              jmp         00000001400229E1
  00000001400229B9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400229C1: 48 83 C0 08        add         rax,8
  00000001400229C5: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400229CD: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400229D5: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400229D9: 48 89 84 24 30 07  mov         qword ptr [rsp+0000000000000730h],rax
                    00 00
  00000001400229E1: 48 8B 84 24 30 07  mov         rax,qword ptr [rsp+0000000000000730h]
                    00 00
  00000001400229E9: 48 89 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],rax
                    00 00
  00000001400229F1: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400229F9: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400229FC: 48 8B 94 24 E0 01  mov         rdx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140022A04: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022A0C: FF 90 30 07 00 00  call        qword ptr [rax+0000000000000730h]
  0000000140022A12: 33 C0              xor         eax,eax
  0000000140022A14: E9 A9 1B 00 00     jmp         00000001400245C2
  0000000140022A19: E9 A4 1B 00 00     jmp         00000001400245C2
  0000000140022A1E: 33 C0              xor         eax,eax
  0000000140022A20: 85 C0              test        eax,eax
  0000000140022A22: 74 2D              je          0000000140022A51
  0000000140022A24: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022A2C: 48 83 C0 08        add         rax,8
  0000000140022A30: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022A38: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022A40: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022A44: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022A47: 48 89 84 24 38 07  mov         qword ptr [rsp+0000000000000738h],rax
                    00 00
  0000000140022A4F: EB 28              jmp         0000000140022A79
  0000000140022A51: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022A59: 48 83 C0 08        add         rax,8
  0000000140022A5D: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022A65: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022A6D: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022A71: 48 89 84 24 38 07  mov         qword ptr [rsp+0000000000000738h],rax
                    00 00
  0000000140022A79: 48 8B 84 24 38 07  mov         rax,qword ptr [rsp+0000000000000738h]
                    00 00
  0000000140022A81: 48 89 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],rax
                    00 00
  0000000140022A89: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022A91: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022A94: 48 8B 94 24 E8 01  mov         rdx,qword ptr [rsp+00000000000001E8h]
                    00 00
  0000000140022A9C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022AA4: FF 90 38 07 00 00  call        qword ptr [rax+0000000000000738h]
  0000000140022AAA: 33 C0              xor         eax,eax
  0000000140022AAC: E9 11 1B 00 00     jmp         00000001400245C2
  0000000140022AB1: E9 0C 1B 00 00     jmp         00000001400245C2
  0000000140022AB6: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022ABE: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022AC1: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022AC9: FF 90 40 07 00 00  call        qword ptr [rax+0000000000000740h]
  0000000140022ACF: 33 C0              xor         eax,eax
  0000000140022AD1: E9 EC 1A 00 00     jmp         00000001400245C2
  0000000140022AD6: E9 E7 1A 00 00     jmp         00000001400245C2
  0000000140022ADB: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022AE3: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022AE6: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022AEE: FF 90 48 07 00 00  call        qword ptr [rax+0000000000000748h]
  0000000140022AF4: 33 C0              xor         eax,eax
  0000000140022AF6: E9 C7 1A 00 00     jmp         00000001400245C2
  0000000140022AFB: E9 C2 1A 00 00     jmp         00000001400245C2
  0000000140022B00: 33 C0              xor         eax,eax
  0000000140022B02: 85 C0              test        eax,eax
  0000000140022B04: 74 2B              je          0000000140022B31
  0000000140022B06: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022B0E: 48 83 C0 08        add         rax,8
  0000000140022B12: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022B1A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022B22: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022B26: 8B 00              mov         eax,dword ptr [rax]
  0000000140022B28: 89 84 24 40 07 00  mov         dword ptr [rsp+0000000000000740h],eax
                    00
  0000000140022B2F: EB 26              jmp         0000000140022B57
  0000000140022B31: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022B39: 48 83 C0 08        add         rax,8
  0000000140022B3D: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022B45: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022B4D: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140022B50: 89 84 24 40 07 00  mov         dword ptr [rsp+0000000000000740h],eax
                    00
  0000000140022B57: 8B 84 24 40 07 00  mov         eax,dword ptr [rsp+0000000000000740h]
                    00
  0000000140022B5E: 89 84 24 F0 01 00  mov         dword ptr [rsp+00000000000001F0h],eax
                    00
  0000000140022B65: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022B6D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022B70: 8B 94 24 F0 01 00  mov         edx,dword ptr [rsp+00000000000001F0h]
                    00
  0000000140022B77: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022B7F: FF 90 50 07 00 00  call        qword ptr [rax+0000000000000750h]
  0000000140022B85: 33 C0              xor         eax,eax
  0000000140022B87: E9 36 1A 00 00     jmp         00000001400245C2
  0000000140022B8C: E9 31 1A 00 00     jmp         00000001400245C2
  0000000140022B91: 33 C0              xor         eax,eax
  0000000140022B93: 85 C0              test        eax,eax
  0000000140022B95: 74 2B              je          0000000140022BC2
  0000000140022B97: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022B9F: 48 83 C0 08        add         rax,8
  0000000140022BA3: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022BAB: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022BB3: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022BB7: 8B 00              mov         eax,dword ptr [rax]
  0000000140022BB9: 89 84 24 44 07 00  mov         dword ptr [rsp+0000000000000744h],eax
                    00
  0000000140022BC0: EB 26              jmp         0000000140022BE8
  0000000140022BC2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022BCA: 48 83 C0 08        add         rax,8
  0000000140022BCE: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022BD6: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022BDE: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140022BE1: 89 84 24 44 07 00  mov         dword ptr [rsp+0000000000000744h],eax
                    00
  0000000140022BE8: 8B 84 24 44 07 00  mov         eax,dword ptr [rsp+0000000000000744h]
                    00
  0000000140022BEF: 89 84 24 F4 01 00  mov         dword ptr [rsp+00000000000001F4h],eax
                    00
  0000000140022BF6: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022BFE: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022C01: 8B 94 24 F4 01 00  mov         edx,dword ptr [rsp+00000000000001F4h]
                    00
  0000000140022C08: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022C10: FF 90 58 07 00 00  call        qword ptr [rax+0000000000000758h]
  0000000140022C16: 33 C0              xor         eax,eax
  0000000140022C18: E9 A5 19 00 00     jmp         00000001400245C2
  0000000140022C1D: E9 A0 19 00 00     jmp         00000001400245C2
  0000000140022C22: 33 C0              xor         eax,eax
  0000000140022C24: 85 C0              test        eax,eax
  0000000140022C26: 74 2B              je          0000000140022C53
  0000000140022C28: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022C30: 48 83 C0 08        add         rax,8
  0000000140022C34: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022C3C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022C44: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022C48: 8B 00              mov         eax,dword ptr [rax]
  0000000140022C4A: 89 84 24 48 07 00  mov         dword ptr [rsp+0000000000000748h],eax
                    00
  0000000140022C51: EB 26              jmp         0000000140022C79
  0000000140022C53: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022C5B: 48 83 C0 08        add         rax,8
  0000000140022C5F: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022C67: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022C6F: 8B 40 F8           mov         eax,dword ptr [rax-8]
  0000000140022C72: 89 84 24 48 07 00  mov         dword ptr [rsp+0000000000000748h],eax
                    00
  0000000140022C79: 8B 84 24 48 07 00  mov         eax,dword ptr [rsp+0000000000000748h]
                    00
  0000000140022C80: 89 84 24 F8 01 00  mov         dword ptr [rsp+00000000000001F8h],eax
                    00
  0000000140022C87: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022C8F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022C92: 8B 94 24 F8 01 00  mov         edx,dword ptr [rsp+00000000000001F8h]
                    00
  0000000140022C99: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022CA1: FF 90 60 07 00 00  call        qword ptr [rax+0000000000000760h]
  0000000140022CA7: 33 C0              xor         eax,eax
  0000000140022CA9: E9 14 19 00 00     jmp         00000001400245C2
  0000000140022CAE: E9 0F 19 00 00     jmp         00000001400245C2
  0000000140022CB3: 33 C0              xor         eax,eax
  0000000140022CB5: 85 C0              test        eax,eax
  0000000140022CB7: 74 2D              je          0000000140022CE6
  0000000140022CB9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022CC1: 48 83 C0 08        add         rax,8
  0000000140022CC5: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022CCD: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022CD5: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022CD9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022CDC: 48 89 84 24 50 07  mov         qword ptr [rsp+0000000000000750h],rax
                    00 00
  0000000140022CE4: EB 28              jmp         0000000140022D0E
  0000000140022CE6: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022CEE: 48 83 C0 08        add         rax,8
  0000000140022CF2: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022CFA: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022D02: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022D06: 48 89 84 24 50 07  mov         qword ptr [rsp+0000000000000750h],rax
                    00 00
  0000000140022D0E: 48 8B 84 24 50 07  mov         rax,qword ptr [rsp+0000000000000750h]
                    00 00
  0000000140022D16: 48 89 84 24 00 02  mov         qword ptr [rsp+0000000000000200h],rax
                    00 00
  0000000140022D1E: 33 C0              xor         eax,eax
  0000000140022D20: 85 C0              test        eax,eax
  0000000140022D22: 74 2D              je          0000000140022D51
  0000000140022D24: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022D2C: 48 83 C0 08        add         rax,8
  0000000140022D30: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022D38: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022D40: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022D44: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022D47: 48 89 84 24 58 07  mov         qword ptr [rsp+0000000000000758h],rax
                    00 00
  0000000140022D4F: EB 28              jmp         0000000140022D79
  0000000140022D51: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022D59: 48 83 C0 08        add         rax,8
  0000000140022D5D: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022D65: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022D6D: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022D71: 48 89 84 24 58 07  mov         qword ptr [rsp+0000000000000758h],rax
                    00 00
  0000000140022D79: 48 8B 84 24 58 07  mov         rax,qword ptr [rsp+0000000000000758h]
                    00 00
  0000000140022D81: 48 89 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],rax
                    00 00
  0000000140022D89: 4C 8B 84 24 08 02  mov         r8,qword ptr [rsp+0000000000000208h]
                    00 00
  0000000140022D91: 48 8B 94 24 00 02  mov         rdx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140022D99: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022DA1: E8 AA 43 C7 00     call        0000000140C97150
  0000000140022DA6: 33 C0              xor         eax,eax
  0000000140022DA8: E9 15 18 00 00     jmp         00000001400245C2
  0000000140022DAD: E9 10 18 00 00     jmp         00000001400245C2
  0000000140022DB2: 33 C0              xor         eax,eax
  0000000140022DB4: 85 C0              test        eax,eax
  0000000140022DB6: 74 2D              je          0000000140022DE5
  0000000140022DB8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022DC0: 48 83 C0 08        add         rax,8
  0000000140022DC4: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022DCC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022DD4: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022DD8: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022DDB: 48 89 84 24 60 07  mov         qword ptr [rsp+0000000000000760h],rax
                    00 00
  0000000140022DE3: EB 28              jmp         0000000140022E0D
  0000000140022DE5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022DED: 48 83 C0 08        add         rax,8
  0000000140022DF1: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022DF9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022E01: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022E05: 48 89 84 24 60 07  mov         qword ptr [rsp+0000000000000760h],rax
                    00 00
  0000000140022E0D: 48 8B 84 24 60 07  mov         rax,qword ptr [rsp+0000000000000760h]
                    00 00
  0000000140022E15: 48 89 84 24 10 02  mov         qword ptr [rsp+0000000000000210h],rax
                    00 00
  0000000140022E1D: 33 C0              xor         eax,eax
  0000000140022E1F: 83 F8 01           cmp         eax,1
  0000000140022E22: 74 2A              je          0000000140022E4E
  0000000140022E24: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022E2C: 48 83 C0 08        add         rax,8
  0000000140022E30: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022E38: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022E40: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022E44: 48 89 84 24 68 07  mov         qword ptr [rsp+0000000000000768h],rax
                    00 00
  0000000140022E4C: EB 28              jmp         0000000140022E76
  0000000140022E4E: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022E56: 48 83 C0 08        add         rax,8
  0000000140022E5A: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022E62: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022E6A: 48 83 E8 08        sub         rax,8
  0000000140022E6E: 48 89 84 24 68 07  mov         qword ptr [rsp+0000000000000768h],rax
                    00 00
  0000000140022E76: 48 8B 84 24 68 07  mov         rax,qword ptr [rsp+0000000000000768h]
                    00 00
  0000000140022E7E: 48 89 84 24 28 04  mov         qword ptr [rsp+0000000000000428h],rax
                    00 00
  0000000140022E86: 48 8D BC 24 20 05  lea         rdi,[rsp+0000000000000520h]
                    00 00
  0000000140022E8E: 48 8B B4 24 28 04  mov         rsi,qword ptr [rsp+0000000000000428h]
                    00 00
  0000000140022E96: B9 10 00 00 00     mov         ecx,10h
  0000000140022E9B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140022E9D: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  0000000140022EA5: 48 8D B4 24 20 05  lea         rsi,[rsp+0000000000000520h]
                    00 00
  0000000140022EAD: B9 10 00 00 00     mov         ecx,10h
  0000000140022EB2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140022EB4: 33 C0              xor         eax,eax
  0000000140022EB6: 85 C0              test        eax,eax
  0000000140022EB8: 74 2A              je          0000000140022EE4
  0000000140022EBA: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022EC2: 48 83 C0 08        add         rax,8
  0000000140022EC6: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022ECE: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022ED6: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022EDA: 48 89 84 24 70 07  mov         qword ptr [rsp+0000000000000770h],rax
                    00 00
  0000000140022EE2: EB 28              jmp         0000000140022F0C
  0000000140022EE4: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022EEC: 48 83 C0 08        add         rax,8
  0000000140022EF0: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022EF8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022F00: 48 83 E8 08        sub         rax,8
  0000000140022F04: 48 89 84 24 70 07  mov         qword ptr [rsp+0000000000000770h],rax
                    00 00
  0000000140022F0C: 48 8B 84 24 70 07  mov         rax,qword ptr [rsp+0000000000000770h]
                    00 00
  0000000140022F14: 48 89 84 24 30 04  mov         qword ptr [rsp+0000000000000430h],rax
                    00 00
  0000000140022F1C: 48 8B 94 24 30 04  mov         rdx,qword ptr [rsp+0000000000000430h]
                    00 00
  0000000140022F24: 48 8D 8C 24 18 02  lea         rcx,[rsp+0000000000000218h]
                    00 00
  0000000140022F2C: FF 15 A6 2A DE 01  call        qword ptr [0000000141E059D8h]
  0000000140022F32: 90                 nop
  0000000140022F33: 48 8D 84 24 38 04  lea         rax,[rsp+0000000000000438h]
                    00 00
  0000000140022F3B: 48 89 84 24 40 04  mov         qword ptr [rsp+0000000000000440h],rax
                    00 00
  0000000140022F43: 48 8D 94 24 18 02  lea         rdx,[rsp+0000000000000218h]
                    00 00
  0000000140022F4B: 48 8B 8C 24 40 04  mov         rcx,qword ptr [rsp+0000000000000440h]
                    00 00
  0000000140022F53: FF 15 7F 2A DE 01  call        qword ptr [0000000141E059D8h]
  0000000140022F59: 48 89 84 24 78 07  mov         qword ptr [rsp+0000000000000778h],rax
                    00 00
  0000000140022F61: 48 8D BC 24 D0 04  lea         rdi,[rsp+00000000000004D0h]
                    00 00
  0000000140022F69: 48 8D B4 24 20 02  lea         rsi,[rsp+0000000000000220h]
                    00 00
  0000000140022F71: B9 10 00 00 00     mov         ecx,10h
  0000000140022F76: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140022F78: 4C 8B 8C 24 78 07  mov         r9,qword ptr [rsp+0000000000000778h]
                    00 00
  0000000140022F80: 4C 8D 84 24 D0 04  lea         r8,[rsp+00000000000004D0h]
                    00 00
  0000000140022F88: 48 8B 94 24 10 02  mov         rdx,qword ptr [rsp+0000000000000210h]
                    00 00
  0000000140022F90: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140022F98: E8 43 42 C7 00     call        0000000140C971E0
  0000000140022F9D: C7 84 24 48 04 00  mov         dword ptr [rsp+0000000000000448h],0
                    00 00 00 00 00
  0000000140022FA8: 48 8D 8C 24 18 02  lea         rcx,[rsp+0000000000000218h]
                    00 00
  0000000140022FB0: FF 15 2A 2A DE 01  call        qword ptr [0000000141E059E0h]
  0000000140022FB6: 8B 84 24 48 04 00  mov         eax,dword ptr [rsp+0000000000000448h]
                    00
  0000000140022FBD: E9 00 16 00 00     jmp         00000001400245C2
  0000000140022FC2: E9 FB 15 00 00     jmp         00000001400245C2
  0000000140022FC7: 33 C0              xor         eax,eax
  0000000140022FC9: 85 C0              test        eax,eax
  0000000140022FCB: 74 2D              je          0000000140022FFA
  0000000140022FCD: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022FD5: 48 83 C0 08        add         rax,8
  0000000140022FD9: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140022FE1: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140022FE9: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140022FED: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140022FF0: 48 89 84 24 80 07  mov         qword ptr [rsp+0000000000000780h],rax
                    00 00
  0000000140022FF8: EB 28              jmp         0000000140023022
  0000000140022FFA: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023002: 48 83 C0 08        add         rax,8
  0000000140023006: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002300E: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023016: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002301A: 48 89 84 24 80 07  mov         qword ptr [rsp+0000000000000780h],rax
                    00 00
  0000000140023022: 48 8B 84 24 80 07  mov         rax,qword ptr [rsp+0000000000000780h]
                    00 00
  000000014002302A: 48 89 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],rax
                    00 00
  0000000140023032: 33 C0              xor         eax,eax
  0000000140023034: 83 F8 01           cmp         eax,1
  0000000140023037: 74 2A              je          0000000140023063
  0000000140023039: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023041: 48 83 C0 08        add         rax,8
  0000000140023045: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002304D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023055: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140023059: 48 89 84 24 88 07  mov         qword ptr [rsp+0000000000000788h],rax
                    00 00
  0000000140023061: EB 28              jmp         000000014002308B
  0000000140023063: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002306B: 48 83 C0 08        add         rax,8
  000000014002306F: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140023077: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002307F: 48 83 E8 08        sub         rax,8
  0000000140023083: 48 89 84 24 88 07  mov         qword ptr [rsp+0000000000000788h],rax
                    00 00
  000000014002308B: 48 8B 84 24 88 07  mov         rax,qword ptr [rsp+0000000000000788h]
                    00 00
  0000000140023093: 48 89 84 24 50 04  mov         qword ptr [rsp+0000000000000450h],rax
                    00 00
  000000014002309B: 48 8D BC 24 10 05  lea         rdi,[rsp+0000000000000510h]
                    00 00
  00000001400230A3: 48 8B B4 24 50 04  mov         rsi,qword ptr [rsp+0000000000000450h]
                    00 00
  00000001400230AB: B9 10 00 00 00     mov         ecx,10h
  00000001400230B0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400230B2: 48 8D BC 24 40 02  lea         rdi,[rsp+0000000000000240h]
                    00 00
  00000001400230BA: 48 8D B4 24 10 05  lea         rsi,[rsp+0000000000000510h]
                    00 00
  00000001400230C2: B9 10 00 00 00     mov         ecx,10h
  00000001400230C7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400230C9: 33 C0              xor         eax,eax
  00000001400230CB: 85 C0              test        eax,eax
  00000001400230CD: 74 2A              je          00000001400230F9
  00000001400230CF: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400230D7: 48 83 C0 08        add         rax,8
  00000001400230DB: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400230E3: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400230EB: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400230EF: 48 89 84 24 90 07  mov         qword ptr [rsp+0000000000000790h],rax
                    00 00
  00000001400230F7: EB 28              jmp         0000000140023121
  00000001400230F9: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023101: 48 83 C0 08        add         rax,8
  0000000140023105: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002310D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023115: 48 83 E8 08        sub         rax,8
  0000000140023119: 48 89 84 24 90 07  mov         qword ptr [rsp+0000000000000790h],rax
                    00 00
  0000000140023121: 48 8B 84 24 90 07  mov         rax,qword ptr [rsp+0000000000000790h]
                    00 00
  0000000140023129: 48 89 84 24 58 04  mov         qword ptr [rsp+0000000000000458h],rax
                    00 00
  0000000140023131: 48 8B 94 24 58 04  mov         rdx,qword ptr [rsp+0000000000000458h]
                    00 00
  0000000140023139: 48 8D 8C 24 38 02  lea         rcx,[rsp+0000000000000238h]
                    00 00
  0000000140023141: FF 15 91 28 DE 01  call        qword ptr [0000000141E059D8h]
  0000000140023147: 90                 nop
  0000000140023148: 48 8D 84 24 60 04  lea         rax,[rsp+0000000000000460h]
                    00 00
  0000000140023150: 48 89 84 24 68 04  mov         qword ptr [rsp+0000000000000468h],rax
                    00 00
  0000000140023158: 48 8D 94 24 38 02  lea         rdx,[rsp+0000000000000238h]
                    00 00
  0000000140023160: 48 8B 8C 24 68 04  mov         rcx,qword ptr [rsp+0000000000000468h]
                    00 00
  0000000140023168: FF 15 6A 28 DE 01  call        qword ptr [0000000141E059D8h]
  000000014002316E: 48 89 84 24 98 07  mov         qword ptr [rsp+0000000000000798h],rax
                    00 00
  0000000140023176: 48 8D BC 24 E0 04  lea         rdi,[rsp+00000000000004E0h]
                    00 00
  000000014002317E: 48 8D B4 24 40 02  lea         rsi,[rsp+0000000000000240h]
                    00 00
  0000000140023186: B9 10 00 00 00     mov         ecx,10h
  000000014002318B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002318D: 4C 8B 8C 24 98 07  mov         r9,qword ptr [rsp+0000000000000798h]
                    00 00
  0000000140023195: 4C 8D 84 24 E0 04  lea         r8,[rsp+00000000000004E0h]
                    00 00
  000000014002319D: 48 8B 94 24 30 02  mov         rdx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400231A5: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400231AD: E8 2E 41 C7 00     call        0000000140C972E0
  00000001400231B2: C7 84 24 70 04 00  mov         dword ptr [rsp+0000000000000470h],0
                    00 00 00 00 00
  00000001400231BD: 48 8D 8C 24 38 02  lea         rcx,[rsp+0000000000000238h]
                    00 00
  00000001400231C5: FF 15 15 28 DE 01  call        qword ptr [0000000141E059E0h]
  00000001400231CB: 8B 84 24 70 04 00  mov         eax,dword ptr [rsp+0000000000000470h]
                    00
  00000001400231D2: E9 EB 13 00 00     jmp         00000001400245C2
  00000001400231D7: E9 E6 13 00 00     jmp         00000001400245C2
  00000001400231DC: 33 C0              xor         eax,eax
  00000001400231DE: 85 C0              test        eax,eax
  00000001400231E0: 74 2D              je          000000014002320F
  00000001400231E2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400231EA: 48 83 C0 08        add         rax,8
  00000001400231EE: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400231F6: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400231FE: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140023202: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023205: 48 89 84 24 A0 07  mov         qword ptr [rsp+00000000000007A0h],rax
                    00 00
  000000014002320D: EB 28              jmp         0000000140023237
  000000014002320F: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023217: 48 83 C0 08        add         rax,8
  000000014002321B: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140023223: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002322B: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002322F: 48 89 84 24 A0 07  mov         qword ptr [rsp+00000000000007A0h],rax
                    00 00
  0000000140023237: 48 8B 84 24 A0 07  mov         rax,qword ptr [rsp+00000000000007A0h]
                    00 00
  000000014002323F: 48 89 84 24 50 02  mov         qword ptr [rsp+0000000000000250h],rax
                    00 00
  0000000140023247: 33 C0              xor         eax,eax
  0000000140023249: 85 C0              test        eax,eax
  000000014002324B: 74 2A              je          0000000140023277
  000000014002324D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023255: 48 83 C0 08        add         rax,8
  0000000140023259: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140023261: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023269: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002326D: 48 89 84 24 A8 07  mov         qword ptr [rsp+00000000000007A8h],rax
                    00 00
  0000000140023275: EB 28              jmp         000000014002329F
  0000000140023277: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002327F: 48 83 C0 08        add         rax,8
  0000000140023283: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002328B: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023293: 48 83 E8 08        sub         rax,8
  0000000140023297: 48 89 84 24 A8 07  mov         qword ptr [rsp+00000000000007A8h],rax
                    00 00
  000000014002329F: 48 8B 84 24 A8 07  mov         rax,qword ptr [rsp+00000000000007A8h]
                    00 00
  00000001400232A7: 48 89 84 24 78 04  mov         qword ptr [rsp+0000000000000478h],rax
                    00 00
  00000001400232AF: 48 8B 84 24 78 04  mov         rax,qword ptr [rsp+0000000000000478h]
                    00 00
  00000001400232B7: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400232BA: 48 89 84 24 60 02  mov         qword ptr [rsp+0000000000000260h],rax
                    00 00
  00000001400232C2: 33 C0              xor         eax,eax
  00000001400232C4: 85 C0              test        eax,eax
  00000001400232C6: 74 2A              je          00000001400232F2
  00000001400232C8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400232D0: 48 83 C0 08        add         rax,8
  00000001400232D4: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400232DC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400232E4: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400232E8: 48 89 84 24 B0 07  mov         qword ptr [rsp+00000000000007B0h],rax
                    00 00
  00000001400232F0: EB 28              jmp         000000014002331A
  00000001400232F2: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400232FA: 48 83 C0 08        add         rax,8
  00000001400232FE: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140023306: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002330E: 48 83 E8 08        sub         rax,8
  0000000140023312: 48 89 84 24 B0 07  mov         qword ptr [rsp+00000000000007B0h],rax
                    00 00
  000000014002331A: 48 8B 84 24 B0 07  mov         rax,qword ptr [rsp+00000000000007B0h]
                    00 00
  0000000140023322: 48 89 84 24 80 04  mov         qword ptr [rsp+0000000000000480h],rax
                    00 00
  000000014002332A: 48 8B 84 24 80 04  mov         rax,qword ptr [rsp+0000000000000480h]
                    00 00
  0000000140023332: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023335: 48 89 84 24 58 02  mov         qword ptr [rsp+0000000000000258h],rax
                    00 00
  000000014002333D: 4C 8B 8C 24 58 02  mov         r9,qword ptr [rsp+0000000000000258h]
                    00 00
  0000000140023345: 4C 8B 84 24 60 02  mov         r8,qword ptr [rsp+0000000000000260h]
                    00 00
  000000014002334D: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  0000000140023355: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002335D: E8 9E 18 C7 00     call        0000000140C94C00
  0000000140023362: 33 C0              xor         eax,eax
  0000000140023364: E9 59 12 00 00     jmp         00000001400245C2
  0000000140023369: E9 54 12 00 00     jmp         00000001400245C2
  000000014002336E: 33 C0              xor         eax,eax
  0000000140023370: 85 C0              test        eax,eax
  0000000140023372: 74 2D              je          00000001400233A1
  0000000140023374: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002337C: 48 83 C0 08        add         rax,8
  0000000140023380: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140023388: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023390: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140023394: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023397: 48 89 84 24 B8 07  mov         qword ptr [rsp+00000000000007B8h],rax
                    00 00
  000000014002339F: EB 28              jmp         00000001400233C9
  00000001400233A1: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400233A9: 48 83 C0 08        add         rax,8
  00000001400233AD: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400233B5: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400233BD: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400233C1: 48 89 84 24 B8 07  mov         qword ptr [rsp+00000000000007B8h],rax
                    00 00
  00000001400233C9: 48 8B 84 24 B8 07  mov         rax,qword ptr [rsp+00000000000007B8h]
                    00 00
  00000001400233D1: 48 89 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],rax
                    00 00
  00000001400233D9: 48 8B 94 24 68 02  mov         rdx,qword ptr [rsp+0000000000000268h]
                    00 00
  00000001400233E1: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400233E9: E8 F2 3F C7 00     call        0000000140C973E0
  00000001400233EE: 33 C0              xor         eax,eax
  00000001400233F0: E9 CD 11 00 00     jmp         00000001400245C2
  00000001400233F5: E9 C8 11 00 00     jmp         00000001400245C2
  00000001400233FA: 33 C0              xor         eax,eax
  00000001400233FC: 85 C0              test        eax,eax
  00000001400233FE: 74 2D              je          000000014002342D
  0000000140023400: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023408: 48 83 C0 08        add         rax,8
  000000014002340C: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140023414: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002341C: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140023420: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023423: 48 89 84 24 C0 07  mov         qword ptr [rsp+00000000000007C0h],rax
                    00 00
  000000014002342B: EB 28              jmp         0000000140023455
  000000014002342D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023435: 48 83 C0 08        add         rax,8
  0000000140023439: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140023441: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023449: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  000000014002344D: 48 89 84 24 C0 07  mov         qword ptr [rsp+00000000000007C0h],rax
                    00 00
  0000000140023455: 48 8B 84 24 C0 07  mov         rax,qword ptr [rsp+00000000000007C0h]
                    00 00
  000000014002345D: 48 89 84 24 70 02  mov         qword ptr [rsp+0000000000000270h],rax
                    00 00
  0000000140023465: 48 8B 94 24 70 02  mov         rdx,qword ptr [rsp+0000000000000270h]
                    00 00
  000000014002346D: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023475: E8 F6 3F C7 00     call        0000000140C97470
  000000014002347A: 33 C0              xor         eax,eax
  000000014002347C: E9 41 11 00 00     jmp         00000001400245C2
  0000000140023481: E9 3C 11 00 00     jmp         00000001400245C2
  0000000140023486: 33 C0              xor         eax,eax
  0000000140023488: 85 C0              test        eax,eax
  000000014002348A: 74 2C              je          00000001400234B8
  000000014002348C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023494: 48 83 C0 08        add         rax,8
  0000000140023498: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400234A0: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400234A8: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400234AC: 0F B6 00           movzx       eax,byte ptr [rax]
  00000001400234AF: 88 84 24 C8 07 00  mov         byte ptr [rsp+00000000000007C8h],al
                    00
  00000001400234B6: EB 27              jmp         00000001400234DF
  00000001400234B8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400234C0: 48 83 C0 08        add         rax,8
  00000001400234C4: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400234CC: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400234D4: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  00000001400234D8: 88 84 24 C8 07 00  mov         byte ptr [rsp+00000000000007C8h],al
                    00
  00000001400234DF: 0F B6 84 24 C8 07  movzx       eax,byte ptr [rsp+00000000000007C8h]
                    00 00
  00000001400234E7: 88 84 24 78 02 00  mov         byte ptr [rsp+0000000000000278h],al
                    00
  00000001400234EE: 33 C0              xor         eax,eax
  00000001400234F0: 85 C0              test        eax,eax
  00000001400234F2: 74 2D              je          0000000140023521
  00000001400234F4: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400234FC: 48 83 C0 08        add         rax,8
  0000000140023500: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140023508: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023510: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140023514: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023517: 48 89 84 24 D0 07  mov         qword ptr [rsp+00000000000007D0h],rax
                    00 00
  000000014002351F: EB 28              jmp         0000000140023549
  0000000140023521: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023529: 48 83 C0 08        add         rax,8
  000000014002352D: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  0000000140023535: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002353D: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140023541: 48 89 84 24 D0 07  mov         qword ptr [rsp+00000000000007D0h],rax
                    00 00
  0000000140023549: 48 8B 84 24 D0 07  mov         rax,qword ptr [rsp+00000000000007D0h]
                    00 00
  0000000140023551: 48 89 84 24 80 02  mov         qword ptr [rsp+0000000000000280h],rax
                    00 00
  0000000140023559: 4C 8B 84 24 80 02  mov         r8,qword ptr [rsp+0000000000000280h]
                    00 00
  0000000140023561: 0F B6 94 24 78 02  movzx       edx,byte ptr [rsp+0000000000000278h]
                    00 00
  0000000140023569: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023571: E8 8A 3F C7 00     call        0000000140C97500
  0000000140023576: 33 C0              xor         eax,eax
  0000000140023578: E9 45 10 00 00     jmp         00000001400245C2
  000000014002357D: E9 40 10 00 00     jmp         00000001400245C2
  0000000140023582: 33 C0              xor         eax,eax
  0000000140023584: 85 C0              test        eax,eax
  0000000140023586: 74 2C              je          00000001400235B4
  0000000140023588: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023590: 48 83 C0 08        add         rax,8
  0000000140023594: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002359C: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400235A4: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  00000001400235A8: 0F B6 00           movzx       eax,byte ptr [rax]
  00000001400235AB: 88 84 24 D8 07 00  mov         byte ptr [rsp+00000000000007D8h],al
                    00
  00000001400235B2: EB 27              jmp         00000001400235DB
  00000001400235B4: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400235BC: 48 83 C0 08        add         rax,8
  00000001400235C0: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  00000001400235C8: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  00000001400235D0: 0F B6 40 F8        movzx       eax,byte ptr [rax-8]
  00000001400235D4: 88 84 24 D8 07 00  mov         byte ptr [rsp+00000000000007D8h],al
                    00
  00000001400235DB: 0F B6 84 24 D8 07  movzx       eax,byte ptr [rsp+00000000000007D8h]
                    00 00
  00000001400235E3: 88 84 24 88 02 00  mov         byte ptr [rsp+0000000000000288h],al
                    00
  00000001400235EA: 44 0F B6 84 24 88  movzx       r8d,byte ptr [rsp+0000000000000288h]
                    02 00 00
  00000001400235F3: 48 8D 94 24 88 04  lea         rdx,[rsp+0000000000000488h]
                    00 00
  00000001400235FB: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023603: E8 C8 9E C7 00     call        0000000140C9D4D0
  0000000140023608: 48 8D 8C 24 88 04  lea         rcx,[rsp+0000000000000488h]
                    00 00
  0000000140023610: FF 15 A2 85 DE 01  call        qword ptr [0000000141E0BBB8h]
  0000000140023616: 33 C0              xor         eax,eax
  0000000140023618: E9 A5 0F 00 00     jmp         00000001400245C2
  000000014002361D: E9 A0 0F 00 00     jmp         00000001400245C2
  0000000140023622: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002362A: E8 91 16 00 00     call        0000000140024CC0
  000000014002362F: 33 C0              xor         eax,eax
  0000000140023631: E9 8C 0F 00 00     jmp         00000001400245C2
  0000000140023636: E9 87 0F 00 00     jmp         00000001400245C2
  000000014002363B: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023643: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023646: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002364E: FF 90 70 07 00 00  call        qword ptr [rax+0000000000000770h]
  0000000140023654: 33 C0              xor         eax,eax
  0000000140023656: E9 67 0F 00 00     jmp         00000001400245C2
  000000014002365B: E9 62 0F 00 00     jmp         00000001400245C2
  0000000140023660: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023668: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002366B: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023673: FF 90 78 07 00 00  call        qword ptr [rax+0000000000000778h]
  0000000140023679: 33 C0              xor         eax,eax
  000000014002367B: E9 42 0F 00 00     jmp         00000001400245C2
  0000000140023680: E9 3D 0F 00 00     jmp         00000001400245C2
  0000000140023685: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002368D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023690: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023698: FF 90 80 07 00 00  call        qword ptr [rax+0000000000000780h]
  000000014002369E: 33 C0              xor         eax,eax
  00000001400236A0: E9 1D 0F 00 00     jmp         00000001400245C2
  00000001400236A5: E9 18 0F 00 00     jmp         00000001400245C2
  00000001400236AA: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400236B2: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400236B5: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400236BD: FF 90 88 07 00 00  call        qword ptr [rax+0000000000000788h]
  00000001400236C3: 33 C0              xor         eax,eax
  00000001400236C5: E9 F8 0E 00 00     jmp         00000001400245C2
  00000001400236CA: E9 F3 0E 00 00     jmp         00000001400245C2
  00000001400236CF: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400236D7: E8 74 21 C8 00     call        0000000140CA5850
  00000001400236DC: 33 C0              xor         eax,eax
  00000001400236DE: E9 DF 0E 00 00     jmp         00000001400245C2
  00000001400236E3: E9 DA 0E 00 00     jmp         00000001400245C2
  00000001400236E8: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400236F0: E8 5B 25 C8 00     call        0000000140CA5C50
  00000001400236F5: 33 C0              xor         eax,eax
  00000001400236F7: E9 C6 0E 00 00     jmp         00000001400245C2
  00000001400236FC: E9 C1 0E 00 00     jmp         00000001400245C2
  0000000140023701: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023709: E8 C2 27 C8 00     call        0000000140CA5ED0
  000000014002370E: 33 C0              xor         eax,eax
  0000000140023710: E9 AD 0E 00 00     jmp         00000001400245C2
  0000000140023715: E9 A8 0E 00 00     jmp         00000001400245C2
  000000014002371A: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023722: E8 C9 27 C8 00     call        0000000140CA5EF0
  0000000140023727: 33 C0              xor         eax,eax
  0000000140023729: E9 94 0E 00 00     jmp         00000001400245C2
  000000014002372E: E9 8F 0E 00 00     jmp         00000001400245C2
  0000000140023733: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002373B: E8 A0 15 00 00     call        0000000140024CE0
  0000000140023740: 33 C0              xor         eax,eax
  0000000140023742: E9 7B 0E 00 00     jmp         00000001400245C2
  0000000140023747: E9 76 0E 00 00     jmp         00000001400245C2
  000000014002374C: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023754: E8 B7 15 00 00     call        0000000140024D10
  0000000140023759: 33 C0              xor         eax,eax
  000000014002375B: E9 62 0E 00 00     jmp         00000001400245C2
  0000000140023760: E9 5D 0E 00 00     jmp         00000001400245C2
  0000000140023765: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002376D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023770: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023778: FF 90 90 07 00 00  call        qword ptr [rax+0000000000000790h]
  000000014002377E: 33 C0              xor         eax,eax
  0000000140023780: E9 3D 0E 00 00     jmp         00000001400245C2
  0000000140023785: E9 38 0E 00 00     jmp         00000001400245C2
  000000014002378A: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023792: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023795: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002379D: FF 90 98 07 00 00  call        qword ptr [rax+0000000000000798h]
  00000001400237A3: 33 C0              xor         eax,eax
  00000001400237A5: E9 18 0E 00 00     jmp         00000001400245C2
  00000001400237AA: E9 13 0E 00 00     jmp         00000001400245C2
  00000001400237AF: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400237B7: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400237BA: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400237C2: FF 90 A0 07 00 00  call        qword ptr [rax+00000000000007A0h]
  00000001400237C8: 33 C0              xor         eax,eax
  00000001400237CA: E9 F3 0D 00 00     jmp         00000001400245C2
  00000001400237CF: E9 EE 0D 00 00     jmp         00000001400245C2
  00000001400237D4: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400237DC: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400237DF: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400237E7: FF 90 A8 07 00 00  call        qword ptr [rax+00000000000007A8h]
  00000001400237ED: 33 C0              xor         eax,eax
  00000001400237EF: E9 CE 0D 00 00     jmp         00000001400245C2
  00000001400237F4: E9 C9 0D 00 00     jmp         00000001400245C2
  00000001400237F9: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023801: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023804: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  000000014002380C: FF 90 B0 07 00 00  call        qword ptr [rax+00000000000007B0h]
  0000000140023812: 33 C0              xor         eax,eax
  0000000140023814: E9 A9 0D 00 00     jmp         00000001400245C2
  0000000140023819: E9 A4 0D 00 00     jmp         00000001400245C2
  000000014002381E: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023826: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140023829: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  0000000140023831: FF 90 B8 07 00 00  call        qword ptr [rax+00000000000007B8h]
  0000000140023837: 33 C0              xor         eax,eax
  0000000140023839: E9 84 0D 00 00     jmp         00000001400245C2
  000000014002383E: E9 7F 0D 00 00     jmp         00000001400245C2
  0000000140023843: 33 C0              xor         eax,eax
  0000000140023845: 85 C0              test        eax,eax
  0000000140023847: 74 2D              je          0000000140023876
  0000000140023849: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023851: 48 83 C0 08        add         rax,8
  0000000140023855: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002385D: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023865: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140023869: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002386C: 48 89 84 24 E0 07  mov         qword ptr [rsp+00000000000007E0h],rax
                    00 00
  0000000140023874: EB 28              jmp         000000014002389E
  0000000140023876: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  000000014002387E: 48 83 C0 08        add         rax,8
  0000000140023882: 48 89 84 24 60 08  mov         qword ptr [rsp+0000000000000860h],rax
                    00 00
  000000014002388A: 48 8B 84 24 60 08  mov         rax,qword ptr [rsp+0000000000000860h]
                    00 00
  0000000140023892: 48 8B 40 F8        mov         rax,qword ptr [rax-8]
  0000000140023896: 48 89 84 24 E0 07  mov         qword ptr [rsp+00000000000007E0h],rax
                    00 00
  000000014002389E: 48 8B 84 24 E0 07  mov         rax,qword ptr [rsp+00000000000007E0h]
                    00 00
  00000001400238A6: 48 89 84 24 90 02  mov         qword ptr [rsp+0000000000000290h],rax
                    00 00
  00000001400238AE: 48 8B 84 24 50 08  mov         rax,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400238B6: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400238B9: 48 8B 94 24 90 02  mov         rdx,qword ptr [rsp+0000000000000290h]
                    00 00
  00000001400238C1: 48 8B 8C 24 50 08  mov         rcx,qword ptr [rsp+0000000000000850h]
                    00 00
  00000001400238C9: FF 90 C0 07 00 00  call        qword ptr [rax+00000000000007C0h]
  00000001400238CF: 33 C0              xor         eax,eax
  00000001400238D1: E9 EC 0C 00 00     jmp         00000001400245C2
  00000001400238D6: E9 E7 0C 00 00     jmp         00000001400245C2
  00000001400238DB: 48
  00000001400238DC: 8B

  Summary

     1E02000 .text
