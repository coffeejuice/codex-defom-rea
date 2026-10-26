Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006506E4D0: 48 8B C4           mov         rax,rsp
  000000006506E4D3: 56                 push        rsi
  000000006506E4D4: 57                 push        rdi
  000000006506E4D5: 41 54              push        r12
  000000006506E4D7: 41 55              push        r13
  000000006506E4D9: 41 56              push        r14
  000000006506E4DB: 48 83 EC 70        sub         rsp,70h
  000000006506E4DF: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006506E4E8: 48 89 58 18        mov         qword ptr [rax+18h],rbx
  000000006506E4EC: 48 89 68 20        mov         qword ptr [rax+20h],rbp
  000000006506E4F0: 4D 8B E0           mov         r12,r8
  000000006506E4F3: 48 8B DA           mov         rbx,rdx
  000000006506E4F6: 48 8B F9           mov         rdi,rcx
  000000006506E4F9: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  000000006506E4FD: 48 89 70 10        mov         qword ptr [rax+10h],rsi
  000000006506E501: E8 3A 87 FA FF     call        0000000065016C40
  000000006506E506: 85 C0              test        eax,eax
  000000006506E508: 75 0D              jne         000000006506E517
  000000006506E50A: 48 8D 0D D7 D2 63  lea         rcx,[00000000656AB7E8h]
                    00
  000000006506E511: FF 15 11 8E 62 00  call        qword ptr [0000000065697328h]
  000000006506E517: 48 8B 0D 22 B4 95  mov         rcx,qword ptr [00000000659C9940h]
                    00
  000000006506E51E: 48 85 C9           test        rcx,rcx
  000000006506E521: 74 1D              je          000000006506E540
  000000006506E523: 4C 8D 8C 24 A0 00  lea         r9,[rsp+00000000000000A0h]
                    00 00
  000000006506E52B: 4C 8D 84 24 A8 00  lea         r8,[rsp+00000000000000A8h]
                    00 00
  000000006506E533: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006506E53B: E8 80 91 00 00     call        00000000650776C0
  000000006506E540: 45 33 F6           xor         r14d,r14d
  000000006506E543: 48 85 DB           test        rbx,rbx
  000000006506E546: 74 0C              je          000000006506E554
  000000006506E548: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006506E54C: 80 78 0C 11        cmp         byte ptr [rax+0Ch],11h
  000000006506E550: 49 0F 44 DE        cmove       rbx,r14
  000000006506E554: 48 8D 87 48 01 00  lea         rax,[rdi+0000000000000148h]
                    00
  000000006506E55B: 48 89 46 20        mov         qword ptr [rsi+20h],rax
  000000006506E55F: 81 8F 58 01 00 00  or          dword ptr [rdi+0000000000000158h],1000h
                    00 10 00 00
  000000006506E569: 4C 89 30           mov         qword ptr [rax],r14
  000000006506E56C: 44 89 B7 50 01 00  mov         dword ptr [rdi+0000000000000150h],r14d
                    00
  000000006506E573: 41 8B 04 24        mov         eax,dword ptr [r12]
  000000006506E577: 89 87 54 01 00 00  mov         dword ptr [rdi+0000000000000154h],eax
  000000006506E57D: 8B 87 58 01 00 00  mov         eax,dword ptr [rdi+0000000000000158h]
  000000006506E583: 25 00 30 F8 FF     and         eax,0FFF83000h
  000000006506E588: 0F BA E8 0D        bts         eax,0Dh
  000000006506E58C: 89 87 58 01 00 00  mov         dword ptr [rdi+0000000000000158h],eax
  000000006506E592: 41 8B 04 24        mov         eax,dword ptr [r12]
  000000006506E596: 25 00 02 00 00     and         eax,200h
  000000006506E59B: 74 10              je          000000006506E5AD
  000000006506E59D: 41 B0 01           mov         r8b,1
  000000006506E5A0: BA 64 00 00 00     mov         edx,64h
  000000006506E5A5: 48 8B CE           mov         rcx,rsi
  000000006506E5A8: E8 33 AA FF FF     call        0000000065068FE0
  000000006506E5AD: 41 B0 01           mov         r8b,1
  000000006506E5B0: BA 4C 00 00 00     mov         edx,4Ch
  000000006506E5B5: 48 8B CE           mov         rcx,rsi
  000000006506E5B8: E8 23 AA FF FF     call        0000000065068FE0
  000000006506E5BD: 48 8B 6F 08        mov         rbp,qword ptr [rdi+8]
  000000006506E5C1: 48 8B CD           mov         rcx,rbp
  000000006506E5C4: FF 15 9E 82 62 00  call        qword ptr [0000000065696868h]
  000000006506E5CA: 48 85 C0           test        rax,rax
  000000006506E5CD: 75 2F              jne         000000006506E5FE
  000000006506E5CF: 48 8B 45 20        mov         rax,qword ptr [rbp+20h]
  000000006506E5D3: 44 0F B6 40 0C     movzx       r8d,byte ptr [rax+0Ch]
  000000006506E5D8: 45 85 C0           test        r8d,r8d
  000000006506E5DB: 74 21              je          000000006506E5FE
  000000006506E5DD: 41 83 F8 12        cmp         r8d,12h
  000000006506E5E1: 74 1B              je          000000006506E5FE
  000000006506E5E3: 41 83 F8 01        cmp         r8d,1
  000000006506E5E7: 74 15              je          000000006506E5FE
  000000006506E5E9: 41 83 F8 03        cmp         r8d,3
  000000006506E5ED: 74 0F              je          000000006506E5FE
  000000006506E5EF: 45 33 C0           xor         r8d,r8d
  000000006506E5F2: 41 8D 50 4C        lea         edx,[r8+4Ch]
  000000006506E5F6: 48 8B CD           mov         rcx,rbp
  000000006506E5F9: E8 E2 A9 FF FF     call        0000000065068FE0
  000000006506E5FE: 41 B0 01           mov         r8b,1
  000000006506E601: BA 10 00 00 00     mov         edx,10h
  000000006506E606: 48 8B CE           mov         rcx,rsi
  000000006506E609: E8 D2 A9 FF FF     call        0000000065068FE0
  000000006506E60E: 48 85 DB           test        rbx,rbx
  000000006506E611: 74 23              je          000000006506E636
  000000006506E613: C7 44 24 20 1E 00  mov         dword ptr [rsp+20h],1Eh
                    00 00
  000000006506E61B: 41 B9 64 00 00 00  mov         r9d,64h
  000000006506E621: 45 33 C0           xor         r8d,r8d
  000000006506E624: 33 D2              xor         edx,edx
  000000006506E626: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006506E62B: FF 15 3F 81 62 00  call        qword ptr [0000000065696770h]
  000000006506E631: 4C 8B D8           mov         r11,rax
  000000006506E634: EB 21              jmp         000000006506E657
  000000006506E636: C7 44 24 20 E0 01  mov         dword ptr [rsp+20h],1E0h
                    00 00
  000000006506E63E: 41 B9 80 02 00 00  mov         r9d,280h
  000000006506E644: 45 33 C0           xor         r8d,r8d
  000000006506E647: 33 D2              xor         edx,edx
  000000006506E649: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006506E64E: FF 15 1C 81 62 00  call        qword ptr [0000000065696770h]
  000000006506E654: 4C 8B D8           mov         r11,rax
  000000006506E657: 41 8B 03           mov         eax,dword ptr [r11]
  000000006506E65A: 89 87 5C 01 00 00  mov         dword ptr [rdi+000000000000015Ch],eax
  000000006506E660: 41 8B 43 04        mov         eax,dword ptr [r11+4]
  000000006506E664: 89 87 60 01 00 00  mov         dword ptr [rdi+0000000000000160h],eax
  000000006506E66A: 41 8B 43 08        mov         eax,dword ptr [r11+8]
  000000006506E66E: 89 87 64 01 00 00  mov         dword ptr [rdi+0000000000000164h],eax
  000000006506E674: 41 8B 43 0C        mov         eax,dword ptr [r11+0Ch]
  000000006506E678: 89 87 68 01 00 00  mov         dword ptr [rdi+0000000000000168h],eax
  000000006506E67E: 48 89 B7 98 00 00  mov         qword ptr [rdi+0000000000000098h],rsi
                    00
  000000006506E685: 48 89 B7 90 00 00  mov         qword ptr [rdi+0000000000000090h],rsi
                    00
  000000006506E68C: 41 80 3C 24 11     cmp         byte ptr [r12],11h
  000000006506E691: 75 16              jne         000000006506E6A9
  000000006506E693: 41 B1 01           mov         r9b,1
  000000006506E696: 45 0F B6 C1        movzx       r8d,r9b
  000000006506E69A: 33 D2              xor         edx,edx
  000000006506E69C: 48 8B CE           mov         rcx,rsi
  000000006506E69F: E8 5C 92 FF FF     call        0000000065067900
  000000006506E6A4: E9 B2 00 00 00     jmp         000000006506E75B
  000000006506E6A9: 48 85 DB           test        rbx,rbx
  000000006506E6AC: 74 25              je          000000006506E6D3
  000000006506E6AE: 8B 87 54 01 00 00  mov         eax,dword ptr [rdi+0000000000000154h]
  000000006506E6B4: 89 84 24 A0 00 00  mov         dword ptr [rsp+00000000000000A0h],eax
                    00
  000000006506E6BB: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  000000006506E6C3: 48 8B D3           mov         rdx,rbx
  000000006506E6C6: 48 8B CE           mov         rcx,rsi
  000000006506E6C9: E8 02 F5 FF FF     call        000000006506DBD0
  000000006506E6CE: E9 88 00 00 00     jmp         000000006506E75B
  000000006506E6D3: 48 8B D6           mov         rdx,rsi
  000000006506E6D6: 48 8D 8F 54 01 00  lea         rcx,[rdi+0000000000000154h]
                    00
  000000006506E6DD: E8 CE E1 FE FF     call        000000006505C8B0
  000000006506E6E2: 48 8B CF           mov         rcx,rdi
  000000006506E6E5: E8 36 BF FF FF     call        000000006506A620
  000000006506E6EA: 48 8B CE           mov         rcx,rsi
  000000006506E6ED: E8 1E 5D FF FF     call        0000000065064410
  000000006506E6F2: BA 04 00 00 00     mov         edx,4
  000000006506E6F7: 44 8D 42 06        lea         r8d,[rdx+6]
  000000006506E6FB: 48 8B C8           mov         rcx,rax
  000000006506E6FE: E8 ED B4 FD FF     call        0000000065049BF0
  000000006506E703: 48 8B 4E 20        mov         rcx,qword ptr [rsi+20h]
  000000006506E707: 0F B6 49 0C        movzx       ecx,byte ptr [rcx+0Ch]
  000000006506E70B: F6 C1 01           test        cl,1
  000000006506E70E: 74 19              je          000000006506E729
  000000006506E710: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006506E713: 44 39 71 04        cmp         dword ptr [rcx+4],r14d
  000000006506E717: 74 10              je          000000006506E729
  000000006506E719: 48 8B C8           mov         rcx,rax
  000000006506E71C: E8 6F D9 0A 00     call        000000006511C090
  000000006506E721: 84 C0              test        al,al
  000000006506E723: 74 04              je          000000006506E729
  000000006506E725: B0 01              mov         al,1
  000000006506E727: EB 02              jmp         000000006506E72B
  000000006506E729: 32 C0              xor         al,al
  000000006506E72B: 8B 97 D8 01 00 00  mov         edx,dword ptr [rdi+00000000000001D8h]
  000000006506E731: 0F B6 C8           movzx       ecx,al
  000000006506E734: 8B C2              mov         eax,edx
  000000006506E736: C1 E8 11           shr         eax,11h
  000000006506E739: 83 E0 01           and         eax,1
  000000006506E73C: 3B C1              cmp         eax,ecx
  000000006506E73E: 74 1B              je          000000006506E75B
  000000006506E740: C1 E1 11           shl         ecx,11h
  000000006506E743: 33 CA              xor         ecx,edx
  000000006506E745: 81 E1 00 00 02 00  and         ecx,20000h
  000000006506E74B: 33 CA              xor         ecx,edx
  000000006506E74D: 89 8F D8 01 00 00  mov         dword ptr [rdi+00000000000001D8h],ecx
  000000006506E753: 48 8B CF           mov         rcx,rdi
  000000006506E756: E8 E5 91 02 00     call        0000000065097940
  000000006506E75B: 4C 8D 46 10        lea         r8,[rsi+10h]
  000000006506E75F: 48 8D 97 80 01 00  lea         rdx,[rdi+0000000000000180h]
                    00
  000000006506E766: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006506E76B: E8 C0 46 18 00     call        00000000651F2E30
  000000006506E770: 90                 nop
  000000006506E771: 48 8B D0           mov         rdx,rax
  000000006506E774: 48 8D 8F 80 01 00  lea         rcx,[rdi+0000000000000180h]
                    00
  000000006506E77B: E8 E0 48 18 00     call        00000000651F3060
  000000006506E780: 90                 nop
  000000006506E781: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006506E786: E8 95 83 19 00     call        0000000065206B20
  000000006506E78B: 41 B0 01           mov         r8b,1
  000000006506E78E: BA 22 00 00 00     mov         edx,22h
  000000006506E793: 48 8B CE           mov         rcx,rsi
  000000006506E796: E8 45 A8 FF FF     call        0000000065068FE0
  000000006506E79B: 41 B0 01           mov         r8b,1
  000000006506E79E: BA 23 00 00 00     mov         edx,23h
  000000006506E7A3: 48 8B CE           mov         rcx,rsi
  000000006506E7A6: E8 35 A8 FF FF     call        0000000065068FE0
  000000006506E7AB: 44 8B 1D 96 B1 95  mov         r11d,dword ptr [00000000659C9948h]
                    00
  000000006506E7B2: 41 FF C3           inc         r11d
  000000006506E7B5: 44 89 1D 8C B1 95  mov         dword ptr [00000000659C9948h],r11d
                    00
  000000006506E7BC: 8B 05 8A B1 95 00  mov         eax,dword ptr [00000000659C994Ch]
  000000006506E7C2: 44 3B D8           cmp         r11d,eax
  000000006506E7C5: 41 0F 4F C3        cmovg       eax,r11d
  000000006506E7C9: 89 05 7D B1 95 00  mov         dword ptr [00000000659C994Ch],eax
  000000006506E7CF: 48 8B 05 F2 8C 62  mov         rax,qword ptr [00000000656974C8h]
                    00
  000000006506E7D6: 81 38 00 02 04 00  cmp         dword ptr [rax],40200h
  000000006506E7DC: 7C 0C              jl          000000006506E7EA
  000000006506E7DE: 48 8B 05 FB 8B 62  mov         rax,qword ptr [00000000656973E0h]
                    00
  000000006506E7E5: F6 00 01           test        byte ptr [rax],1
  000000006506E7E8: 74 11              je          000000006506E7FB
  000000006506E7EA: 41 B1 01           mov         r9b,1
  000000006506E7ED: 45 0F B6 C1        movzx       r8d,r9b
  000000006506E7F1: 33 D2              xor         edx,edx
  000000006506E7F3: 48 8B CE           mov         rcx,rsi
  000000006506E7F6: E8 05 91 FF FF     call        0000000065067900
  000000006506E7FB: BA 0F 00 00 00     mov         edx,0Fh
  000000006506E800: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006506E805: FF 15 0D 80 62 00  call        qword ptr [0000000065696818h]
  000000006506E80B: 90                 nop
  000000006506E80C: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006506E811: 48 8B CE           mov         rcx,rsi
  000000006506E814: FF 15 F6 7F 62 00  call        qword ptr [0000000065696810h]
  000000006506E81A: B9 18 00 00 00     mov         ecx,18h
  000000006506E81F: E8 4E 29 57 00     call        00000000655E1172
  000000006506E824: 48 8B D8           mov         rbx,rax
  000000006506E827: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  000000006506E82F: 48 85 C0           test        rax,rax
  000000006506E832: 74 1A              je          000000006506E84E
  000000006506E834: BA 4A 00 00 00     mov         edx,4Ah
  000000006506E839: 48 8B C8           mov         rcx,rax
  000000006506E83C: FF 15 D6 7F 62 00  call        qword ptr [0000000065696818h]
  000000006506E842: 4C 8D 1D 07 FB 62  lea         r11,[000000006569E350h]
                    00
  000000006506E849: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006506E84C: EB 03              jmp         000000006506E851
  000000006506E84E: 49 8B DE           mov         rbx,r14
  000000006506E851: 48 8B D3           mov         rdx,rbx
  000000006506E854: 48 8B CE           mov         rcx,rsi
  000000006506E857: FF 15 93 83 62 00  call        qword ptr [0000000065696BF0h]
  000000006506E85D: 4C 89 B7 C8 00 00  mov         qword ptr [rdi+00000000000000C8h],r14
                    00
  000000006506E864: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006506E869: FF 15 99 7F 62 00  call        qword ptr [0000000065696808h]
  000000006506E86F: 4C 8D 5C 24 70     lea         r11,[rsp+70h]
  000000006506E874: 49 8B 5B 40        mov         rbx,qword ptr [r11+40h]
  000000006506E878: 49 8B 6B 48        mov         rbp,qword ptr [r11+48h]
  000000006506E87C: 49 8B E3           mov         rsp,r11
  000000006506E87F: 41 5E              pop         r14
  000000006506E881: 41 5D              pop         r13
  000000006506E883: 41 5C              pop         r12
  000000006506E885: 5F                 pop         rdi
  000000006506E886: 5E                 pop         rsi
  000000006506E887: C3                 ret

  Summary

      695000 .text
