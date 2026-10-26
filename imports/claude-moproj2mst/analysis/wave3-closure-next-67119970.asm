Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067119970: 48 85 C9           test        rcx,rcx
  0000000067119973: 0F 84 88 01 00 00  je          0000000067119B01
  0000000067119979: 57                 push        rdi
  000000006711997A: 41 54              push        r12
  000000006711997C: 41 55              push        r13
  000000006711997E: 48 83 EC 30        sub         rsp,30h
  0000000067119982: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711998B: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  0000000067119990: 48 89 6C 24 60     mov         qword ptr [rsp+60h],rbp
  0000000067119995: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  000000006711999A: 48 8B F9           mov         rdi,rcx
  000000006711999D: F6 41 12 01        test        byte ptr [rcx+12h],1
  00000000671199A1: 0F 84 42 01 00 00  je          0000000067119AE9
  00000000671199A7: E8 F4 12 F1 FF     call        000000006702ACA0
  00000000671199AC: 48 8B F0           mov         rsi,rax
  00000000671199AF: 48 8D 58 48        lea         rbx,[rax+48h]
  00000000671199B3: 45 33 E4           xor         r12d,r12d
  00000000671199B6: 45 8D 6C 24 01     lea         r13d,[r12+1]
  00000000671199BB: 48 85 DB           test        rbx,rbx
  00000000671199BE: 74 33              je          00000000671199F3
  00000000671199C0: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000671199C3: 44 84 69 04        test        byte ptr [rcx+4],r13b
  00000000671199C7: 74 0A              je          00000000671199D3
  00000000671199C9: 48 8B CB           mov         rcx,rbx
  00000000671199CC: E8 DF C3 F0 FF     call        0000000067025DB0
  00000000671199D1: EB 16              jmp         00000000671199E9
  00000000671199D3: 33 C0              xor         eax,eax
  00000000671199D5: F0 44 0F B1 29     lock cmpxchg dword ptr [rcx],r13d
  00000000671199DA: 0F 94 C1           sete        cl
  00000000671199DD: 84 C9              test        cl,cl
  00000000671199DF: 75 08              jne         00000000671199E9
  00000000671199E1: 48 8B CB           mov         rcx,rbx
  00000000671199E4: E8 C7 C1 F0 FF     call        0000000067025BB0
  00000000671199E9: 49 0B DD           or          rbx,r13
  00000000671199EC: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000671199F1: EB 08              jmp         00000000671199FB
  00000000671199F3: 49 8B DC           mov         rbx,r12
  00000000671199F6: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000671199FB: 45 8B C4           mov         r8d,r12d
  00000000671199FE: 48 8B 4E 30        mov         rcx,qword ptr [rsi+30h]
  0000000067119A02: 49 8B D4           mov         rdx,r12
  0000000067119A05: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  0000000067119A08: 2B 41 08           sub         eax,dword ptr [rcx+8]
  0000000067119A0B: 85 C0              test        eax,eax
  0000000067119A0D: 0F 8E 9F 00 00 00  jle         0000000067119AB2
  0000000067119A13: 66 66 66 66 66 0F  nop         word ptr [rax+rax+0000000000000000h]
                    1F 84 00 00 00 00
                    00
  0000000067119A20: 48 8B 4E 30        mov         rcx,qword ptr [rsi+30h]
  0000000067119A24: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  0000000067119A28: 48 03 C2           add         rax,rdx
  0000000067119A2B: 48 8B 6C C1 18     mov         rbp,qword ptr [rcx+rax*8+18h]
  0000000067119A30: 48 39 7D 08        cmp         qword ptr [rbp+8],rdi
  0000000067119A34: 74 16              je          0000000067119A4C
  0000000067119A36: 41 FF C0           inc         r8d
  0000000067119A39: 48 FF C2           inc         rdx
  0000000067119A3C: 48 8B C1           mov         rax,rcx
  0000000067119A3F: 8B 49 0C           mov         ecx,dword ptr [rcx+0Ch]
  0000000067119A42: 2B 48 08           sub         ecx,dword ptr [rax+8]
  0000000067119A45: 44 3B C1           cmp         r8d,ecx
  0000000067119A48: 7C D6              jl          0000000067119A20
  0000000067119A4A: EB 66              jmp         0000000067119AB2
  0000000067119A4C: 48 8B 45 00        mov         rax,qword ptr [rbp]
  0000000067119A50: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000067119A54: FF 49 24           dec         dword ptr [rcx+24h]
  0000000067119A57: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  0000000067119A5B: B9 FE FF 00 00     mov         ecx,0FFFEh
  0000000067119A60: 66 21 48 12        and         word ptr [rax+12h],cx
  0000000067119A64: 48 8B 4D 08        mov         rcx,qword ptr [rbp+8]
  0000000067119A68: 48 85 C9           test        rcx,rcx
  0000000067119A6B: 74 08              je          0000000067119A75
  0000000067119A6D: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067119A70: 41 8B D5           mov         edx,r13d
  0000000067119A73: FF 10              call        qword ptr [rax]
  0000000067119A75: 4C 89 65 08        mov         qword ptr [rbp+8],r12
  0000000067119A79: F6 C3 01           test        bl,1
  0000000067119A7C: 74 6B              je          0000000067119AE9
  0000000067119A7E: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  0000000067119A82: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  0000000067119A87: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  0000000067119A8B: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067119A8E: F6 41 04 01        test        byte ptr [rcx+4],1
  0000000067119A92: 74 0A              je          0000000067119A9E
  0000000067119A94: 48 8B CB           mov         rcx,rbx
  0000000067119A97: E8 94 C0 F0 FF     call        0000000067025B30
  0000000067119A9C: EB 4B              jmp         0000000067119AE9
  0000000067119A9E: 41 8B C5           mov         eax,r13d
  0000000067119AA1: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  0000000067119AA6: 74 41              je          0000000067119AE9
  0000000067119AA8: 48 8B CB           mov         rcx,rbx
  0000000067119AAB: E8 C0 C0 F0 FF     call        0000000067025B70
  0000000067119AB0: EB 37              jmp         0000000067119AE9
  0000000067119AB2: F6 C3 01           test        bl,1
  0000000067119AB5: 74 32              je          0000000067119AE9
  0000000067119AB7: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  0000000067119ABB: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  0000000067119AC0: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  0000000067119AC4: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067119AC7: F6 41 04 01        test        byte ptr [rcx+4],1
  0000000067119ACB: 74 0A              je          0000000067119AD7
  0000000067119ACD: 48 8B CB           mov         rcx,rbx
  0000000067119AD0: E8 5B C0 F0 FF     call        0000000067025B30
  0000000067119AD5: EB 12              jmp         0000000067119AE9
  0000000067119AD7: 41 8B C5           mov         eax,r13d
  0000000067119ADA: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  0000000067119ADF: 74 08              je          0000000067119AE9
  0000000067119AE1: 48 8B CB           mov         rcx,rbx
  0000000067119AE4: E8 87 C0 F0 FF     call        0000000067025B70
  0000000067119AE9: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  0000000067119AEE: 48 8B 6C 24 60     mov         rbp,qword ptr [rsp+60h]
  0000000067119AF3: 48 8B 74 24 68     mov         rsi,qword ptr [rsp+68h]
  0000000067119AF8: 48 83 C4 30        add         rsp,30h
  0000000067119AFC: 41 5D              pop         r13
  0000000067119AFE: 41 5C              pop         r12
  0000000067119B00: 5F                 pop         rdi
  0000000067119B01: F3 C3              rep ret

  Summary

      1A7000 .text
