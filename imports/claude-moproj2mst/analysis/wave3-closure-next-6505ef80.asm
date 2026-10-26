Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505EF80: 40 55              push        rbp
  000000006505EF82: 56                 push        rsi
  000000006505EF83: 57                 push        rdi
  000000006505EF84: 41 54              push        r12
  000000006505EF86: 41 55              push        r13
  000000006505EF88: 48 83 EC 30        sub         rsp,30h
  000000006505EF8C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006505EF95: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  000000006505EF9A: 48 8B F1           mov         rsi,rcx
  000000006505EF9D: 45 33 ED           xor         r13d,r13d
  000000006505EFA0: 44 89 6C 24 60     mov         dword ptr [rsp+60h],r13d
  000000006505EFA5: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  000000006505EFA9: 48 8B 91 88 00 00  mov         rdx,qword ptr [rcx+0000000000000088h]
                    00
  000000006505EFB0: 48 85 D2           test        rdx,rdx
  000000006505EFB3: 74 16              je          000000006505EFCB
  000000006505EFB5: 48 8B 52 20        mov         rdx,qword ptr [rdx+20h]
  000000006505EFB9: 48 8D 0D 80 A2 68  lea         rcx,[00000000656E9240h]
                    00
  000000006505EFC0: FF 15 42 7A 63 00  call        qword ptr [0000000065696A08h]
  000000006505EFC6: 48 8B F8           mov         rdi,rax
  000000006505EFC9: EB 03              jmp         000000006505EFCE
  000000006505EFCB: 49 8B FD           mov         rdi,r13
  000000006505EFCE: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  000000006505EFD3: 49 8B CC           mov         rcx,r12
  000000006505EFD6: E8 35 E6 FF FF     call        000000006505D610
  000000006505EFDB: 90                 nop
  000000006505EFDC: 48 8B C8           mov         rcx,rax
  000000006505EFDF: FF 15 03 7A 63 00  call        qword ptr [00000000656969E8h]
  000000006505EFE5: 84 C0              test        al,al
  000000006505EFE7: 0F 94 C3           sete        bl
  000000006505EFEA: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006505EFEF: FF 15 6B 77 63 00  call        qword ptr [0000000065696760h]
  000000006505EFF5: 84 DB              test        bl,bl
  000000006505EFF7: 74 10              je          000000006505F009
  000000006505EFF9: 49 8B D4           mov         rdx,r12
  000000006505EFFC: 48 8B CF           mov         rcx,rdi
  000000006505EFFF: E8 5C 46 28 00     call        00000000652E3660
  000000006505F004: E9 45 01 00 00     jmp         000000006505F14E
  000000006505F009: 48 85 FF           test        rdi,rdi
  000000006505F00C: 74 06              je          000000006505F014
  000000006505F00E: 48 8B 7F 18        mov         rdi,qword ptr [rdi+18h]
  000000006505F012: EB 15              jmp         000000006505F029
  000000006505F014: 48 8B BE 88 00 00  mov         rdi,qword ptr [rsi+0000000000000088h]
                    00
  000000006505F01B: 48 85 FF           test        rdi,rdi
  000000006505F01E: 74 06              je          000000006505F026
  000000006505F020: 48 8B 7F 20        mov         rdi,qword ptr [rdi+20h]
  000000006505F024: EB 03              jmp         000000006505F029
  000000006505F026: 49 8B FD           mov         rdi,r13
  000000006505F029: 49 8B CC           mov         rcx,r12
  000000006505F02C: FF 15 36 78 63 00  call        qword ptr [0000000065696868h]
  000000006505F032: 48 85 C0           test        rax,rax
  000000006505F035: 74 1B              je          000000006505F052
  000000006505F037: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006505F03B: 48 83 B8 88 00 00  cmp         qword ptr [rax+0000000000000088h],0
                    00 00
  000000006505F043: 74 0D              je          000000006505F052
  000000006505F045: 48 8B 80 88 00 00  mov         rax,qword ptr [rax+0000000000000088h]
                    00
  000000006505F04C: 48 8B 58 20        mov         rbx,qword ptr [rax+20h]
  000000006505F050: EB 03              jmp         000000006505F055
  000000006505F052: 49 8B DD           mov         rbx,r13
  000000006505F055: FF 15 C5 77 63 00  call        qword ptr [0000000065696820h]
  000000006505F05B: 48 8B C8           mov         rcx,rax
  000000006505F05E: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000006505F063: E8 58 7E FB FF     call        0000000065016EC0
  000000006505F068: 90                 nop
  000000006505F069: C7 44 24 60 01 00  mov         dword ptr [rsp+60h],1
                    00 00
  000000006505F071: 48 8B C8           mov         rcx,rax
  000000006505F074: FF 15 6E 79 63 00  call        qword ptr [00000000656969E8h]
  000000006505F07A: 84 C0              test        al,al
  000000006505F07C: 74 1A              je          000000006505F098
  000000006505F07E: 48 8B D3           mov         rdx,rbx
  000000006505F081: 48 8D 0D B8 A1 68  lea         rcx,[00000000656E9240h]
                    00
  000000006505F088: FF 15 7A 79 63 00  call        qword ptr [0000000065696A08h]
  000000006505F08E: 48 85 C0           test        rax,rax
  000000006505F091: 75 05              jne         000000006505F098
  000000006505F093: 40 32 ED           xor         bpl,bpl
  000000006505F096: EB 03              jmp         000000006505F09B
  000000006505F098: 40 B5 01           mov         bpl,1
  000000006505F09B: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006505F0A0: FF 15 BA 76 63 00  call        qword ptr [0000000065696760h]
  000000006505F0A6: 40 84 ED           test        bpl,bpl
  000000006505F0A9: 74 67              je          000000006505F112
  000000006505F0AB: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  000000006505F0B0: 8B 88 CC 01 00 00  mov         ecx,dword ptr [rax+00000000000001CCh]
  000000006505F0B6: C1 E9 16           shr         ecx,16h
  000000006505F0B9: 80 E1 01           and         cl,1
  000000006505F0BC: 74 29              je          000000006505F0E7
  000000006505F0BE: B9 48 00 00 00     mov         ecx,48h
  000000006505F0C3: E8 AA 20 58 00     call        00000000655E1172
  000000006505F0C8: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006505F0CD: 48 85 C0           test        rax,rax
  000000006505F0D0: 74 10              je          000000006505F0E2
  000000006505F0D2: 48 8B D7           mov         rdx,rdi
  000000006505F0D5: 48 8B C8           mov         rcx,rax
  000000006505F0D8: E8 03 18 28 00     call        00000000652E08E0
  000000006505F0DD: 48 8B D8           mov         rbx,rax
  000000006505F0E0: EB 03              jmp         000000006505F0E5
  000000006505F0E2: 49 8B DD           mov         rbx,r13
  000000006505F0E5: EB 18              jmp         000000006505F0FF
  000000006505F0E7: 48 8B D3           mov         rdx,rbx
  000000006505F0EA: 48 8D 0D 4F A1 68  lea         rcx,[00000000656E9240h]
                    00
  000000006505F0F1: FF 15 11 79 63 00  call        qword ptr [0000000065696A08h]
  000000006505F0F7: 48 85 C0           test        rax,rax
  000000006505F0FA: 74 03              je          000000006505F0FF
  000000006505F0FC: FF 40 20           inc         dword ptr [rax+20h]
  000000006505F0FF: 45 33 C9           xor         r9d,r9d
  000000006505F102: 41 B0 01           mov         r8b,1
  000000006505F105: 48 8B D3           mov         rdx,rbx
  000000006505F108: 48 8B CE           mov         rcx,rsi
  000000006505F10B: E8 00 4B 00 00     call        0000000065063C10
  000000006505F110: EB 3C              jmp         000000006505F14E
  000000006505F112: 48 8B 86 88 00 00  mov         rax,qword ptr [rsi+0000000000000088h]
                    00
  000000006505F119: 48 85 C0           test        rax,rax
  000000006505F11C: 74 06              je          000000006505F124
  000000006505F11E: 48 8B 40 20        mov         rax,qword ptr [rax+20h]
  000000006505F122: EB 03              jmp         000000006505F127
  000000006505F124: 49 8B C5           mov         rax,r13
  000000006505F127: 48 3B F8           cmp         rdi,rax
  000000006505F12A: 74 22              je          000000006505F14E
  000000006505F12C: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  000000006505F131: 0F BA A0 CC 01 00  bt          dword ptr [rax+00000000000001CCh],16h
                    00 16
  000000006505F139: 49 0F 43 FD        cmovae      rdi,r13
  000000006505F13D: 45 33 C9           xor         r9d,r9d
  000000006505F140: 41 B0 01           mov         r8b,1
  000000006505F143: 48 8B D7           mov         rdx,rdi
  000000006505F146: 48 8B CE           mov         rcx,rsi
  000000006505F149: E8 C2 4A 00 00     call        0000000065063C10
  000000006505F14E: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  000000006505F153: 48 83 C4 30        add         rsp,30h
  000000006505F157: 41 5D              pop         r13
  000000006505F159: 41 5C              pop         r12
  000000006505F15B: 5F                 pop         rdi
  000000006505F15C: 5E                 pop         rsi
  000000006505F15D: 5D                 pop         rbp
  000000006505F15E: C3                 ret

  Summary

      695000 .text
