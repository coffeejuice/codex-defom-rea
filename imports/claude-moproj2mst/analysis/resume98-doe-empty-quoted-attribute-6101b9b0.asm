Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  000000006101B9B0: 40 53              push        rbx
  000000006101B9B2: 55                 push        rbp
  000000006101B9B3: 56                 push        rsi
  000000006101B9B4: 57                 push        rdi
  000000006101B9B5: 41 54              push        r12
  000000006101B9B7: 41 55              push        r13
  000000006101B9B9: 48 83 EC 68        sub         rsp,68h
  000000006101B9BD: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006101B9C6: 48 8B F9           mov         rdi,rcx
  000000006101B9C9: 48 8B 89 D8 00 00  mov         rcx,qword ptr [rcx+00000000000000D8h]
                    00
  000000006101B9D0: 48 85 C9           test        rcx,rcx
  000000006101B9D3: 74 6D              je          000000006101BA42
  000000006101B9D5: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006101B9D8: 83 78 08 00        cmp         dword ptr [rax+8],0
  000000006101B9DC: 74 64              je          000000006101BA42
  000000006101B9DE: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006101B9E3: E8 38 95 FF FF     call        0000000061014F20
  000000006101B9E8: 8B 70 08           mov         esi,dword ptr [rax+8]
  000000006101B9EB: 48 8B 8F D8 00 00  mov         rcx,qword ptr [rdi+00000000000000D8h]
                    00
  000000006101B9F2: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006101B9F5: 83 78 08 00        cmp         dword ptr [rax+8],0
  000000006101B9F9: 74 49              je          000000006101BA44
  000000006101B9FB: E8 D0 95 FF FF     call        0000000061014FD0
  000000006101BA00: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006101BA03: 48 8D 05 66 B6 FF  lea         rax,[0000000061017070h]
                    FF
  000000006101BA0A: 48 3B D8           cmp         rbx,rax
  000000006101BA0D: 75 11              jne         000000006101BA20
  000000006101BA0F: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006101BA14: 48 8B 8F D8 00 00  mov         rcx,qword ptr [rdi+00000000000000D8h]
                    00
  000000006101BA1B: E8 00 95 FF FF     call        0000000061014F20
  000000006101BA20: 48 8B CF           mov         rcx,rdi
  000000006101BA23: FF D3              call        rbx
  000000006101BA25: 84 C0              test        al,al
  000000006101BA27: 75 1B              jne         000000006101BA44
  000000006101BA29: 44 8B C6           mov         r8d,esi
  000000006101BA2C: 48 8D 15 7D FF FF  lea         rdx,[000000006101B9B0h]
                    FF
  000000006101BA33: 48 8B CF           mov         rcx,rdi
  000000006101BA36: E8 75 AA FF FF     call        00000000610164B0
  000000006101BA3B: 32 C0              xor         al,al
  000000006101BA3D: E9 9D 03 00 00     jmp         000000006101BDDF
  000000006101BA42: 33 F6              xor         esi,esi
  000000006101BA44: 83 FE FF           cmp         esi,0FFFFFFFFh
  000000006101BA47: 0F 84 4A 03 00 00  je          000000006101BD97
  000000006101BA4D: 41 BD FF FF 00 00  mov         r13d,0FFFFh
  000000006101BA53: 4C 8D 25 A6 45 FE  lea         r12,[0000000061000000h]
                    FF
  000000006101BA5A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006101BA60: 83 FE 04           cmp         esi,4
  000000006101BA63: 0F 84 2A 03 00 00  je          000000006101BD93
  000000006101BA69: 48 8D 8F 20 01 00  lea         rcx,[rdi+0000000000000120h]
                    00
  000000006101BA70: FF 15 52 7E 02 00  call        qword ptr [00000000610438C8h]
  000000006101BA76: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101BA79: 66 83 C9 01        or          cx,1
  000000006101BA7D: 66 41 3B CD        cmp         cx,r13w
  000000006101BA81: 0F 84 F6 02 00 00  je          000000006101BD7D
  000000006101BA87: 0F B7 87 20 01 00  movzx       eax,word ptr [rdi+0000000000000120h]
                    00
  000000006101BA8E: 66 89 84 24 A0 00  mov         word ptr [rsp+00000000000000A0h],ax
                    00 00
  000000006101BA96: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006101BA9E: FF 15 24 7E 02 00  call        qword ptr [00000000610438C8h]
  000000006101BAA4: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101BAA7: F7 C1 80 FF FF FF  test        ecx,0FFFFFF80h
  000000006101BAAD: 75 15              jne         000000006101BAC4
  000000006101BAAF: 0F B7 C1           movzx       eax,cx
  000000006101BAB2: 42 0F BE 84 20 70  movsx       eax,byte ptr [rax+r12+0000000000046870h]
                    68 04 00
  000000006101BABB: 85 C0              test        eax,eax
  000000006101BABD: 75 2A              jne         000000006101BAE9
  000000006101BABF: E9 17 01 00 00     jmp         000000006101BBDB
  000000006101BAC4: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006101BACC: FF 15 E6 78 02 00  call        qword ptr [00000000610433B8h]
  000000006101BAD2: 83 F8 0F           cmp         eax,0Fh
  000000006101BAD5: 7C 09              jl          000000006101BAE0
  000000006101BAD7: 83 F8 13           cmp         eax,13h
  000000006101BADA: 0F 8E FB 00 00 00  jle         000000006101BBDB
  000000006101BAE0: 83 F8 05           cmp         eax,5
  000000006101BAE3: 0F 84 F2 00 00 00  je          000000006101BBDB
  000000006101BAE9: B2 3D              mov         dl,3Dh
  000000006101BAEB: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006101BAF0: FF 15 92 7D 02 00  call        qword ptr [0000000061043888h]
  000000006101BAF6: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101BAF9: 66 89 8C 24 A8 00  mov         word ptr [rsp+00000000000000A8h],cx
                    00 00
  000000006101BB01: 0F B7 87 20 01 00  movzx       eax,word ptr [rdi+0000000000000120h]
                    00
  000000006101BB08: 66 89 84 24 B0 00  mov         word ptr [rsp+00000000000000B0h],ax
                    00 00
  000000006101BB10: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006101BB18: FF 15 AA 7D 02 00  call        qword ptr [00000000610438C8h]
  000000006101BB1E: 48 8B D8           mov         rbx,rax
  000000006101BB21: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000006101BB29: FF 15 99 7D 02 00  call        qword ptr [00000000610438C8h]
  000000006101BB2F: 0F B7 0B           movzx       ecx,word ptr [rbx]
  000000006101BB32: 66 39 08           cmp         word ptr [rax],cx
  000000006101BB35: 75 0A              jne         000000006101BB41
  000000006101BB37: BA 01 00 00 00     mov         edx,1
  000000006101BB3C: E9 9C 00 00 00     jmp         000000006101BBDD
  000000006101BB41: B2 22              mov         dl,22h
  000000006101BB43: 48 8D 4C 24 3A     lea         rcx,[rsp+3Ah]
  000000006101BB48: FF 15 3A 7D 02 00  call        qword ptr [0000000061043888h]
  000000006101BB4E: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101BB51: 66 89 8C 24 B8 00  mov         word ptr [rsp+00000000000000B8h],cx
                    00 00
  000000006101BB59: 0F B7 87 20 01 00  movzx       eax,word ptr [rdi+0000000000000120h]
                    00
  000000006101BB60: 66 89 44 24 20     mov         word ptr [rsp+20h],ax
  000000006101BB65: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000006101BB6D: FF 15 55 7D 02 00  call        qword ptr [00000000610438C8h]
  000000006101BB73: 48 8B D8           mov         rbx,rax
  000000006101BB76: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006101BB7B: FF 15 47 7D 02 00  call        qword ptr [00000000610438C8h]
  000000006101BB81: 0F B7 0B           movzx       ecx,word ptr [rbx]
  000000006101BB84: 66 39 08           cmp         word ptr [rax],cx
  000000006101BB87: 75 07              jne         000000006101BB90
  000000006101BB89: BA 02 00 00 00     mov         edx,2
  000000006101BB8E: EB 4D              jmp         000000006101BBDD
  000000006101BB90: B2 27              mov         dl,27h
  000000006101BB92: 48 8D 4C 24 3C     lea         rcx,[rsp+3Ch]
  000000006101BB97: FF 15 EB 7C 02 00  call        qword ptr [0000000061043888h]
  000000006101BB9D: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101BBA0: 66 89 4C 24 28     mov         word ptr [rsp+28h],cx
  000000006101BBA5: 0F B7 87 20 01 00  movzx       eax,word ptr [rdi+0000000000000120h]
                    00
  000000006101BBAC: 66 89 44 24 30     mov         word ptr [rsp+30h],ax
  000000006101BBB1: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006101BBB6: FF 15 0C 7D 02 00  call        qword ptr [00000000610438C8h]
  000000006101BBBC: 48 8B D8           mov         rbx,rax
  000000006101BBBF: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006101BBC4: FF 15 FE 7C 02 00  call        qword ptr [00000000610438C8h]
  000000006101BBCA: 33 D2              xor         edx,edx
  000000006101BBCC: 0F B7 0B           movzx       ecx,word ptr [rbx]
  000000006101BBCF: 66 39 08           cmp         word ptr [rax],cx
  000000006101BBD2: 0F 95 C2           setne       dl
  000000006101BBD5: 48 83 C2 03        add         rdx,3
  000000006101BBD9: EB 02              jmp         000000006101BBDD
  000000006101BBDB: 33 D2              xor         edx,edx
  000000006101BBDD: 48 63 CE           movsxd      rcx,esi
  000000006101BBE0: 48 8D 04 8A        lea         rax,[rdx+rcx*4]
  000000006101BBE4: 48 03 C8           add         rcx,rax
  000000006101BBE7: 41 8B B4 8C 20 7F  mov         esi,dword ptr [r12+rcx*4+0000000000047F20h]
                    04 00
  000000006101BBEF: 8B CE              mov         ecx,esi
  000000006101BBF1: 83 E9 01           sub         ecx,1
  000000006101BBF4: 74 49              je          000000006101BC3F
  000000006101BBF6: 83 E9 01           sub         ecx,1
  000000006101BBF9: 74 32              je          000000006101BC2D
  000000006101BBFB: 83 E9 01           sub         ecx,1
  000000006101BBFE: 74 13              je          000000006101BC13
  000000006101BC00: 83 F9 01           cmp         ecx,1
  000000006101BC03: 75 51              jne         000000006101BC56
  000000006101BC05: 48 8B CF           mov         rcx,rdi
  000000006101BC08: E8 D3 EB FF FF     call        000000006101A7E0
  000000006101BC0D: 84 C0              test        al,al
  000000006101BC0F: 74 53              je          000000006101BC64
  000000006101BC11: EB 43              jmp         000000006101BC56
  000000006101BC13: 48 8B CF           mov         rcx,rdi
  000000006101BC16: E8 25 B2 FF FF     call        0000000061016E40
  000000006101BC1B: 48 8B CF           mov         rcx,rdi
  000000006101BC1E: E8 4D B4 FF FF     call        0000000061017070
  000000006101BC23: 84 C0              test        al,al
  000000006101BC25: 0F 84 80 00 00 00  je          000000006101BCAB
  000000006101BC2B: EB 29              jmp         000000006101BC56
  000000006101BC2D: 48 8B CF           mov         rcx,rdi
  000000006101BC30: E8 3B B4 FF FF     call        0000000061017070
  000000006101BC35: 84 C0              test        al,al
  000000006101BC37: 0F 84 B5 00 00 00  je          000000006101BCF2
  000000006101BC3D: EB 17              jmp         000000006101BC56
  000000006101BC3F: C6 87 C1 00 00 00  mov         byte ptr [rdi+00000000000000C1h],0
                    00
  000000006101BC46: 48 8B CF           mov         rcx,rdi
  000000006101BC49: E8 22 C8 FF FF     call        0000000061018470
  000000006101BC4E: 84 C0              test        al,al
  000000006101BC50: 0F 84 E3 00 00 00  je          000000006101BD39
  000000006101BC56: 83 FE FF           cmp         esi,0FFFFFFFFh
  000000006101BC59: 0F 84 38 01 00 00  je          000000006101BD97
  000000006101BC5F: E9 FC FD FF FF     jmp         000000006101BA60
  000000006101BC64: 48 83 BF D8 00 00  cmp         qword ptr [rdi+00000000000000D8h],0
                    00 00
  000000006101BC6C: 74 36              je          000000006101BCA4
  000000006101BC6E: 48 8D 8F B8 00 00  lea         rcx,[rdi+00000000000000B8h]
                    00
  000000006101BC75: FF 15 F5 77 02 00  call        qword ptr [0000000061043470h]
  000000006101BC7B: 84 C0              test        al,al
  000000006101BC7D: 74 25              je          000000006101BCA4
  000000006101BC7F: 48 8D 15 2A FD FF  lea         rdx,[000000006101B9B0h]
                    FF
  000000006101BC86: 48 89 54 24 48     mov         qword ptr [rsp+48h],rdx
  000000006101BC8B: C7 44 24 50 04 00  mov         dword ptr [rsp+50h],4
                    00 00
  000000006101BC93: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006101BC98: 48 8B 8F D8 00 00  mov         rcx,qword ptr [rdi+00000000000000D8h]
                    00
  000000006101BC9F: E8 4C 78 FF FF     call        00000000610134F0
  000000006101BCA4: 32 C0              xor         al,al
  000000006101BCA6: E9 34 01 00 00     jmp         000000006101BDDF
  000000006101BCAB: 48 83 BF D8 00 00  cmp         qword ptr [rdi+00000000000000D8h],0
                    00 00
  000000006101BCB3: 74 36              je          000000006101BCEB
  000000006101BCB5: 48 8D 8F B8 00 00  lea         rcx,[rdi+00000000000000B8h]
                    00
  000000006101BCBC: FF 15 AE 77 02 00  call        qword ptr [0000000061043470h]
  000000006101BCC2: 84 C0              test        al,al
  000000006101BCC4: 74 25              je          000000006101BCEB
  000000006101BCC6: 48 8D 15 E3 FC FF  lea         rdx,[000000006101B9B0h]
                    FF
  000000006101BCCD: 48 89 54 24 48     mov         qword ptr [rsp+48h],rdx
  000000006101BCD2: C7 44 24 50 03 00  mov         dword ptr [rsp+50h],3
                    00 00
  000000006101BCDA: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006101BCDF: 48 8B 8F D8 00 00  mov         rcx,qword ptr [rdi+00000000000000D8h]
                    00
  000000006101BCE6: E8 05 78 FF FF     call        00000000610134F0
  000000006101BCEB: 32 C0              xor         al,al
  000000006101BCED: E9 ED 00 00 00     jmp         000000006101BDDF
  000000006101BCF2: 48 83 BF D8 00 00  cmp         qword ptr [rdi+00000000000000D8h],0
                    00 00
  000000006101BCFA: 74 36              je          000000006101BD32
  000000006101BCFC: 48 8D 8F B8 00 00  lea         rcx,[rdi+00000000000000B8h]
                    00
  000000006101BD03: FF 15 67 77 02 00  call        qword ptr [0000000061043470h]
  000000006101BD09: 84 C0              test        al,al
  000000006101BD0B: 74 25              je          000000006101BD32
  000000006101BD0D: 48 8D 15 9C FC FF  lea         rdx,[000000006101B9B0h]
                    FF
  000000006101BD14: 48 89 54 24 48     mov         qword ptr [rsp+48h],rdx
  000000006101BD19: C7 44 24 50 02 00  mov         dword ptr [rsp+50h],2
                    00 00
  000000006101BD21: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006101BD26: 48 8B 8F D8 00 00  mov         rcx,qword ptr [rdi+00000000000000D8h]
                    00
  000000006101BD2D: E8 BE 77 FF FF     call        00000000610134F0
  000000006101BD32: 32 C0              xor         al,al
  000000006101BD34: E9 A6 00 00 00     jmp         000000006101BDDF
  000000006101BD39: 48 83 BF D8 00 00  cmp         qword ptr [rdi+00000000000000D8h],0
                    00 00
  000000006101BD41: 74 36              je          000000006101BD79
  000000006101BD43: 48 8D 8F B8 00 00  lea         rcx,[rdi+00000000000000B8h]
                    00
  000000006101BD4A: FF 15 20 77 02 00  call        qword ptr [0000000061043470h]
  000000006101BD50: 84 C0              test        al,al
  000000006101BD52: 74 25              je          000000006101BD79
  000000006101BD54: 48 8D 15 55 FC FF  lea         rdx,[000000006101B9B0h]
                    FF
  000000006101BD5B: 48 89 54 24 48     mov         qword ptr [rsp+48h],rdx
  000000006101BD60: C7 44 24 50 01 00  mov         dword ptr [rsp+50h],1
                    00 00
  000000006101BD68: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006101BD6D: 48 8B 8F D8 00 00  mov         rcx,qword ptr [rdi+00000000000000D8h]
                    00
  000000006101BD74: E8 77 77 FF FF     call        00000000610134F0
  000000006101BD79: 32 C0              xor         al,al
  000000006101BD7B: EB 62              jmp         000000006101BDDF
  000000006101BD7D: 44 8B C6           mov         r8d,esi
  000000006101BD80: 48 8D 15 29 FC FF  lea         rdx,[000000006101B9B0h]
                    FF
  000000006101BD87: 48 8B CF           mov         rcx,rdi
  000000006101BD8A: E8 11 A6 FF FF     call        00000000610163A0
  000000006101BD8F: 32 C0              xor         al,al
  000000006101BD91: EB 4C              jmp         000000006101BDDF
  000000006101BD93: B0 01              mov         al,1
  000000006101BD95: EB 48              jmp         000000006101BDDF
  000000006101BD97: 48 8D 15 8A BD 02  lea         rdx,[0000000061047B28h]
                    00
  000000006101BD9E: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006101BDA6: FF 15 EC 7A 02 00  call        qword ptr [0000000061043898h]
  000000006101BDAC: 48 8B D0           mov         rdx,rax
  000000006101BDAF: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006101BDB7: FF 15 03 7A 02 00  call        qword ptr [00000000610437C0h]
  000000006101BDBD: 90                 nop
  000000006101BDBE: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006101BDC6: 48 8B CF           mov         rcx,rdi
  000000006101BDC9: E8 82 80 FF FF     call        0000000061013E50
  000000006101BDCE: 90                 nop
  000000006101BDCF: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006101BDD7: FF 15 03 7B 02 00  call        qword ptr [00000000610438E0h]
  000000006101BDDD: 32 C0              xor         al,al
  000000006101BDDF: 48 83 C4 68        add         rsp,68h
  000000006101BDE3: 41 5D              pop         r13
  000000006101BDE5: 41 5C              pop         r12
  000000006101BDE7: 5F                 pop         rdi
  000000006101BDE8: 5E                 pop         rsi
  000000006101BDE9: 5D                 pop         rbp
  000000006101BDEA: 5B                 pop         rbx
  000000006101BDEB: C3                 ret

  Summary

       42000 .text
