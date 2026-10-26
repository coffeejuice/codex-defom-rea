Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000655028C0: 4D 85 C0           test        r8,r8
  00000000655028C3: 0F 84 17 02 00 00  je          0000000065502AE0
  00000000655028C9: 48 8B C4           mov         rax,rsp
  00000000655028CC: 4C 89 40 18        mov         qword ptr [rax+18h],r8
  00000000655028D0: 56                 push        rsi
  00000000655028D1: 57                 push        rdi
  00000000655028D2: 41 54              push        r12
  00000000655028D4: 41 55              push        r13
  00000000655028D6: 41 56              push        r14
  00000000655028D8: 48 83 EC 50        sub         rsp,50h
  00000000655028DC: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000655028E5: 48 89 58 08        mov         qword ptr [rax+8],rbx
  00000000655028E9: 48 89 68 10        mov         qword ptr [rax+10h],rbp
  00000000655028ED: 4D 8B E0           mov         r12,r8
  00000000655028F0: 8B DA              mov         ebx,edx
  00000000655028F2: 4C 8B F1           mov         r14,rcx
  00000000655028F5: FF 15 6D 3F 19 00  call        qword ptr [0000000065696868h]
  00000000655028FB: 48 8B D0           mov         rdx,rax
  00000000655028FE: 48 8D 0D 03 24 23  lea         rcx,[0000000065734D08h]
                    00
  0000000065502905: FF 15 FD 40 19 00  call        qword ptr [0000000065696A08h]
  000000006550290B: 49 89 44 24 18     mov         qword ptr [r12+18h],rax
  0000000065502910: 48 83 F8 00        cmp         rax,0
  0000000065502914: 0F 84 30 01 00 00  je          0000000065502A4A
  000000006550291A: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006550291E: 80 B8 2C 04 00 00  cmp         byte ptr [rax+000000000000042Ch],0
                    00
  0000000065502925: 0F 84 1F 01 00 00  je          0000000065502A4A
  000000006550292B: 8B B0 28 04 00 00  mov         esi,dword ptr [rax+0000000000000428h]
  0000000065502931: BA 01 00 00 00     mov         edx,1
  0000000065502936: 49 8B 4E 10        mov         rcx,qword ptr [r14+10h]
  000000006550293A: FF 15 30 41 19 00  call        qword ptr [0000000065696A70h]
  0000000065502940: 84 C0              test        al,al
  0000000065502942: 74 10              je          0000000065502954
  0000000065502944: 49 8B 46 10        mov         rax,qword ptr [r14+10h]
  0000000065502948: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006550294B: 49 8D 4E 10        lea         rcx,[r14+10h]
  000000006550294F: E8 1C CB 00 00     call        000000006550F470
  0000000065502954: 49 8D 4E 10        lea         rcx,[r14+10h]
  0000000065502958: FF 15 F2 3F 19 00  call        qword ptr [0000000065696950h]
  000000006550295E: 48 8B D8           mov         rbx,rax
  0000000065502961: BA 01 00 00 00     mov         edx,1
  0000000065502966: 49 8B 4E 10        mov         rcx,qword ptr [r14+10h]
  000000006550296A: FF 15 00 41 19 00  call        qword ptr [0000000065696A70h]
  0000000065502970: 84 C0              test        al,al
  0000000065502972: 74 10              je          0000000065502984
  0000000065502974: 49 8B 46 10        mov         rax,qword ptr [r14+10h]
  0000000065502978: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006550297B: 49 8D 4E 10        lea         rcx,[r14+10h]
  000000006550297F: E8 EC CA 00 00     call        000000006550F470
  0000000065502984: 49 8D 4E 10        lea         rcx,[r14+10h]
  0000000065502988: FF 15 BA 3F 19 00  call        qword ptr [0000000065696948h]
  000000006550298E: 48 8B F8           mov         rdi,rax
  0000000065502991: 85 F6              test        esi,esi
  0000000065502993: 75 3B              jne         00000000655029D0
  0000000065502995: 48 2B D8           sub         rbx,rax
  0000000065502998: 48 C1 FB 03        sar         rbx,3
  000000006550299C: 85 DB              test        ebx,ebx
  000000006550299E: 7E 6F              jle         0000000065502A0F
  00000000655029A0: 8B EB              mov         ebp,ebx
  00000000655029A2: D1 FD              sar         ebp,1
  00000000655029A4: 48 63 C5           movsxd      rax,ebp
  00000000655029A7: 48 8D 34 C7        lea         rsi,[rdi+rax*8]
  00000000655029AB: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000655029AE: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000655029B1: 49 8B D4           mov         rdx,r12
  00000000655029B4: FF 50 28           call        qword ptr [rax+28h]
  00000000655029B7: 84 C0              test        al,al
  00000000655029B9: 74 0D              je          00000000655029C8
  00000000655029BB: 48 8D 7E 08        lea         rdi,[rsi+8]
  00000000655029BF: 83 C8 FF           or          eax,0FFFFFFFFh
  00000000655029C2: 2B C5              sub         eax,ebp
  00000000655029C4: 03 D8              add         ebx,eax
  00000000655029C6: EB 02              jmp         00000000655029CA
  00000000655029C8: 8B DD              mov         ebx,ebp
  00000000655029CA: 85 DB              test        ebx,ebx
  00000000655029CC: 7F D2              jg          00000000655029A0
  00000000655029CE: EB 3F              jmp         0000000065502A0F
  00000000655029D0: 48 2B D8           sub         rbx,rax
  00000000655029D3: 48 C1 FB 03        sar         rbx,3
  00000000655029D7: 85 DB              test        ebx,ebx
  00000000655029D9: 7E 34              jle         0000000065502A0F
  00000000655029DB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  00000000655029E0: 8B EB              mov         ebp,ebx
  00000000655029E2: D1 FD              sar         ebp,1
  00000000655029E4: 48 63 C5           movsxd      rax,ebp
  00000000655029E7: 48 8D 34 C7        lea         rsi,[rdi+rax*8]
  00000000655029EB: 49 8B 04 24        mov         rax,qword ptr [r12]
  00000000655029EF: 48 8B 16           mov         rdx,qword ptr [rsi]
  00000000655029F2: 49 8B CC           mov         rcx,r12
  00000000655029F5: FF 50 28           call        qword ptr [rax+28h]
  00000000655029F8: 84 C0              test        al,al
  00000000655029FA: 74 0D              je          0000000065502A09
  00000000655029FC: 48 8D 7E 08        lea         rdi,[rsi+8]
  0000000065502A00: 83 C8 FF           or          eax,0FFFFFFFFh
  0000000065502A03: 2B C5              sub         eax,ebp
  0000000065502A05: 03 D8              add         ebx,eax
  0000000065502A07: EB 02              jmp         0000000065502A0B
  0000000065502A09: 8B DD              mov         ebx,ebp
  0000000065502A0B: 85 DB              test        ebx,ebx
  0000000065502A0D: 7F D1              jg          00000000655029E0
  0000000065502A0F: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00
  0000000065502A1A: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000065502A1F: 49 8D 4E 10        lea         rcx,[r14+10h]
  0000000065502A23: E8 E8 FC FF FF     call        0000000065502710
  0000000065502A28: 48 2B 38           sub         rdi,qword ptr [rax]
  0000000065502A2B: 48 C1 FF 03        sar         rdi,3
  0000000065502A2F: 89 7C 24 20        mov         dword ptr [rsp+20h],edi
  0000000065502A33: 48 8D 84 24 98 00  lea         rax,[rsp+0000000000000098h]
                    00 00
  0000000065502A3B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000065502A40: 85 FF              test        edi,edi
  0000000065502A42: 48 0F 49 C1        cmovns      rax,rcx
  0000000065502A46: 8B 18              mov         ebx,dword ptr [rax]
  0000000065502A48: EB 22              jmp         0000000065502A6C
  0000000065502A4A: 85 DB              test        ebx,ebx
  0000000065502A4C: 79 04              jns         0000000065502A52
  0000000065502A4E: 33 DB              xor         ebx,ebx
  0000000065502A50: EB 1A              jmp         0000000065502A6C
  0000000065502A52: 49 8D 4E 10        lea         rcx,[r14+10h]
  0000000065502A56: FF 15 5C 3E 19 00  call        qword ptr [00000000656968B8h]
  0000000065502A5C: 3B D8              cmp         ebx,eax
  0000000065502A5E: 7E 0C              jle         0000000065502A6C
  0000000065502A60: 49 8D 4E 10        lea         rcx,[r14+10h]
  0000000065502A64: FF 15 4E 3E 19 00  call        qword ptr [00000000656968B8h]
  0000000065502A6A: 8B D8              mov         ebx,eax
  0000000065502A6C: C7 44 24 30 FF FF  mov         dword ptr [rsp+30h],0FFFFFFFFh
                    FF FF
  0000000065502A74: C7 44 24 34 FF FF  mov         dword ptr [rsp+34h],0FFFFFFFFh
                    FF FF
  0000000065502A7C: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000065502A85: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000065502A8E: 44 8B CB           mov         r9d,ebx
  0000000065502A91: 44 8B C3           mov         r8d,ebx
  0000000065502A94: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000065502A99: 49 8B CE           mov         rcx,r14
  0000000065502A9C: FF 15 26 60 19 00  call        qword ptr [0000000065698AC8h]
  0000000065502AA2: 90                 nop
  0000000065502AA3: 49 8D 4E 10        lea         rcx,[r14+10h]
  0000000065502AA7: 4C 8D 84 24 90 00  lea         r8,[rsp+0000000000000090h]
                    00 00
  0000000065502AAF: 8B D3              mov         edx,ebx
  0000000065502AB1: E8 CA E4 FF FF     call        0000000065500F80
  0000000065502AB6: 4D 8B 5C 24 20     mov         r11,qword ptr [r12+20h]
  0000000065502ABB: 41 89 5B 10        mov         dword ptr [r11+10h],ebx
  0000000065502ABF: 49 8B CE           mov         rcx,r14
  0000000065502AC2: FF 15 F8 5F 19 00  call        qword ptr [0000000065698AC0h]
  0000000065502AC8: 4C 8D 5C 24 50     lea         r11,[rsp+50h]
  0000000065502ACD: 49 8B 5B 30        mov         rbx,qword ptr [r11+30h]
  0000000065502AD1: 49 8B 6B 38        mov         rbp,qword ptr [r11+38h]
  0000000065502AD5: 49 8B E3           mov         rsp,r11
  0000000065502AD8: 41 5E              pop         r14
  0000000065502ADA: 41 5D              pop         r13
  0000000065502ADC: 41 5C              pop         r12
  0000000065502ADE: 5F                 pop         rdi
  0000000065502ADF: 5E                 pop         rsi
  0000000065502AE0: F3 C3              rep ret

  Summary

      695000 .text
