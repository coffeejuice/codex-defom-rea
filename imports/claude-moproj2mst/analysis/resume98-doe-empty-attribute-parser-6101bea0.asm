Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  000000006101BEA0: 40 53              push        rbx
  000000006101BEA2: 55                 push        rbp
  000000006101BEA3: 56                 push        rsi
  000000006101BEA4: 57                 push        rdi
  000000006101BEA5: 41 54              push        r12
  000000006101BEA7: 41 55              push        r13
  000000006101BEA9: 41 56              push        r14
  000000006101BEAB: 41 57              push        r15
  000000006101BEAD: 48 81 EC A8 00 00  sub         rsp,0A8h
                    00
  000000006101BEB4: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000006101BEC0: 48 8B E9           mov         rbp,rcx
  000000006101BEC3: 45 33 F6           xor         r14d,r14d
  000000006101BEC6: 44 89 B4 24 F0 00  mov         dword ptr [rsp+00000000000000F0h],r14d
                    00 00
  000000006101BECE: 48 8B 89 D8 00 00  mov         rcx,qword ptr [rcx+00000000000000D8h]
                    00
  000000006101BED5: 48 85 C9           test        rcx,rcx
  000000006101BED8: 74 74              je          000000006101BF4E
  000000006101BEDA: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006101BEDD: 44 39 70 08        cmp         dword ptr [rax+8],r14d
  000000006101BEE1: 74 6B              je          000000006101BF4E
  000000006101BEE3: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000006101BEEB: E8 30 90 FF FF     call        0000000061014F20
  000000006101BEF0: 44 8B 60 08        mov         r12d,dword ptr [rax+8]
  000000006101BEF4: 48 8B 8D D8 00 00  mov         rcx,qword ptr [rbp+00000000000000D8h]
                    00
  000000006101BEFB: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006101BEFE: 44 39 70 08        cmp         dword ptr [rax+8],r14d
  000000006101BF02: 74 4D              je          000000006101BF51
  000000006101BF04: E8 C7 90 FF FF     call        0000000061014FD0
  000000006101BF09: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006101BF0C: 48 8D 05 5D B1 FF  lea         rax,[0000000061017070h]
                    FF
  000000006101BF13: 48 3B D8           cmp         rbx,rax
  000000006101BF16: 75 14              jne         000000006101BF2C
  000000006101BF18: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000006101BF20: 48 8B 8D D8 00 00  mov         rcx,qword ptr [rbp+00000000000000D8h]
                    00
  000000006101BF27: E8 F4 8F FF FF     call        0000000061014F20
  000000006101BF2C: 48 8B CD           mov         rcx,rbp
  000000006101BF2F: FF D3              call        rbx
  000000006101BF31: 84 C0              test        al,al
  000000006101BF33: 75 1C              jne         000000006101BF51
  000000006101BF35: 45 8B C4           mov         r8d,r12d
  000000006101BF38: 48 8D 15 61 FF FF  lea         rdx,[000000006101BEA0h]
                    FF
  000000006101BF3F: 48 8B CD           mov         rcx,rbp
  000000006101BF42: E8 69 A5 FF FF     call        00000000610164B0
  000000006101BF47: 32 C0              xor         al,al
  000000006101BF49: E9 08 06 00 00     jmp         000000006101C556
  000000006101BF4E: 45 8B E6           mov         r12d,r14d
  000000006101BF51: 41 83 FC FF        cmp         r12d,0FFFFFFFFh
  000000006101BF55: 0F 84 BC 05 00 00  je          000000006101C517
  000000006101BF5B: 41 BF FF FF 00 00  mov         r15d,0FFFFh
  000000006101BF61: 4C 8D 2D 98 40 FE  lea         r13,[0000000061000000h]
                    FF
  000000006101BF68: 41 83 FC 01        cmp         r12d,1
  000000006101BF6C: 74 4B              je          000000006101BFB9
  000000006101BF6E: 41 83 FC 06        cmp         r12d,6
  000000006101BF72: 74 30              je          000000006101BFA4
  000000006101BF74: 41 83 FC 09        cmp         r12d,9
  000000006101BF78: 74 0F              je          000000006101BF89
  000000006101BF7A: 41 83 FC 0C        cmp         r12d,0Ch
  000000006101BF7E: 0F 84 2C 04 00 00  je          000000006101C3B0
  000000006101BF84: E9 C3 00 00 00     jmp         000000006101C04C
  000000006101BF89: 48 8B CD           mov         rcx,rbp
  000000006101BF8C: E8 EF 86 FF FF     call        0000000061014680
  000000006101BF91: 84 C0              test        al,al
  000000006101BF93: 0F 84 1E 04 00 00  je          000000006101C3B7
  000000006101BF99: 41 BC 0A 00 00 00  mov         r12d,0Ah
  000000006101BF9F: E9 A8 00 00 00     jmp         000000006101C04C
  000000006101BFA4: 48 8B CD           mov         rcx,rbp
  000000006101BFA7: E8 E4 BB FF FF     call        0000000061017B90
  000000006101BFAC: 84 C0              test        al,al
  000000006101BFAE: 0F 84 0A 04 00 00  je          000000006101C3BE
  000000006101BFB4: E9 93 00 00 00     jmp         000000006101C04C
  000000006101BFB9: 8B 95 3C 03 00 00  mov         edx,dword ptr [rbp+000000000000033Ch]
  000000006101BFBF: 03 95 38 03 00 00  add         edx,dword ptr [rbp+0000000000000338h]
  000000006101BFC5: 48 8D 8D 30 03 00  lea         rcx,[rbp+0000000000000330h]
                    00
  000000006101BFCC: FF 15 7E 72 02 00  call        qword ptr [0000000061043250h]
  000000006101BFD2: 48 63 BD 38 03 00  movsxd      rdi,dword ptr [rbp+0000000000000338h]
                    00
  000000006101BFD9: 48 03 FF           add         rdi,rdi
  000000006101BFDC: 48 8D 8D 30 03 00  lea         rcx,[rbp+0000000000000330h]
                    00
  000000006101BFE3: FF 15 2F 72 02 00  call        qword ptr [0000000061043218h]
  000000006101BFE9: 48 63 8D 3C 03 00  movsxd      rcx,dword ptr [rbp+000000000000033Ch]
                    00
  000000006101BFF0: 48 8D 0C 48        lea         rcx,[rax+rcx*2]
  000000006101BFF4: 4C 8B C7           mov         r8,rdi
  000000006101BFF7: 48 8D 95 2C 01 00  lea         rdx,[rbp+000000000000012Ch]
                    00
  000000006101BFFE: E8 3D A8 01 00     call        0000000061036840
  000000006101C003: 44 8B 9D 38 03 00  mov         r11d,dword ptr [rbp+0000000000000338h]
                    00
  000000006101C00A: 44 01 9D 3C 03 00  add         dword ptr [rbp+000000000000033Ch],r11d
                    00
  000000006101C011: 44 89 B5 38 03 00  mov         dword ptr [rbp+0000000000000338h],r14d
                    00
  000000006101C018: 48 8D 95 30 03 00  lea         rdx,[rbp+0000000000000330h]
                    00
  000000006101C01F: 48 8B CD           mov         rcx,rbp
  000000006101C022: E8 E9 6A FF FF     call        0000000061012B10
  000000006101C027: 48 8D 8D 90 00 00  lea         rcx,[rbp+0000000000000090h]
                    00
  000000006101C02E: E8 2D 80 FF FF     call        0000000061014060
  000000006101C033: 80 BD 80 00 00 00  cmp         byte ptr [rbp+0000000000000080h],0
                    00
  000000006101C03A: 74 10              je          000000006101C04C
  000000006101C03C: 48 8B 8D B0 00 00  mov         rcx,qword ptr [rbp+00000000000000B0h]
                    00
  000000006101C043: 48 8D 51 08        lea         rdx,[rcx+8]
  000000006101C047: E8 A4 84 FF FF     call        00000000610144F0
  000000006101C04C: 48 8D 8D 20 01 00  lea         rcx,[rbp+0000000000000120h]
                    00
  000000006101C053: FF 15 6F 78 02 00  call        qword ptr [00000000610438C8h]
  000000006101C059: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101C05C: 66 83 C9 01        or          cx,1
  000000006101C060: 66 41 3B CF        cmp         cx,r15w
  000000006101C064: 0F 84 97 04 00 00  je          000000006101C501
  000000006101C06A: 0F B7 85 20 01 00  movzx       eax,word ptr [rbp+0000000000000120h]
                    00
  000000006101C071: 66 89 84 24 F0 00  mov         word ptr [rsp+00000000000000F0h],ax
                    00 00
  000000006101C079: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000006101C081: FF 15 41 78 02 00  call        qword ptr [00000000610438C8h]
  000000006101C087: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101C08A: F7 C1 80 FF FF FF  test        ecx,0FFFFFF80h
  000000006101C090: 75 15              jne         000000006101C0A7
  000000006101C092: 0F B7 C1           movzx       eax,cx
  000000006101C095: 42 0F BE 84 28 70  movsx       eax,byte ptr [rax+r13+0000000000046870h]
                    68 04 00
  000000006101C09E: 85 C0              test        eax,eax
  000000006101C0A0: 75 2A              jne         000000006101C0CC
  000000006101C0A2: E9 0C 01 00 00     jmp         000000006101C1B3
  000000006101C0A7: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000006101C0AF: FF 15 03 73 02 00  call        qword ptr [00000000610433B8h]
  000000006101C0B5: 83 F8 0F           cmp         eax,0Fh
  000000006101C0B8: 7C 09              jl          000000006101C0C3
  000000006101C0BA: 83 F8 13           cmp         eax,13h
  000000006101C0BD: 0F 8E F0 00 00 00  jle         000000006101C1B3
  000000006101C0C3: 83 F8 05           cmp         eax,5
  000000006101C0C6: 0F 84 E7 00 00 00  je          000000006101C1B3
  000000006101C0CC: B2 3E              mov         dl,3Eh
  000000006101C0CE: 48 8D 4C 24 4A     lea         rcx,[rsp+4Ah]
  000000006101C0D3: FF 15 AF 77 02 00  call        qword ptr [0000000061043888h]
  000000006101C0D9: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101C0DC: 66 89 8C 24 F8 00  mov         word ptr [rsp+00000000000000F8h],cx
                    00 00
  000000006101C0E4: 0F B7 85 20 01 00  movzx       eax,word ptr [rbp+0000000000000120h]
                    00
  000000006101C0EB: 66 89 84 24 00 01  mov         word ptr [rsp+0000000000000100h],ax
                    00 00
  000000006101C0F3: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000006101C0FB: FF 15 C7 77 02 00  call        qword ptr [00000000610438C8h]
  000000006101C101: 48 8B D8           mov         rbx,rax
  000000006101C104: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  000000006101C10C: FF 15 B6 77 02 00  call        qword ptr [00000000610438C8h]
  000000006101C112: 0F B7 0B           movzx       ecx,word ptr [rbx]
  000000006101C115: 66 39 08           cmp         word ptr [rax],cx
  000000006101C118: 75 0A              jne         000000006101C124
  000000006101C11A: BA 02 00 00 00     mov         edx,2
  000000006101C11F: E9 94 00 00 00     jmp         000000006101C1B8
  000000006101C124: 0F B7 85 20 01 00  movzx       eax,word ptr [rbp+0000000000000120h]
                    00
  000000006101C12B: 66 89 84 24 F0 00  mov         word ptr [rsp+00000000000000F0h],ax
                    00 00
  000000006101C133: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000006101C13B: FF 15 87 77 02 00  call        qword ptr [00000000610438C8h]
  000000006101C141: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101C144: 66 83 F9 20        cmp         cx,20h
  000000006101C148: 74 64              je          000000006101C1AE
  000000006101C14A: 66 83 F9 09        cmp         cx,9
  000000006101C14E: 74 5E              je          000000006101C1AE
  000000006101C150: 66 83 F9 0A        cmp         cx,0Ah
  000000006101C154: 74 58              je          000000006101C1AE
  000000006101C156: 66 83 F9 0D        cmp         cx,0Dh
  000000006101C15A: 74 52              je          000000006101C1AE
  000000006101C15C: B2 2F              mov         dl,2Fh
  000000006101C15E: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006101C163: FF 15 1F 77 02 00  call        qword ptr [0000000061043888h]
  000000006101C169: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101C16C: 66 89 8C 24 08 01  mov         word ptr [rsp+0000000000000108h],cx
                    00 00
  000000006101C174: 0F B7 85 20 01 00  movzx       eax,word ptr [rbp+0000000000000120h]
                    00
  000000006101C17B: 66 89 44 24 30     mov         word ptr [rsp+30h],ax
  000000006101C180: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  000000006101C188: FF 15 3A 77 02 00  call        qword ptr [00000000610438C8h]
  000000006101C18E: 48 8B D8           mov         rbx,rax
  000000006101C191: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006101C196: FF 15 2C 77 02 00  call        qword ptr [00000000610438C8h]
  000000006101C19C: 49 8B D6           mov         rdx,r14
  000000006101C19F: 0F B7 0B           movzx       ecx,word ptr [rbx]
  000000006101C1A2: 66 39 08           cmp         word ptr [rax],cx
  000000006101C1A5: 0F 95 C2           setne       dl
  000000006101C1A8: 48 83 C2 03        add         rdx,3
  000000006101C1AC: EB 0A              jmp         000000006101C1B8
  000000006101C1AE: 49 8B D6           mov         rdx,r14
  000000006101C1B1: EB 05              jmp         000000006101C1B8
  000000006101C1B3: BA 01 00 00 00     mov         edx,1
  000000006101C1B8: 49 63 CC           movsxd      rcx,r12d
  000000006101C1BB: 48 8D 04 8A        lea         rax,[rdx+rcx*4]
  000000006101C1BF: 48 03 C8           add         rcx,rax
  000000006101C1C2: 45 8B A4 8D E0 80  mov         r12d,dword ptr [r13+rcx*4+00000000000480E0h]
                    04 00
  000000006101C1CA: 41 8D 44 24 FF     lea         eax,[r12-1]
  000000006101C1CF: 83 F8 0B           cmp         eax,0Bh
  000000006101C1D2: 0F 87 C9 01 00 00  ja          000000006101C3A1
  000000006101C1D8: 48 98              cdqe
  000000006101C1DA: 41 8B 8C 85 6C C5  mov         ecx,dword ptr [r13+rax*4+000000000001C56Ch]
                    01 00
  000000006101C1E2: 49 03 CD           add         rcx,r13
  000000006101C1E5: FF E1              jmp         rcx
  000000006101C1E7: C6 85 C1 00 00 00  mov         byte ptr [rbp+00000000000000C1h],0
                    00
  000000006101C1EE: 48 8B CD           mov         rcx,rbp
  000000006101C1F1: E8 7A C2 FF FF     call        0000000061018470
  000000006101C1F6: 84 C0              test        al,al
  000000006101C1F8: 0F 84 C7 01 00 00  je          000000006101C3C5
  000000006101C1FE: E9 9E 01 00 00     jmp         000000006101C3A1
  000000006101C203: 48 8B CD           mov         rcx,rbp
  000000006101C206: E8 65 AE FF FF     call        0000000061017070
  000000006101C20B: 84 C0              test        al,al
  000000006101C20D: 0F 84 CB 01 00 00  je          000000006101C3DE
  000000006101C213: E9 89 01 00 00     jmp         000000006101C3A1
  000000006101C218: 48 83 BD E8 00 00  cmp         qword ptr [rbp+00000000000000E8h],0
                    00 00
  000000006101C220: 0F 84 FA 00 00 00  je          000000006101C320
  000000006101C226: 48 8B CD           mov         rcx,rbp
  000000006101C229: E8 82 8B FF FF     call        0000000061014DB0
  000000006101C22E: 48 8B F8           mov         rdi,rax
  000000006101C231: 80 BD 80 00 00 00  cmp         byte ptr [rbp+0000000000000080h],0
                    00
  000000006101C238: 0F 84 81 00 00 00  je          000000006101C2BF
  000000006101C23E: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006101C243: FF 15 87 76 02 00  call        qword ptr [00000000610438D0h]
  000000006101C249: 90                 nop
  000000006101C24A: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006101C24F: FF 15 7B 76 02 00  call        qword ptr [00000000610438D0h]
  000000006101C255: 90                 nop
  000000006101C256: 48 8D 8D B0 00 00  lea         rcx,[rbp+00000000000000B0h]
                    00
  000000006101C25D: 48 8D 44 24 40     lea         rax,[rsp+40h]
  000000006101C262: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006101C267: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  000000006101C26C: 45 33 C0           xor         r8d,r8d
  000000006101C26F: 48 8B D7           mov         rdx,rdi
  000000006101C272: E8 49 79 FF FF     call        0000000061013BC0
  000000006101C277: 48 8B 8D E8 00 00  mov         rcx,qword ptr [rbp+00000000000000E8h]
                    00
  000000006101C27E: 4C 8B 11           mov         r10,qword ptr [rcx]
  000000006101C281: 48 8D 85 88 00 00  lea         rax,[rbp+0000000000000088h]
                    00
  000000006101C288: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006101C28D: 4C 8B CF           mov         r9,rdi
  000000006101C290: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  000000006101C295: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006101C29A: 41 FF 52 30        call        qword ptr [r10+30h]
  000000006101C29E: 84 C0              test        al,al
  000000006101C2A0: 0F 84 51 01 00 00  je          000000006101C3F7
  000000006101C2A6: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006101C2AB: FF 15 2F 76 02 00  call        qword ptr [00000000610438E0h]
  000000006101C2B1: 90                 nop
  000000006101C2B2: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006101C2B7: FF 15 23 76 02 00  call        qword ptr [00000000610438E0h]
  000000006101C2BD: EB 61              jmp         000000006101C320
  000000006101C2BF: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000006101C2C4: FF 15 06 76 02 00  call        qword ptr [00000000610438D0h]
  000000006101C2CA: 48 8B D8           mov         rbx,rax
  000000006101C2CD: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000006101C2D2: FF 15 F8 75 02 00  call        qword ptr [00000000610438D0h]
  000000006101C2D8: 90                 nop
  000000006101C2D9: 48 8B 8D E8 00 00  mov         rcx,qword ptr [rbp+00000000000000E8h]
                    00
  000000006101C2E0: 4C 8B 11           mov         r10,qword ptr [rcx]
  000000006101C2E3: 48 8D 95 88 00 00  lea         rdx,[rbp+0000000000000088h]
                    00
  000000006101C2EA: 48 89 54 24 20     mov         qword ptr [rsp+20h],rdx
  000000006101C2EF: 4C 8B CF           mov         r9,rdi
  000000006101C2F2: 4C 8B C3           mov         r8,rbx
  000000006101C2F5: 48 8B D0           mov         rdx,rax
  000000006101C2F8: 41 FF 52 30        call        qword ptr [r10+30h]
  000000006101C2FC: 84 C0              test        al,al
  000000006101C2FE: 0F 94 C3           sete        bl
  000000006101C301: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000006101C306: FF 15 D4 75 02 00  call        qword ptr [00000000610438E0h]
  000000006101C30C: 90                 nop
  000000006101C30D: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000006101C312: FF 15 C8 75 02 00  call        qword ptr [00000000610438E0h]
  000000006101C318: 84 DB              test        bl,bl
  000000006101C31A: 0F 85 20 01 00 00  jne         000000006101C440
  000000006101C320: 48 8B CD           mov         rcx,rbp
  000000006101C323: E8 18 AB FF FF     call        0000000061016E40
  000000006101C328: EB 77              jmp         000000006101C3A1
  000000006101C32A: 48 8B CD           mov         rcx,rbp
  000000006101C32D: E8 BE 2D 00 00     call        000000006101F0F0
  000000006101C332: 84 C0              test        al,al
  000000006101C334: 0F 84 37 01 00 00  je          000000006101C471
  000000006101C33A: EB 65              jmp         000000006101C3A1
  000000006101C33C: 48 8B CD           mov         rcx,rbp
  000000006101C33F: E8 FC AA FF FF     call        0000000061016E40
  000000006101C344: EB 5B              jmp         000000006101C3A1
  000000006101C346: C6 85 C1 00 00 00  mov         byte ptr [rbp+00000000000000C1h],0
                    00
  000000006101C34D: 48 8B CD           mov         rcx,rbp
  000000006101C350: E8 1B C1 FF FF     call        0000000061018470
  000000006101C355: 84 C0              test        al,al
  000000006101C357: 0F 84 2D 01 00 00  je          000000006101C48A
  000000006101C35D: EB 42              jmp         000000006101C3A1
  000000006101C35F: 48 8B 45 00        mov         rax,qword ptr [rbp]
  000000006101C363: 83 78 08 00        cmp         dword ptr [rax+8],0
  000000006101C367: 0F 84 3D 01 00 00  je          000000006101C4AA
  000000006101C36D: 48 8B CD           mov         rcx,rbp
  000000006101C370: E8 BB AF FF FF     call        0000000061017330
  000000006101C375: 84 C0              test        al,al
  000000006101C377: 0F 84 26 01 00 00  je          000000006101C4A3
  000000006101C37D: 48 8B CD           mov         rcx,rbp
  000000006101C380: E8 BB AA FF FF     call        0000000061016E40
  000000006101C385: EB 1A              jmp         000000006101C3A1
  000000006101C387: 48 8B CD           mov         rcx,rbp
  000000006101C38A: E8 21 F6 FF FF     call        000000006101B9B0
  000000006101C38F: 84 C0              test        al,al
  000000006101C391: 0F 84 54 01 00 00  je          000000006101C4EB
  000000006101C397: EB 08              jmp         000000006101C3A1
  000000006101C399: 48 8B CD           mov         rcx,rbp
  000000006101C39C: E8 9F AA FF FF     call        0000000061016E40
  000000006101C3A1: 41 83 FC FF        cmp         r12d,0FFFFFFFFh
  000000006101C3A5: 0F 84 6C 01 00 00  je          000000006101C517
  000000006101C3AB: E9 B8 FB FF FF     jmp         000000006101BF68
  000000006101C3B0: B0 01              mov         al,1
  000000006101C3B2: E9 9F 01 00 00     jmp         000000006101C556
  000000006101C3B7: 32 C0              xor         al,al
  000000006101C3B9: E9 98 01 00 00     jmp         000000006101C556
  000000006101C3BE: 32 C0              xor         al,al
  000000006101C3C0: E9 91 01 00 00     jmp         000000006101C556
  000000006101C3C5: 45 8B C4           mov         r8d,r12d
  000000006101C3C8: 48 8D 15 D1 FA FF  lea         rdx,[000000006101BEA0h]
                    FF
  000000006101C3CF: 48 8B CD           mov         rcx,rbp
  000000006101C3D2: E8 D9 A0 FF FF     call        00000000610164B0
  000000006101C3D7: 32 C0              xor         al,al
  000000006101C3D9: E9 78 01 00 00     jmp         000000006101C556
  000000006101C3DE: 45 8B C4           mov         r8d,r12d
  000000006101C3E1: 48 8D 15 B8 FA FF  lea         rdx,[000000006101BEA0h]
                    FF
  000000006101C3E8: 48 8B CD           mov         rcx,rbp
  000000006101C3EB: E8 C0 A0 FF FF     call        00000000610164B0
  000000006101C3F0: 32 C0              xor         al,al
  000000006101C3F2: E9 5F 01 00 00     jmp         000000006101C556
  000000006101C3F7: 48 8B 8D E8 00 00  mov         rcx,qword ptr [rbp+00000000000000E8h]
                    00
  000000006101C3FE: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006101C401: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  000000006101C406: FF 50 60           call        qword ptr [rax+60h]
  000000006101C409: 90                 nop
  000000006101C40A: 48 8B D0           mov         rdx,rax
  000000006101C40D: 48 8B CD           mov         rcx,rbp
  000000006101C410: E8 3B 7A FF FF     call        0000000061013E50
  000000006101C415: 90                 nop
  000000006101C416: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006101C41B: FF 15 BF 74 02 00  call        qword ptr [00000000610438E0h]
  000000006101C421: 90                 nop
  000000006101C422: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006101C427: FF 15 B3 74 02 00  call        qword ptr [00000000610438E0h]
  000000006101C42D: 90                 nop
  000000006101C42E: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006101C433: FF 15 A7 74 02 00  call        qword ptr [00000000610438E0h]
  000000006101C439: 32 C0              xor         al,al
  000000006101C43B: E9 16 01 00 00     jmp         000000006101C556
  000000006101C440: 48 8B 8D E8 00 00  mov         rcx,qword ptr [rbp+00000000000000E8h]
                    00
  000000006101C447: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006101C44A: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000006101C44F: FF 50 60           call        qword ptr [rax+60h]
  000000006101C452: 90                 nop
  000000006101C453: 48 8B D0           mov         rdx,rax
  000000006101C456: 48 8B CD           mov         rcx,rbp
  000000006101C459: E8 F2 79 FF FF     call        0000000061013E50
  000000006101C45E: 90                 nop
  000000006101C45F: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006101C464: FF 15 76 74 02 00  call        qword ptr [00000000610438E0h]
  000000006101C46A: 32 C0              xor         al,al
  000000006101C46C: E9 E5 00 00 00     jmp         000000006101C556
  000000006101C471: 45 8B C4           mov         r8d,r12d
  000000006101C474: 48 8D 15 25 FA FF  lea         rdx,[000000006101BEA0h]
                    FF
  000000006101C47B: 48 8B CD           mov         rcx,rbp
  000000006101C47E: E8 2D A0 FF FF     call        00000000610164B0
  000000006101C483: 32 C0              xor         al,al
  000000006101C485: E9 CC 00 00 00     jmp         000000006101C556
  000000006101C48A: 45 8B C4           mov         r8d,r12d
  000000006101C48D: 48 8D 15 0C FA FF  lea         rdx,[000000006101BEA0h]
                    FF
  000000006101C494: 48 8B CD           mov         rcx,rbp
  000000006101C497: E8 14 A0 FF FF     call        00000000610164B0
  000000006101C49C: 32 C0              xor         al,al
  000000006101C49E: E9 B3 00 00 00     jmp         000000006101C556
  000000006101C4A3: 32 C0              xor         al,al
  000000006101C4A5: E9 AC 00 00 00     jmp         000000006101C556
  000000006101C4AA: 48 8D 15 97 B4 02  lea         rdx,[0000000061047948h]
                    00
  000000006101C4B1: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000006101C4B9: FF 15 D9 73 02 00  call        qword ptr [0000000061043898h]
  000000006101C4BF: 48 8B D0           mov         rdx,rax
  000000006101C4C2: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006101C4C7: FF 15 F3 72 02 00  call        qword ptr [00000000610437C0h]
  000000006101C4CD: 90                 nop
  000000006101C4CE: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006101C4D3: 48 8B CD           mov         rcx,rbp
  000000006101C4D6: E8 75 79 FF FF     call        0000000061013E50
  000000006101C4DB: 90                 nop
  000000006101C4DC: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006101C4E1: FF 15 F9 73 02 00  call        qword ptr [00000000610438E0h]
  000000006101C4E7: 32 C0              xor         al,al
  000000006101C4E9: EB 6B              jmp         000000006101C556
  000000006101C4EB: 45 8B C4           mov         r8d,r12d
  000000006101C4EE: 48 8D 15 AB F9 FF  lea         rdx,[000000006101BEA0h]
                    FF
  000000006101C4F5: 48 8B CD           mov         rcx,rbp
  000000006101C4F8: E8 B3 9F FF FF     call        00000000610164B0
  000000006101C4FD: 32 C0              xor         al,al
  000000006101C4FF: EB 55              jmp         000000006101C556
  000000006101C501: 45 8B C4           mov         r8d,r12d
  000000006101C504: 48 8D 15 95 F9 FF  lea         rdx,[000000006101BEA0h]
                    FF
  000000006101C50B: 48 8B CD           mov         rcx,rbp
  000000006101C50E: E8 8D 9E FF FF     call        00000000610163A0
  000000006101C513: 32 C0              xor         al,al
  000000006101C515: EB 3F              jmp         000000006101C556
  000000006101C517: 48 8D 15 9A BB 02  lea         rdx,[00000000610480B8h]
                    00
  000000006101C51E: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006101C526: FF 15 6C 73 02 00  call        qword ptr [0000000061043898h]
  000000006101C52C: 48 8B D0           mov         rdx,rax
  000000006101C52F: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006101C534: FF 15 86 72 02 00  call        qword ptr [00000000610437C0h]
  000000006101C53A: 90                 nop
  000000006101C53B: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006101C540: 48 8B CD           mov         rcx,rbp
  000000006101C543: E8 08 79 FF FF     call        0000000061013E50
  000000006101C548: 90                 nop
  000000006101C549: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006101C54E: FF 15 8C 73 02 00  call        qword ptr [00000000610438E0h]
  000000006101C554: 32 C0              xor         al,al
  000000006101C556: 48 81 C4 A8 00 00  add         rsp,0A8h
                    00
  000000006101C55D: 41 5F              pop         r15
  000000006101C55F: 41 5E              pop         r14
  000000006101C561: 41 5D              pop         r13
  000000006101C563: 41 5C              pop         r12
  000000006101C565: 5F                 pop         rdi
  000000006101C566: 5E                 pop         rsi
  000000006101C567: 5D                 pop         rbp
  000000006101C568: 5B                 pop         rbx
  000000006101C569: C3                 ret
  000000006101C56A: 66 90              nop
  000000006101C56C: E7 C1              out         0C1h,eax
  000000006101C56E: 01 00              add         dword ptr [rax],eax
  000000006101C570: 03 C2              add         eax,edx
  000000006101C572: 01 00              add         dword ptr [rax],eax
  000000006101C574: 18 C2              sbb         dl,al
  000000006101C576: 01 00              add         dword ptr [rax],eax
  000000006101C578: 2A C3              sub         al,bl
  000000006101C57A: 01 00              add         dword ptr [rax],eax
  000000006101C57C: 3C C3              cmp         al,0C3h
  000000006101C57E: 01 00              add         dword ptr [rax],eax
  000000006101C580: 46 C3              ret
  000000006101C582: 01 00              add         dword ptr [rax],eax
  000000006101C584: 03 C2              add         eax,edx
  000000006101C586: 01 00              add         dword ptr [rax],eax
  000000006101C588: 5F                 pop         rdi
  000000006101C589: C3                 ret
  000000006101C58A: 01 00              add         dword ptr [rax],eax
  000000006101C58C: 87 C3              xchg        eax,ebx
  000000006101C58E: 01 00              add         dword ptr [rax],eax
  000000006101C590: 87 C3              xchg        eax,ebx
  000000006101C592: 01 00              add         dword ptr [rax],eax
  000000006101C594: 03 C2              add         eax,edx
  000000006101C596: 01 00              add         dword ptr [rax],eax
  000000006101C598: 99                 cdq
  000000006101C599: C3                 ret
  000000006101C59A: 01 00              add         dword ptr [rax],eax

  Summary

       42000 .text
