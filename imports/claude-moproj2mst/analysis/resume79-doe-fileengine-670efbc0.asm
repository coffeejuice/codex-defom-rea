Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670EFBC0: 44 88 4C 24 20     mov         byte ptr [rsp+20h],r9b
  00000000670EFBC5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000000670EFBCA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670EFBCF: 53                 push        rbx
  00000000670EFBD0: 55                 push        rbp
  00000000670EFBD1: 56                 push        rsi
  00000000670EFBD2: 57                 push        rdi
  00000000670EFBD3: 48 81 EC B8 00 00  sub         rsp,0B8h
                    00
  00000000670EFBDA: 48 C7 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000000670EFBE6: 41 0F B6 D9        movzx       ebx,r9b
  00000000670EFBEA: 49 8B E8           mov         rbp,r8
  00000000670EFBED: 48 8B F9           mov         rdi,rcx
  00000000670EFBF0: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670EFBF5: E8 46 F4 FF FF     call        00000000670EF040
  00000000670EFBFA: 90                 nop
  00000000670EFBFB: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670EFC00: E8 CB 81 FB FF     call        00000000670A7DD0
  00000000670EFC05: 48 89 45 00        mov         qword ptr [rbp],rax
  00000000670EFC09: 48 85 C0           test        rax,rax
  00000000670EFC0C: 74 68              je          00000000670EFC76
  00000000670EFC0E: 84 DB              test        bl,bl
  00000000670EFC10: 74 46              je          00000000670EFC58
  00000000670EFC12: C7 44 24 44 00 00  mov         dword ptr [rsp+44h],0FF00000h
                    F0 0F
  00000000670EFC1A: 4C 8B 08           mov         r9,qword ptr [rax]
  00000000670EFC1D: 4C 8D 44 24 44     lea         r8,[rsp+44h]
  00000000670EFC22: 48 8D 54 24 6C     lea         rdx,[rsp+6Ch]
  00000000670EFC27: 48 8B C8           mov         rcx,rax
  00000000670EFC2A: 41 FF 91 90 00 00  call        qword ptr [r9+0000000000000090h]
                    00
  00000000670EFC31: 8B 08              mov         ecx,dword ptr [rax]
  00000000670EFC33: 81 E1 00 00 40 00  and         ecx,400000h
  00000000670EFC39: 75 1D              jne         00000000670EFC58
  00000000670EFC3B: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000670EFC3F: 48 85 C9           test        rcx,rcx
  00000000670EFC42: 74 0A              je          00000000670EFC4E
  00000000670EFC44: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670EFC47: BA 01 00 00 00     mov         edx,1
  00000000670EFC4C: FF 10              call        qword ptr [rax]
  00000000670EFC4E: 33 F6              xor         esi,esi
  00000000670EFC50: 48 89 75 00        mov         qword ptr [rbp],rsi
  00000000670EFC54: 32 DB              xor         bl,bl
  00000000670EFC56: EB 02              jmp         00000000670EFC5A
  00000000670EFC58: B3 01              mov         bl,1
  00000000670EFC5A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670EFC5F: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670EFC63: 75 0A              jne         00000000670EFC6F
  00000000670EFC65: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670EFC6A: E8 61 04 F8 FF     call        00000000670700D0
  00000000670EFC6F: 8A C3              mov         al,bl
  00000000670EFC71: E9 F4 03 00 00     jmp         00000000670F006A
  00000000670EFC76: 33 F6              xor         esi,esi
  00000000670EFC78: 8B DE              mov         ebx,esi
  00000000670EFC7A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670EFC7F: 44 8B 41 08        mov         r8d,dword ptr [rcx+8]
  00000000670EFC83: 45 85 C0           test        r8d,r8d
  00000000670EFC86: 0F 8E 80 03 00 00  jle         00000000670F000C
  00000000670EFC8C: 48 8B 51 10        mov         rdx,qword ptr [rcx+10h]
  00000000670EFC90: 0F B7 02           movzx       eax,word ptr [rdx]
  00000000670EFC93: 66 83 F8 2F        cmp         ax,2Fh
  00000000670EFC97: 0F 84 6F 03 00 00  je          00000000670F000C
  00000000670EFC9D: 66 83 F8 3A        cmp         ax,3Ah
  00000000670EFCA1: 74 10              je          00000000670EFCB3
  00000000670EFCA3: FF C3              inc         ebx
  00000000670EFCA5: 48 83 C2 02        add         rdx,2
  00000000670EFCA9: 41 3B D8           cmp         ebx,r8d
  00000000670EFCAC: 7C E2              jl          00000000670EFC90
  00000000670EFCAE: E9 59 03 00 00     jmp         00000000670F000C
  00000000670EFCB3: 85 DB              test        ebx,ebx
  00000000670EFCB5: 0F 85 96 00 00 00  jne         00000000670EFD51
  00000000670EFCBB: 8D 4B 10           lea         ecx,[rbx+10h]
  00000000670EFCBE: E8 C3 8D 08 00     call        0000000067178A86
  00000000670EFCC3: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000000670EFCCB: 48 85 C0           test        rax,rax
  00000000670EFCCE: 74 12              je          00000000670EFCE2
  00000000670EFCD0: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670EFCD5: 48 8B C8           mov         rcx,rax
  00000000670EFCD8: E8 C3 CC FD FF     call        00000000670CC9A0
  00000000670EFCDD: 48 8B C8           mov         rcx,rax
  00000000670EFCE0: EB 03              jmp         00000000670EFCE5
  00000000670EFCE2: 48 8B CE           mov         rcx,rsi
  00000000670EFCE5: 48 89 4D 00        mov         qword ptr [rbp],rcx
  00000000670EFCE9: 80 BC 24 F8 00 00  cmp         byte ptr [rsp+00000000000000F8h],0
                    00 00
  00000000670EFCF1: 74 40              je          00000000670EFD33
  00000000670EFCF3: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0FF00000h
                    F0 0F
  00000000670EFCFB: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670EFCFE: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  00000000670EFD03: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000000670EFD08: FF 90 90 00 00 00  call        qword ptr [rax+0000000000000090h]
  00000000670EFD0E: 8B 08              mov         ecx,dword ptr [rax]
  00000000670EFD10: 81 E1 00 00 40 00  and         ecx,400000h
  00000000670EFD16: 75 1B              jne         00000000670EFD33
  00000000670EFD18: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  00000000670EFD1C: 48 85 C9           test        rcx,rcx
  00000000670EFD1F: 74 0A              je          00000000670EFD2B
  00000000670EFD21: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670EFD24: BA 01 00 00 00     mov         edx,1
  00000000670EFD29: FF 10              call        qword ptr [rax]
  00000000670EFD2B: 48 89 75 00        mov         qword ptr [rbp],rsi
  00000000670EFD2F: 32 DB              xor         bl,bl
  00000000670EFD31: EB 02              jmp         00000000670EFD35
  00000000670EFD33: B3 01              mov         bl,1
  00000000670EFD35: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670EFD3A: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670EFD3E: 75 0A              jne         00000000670EFD4A
  00000000670EFD40: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670EFD45: E8 86 03 F8 FF     call        00000000670700D0
  00000000670EFD4A: 8A C3              mov         al,bl
  00000000670EFD4C: E9 19 03 00 00     jmp         00000000670F006A
  00000000670EFD51: 83 FB 01           cmp         ebx,1
  00000000670EFD54: 0F 84 B2 02 00 00  je          00000000670F000C
  00000000670EFD5A: 44 8B C3           mov         r8d,ebx
  00000000670EFD5D: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000000670EFD62: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670EFD67: E8 84 41 F8 FF     call        0000000067073EF0
  00000000670EFD6C: 90                 nop
  00000000670EFD6D: 48 8B D0           mov         rdx,rax
  00000000670EFD70: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670EFD75: E8 F6 18 FC FF     call        00000000670B1670
  00000000670EFD7A: 90                 nop
  00000000670EFD7B: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000000670EFD80: 83 CF FF           or          edi,0FFFFFFFFh
  00000000670EFD83: F0 01 39           lock add    dword ptr [rcx],edi
  00000000670EFD86: 75 0A              jne         00000000670EFD92
  00000000670EFD88: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000000670EFD8D: E8 3E 03 F8 FF     call        00000000670700D0
  00000000670EFD92: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670EFD97: 8B 48 0C           mov         ecx,dword ptr [rax+0Ch]
  00000000670EFD9A: 2B 48 08           sub         ecx,dword ptr [rax+8]
  00000000670EFD9D: 85 C9              test        ecx,ecx
  00000000670EFD9F: 0F 8E FD 01 00 00  jle         00000000670EFFA2
  00000000670EFDA5: 8D 4B 01           lea         ecx,[rbx+1]
  00000000670EFDA8: 89 4C 24 3C        mov         dword ptr [rsp+3Ch],ecx
  00000000670EFDAC: 48 8D 84 24 88 00  lea         rax,[rsp+0000000000000088h]
                    00 00
  00000000670EFDB4: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000000670EFDBC: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000000670EFDC4: 48 83 C0 08        add         rax,8
  00000000670EFDC8: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000000670EFDCD: 0F 1F 00           nop         dword ptr [rax]
  00000000670EFDD0: 44 8B CF           mov         r9d,edi
  00000000670EFDD3: 44 8B C1           mov         r8d,ecx
  00000000670EFDD6: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000670EFDDB: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670EFDE0: E8 BB 41 F8 FF     call        0000000067073FA0
  00000000670EFDE5: 90                 nop
  00000000670EFDE6: C6 84 24 F0 00 00  mov         byte ptr [rsp+00000000000000F0h],2Fh
                    00 2F
  00000000670EFDEE: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670EFDF3: 4C 63 41 08        movsxd      r8,dword ptr [rcx+8]
  00000000670EFDF7: 48 63 D6           movsxd      rdx,esi
  00000000670EFDFA: 4C 03 C2           add         r8,rdx
  00000000670EFDFD: 4A 8D 54 C1 18     lea         rdx,[rcx+r8*8+18h]
  00000000670EFE02: 48 89 94 24 88 00  mov         qword ptr [rsp+0000000000000088h],rdx
                    00 00
  00000000670EFE0A: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  00000000670EFE12: 48 89 8C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rcx
                    00 00
  00000000670EFE1A: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000000670EFE22: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000670EFE27: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000000670EFE2F: E8 FC EC F2 FF     call        000000006701EB30
  00000000670EFE34: 90                 nop
  00000000670EFE35: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000670EFE3A: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000670EFE3F: E8 DC C7 FB FF     call        00000000670AC620
  00000000670EFE44: 90                 nop
  00000000670EFE45: 48 8B D0           mov         rdx,rax
  00000000670EFE48: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000000670EFE4D: E8 BE EE FF FF     call        00000000670EED10
  00000000670EFE52: 48 8B D8           mov         rbx,rax
  00000000670EFE55: 48 8B D0           mov         rdx,rax
  00000000670EFE58: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000000670EFE60: E8 6B 05 F8 FF     call        00000000670703D0
  00000000670EFE65: 48 8D 53 08        lea         rdx,[rbx+8]
  00000000670EFE69: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000000670EFE6E: E8 5D 05 F8 FF     call        00000000670703D0
  00000000670EFE73: 44 0F B7 5B 10     movzx       r11d,word ptr [rbx+10h]
  00000000670EFE78: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000000670EFE80: 66 44 89 5A 10     mov         word ptr [rdx+10h],r11w
  00000000670EFE85: 0F B7 4B 12        movzx       ecx,word ptr [rbx+12h]
  00000000670EFE89: 66 89 4A 12        mov         word ptr [rdx+12h],cx
  00000000670EFE8D: 0F B7 43 14        movzx       eax,word ptr [rbx+14h]
  00000000670EFE91: 66 89 42 14        mov         word ptr [rdx+14h],ax
  00000000670EFE95: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000000670EFE9A: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EFE9E: 75 0B              jne         00000000670EFEAB
  00000000670EFEA0: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000000670EFEA5: E8 26 02 F8 FF     call        00000000670700D0
  00000000670EFEAA: 90                 nop
  00000000670EFEAB: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  00000000670EFEB0: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EFEB4: 75 0B              jne         00000000670EFEC1
  00000000670EFEB6: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000000670EFEBB: E8 10 02 F8 FF     call        00000000670700D0
  00000000670EFEC0: 90                 nop
  00000000670EFEC1: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000000670EFEC6: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EFECA: 75 0B              jne         00000000670EFED7
  00000000670EFECC: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000000670EFED1: E8 FA 01 F8 FF     call        00000000670700D0
  00000000670EFED6: 90                 nop
  00000000670EFED7: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000000670EFEDC: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EFEE0: 75 0B              jne         00000000670EFEED
  00000000670EFEE2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670EFEE7: E8 E4 01 F8 FF     call        00000000670700D0
  00000000670EFEEC: 90                 nop
  00000000670EFEED: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000000670EFEF2: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EFEF6: 75 0A              jne         00000000670EFF02
  00000000670EFEF8: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000670EFEFD: E8 CE 01 F8 FF     call        00000000670700D0
  00000000670EFF02: 41 B1 01           mov         r9b,1
  00000000670EFF05: 4C 8B C5           mov         r8,rbp
  00000000670EFF08: 48 8B 94 24 E8 00  mov         rdx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000000670EFF10: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000000670EFF18: E8 A3 FC FF FF     call        00000000670EFBC0
  00000000670EFF1D: 84 C0              test        al,al
  00000000670EFF1F: 75 1B              jne         00000000670EFF3C
  00000000670EFF21: FF C6              inc         esi
  00000000670EFF23: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670EFF28: 8B 48 0C           mov         ecx,dword ptr [rax+0Ch]
  00000000670EFF2B: 2B 48 08           sub         ecx,dword ptr [rax+8]
  00000000670EFF2E: 3B F1              cmp         esi,ecx
  00000000670EFF30: 8B 4C 24 3C        mov         ecx,dword ptr [rsp+3Ch]
  00000000670EFF34: 0F 8C 96 FE FF FF  jl          00000000670EFDD0
  00000000670EFF3A: EB 66              jmp         00000000670EFFA2
  00000000670EFF3C: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670EFF41: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EFF45: 75 3F              jne         00000000670EFF86
  00000000670EFF47: 48 8B 6C 24 28     mov         rbp,qword ptr [rsp+28h]
  00000000670EFF4C: 48 63 45 0C        movsxd      rax,dword ptr [rbp+0Ch]
  00000000670EFF50: 48 8D 5C C5 18     lea         rbx,[rbp+rax*8+18h]
  00000000670EFF55: 48 63 45 08        movsxd      rax,dword ptr [rbp+8]
  00000000670EFF59: 48 8D 74 C5 18     lea         rsi,[rbp+rax*8+18h]
  00000000670EFF5E: 48 3B F3           cmp         rsi,rbx
  00000000670EFF61: 74 1A              je          00000000670EFF7D
  00000000670EFF63: 48 83 EB 08        sub         rbx,8
  00000000670EFF67: 48 8B 13           mov         rdx,qword ptr [rbx]
  00000000670EFF6A: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  00000000670EFF6E: 75 08              jne         00000000670EFF78
  00000000670EFF70: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670EFF73: E8 58 01 F8 FF     call        00000000670700D0
  00000000670EFF78: 48 3B DE           cmp         rbx,rsi
  00000000670EFF7B: 75 E6              jne         00000000670EFF63
  00000000670EFF7D: 48 8B CD           mov         rcx,rbp
  00000000670EFF80: E8 2B 04 F3 FF     call        00000000670203B0
  00000000670EFF85: 90                 nop
  00000000670EFF86: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670EFF8B: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EFF8F: 75 0A              jne         00000000670EFF9B
  00000000670EFF91: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670EFF96: E8 35 01 F8 FF     call        00000000670700D0
  00000000670EFF9B: B0 01              mov         al,1
  00000000670EFF9D: E9 C8 00 00 00     jmp         00000000670F006A
  00000000670EFFA2: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EFFA6: 75 4B              jne         00000000670EFFF3
  00000000670EFFA8: 48 8B 6C 24 28     mov         rbp,qword ptr [rsp+28h]
  00000000670EFFAD: 48 63 45 0C        movsxd      rax,dword ptr [rbp+0Ch]
  00000000670EFFB1: 48 8D 5C C5 18     lea         rbx,[rbp+rax*8+18h]
  00000000670EFFB6: 48 63 45 08        movsxd      rax,dword ptr [rbp+8]
  00000000670EFFBA: 48 8D 74 C5 18     lea         rsi,[rbp+rax*8+18h]
  00000000670EFFBF: 48 3B F3           cmp         rsi,rbx
  00000000670EFFC2: 74 26              je          00000000670EFFEA
  00000000670EFFC4: 66 66 66 66 0F 1F  nop         word ptr [rax+rax+0000000000000000h]
                    84 00 00 00 00 00
  00000000670EFFD0: 48 83 EB 08        sub         rbx,8
  00000000670EFFD4: 48 8B 13           mov         rdx,qword ptr [rbx]
  00000000670EFFD7: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  00000000670EFFDB: 75 08              jne         00000000670EFFE5
  00000000670EFFDD: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670EFFE0: E8 EB 00 F8 FF     call        00000000670700D0
  00000000670EFFE5: 48 3B DE           cmp         rbx,rsi
  00000000670EFFE8: 75 E6              jne         00000000670EFFD0
  00000000670EFFEA: 48 8B CD           mov         rcx,rbp
  00000000670EFFED: E8 BE 03 F3 FF     call        00000000670203B0
  00000000670EFFF2: 90                 nop
  00000000670EFFF3: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670EFFF8: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EFFFC: 75 0A              jne         00000000670F0008
  00000000670EFFFE: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670F0003: E8 C8 00 F8 FF     call        00000000670700D0
  00000000670F0008: 32 C0              xor         al,al
  00000000670F000A: EB 5E              jmp         00000000670F006A
  00000000670F000C: 40 38 B4 24 F8 00  cmp         byte ptr [rsp+00000000000000F8h],sil
                    00 00
  00000000670F0014: 74 40              je          00000000670F0056
  00000000670F0016: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],400000h
                    40 00
  00000000670F001E: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  00000000670F0023: 48 8B 9C 24 E8 00  mov         rbx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000000670F002B: 48 8B D3           mov         rdx,rbx
  00000000670F002E: 48 8B CF           mov         rcx,rdi
  00000000670F0031: E8 5A 20 01 00     call        0000000067102090
  00000000670F0036: 84 C0              test        al,al
  00000000670F0038: 74 11              je          00000000670F004B
  00000000670F003A: 8B 43 04           mov         eax,dword ptr [rbx+4]
  00000000670F003D: 25 00 00 40 00     and         eax,400000h
  00000000670F0042: 74 07              je          00000000670F004B
  00000000670F0044: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670F0049: EB 0B              jmp         00000000670F0056
  00000000670F004B: 89 33              mov         dword ptr [rbx],esi
  00000000670F004D: 32 DB              xor         bl,bl
  00000000670F004F: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670F0054: EB 02              jmp         00000000670F0058
  00000000670F0056: B3 01              mov         bl,1
  00000000670F0058: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670F005C: 75 0A              jne         00000000670F0068
  00000000670F005E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670F0063: E8 68 00 F8 FF     call        00000000670700D0
  00000000670F0068: 8A C3              mov         al,bl
  00000000670F006A: 48 81 C4 B8 00 00  add         rsp,0B8h
                    00
  00000000670F0071: 5F                 pop         rdi
  00000000670F0072: 5E                 pop         rsi
  00000000670F0073: 5D                 pop         rbp
  00000000670F0074: 5B                 pop         rbx
  00000000670F0075: C3                 ret

  Summary

      1A7000 .text
