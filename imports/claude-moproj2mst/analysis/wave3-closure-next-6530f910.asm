Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006530F910: 48 8B C4           mov         rax,rsp
  000000006530F913: 57                 push        rdi
  000000006530F914: 41 54              push        r12
  000000006530F916: 41 55              push        r13
  000000006530F918: 41 56              push        r14
  000000006530F91A: 41 57              push        r15
  000000006530F91C: 48 81 EC 70 01 00  sub         rsp,170h
                    00
  000000006530F923: 48 C7 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000006530F92F: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006530F933: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  000000006530F937: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  000000006530F93B: 4D 8B E1           mov         r12,r9
  000000006530F93E: 49 8B D8           mov         rbx,r8
  000000006530F941: 4C 8B D1           mov         r10,rcx
  000000006530F944: 4C 8B 71 08        mov         r14,qword ptr [rcx+8]
  000000006530F948: 80 3D 59 D1 6B 00  cmp         byte ptr [00000000659CCAA8h],0
                    00
  000000006530F94F: 75 17              jne         000000006530F968
  000000006530F951: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000006530F959: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006530F95E: E8 ED B2 04 00     call        000000006535AC50
  000000006530F963: E9 10 04 00 00     jmp         000000006530FD78
  000000006530F968: 83 FA 5D           cmp         edx,5Dh
  000000006530F96B: 0F 87 EB 03 00 00  ja          000000006530FD5C
  000000006530F971: 48 63 C2           movsxd      rax,edx
  000000006530F974: 4C 8D 05 85 06 CF  lea         r8,[0000000065000000h]
                    FF
  000000006530F97B: 41 0F B6 84 00 C0  movzx       eax,byte ptr [r8+rax+000000000030FDC0h]
                    FD 30 00
  000000006530F984: 41 8B 8C 80 9C FD  mov         ecx,dword ptr [r8+rax*4+000000000030FD9Ch]
                    30 00
  000000006530F98C: 49 03 C8           add         rcx,r8
  000000006530F98F: FF E1              jmp         rcx
  000000006530F991: 49 8B D1           mov         rdx,r9
  000000006530F994: 48 8D 0D 45 0D 3F  lea         rcx,[00000000657006E0h]
                    00
  000000006530F99B: FF 15 37 78 38 00  call        qword ptr [00000000656971D8h]
  000000006530F9A1: 33 ED              xor         ebp,ebp
  000000006530F9A3: 48 85 C0           test        rax,rax
  000000006530F9A6: 40 0F 95 C5        setne       bpl
  000000006530F9AA: 8B F5              mov         esi,ebp
  000000006530F9AC: E9 C5 03 00 00     jmp         000000006530FD76
  000000006530F9B1: 33 ED              xor         ebp,ebp
  000000006530F9B3: 8B F5              mov         esi,ebp
  000000006530F9B5: E9 BC 03 00 00     jmp         000000006530FD76
  000000006530F9BA: BE 01 00 00 00     mov         esi,1
  000000006530F9BF: E9 B2 03 00 00     jmp         000000006530FD76
  000000006530F9C4: 4D 85 C9           test        r9,r9
  000000006530F9C7: 74 16              je          000000006530F9DF
  000000006530F9C9: 49 8B 41 20        mov         rax,qword ptr [r9+20h]
  000000006530F9CD: F6 40 08 01        test        byte ptr [rax+8],1
  000000006530F9D1: 74 0C              je          000000006530F9DF
  000000006530F9D3: 41 8B B6 6C 01 00  mov         esi,dword ptr [r14+000000000000016Ch]
                    00
  000000006530F9DA: E9 97 03 00 00     jmp         000000006530FD76
  000000006530F9DF: 41 8B B6 68 01 00  mov         esi,dword ptr [r14+0000000000000168h]
                    00
  000000006530F9E6: E9 8B 03 00 00     jmp         000000006530FD76
  000000006530F9EB: BE C0 C0 C0 00     mov         esi,0C0C0C0h
  000000006530F9F0: E9 81 03 00 00     jmp         000000006530FD76
  000000006530F9F5: BE 01 00 00 00     mov         esi,1
  000000006530F9FA: 33 ED              xor         ebp,ebp
  000000006530F9FC: 4C 8B AC 24 C0 01  mov         r13,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000006530FA04: 4D 85 ED           test        r13,r13
  000000006530FA07: 74 10              je          000000006530FA19
  000000006530FA09: 41 39 75 00        cmp         dword ptr [r13],esi
  000000006530FA0D: 7F 0A              jg          000000006530FA19
  000000006530FA0F: 41 81 7D 04 01 F0  cmp         dword ptr [r13+4],0F001h
                    00 00
  000000006530FA17: 74 03              je          000000006530FA1C
  000000006530FA19: 4C 8B ED           mov         r13,rbp
  000000006530FA1C: 48 85 DB           test        rbx,rbx
  000000006530FA1F: 74 12              je          000000006530FA33
  000000006530FA21: 39 33              cmp         dword ptr [rbx],esi
  000000006530FA23: 7C 0E              jl          000000006530FA33
  000000006530FA25: 81 7B 04 06 00 0F  cmp         dword ptr [rbx+4],0F0006h
                    00
  000000006530FA2C: 75 05              jne         000000006530FA33
  000000006530FA2E: 4C 8B FB           mov         r15,rbx
  000000006530FA31: EB 03              jmp         000000006530FA36
  000000006530FA33: 4C 8B FD           mov         r15,rbp
  000000006530FA36: 4D 85 ED           test        r13,r13
  000000006530FA39: 0F 84 37 03 00 00  je          000000006530FD76
  000000006530FA3F: 4D 85 FF           test        r15,r15
  000000006530FA42: 0F 84 2E 03 00 00  je          000000006530FD76
  000000006530FA48: 49 8B CA           mov         rcx,r10
  000000006530FA4B: E8 50 85 F9 FF     call        00000000652A7FA0
  000000006530FA50: 4C 8B 10           mov         r10,qword ptr [rax]
  000000006530FA53: 4D 8B CC           mov         r9,r12
  000000006530FA56: 4C 8B C3           mov         r8,rbx
  000000006530FA59: BA 1A 00 00 00     mov         edx,1Ah
  000000006530FA5E: 48 8B C8           mov         rcx,rax
  000000006530FA61: 41 FF 92 D8 00 00  call        qword ptr [r10+00000000000000D8h]
                    00
  000000006530FA68: 8B D0              mov         edx,eax
  000000006530FA6A: 89 84 24 A0 01 00  mov         dword ptr [rsp+00000000000001A0h],eax
                    00
  000000006530FA71: 48 8D 43 10        lea         rax,[rbx+10h]
  000000006530FA75: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000006530FA7A: 4C 8D 44 24 58     lea         r8,[rsp+58h]
  000000006530FA7F: 8B 08              mov         ecx,dword ptr [rax]
  000000006530FA81: 41 89 08           mov         dword ptr [r8],ecx
  000000006530FA84: 8B 48 04           mov         ecx,dword ptr [rax+4]
  000000006530FA87: 41 89 48 04        mov         dword ptr [r8+4],ecx
  000000006530FA8B: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006530FA8E: 41 89 48 08        mov         dword ptr [r8+8],ecx
  000000006530FA92: 8B 48 0C           mov         ecx,dword ptr [rax+0Ch]
  000000006530FA95: 41 89 48 0C        mov         dword ptr [r8+0Ch],ecx
  000000006530FA99: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006530FA9E: FF 15 44 81 38 00  call        qword ptr [0000000065697BE8h]
  000000006530FAA4: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006530FAA9: FF 15 B9 6C 38 00  call        qword ptr [0000000065696768h]
  000000006530FAAF: 48 8B D8           mov         rbx,rax
  000000006530FAB2: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  000000006530FABA: FF 15 78 6D 38 00  call        qword ptr [0000000065696838h]
  000000006530FAC0: 48 8B F8           mov         rdi,rax
  000000006530FAC3: 48 89 6C 24 70     mov         qword ptr [rsp+70h],rbp
  000000006530FAC8: 48 89 6C 24 78     mov         qword ptr [rsp+78h],rbp
  000000006530FACD: 48 8B D3           mov         rdx,rbx
  000000006530FAD0: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006530FAD8: FF 15 12 6F 38 00  call        qword ptr [00000000656969F0h]
  000000006530FADE: 48 89 AC 24 88 00  mov         qword ptr [rsp+0000000000000088h],rbp
                    00 00
  000000006530FAE6: 89 AC 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],ebp
                    00
  000000006530FAED: 89 AC 24 94 00 00  mov         dword ptr [rsp+0000000000000094h],ebp
                    00
  000000006530FAF4: 83 A4 24 98 00 00  and         dword ptr [rsp+0000000000000098h],0FFFFFFF0h
                    00 F0
  000000006530FAFC: 89 AC 24 9C 00 00  mov         dword ptr [rsp+000000000000009Ch],ebp
                    00
  000000006530FB03: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006530FB0B: 8B 0F              mov         ecx,dword ptr [rdi]
  000000006530FB0D: 89 0A              mov         dword ptr [rdx],ecx
  000000006530FB0F: 8B 47 04           mov         eax,dword ptr [rdi+4]
  000000006530FB12: 89 42 04           mov         dword ptr [rdx+4],eax
  000000006530FB15: 8B 47 08           mov         eax,dword ptr [rdi+8]
  000000006530FB18: 89 42 08           mov         dword ptr [rdx+8],eax
  000000006530FB1B: 8B 47 0C           mov         eax,dword ptr [rdi+0Ch]
  000000006530FB1E: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  000000006530FB21: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006530FB26: FF 15 34 6C 38 00  call        qword ptr [0000000065696760h]
  000000006530FB2C: 41 F6 47 50 01     test        byte ptr [r15+50h],1
  000000006530FB31: 0F 84 BD 00 00 00  je          000000006530FBF4
  000000006530FB37: 48 8D 15 22 D0 3D  lea         rdx,[00000000656ECB60h]
                    00
  000000006530FB3E: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  000000006530FB46: FF 15 94 6E 38 00  call        qword ptr [00000000656969E0h]
  000000006530FB4C: 48 8B D0           mov         rdx,rax
  000000006530FB4F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006530FB54: FF 15 AE 70 38 00  call        qword ptr [0000000065696C08h]
  000000006530FB5A: 90                 nop
  000000006530FB5B: 4C 89 A4 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],r12
                    00 00
  000000006530FB63: 48 89 AC 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rbp
                    00 00
  000000006530FB6B: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006530FB70: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  000000006530FB78: FF 15 72 6E 38 00  call        qword ptr [00000000656969F0h]
  000000006530FB7E: 48 89 AC 24 08 01  mov         qword ptr [rsp+0000000000000108h],rbp
                    00 00
  000000006530FB86: C7 84 24 10 01 00  mov         dword ptr [rsp+0000000000000110h],3
                    00 03 00 00 00
  000000006530FB91: 89 B4 24 14 01 00  mov         dword ptr [rsp+0000000000000114h],esi
                    00
  000000006530FB98: 83 A4 24 18 01 00  and         dword ptr [rsp+0000000000000118h],0FFFFFFF0h
                    00 F0
  000000006530FBA0: 89 AC 24 1C 01 00  mov         dword ptr [rsp+000000000000011Ch],ebp
                    00
  000000006530FBA7: 48 8D 94 24 20 01  lea         rdx,[rsp+0000000000000120h]
                    00 00
  000000006530FBAF: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006530FBB4: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006530FBB7: 48 89 02           mov         qword ptr [rdx],rax
  000000006530FBBA: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006530FBBE: 48 89 42 08        mov         qword ptr [rdx+8],rax
  000000006530FBC2: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  000000006530FBCA: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000006530FBCF: E8 4C EC FE FF     call        00000000652FE820
  000000006530FBD4: 90                 nop
  000000006530FBD5: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  000000006530FBDD: FF 15 7D 6B 38 00  call        qword ptr [0000000065696760h]
  000000006530FBE3: 90                 nop
  000000006530FBE4: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006530FBE9: FF 15 71 6B 38 00  call        qword ptr [0000000065696760h]
  000000006530FBEF: E9 B4 00 00 00     jmp         000000006530FCA8
  000000006530FBF4: 48 8D 15 65 CF 3D  lea         rdx,[00000000656ECB60h]
                    00
  000000006530FBFB: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  000000006530FC03: FF 15 D7 6D 38 00  call        qword ptr [00000000656969E0h]
  000000006530FC09: 48 8B D0           mov         rdx,rax
  000000006530FC0C: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006530FC11: FF 15 F1 6F 38 00  call        qword ptr [0000000065696C08h]
  000000006530FC17: 90                 nop
  000000006530FC18: 4C 89 A4 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],r12
                    00 00
  000000006530FC20: 48 89 AC 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rbp
                    00 00
  000000006530FC28: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006530FC2D: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000006530FC35: FF 15 B5 6D 38 00  call        qword ptr [00000000656969F0h]
  000000006530FC3B: 48 89 AC 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rbp
                    00 00
  000000006530FC43: 89 B4 24 D0 00 00  mov         dword ptr [rsp+00000000000000D0h],esi
                    00
  000000006530FC4A: 89 B4 24 D4 00 00  mov         dword ptr [rsp+00000000000000D4h],esi
                    00
  000000006530FC51: 83 A4 24 D8 00 00  and         dword ptr [rsp+00000000000000D8h],0FFFFFFF0h
                    00 F0
  000000006530FC59: 89 AC 24 DC 00 00  mov         dword ptr [rsp+00000000000000DCh],ebp
                    00
  000000006530FC60: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  000000006530FC68: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006530FC6D: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006530FC70: 48 89 02           mov         qword ptr [rdx],rax
  000000006530FC73: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006530FC77: 48 89 42 08        mov         qword ptr [rdx+8],rax
  000000006530FC7B: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  000000006530FC83: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000006530FC88: E8 93 EB FE FF     call        00000000652FE820
  000000006530FC8D: 90                 nop
  000000006530FC8E: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000006530FC96: FF 15 C4 6A 38 00  call        qword ptr [0000000065696760h]
  000000006530FC9C: 90                 nop
  000000006530FC9D: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006530FCA2: FF 15 B8 6A 38 00  call        qword ptr [0000000065696760h]
  000000006530FCA8: 4C 8D 44 24 70     lea         r8,[rsp+70h]
  000000006530FCAD: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000006530FCB2: 49 8B CE           mov         rcx,r14
  000000006530FCB5: E8 96 CF FF FF     call        000000006530CC50
  000000006530FCBA: 48 8B F8           mov         rdi,rax
  000000006530FCBD: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  000000006530FCC2: 48 8B CD           mov         rcx,rbp
  000000006530FCC5: FF 15 5D 6A 38 00  call        qword ptr [0000000065696728h]
  000000006530FCCB: 8B D8              mov         ebx,eax
  000000006530FCCD: 44 8B A4 24 A0 01  mov         r12d,dword ptr [rsp+00000000000001A0h]
                    00 00
  000000006530FCD5: 41 2B DC           sub         ebx,r12d
  000000006530FCD8: 48 8B CD           mov         rcx,rbp
  000000006530FCDB: FF 15 7F 6B 38 00  call        qword ptr [0000000065696860h]
  000000006530FCE1: 44 8B C8           mov         r9d,eax
  000000006530FCE4: 89 5C 24 20        mov         dword ptr [rsp+20h],ebx
  000000006530FCE8: 45 8B C4           mov         r8d,r12d
  000000006530FCEB: 33 D2              xor         edx,edx
  000000006530FCED: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  000000006530FCF5: FF 15 75 6A 38 00  call        qword ptr [0000000065696770h]
  000000006530FCFB: 4C 8B C0           mov         r8,rax
  000000006530FCFE: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006530FD03: 48 8B CF           mov         rcx,rdi
  000000006530FD06: E8 D5 F2 E7 FF     call        000000006518EFE0
  000000006530FD0B: 90                 nop
  000000006530FD0C: 49 8D 4D 08        lea         rcx,[r13+8]
  000000006530FD10: 48 8B D0           mov         rdx,rax
  000000006530FD13: E8 38 97 E7 FF     call        0000000065189450
  000000006530FD18: 90                 nop
  000000006530FD19: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006530FD1E: E8 FD 96 E7 FF     call        0000000065189420
  000000006530FD23: 90                 nop
  000000006530FD24: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006530FD29: E8 F2 96 E7 FF     call        0000000065189420
  000000006530FD2E: 90                 nop
  000000006530FD2F: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006530FD37: FF 15 23 6A 38 00  call        qword ptr [0000000065696760h]
  000000006530FD3D: EB 37              jmp         000000006530FD76
  000000006530FD3F: 48 85 DB           test        rbx,rbx
  000000006530FD42: 74 11              je          000000006530FD55
  000000006530FD44: 83 3B 01           cmp         dword ptr [rbx],1
  000000006530FD47: 7C 0C              jl          000000006530FD55
  000000006530FD49: 83 7B 04 0F        cmp         dword ptr [rbx+4],0Fh
  000000006530FD4D: 75 06              jne         000000006530FD55
  000000006530FD4F: 33 ED              xor         ebp,ebp
  000000006530FD51: 8B F5              mov         esi,ebp
  000000006530FD53: EB 21              jmp         000000006530FD76
  000000006530FD55: BE 01 00 00 00     mov         esi,1
  000000006530FD5A: EB 1A              jmp         000000006530FD76
  000000006530FD5C: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000006530FD64: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006530FD69: 4C 8B C3           mov         r8,rbx
  000000006530FD6C: 49 8B CA           mov         rcx,r10
  000000006530FD6F: E8 DC AE 04 00     call        000000006535AC50
  000000006530FD74: 8B F0              mov         esi,eax
  000000006530FD76: 8B C6              mov         eax,esi
  000000006530FD78: 4C 8D 9C 24 70 01  lea         r11,[rsp+0000000000000170h]
                    00 00
  000000006530FD80: 49 8B 5B 38        mov         rbx,qword ptr [r11+38h]
  000000006530FD84: 49 8B 6B 40        mov         rbp,qword ptr [r11+40h]
  000000006530FD88: 49 8B 73 48        mov         rsi,qword ptr [r11+48h]
  000000006530FD8C: 49 8B E3           mov         rsp,r11
  000000006530FD8F: 41 5F              pop         r15
  000000006530FD91: 41 5E              pop         r14
  000000006530FD93: 41 5D              pop         r13
  000000006530FD95: 41 5C              pop         r12
  000000006530FD97: 5F                 pop         rdi
  000000006530FD98: C3                 ret
  000000006530FD99: 0F 1F 00           nop         dword ptr [rax]
  000000006530FD9C: 91                 xchg        eax,ecx
  000000006530FD9D: F9                 stc
  000000006530FD9E: 30 00              xor         byte ptr [rax],al
  000000006530FDA0: BA F9 30 00 C4     mov         edx,0C40030F9h
  000000006530FDA5: F9                 stc
  000000006530FDA6: 30 00              xor         byte ptr [rax],al
  000000006530FDA8: EB F9              jmp         000000006530FDA3
  000000006530FDAA: 30 00              xor         byte ptr [rax],al
  000000006530FDAC: 3F
  000000006530FDAD: FD                 std
  000000006530FDAE: 30 00              xor         byte ptr [rax],al
  000000006530FDB0: F5                 cmc
  000000006530FDB1: F9                 stc
  000000006530FDB2: 30 00              xor         byte ptr [rax],al
  000000006530FDB4: B1 F9              mov         cl,0F9h
  000000006530FDB6: 30 00              xor         byte ptr [rax],al
  000000006530FDB8: 55                 push        rbp
  000000006530FDB9: FD                 std
  000000006530FDBA: 30 00              xor         byte ptr [rax],al
  000000006530FDBC: 5C                 pop         rsp
  000000006530FDBD: FD                 std
  000000006530FDBE: 30 00              xor         byte ptr [rax],al
  000000006530FDC0: 00 08              add         byte ptr [rax],cl
  000000006530FDC2: 08 08              or          byte ptr [rax],cl
  000000006530FDC4: 08 08              or          byte ptr [rax],cl
  000000006530FDC6: 08 08              or          byte ptr [rax],cl
  000000006530FDC8: 08 08              or          byte ptr [rax],cl
  000000006530FDCA: 08 08              or          byte ptr [rax],cl
  000000006530FDCC: 08 08              or          byte ptr [rax],cl
  000000006530FDCE: 08 08              or          byte ptr [rax],cl
  000000006530FDD0: 08 08              or          byte ptr [rax],cl
  000000006530FDD2: 08 08              or          byte ptr [rax],cl
  000000006530FDD4: 08 08              or          byte ptr [rax],cl
  000000006530FDD6: 08 08              or          byte ptr [rax],cl
  000000006530FDD8: 08 08              or          byte ptr [rax],cl
  000000006530FDDA: 01 08              add         dword ptr [rax],ecx
  000000006530FDDC: 08 08              or          byte ptr [rax],cl
  000000006530FDDE: 08 08              or          byte ptr [rax],cl
  000000006530FDE0: 02 08              add         cl,byte ptr [rax]
  000000006530FDE2: 03 08              add         ecx,dword ptr [rax]
  000000006530FDE4: 08 08              or          byte ptr [rax],cl
  000000006530FDE6: 08 08              or          byte ptr [rax],cl
  000000006530FDE8: 08 08              or          byte ptr [rax],cl
  000000006530FDEA: 08 08              or          byte ptr [rax],cl
  000000006530FDEC: 08 08              or          byte ptr [rax],cl
  000000006530FDEE: 08 08              or          byte ptr [rax],cl
  000000006530FDF0: 08 08              or          byte ptr [rax],cl
  000000006530FDF2: 08 08              or          byte ptr [rax],cl
  000000006530FDF4: 01 08              add         dword ptr [rax],ecx
  000000006530FDF6: 08 04 05 06 08 08  or          byte ptr [rax+0000000008080806h],al
                    08
  000000006530FDFD: 08 08              or          byte ptr [rax],cl
  000000006530FDFF: 08 08              or          byte ptr [rax],cl
  000000006530FE01: 08 08              or          byte ptr [rax],cl
  000000006530FE03: 08 08              or          byte ptr [rax],cl
  000000006530FE05: 08 08              or          byte ptr [rax],cl
  000000006530FE07: 08 08              or          byte ptr [rax],cl
  000000006530FE09: 08 08              or          byte ptr [rax],cl
  000000006530FE0B: 08 08              or          byte ptr [rax],cl
  000000006530FE0D: 08 08              or          byte ptr [rax],cl
  000000006530FE0F: 08 08              or          byte ptr [rax],cl
  000000006530FE11: 08 08              or          byte ptr [rax],cl
  000000006530FE13: 08 08              or          byte ptr [rax],cl
  000000006530FE15: 08 08              or          byte ptr [rax],cl
  000000006530FE17: 08 08              or          byte ptr [rax],cl
  000000006530FE19: 08 08              or          byte ptr [rax],cl
  000000006530FE1B: 08 08              or          byte ptr [rax],cl
  000000006530FE1D: 07

  Summary

      695000 .text
