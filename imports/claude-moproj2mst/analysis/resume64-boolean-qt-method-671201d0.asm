Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000671201D0: 48 8B C4           mov         rax,rsp
  00000000671201D3: 44 89 40 18        mov         dword ptr [rax+18h],r8d
  00000000671201D7: 48 89 50 10        mov         qword ptr [rax+10h],rdx
  00000000671201DB: 48 89 48 08        mov         qword ptr [rax+8],rcx
  00000000671201DF: 55                 push        rbp
  00000000671201E0: 56                 push        rsi
  00000000671201E1: 57                 push        rdi
  00000000671201E2: 41 54              push        r12
  00000000671201E4: 41 55              push        r13
  00000000671201E6: 41 56              push        r14
  00000000671201E8: 41 57              push        r15
  00000000671201EA: 48 81 EC 50 01 00  sub         rsp,150h
                    00
  00000000671201F1: 48 C7 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000000671201FD: 48 89 58 20        mov         qword ptr [rax+20h],rbx
  0000000067120201: 4D 8B F1           mov         r14,r9
  0000000067120204: 48 8B EA           mov         rbp,rdx
  0000000067120207: 4C 8B C1           mov         r8,rcx
  000000006712020A: 48 85 D2           test        rdx,rdx
  000000006712020D: 0F 84 39 07 00 00  je          000000006712094C
  0000000067120213: 48 8B 11           mov         rdx,qword ptr [rcx]
  0000000067120216: 48 85 D2           test        rdx,rdx
  0000000067120219: 0F 84 2D 07 00 00  je          000000006712094C
  000000006712021F: 48 83 CE FF        or          rsi,0FFFFFFFFFFFFFFFFh
  0000000067120223: 49 83 39 00        cmp         qword ptr [r9],0
  0000000067120227: 0F 84 5F 01 00 00  je          000000006712038C
  000000006712022D: 8B 49 08           mov         ecx,dword ptr [rcx+8]
  0000000067120230: 83 C1 02           add         ecx,2
  0000000067120233: 48 8B 42 10        mov         rax,qword ptr [rdx+10h]
  0000000067120237: 8B 3C 88           mov         edi,dword ptr [rax+rcx*4]
  000000006712023A: 48 03 7A 08        add         rdi,qword ptr [rdx+8]
  000000006712023E: 48 8B D7           mov         rdx,rdi
  0000000067120241: 49 8B 49 08        mov         rcx,qword ptr [r9+8]
  0000000067120245: E8 06 C9 F0 FF     call        000000006702CB50
  000000006712024A: 85 C0              test        eax,eax
  000000006712024C: 0F 84 28 01 00 00  je          000000006712037A
  0000000067120252: 48 8D 05 A7 06 1D  lea         rax,[00000000672F0900h]
                    00
  0000000067120259: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000006712025E: F0 83 05 9A 06 1D  lock add    dword ptr [00000000672F0900h],1
                    00 01
  0000000067120266: 49 8B 4E 08        mov         rcx,qword ptr [r14+8]
  000000006712026A: E8 41 19 EE FF     call        0000000067001BB0
  000000006712026F: 8D 50 03           lea         edx,[rax+3]
  0000000067120272: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000067120277: 8B 08              mov         ecx,dword ptr [rax]
  0000000067120279: 83 F9 01           cmp         ecx,1
  000000006712027C: 75 0A              jne         0000000067120288
  000000006712027E: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000067120283: 3B 50 04           cmp         edx,dword ptr [rax+4]
  0000000067120286: 7E 0A              jle         0000000067120292
  0000000067120288: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006712028D: E8 7E D4 F0 FF     call        000000006702D710
  0000000067120292: 48 8D 15 6F A5 11  lea         rdx,[000000006723A808h]
                    00
  0000000067120299: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006712029E: E8 8D E8 F0 FF     call        000000006702EB30
  00000000671202A3: 49 8B 56 08        mov         rdx,qword ptr [r14+8]
  00000000671202A7: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000671202AC: E8 2F D8 F0 FF     call        000000006702DAE0
  00000000671202B1: B2 29              mov         dl,29h
  00000000671202B3: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000671202B8: E8 F3 D9 F0 FF     call        000000006702DCB0
  00000000671202BD: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000671202C2: 48 8B 51 10        mov         rdx,qword ptr [rcx+10h]
  00000000671202C6: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000671202CB: E8 90 FC FF FF     call        000000006711FF60
  00000000671202D0: 90                 nop
  00000000671202D1: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000000671202D6: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000671202D9: FF CA              dec         edx
  00000000671202DB: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000671202E0: E8 EB 02 F1 FF     call        00000000670305D0
  00000000671202E5: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000000671202EA: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000000671202EE: 48 83 C1 02        add         rcx,2
  00000000671202F2: 48 8B D7           mov         rdx,rdi
  00000000671202F5: E8 56 C8 F0 FF     call        000000006702CB50
  00000000671202FA: 85 C0              test        eax,eax
  00000000671202FC: 74 3D              je          000000006712033B
  00000000671202FE: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000067120303: 48 83 CF FF        or          rdi,0FFFFFFFFFFFFFFFFh
  0000000067120307: 8B C7              mov         eax,edi
  0000000067120309: F0 0F C1 01        lock xadd   dword ptr [rcx],eax
  000000006712030D: 03 C7              add         eax,edi
  000000006712030F: 75 0B              jne         000000006712031C
  0000000067120311: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000067120316: E8 95 00 F0 FF     call        00000000670203B0
  000000006712031B: 90                 nop
  000000006712031C: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000067120321: F0 0F C1 38        lock xadd   dword ptr [rax],edi
  0000000067120325: 83 C7 FF           add         edi,0FFFFFFFFh
  0000000067120328: 75 0A              jne         0000000067120334
  000000006712032A: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000006712032F: E8 7C 00 F0 FF     call        00000000670203B0
  0000000067120334: 32 C0              xor         al,al
  0000000067120336: E9 13 06 00 00     jmp         000000006712094E
  000000006712033B: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000067120340: 8B CE              mov         ecx,esi
  0000000067120342: F0 0F C1 08        lock xadd   dword ptr [rax],ecx
  0000000067120346: 83 C1 FF           add         ecx,0FFFFFFFFh
  0000000067120349: 75 0B              jne         0000000067120356
  000000006712034B: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000067120350: E8 5B 00 F0 FF     call        00000000670203B0
  0000000067120355: 90                 nop
  0000000067120356: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000006712035B: 8B CE              mov         ecx,esi
  000000006712035D: F0 0F C1 08        lock xadd   dword ptr [rax],ecx
  0000000067120361: 83 C1 FF           add         ecx,0FFFFFFFFh
  0000000067120364: 75 1E              jne         0000000067120384
  0000000067120366: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000006712036B: E8 40 00 F0 FF     call        00000000670203B0
  0000000067120370: 4C 8B 84 24 90 01  mov         r8,qword ptr [rsp+0000000000000190h]
                    00 00
  0000000067120378: EB 12              jmp         000000006712038C
  000000006712037A: 4C 8B 84 24 90 01  mov         r8,qword ptr [rsp+0000000000000190h]
                    00 00
  0000000067120382: EB 08              jmp         000000006712038C
  0000000067120384: 4C 8B 84 24 90 01  mov         r8,qword ptr [rsp+0000000000000190h]
                    00 00
  000000006712038C: 49 8B 46 08        mov         rax,qword ptr [r14+8]
  0000000067120390: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000067120398: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000000671203A0: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000671203A4: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  00000000671203AC: 48 8B 84 24 B8 01  mov         rax,qword ptr [rsp+00000000000001B8h]
                    00 00
  00000000671203B4: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000671203B8: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000000671203C0: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000000671203C8: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000671203CC: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  00000000671203D4: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000000671203DC: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000671203E0: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  00000000671203E8: 48 8B 84 24 D0 01  mov         rax,qword ptr [rsp+00000000000001D0h]
                    00 00
  00000000671203F0: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000671203F4: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  00000000671203FC: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000067120404: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067120408: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  0000000067120410: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000067120418: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006712041C: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  0000000067120424: 48 8B 84 24 E8 01  mov         rax,qword ptr [rsp+00000000000001E8h]
                    00 00
  000000006712042C: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067120430: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  0000000067120438: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000067120440: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067120444: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  000000006712044C: 48 8B 84 24 F8 01  mov         rax,qword ptr [rsp+00000000000001F8h]
                    00 00
  0000000067120454: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000067120458: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  0000000067120460: BB 01 00 00 00     mov         ebx,1
  0000000067120465: 44 8B E3           mov         r12d,ebx
  0000000067120468: 48 8B D3           mov         rdx,rbx
  000000006712046B: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  0000000067120470: 48 8B BC D4 E0 00  mov         rdi,qword ptr [rsp+rdx*8+00000000000000E0h]
                    00 00
  0000000067120478: 48 85 FF           test        rdi,rdi
  000000006712047B: 74 1D              je          000000006712049A
  000000006712047D: 33 C0              xor         eax,eax
  000000006712047F: 48 8B CE           mov         rcx,rsi
  0000000067120482: F2 AE              repne scas  byte ptr [rdi]
  0000000067120484: 48 F7 D1           not         rcx
  0000000067120487: 48 FF C9           dec         rcx
  000000006712048A: 85 C9              test        ecx,ecx
  000000006712048C: 74 0C              je          000000006712049A
  000000006712048E: 41 FF C4           inc         r12d
  0000000067120491: 48 FF C2           inc         rdx
  0000000067120494: 48 83 FA 0B        cmp         rdx,0Bh
  0000000067120498: 7C D6              jl          0000000067120470
  000000006712049A: 44 89 64 24 60     mov         dword ptr [rsp+60h],r12d
  000000006712049F: 45 33 FF           xor         r15d,r15d
  00000000671204A2: 41 8B D7           mov         edx,r15d
  00000000671204A5: 41 8B 78 08        mov         edi,dword ptr [r8+8]
  00000000671204A9: 49 8B 00           mov         rax,qword ptr [r8]
  00000000671204AC: 4C 8B 40 10        mov         r8,qword ptr [rax+10h]
  00000000671204B0: 4C 8B 48 08        mov         r9,qword ptr [rax+8]
  00000000671204B4: 8D 47 01           lea         eax,[rdi+1]
  00000000671204B7: 8B C8              mov         ecx,eax
  00000000671204B9: 41 8B 04 80        mov         eax,dword ptr [r8+rax*4]
  00000000671204BD: 49 03 C1           add         rax,r9
  00000000671204C0: 38 10              cmp         byte ptr [rax],dl
  00000000671204C2: 75 28              jne         00000000671204EC
  00000000671204C4: 48 8B C7           mov         rax,rdi
  00000000671204C7: 41 8B 0C B8        mov         ecx,dword ptr [r8+rdi*4]
  00000000671204CB: 49 03 C9           add         rcx,r9
  00000000671204CE: 0F B6 01           movzx       eax,byte ptr [rcx]
  00000000671204D1: 84 C0              test        al,al
  00000000671204D3: 74 0D              je          00000000671204E2
  00000000671204D5: 3C 28              cmp         al,28h
  00000000671204D7: 74 09              je          00000000671204E2
  00000000671204D9: 48 FF C1           inc         rcx
  00000000671204DC: 8A 01              mov         al,byte ptr [rcx]
  00000000671204DE: 84 C0              test        al,al
  00000000671204E0: 75 F3              jne         00000000671204D5
  00000000671204E2: 80 79 01 29        cmp         byte ptr [rcx+1],29h
  00000000671204E6: 74 2D              je          0000000067120515
  00000000671204E8: 8B D3              mov         edx,ebx
  00000000671204EA: EB 29              jmp         0000000067120515
  00000000671204EC: 48 FF C8           dec         rax
  00000000671204EF: 90                 nop
  00000000671204F0: 48 FF C0           inc         rax
  00000000671204F3: 0F B6 08           movzx       ecx,byte ptr [rax]
  00000000671204F6: 84 C9              test        cl,cl
  00000000671204F8: 74 14              je          000000006712050E
  00000000671204FA: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  0000000067120500: 80 F9 2C           cmp         cl,2Ch
  0000000067120503: 74 09              je          000000006712050E
  0000000067120505: 48 FF C0           inc         rax
  0000000067120508: 8A 08              mov         cl,byte ptr [rax]
  000000006712050A: 84 C9              test        cl,cl
  000000006712050C: 75 F2              jne         0000000067120500
  000000006712050E: FF C2              inc         edx
  0000000067120510: 44 38 38           cmp         byte ptr [rax],r15b
  0000000067120513: 75 DB              jne         00000000671204F0
  0000000067120515: 44 3B E2           cmp         r12d,edx
  0000000067120518: 7F 07              jg          0000000067120521
  000000006712051A: 32 C0              xor         al,al
  000000006712051C: E9 2D 04 00 00     jmp         000000006712094E
  0000000067120521: E8 2A 71 F0 FF     call        0000000067027650
  0000000067120526: 4C 8B E8           mov         r13,rax
  0000000067120529: 48 8B CD           mov         rcx,rbp
  000000006712052C: E8 DF AB 00 00     call        000000006712B110
  0000000067120531: 4C 8B D8           mov         r11,rax
  0000000067120534: 44 8B 94 24 A0 01  mov         r10d,dword ptr [rsp+00000000000001A0h]
                    00 00
  000000006712053C: 45 85 D2           test        r10d,r10d
  000000006712053F: 75 0D              jne         000000006712054E
  0000000067120541: 45 8B D7           mov         r10d,r15d
  0000000067120544: 4C 3B E8           cmp         r13,rax
  0000000067120547: 41 0F 95 C2        setne       r10b
  000000006712054B: 41 FF C2           inc         r10d
  000000006712054E: 4D 8B 06           mov         r8,qword ptr [r14]
  0000000067120551: 4C 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],r8
                    00 00
  0000000067120559: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000067120561: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000067120564: 48 89 8C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rcx
                    00 00
  000000006712056C: 48 8B 84 24 B8 01  mov         rax,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000067120574: 48 8B 00           mov         rax,qword ptr [rax]
  0000000067120577: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  000000006712057F: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000067120587: 48 8B 00           mov         rax,qword ptr [rax]
  000000006712058A: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000067120592: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  000000006712059A: 48 8B 00           mov         rax,qword ptr [rax]
  000000006712059D: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000000671205A5: 48 8B 84 24 D0 01  mov         rax,qword ptr [rsp+00000000000001D0h]
                    00 00
  00000000671205AD: 48 8B 00           mov         rax,qword ptr [rax]
  00000000671205B0: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000000671205B8: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000000671205C0: 48 8B 00           mov         rax,qword ptr [rax]
  00000000671205C3: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000000671205CB: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000000671205D3: 48 8B 00           mov         rax,qword ptr [rax]
  00000000671205D6: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000000671205DE: 48 8B 84 24 E8 01  mov         rax,qword ptr [rsp+00000000000001E8h]
                    00 00
  00000000671205E6: 48 8B 00           mov         rax,qword ptr [rax]
  00000000671205E9: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  00000000671205F1: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000000671205F9: 48 8B 00           mov         rax,qword ptr [rax]
  00000000671205FC: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000067120604: 48 8B 84 24 F8 01  mov         rax,qword ptr [rsp+00000000000001F8h]
                    00 00
  000000006712060C: 48 8B 00           mov         rax,qword ptr [rax]
  000000006712060F: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000067120617: 48 8B 8C 24 90 01  mov         rcx,qword ptr [rsp+0000000000000190h]
                    00 00
  000000006712061F: 4C 8B 09           mov         r9,qword ptr [rcx]
  0000000067120622: 49 8B 41 10        mov         rax,qword ptr [r9+10h]
  0000000067120626: 8B 49 08           mov         ecx,dword ptr [rcx+8]
  0000000067120629: 2B 48 14           sub         ecx,dword ptr [rax+14h]
  000000006712062C: B8 CD CC CC CC     mov         eax,0CCCCCCCDh
  0000000067120631: F7 E1              mul         eax,ecx
  0000000067120633: 8B EA              mov         ebp,edx
  0000000067120635: C1 ED 02           shr         ebp,2
  0000000067120638: 89 6C 24 50        mov         dword ptr [rsp+50h],ebp
  000000006712063C: 45 8B F7           mov         r14d,r15d
  000000006712063F: 49 8B 09           mov         rcx,qword ptr [r9]
  0000000067120642: 48 85 C9           test        rcx,rcx
  0000000067120645: 74 19              je          0000000067120660
  0000000067120647: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  0000000067120650: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  0000000067120654: 44 03 70 10        add         r14d,dword ptr [rax+10h]
  0000000067120658: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000006712065B: 48 85 C9           test        rcx,rcx
  000000006712065E: 75 F0              jne         0000000067120650
  0000000067120660: 49 8B 41 10        mov         rax,qword ptr [r9+10h]
  0000000067120664: 83 38 06           cmp         dword ptr [rax],6
  0000000067120667: 7C 14              jl          000000006712067D
  0000000067120669: 49 8B 79 18        mov         rdi,qword ptr [r9+18h]
  000000006712066D: 48 85 FF           test        rdi,rdi
  0000000067120670: 74 0B              je          000000006712067D
  0000000067120672: 48 8B 7F 08        mov         rdi,qword ptr [rdi+8]
  0000000067120676: 48 89 7C 24 70     mov         qword ptr [rsp+70h],rdi
  000000006712067B: EB 08              jmp         0000000067120685
  000000006712067D: 49 8B FF           mov         rdi,r15
  0000000067120680: 4C 89 7C 24 70     mov         qword ptr [rsp+70h],r15
  0000000067120685: 44 3B D3           cmp         r10d,ebx
  0000000067120688: 75 71              jne         00000000671206FB
  000000006712068A: 48 85 FF           test        rdi,rdi
  000000006712068D: 74 1E              je          00000000671206AD
  000000006712068F: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  0000000067120697: 44 8B C5           mov         r8d,ebp
  000000006712069A: 33 D2              xor         edx,edx
  000000006712069C: 48 8B 8C 24 98 01  mov         rcx,qword ptr [rsp+0000000000000198h]
                    00 00
  00000000671206A4: FF D7              call        rdi
  00000000671206A6: 8A C3              mov         al,bl
  00000000671206A8: E9 A1 02 00 00     jmp         000000006712094E
  00000000671206AD: 45 8D 04 2E        lea         r8d,[r14+rbp]
  00000000671206B1: 4C 8B A4 24 98 01  mov         r12,qword ptr [rsp+0000000000000198h]
                    00 00
  00000000671206B9: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  00000000671206BE: 48 8B 48 28        mov         rcx,qword ptr [rax+28h]
  00000000671206C2: 48 85 C9           test        rcx,rcx
  00000000671206C5: 74 16              je          00000000671206DD
  00000000671206C7: 48 83 C1 F8        add         rcx,0FFFFFFFFFFFFFFF8h
  00000000671206CB: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000671206CE: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  00000000671206D6: 33 D2              xor         edx,edx
  00000000671206D8: FF 50 08           call        qword ptr [rax+8]
  00000000671206DB: EB 14              jmp         00000000671206F1
  00000000671206DD: 49 8B 04 24        mov         rax,qword ptr [r12]
  00000000671206E1: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  00000000671206E9: 33 D2              xor         edx,edx
  00000000671206EB: 49 8B CC           mov         rcx,r12
  00000000671206EE: FF 50 10           call        qword ptr [rax+10h]
  00000000671206F1: 85 C0              test        eax,eax
  00000000671206F3: 0F 98 C0           sets        al
  00000000671206F6: E9 53 02 00 00     jmp         000000006712094E
  00000000671206FB: 41 83 FA 02        cmp         r10d,2
  00000000671206FF: 0F 85 93 01 00 00  jne         0000000067120898
  0000000067120705: 4D 85 C0           test        r8,r8
  0000000067120708: 74 13              je          000000006712071D
  000000006712070A: 48 8D 0D 9F A0 11  lea         rcx,[000000006723A7B0h]
                    00
  0000000067120711: E8 CA C7 EF FF     call        000000006701CEE0
  0000000067120716: 32 C0              xor         al,al
  0000000067120718: E9 31 02 00 00     jmp         000000006712094E
  000000006712071D: 8B F3              mov         esi,ebx
  000000006712071F: 89 5C 24 58        mov         dword ptr [rsp+58h],ebx
  0000000067120723: 49 63 FC           movsxd      rdi,r12d
  0000000067120726: 48 8D 0C FD 00 00  lea         rcx,[rdi*8+0000000000000000h]
                    00 00
  000000006712072E: E8 6D FC EF FF     call        00000000670203A0
  0000000067120733: 4C 8B E0           mov         r12,rax
  0000000067120736: 48 85 C0           test        rax,rax
  0000000067120739: 75 05              jne         0000000067120740
  000000006712073B: E8 70 B8 EF FF     call        000000006701BFB0
  0000000067120740: 48 8D 0C BD 00 00  lea         rcx,[rdi*4+0000000000000000h]
                    00 00
  0000000067120748: E8 53 FC EF FF     call        00000000670203A0
  000000006712074D: 48 8B E8           mov         rbp,rax
  0000000067120750: 48 85 C0           test        rax,rax
  0000000067120753: 75 05              jne         000000006712075A
  0000000067120755: E8 56 B8 EF FF     call        000000006701BFB0
  000000006712075A: 44 89 7D 00        mov         dword ptr [rbp],r15d
  000000006712075E: 4D 89 3C 24        mov         qword ptr [r12],r15
  0000000067120762: 44 8B EB           mov         r13d,ebx
  0000000067120765: 48 8B FB           mov         rdi,rbx
  0000000067120768: 48 63 4C 24 60     movsxd      rcx,dword ptr [rsp+60h]
  000000006712076D: 48 3B D9           cmp         rbx,rcx
  0000000067120770: 0F 8D BF 00 00 00  jge         0000000067120835
  0000000067120776: 49 8D 74 24 08     lea         rsi,[r12+8]
  000000006712077B: 48 8D 84 24 E0 00  lea         rax,[rsp+00000000000000E0h]
                    00 00
  0000000067120783: 49 2B C4           sub         rax,r12
  0000000067120786: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000006712078B: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  0000000067120790: 48 8B 0C 30        mov         rcx,qword ptr [rax+rsi]
  0000000067120794: E8 E7 47 00 00     call        0000000067124F80
  0000000067120799: 89 44 BD 00        mov         dword ptr [rbp+rdi*4],eax
  000000006712079D: 85 C0              test        eax,eax
  000000006712079F: 74 18              je          00000000671207B9
  00000000671207A1: 48 8B 94 FC 80 00  mov         rdx,qword ptr [rsp+rdi*8+0000000000000080h]
                    00 00
  00000000671207A9: 8B C8              mov         ecx,eax
  00000000671207AB: E8 E0 5D 00 00     call        0000000067126590
  00000000671207B0: 48 89 06           mov         qword ptr [rsi],rax
  00000000671207B3: FF 44 24 58        inc         dword ptr [rsp+58h]
  00000000671207B7: EB 0A              jmp         00000000671207C3
  00000000671207B9: 4C 39 BC FC 80 00  cmp         qword ptr [rsp+rdi*8+0000000000000080h],r15
                    00 00
  00000000671207C1: 75 1F              jne         00000000671207E2
  00000000671207C3: 41 FF C5           inc         r13d
  00000000671207C6: 48 FF C7           inc         rdi
  00000000671207C9: 48 83 C6 08        add         rsi,8
  00000000671207CD: 48 63 4C 24 60     movsxd      rcx,dword ptr [rsp+60h]
  00000000671207D2: 48 3B F9           cmp         rdi,rcx
  00000000671207D5: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  00000000671207DA: 7C B4              jl          0000000067120790
  00000000671207DC: 8B 74 24 58        mov         esi,dword ptr [rsp+58h]
  00000000671207E0: EB 53              jmp         0000000067120835
  00000000671207E2: 49 63 D5           movsxd      rdx,r13d
  00000000671207E5: 48 8B 94 D4 E0 00  mov         rdx,qword ptr [rsp+rdx*8+00000000000000E0h]
                    00 00
  00000000671207ED: 48 8D 0D 6C 9F 11  lea         rcx,[000000006723A760h]
                    00
  00000000671207F4: E8 E7 C6 EF FF     call        000000006701CEE0
  00000000671207F9: 48 3B DF           cmp         rbx,rdi
  00000000671207FC: 7D 20              jge         000000006712081E
  00000000671207FE: 66 90              nop
  0000000067120800: 8B 4C 9D 00        mov         ecx,dword ptr [rbp+rbx*4]
  0000000067120804: 85 C9              test        ecx,ecx
  0000000067120806: 74 0E              je          0000000067120816
  0000000067120808: 49 8B 14 DC        mov         rdx,qword ptr [r12+rbx*8]
  000000006712080C: 48 85 D2           test        rdx,rdx
  000000006712080F: 74 05              je          0000000067120816
  0000000067120811: E8 7A 4D 00 00     call        0000000067125590
  0000000067120816: 48 FF C3           inc         rbx
  0000000067120819: 48 3B DF           cmp         rbx,rdi
  000000006712081C: 7C E2              jl          0000000067120800
  000000006712081E: 48 8B CD           mov         rcx,rbp
  0000000067120821: E8 8A FB EF FF     call        00000000670203B0
  0000000067120826: 49 8B CC           mov         rcx,r12
  0000000067120829: E8 82 FB EF FF     call        00000000670203B0
  000000006712082E: 32 C0              xor         al,al
  0000000067120830: E9 19 01 00 00     jmp         000000006712094E
  0000000067120835: B9 50 00 00 00     mov         ecx,50h
  000000006712083A: E8 47 82 05 00     call        0000000067178A86
  000000006712083F: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000067120844: 48 85 C0           test        rax,rax
  0000000067120847: 74 3A              je          0000000067120883
  0000000067120849: 4C 89 7C 24 48     mov         qword ptr [rsp+48h],r15
  000000006712084E: 4C 89 64 24 40     mov         qword ptr [rsp+40h],r12
  0000000067120853: 48 89 6C 24 38     mov         qword ptr [rsp+38h],rbp
  0000000067120858: 89 74 24 30        mov         dword ptr [rsp+30h],esi
  000000006712085C: 48 83 CF FF        or          rdi,0FFFFFFFFFFFFFFFFh
  0000000067120860: 89 7C 24 28        mov         dword ptr [rsp+28h],edi
  0000000067120864: 4C 89 7C 24 20     mov         qword ptr [rsp+20h],r15
  0000000067120869: 4C 8B 4C 24 70     mov         r9,qword ptr [rsp+70h]
  000000006712086E: 44 0F B7 44 24 50  movzx       r8d,word ptr [rsp+50h]
  0000000067120874: 41 0F B7 D6        movzx       edx,r14w
  0000000067120878: 48 8B C8           mov         rcx,rax
  000000006712087B: E8 50 90 00 00     call        00000000671298D0
  0000000067120880: 4C 8B F8           mov         r15,rax
  0000000067120883: 49 8B D7           mov         rdx,r15
  0000000067120886: 48 8B 8C 24 98 01  mov         rcx,qword ptr [rsp+0000000000000198h]
                    00 00
  000000006712088E: E8 3D B7 FF FF     call        000000006711BFD0
  0000000067120893: E9 B0 00 00 00     jmp         0000000067120948
  0000000067120898: 4D 3B EB           cmp         r13,r11
  000000006712089B: 75 1D              jne         00000000671208BA
  000000006712089D: 4C 8B A4 24 98 01  mov         r12,qword ptr [rsp+0000000000000198h]
                    00 00
  00000000671208A5: 4D 8B C4           mov         r8,r12
  00000000671208A8: 49 8B 51 08        mov         rdx,qword ptr [r9+8]
  00000000671208AC: 48 8D 0D 4D 9E 11  lea         rcx,[000000006723A700h]
                    00
  00000000671208B3: E8 28 C6 EF FF     call        000000006701CEE0
  00000000671208B8: EB 08              jmp         00000000671208C2
  00000000671208BA: 4C 8B A4 24 98 01  mov         r12,qword ptr [rsp+0000000000000198h]
                    00 00
  00000000671208C2: 33 D2              xor         edx,edx
  00000000671208C4: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000671208C9: E8 22 67 F0 FF     call        0000000067026FF0
  00000000671208CE: 90                 nop
  00000000671208CF: B9 50 00 00 00     mov         ecx,50h
  00000000671208D4: E8 AD 81 05 00     call        0000000067178A86
  00000000671208D9: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  00000000671208E1: 48 85 C0           test        rax,rax
  00000000671208E4: 74 40              je          0000000067120926
  00000000671208E6: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000671208EB: 48 89 4C 24 48     mov         qword ptr [rsp+48h],rcx
  00000000671208F0: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000000671208F8: 48 89 4C 24 40     mov         qword ptr [rsp+40h],rcx
  00000000671208FD: 4C 89 7C 24 38     mov         qword ptr [rsp+38h],r15
  0000000067120902: 44 89 7C 24 30     mov         dword ptr [rsp+30h],r15d
  0000000067120907: 89 74 24 28        mov         dword ptr [rsp+28h],esi
  000000006712090B: 4C 89 7C 24 20     mov         qword ptr [rsp+20h],r15
  0000000067120910: 4C 8B CF           mov         r9,rdi
  0000000067120913: 44 0F B7 C5        movzx       r8d,bp
  0000000067120917: 41 0F B7 D6        movzx       edx,r14w
  000000006712091B: 48 8B C8           mov         rcx,rax
  000000006712091E: E8 AD 8F 00 00     call        00000000671298D0
  0000000067120923: 4C 8B F8           mov         r15,rax
  0000000067120926: 49 8B D7           mov         rdx,r15
  0000000067120929: 49 8B CC           mov         rcx,r12
  000000006712092C: E8 9F B6 FF FF     call        000000006711BFD0
  0000000067120931: 8B D3              mov         edx,ebx
  0000000067120933: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000067120938: E8 63 67 F0 FF     call        00000000670270A0
  000000006712093D: 90                 nop
  000000006712093E: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000067120943: E8 18 67 F0 FF     call        0000000067027060
  0000000067120948: B0 01              mov         al,1
  000000006712094A: EB 02              jmp         000000006712094E
  000000006712094C: 32 C0              xor         al,al
  000000006712094E: 48 8B 9C 24 A8 01  mov         rbx,qword ptr [rsp+00000000000001A8h]
                    00 00
  0000000067120956: 48 81 C4 50 01 00  add         rsp,150h
                    00
  000000006712095D: 41 5F              pop         r15
  000000006712095F: 41 5E              pop         r14
  0000000067120961: 41 5D              pop         r13
  0000000067120963: 41 5C              pop         r12
  0000000067120965: 5F                 pop         rdi
  0000000067120966: 5E                 pop         rsi
  0000000067120967: 5D                 pop         rbp
  0000000067120968: C3                 ret

  Summary

      1A7000 .text
