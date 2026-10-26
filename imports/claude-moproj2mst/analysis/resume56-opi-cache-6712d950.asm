Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712D950: 40 55              push        rbp
  000000006712D952: 56                 push        rsi
  000000006712D953: 57                 push        rdi
  000000006712D954: 41 54              push        r12
  000000006712D956: 41 55              push        r13
  000000006712D958: 48 83 EC 30        sub         rsp,30h
  000000006712D95C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006712D965: 48 89 5C 24 78     mov         qword ptr [rsp+78h],rbx
  000000006712D96A: 48 8B F9           mov         rdi,rcx
  000000006712D96D: 48 83 39 00        cmp         qword ptr [rcx],0
  000000006712D971: 0F 84 95 01 00 00  je          000000006712DB0C
  000000006712D977: E8 44 FD FF FF     call        000000006712D6C0
  000000006712D97C: 48 8B F0           mov         rsi,rax
  000000006712D97F: 48 85 C0           test        rax,rax
  000000006712D982: 0F 84 84 01 00 00  je          000000006712DB0C
  000000006712D988: 48 8B 10           mov         rdx,qword ptr [rax]
  000000006712D98B: 83 7A 14 00        cmp         dword ptr [rdx+14h],0
  000000006712D98F: 0F 84 77 01 00 00  je          000000006712DB0C
  000000006712D995: E8 76 D3 FF FF     call        000000006712AD10
  000000006712D99A: 48 8B D8           mov         rbx,rax
  000000006712D99D: 45 33 E4           xor         r12d,r12d
  000000006712D9A0: 45 8D 6C 24 01     lea         r13d,[r12+1]
  000000006712D9A5: 48 85 C0           test        rax,rax
  000000006712D9A8: 74 33              je          000000006712D9DD
  000000006712D9AA: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006712D9AD: 44 84 69 04        test        byte ptr [rcx+4],r13b
  000000006712D9B1: 74 0A              je          000000006712D9BD
  000000006712D9B3: 48 8B C8           mov         rcx,rax
  000000006712D9B6: E8 F5 83 EF FF     call        0000000067025DB0
  000000006712D9BB: EB 16              jmp         000000006712D9D3
  000000006712D9BD: 33 C0              xor         eax,eax
  000000006712D9BF: F0 44 0F B1 29     lock cmpxchg dword ptr [rcx],r13d
  000000006712D9C4: 0F 94 C1           sete        cl
  000000006712D9C7: 84 C9              test        cl,cl
  000000006712D9C9: 75 08              jne         000000006712D9D3
  000000006712D9CB: 48 8B CB           mov         rcx,rbx
  000000006712D9CE: E8 DD 81 EF FF     call        0000000067025BB0
  000000006712D9D3: 49 0B DD           or          rbx,r13
  000000006712D9D6: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  000000006712D9DB: EB 08              jmp         000000006712D9E5
  000000006712D9DD: 49 8B DC           mov         rbx,r12
  000000006712D9E0: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  000000006712D9E5: 48 83 3F 00        cmp         qword ptr [rdi],0
  000000006712D9E9: 75 47              jne         000000006712DA32
  000000006712D9EB: F6 C3 01           test        bl,1
  000000006712D9EE: 0F 84 18 01 00 00  je          000000006712DB0C
  000000006712D9F4: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006712D9F8: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  000000006712D9FD: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006712DA01: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006712DA04: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006712DA08: 74 0D              je          000000006712DA17
  000000006712DA0A: 48 8B CB           mov         rcx,rbx
  000000006712DA0D: E8 1E 81 EF FF     call        0000000067025B30
  000000006712DA12: E9 F5 00 00 00     jmp         000000006712DB0C
  000000006712DA17: 41 8B C5           mov         eax,r13d
  000000006712DA1A: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  000000006712DA1F: 0F 84 E7 00 00 00  je          000000006712DB0C
  000000006712DA25: 48 8B CB           mov         rcx,rbx
  000000006712DA28: E8 43 81 EF FF     call        0000000067025B70
  000000006712DA2D: E9 DA 00 00 00     jmp         000000006712DB0C
  000000006712DA32: 4C 8B C7           mov         r8,rdi
  000000006712DA35: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000006712DA3A: 48 8B CE           mov         rcx,rsi
  000000006712DA3D: E8 0E E5 FF FF     call        000000006712BF50
  000000006712DA42: 4C 8B 1E           mov         r11,qword ptr [rsi]
  000000006712DA45: 41 8B 43 10        mov         eax,dword ptr [r11+10h]
  000000006712DA49: 83 F8 01           cmp         eax,1
  000000006712DA4C: 74 08              je          000000006712DA56
  000000006712DA4E: 48 8B CE           mov         rcx,rsi
  000000006712DA51: E8 6A CD FF FF     call        000000006712A7C0
  000000006712DA56: 48 8B 1E           mov         rbx,qword ptr [rsi]
  000000006712DA59: 40 32 ED           xor         bpl,bpl
  000000006712DA5C: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006712DA5F: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  000000006712DA64: 48 39 48 10        cmp         qword ptr [rax+10h],rcx
  000000006712DA68: 75 5E              jne         000000006712DAC8
  000000006712DA6A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006712DA70: 48 3B C3           cmp         rax,rbx
  000000006712DA73: 74 4E              je          000000006712DAC3
  000000006712DA75: 48 39 78 18        cmp         qword ptr [rax+18h],rdi
  000000006712DA79: 74 1B              je          000000006712DA96
  000000006712DA7B: 40 B5 01           mov         bpl,1
  000000006712DA7E: 48 8B C8           mov         rcx,rax
  000000006712DA81: E8 2A A2 F1 FF     call        0000000067047CB0
  000000006712DA86: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000006712DA8B: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006712DA8E: 48 39 48 10        cmp         qword ptr [rax+10h],rcx
  000000006712DA92: 74 DC              je          000000006712DA70
  000000006712DA94: EB 40              jmp         000000006712DAD6
  000000006712DA96: 4C 8B C0           mov         r8,rax
  000000006712DA99: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  000000006712DA9E: 48 8B CE           mov         rcx,rsi
  000000006712DAA1: E8 7A CC FF FF     call        000000006712A720
  000000006712DAA6: 48 8B 00           mov         rax,qword ptr [rax]
  000000006712DAA9: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000006712DAAE: 40 84 ED           test        bpl,bpl
  000000006712DAB1: 75 23              jne         000000006712DAD6
  000000006712DAB3: 48 3B C3           cmp         rax,rbx
  000000006712DAB6: 74 10              je          000000006712DAC8
  000000006712DAB8: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006712DABB: 48 39 48 10        cmp         qword ptr [rax+10h],rcx
  000000006712DABF: 74 15              je          000000006712DAD6
  000000006712DAC1: EB 05              jmp         000000006712DAC8
  000000006712DAC3: 40 84 ED           test        bpl,bpl
  000000006712DAC6: 75 0E              jne         000000006712DAD6
  000000006712DAC8: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006712DACB: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006712DACF: 81 61 20 FF FD FF  and         dword ptr [rcx+20h],0FFFFFDFFh
                    FF
  000000006712DAD6: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006712DADB: F6 C1 01           test        cl,1
  000000006712DADE: 74 2C              je          000000006712DB0C
  000000006712DAE0: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712DAE4: 48 89 4C 24 60     mov         qword ptr [rsp+60h],rcx
  000000006712DAE9: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006712DAED: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006712DAF0: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006712DAF4: 74 07              je          000000006712DAFD
  000000006712DAF6: E8 35 80 EF FF     call        0000000067025B30
  000000006712DAFB: EB 0F              jmp         000000006712DB0C
  000000006712DAFD: 41 8B C5           mov         eax,r13d
  000000006712DB00: F0 44 0F B1 22     lock cmpxchg dword ptr [rdx],r12d
  000000006712DB05: 74 05              je          000000006712DB0C
  000000006712DB07: E8 64 80 EF FF     call        0000000067025B70
  000000006712DB0C: 48 8B 5C 24 78     mov         rbx,qword ptr [rsp+78h]
  000000006712DB11: 48 83 C4 30        add         rsp,30h
  000000006712DB15: 41 5D              pop         r13
  000000006712DB17: 41 5C              pop         r12
  000000006712DB19: 5F                 pop         rdi
  000000006712DB1A: 5E                 pop         rsi
  000000006712DB1B: 5D                 pop         rbp
  000000006712DB1C: C3                 ret

  Summary

      1A7000 .text
