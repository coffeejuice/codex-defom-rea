Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006506DBD0: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000006506DBD5: 53                 push        rbx
  000000006506DBD6: 55                 push        rbp
  000000006506DBD7: 56                 push        rsi
  000000006506DBD8: 57                 push        rdi
  000000006506DBD9: 41 54              push        r12
  000000006506DBDB: 41 55              push        r13
  000000006506DBDD: 41 56              push        r14
  000000006506DBDF: 41 57              push        r15
  000000006506DBE1: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  000000006506DBE8: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006506DBF1: 48 8B F2           mov         rsi,rdx
  000000006506DBF4: 48 8B D9           mov         rbx,rcx
  000000006506DBF7: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  000000006506DBFB: 81 8F D8 01 00 00  or          dword ptr [rdi+00000000000001D8h],1000000h
                    00 00 00 01
  000000006506DC05: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006506DC09: 8B 88 C8 01 00 00  mov         ecx,dword ptr [rax+00000000000001C8h]
  000000006506DC0F: C1 E9 0A           shr         ecx,0Ah
  000000006506DC12: 80 E1 01           and         cl,1
  000000006506DC15: 89 8C 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],ecx
                    00
  000000006506DC1C: 8B A8 C8 01 00 00  mov         ebp,dword ptr [rax+00000000000001C8h]
  000000006506DC22: C1 ED 1C           shr         ebp,1Ch
  000000006506DC25: 40 80 E5 01        and         bpl,1
  000000006506DC29: 89 AC 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],ebp
                    00
  000000006506DC30: 4C 8B FB           mov         r15,rbx
  000000006506DC33: 48 8B CB           mov         rcx,rbx
  000000006506DC36: FF 15 2C 8C 62 00  call        qword ptr [0000000065696868h]
  000000006506DC3C: 4C 8B D8           mov         r11,rax
  000000006506DC3F: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  000000006506DC43: 0F B6 49 0C        movzx       ecx,byte ptr [rcx+0Ch]
  000000006506DC47: F6 C1 01           test        cl,1
  000000006506DC4A: 75 24              jne         000000006506DC70
  000000006506DC4C: 0F 1F 40 00        nop         dword ptr [rax]
  000000006506DC50: 4D 85 DB           test        r11,r11
  000000006506DC53: 74 1B              je          000000006506DC70
  000000006506DC55: 4D 8B FB           mov         r15,r11
  000000006506DC58: 49 8B CB           mov         rcx,r11
  000000006506DC5B: FF 15 07 8C 62 00  call        qword ptr [0000000065696868h]
  000000006506DC61: 4C 8B D8           mov         r11,rax
  000000006506DC64: 49 8B 47 20        mov         rax,qword ptr [r15+20h]
  000000006506DC68: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506DC6C: A8 01              test        al,1
  000000006506DC6E: 74 E0              je          000000006506DC50
  000000006506DC70: 45 33 E4           xor         r12d,r12d
  000000006506DC73: 4D 8B EC           mov         r13,r12
  000000006506DC76: 48 85 F6           test        rsi,rsi
  000000006506DC79: 74 0C              je          000000006506DC87
  000000006506DC7B: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006506DC7F: 80 78 0C 11        cmp         byte ptr [rax+0Ch],11h
  000000006506DC83: 4C 0F 44 EE        cmove       r13,rsi
  000000006506DC87: 48 8B CB           mov         rcx,rbx
  000000006506DC8A: FF 15 D8 8B 62 00  call        qword ptr [0000000065696868h]
  000000006506DC90: 48 3B F0           cmp         rsi,rax
  000000006506DC93: 75 0F              jne         000000006506DCA4
  000000006506DC95: 40 84 ED           test        bpl,bpl
  000000006506DC98: 74 0A              je          000000006506DCA4
  000000006506DC9A: 4D 85 ED           test        r13,r13
  000000006506DC9D: 75 05              jne         000000006506DCA4
  000000006506DC9F: 45 32 F6           xor         r14b,r14b
  000000006506DCA2: EB 7A              jmp         000000006506DD1E
  000000006506DCA4: 41 B6 01           mov         r14b,1
  000000006506DCA7: 48 85 F6           test        rsi,rsi
  000000006506DCAA: 74 72              je          000000006506DD1E
  000000006506DCAC: 4D 85 ED           test        r13,r13
  000000006506DCAF: 75 6D              jne         000000006506DD1E
  000000006506DCB1: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506DCB5: 8B 88 D0 01 00 00  mov         ecx,dword ptr [rax+00000000000001D0h]
  000000006506DCBB: C1 E9 04           shr         ecx,4
  000000006506DCBE: 41 22 CE           and         cl,r14b
  000000006506DCC1: 74 1F              je          000000006506DCE2
  000000006506DCC3: FF 15 57 8B 62 00  call        qword ptr [0000000065696820h]
  000000006506DCC9: 41 8D 4D 04        lea         ecx,[r13+4]
  000000006506DCCD: FF 15 25 8D 62 00  call        qword ptr [00000000656969F8h]
  000000006506DCD3: 84 C0              test        al,al
  000000006506DCD5: 75 0B              jne         000000006506DCE2
  000000006506DCD7: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006506DCDB: E8 E0 ED F9 FF     call        000000006500CAC0
  000000006506DCE0: EB 3C              jmp         000000006506DD1E
  000000006506DCE2: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  000000006506DCE6: 48 8B 88 88 00 00  mov         rcx,qword ptr [rax+0000000000000088h]
                    00
  000000006506DCED: 48 85 C9           test        rcx,rcx
  000000006506DCF0: 74 0D              je          000000006506DCFF
  000000006506DCF2: 8B 41 5C           mov         eax,dword ptr [rcx+5Ch]
  000000006506DCF5: C1 E8 05           shr         eax,5
  000000006506DCF8: 83 E0 01           and         eax,1
  000000006506DCFB: 84 C0              test        al,al
  000000006506DCFD: 75 0F              jne         000000006506DD0E
  000000006506DCFF: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006506DD03: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006506DD06: C1 E9 08           shr         ecx,8
  000000006506DD09: 80 E1 01           and         cl,1
  000000006506DD0C: 74 10              je          000000006506DD1E
  000000006506DD0E: 41 B0 01           mov         r8b,1
  000000006506DD11: BA 64 00 00 00     mov         edx,64h
  000000006506DD16: 48 8B CB           mov         rcx,rbx
  000000006506DD19: E8 C2 B2 FF FF     call        0000000065068FE0
  000000006506DD1E: 40 84 ED           test        bpl,bpl
  000000006506DD21: 74 57              je          000000006506DD7A
  000000006506DD23: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006506DD27: 0F B6 48 0A        movzx       ecx,byte ptr [rax+0Ah]
  000000006506DD2B: 80 E1 01           and         cl,1
  000000006506DD2E: 75 1A              jne         000000006506DD4A
  000000006506DD30: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006506DD33: 33 D2              xor         edx,edx
  000000006506DD35: 48 8B CB           mov         rcx,rbx
  000000006506DD38: FF 50 58           call        qword ptr [rax+58h]
  000000006506DD3B: 45 33 C0           xor         r8d,r8d
  000000006506DD3E: 41 8D 50 45        lea         edx,[r8+45h]
  000000006506DD42: 48 8B CB           mov         rcx,rbx
  000000006506DD45: E8 96 B2 FF FF     call        0000000065068FE0
  000000006506DD4A: 45 84 F6           test        r14b,r14b
  000000006506DD4D: 74 72              je          000000006506DDC1
  000000006506DD4F: BA 83 00 00 00     mov         edx,83h
  000000006506DD54: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006506DD59: FF 15 B9 8A 62 00  call        qword ptr [0000000065696818h]
  000000006506DD5F: 90                 nop
  000000006506DD60: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006506DD65: 48 8B CB           mov         rcx,rbx
  000000006506DD68: FF 15 A2 8A 62 00  call        qword ptr [0000000065696810h]
  000000006506DD6E: 90                 nop
  000000006506DD6F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006506DD74: FF 15 8E 8A 62 00  call        qword ptr [0000000065696808h]
  000000006506DD7A: 45 84 F6           test        r14b,r14b
  000000006506DD7D: 74 42              je          000000006506DDC1
  000000006506DD7F: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506DD83: 48 8B 88 A0 00 00  mov         rcx,qword ptr [rax+00000000000000A0h]
                    00
  000000006506DD8A: 48 85 C9           test        rcx,rcx
  000000006506DD8D: 74 32              je          000000006506DDC1
  000000006506DD8F: 90                 nop
  000000006506DD90: 48 3B CB           cmp         rcx,rbx
  000000006506DD93: 74 1C              je          000000006506DDB1
  000000006506DD95: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006506DD99: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506DD9D: A8 01              test        al,1
  000000006506DD9F: 75 20              jne         000000006506DDC1
  000000006506DDA1: FF 15 C1 8A 62 00  call        qword ptr [0000000065696868h]
  000000006506DDA7: 48 8B C8           mov         rcx,rax
  000000006506DDAA: 48 85 C0           test        rax,rax
  000000006506DDAD: 75 E1              jne         000000006506DD90
  000000006506DDAF: EB 10              jmp         000000006506DDC1
  000000006506DDB1: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506DDB5: 48 8B 88 A0 00 00  mov         rcx,qword ptr [rax+00000000000000A0h]
                    00
  000000006506DDBC: E8 AF 1D FF FF     call        000000006505FB70
  000000006506DDC1: 48 8B EB           mov         rbp,rbx
  000000006506DDC4: 48 8B CB           mov         rcx,rbx
  000000006506DDC7: FF 15 9B 8A 62 00  call        qword ptr [0000000065696868h]
  000000006506DDCD: 4C 8B D8           mov         r11,rax
  000000006506DDD0: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  000000006506DDD4: 0F B6 49 0C        movzx       ecx,byte ptr [rcx+0Ch]
  000000006506DDD8: F6 C1 01           test        cl,1
  000000006506DDDB: 75 23              jne         000000006506DE00
  000000006506DDDD: 0F 1F 00           nop         dword ptr [rax]
  000000006506DDE0: 4D 85 DB           test        r11,r11
  000000006506DDE3: 74 1B              je          000000006506DE00
  000000006506DDE5: 49 8B EB           mov         rbp,r11
  000000006506DDE8: 49 8B CB           mov         rcx,r11
  000000006506DDEB: FF 15 77 8A 62 00  call        qword ptr [0000000065696868h]
  000000006506DDF1: 4C 8B D8           mov         r11,rax
  000000006506DDF4: 48 8B 45 20        mov         rax,qword ptr [rbp+20h]
  000000006506DDF8: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506DDFC: A8 01              test        al,1
  000000006506DDFE: 74 E0              je          000000006506DDE0
  000000006506DE00: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  000000006506DE04: 48 8B 80 88 00 00  mov         rax,qword ptr [rax+0000000000000088h]
                    00
  000000006506DE0B: 48 85 C0           test        rax,rax
  000000006506DE0E: 74 0D              je          000000006506DE1D
  000000006506DE10: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006506DE14: 48 85 C0           test        rax,rax
  000000006506DE17: 74 04              je          000000006506DE1D
  000000006506DE19: 4C 8D 60 10        lea         r12,[rax+10h]
  000000006506DE1D: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000006506DE25: 8B 00              mov         eax,dword ptr [rax]
  000000006506DE27: 89 84 24 D0 00 00  mov         dword ptr [rsp+00000000000000D0h],eax
                    00
  000000006506DE2E: 4C 8D 84 24 D0 00  lea         r8,[rsp+00000000000000D0h]
                    00 00
  000000006506DE36: 48 8B D6           mov         rdx,rsi
  000000006506DE39: 48 8B CF           mov         rcx,rdi
  000000006506DE3C: E8 6F CF 02 00     call        000000006509ADB0
  000000006506DE41: 48 8B EB           mov         rbp,rbx
  000000006506DE44: 48 8B CB           mov         rcx,rbx
  000000006506DE47: FF 15 1B 8A 62 00  call        qword ptr [0000000065696868h]
  000000006506DE4D: 4C 8B D8           mov         r11,rax
  000000006506DE50: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  000000006506DE54: 0F B6 49 0C        movzx       ecx,byte ptr [rcx+0Ch]
  000000006506DE58: F6 C1 01           test        cl,1
  000000006506DE5B: 75 23              jne         000000006506DE80
  000000006506DE5D: 0F 1F 00           nop         dword ptr [rax]
  000000006506DE60: 4D 85 DB           test        r11,r11
  000000006506DE63: 74 1B              je          000000006506DE80
  000000006506DE65: 49 8B EB           mov         rbp,r11
  000000006506DE68: 49 8B CB           mov         rcx,r11
  000000006506DE6B: FF 15 F7 89 62 00  call        qword ptr [0000000065696868h]
  000000006506DE71: 4C 8B D8           mov         r11,rax
  000000006506DE74: 48 8B 45 20        mov         rax,qword ptr [rbp+20h]
  000000006506DE78: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506DE7C: A8 01              test        al,1
  000000006506DE7E: 74 E0              je          000000006506DE60
  000000006506DE80: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  000000006506DE84: 48 8B 80 88 00 00  mov         rax,qword ptr [rax+0000000000000088h]
                    00
  000000006506DE8B: 48 85 C0           test        rax,rax
  000000006506DE8E: 74 0F              je          000000006506DE9F
  000000006506DE90: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006506DE94: 48 85 C0           test        rax,rax
  000000006506DE97: 74 06              je          000000006506DE9F
  000000006506DE99: 48 83 C0 10        add         rax,10h
  000000006506DE9D: EB 02              jmp         000000006506DEA1
  000000006506DE9F: 33 C0              xor         eax,eax
  000000006506DEA1: 4D 85 E4           test        r12,r12
  000000006506DEA4: 74 10              je          000000006506DEB6
  000000006506DEA6: 4C 3B E0           cmp         r12,rax
  000000006506DEA9: 74 0B              je          000000006506DEB6
  000000006506DEAB: 48 8B D3           mov         rdx,rbx
  000000006506DEAE: 49 8B CC           mov         rcx,r12
  000000006506DEB1: E8 5A 79 FF FF     call        0000000065065810
  000000006506DEB6: 4D 85 ED           test        r13,r13
  000000006506DEB9: 41 BC 00 00 00 00  mov         r12d,0
  000000006506DEBF: 49 0F 45 F4        cmovne      rsi,r12
  000000006506DEC3: 49 8B 4F 08        mov         rcx,qword ptr [r15+8]
  000000006506DEC7: E8 24 1A FA FF     call        000000006500F8F0
  000000006506DECC: 48 8B E8           mov         rbp,rax
  000000006506DECF: 48 85 C0           test        rax,rax
  000000006506DED2: 74 1B              je          000000006506DEEF
  000000006506DED4: 45 84 F6           test        r14b,r14b
  000000006506DED7: 74 0B              je          000000006506DEE4
  000000006506DED9: 48 8B D3           mov         rdx,rbx
  000000006506DEDC: 48 8B C8           mov         rcx,rax
  000000006506DEDF: E8 3C 2B 18 00     call        00000000651F0A20
  000000006506DEE4: 48 8B D3           mov         rdx,rbx
  000000006506DEE7: 48 8B CD           mov         rcx,rbp
  000000006506DEEA: E8 41 4B FF FF     call        0000000065062A30
  000000006506DEEF: 48 8B 05 D2 95 62  mov         rax,qword ptr [00000000656974C8h]
                    00
  000000006506DEF6: 81 38 00 02 04 00  cmp         dword ptr [rax],40200h
  000000006506DEFC: 7C 0C              jl          000000006506DF0A
  000000006506DEFE: 48 8B 05 DB 94 62  mov         rax,qword ptr [00000000656973E0h]
                    00
  000000006506DF05: F6 00 01           test        byte ptr [rax],1
  000000006506DF08: 74 23              je          000000006506DF2D
  000000006506DF0A: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506DF0E: 8B 88 C8 01 00 00  mov         ecx,dword ptr [rax+00000000000001C8h]
  000000006506DF14: C1 E9 1C           shr         ecx,1Ch
  000000006506DF17: 80 E1 01           and         cl,1
  000000006506DF1A: 75 11              jne         000000006506DF2D
  000000006506DF1C: 41 B1 01           mov         r9b,1
  000000006506DF1F: 45 0F B6 C1        movzx       r8d,r9b
  000000006506DF23: 33 D2              xor         edx,edx
  000000006506DF25: 48 8B CB           mov         rcx,rbx
  000000006506DF28: E8 D3 99 FF FF     call        0000000065067900
  000000006506DF2D: 49 8B D7           mov         rdx,r15
  000000006506DF30: 48 8B CF           mov         rcx,rdi
  000000006506DF33: E8 08 1D FF FF     call        000000006505FC40
  000000006506DF38: 44 0F B6 84 24 D8  movzx       r8d,byte ptr [rsp+00000000000000D8h]
                    00 00 00
  000000006506DF41: BA 2A 00 00 00     mov         edx,2Ah
  000000006506DF46: 48 8B CB           mov         rcx,rbx
  000000006506DF49: E8 92 B0 FF FF     call        0000000065068FE0
  000000006506DF4E: 4C 8B 5B 08        mov         r11,qword ptr [rbx+8]
  000000006506DF52: 41 8B 83 D0 01 00  mov         eax,dword ptr [r11+00000000000001D0h]
                    00
  000000006506DF59: D1 E8              shr         eax,1
  000000006506DF5B: 24 01              and         al,1
  000000006506DF5D: 75 26              jne         000000006506DF85
  000000006506DF5F: 48 85 F6           test        rsi,rsi
  000000006506DF62: 74 11              je          000000006506DF75
  000000006506DF64: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  000000006506DF68: 8B 88 D0 01 00 00  mov         ecx,dword ptr [rax+00000000000001D0h]
  000000006506DF6E: D1 E9              shr         ecx,1
  000000006506DF70: 80 E1 01           and         cl,1
  000000006506DF73: 75 10              jne         000000006506DF85
  000000006506DF75: 48 8B CF           mov         rcx,rdi
  000000006506DF78: E8 53 32 FF FF     call        00000000650611D0
  000000006506DF7D: 48 8B CF           mov         rcx,rdi
  000000006506DF80: E8 FB A0 FF FF     call        0000000065068080
  000000006506DF85: 48 8B 57 08        mov         rdx,qword ptr [rdi+8]
  000000006506DF89: 48 8B 42 08        mov         rax,qword ptr [rdx+8]
  000000006506DF8D: 8B 88 C8 01 00 00  mov         ecx,dword ptr [rax+00000000000001C8h]
  000000006506DF93: C1 E9 19           shr         ecx,19h
  000000006506DF96: 80 E1 01           and         cl,1
  000000006506DF99: 75 39              jne         000000006506DFD4
  000000006506DF9B: 48 8B 42 20        mov         rax,qword ptr [rdx+20h]
  000000006506DF9F: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506DFA3: A8 01              test        al,1
  000000006506DFA5: 74 07              je          000000006506DFAE
  000000006506DFA7: E8 34 92 FA FF     call        00000000650171E0
  000000006506DFAC: EB 1C              jmp         000000006506DFCA
  000000006506DFAE: 48 8B CA           mov         rcx,rdx
  000000006506DFB1: FF 15 B1 88 62 00  call        qword ptr [0000000065696868h]
  000000006506DFB7: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006506DFBB: 0F B6 81 CB 01 00  movzx       eax,byte ptr [rcx+00000000000001CBh]
                    00
  000000006506DFC2: 24 01              and         al,1
  000000006506DFC4: 41 8B C4           mov         eax,r12d
  000000006506DFC7: 0F 95 C0           setne       al
  000000006506DFCA: 8B D0              mov         edx,eax
  000000006506DFCC: 48 8B CF           mov         rcx,rdi
  000000006506DFCF: E8 3C C5 FF FF     call        000000006506A510
  000000006506DFD4: 48 8B CF           mov         rcx,rdi
  000000006506DFD7: E8 64 19 FF FF     call        000000006505F940
  000000006506DFDC: 45 84 F6           test        r14b,r14b
  000000006506DFDF: 75 15              jne         000000006506DFF6
  000000006506DFE1: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000006506DFE9: 8B 00              mov         eax,dword ptr [rax]
  000000006506DFEB: 25 00 02 00 00     and         eax,200h
  000000006506DFF0: 0F 84 C2 01 00 00  je          000000006506E1B8
  000000006506DFF6: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006506DFFA: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506DFFE: A8 01              test        al,1
  000000006506E000: 75 62              jne         000000006506E064
  000000006506E002: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506E006: 0F B6 88 C8 01 00  movzx       ecx,byte ptr [rax+00000000000001C8h]
                    00
  000000006506E00D: 80 E1 01           and         cl,1
  000000006506E010: 75 1F              jne         000000006506E031
  000000006506E012: 48 85 F6           test        rsi,rsi
  000000006506E015: 74 10              je          000000006506E027
  000000006506E017: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006506E01B: 0F B6 48 08        movzx       ecx,byte ptr [rax+8]
  000000006506E01F: 80 E1 01           and         cl,1
  000000006506E022: 0F 94 C2           sete        dl
  000000006506E025: EB 02              jmp         000000006506E029
  000000006506E027: B2 01              mov         dl,1
  000000006506E029: 48 8B CF           mov         rcx,rdi
  000000006506E02C: E8 CF BE FF FF     call        0000000065069F00
  000000006506E031: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506E035: 8B 88 C8 01 00 00  mov         ecx,dword ptr [rax+00000000000001C8h]
  000000006506E03B: C1 E9 1B           shr         ecx,1Bh
  000000006506E03E: 80 E1 01           and         cl,1
  000000006506E041: 75 21              jne         000000006506E064
  000000006506E043: 48 85 F6           test        rsi,rsi
  000000006506E046: 74 12              je          000000006506E05A
  000000006506E048: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006506E04C: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006506E04F: C1 E9 0A           shr         ecx,0Ah
  000000006506E052: 80 E1 01           and         cl,1
  000000006506E055: 0F 94 C2           sete        dl
  000000006506E058: EB 02              jmp         000000006506E05C
  000000006506E05A: B2 01              mov         dl,1
  000000006506E05C: 48 8B CF           mov         rcx,rdi
  000000006506E05F: E8 6C BB FF FF     call        0000000065069BD0
  000000006506E064: 48 8B CF           mov         rcx,rdi
  000000006506E067: E8 14 0F FF FF     call        000000006505EF80
  000000006506E06C: 48 85 F6           test        rsi,rsi
  000000006506E06F: 0F 84 18 01 00 00  je          000000006506E18D
  000000006506E075: F6 47 20 20        test        byte ptr [rdi+20h],20h
  000000006506E079: 0F 84 D0 00 00 00  je          000000006506E14F
  000000006506E07F: 4C 8B C3           mov         r8,rbx
  000000006506E082: BA 44 00 00 00     mov         edx,44h
  000000006506E087: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006506E08C: FF 15 36 93 62 00  call        qword ptr [00000000656973C8h]
  000000006506E092: 90                 nop
  000000006506E093: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006506E098: 48 8B CE           mov         rcx,rsi
  000000006506E09B: FF 15 6F 87 62 00  call        qword ptr [0000000065696810h]
  000000006506E0A1: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  000000006506E0A5: 48 8B 48 68        mov         rcx,qword ptr [rax+68h]
  000000006506E0A9: 83 79 08 00        cmp         dword ptr [rcx+8],0
  000000006506E0AD: 75 4A              jne         000000006506E0F9
  000000006506E0AF: B9 18 00 00 00     mov         ecx,18h
  000000006506E0B4: E8 B9 30 57 00     call        00000000655E1172
  000000006506E0B9: 48 8B E8           mov         rbp,rax
  000000006506E0BC: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  000000006506E0C4: 48 85 C0           test        rax,rax
  000000006506E0C7: 74 1B              je          000000006506E0E4
  000000006506E0C9: BA 43 00 00 00     mov         edx,43h
  000000006506E0CE: 48 8B C8           mov         rcx,rax
  000000006506E0D1: FF 15 41 87 62 00  call        qword ptr [0000000065696818h]
  000000006506E0D7: 4C 8D 1D 72 02 63  lea         r11,[000000006569E350h]
                    00
  000000006506E0DE: 4C 89 5D 00        mov         qword ptr [rbp],r11
  000000006506E0E2: EB 03              jmp         000000006506E0E7
  000000006506E0E4: 49 8B EC           mov         rbp,r12
  000000006506E0E7: 41 B8 01 00 00 00  mov         r8d,1
  000000006506E0ED: 48 8B D5           mov         rdx,rbp
  000000006506E0F0: 48 8B CE           mov         rcx,rsi
  000000006506E0F3: FF 15 C7 93 62 00  call        qword ptr [00000000656974C0h]
  000000006506E0F9: 48 8B CB           mov         rcx,rbx
  000000006506E0FC: FF 15 5E 8A 62 00  call        qword ptr [0000000065696B60h]
  000000006506E102: 48 8B E8           mov         rbp,rax
  000000006506E105: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000006506E10A: 48 89 5C 24 30     mov         qword ptr [rsp+30h],rbx
  000000006506E10F: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006506E113: 48 83 C1 68        add         rcx,68h
  000000006506E117: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006506E11C: E8 BF 04 FF FF     call        000000006505E5E0
  000000006506E121: 90                 nop
  000000006506E122: 48 85 ED           test        rbp,rbp
  000000006506E125: 74 1D              je          000000006506E144
  000000006506E127: 48 8D 4D 08        lea         rcx,[rbp+8]
  000000006506E12B: FF 15 47 88 62 00  call        qword ptr [0000000065696978h]
  000000006506E131: 84 C0              test        al,al
  000000006506E133: 75 0F              jne         000000006506E144
  000000006506E135: 48 8B 45 00        mov         rax,qword ptr [rbp]
  000000006506E139: BA 01 00 00 00     mov         edx,1
  000000006506E13E: 48 8B CD           mov         rcx,rbp
  000000006506E141: FF 10              call        qword ptr [rax]
  000000006506E143: 90                 nop
  000000006506E144: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006506E149: FF 15 71 92 62 00  call        qword ptr [00000000656973C0h]
  000000006506E14F: F6 47 20 20        test        byte ptr [rdi+20h],20h
  000000006506E153: 74 38              je          000000006506E18D
  000000006506E155: 48 83 BF D0 00 00  cmp         qword ptr [rdi+00000000000000D0h],0
                    00 00
  000000006506E15D: 74 2E              je          000000006506E18D
  000000006506E15F: 4C 8B C3           mov         r8,rbx
  000000006506E162: BA 45 00 00 00     mov         edx,45h
  000000006506E167: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006506E16C: FF 15 56 92 62 00  call        qword ptr [00000000656973C8h]
  000000006506E172: 90                 nop
  000000006506E173: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006506E178: 48 8B CE           mov         rcx,rsi
  000000006506E17B: FF 15 8F 86 62 00  call        qword ptr [0000000065696810h]
  000000006506E181: 90                 nop
  000000006506E182: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006506E187: FF 15 33 92 62 00  call        qword ptr [00000000656973C0h]
  000000006506E18D: BA 15 00 00 00     mov         edx,15h
  000000006506E192: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506E197: FF 15 7B 86 62 00  call        qword ptr [0000000065696818h]
  000000006506E19D: 90                 nop
  000000006506E19E: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006506E1A3: 48 8B CB           mov         rcx,rbx
  000000006506E1A6: FF 15 64 86 62 00  call        qword ptr [0000000065696810h]
  000000006506E1AC: 90                 nop
  000000006506E1AD: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506E1B2: FF 15 50 86 62 00  call        qword ptr [0000000065696808h]
  000000006506E1B8: 80 BC 24 E8 00 00  cmp         byte ptr [rsp+00000000000000E8h],0
                    00 00
  000000006506E1C0: 75 4F              jne         000000006506E211
  000000006506E1C2: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006506E1C6: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  000000006506E1CA: 75 35              jne         000000006506E201
  000000006506E1CC: 48 8B CB           mov         rcx,rbx
  000000006506E1CF: FF 15 93 86 62 00  call        qword ptr [0000000065696868h]
  000000006506E1D5: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  000000006506E1D9: 8B 41 08           mov         eax,dword ptr [rcx+8]
  000000006506E1DC: C1 E8 0F           shr         eax,0Fh
  000000006506E1DF: A8 01              test        al,1
  000000006506E1E1: 75 1E              jne         000000006506E201
  000000006506E1E3: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506E1E7: F6 80 CC 01 00 00  test        byte ptr [rax+00000000000001CCh],20h
                    20
  000000006506E1EE: 75 21              jne         000000006506E211
  000000006506E1F0: 45 33 C0           xor         r8d,r8d
  000000006506E1F3: 41 8D 50 10        lea         edx,[r8+10h]
  000000006506E1F7: 48 8B CB           mov         rcx,rbx
  000000006506E1FA: E8 E1 AD FF FF     call        0000000065068FE0
  000000006506E1FF: EB 10              jmp         000000006506E211
  000000006506E201: 41 B0 01           mov         r8b,1
  000000006506E204: BA 10 00 00 00     mov         edx,10h
  000000006506E209: 48 8B CB           mov         rcx,rbx
  000000006506E20C: E8 CF AD FF FF     call        0000000065068FE0
  000000006506E211: 48 8B CF           mov         rcx,rdi
  000000006506E214: E8 C7 7C FF FF     call        0000000065065EE0
  000000006506E219: 4D 8B 5F 08        mov         r11,qword ptr [r15+8]
  000000006506E21D: 49 8B 83 88 00 00  mov         rax,qword ptr [r11+0000000000000088h]
                    00
  000000006506E224: 48 85 C0           test        rax,rax
  000000006506E227: 74 37              je          000000006506E260
  000000006506E229: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  000000006506E22E: 74 30              je          000000006506E260
  000000006506E230: 48 85 C0           test        rax,rax
  000000006506E233: 74 09              je          000000006506E23E
  000000006506E235: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  000000006506E239: 48 85 C0           test        rax,rax
  000000006506E23C: 75 11              jne         000000006506E24F
  000000006506E23E: 49 8B CF           mov         rcx,r15
  000000006506E241: FF 15 21 86 62 00  call        qword ptr [0000000065696868h]
  000000006506E247: 48 8B C8           mov         rcx,rax
  000000006506E24A: E8 11 F7 FE FF     call        000000006505D960
  000000006506E24F: 48 85 C0           test        rax,rax
  000000006506E252: 74 0C              je          000000006506E260
  000000006506E254: 48 8B D3           mov         rdx,rbx
  000000006506E257: 48 8B 48 18        mov         rcx,qword ptr [rax+18h]
  000000006506E25B: E8 E0 53 4F 00     call        0000000065563640
  000000006506E260: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006506E264: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  000000006506E268: 0F 84 94 00 00 00  je          000000006506E302
  000000006506E26E: 48 85 F6           test        rsi,rsi
  000000006506E271: 0F 84 7F 00 00 00  je          000000006506E2F6
  000000006506E277: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506E27B: 48 8B 88 88 00 00  mov         rcx,qword ptr [rax+0000000000000088h]
                    00
  000000006506E282: 48 85 C9           test        rcx,rcx
  000000006506E285: 74 13              je          000000006506E29A
  000000006506E287: 48 83 79 10 00     cmp         qword ptr [rcx+10h],0
  000000006506E28C: 74 0C              je          000000006506E29A
  000000006506E28E: 81 A7 D8 01 00 00  and         dword ptr [rdi+00000000000001D8h],0FEFFFFFFh
                    FF FF FF FE
  000000006506E298: EB 72              jmp         000000006506E30C
  000000006506E29A: 48 8B CB           mov         rcx,rbx
  000000006506E29D: 0F 1F 00           nop         dword ptr [rax]
  000000006506E2A0: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006506E2A4: 0F BA 60 0C 1D     bt          dword ptr [rax+0Ch],1Dh
  000000006506E2A9: 72 3F              jb          000000006506E2EA
  000000006506E2AB: FF 15 B7 85 62 00  call        qword ptr [0000000065696868h]
  000000006506E2B1: 48 8B C8           mov         rcx,rax
  000000006506E2B4: 48 85 C0           test        rax,rax
  000000006506E2B7: 75 E7              jne         000000006506E2A0
  000000006506E2B9: 48 8B CE           mov         rcx,rsi
  000000006506E2BC: E8 9F F6 FE FF     call        000000006505D960
  000000006506E2C1: 48 85 C0           test        rax,rax
  000000006506E2C4: 74 18              je          000000006506E2DE
  000000006506E2C6: 48 8B D3           mov         rdx,rbx
  000000006506E2C9: 48 8B 48 18        mov         rcx,qword ptr [rax+18h]
  000000006506E2CD: E8 8E 4F 4F 00     call        0000000065563260
  000000006506E2D2: 81 A7 D8 01 00 00  and         dword ptr [rdi+00000000000001D8h],0FEFFFFFFh
                    FF FF FF FE
  000000006506E2DC: EB 2E              jmp         000000006506E30C
  000000006506E2DE: 81 A7 D8 01 00 00  and         dword ptr [rdi+00000000000001D8h],0FEFFFFFFh
                    FF FF FF FE
  000000006506E2E8: EB 22              jmp         000000006506E30C
  000000006506E2EA: 81 A7 D8 01 00 00  and         dword ptr [rdi+00000000000001D8h],0FEFFFFFFh
                    FF FF FF FE
  000000006506E2F4: EB 16              jmp         000000006506E30C
  000000006506E2F6: 81 A7 D8 01 00 00  and         dword ptr [rdi+00000000000001D8h],0FEFFFFFFh
                    FF FF FF FE
  000000006506E300: EB 0A              jmp         000000006506E30C
  000000006506E302: 81 A7 D8 01 00 00  and         dword ptr [rdi+00000000000001D8h],0FEFFFFFFh
                    FF FF FF FE
  000000006506E30C: 48 81 C4 88 00 00  add         rsp,88h
                    00
  000000006506E313: 41 5F              pop         r15
  000000006506E315: 41 5E              pop         r14
  000000006506E317: 41 5D              pop         r13
  000000006506E319: 41 5C              pop         r12
  000000006506E31B: 5F                 pop         rdi
  000000006506E31C: 5E                 pop         rsi
  000000006506E31D: 5D                 pop         rbp
  000000006506E31E: 5B                 pop         rbx
  000000006506E31F: C3                 ret

  Summary

      695000 .text
