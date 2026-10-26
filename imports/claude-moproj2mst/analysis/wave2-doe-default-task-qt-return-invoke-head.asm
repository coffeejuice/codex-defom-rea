
Dump of file evidence\original-binaries\QtCore4_64.dll

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

  Summary

      1A7000 .text
