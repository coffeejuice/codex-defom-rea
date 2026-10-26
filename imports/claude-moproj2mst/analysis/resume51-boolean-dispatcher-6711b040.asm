Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711B040: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000006711B045: 53                 push        rbx
  000000006711B046: 56                 push        rsi
  000000006711B047: 57                 push        rdi
  000000006711B048: 41 54              push        r12
  000000006711B04A: 41 55              push        r13
  000000006711B04C: 41 56              push        r14
  000000006711B04E: 41 57              push        r15
  000000006711B050: 48 83 EC 50        sub         rsp,50h
  000000006711B054: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711B05D: 49 8B F0           mov         rsi,r8
  000000006711B060: 44 8B F2           mov         r14d,edx
  000000006711B063: 4C 8B F9           mov         r15,rcx
  000000006711B066: 45 33 D2           xor         r10d,r10d
  000000006711B069: 83 FA FF           cmp         edx,0FFFFFFFFh
  000000006711B06C: 45 0F 44 F2        cmove       r14d,r10d
  000000006711B070: 48 85 C9           test        rcx,rcx
  000000006711B073: 74 1B              je          000000006711B090
  000000006711B075: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006711B079: 4C 39 40 40        cmp         qword ptr [rax+40h],r8
  000000006711B07D: 74 11              je          000000006711B090
  000000006711B07F: 48 8D 0D 5A EF 11  lea         rcx,[0000000067239FE0h]
                    00
  000000006711B086: E8 55 1E F0 FF     call        000000006701CEE0
  000000006711B08B: E9 5E 04 00 00     jmp         000000006711B4EE
  000000006711B090: 41 FF 40 38        inc         dword ptr [r8+38h]
  000000006711B094: 41 83 FE 46        cmp         r14d,46h
  000000006711B098: 75 1C              jne         000000006711B0B6
  000000006711B09A: 48 85 C9           test        rcx,rcx
  000000006711B09D: 74 11              je          000000006711B0B0
  000000006711B09F: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006711B0A3: E8 A8 16 01 00     call        000000006712C750
  000000006711B0A8: FF 4E 38           dec         dword ptr [rsi+38h]
  000000006711B0AB: E9 3E 04 00 00     jmp         000000006711B4EE
  000000006711B0B0: 41 BE 43 00 00 00  mov         r14d,43h
  000000006711B0B6: 49 8D 58 48        lea         rbx,[r8+48h]
  000000006711B0BA: 48 85 DB           test        rbx,rbx
  000000006711B0BD: 74 49              je          000000006711B108
  000000006711B0BF: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006711B0C2: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006711B0C6: 74 13              je          000000006711B0DB
  000000006711B0C8: 48 8B CB           mov         rcx,rbx
  000000006711B0CB: E8 E0 AC F0 FF     call        0000000067025DB0
  000000006711B0D0: 41 BC 01 00 00 00  mov         r12d,1
  000000006711B0D6: 45 33 D2           xor         r10d,r10d
  000000006711B0D9: EB 1F              jmp         000000006711B0FA
  000000006711B0DB: 33 C0              xor         eax,eax
  000000006711B0DD: 41 BC 01 00 00 00  mov         r12d,1
  000000006711B0E3: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  000000006711B0E8: 0F 94 C1           sete        cl
  000000006711B0EB: 84 C9              test        cl,cl
  000000006711B0ED: 75 0B              jne         000000006711B0FA
  000000006711B0EF: 48 8B CB           mov         rcx,rbx
  000000006711B0F2: E8 B9 AA F0 FF     call        0000000067025BB0
  000000006711B0F7: 45 33 D2           xor         r10d,r10d
  000000006711B0FA: 48 83 CB 01        or          rbx,1
  000000006711B0FE: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  000000006711B106: EB 11              jmp         000000006711B119
  000000006711B108: 49 8B DA           mov         rbx,r10
  000000006711B10B: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  000000006711B113: 41 BC 01 00 00 00  mov         r12d,1
  000000006711B119: 48 8D 7E 30        lea         rdi,[rsi+30h]
  000000006711B11D: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006711B120: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  000000006711B123: 3B 41 08           cmp         eax,dword ptr [rcx+8]
  000000006711B126: 0F 94 C1           sete        cl
  000000006711B129: 88 4E 50           mov         byte ptr [rsi+50h],cl
  000000006711B12C: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006711B12F: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006711B132: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000006711B135: 3B C1              cmp         eax,ecx
  000000006711B137: 0F 84 74 03 00 00  je          000000006711B4B1
  000000006711B13D: 4D 85 FF           test        r15,r15
  000000006711B140: 74 0E              je          000000006711B150
  000000006711B142: 49 8B 47 08        mov         rax,qword ptr [r15+8]
  000000006711B146: 83 78 24 00        cmp         dword ptr [rax+24h],0
  000000006711B14A: 0F 84 61 03 00 00  je          000000006711B4B1
  000000006711B150: C6 46 50 01        mov         byte ptr [rsi+50h],1
  000000006711B154: 4C 8D 4E 3C        lea         r9,[rsi+3Ch]
  000000006711B158: 4C 89 8C 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],r9
                    00 00
  000000006711B160: 41 8B 01           mov         eax,dword ptr [r9]
  000000006711B163: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  000000006711B16A: 45 85 F6           test        r14d,r14d
  000000006711B16D: 75 05              jne         000000006711B174
  000000006711B16F: 4D 85 FF           test        r15,r15
  000000006711B172: 74 10              je          000000006711B184
  000000006711B174: 4C 8D 8C 24 98 00  lea         r9,[rsp+0000000000000098h]
                    00 00
  000000006711B17C: 4C 89 8C 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],r9
                    00 00
  000000006711B184: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006711B187: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  000000006711B18A: 2B 41 08           sub         eax,dword ptr [rcx+8]
  000000006711B18D: 89 46 40           mov         dword ptr [rsi+40h],eax
  000000006711B190: 41 BD FE FF 00 00  mov         r13d,0FFFEh
  000000006711B196: 45 8D 5D FF        lea         r11d,[r13-1]
  000000006711B19A: EB 1A              jmp         000000006711B1B6
  000000006711B19C: 0F 1F 40 00        nop         dword ptr [rax]
  000000006711B1A0: 41 BC 01 00 00 00  mov         r12d,1
  000000006711B1A6: EB 0E              jmp         000000006711B1B6
  000000006711B1A8: 41 BC 01 00 00 00  mov         r12d,1
  000000006711B1AE: EB 06              jmp         000000006711B1B6
  000000006711B1B0: 41 BC 01 00 00 00  mov         r12d,1
  000000006711B1B6: 48 8B 0F           mov         rcx,qword ptr [rdi]
  000000006711B1B9: 4D 63 01           movsxd      r8,dword ptr [r9]
  000000006711B1BC: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  000000006711B1BF: 2B 41 08           sub         eax,dword ptr [rcx+8]
  000000006711B1C2: 44 3B C0           cmp         r8d,eax
  000000006711B1C5: 0F 8D 0F 02 00 00  jge         000000006711B3DA
  000000006711B1CB: 44 3B 46 40        cmp         r8d,dword ptr [rsi+40h]
  000000006711B1CF: 0F 8D 05 02 00 00  jge         000000006711B3DA
  000000006711B1D5: 48 8B D1           mov         rdx,rcx
  000000006711B1D8: 48 63 49 08        movsxd      rcx,dword ptr [rcx+8]
  000000006711B1DC: 49 03 C8           add         rcx,r8
  000000006711B1DF: 4C 8B 64 CA 18     mov         r12,qword ptr [rdx+rcx*8+18h]
  000000006711B1E4: 41 8D 40 01        lea         eax,[r8+1]
  000000006711B1E8: 41 89 01           mov         dword ptr [r9],eax
  000000006711B1EB: 49 8B 54 24 08     mov         rdx,qword ptr [r12+8]
  000000006711B1F0: 48 85 D2           test        rdx,rdx
  000000006711B1F3: 74 BB              je          000000006711B1B0
  000000006711B1F5: 4D 85 FF           test        r15,r15
  000000006711B1F8: 74 06              je          000000006711B200
  000000006711B1FA: 4D 3B 3C 24        cmp         r15,qword ptr [r12]
  000000006711B1FE: 75 0E              jne         000000006711B20E
  000000006711B200: 45 85 F6           test        r14d,r14d
  000000006711B203: 74 15              je          000000006711B21A
  000000006711B205: 0F B7 42 10        movzx       eax,word ptr [rdx+10h]
  000000006711B209: 44 3B F0           cmp         r14d,eax
  000000006711B20C: 74 0C              je          000000006711B21A
  000000006711B20E: C6 46 50 00        mov         byte ptr [rsi+50h],0
  000000006711B212: 41 BC 01 00 00 00  mov         r12d,1
  000000006711B218: EB 9C              jmp         000000006711B1B6
  000000006711B21A: 66 83 7A 10 34     cmp         word ptr [rdx+10h],34h
  000000006711B21F: 75 65              jne         000000006711B286
  000000006711B221: 8B 46 1C           mov         eax,dword ptr [rsi+1Ch]
  000000006711B224: 48 8B 4A 08        mov         rcx,qword ptr [rdx+8]
  000000006711B228: 48 3B C8           cmp         rcx,rax
  000000006711B22B: 77 59              ja          000000006711B286
  000000006711B22D: 48 85 C9           test        rcx,rcx
  000000006711B230: 75 04              jne         000000006711B236
  000000006711B232: 85 C0              test        eax,eax
  000000006711B234: 7F 50              jg          000000006711B286
  000000006711B236: 41 83 FE 34        cmp         r14d,34h
  000000006711B23A: 75 0E              jne         000000006711B24A
  000000006711B23C: 48 3B C8           cmp         rcx,rax
  000000006711B23F: 74 45              je          000000006711B286
  000000006711B241: 45 8D 66 CD        lea         r12d,[r14-33h]
  000000006711B245: E9 6C FF FF FF     jmp         000000006711B1B6
  000000006711B24A: 45 85 F6           test        r14d,r14d
  000000006711B24D: 0F 85 55 FF FF FF  jne         000000006711B1A8
  000000006711B253: 4D 85 FF           test        r15,r15
  000000006711B256: 0F 85 44 FF FF FF  jne         000000006711B1A0
  000000006711B25C: 49 8B D4           mov         rdx,r12
  000000006711B25F: 48 8B CF           mov         rcx,rdi
  000000006711B262: E8 69 FA FF FF     call        000000006711ACD0
  000000006711B267: 45 33 D2           xor         r10d,r10d
  000000006711B26A: 4D 89 54 24 08     mov         qword ptr [r12+8],r10
  000000006711B26F: 45 8D 66 01        lea         r12d,[r14+1]
  000000006711B273: 4C 8B 8C 24 A8 00  mov         r9,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006711B27B: 41 BB FD FF 00 00  mov         r11d,0FFFDh
  000000006711B281: E9 30 FF FF FF     jmp         000000006711B1B6
  000000006711B286: 66 44 21 6A 12     and         word ptr [rdx+12h],r13w
  000000006711B28B: 49 8B 7C 24 08     mov         rdi,qword ptr [r12+8]
  000000006711B290: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  000000006711B295: 4D 8B 2C 24        mov         r13,qword ptr [r12]
  000000006711B299: 49 8B 45 08        mov         rax,qword ptr [r13+8]
  000000006711B29D: FF 48 24           dec         dword ptr [rax+24h]
  000000006711B2A0: 4D 89 54 24 08     mov         qword ptr [r12+8],r10
  000000006711B2A5: F6 C3 01           test        bl,1
  000000006711B2A8: 74 5E              je          000000006711B308
  000000006711B2AA: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711B2AE: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  000000006711B2B6: 48 8B CB           mov         rcx,rbx
  000000006711B2B9: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006711B2BD: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006711B2C0: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006711B2C4: 74 13              je          000000006711B2D9
  000000006711B2C6: E8 65 A8 F0 FF     call        0000000067025B30
  000000006711B2CB: 41 BC 01 00 00 00  mov         r12d,1
  000000006711B2D1: 41 BB FD FF 00 00  mov         r11d,0FFFDh
  000000006711B2D7: EB 35              jmp         000000006711B30E
  000000006711B2D9: 41 BC 01 00 00 00  mov         r12d,1
  000000006711B2DF: 41 8B C4           mov         eax,r12d
  000000006711B2E2: F0 44 0F B1 12     lock cmpxchg dword ptr [rdx],r10d
  000000006711B2E7: 74 15              je          000000006711B2FE
  000000006711B2E9: E8 82 A8 F0 FF     call        0000000067025B70
  000000006711B2EE: 48 8B 9C 24 90 00  mov         rbx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000006711B2F6: 41 BB FD FF 00 00  mov         r11d,0FFFDh
  000000006711B2FC: EB 10              jmp         000000006711B30E
  000000006711B2FE: 48 8B 9C 24 90 00  mov         rbx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000006711B306: EB 06              jmp         000000006711B30E
  000000006711B308: 41 BC 01 00 00 00  mov         r12d,1
  000000006711B30E: 48 85 FF           test        rdi,rdi
  000000006711B311: 74 05              je          000000006711B318
  000000006711B313: 66 44 21 5F 12     and         word ptr [rdi+12h],r11w
  000000006711B318: 48 8B 0D 01 8F 1D  mov         rcx,qword ptr [00000000672F4220h]
                    00
  000000006711B31F: 48 85 C9           test        rcx,rcx
  000000006711B322: 74 0C              je          000000006711B330
  000000006711B324: 4C 8B C7           mov         r8,rdi
  000000006711B327: 49 8B D5           mov         rdx,r13
  000000006711B32A: E8 41 E2 FF FF     call        0000000067119570
  000000006711B32F: 90                 nop
  000000006711B330: 48 85 FF           test        rdi,rdi
  000000006711B333: 74 0B              je          000000006711B340
  000000006711B335: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006711B338: 41 8B D4           mov         edx,r12d
  000000006711B33B: 48 8B CF           mov         rcx,rdi
  000000006711B33E: FF 10              call        qword ptr [rax]
  000000006711B340: 48 85 DB           test        rbx,rbx
  000000006711B343: 48 8D 7E 30        lea         rdi,[rsi+30h]
  000000006711B347: 4C 8B 8C 24 A8 00  mov         r9,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006711B34F: 41 BA 00 00 00 00  mov         r10d,0
  000000006711B355: 41 BB FD FF 00 00  mov         r11d,0FFFDh
  000000006711B35B: 45 8D 6B 01        lea         r13d,[r11+1]
  000000006711B35F: 0F 84 51 FE FF FF  je          000000006711B1B6
  000000006711B365: F6 C3 01           test        bl,1
  000000006711B368: 48 8D 7E 30        lea         rdi,[rsi+30h]
  000000006711B36C: 0F 85 44 FE FF FF  jne         000000006711B1B6
  000000006711B372: 48 8B D3           mov         rdx,rbx
  000000006711B375: 48 83 E2 FE        and         rdx,0FFFFFFFFFFFFFFFEh
  000000006711B379: 48 8B 0A           mov         rcx,qword ptr [rdx]
  000000006711B37C: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006711B380: 74 0A              je          000000006711B38C
  000000006711B382: 48 8B CA           mov         rcx,rdx
  000000006711B385: E8 26 AA F0 FF     call        0000000067025DB0
  000000006711B38A: EB 28              jmp         000000006711B3B4
  000000006711B38C: 33 C0              xor         eax,eax
  000000006711B38E: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  000000006711B393: 0F 94 C1           sete        cl
  000000006711B396: 84 C9              test        cl,cl
  000000006711B398: 75 12              jne         000000006711B3AC
  000000006711B39A: 48 8B CA           mov         rcx,rdx
  000000006711B39D: E8 0E A8 F0 FF     call        0000000067025BB0
  000000006711B3A2: 48 8B 9C 24 90 00  mov         rbx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000006711B3AA: EB 08              jmp         000000006711B3B4
  000000006711B3AC: 48 8B 9C 24 90 00  mov         rbx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000006711B3B4: 48 83 CB 01        or          rbx,1
  000000006711B3B8: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  000000006711B3C0: 48 8D 7E 30        lea         rdi,[rsi+30h]
  000000006711B3C4: 4C 8B 8C 24 A8 00  mov         r9,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006711B3CC: 45 33 D2           xor         r10d,r10d
  000000006711B3CF: 41 BB FD FF 00 00  mov         r11d,0FFFDh
  000000006711B3D5: E9 DC FD FF FF     jmp         000000006711B1B6
  000000006711B3DA: 83 46 38 FF        add         dword ptr [rsi+38h],0FFFFFFFFh
  000000006711B3DE: 75 18              jne         000000006711B3F8
  000000006711B3E0: 80 7E 50 00        cmp         byte ptr [rsi+50h],0
  000000006711B3E4: 75 12              jne         000000006711B3F8
  000000006711B3E6: 48 8B 4E 20        mov         rcx,qword ptr [rsi+20h]
  000000006711B3EA: 48 85 C9           test        rcx,rcx
  000000006711B3ED: 74 09              je          000000006711B3F8
  000000006711B3EF: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006711B3F2: FF 90 98 00 00 00  call        qword ptr [rax+0000000000000098h]
  000000006711B3F8: 45 85 F6           test        r14d,r14d
  000000006711B3FB: 75 77              jne         000000006711B474
  000000006711B3FD: 4D 85 FF           test        r15,r15
  000000006711B400: 75 6E              jne         000000006711B470
  000000006711B402: 44 39 7E 3C        cmp         dword ptr [rsi+3Ch],r15d
  000000006711B406: 7C 64              jl          000000006711B46C
  000000006711B408: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006711B40B: 8B 08              mov         ecx,dword ptr [rax]
  000000006711B40D: 83 F9 01           cmp         ecx,1
  000000006711B410: 74 0E              je          000000006711B420
  000000006711B412: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006711B415: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006711B418: 48 8B CF           mov         rcx,rdi
  000000006711B41B: E8 A0 DA FF FF     call        0000000067118EC0
  000000006711B420: 48 8B 17           mov         rdx,qword ptr [rdi]
  000000006711B423: 48 63 42 08        movsxd      rax,dword ptr [rdx+8]
  000000006711B427: 48 8D 44 C2 18     lea         rax,[rdx+rax*8+18h]
  000000006711B42C: 48 63 56 3C        movsxd      rdx,dword ptr [rsi+3Ch]
  000000006711B430: 4C 8D 04 D0        lea         r8,[rax+rdx*8]
  000000006711B434: 4C 89 44 24 28     mov         qword ptr [rsp+28h],r8
  000000006711B439: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006711B43E: 4C 8D 4C 24 28     lea         r9,[rsp+28h]
  000000006711B443: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  000000006711B448: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006711B44D: 48 8B CF           mov         rcx,rdi
  000000006711B450: E8 3B F7 FF FF     call        000000006711AB90
  000000006711B455: 44 8B 5E 3C        mov         r11d,dword ptr [rsi+3Ch]
  000000006711B459: 44 29 5E 40        sub         dword ptr [rsi+40h],r11d
  000000006711B45D: 33 D2              xor         edx,edx
  000000006711B45F: 89 56 3C           mov         dword ptr [rsi+3Ch],edx
  000000006711B462: 48 8B 9C 24 90 00  mov         rbx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000006711B46A: EB 0A              jmp         000000006711B476
  000000006711B46C: 33 D2              xor         edx,edx
  000000006711B46E: EB 06              jmp         000000006711B476
  000000006711B470: 33 D2              xor         edx,edx
  000000006711B472: EB 02              jmp         000000006711B476
  000000006711B474: 33 D2              xor         edx,edx
  000000006711B476: F6 C3 01           test        bl,1
  000000006711B479: 74 73              je          000000006711B4EE
  000000006711B47B: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711B47F: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  000000006711B487: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711B48B: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006711B48E: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006711B492: 74 0A              je          000000006711B49E
  000000006711B494: 48 8B CB           mov         rcx,rbx
  000000006711B497: E8 94 A6 F0 FF     call        0000000067025B30
  000000006711B49C: EB 50              jmp         000000006711B4EE
  000000006711B49E: 41 8B C4           mov         eax,r12d
  000000006711B4A1: F0 0F B1 11        lock cmpxchg dword ptr [rcx],edx
  000000006711B4A5: 74 47              je          000000006711B4EE
  000000006711B4A7: 48 8B CB           mov         rcx,rbx
  000000006711B4AA: E8 C1 A6 F0 FF     call        0000000067025B70
  000000006711B4AF: EB 3D              jmp         000000006711B4EE
  000000006711B4B1: FF 4E 38           dec         dword ptr [rsi+38h]
  000000006711B4B4: F6 C3 01           test        bl,1
  000000006711B4B7: 74 35              je          000000006711B4EE
  000000006711B4B9: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711B4BD: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  000000006711B4C5: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711B4C9: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006711B4CC: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006711B4D0: 74 0A              je          000000006711B4DC
  000000006711B4D2: 48 8B CB           mov         rcx,rbx
  000000006711B4D5: E8 56 A6 F0 FF     call        0000000067025B30
  000000006711B4DA: EB 12              jmp         000000006711B4EE
  000000006711B4DC: 41 8B C4           mov         eax,r12d
  000000006711B4DF: F0 44 0F B1 11     lock cmpxchg dword ptr [rcx],r10d
  000000006711B4E4: 74 08              je          000000006711B4EE
  000000006711B4E6: 48 8B CB           mov         rcx,rbx
  000000006711B4E9: E8 82 A6 F0 FF     call        0000000067025B70
  000000006711B4EE: 48 83 C4 50        add         rsp,50h
  000000006711B4F2: 41 5F              pop         r15
  000000006711B4F4: 41 5E              pop         r14
  000000006711B4F6: 41 5D              pop         r13
  000000006711B4F8: 41 5C              pop         r12
  000000006711B4FA: 5F                 pop         rdi
  000000006711B4FB: 5E                 pop         rsi
  000000006711B4FC: 5B                 pop         rbx
  000000006711B4FD: C3                 ret

  Summary

      1A7000 .text
