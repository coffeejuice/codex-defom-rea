Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065404FB1: 48 89 7C 24 50     mov         qword ptr [rsp+50h],rdi
  0000000065404FB6: FF 15 DC 20 29 00  call        qword ptr [0000000065697098h]
  0000000065404FBC: 48 8B CB           mov         rcx,rbx
  0000000065404FBF: 48 8B F8           mov         rdi,rax
  0000000065404FC2: FF 15 C8 3D 29 00  call        qword ptr [0000000065698D90h]
  0000000065404FC8: 84 C0              test        al,al
  0000000065404FCA: 74 51              je          000000006540501D
  0000000065404FCC: 41 80 BC 24 24 02  cmp         byte ptr [r12+0000000000000224h],0
                    00 00 00
  0000000065404FD5: 75 46              jne         000000006540501D
  0000000065404FD7: 48 8B 4F 20        mov         rcx,qword ptr [rdi+20h]
  0000000065404FDB: F6 41 0C 01        test        byte ptr [rcx+0Ch],1
  0000000065404FDF: 75 3C              jne         000000006540501D
  0000000065404FE1: 48 8B D7           mov         rdx,rdi
  0000000065404FE4: 49 8B CC           mov         rcx,r12
  0000000065404FE7: E8 E4 CC FF FF     call        0000000065401CD0
  0000000065404FEC: 48 85 C0           test        rax,rax
  0000000065404FEF: 75 2C              jne         000000006540501D
  0000000065404FF1: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  0000000065404FF9: FF 15 B9 18 29 00  call        qword ptr [00000000656968B8h]
  0000000065404FFF: 45 33 C9           xor         r9d,r9d
  0000000065405002: 4C 8B C7           mov         r8,rdi
  0000000065405005: 8B D0              mov         edx,eax
  0000000065405007: 49 8B CC           mov         rcx,r12
  000000006540500A: 48 8B 7C 24 50     mov         rdi,qword ptr [rsp+50h]
  000000006540500F: 48 83 C4 20        add         rsp,20h
  0000000065405013: 41 5D              pop         r13
  0000000065405015: 41 5C              pop         r12
  0000000065405017: 5B                 pop         rbx
  0000000065405018: E9 E3 FC FF FF     jmp         0000000065404D00
  000000006540501D: 48 8B CB           mov         rcx,rbx
  0000000065405020: FF 15 62 3D 29 00  call        qword ptr [0000000065698D88h]
  0000000065405026: 84 C0              test        al,al
  0000000065405028: 74 57              je          0000000065405081
  000000006540502A: 41 80 BC 24 24 02  cmp         byte ptr [r12+0000000000000224h],0
                    00 00 00
  0000000065405033: 75 4C              jne         0000000065405081
  0000000065405035: 49 8B 45 20        mov         rax,qword ptr [r13+20h]
  0000000065405039: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006540503C: C1 E9 0F           shr         ecx,0Fh
  000000006540503F: F6 C1 01           test        cl,1
  0000000065405042: 0F 84 4B 01 00 00  je          0000000065405193
  0000000065405048: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006540504C: F6 40 0A 01        test        byte ptr [rax+0Ah],1
  0000000065405050: 74 15              je          0000000065405067
  0000000065405052: BA 45 00 00 00     mov         edx,45h
  0000000065405057: 48 8B CF           mov         rcx,rdi
  000000006540505A: E8 B1 8F C5 FF     call        000000006505E010
  000000006540505F: 84 C0              test        al,al
  0000000065405061: 0F 85 2C 01 00 00  jne         0000000065405193
  0000000065405067: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006540506A: B2 01              mov         dl,1
  000000006540506C: 48 8B CF           mov         rcx,rdi
  000000006540506F: 48 8B 7C 24 50     mov         rdi,qword ptr [rsp+50h]
  0000000065405074: 48 83 C4 20        add         rsp,20h
  0000000065405078: 41 5D              pop         r13
  000000006540507A: 41 5C              pop         r12
  000000006540507C: 5B                 pop         rbx
  000000006540507D: 48 FF 60 58        jmp         qword ptr [rax+58h]
  0000000065405081: 48 8B CB           mov         rcx,rbx
  0000000065405084: FF 15 DE 19 29 00  call        qword ptr [0000000065696A68h]
  000000006540508A: 83 F8 47           cmp         eax,47h
  000000006540508D: 0F 85 00 01 00 00  jne         0000000065405193
  0000000065405093: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  000000006540509B: 48 89 74 24 48     mov         qword ptr [rsp+48h],rsi
  00000000654050A0: 33 DB              xor         ebx,ebx
  00000000654050A2: FF 15 10 18 29 00  call        qword ptr [00000000656968B8h]
  00000000654050A8: 85 C0              test        eax,eax
  00000000654050AA: 0F 8E DE 00 00 00  jle         000000006540518E
  00000000654050B0: 48 89 6C 24 40     mov         qword ptr [rsp+40h],rbp
  00000000654050B5: 66 66 66 0F 1F 84  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00 00
  00000000654050C0: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  00000000654050C8: 8B D3              mov         edx,ebx
  00000000654050CA: FF 15 70 18 29 00  call        qword ptr [0000000065696940h]
  00000000654050D0: 48 8B 28           mov         rbp,qword ptr [rax]
  00000000654050D3: 48 39 7D 18        cmp         qword ptr [rbp+18h],rdi
  00000000654050D7: 74 19              je          00000000654050F2
  00000000654050D9: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  00000000654050E1: FF C3              inc         ebx
  00000000654050E3: FF 15 CF 17 29 00  call        qword ptr [00000000656968B8h]
  00000000654050E9: 3B D8              cmp         ebx,eax
  00000000654050EB: 7C D3              jl          00000000654050C0
  00000000654050ED: E9 97 00 00 00     jmp         0000000065405189
  00000000654050F2: 85 DB              test        ebx,ebx
  00000000654050F4: 78 61              js          0000000065405157
  00000000654050F6: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  00000000654050FE: FF 15 B4 17 29 00  call        qword ptr [00000000656968B8h]
  0000000065405104: 3B D8              cmp         ebx,eax
  0000000065405106: 7D 4F              jge         0000000065405157
  0000000065405108: 49 8B 8C 24 10 02  mov         rcx,qword ptr [r12+0000000000000210h]
                    00 00
  0000000065405110: BA 01 00 00 00     mov         edx,1
  0000000065405115: FF 15 55 19 29 00  call        qword ptr [0000000065696A70h]
  000000006540511B: 84 C0              test        al,al
  000000006540511D: 74 18              je          0000000065405137
  000000006540511F: 49 8B 84 24 10 02  mov         rax,qword ptr [r12+0000000000000210h]
                    00 00
  0000000065405127: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  000000006540512F: 8B 50 04           mov         edx,dword ptr [rax+4]
  0000000065405132: E8 E9 ED D9 FF     call        00000000651A3F20
  0000000065405137: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  000000006540513F: 8B D3              mov         edx,ebx
  0000000065405141: FF 15 F9 17 29 00  call        qword ptr [0000000065696940h]
  0000000065405147: 49 8D 8C 24 10 02  lea         rcx,[r12+0000000000000210h]
                    00 00
  000000006540514F: 8B D3              mov         edx,ebx
  0000000065405151: FF 15 29 19 29 00  call        qword ptr [0000000065696A80h]
  0000000065405157: 48 8B 4D 20        mov         rcx,qword ptr [rbp+20h]
  000000006540515B: 48 85 C9           test        rcx,rcx
  000000006540515E: 74 0B              je          000000006540516B
  0000000065405160: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065405163: BA 01 00 00 00     mov         edx,1
  0000000065405168: FF 50 18           call        qword ptr [rax+18h]
  000000006540516B: 48 8B CD           mov         rcx,rbp
  000000006540516E: E8 3B BF 1D 00     call        00000000655E10AE
  0000000065405173: 4D 8B 5D 20        mov         r11,qword ptr [r13+20h]
  0000000065405177: 49 8B CC           mov         rcx,r12
  000000006540517A: 41 8B 53 08        mov         edx,dword ptr [r11+8]
  000000006540517E: C1 EA 0F           shr         edx,0Fh
  0000000065405181: 80 E2 01           and         dl,1
  0000000065405184: E8 97 F7 FF FF     call        0000000065404920
  0000000065405189: 48 8B 6C 24 40     mov         rbp,qword ptr [rsp+40h]
  000000006540518E: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  0000000065405193: 48 8B 7C 24 50     mov         rdi,qword ptr [rsp+50h]
  0000000065405198: 48 83 C4 20        add         rsp,20h
  000000006540519C: 41 5D              pop         r13
  000000006540519E: 41 5C              pop         r12
  00000000654051A0: 5B                 pop         rbx
  00000000654051A1: C3                 ret

  Summary

      695000 .text
