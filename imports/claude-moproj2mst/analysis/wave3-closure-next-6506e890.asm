Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006506E890: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006506E895: 53                 push        rbx
  000000006506E896: 56                 push        rsi
  000000006506E897: 57                 push        rdi
  000000006506E898: 41 54              push        r12
  000000006506E89A: 41 55              push        r13
  000000006506E89C: 41 56              push        r14
  000000006506E89E: 41 57              push        r15
  000000006506E8A0: 48 81 EC 90 00 00  sub         rsp,90h
                    00
  000000006506E8A7: 48 C7 44 24 68 FE  mov         qword ptr [rsp+68h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006506E8B0: 4C 8B F1           mov         r14,rcx
  000000006506E8B3: 33 FF              xor         edi,edi
  000000006506E8B5: 89 BC 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],edi
                    00
  000000006506E8BC: 48 8D 05 D5 CF 63  lea         rax,[00000000656AB898h]
                    00
  000000006506E8C3: 48 89 01           mov         qword ptr [rcx],rax
  000000006506E8C6: 48 8D 05 93 CF 63  lea         rax,[00000000656AB860h]
                    00
  000000006506E8CD: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  000000006506E8D1: 4C 8B 79 08        mov         r15,qword ptr [rcx+8]
  000000006506E8D5: 4C 89 BC 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],r15
                    00 00
  000000006506E8DD: 41 81 8F 58 01 00  or          dword ptr [r15+0000000000000158h],40000h
                    00 00 00 04 00
  000000006506E8E8: 49 8D 8F C0 01 00  lea         rcx,[r15+00000000000001C0h]
                    00
  000000006506E8EF: 48 8D 94 24 E8 00  lea         rdx,[rsp+00000000000000E8h]
                    00 00
  000000006506E8F7: E8 74 40 FF FF     call        0000000065062970
  000000006506E8FC: 90                 nop
  000000006506E8FD: 48 8B D0           mov         rdx,rax
  000000006506E900: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006506E905: E8 16 FB 36 00     call        00000000653DE420
  000000006506E90A: 90                 nop
  000000006506E90B: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000006506E913: FF 15 5F 80 62 00  call        qword ptr [0000000065696978h]
  000000006506E919: 84 C0              test        al,al
  000000006506E91B: 75 43              jne         000000006506E960
  000000006506E91D: 4C 8B A4 24 E8 00  mov         r12,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000006506E925: 49 63 44 24 0C     movsxd      rax,dword ptr [r12+0Ch]
  000000006506E92A: 49 8D 5C C4 18     lea         rbx,[r12+rax*8+18h]
  000000006506E92F: 49 63 44 24 08     movsxd      rax,dword ptr [r12+8]
  000000006506E934: 49 8D 74 C4 18     lea         rsi,[r12+rax*8+18h]
  000000006506E939: 48 3B F3           cmp         rsi,rbx
  000000006506E93C: 74 13              je          000000006506E951
  000000006506E93E: 66 90              nop
  000000006506E940: 48 83 EB 08        sub         rbx,8
  000000006506E944: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006506E947: E8 62 27 57 00     call        00000000655E10AE
  000000006506E94C: 48 3B DE           cmp         rbx,rsi
  000000006506E94F: 75 EF              jne         000000006506E940
  000000006506E951: 49 8B CC           mov         rcx,r12
  000000006506E954: FF 15 06 80 62 00  call        qword ptr [0000000065696960h]
  000000006506E95A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006506E960: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  000000006506E964: FF 44 24 28        inc         dword ptr [rsp+28h]
  000000006506E968: 85 C0              test        eax,eax
  000000006506E96A: 75 11              jne         000000006506E97D
  000000006506E96C: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000006506E971: 48 39 44 24 30     cmp         qword ptr [rsp+30h],rax
  000000006506E976: 74 05              je          000000006506E97D
  000000006506E978: 41 B5 01           mov         r13b,1
  000000006506E97B: EB 03              jmp         000000006506E980
  000000006506E97D: 45 32 ED           xor         r13b,r13b
  000000006506E980: 40 F6 C7 02        test        dil,2
  000000006506E984: 74 54              je          000000006506E9DA
  000000006506E986: 83 E7 FD           and         edi,0FFFFFFFDh
  000000006506E989: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006506E98E: FF 15 E4 7F 62 00  call        qword ptr [0000000065696978h]
  000000006506E994: 84 C0              test        al,al
  000000006506E996: 75 42              jne         000000006506E9DA
  000000006506E998: 4C 8B 64 24 60     mov         r12,qword ptr [rsp+60h]
  000000006506E99D: 49 63 44 24 0C     movsxd      rax,dword ptr [r12+0Ch]
  000000006506E9A2: 49 8D 5C C4 18     lea         rbx,[r12+rax*8+18h]
  000000006506E9A7: 49 63 44 24 08     movsxd      rax,dword ptr [r12+8]
  000000006506E9AC: 49 8D 74 C4 18     lea         rsi,[r12+rax*8+18h]
  000000006506E9B1: 48 3B F3           cmp         rsi,rbx
  000000006506E9B4: 74 1B              je          000000006506E9D1
  000000006506E9B6: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  000000006506E9C0: 48 83 EB 08        sub         rbx,8
  000000006506E9C4: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006506E9C7: E8 E2 26 57 00     call        00000000655E10AE
  000000006506E9CC: 48 3B DE           cmp         rbx,rsi
  000000006506E9CF: 75 EF              jne         000000006506E9C0
  000000006506E9D1: 49 8B CC           mov         rcx,r12
  000000006506E9D4: FF 15 86 7F 62 00  call        qword ptr [0000000065696960h]
  000000006506E9DA: 45 84 ED           test        r13b,r13b
  000000006506E9DD: 0F 84 C0 01 00 00  je          000000006506EBA3
  000000006506E9E3: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000006506E9E8: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006506E9EB: 44 8B 39           mov         r15d,dword ptr [rcx]
  000000006506E9EE: 40 F6 C7 04        test        dil,4
  000000006506E9F2: 74 4C              je          000000006506EA40
  000000006506E9F4: 83 E7 FB           and         edi,0FFFFFFFBh
  000000006506E9F7: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006506E9FC: FF 15 76 7F 62 00  call        qword ptr [0000000065696978h]
  000000006506EA02: 84 C0              test        al,al
  000000006506EA04: 75 3A              jne         000000006506EA40
  000000006506EA06: 4C 8B 64 24 58     mov         r12,qword ptr [rsp+58h]
  000000006506EA0B: 49 63 44 24 0C     movsxd      rax,dword ptr [r12+0Ch]
  000000006506EA10: 49 8D 5C C4 18     lea         rbx,[r12+rax*8+18h]
  000000006506EA15: 49 63 44 24 08     movsxd      rax,dword ptr [r12+8]
  000000006506EA1A: 49 8D 74 C4 18     lea         rsi,[r12+rax*8+18h]
  000000006506EA1F: 48 3B F3           cmp         rsi,rbx
  000000006506EA22: 74 11              je          000000006506EA35
  000000006506EA24: 48 83 EB 08        sub         rbx,8
  000000006506EA28: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006506EA2B: E8 7E 26 57 00     call        00000000655E10AE
  000000006506EA30: 48 3B DE           cmp         rbx,rsi
  000000006506EA33: 75 EF              jne         000000006506EA24
  000000006506EA35: 49 8B CC           mov         rcx,r12
  000000006506EA38: FF 15 22 7F 62 00  call        qword ptr [0000000065696960h]
  000000006506EA3E: 66 90              nop
  000000006506EA40: 44 8B 6C 24 28     mov         r13d,dword ptr [rsp+28h]
  000000006506EA45: 40 F6 C7 10        test        dil,10h
  000000006506EA49: 74 4F              je          000000006506EA9A
  000000006506EA4B: 83 E7 EF           and         edi,0FFFFFFEFh
  000000006506EA4E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000006506EA53: FF 15 1F 7F 62 00  call        qword ptr [0000000065696978h]
  000000006506EA59: 84 C0              test        al,al
  000000006506EA5B: 75 3D              jne         000000006506EA9A
  000000006506EA5D: 4C 8B 64 24 50     mov         r12,qword ptr [rsp+50h]
  000000006506EA62: 49 63 44 24 0C     movsxd      rax,dword ptr [r12+0Ch]
  000000006506EA67: 49 8D 5C C4 18     lea         rbx,[r12+rax*8+18h]
  000000006506EA6C: 49 63 44 24 08     movsxd      rax,dword ptr [r12+8]
  000000006506EA71: 49 8D 74 C4 18     lea         rsi,[r12+rax*8+18h]
  000000006506EA76: 48 3B F3           cmp         rsi,rbx
  000000006506EA79: 74 16              je          000000006506EA91
  000000006506EA7B: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  000000006506EA80: 48 83 EB 08        sub         rbx,8
  000000006506EA84: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006506EA87: E8 22 26 57 00     call        00000000655E10AE
  000000006506EA8C: 48 3B DE           cmp         rbx,rsi
  000000006506EA8F: 75 EF              jne         000000006506EA80
  000000006506EA91: 49 8B CC           mov         rcx,r12
  000000006506EA94: FF 15 C6 7E 62 00  call        qword ptr [0000000065696960h]
  000000006506EA9A: 45 85 ED           test        r13d,r13d
  000000006506EA9D: 0F 84 9D 00 00 00  je          000000006506EB40
  000000006506EAA3: 44 89 BC 24 D8 00  mov         dword ptr [rsp+00000000000000D8h],r15d
                    00 00
  000000006506EAAB: 49 8B 4E 08        mov         rcx,qword ptr [r14+8]
  000000006506EAAF: 48 81 C1 C0 01 00  add         rcx,1C0h
                    00
  000000006506EAB6: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  000000006506EABE: E8 0D 17 FF FF     call        00000000650601D0
  000000006506EAC3: 85 C0              test        eax,eax
  000000006506EAC5: 74 18              je          000000006506EADF
  000000006506EAC7: E8 E4 88 FA FF     call        00000000650173B0
  000000006506EACC: 48 85 C0           test        rax,rax
  000000006506EACF: 74 0E              je          000000006506EADF
  000000006506EAD1: 45 8B C7           mov         r8d,r15d
  000000006506EAD4: 49 8B D6           mov         rdx,r14
  000000006506EAD7: 48 8B C8           mov         rcx,rax
  000000006506EADA: E8 81 D7 00 00     call        000000006507C260
  000000006506EADF: FF 4C 24 28        dec         dword ptr [rsp+28h]
  000000006506EAE3: 40 F6 C7 08        test        dil,8
  000000006506EAE7: 0F 84 53 FF FF FF  je          000000006506EA40
  000000006506EAED: 83 E7 F7           and         edi,0FFFFFFF7h
  000000006506EAF0: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006506EAF5: FF 15 7D 7E 62 00  call        qword ptr [0000000065696978h]
  000000006506EAFB: 84 C0              test        al,al
  000000006506EAFD: 0F 85 3D FF FF FF  jne         000000006506EA40
  000000006506EB03: 4C 8B 64 24 40     mov         r12,qword ptr [rsp+40h]
  000000006506EB08: 49 63 44 24 0C     movsxd      rax,dword ptr [r12+0Ch]
  000000006506EB0D: 49 8D 5C C4 18     lea         rbx,[r12+rax*8+18h]
  000000006506EB12: 49 63 44 24 08     movsxd      rax,dword ptr [r12+8]
  000000006506EB17: 49 8D 74 C4 18     lea         rsi,[r12+rax*8+18h]
  000000006506EB1C: 48 3B F3           cmp         rsi,rbx
  000000006506EB1F: 74 11              je          000000006506EB32
  000000006506EB21: 48 83 EB 08        sub         rbx,8
  000000006506EB25: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006506EB28: E8 81 25 57 00     call        00000000655E10AE
  000000006506EB2D: 48 3B DE           cmp         rbx,rsi
  000000006506EB30: 75 EF              jne         000000006506EB21
  000000006506EB32: 49 8B CC           mov         rcx,r12
  000000006506EB35: FF 15 25 7E 62 00  call        qword ptr [0000000065696960h]
  000000006506EB3B: E9 00 FF FF FF     jmp         000000006506EA40
  000000006506EB40: 48 83 44 24 30 08  add         qword ptr [rsp+30h],8
  000000006506EB46: 40 F6 C7 01        test        dil,1
  000000006506EB4A: 0F 84 10 FE FF FF  je          000000006506E960
  000000006506EB50: 83 E7 FE           and         edi,0FFFFFFFEh
  000000006506EB53: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006506EB58: FF 15 1A 7E 62 00  call        qword ptr [0000000065696978h]
  000000006506EB5E: 84 C0              test        al,al
  000000006506EB60: 0F 85 FA FD FF FF  jne         000000006506E960
  000000006506EB66: 4C 8B 64 24 48     mov         r12,qword ptr [rsp+48h]
  000000006506EB6B: 49 63 44 24 0C     movsxd      rax,dword ptr [r12+0Ch]
  000000006506EB70: 49 8D 5C C4 18     lea         rbx,[r12+rax*8+18h]
  000000006506EB75: 49 63 44 24 08     movsxd      rax,dword ptr [r12+8]
  000000006506EB7A: 49 8D 74 C4 18     lea         rsi,[r12+rax*8+18h]
  000000006506EB7F: 48 3B F3           cmp         rsi,rbx
  000000006506EB82: 74 11              je          000000006506EB95
  000000006506EB84: 48 83 EB 08        sub         rbx,8
  000000006506EB88: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006506EB8B: E8 1E 25 57 00     call        00000000655E10AE
  000000006506EB90: 48 3B DE           cmp         rbx,rsi
  000000006506EB93: 75 EF              jne         000000006506EB84
  000000006506EB95: 49 8B CC           mov         rcx,r12
  000000006506EB98: FF 15 C2 7D 62 00  call        qword ptr [0000000065696960h]
  000000006506EB9E: E9 BD FD FF FF     jmp         000000006506E960
  000000006506EBA3: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006506EBA8: FF 15 CA 7D 62 00  call        qword ptr [0000000065696978h]
  000000006506EBAE: 84 C0              test        al,al
  000000006506EBB0: 75 38              jne         000000006506EBEA
  000000006506EBB2: 48 8B 74 24 20     mov         rsi,qword ptr [rsp+20h]
  000000006506EBB7: 48 63 46 0C        movsxd      rax,dword ptr [rsi+0Ch]
  000000006506EBBB: 48 8D 5C C6 18     lea         rbx,[rsi+rax*8+18h]
  000000006506EBC0: 48 63 46 08        movsxd      rax,dword ptr [rsi+8]
  000000006506EBC4: 48 8D 7C C6 18     lea         rdi,[rsi+rax*8+18h]
  000000006506EBC9: 48 3B FB           cmp         rdi,rbx
  000000006506EBCC: 74 13              je          000000006506EBE1
  000000006506EBCE: 66 90              nop
  000000006506EBD0: 48 83 EB 08        sub         rbx,8
  000000006506EBD4: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006506EBD7: E8 D2 24 57 00     call        00000000655E10AE
  000000006506EBDC: 48 3B DF           cmp         rbx,rdi
  000000006506EBDF: 75 EF              jne         000000006506EBD0
  000000006506EBE1: 48 8B CE           mov         rcx,rsi
  000000006506EBE4: FF 15 76 7D 62 00  call        qword ptr [0000000065696960h]
  000000006506EBEA: 33 D2              xor         edx,edx
  000000006506EBEC: 4C 8B BC 24 E0 00  mov         r15,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000006506EBF4: 49 8B CF           mov         rcx,r15
  000000006506EBF7: E8 84 CA 02 00     call        000000006509B680
  000000006506EBFC: 33 F6              xor         esi,esi
  000000006506EBFE: 8B DE              mov         ebx,esi
  000000006506EC00: 49 8D 8F B8 01 00  lea         rcx,[r15+00000000000001B8h]
                    00
  000000006506EC07: FF 15 AB 7C 62 00  call        qword ptr [00000000656968B8h]
  000000006506EC0D: 85 C0              test        eax,eax
  000000006506EC0F: 7E 4C              jle         000000006506EC5D
  000000006506EC11: 4C 89 B4 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],r14
                    00 00
  000000006506EC19: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006506EC20: 8B D3              mov         edx,ebx
  000000006506EC22: 49 8D 8F B8 01 00  lea         rcx,[r15+00000000000001B8h]
                    00
  000000006506EC29: FF 15 11 7D 62 00  call        qword ptr [0000000065696940h]
  000000006506EC2F: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006506EC32: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006506EC36: 48 81 C1 20 01 00  add         rcx,120h
                    00
  000000006506EC3D: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  000000006506EC45: E8 26 36 2F 00     call        0000000065362270
  000000006506EC4A: FF C3              inc         ebx
  000000006506EC4C: 49 8D 8F B8 01 00  lea         rcx,[r15+00000000000001B8h]
                    00
  000000006506EC53: FF 15 5F 7C 62 00  call        qword ptr [00000000656968B8h]
  000000006506EC59: 3B D8              cmp         ebx,eax
  000000006506EC5B: 7C C3              jl          000000006506EC20
  000000006506EC5D: 48 8B 1D 64 7C 62  mov         rbx,qword ptr [00000000656968C8h]
                    00
  000000006506EC64: 48 89 9C 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rbx
                    00 00
  000000006506EC6C: 48 8B CB           mov         rcx,rbx
  000000006506EC6F: FF 15 4B 7C 62 00  call        qword ptr [00000000656968C0h]
  000000006506EC75: 90                 nop
  000000006506EC76: 49 39 9F B8 01 00  cmp         qword ptr [r15+00000000000001B8h],rbx
                    00
  000000006506EC7D: 74 44              je          000000006506ECC3
  000000006506EC7F: 48 8B CB           mov         rcx,rbx
  000000006506EC82: FF 15 38 7C 62 00  call        qword ptr [00000000656968C0h]
  000000006506EC88: 49 8B 8F B8 01 00  mov         rcx,qword ptr [r15+00000000000001B8h]
                    00
  000000006506EC8F: FF 15 E3 7C 62 00  call        qword ptr [0000000065696978h]
  000000006506EC95: 84 C0              test        al,al
  000000006506EC97: 75 0D              jne         000000006506ECA6
  000000006506EC99: 49 8B 8F B8 01 00  mov         rcx,qword ptr [r15+00000000000001B8h]
                    00
  000000006506ECA0: FF 15 BA 7C 62 00  call        qword ptr [0000000065696960h]
  000000006506ECA6: 49 89 9F B8 01 00  mov         qword ptr [r15+00000000000001B8h],rbx
                    00
  000000006506ECAD: F6 43 10 01        test        byte ptr [rbx+10h],1
  000000006506ECB1: 75 10              jne         000000006506ECC3
  000000006506ECB3: 8B 53 04           mov         edx,dword ptr [rbx+4]
  000000006506ECB6: 49 8D 8F B8 01 00  lea         rcx,[r15+00000000000001B8h]
                    00
  000000006506ECBD: E8 AE 07 4A 00     call        000000006550F470
  000000006506ECC2: 90                 nop
  000000006506ECC3: 48 8B CB           mov         rcx,rbx
  000000006506ECC6: FF 15 AC 7C 62 00  call        qword ptr [0000000065696978h]
  000000006506ECCC: 84 C0              test        al,al
  000000006506ECCE: 75 09              jne         000000006506ECD9
  000000006506ECD0: 48 8B CB           mov         rcx,rbx
  000000006506ECD3: FF 15 87 7C 62 00  call        qword ptr [0000000065696960h]
  000000006506ECD9: 48 8B 05 80 7F 62  mov         rax,qword ptr [0000000065696C60h]
                    00
  000000006506ECE0: 80 38 00           cmp         byte ptr [rax],0
  000000006506ECE3: 75 4E              jne         000000006506ED33
  000000006506ECE5: 49 8B 46 08        mov         rax,qword ptr [r14+8]
  000000006506ECE9: 8B 88 C8 01 00 00  mov         ecx,dword ptr [rax+00000000000001C8h]
  000000006506ECEF: C1 E9 12           shr         ecx,12h
  000000006506ECF2: 80 E1 01           and         cl,1
  000000006506ECF5: 74 3C              je          000000006506ED33
  000000006506ECF7: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  000000006506ECFF: E8 8C 1C FD FF     call        0000000065040990
  000000006506ED04: 48 8B D8           mov         rbx,rax
  000000006506ED07: FF 15 13 7B 62 00  call        qword ptr [0000000065696820h]
  000000006506ED0D: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006506ED11: 48 81 C1 20 01 00  add         rcx,120h
                    00
  000000006506ED18: 4C 8B CB           mov         r9,rbx
  000000006506ED1B: 4D 8B C6           mov         r8,r14
  000000006506ED1E: 33 D2              xor         edx,edx
  000000006506ED20: E8 9B FD FD FF     call        000000006504EAC0
  000000006506ED25: 90                 nop
  000000006506ED26: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  000000006506ED2E: E8 9D 1D FD FF     call        0000000065040AD0
  000000006506ED33: 49 8B 8F A8 00 00  mov         rcx,qword ptr [r15+00000000000000A8h]
                    00
  000000006506ED3A: 48 85 C9           test        rcx,rcx
  000000006506ED3D: 74 0B              je          000000006506ED4A
  000000006506ED3F: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006506ED42: BA 01 00 00 00     mov         edx,1
  000000006506ED47: FF 50 18           call        qword ptr [rax+18h]
  000000006506ED4A: 49 89 B7 A8 00 00  mov         qword ptr [r15+00000000000000A8h],rsi
                    00
  000000006506ED51: 49 8B 8F 90 00 00  mov         rcx,qword ptr [r15+0000000000000090h]
                    00
  000000006506ED58: 49 3B CE           cmp         rcx,r14
  000000006506ED5B: 74 39              je          000000006506ED96
  000000006506ED5D: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006506ED61: 49 8B 87 98 00 00  mov         rax,qword ptr [r15+0000000000000098h]
                    00
  000000006506ED68: 48 89 81 98 00 00  mov         qword ptr [rcx+0000000000000098h],rax
                    00
  000000006506ED6F: 49 8B 87 98 00 00  mov         rax,qword ptr [r15+0000000000000098h]
                    00
  000000006506ED76: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006506ED7A: 49 8B 87 90 00 00  mov         rax,qword ptr [r15+0000000000000090h]
                    00
  000000006506ED81: 48 89 81 90 00 00  mov         qword ptr [rcx+0000000000000090h],rax
                    00
  000000006506ED88: 49 89 B7 98 00 00  mov         qword ptr [r15+0000000000000098h],rsi
                    00
  000000006506ED8F: 49 89 B7 90 00 00  mov         qword ptr [r15+0000000000000090h],rsi
                    00
  000000006506ED96: 4C 39 35 3B A9 95  cmp         qword ptr [00000000659C96D8h],r14
                    00
  000000006506ED9D: 75 0E              jne         000000006506EDAD
  000000006506ED9F: 48 89 35 32 A9 95  mov         qword ptr [00000000659C96D8h],rsi
                    00
  000000006506EDA6: FF 15 E4 86 62 00  call        qword ptr [0000000065697490h]
  000000006506EDAC: 90                 nop
  000000006506EDAD: 49 8B CE           mov         rcx,r14
  000000006506EDB0: E8 BB 0D FF FF     call        000000006505FB70
  000000006506EDB5: 90                 nop
  000000006506EDB6: EB 12              jmp         000000006506EDCA
  000000006506EDB8: 33 F6              xor         esi,esi
  000000006506EDBA: 4C 8B B4 24 D0 00  mov         r14,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006506EDC2: 4C 8B BC 24 E0 00  mov         r15,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000006506EDCA: 49 8B 7F 08        mov         rdi,qword ptr [r15+8]
  000000006506EDCE: 41 81 8F D8 01 00  or          dword ptr [r15+00000000000001D8h],10000h
                    00 00 00 01 00
  000000006506EDD9: 48 8B DF           mov         rbx,rdi
  000000006506EDDC: 0F 1F 40 00        nop         dword ptr [rax]
  000000006506EDE0: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006506EDE4: 48 83 B9 D8 00 00  cmp         qword ptr [rcx+00000000000000D8h],0
                    00 00
  000000006506EDEC: 74 37              je          000000006506EE25
  000000006506EDEE: 48 8B 89 D8 00 00  mov         rcx,qword ptr [rcx+00000000000000D8h]
                    00
  000000006506EDF5: E8 36 27 48 00     call        00000000654F1530
  000000006506EDFA: 48 8B 50 08        mov         rdx,qword ptr [rax+8]
  000000006506EDFE: 80 BA 08 01 00 00  cmp         byte ptr [rdx+0000000000000108h],0
                    00
  000000006506EE05: 75 1E              jne         000000006506EE25
  000000006506EE07: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506EE0B: 48 8B 88 D8 00 00  mov         rcx,qword ptr [rax+00000000000000D8h]
                    00
  000000006506EE12: E8 19 27 48 00     call        00000000654F1530
  000000006506EE17: BA 02 00 00 00     mov         edx,2
  000000006506EE1C: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006506EE20: E8 2B B2 55 00     call        00000000655CA050
  000000006506EE25: 48 8B CB           mov         rcx,rbx
  000000006506EE28: FF 15 3A 7A 62 00  call        qword ptr [0000000065696868h]
  000000006506EE2E: 48 8B D8           mov         rbx,rax
  000000006506EE31: 48 85 C0           test        rax,rax
  000000006506EE34: 75 AA              jne         000000006506EDE0
  000000006506EE36: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006506EE3A: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506EE3E: A8 01              test        al,1
  000000006506EE40: 75 20              jne         000000006506EE62
  000000006506EE42: 48 8B CF           mov         rcx,rdi
  000000006506EE45: FF 15 1D 7A 62 00  call        qword ptr [0000000065696868h]
  000000006506EE4B: 48 85 C0           test        rax,rax
  000000006506EE4E: 74 12              je          000000006506EE62
  000000006506EE50: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006506EE54: F6 81 DA 01 00 00  test        byte ptr [rcx+00000000000001DAh],1
                    01
  000000006506EE5B: 75 05              jne         000000006506EE62
  000000006506EE5D: E8 5E FD FE FF     call        000000006505EBC0
  000000006506EE62: 49 8B 4E 20        mov         rcx,qword ptr [r14+20h]
  000000006506EE66: 0F B6 41 0C        movzx       eax,byte ptr [rcx+0Ch]
  000000006506EE6A: A8 01              test        al,1
  000000006506EE6C: 74 31              je          000000006506EE9F
  000000006506EE6E: 8B 41 08           mov         eax,dword ptr [rcx+8]
  000000006506EE71: C1 E8 0F           shr         eax,0Fh
  000000006506EE74: 24 01              and         al,1
  000000006506EE76: 74 27              je          000000006506EE9F
  000000006506EE78: 48 83 39 00        cmp         qword ptr [rcx],0
  000000006506EE7C: 74 21              je          000000006506EE9F
  000000006506EE7E: 33 D2              xor         edx,edx
  000000006506EE80: 49 8B CF           mov         rcx,r15
  000000006506EE83: E8 48 D7 FF FF     call        000000006506C5D0
  000000006506EE88: 90                 nop
  000000006506EE89: EB 36              jmp         000000006506EEC1
  000000006506EE8B: 33 F6              xor         esi,esi
  000000006506EE8D: 4C 8B B4 24 D0 00  mov         r14,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006506EE95: 4C 8B BC 24 E0 00  mov         r15,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000006506EE9D: EB 22              jmp         000000006506EEC1
  000000006506EE9F: 48 83 39 00        cmp         qword ptr [rcx],0
  000000006506EEA3: 75 1C              jne         000000006506EEC1
  000000006506EEA5: 8B 41 08           mov         eax,dword ptr [rcx+8]
  000000006506EEA8: C1 E8 0F           shr         eax,0Fh
  000000006506EEAB: 24 01              and         al,1
  000000006506EEAD: 74 12              je          000000006506EEC1
  000000006506EEAF: FF 15 6B 79 62 00  call        qword ptr [0000000065696820h]
  000000006506EEB5: 49 8B D6           mov         rdx,r14
  000000006506EEB8: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006506EEBC: E8 FF 4E FB FF     call        0000000065023DC0
  000000006506EEC1: 49 8B 5F 08        mov         rbx,qword ptr [r15+8]
  000000006506EEC5: 48 8B CB           mov         rcx,rbx
  000000006506EEC8: FF 15 9A 79 62 00  call        qword ptr [0000000065696868h]
  000000006506EECE: 4C 8B D8           mov         r11,rax
  000000006506EED1: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  000000006506EED5: 0F B6 49 0C        movzx       ecx,byte ptr [rcx+0Ch]
  000000006506EED9: F6 C1 01           test        cl,1
  000000006506EEDC: 75 22              jne         000000006506EF00
  000000006506EEDE: 66 90              nop
  000000006506EEE0: 4D 85 DB           test        r11,r11
  000000006506EEE3: 74 1B              je          000000006506EF00
  000000006506EEE5: 49 8B DB           mov         rbx,r11
  000000006506EEE8: 49 8B CB           mov         rcx,r11
  000000006506EEEB: FF 15 77 79 62 00  call        qword ptr [0000000065696868h]
  000000006506EEF1: 4C 8B D8           mov         r11,rax
  000000006506EEF4: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006506EEF8: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506EEFC: A8 01              test        al,1
  000000006506EEFE: 74 E0              je          000000006506EEE0
  000000006506EF00: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506EF04: 48 8B 98 88 00 00  mov         rbx,qword ptr [rax+0000000000000088h]
                    00
  000000006506EF0B: 48 85 DB           test        rbx,rbx
  000000006506EF0E: 74 0F              je          000000006506EF1F
  000000006506EF10: 48 8B 5B 08        mov         rbx,qword ptr [rbx+8]
  000000006506EF14: 48 85 DB           test        rbx,rbx
  000000006506EF17: 74 06              je          000000006506EF1F
  000000006506EF19: 48 8B 5B 10        mov         rbx,qword ptr [rbx+10h]
  000000006506EF1D: EB 03              jmp         000000006506EF22
  000000006506EF1F: 48 8B DE           mov         rbx,rsi
  000000006506EF22: 48 85 DB           test        rbx,rbx
  000000006506EF25: 74 33              je          000000006506EF5A
  000000006506EF27: 49 8B D6           mov         rdx,r14
  000000006506EF2A: 48 8B CB           mov         rcx,rbx
  000000006506EF2D: E8 EE 1A 18 00     call        00000000651F0A20
  000000006506EF32: 4D 8B 5E 20        mov         r11,qword ptr [r14+20h]
  000000006506EF36: 41 8B 43 08        mov         eax,dword ptr [r11+8]
  000000006506EF3A: C1 E8 05           shr         eax,5
  000000006506EF3D: 24 01              and         al,1
  000000006506EF3F: 74 19              je          000000006506EF5A
  000000006506EF41: 4C 89 B4 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],r14
                    00 00
  000000006506EF49: 48 8D 4B 30        lea         rcx,[rbx+30h]
  000000006506EF4D: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  000000006506EF55: E8 16 33 2F 00     call        0000000065362270
  000000006506EF5A: 49 8B 9F B0 00 00  mov         rbx,qword ptr [r15+00000000000000B0h]
                    00
  000000006506EF61: 48 85 DB           test        rbx,rbx
  000000006506EF64: 74 10              je          000000006506EF76
  000000006506EF66: 48 8B CB           mov         rcx,rbx
  000000006506EF69: E8 B2 A4 11 00     call        0000000065189420
  000000006506EF6E: 48 8B CB           mov         rcx,rbx
  000000006506EF71: E8 38 21 57 00     call        00000000655E10AE
  000000006506EF76: 49 89 B7 B0 00 00  mov         qword ptr [r15+00000000000000B0h],rsi
                    00
  000000006506EF7D: 41 0F BA 67 20 09  bt          dword ptr [r15+20h],9
  000000006506EF83: 73 09              jae         000000006506EF8E
  000000006506EF85: 49 8B CE           mov         rcx,r14
  000000006506EF88: FF 15 62 85 62 00  call        qword ptr [00000000656974F0h]
  000000006506EF8E: 49 8B 4F 78        mov         rcx,qword ptr [r15+78h]
  000000006506EF92: 48 85 C9           test        rcx,rcx
  000000006506EF95: 74 10              je          000000006506EFA7
  000000006506EF97: 49 8B D6           mov         rdx,r14
  000000006506EF9A: 48 8B 05 47 85 62  mov         rax,qword ptr [00000000656974E8h]
                    00
  000000006506EFA1: FF 10              call        qword ptr [rax]
  000000006506EFA3: 49 89 77 78        mov         qword ptr [r15+78h],rsi
  000000006506EFA7: 49 8D 4F 18        lea         rcx,[r15+18h]
  000000006506EFAB: FF 15 1F 79 62 00  call        qword ptr [00000000656968D0h]
  000000006506EFB1: 84 C0              test        al,al
  000000006506EFB3: 75 09              jne         000000006506EFBE
  000000006506EFB5: 49 8B CF           mov         rcx,r15
  000000006506EFB8: FF 15 22 85 62 00  call        qword ptr [00000000656974E0h]
  000000006506EFBE: 33 D2              xor         edx,edx
  000000006506EFC0: 41 B8 01 80 00 00  mov         r8d,8001h
  000000006506EFC6: 49 8B CE           mov         rcx,r14
  000000006506EFC9: E8 92 39 43 00     call        00000000654A2960
  000000006506EFCE: 49 8B CE           mov         rcx,r14
  000000006506EFD1: FF 15 01 85 62 00  call        qword ptr [00000000656974D8h]
  000000006506EFD7: 90                 nop
  000000006506EFD8: 41 B0 01           mov         r8b,1
  000000006506EFDB: 41 0F B6 D0        movzx       edx,r8b
  000000006506EFDF: 49 8B CE           mov         rcx,r14
  000000006506EFE2: E8 89 A5 02 00     call        0000000065099570
  000000006506EFE7: 90                 nop
  000000006506EFE8: EB 08              jmp         000000006506EFF2
  000000006506EFEA: 4C 8B B4 24 D0 00  mov         r14,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006506EFF2: FF 0D 50 A9 95 00  dec         dword ptr [00000000659C9948h]
  000000006506EFF8: 48 8B 0D 41 A9 95  mov         rcx,qword ptr [00000000659C9940h]
                    00
  000000006506EFFF: 48 85 C9           test        rcx,rcx
  000000006506F002: 74 16              je          000000006506F01A
  000000006506F004: 4C 89 B4 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],r14
                    00 00
  000000006506F00C: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  000000006506F014: E8 67 6D 46 00     call        00000000654D5D80
  000000006506F019: 90                 nop
  000000006506F01A: BA 10 00 00 00     mov         edx,10h
  000000006506F01F: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000006506F024: FF 15 EE 77 62 00  call        qword ptr [0000000065696818h]
  000000006506F02A: 90                 nop
  000000006506F02B: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  000000006506F030: 49 8B CE           mov         rcx,r14
  000000006506F033: FF 15 D7 77 62 00  call        qword ptr [0000000065696810h]
  000000006506F039: 90                 nop
  000000006506F03A: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000006506F03F: FF 15 C3 77 62 00  call        qword ptr [0000000065696808h]
  000000006506F045: 90                 nop
  000000006506F046: EB 08              jmp         000000006506F050
  000000006506F048: 4C 8B B4 24 D0 00  mov         r14,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000006506F050: 49 8D 4E 10        lea         rcx,[r14+10h]
  000000006506F054: E8 37 91 0C 00     call        0000000065138190
  000000006506F059: 90                 nop
  000000006506F05A: 49 8B CE           mov         rcx,r14
  000000006506F05D: 48 81 C4 90 00 00  add         rsp,90h
                    00
  000000006506F064: 41 5F              pop         r15
  000000006506F066: 41 5E              pop         r14
  000000006506F068: 41 5D              pop         r13
  000000006506F06A: 41 5C              pop         r12
  000000006506F06C: 5F                 pop         rdi
  000000006506F06D: 5E                 pop         rsi
  000000006506F06E: 5B                 pop         rbx
  000000006506F06F: 48 FF 25 CA 76 62  jmp         qword ptr [0000000065696740h]
                    00

  Summary

      695000 .text
