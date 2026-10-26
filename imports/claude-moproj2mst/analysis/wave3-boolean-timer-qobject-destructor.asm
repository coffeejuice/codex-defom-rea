Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712F870: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006712F875: 53                 push        rbx
  000000006712F876: 56                 push        rsi
  000000006712F877: 57                 push        rdi
  000000006712F878: 41 54              push        r12
  000000006712F87A: 41 55              push        r13
  000000006712F87C: 41 56              push        r14
  000000006712F87E: 41 57              push        r15
  000000006712F880: 48 83 EC 60        sub         rsp,60h
  000000006712F884: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006712F88D: 48 8B F9           mov         rdi,rcx
  000000006712F890: 48 8D 05 31 D2 10  lea         rax,[000000006723CAC8h]
                    00
  000000006712F897: 48 89 01           mov         qword ptr [rcx],rax
  000000006712F89A: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  000000006712F89E: 4C 89 64 24 40     mov         qword ptr [rsp+40h],r12
  000000006712F8A3: 41 8B 44 24 20     mov         eax,dword ptr [r12+20h]
  000000006712F8A8: 83 E0 FB           and         eax,0FFFFFFFBh
  000000006712F8AB: 83 C8 08           or          eax,8
  000000006712F8AE: 41 89 44 24 20     mov         dword ptr [r12+20h],eax
  000000006712F8B3: 0F BA E0 09        bt          eax,9
  000000006712F8B7: 73 09              jae         000000006712F8C2
  000000006712F8B9: A8 01              test        al,1
  000000006712F8BB: 75 05              jne         000000006712F8C2
  000000006712F8BD: E8 5E E5 FF FF     call        000000006712DE20
  000000006712F8C2: 49 8B 84 24 80 00  mov         rax,qword ptr [r12+0000000000000080h]
                    00 00
  000000006712F8CA: 48 85 C0           test        rax,rax
  000000006712F8CD: 74 60              je          000000006712F92F
  000000006712F8CF: 49 8B 84 24 80 00  mov         rax,qword ptr [r12+0000000000000080h]
                    00 00
  000000006712F8D7: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000006712F8DA: 85 C0              test        eax,eax
  000000006712F8DC: 7E 0C              jle         000000006712F8EA
  000000006712F8DE: 48 8D 0D AB D4 10  lea         rcx,[000000006723CD90h]
                    00
  000000006712F8E5: E8 F6 D5 EE FF     call        000000006701CEE0
  000000006712F8EA: 49 8B 84 24 80 00  mov         rax,qword ptr [r12+0000000000000080h]
                    00 00
  000000006712F8F2: 45 33 ED           xor         r13d,r13d
  000000006712F8F5: 44 89 68 0C        mov         dword ptr [rax+0Ch],r13d
  000000006712F8F9: 49 8B 8C 24 80 00  mov         rcx,qword ptr [r12+0000000000000080h]
                    00 00
  000000006712F901: 83 CB FF           or          ebx,0FFFFFFFFh
  000000006712F904: F0 01 59 08        lock add    dword ptr [rcx+8],ebx
  000000006712F908: 75 1D              jne         000000006712F927
  000000006712F90A: 49 8B 8C 24 80 00  mov         rcx,qword ptr [r12+0000000000000080h]
                    00 00
  000000006712F912: 48 85 C9           test        rcx,rcx
  000000006712F915: 74 10              je          000000006712F927
  000000006712F917: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006712F91A: 41 BF 01 00 00 00  mov         r15d,1
  000000006712F920: 41 8B D7           mov         edx,r15d
  000000006712F923: FF 10              call        qword ptr [rax]
  000000006712F925: EB 12              jmp         000000006712F939
  000000006712F927: 41 BF 01 00 00 00  mov         r15d,1
  000000006712F92D: EB 0A              jmp         000000006712F939
  000000006712F92F: 45 33 ED           xor         r13d,r13d
  000000006712F932: 83 CB FF           or          ebx,0FFFFFFFFh
  000000006712F935: 45 8D 7D 01        lea         r15d,[r13+1]
  000000006712F939: 41 F6 44 24 60 01  test        byte ptr [r12+60h],1
  000000006712F93F: 75 14              jne         000000006712F955
  000000006712F941: 48 83 3D B7 48 1C  cmp         qword ptr [00000000672F4200h],0
                    00 00
  000000006712F949: 75 0A              jne         000000006712F955
  000000006712F94B: 48 83 3D BD 48 1C  cmp         qword ptr [00000000672F4210h],0
                    00 00
  000000006712F953: 74 2C              je          000000006712F981
  000000006712F955: 48 89 7C 24 38     mov         qword ptr [rsp+38h],rdi
  000000006712F95A: 4C 89 6C 24 50     mov         qword ptr [rsp+50h],r13
  000000006712F95F: 48 8D 44 24 38     lea         rax,[rsp+38h]
  000000006712F964: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000006712F969: 4C 8D 4C 24 50     lea         r9,[rsp+50h]
  000000006712F96E: 45 33 C0           xor         r8d,r8d
  000000006712F971: 48 8D 15 78 C6 10  lea         rdx,[000000006723BFF0h]
                    00
  000000006712F978: 48 8B CF           mov         rcx,rdi
  000000006712F97B: E8 F0 F2 FF FF     call        000000006712EC70
  000000006712F980: 90                 nop
  000000006712F981: 49 8B 4C 24 78     mov         rcx,qword ptr [r12+78h]
  000000006712F986: 48 85 C9           test        rcx,rcx
  000000006712F989: 74 09              je          000000006712F994
  000000006712F98B: 48 8B D7           mov         rdx,rdi
  000000006712F98E: FF 15 BC 49 1C 00  call        qword ptr [00000000672F4350h]
  000000006712F994: 49 8B 44 24 58     mov         rax,qword ptr [r12+58h]
  000000006712F999: 48 85 C0           test        rax,rax
  000000006712F99C: 74 04              je          000000006712F9A2
  000000006712F99E: 44 89 68 0C        mov         dword ptr [rax+0Ch],r13d
  000000006712F9A2: 4D 89 6C 24 58     mov         qword ptr [r12+58h],r13
  000000006712F9A7: 49 83 7C 24 48 00  cmp         qword ptr [r12+48h],0
  000000006712F9AD: 75 0C              jne         000000006712F9BB
  000000006712F9AF: 49 83 7C 24 50 00  cmp         qword ptr [r12+50h],0
  000000006712F9B5: 0F 84 85 03 00 00  je          000000006712FD40
  000000006712F9BB: 48 8B CF           mov         rcx,rdi
  000000006712F9BE: E8 FD A9 FF FF     call        000000006712A3C0
  000000006712F9C3: 48 8B F0           mov         rsi,rax
  000000006712F9C6: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006712F9CB: 48 85 C0           test        rax,rax
  000000006712F9CE: 74 37              je          000000006712FA07
  000000006712F9D0: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006712F9D3: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712F9D7: 74 0A              je          000000006712F9E3
  000000006712F9D9: 48 8B C8           mov         rcx,rax
  000000006712F9DC: E8 CF 63 EF FF     call        0000000067025DB0
  000000006712F9E1: EB 16              jmp         000000006712F9F9
  000000006712F9E3: 33 C0              xor         eax,eax
  000000006712F9E5: F0 44 0F B1 39     lock cmpxchg dword ptr [rcx],r15d
  000000006712F9EA: 0F 94 C1           sete        cl
  000000006712F9ED: 84 C9              test        cl,cl
  000000006712F9EF: 75 08              jne         000000006712F9F9
  000000006712F9F1: 48 8B CE           mov         rcx,rsi
  000000006712F9F4: E8 B7 61 EF FF     call        0000000067025BB0
  000000006712F9F9: 48 8B C6           mov         rax,rsi
  000000006712F9FC: 48 83 C8 01        or          rax,1
  000000006712FA00: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006712FA05: EB 05              jmp         000000006712FA0C
  000000006712FA07: 4C 89 6C 24 20     mov         qword ptr [rsp+20h],r13
  000000006712FA0C: 49 8B 44 24 48     mov         rax,qword ptr [r12+48h]
  000000006712FA11: 48 85 C0           test        rax,rax
  000000006712FA14: 0F 84 DA 01 00 00  je          000000006712FBF4
  000000006712FA1A: FF 40 0C           inc         dword ptr [rax+0Ch]
  000000006712FA1D: 49 8B 44 24 48     mov         rax,qword ptr [r12+48h]
  000000006712FA22: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006712FA25: 8B 41 08           mov         eax,dword ptr [rcx+8]
  000000006712FA28: 89 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],eax
                    00
  000000006712FA2F: 44 8B F3           mov         r14d,ebx
  000000006712FA32: 89 9C 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],ebx
                    00
  000000006712FA39: 83 F8 FF           cmp         eax,0FFFFFFFFh
  000000006712FA3C: 0F 8E 57 01 00 00  jle         000000006712FB99
  000000006712FA42: 48 C7 C7 F0 FF FF  mov         rdi,0FFFFFFFFFFFFFFF0h
                    FF
  000000006712FA49: 48 89 7C 24 28     mov         qword ptr [rsp+28h],rdi
  000000006712FA4E: 66 90              nop
  000000006712FA50: 49 8B 5C 24 48     mov         rbx,qword ptr [r12+48h]
  000000006712FA55: 45 85 F6           test        r14d,r14d
  000000006712FA58: 79 06              jns         000000006712FA60
  000000006712FA5A: 48 8D 73 10        lea         rsi,[rbx+10h]
  000000006712FA5E: EB 2B              jmp         000000006712FA8B
  000000006712FA60: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712FA63: 8B 08              mov         ecx,dword ptr [rax]
  000000006712FA65: 83 F9 01           cmp         ecx,1
  000000006712FA68: 74 12              je          000000006712FA7C
  000000006712FA6A: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712FA6D: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006712FA71: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006712FA74: 48 8B CB           mov         rcx,rbx
  000000006712FA77: E8 44 AF FF FF     call        000000006712A9C0
  000000006712FA7C: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712FA7F: 48 8D 74 38 10     lea         rsi,[rax+rdi+10h]
  000000006712FA84: 8B 84 24 A8 00 00  mov         eax,dword ptr [rsp+00000000000000A8h]
                    00
  000000006712FA8B: 48 8B 1E           mov         rbx,qword ptr [rsi]
  000000006712FA8E: 48 85 DB           test        rbx,rbx
  000000006712FA91: 0F 84 E0 00 00 00  je          000000006712FB77
  000000006712FA97: 4C 8B 64 24 30     mov         r12,qword ptr [rsp+30h]
  000000006712FA9C: 4C 8D 35 9D 48 1C  lea         r14,[00000000672F4340h]
                    00
  000000006712FAA3: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006712FAA7: 48 85 C9           test        rcx,rcx
  000000006712FAAA: 75 26              jne         000000006712FAD2
  000000006712FAAC: 48 8B 43 18        mov         rax,qword ptr [rbx+18h]
  000000006712FAB0: 48 89 06           mov         qword ptr [rsi],rax
  000000006712FAB3: 48 8B 43 30        mov         rax,qword ptr [rbx+30h]
  000000006712FAB7: 49 3B C6           cmp         rax,r14
  000000006712FABA: 74 09              je          000000006712FAC5
  000000006712FABC: 48 8B 4B 30        mov         rcx,qword ptr [rbx+30h]
  000000006712FAC0: E8 97 8E 04 00     call        000000006717895C
  000000006712FAC5: 48 8B CB           mov         rcx,rbx
  000000006712FAC8: E8 89 8E 04 00     call        0000000067178956
  000000006712FACD: E9 80 00 00 00     jmp         000000006712FB52
  000000006712FAD2: E8 E9 A8 FF FF     call        000000006712A3C0
  000000006712FAD7: 48 8B F8           mov         rdi,rax
  000000006712FADA: 48 8B D0           mov         rdx,rax
  000000006712FADD: 49 8B CC           mov         rcx,r12
  000000006712FAE0: E8 2B 9D FF FF     call        0000000067129810
  000000006712FAE5: 48 83 7B 08 00     cmp         qword ptr [rbx+8],0
  000000006712FAEA: 74 1C              je          000000006712FB08
  000000006712FAEC: 48 8B 53 28        mov         rdx,qword ptr [rbx+28h]
  000000006712FAF0: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  000000006712FAF4: 48 89 0A           mov         qword ptr [rdx],rcx
  000000006712FAF7: 48 8B 53 20        mov         rdx,qword ptr [rbx+20h]
  000000006712FAFB: 48 85 D2           test        rdx,rdx
  000000006712FAFE: 74 08              je          000000006712FB08
  000000006712FB00: 48 8B 4B 28        mov         rcx,qword ptr [rbx+28h]
  000000006712FB04: 48 89 4A 28        mov         qword ptr [rdx+28h],rcx
  000000006712FB08: 84 C0              test        al,al
  000000006712FB0A: 74 25              je          000000006712FB31
  000000006712FB0C: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006712FB0F: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712FB13: 74 0A              je          000000006712FB1F
  000000006712FB15: 48 8B CF           mov         rcx,rdi
  000000006712FB18: E8 13 60 EF FF     call        0000000067025B30
  000000006712FB1D: EB 12              jmp         000000006712FB31
  000000006712FB1F: 41 8B C7           mov         eax,r15d
  000000006712FB22: F0 44 0F B1 29     lock cmpxchg dword ptr [rcx],r13d
  000000006712FB27: 74 08              je          000000006712FB31
  000000006712FB29: 48 8B CF           mov         rcx,rdi
  000000006712FB2C: E8 3F 60 EF FF     call        0000000067025B70
  000000006712FB31: 48 8B 43 18        mov         rax,qword ptr [rbx+18h]
  000000006712FB35: 48 89 06           mov         qword ptr [rsi],rax
  000000006712FB38: 48 8B 43 30        mov         rax,qword ptr [rbx+30h]
  000000006712FB3C: 49 3B C6           cmp         rax,r14
  000000006712FB3F: 74 09              je          000000006712FB4A
  000000006712FB41: 48 8B 4B 30        mov         rcx,qword ptr [rbx+30h]
  000000006712FB45: E8 12 8E 04 00     call        000000006717895C
  000000006712FB4A: 48 8B CB           mov         rcx,rbx
  000000006712FB4D: E8 04 8E 04 00     call        0000000067178956
  000000006712FB52: 48 8B 1E           mov         rbx,qword ptr [rsi]
  000000006712FB55: 48 85 DB           test        rbx,rbx
  000000006712FB58: 0F 85 45 FF FF FF  jne         000000006712FAA3
  000000006712FB5E: 4C 8B 64 24 40     mov         r12,qword ptr [rsp+40h]
  000000006712FB63: 44 8B B4 24 B0 00  mov         r14d,dword ptr [rsp+00000000000000B0h]
                    00 00
  000000006712FB6B: 48 8B 7C 24 28     mov         rdi,qword ptr [rsp+28h]
  000000006712FB70: 8B 84 24 A8 00 00  mov         eax,dword ptr [rsp+00000000000000A8h]
                    00
  000000006712FB77: 41 FF C6           inc         r14d
  000000006712FB7A: 44 89 B4 24 B0 00  mov         dword ptr [rsp+00000000000000B0h],r14d
                    00 00
  000000006712FB82: 48 83 C7 10        add         rdi,10h
  000000006712FB86: 48 89 7C 24 28     mov         qword ptr [rsp+28h],rdi
  000000006712FB8B: 44 3B F0           cmp         r14d,eax
  000000006712FB8E: 0F 8C BC FE FF FF  jl          000000006712FA50
  000000006712FB94: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  000000006712FB99: 49 8B 44 24 48     mov         rax,qword ptr [r12+48h]
  000000006712FB9E: FF 48 0C           dec         dword ptr [rax+0Ch]
  000000006712FBA1: 49 8B 5C 24 48     mov         rbx,qword ptr [r12+48h]
  000000006712FBA6: 83 7B 0C 00        cmp         dword ptr [rbx+0Ch],0
  000000006712FBAA: 75 33              jne         000000006712FBDF
  000000006712FBAC: 48 85 DB           test        rbx,rbx
  000000006712FBAF: 74 38              je          000000006712FBE9
  000000006712FBB1: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006712FBB4: 48 85 C9           test        rcx,rcx
  000000006712FBB7: 74 18              je          000000006712FBD1
  000000006712FBB9: 41 83 CE FF        or          r14d,0FFFFFFFFh
  000000006712FBBD: F0 44 01 31        lock add    dword ptr [rcx],r14d
  000000006712FBC1: 75 12              jne         000000006712FBD5
  000000006712FBC3: 41 8D 56 09        lea         edx,[r14+9]
  000000006712FBC7: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006712FBCA: E8 C1 C9 F4 FF     call        000000006707C590
  000000006712FBCF: EB 04              jmp         000000006712FBD5
  000000006712FBD1: 41 83 CE FF        or          r14d,0FFFFFFFFh
  000000006712FBD5: 48 8B CB           mov         rcx,rbx
  000000006712FBD8: E8 79 8D 04 00     call        0000000067178956
  000000006712FBDD: EB 0E              jmp         000000006712FBED
  000000006712FBDF: C6 43 08 01        mov         byte ptr [rbx+8],1
  000000006712FBE3: 41 83 CE FF        or          r14d,0FFFFFFFFh
  000000006712FBE7: EB 04              jmp         000000006712FBED
  000000006712FBE9: 41 83 CE FF        or          r14d,0FFFFFFFFh
  000000006712FBED: 4D 89 6C 24 48     mov         qword ptr [r12+48h],r13
  000000006712FBF2: EB 04              jmp         000000006712FBF8
  000000006712FBF4: 41 83 CE FF        or          r14d,0FFFFFFFFh
  000000006712FBF8: 49 8B 44 24 50     mov         rax,qword ptr [r12+50h]
  000000006712FBFD: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  000000006712FC05: 48 85 C0           test        rax,rax
  000000006712FC08: 0F 84 D6 00 00 00  je          000000006712FCE4
  000000006712FC0E: 66 90              nop
  000000006712FC10: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000006712FC18: 48 8B 38           mov         rdi,qword ptr [rax]
  000000006712FC1B: 48 8B CF           mov         rcx,rdi
  000000006712FC1E: E8 9D A7 FF FF     call        000000006712A3C0
  000000006712FC23: 48 8B D8           mov         rbx,rax
  000000006712FC26: 48 8D 84 24 B8 00  lea         rax,[rsp+00000000000000B8h]
                    00 00
  000000006712FC2E: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000006712FC36: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  000000006712FC3A: 48 8B D3           mov         rdx,rbx
  000000006712FC3D: 48 8B CE           mov         rcx,rsi
  000000006712FC40: E8 CB 9B FF FF     call        0000000067129810
  000000006712FC45: 44 0F B6 D8        movzx       r11d,al
  000000006712FC49: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000006712FC51: 48 85 C9           test        rcx,rcx
  000000006712FC54: 74 5A              je          000000006712FCB0
  000000006712FC56: 48 39 39           cmp         qword ptr [rcx],rdi
  000000006712FC59: 75 55              jne         000000006712FCB0
  000000006712FC5B: 4C 89 69 08        mov         qword ptr [rcx+8],r13
  000000006712FC5F: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006712FC63: 48 8B 51 48        mov         rdx,qword ptr [rcx+48h]
  000000006712FC67: 48 85 D2           test        rdx,rdx
  000000006712FC6A: 74 04              je          000000006712FC70
  000000006712FC6C: C6 42 09 01        mov         byte ptr [rdx+9],1
  000000006712FC70: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000006712FC78: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  000000006712FC7C: 48 89 8C 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rcx
                    00 00
  000000006712FC84: 45 84 DB           test        r11b,r11b
  000000006712FC87: 74 4C              je          000000006712FCD5
  000000006712FC89: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006712FC8C: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712FC90: 74 0A              je          000000006712FC9C
  000000006712FC92: 48 8B CB           mov         rcx,rbx
  000000006712FC95: E8 96 5E EF FF     call        0000000067025B30
  000000006712FC9A: EB 39              jmp         000000006712FCD5
  000000006712FC9C: 41 8B C7           mov         eax,r15d
  000000006712FC9F: F0 44 0F B1 29     lock cmpxchg dword ptr [rcx],r13d
  000000006712FCA4: 74 2F              je          000000006712FCD5
  000000006712FCA6: 48 8B CB           mov         rcx,rbx
  000000006712FCA9: E8 C2 5E EF FF     call        0000000067025B70
  000000006712FCAE: EB 25              jmp         000000006712FCD5
  000000006712FCB0: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006712FCB3: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712FCB7: 74 0A              je          000000006712FCC3
  000000006712FCB9: 48 8B CB           mov         rcx,rbx
  000000006712FCBC: E8 6F 5E EF FF     call        0000000067025B30
  000000006712FCC1: EB 12              jmp         000000006712FCD5
  000000006712FCC3: 41 8B C7           mov         eax,r15d
  000000006712FCC6: F0 44 0F B1 29     lock cmpxchg dword ptr [rcx],r13d
  000000006712FCCB: 74 08              je          000000006712FCD5
  000000006712FCCD: 48 8B CB           mov         rcx,rbx
  000000006712FCD0: E8 9B 5E EF FF     call        0000000067025B70
  000000006712FCD5: 48 83 BC 24 B8 00  cmp         qword ptr [rsp+00000000000000B8h],0
                    00 00 00
  000000006712FCDE: 0F 85 2C FF FF FF  jne         000000006712FC10
  000000006712FCE4: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006712FCE9: F6 C1 01           test        cl,1
  000000006712FCEC: 74 3E              je          000000006712FD2C
  000000006712FCEE: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712FCF2: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  000000006712FCF7: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712FCFB: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006712FCFE: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006712FD02: 74 0F              je          000000006712FD13
  000000006712FD04: E8 27 5E EF FF     call        0000000067025B30
  000000006712FD09: 48 8B BC 24 A0 00  mov         rdi,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712FD11: EB 31              jmp         000000006712FD44
  000000006712FD13: 41 8B C7           mov         eax,r15d
  000000006712FD16: F0 44 0F B1 2A     lock cmpxchg dword ptr [rdx],r13d
  000000006712FD1B: 74 19              je          000000006712FD36
  000000006712FD1D: E8 4E 5E EF FF     call        0000000067025B70
  000000006712FD22: 48 8B BC 24 A0 00  mov         rdi,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712FD2A: EB 18              jmp         000000006712FD44
  000000006712FD2C: 48 8B BC 24 A0 00  mov         rdi,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712FD34: EB 0E              jmp         000000006712FD44
  000000006712FD36: 48 8B BC 24 A0 00  mov         rdi,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712FD3E: EB 04              jmp         000000006712FD44
  000000006712FD40: 41 83 CE FF        or          r14d,0FFFFFFFFh
  000000006712FD44: 49 8B 4C 24 18     mov         rcx,qword ptr [r12+18h]
  000000006712FD49: 8B 41 08           mov         eax,dword ptr [rcx+8]
  000000006712FD4C: 39 41 0C           cmp         dword ptr [rcx+0Ch],eax
  000000006712FD4F: 74 08              je          000000006712FD59
  000000006712FD51: 49 8B CC           mov         rcx,r12
  000000006712FD54: E8 A7 D5 FF FF     call        000000006712D300
  000000006712FD59: F0 44 0F C1 35 E2  lock xadd   dword ptr [00000000672F4344h],r14d
                    45 1C 00
  000000006712FD62: 41 83 C6 FF        add         r14d,0FFFFFFFFh
  000000006712FD66: 75 1F              jne         000000006712FD87
  000000006712FD68: 49 8B DD           mov         rbx,r13
  000000006712FD6B: 48 87 1D D6 45 1C  xchg        rbx,qword ptr [00000000672F4348h]
                    00
  000000006712FD72: 48 85 DB           test        rbx,rbx
  000000006712FD75: 74 10              je          000000006712FD87
  000000006712FD77: 48 8B CB           mov         rcx,rbx
  000000006712FD7A: E8 C1 6F EF FF     call        0000000067026D40
  000000006712FD7F: 48 8B CB           mov         rcx,rbx
  000000006712FD82: E8 CF 8B 04 00     call        0000000067178956
  000000006712FD87: 49 83 7C 24 10 00  cmp         qword ptr [r12+10h],0
  000000006712FD8D: 74 0B              je          000000006712FD9A
  000000006712FD8F: 33 D2              xor         edx,edx
  000000006712FD91: 49 8B CC           mov         rcx,r12
  000000006712FD94: E8 57 D6 FF FF     call        000000006712D3F0
  000000006712FD99: 90                 nop
  000000006712FD9A: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006712FD9E: 48 85 C9           test        rcx,rcx
  000000006712FDA1: 74 08              je          000000006712FDAB
  000000006712FDA3: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006712FDA6: 41 8B D7           mov         edx,r15d
  000000006712FDA9: FF 10              call        qword ptr [rax]
  000000006712FDAB: 4C 89 6F 08        mov         qword ptr [rdi+8],r13
  000000006712FDAF: 48 83 C4 60        add         rsp,60h
  000000006712FDB3: 41 5F              pop         r15
  000000006712FDB5: 41 5E              pop         r14
  000000006712FDB7: 41 5D              pop         r13
  000000006712FDB9: 41 5C              pop         r12
  000000006712FDBB: 5F                 pop         rdi
  000000006712FDBC: 5E                 pop         rsi
  000000006712FDBD: 5B                 pop         rbx
  000000006712FDBE: C3                 ret

  Summary

      1A7000 .text
