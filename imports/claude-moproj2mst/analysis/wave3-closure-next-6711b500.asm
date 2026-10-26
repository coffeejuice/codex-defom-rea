Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711B500: 40 53              push        rbx
  000000006711B502: 55                 push        rbp
  000000006711B503: 56                 push        rsi
  000000006711B504: 57                 push        rdi
  000000006711B505: 41 54              push        r12
  000000006711B507: 41 55              push        r13
  000000006711B509: 41 56              push        r14
  000000006711B50B: 41 57              push        r15
  000000006711B50D: 48 81 EC 68 08 00  sub         rsp,868h
                    00
  000000006711B514: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711B51D: 8B FA              mov         edi,edx
  000000006711B51F: 4C 8B F1           mov         r14,rcx
  000000006711B522: B8 43 00 00 00     mov         eax,43h
  000000006711B527: 83 FA 46           cmp         edx,46h
  000000006711B52A: 0F 44 F8           cmove       edi,eax
  000000006711B52D: 89 BC 24 B8 08 00  mov         dword ptr [rsp+00000000000008B8h],edi
                    00
  000000006711B534: 48 85 C9           test        rcx,rcx
  000000006711B537: 74 0A              je          000000006711B543
  000000006711B539: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006711B53D: 4C 8B 78 40        mov         r15,qword ptr [rax+40h]
  000000006711B541: EB 08              jmp         000000006711B54B
  000000006711B543: E8 58 F7 F0 FF     call        000000006702ACA0
  000000006711B548: 4C 8B F8           mov         r15,rax
  000000006711B54B: 49 8D 5F 48        lea         rbx,[r15+48h]
  000000006711B54F: 33 D2              xor         edx,edx
  000000006711B551: 8D 72 01           lea         esi,[rdx+1]
  000000006711B554: 48 85 DB           test        rbx,rbx
  000000006711B557: 74 41              je          000000006711B59A
  000000006711B559: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006711B55C: 40 84 71 04        test        byte ptr [rcx+4],sil
  000000006711B560: 74 0C              je          000000006711B56E
  000000006711B562: 48 8B CB           mov         rcx,rbx
  000000006711B565: E8 46 A8 F0 FF     call        0000000067025DB0
  000000006711B56A: 33 D2              xor         edx,edx
  000000006711B56C: EB 17              jmp         000000006711B585
  000000006711B56E: 33 C0              xor         eax,eax
  000000006711B570: F0 0F B1 31        lock cmpxchg dword ptr [rcx],esi
  000000006711B574: 0F 94 C1           sete        cl
  000000006711B577: 84 C9              test        cl,cl
  000000006711B579: 75 0A              jne         000000006711B585
  000000006711B57B: 48 8B CB           mov         rcx,rbx
  000000006711B57E: E8 2D A6 F0 FF     call        0000000067025BB0
  000000006711B583: 33 D2              xor         edx,edx
  000000006711B585: 48 0B DE           or          rbx,rsi
  000000006711B588: 48 89 9C 24 C0 08  mov         qword ptr [rsp+00000000000008C0h],rbx
                    00 00
  000000006711B590: 48 89 9C 24 B0 08  mov         qword ptr [rsp+00000000000008B0h],rbx
                    00 00
  000000006711B598: EB 13              jmp         000000006711B5AD
  000000006711B59A: 48 8B DA           mov         rbx,rdx
  000000006711B59D: 48 89 94 24 C0 08  mov         qword ptr [rsp+00000000000008C0h],rdx
                    00 00
  000000006711B5A5: 48 89 94 24 B0 08  mov         qword ptr [rsp+00000000000008B0h],rdx
                    00 00
  000000006711B5AD: 4D 85 F6           test        r14,r14
  000000006711B5B0: 74 52              je          000000006711B604
  000000006711B5B2: 49 8B 46 08        mov         rax,qword ptr [r14+8]
  000000006711B5B6: 83 78 24 00        cmp         dword ptr [rax+24h],0
  000000006711B5BA: 75 48              jne         000000006711B604
  000000006711B5BC: F6 C3 01           test        bl,1
  000000006711B5BF: 0F 84 DD 02 00 00  je          000000006711B8A2
  000000006711B5C5: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711B5C9: 48 89 9C 24 B0 08  mov         qword ptr [rsp+00000000000008B0h],rbx
                    00 00
  000000006711B5D1: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711B5D5: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006711B5D8: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006711B5DC: 74 0D              je          000000006711B5EB
  000000006711B5DE: 48 8B CB           mov         rcx,rbx
  000000006711B5E1: E8 4A A5 F0 FF     call        0000000067025B30
  000000006711B5E6: E9 B7 02 00 00     jmp         000000006711B8A2
  000000006711B5EB: 8B C6              mov         eax,esi
  000000006711B5ED: F0 0F B1 11        lock cmpxchg dword ptr [rcx],edx
  000000006711B5F1: 0F 84 AB 02 00 00  je          000000006711B8A2
  000000006711B5F7: 48 8B CB           mov         rcx,rbx
  000000006711B5FA: E8 71 A5 F0 FF     call        0000000067025B70
  000000006711B5FF: E9 9E 02 00 00     jmp         000000006711B8A2
  000000006711B604: 89 54 24 44        mov         dword ptr [rsp+44h],edx
  000000006711B608: 48 8D 44 24 50     lea         rax,[rsp+50h]
  000000006711B60D: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000006711B612: C7 44 24 40 00 01  mov         dword ptr [rsp+40h],100h
                    00 00
  000000006711B61A: 49 8B 47 30        mov         rax,qword ptr [r15+30h]
  000000006711B61E: 8B 48 0C           mov         ecx,dword ptr [rax+0Ch]
  000000006711B621: 2B 48 08           sub         ecx,dword ptr [rax+8]
  000000006711B624: 8B EA              mov         ebp,edx
  000000006711B626: 44 8B E2           mov         r12d,edx
  000000006711B629: 4C 8B EA           mov         r13,rdx
  000000006711B62C: 48 63 D7           movsxd      rdx,edi
  000000006711B62F: 48 63 C1           movsxd      rax,ecx
  000000006711B632: 48 89 84 24 C8 08  mov         qword ptr [rsp+00000000000008C8h],rax
                    00 00
  000000006711B63A: 85 C9              test        ecx,ecx
  000000006711B63C: 0F 8E EC 00 00 00  jle         000000006711B72E
  000000006711B642: 48 63 9C 24 B8 08  movsxd      rbx,dword ptr [rsp+00000000000008B8h]
                    00 00
  000000006711B64A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006711B650: 49 8B 4F 30        mov         rcx,qword ptr [r15+30h]
  000000006711B654: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  000000006711B658: 49 03 C5           add         rax,r13
  000000006711B65B: 48 8B 7C C1 18     mov         rdi,qword ptr [rcx+rax*8+18h]
  000000006711B660: 4D 85 F6           test        r14,r14
  000000006711B663: 74 09              je          000000006711B66E
  000000006711B665: 4C 39 37           cmp         qword ptr [rdi],r14
  000000006711B668: 0F 85 84 00 00 00  jne         000000006711B6F2
  000000006711B66E: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006711B672: 48 85 C0           test        rax,rax
  000000006711B675: 74 7B              je          000000006711B6F2
  000000006711B677: 48 85 D2           test        rdx,rdx
  000000006711B67A: 74 08              je          000000006711B684
  000000006711B67C: 0F B7 40 10        movzx       eax,word ptr [rax+10h]
  000000006711B680: 3B C3              cmp         eax,ebx
  000000006711B682: 75 6E              jne         000000006711B6F2
  000000006711B684: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006711B687: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006711B68B: FF 49 24           dec         dword ptr [rcx+24h]
  000000006711B68E: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006711B692: 66 83 78 10 43     cmp         word ptr [rax+10h],43h
  000000006711B697: 75 10              jne         000000006711B6A9
  000000006711B699: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006711B69C: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006711B6A0: 48 83 C1 68        add         rcx,68h
  000000006711B6A4: E8 57 EE FF FF     call        000000006711A500
  000000006711B6A9: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006711B6AD: B9 FE FF 00 00     mov         ecx,0FFFEh
  000000006711B6B2: 66 21 48 12        and         word ptr [rax+12h],cx
  000000006711B6B6: 8B 4C 24 44        mov         ecx,dword ptr [rsp+44h]
  000000006711B6BA: 3B 4C 24 40        cmp         ecx,dword ptr [rsp+40h]
  000000006711B6BE: 75 14              jne         000000006711B6D4
  000000006711B6C0: 44 8D 04 09        lea         r8d,[rcx+rcx]
  000000006711B6C4: 8B D1              mov         edx,ecx
  000000006711B6C6: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006711B6CB: E8 80 D2 FF FF     call        0000000067118950
  000000006711B6D0: 8B 4C 24 44        mov         ecx,dword ptr [rsp+44h]
  000000006711B6D4: 48 63 D1           movsxd      rdx,ecx
  000000006711B6D7: FF C1              inc         ecx
  000000006711B6D9: 89 4C 24 44        mov         dword ptr [rsp+44h],ecx
  000000006711B6DD: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006711B6E1: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000006711B6E6: 48 89 0C D0        mov         qword ptr [rax+rdx*8],rcx
  000000006711B6EA: 33 C0              xor         eax,eax
  000000006711B6EC: 48 89 47 08        mov         qword ptr [rdi+8],rax
  000000006711B6F0: EB 1D              jmp         000000006711B70F
  000000006711B6F2: 41 83 7F 38 00     cmp         dword ptr [r15+38h],0
  000000006711B6F7: 75 16              jne         000000006711B70F
  000000006711B6F9: 44 3B E5           cmp         r12d,ebp
  000000006711B6FC: 74 0F              je          000000006711B70D
  000000006711B6FE: 44 8B C5           mov         r8d,ebp
  000000006711B701: 41 8B D4           mov         edx,r12d
  000000006711B704: 49 8D 4F 30        lea         rcx,[r15+30h]
  000000006711B708: E8 D3 EE FF FF     call        000000006711A5E0
  000000006711B70D: FF C5              inc         ebp
  000000006711B70F: 41 FF C4           inc         r12d
  000000006711B712: 49 FF C5           inc         r13
  000000006711B715: 4C 3B AC 24 C8 08  cmp         r13,qword ptr [rsp+00000000000008C8h]
                    00 00
  000000006711B71D: 48 8B D3           mov         rdx,rbx
  000000006711B720: 0F 8C 2A FF FF FF  jl          000000006711B650
  000000006711B726: 48 8B 9C 24 C0 08  mov         rbx,qword ptr [rsp+00000000000008C0h]
                    00 00
  000000006711B72E: 41 83 7F 38 00     cmp         dword ptr [r15+38h],0
  000000006711B733: 0F 85 81 00 00 00  jne         000000006711B7BA
  000000006711B739: 49 8B 47 30        mov         rax,qword ptr [r15+30h]
  000000006711B73D: 8B 08              mov         ecx,dword ptr [rax]
  000000006711B73F: 83 F9 01           cmp         ecx,1
  000000006711B742: 74 10              je          000000006711B754
  000000006711B744: 49 8B 47 30        mov         rax,qword ptr [r15+30h]
  000000006711B748: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006711B74B: 49 8D 4F 30        lea         rcx,[r15+30h]
  000000006711B74F: E8 6C D7 FF FF     call        0000000067118EC0
  000000006711B754: 49 8B 4F 30        mov         rcx,qword ptr [r15+30h]
  000000006711B758: 48 63 41 0C        movsxd      rax,dword ptr [rcx+0Ch]
  000000006711B75C: 48 8D 4C C1 18     lea         rcx,[rcx+rax*8+18h]
  000000006711B761: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  000000006711B766: 49 8B 47 30        mov         rax,qword ptr [r15+30h]
  000000006711B76A: 8B 08              mov         ecx,dword ptr [rax]
  000000006711B76C: 83 F9 01           cmp         ecx,1
  000000006711B76F: 74 10              je          000000006711B781
  000000006711B771: 49 8B 47 30        mov         rax,qword ptr [r15+30h]
  000000006711B775: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006711B778: 49 8D 4F 30        lea         rcx,[r15+30h]
  000000006711B77C: E8 3F D7 FF FF     call        0000000067118EC0
  000000006711B781: 49 8B 57 30        mov         rdx,qword ptr [r15+30h]
  000000006711B785: 48 63 42 08        movsxd      rax,dword ptr [rdx+8]
  000000006711B789: 48 8D 44 C2 18     lea         rax,[rdx+rax*8+18h]
  000000006711B78E: 48 63 D5           movsxd      rdx,ebp
  000000006711B791: 4C 8D 04 D0        lea         r8,[rax+rdx*8]
  000000006711B795: 4C 89 44 24 28     mov         qword ptr [rsp+28h],r8
  000000006711B79A: 4C 8D 4C 24 20     lea         r9,[rsp+20h]
  000000006711B79F: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  000000006711B7A4: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006711B7A9: 49 8D 4F 30        lea         rcx,[r15+30h]
  000000006711B7AD: E8 DE F3 FF FF     call        000000006711AB90
  000000006711B7B2: 48 8B 9C 24 B0 08  mov         rbx,qword ptr [rsp+00000000000008B0h]
                    00 00
  000000006711B7BA: F6 C3 01           test        bl,1
  000000006711B7BD: 74 52              je          000000006711B811
  000000006711B7BF: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711B7C3: 48 89 9C 24 B0 08  mov         qword ptr [rsp+00000000000008B0h],rbx
                    00 00
  000000006711B7CB: 48 8B CB           mov         rcx,rbx
  000000006711B7CE: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006711B7D2: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006711B7D5: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006711B7D9: 74 0C              je          000000006711B7E7
  000000006711B7DB: E8 50 A3 F0 FF     call        0000000067025B30
  000000006711B7E0: BD 01 00 00 00     mov         ebp,1
  000000006711B7E5: EB 2F              jmp         000000006711B816
  000000006711B7E7: BD 01 00 00 00     mov         ebp,1
  000000006711B7EC: 8B C5              mov         eax,ebp
  000000006711B7EE: 45 33 C0           xor         r8d,r8d
  000000006711B7F1: F0 44 0F B1 02     lock cmpxchg dword ptr [rdx],r8d
  000000006711B7F6: 74 0F              je          000000006711B807
  000000006711B7F8: E8 73 A3 F0 FF     call        0000000067025B70
  000000006711B7FD: 48 8B 9C 24 B0 08  mov         rbx,qword ptr [rsp+00000000000008B0h]
                    00 00
  000000006711B805: EB 0F              jmp         000000006711B816
  000000006711B807: 48 8B 9C 24 B0 08  mov         rbx,qword ptr [rsp+00000000000008B0h]
                    00 00
  000000006711B80F: EB 05              jmp         000000006711B816
  000000006711B811: BD 01 00 00 00     mov         ebp,1
  000000006711B816: 33 C0              xor         eax,eax
  000000006711B818: 8B F8              mov         edi,eax
  000000006711B81A: 39 44 24 44        cmp         dword ptr [rsp+44h],eax
  000000006711B81E: 7E 31              jle         000000006711B851
  000000006711B820: 48 8B F0           mov         rsi,rax
  000000006711B823: 66 66 66 66 66 0F  nop         word ptr [rax+rax+0000000000000000h]
                    1F 84 00 00 00 00
                    00
  000000006711B830: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000006711B835: 48 8B 0C 06        mov         rcx,qword ptr [rsi+rax]
  000000006711B839: 48 85 C9           test        rcx,rcx
  000000006711B83C: 74 07              je          000000006711B845
  000000006711B83E: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006711B841: 8B D5              mov         edx,ebp
  000000006711B843: FF 10              call        qword ptr [rax]
  000000006711B845: FF C7              inc         edi
  000000006711B847: 48 83 C6 08        add         rsi,8
  000000006711B84B: 3B 7C 24 44        cmp         edi,dword ptr [rsp+44h]
  000000006711B84F: 7C DF              jl          000000006711B830
  000000006711B851: 48 8D 44 24 50     lea         rax,[rsp+50h]
  000000006711B856: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006711B85B: 48 3B C8           cmp         rcx,rax
  000000006711B85E: 74 06              je          000000006711B866
  000000006711B860: E8 4B 4B F0 FF     call        00000000670203B0
  000000006711B865: 90                 nop
  000000006711B866: F6 C3 01           test        bl,1
  000000006711B869: 74 37              je          000000006711B8A2
  000000006711B86B: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711B86F: 48 89 9C 24 B0 08  mov         qword ptr [rsp+00000000000008B0h],rbx
                    00 00
  000000006711B877: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006711B87B: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006711B87E: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006711B882: 74 0A              je          000000006711B88E
  000000006711B884: 48 8B CB           mov         rcx,rbx
  000000006711B887: E8 A4 A2 F0 FF     call        0000000067025B30
  000000006711B88C: EB 14              jmp         000000006711B8A2
  000000006711B88E: 8B C5              mov         eax,ebp
  000000006711B890: 45 33 C0           xor         r8d,r8d
  000000006711B893: F0 44 0F B1 01     lock cmpxchg dword ptr [rcx],r8d
  000000006711B898: 74 08              je          000000006711B8A2
  000000006711B89A: 48 8B CB           mov         rcx,rbx
  000000006711B89D: E8 CE A2 F0 FF     call        0000000067025B70
  000000006711B8A2: 48 81 C4 68 08 00  add         rsp,868h
                    00
  000000006711B8A9: 41 5F              pop         r15
  000000006711B8AB: 41 5E              pop         r14
  000000006711B8AD: 41 5D              pop         r13
  000000006711B8AF: 41 5C              pop         r12
  000000006711B8B1: 5F                 pop         rdi
  000000006711B8B2: 5E                 pop         rsi
  000000006711B8B3: 5D                 pop         rbp
  000000006711B8B4: 5B                 pop         rbx
  000000006711B8B5: C3                 ret

  Summary

      1A7000 .text
