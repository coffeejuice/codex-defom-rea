Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B5340: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000000670B5344: 57                 push        rdi
  00000000670B5345: 48 83 EC 30        sub         rsp,30h
  00000000670B5349: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B5352: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000670B5357: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  00000000670B535C: 48 8B F9           mov         rdi,rcx
  00000000670B535F: 8B 41 18           mov         eax,dword ptr [rcx+18h]
  00000000670B5362: 85 C0              test        eax,eax
  00000000670B5364: 0F 85 A2 00 00 00  jne         00000000670B540C
  00000000670B536A: 48 8D 41 14        lea         rax,[rcx+14h]
  00000000670B536E: 48 8D 5C 24 48     lea         rbx,[rsp+48h]
  00000000670B5373: 39 10              cmp         dword ptr [rax],edx
  00000000670B5375: 48 0F 4D D8        cmovge      rbx,rax
  00000000670B5379: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670B537C: 44 8B 00           mov         r8d,dword ptr [rax]
  00000000670B537F: 41 83 F8 01        cmp         r8d,1
  00000000670B5383: 74 0B              je          00000000670B5390
  00000000670B5385: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670B5388: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B538B: E8 D0 C5 F7 FF     call        0000000067031960
  00000000670B5390: 48 8B 0F           mov         rcx,qword ptr [rdi]
  00000000670B5393: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  00000000670B5397: 48 8D 4C C1 18     lea         rcx,[rcx+rax*8+18h]
  00000000670B539C: 8B 13              mov         edx,dword ptr [rbx]
  00000000670B539E: E8 5D 98 F7 FF     call        000000006702EC00
  00000000670B53A3: 44 8B 5C 24 48     mov         r11d,dword ptr [rsp+48h]
  00000000670B53A8: 44 01 5F 18        add         dword ptr [rdi+18h],r11d
  00000000670B53AC: 44 89 5F 0C        mov         dword ptr [rdi+0Ch],r11d
  00000000670B53B0: 48 63 5F 10        movsxd      rbx,dword ptr [rdi+10h]
  00000000670B53B4: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B53B7: 8B 08              mov         ecx,dword ptr [rax]
  00000000670B53B9: 83 F9 01           cmp         ecx,1
  00000000670B53BC: 74 0E              je          00000000670B53CC
  00000000670B53BE: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B53C1: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B53C4: 48 8B CF           mov         rcx,rdi
  00000000670B53C7: E8 94 C5 F7 FF     call        0000000067031960
  00000000670B53CC: 48 8B 17           mov         rdx,qword ptr [rdi]
  00000000670B53CF: 48 63 4A 08        movsxd      rcx,dword ptr [rdx+8]
  00000000670B53D3: 48 03 CB           add         rcx,rbx
  00000000670B53D6: 48 8D 5C CA 18     lea         rbx,[rdx+rcx*8+18h]
  00000000670B53DB: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B53DE: 8B 08              mov         ecx,dword ptr [rax]
  00000000670B53E0: 83 F9 01           cmp         ecx,1
  00000000670B53E3: 75 0D              jne         00000000670B53F2
  00000000670B53E5: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670B53E8: 48 8D 41 18        lea         rax,[rcx+18h]
  00000000670B53EC: 48 39 41 10        cmp         qword ptr [rcx+10h],rax
  00000000670B53F0: 74 0E              je          00000000670B5400
  00000000670B53F2: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B53F5: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000670B53F8: 48 8B CB           mov         rcx,rbx
  00000000670B53FB: E8 10 83 F7 FF     call        000000006702D710
  00000000670B5400: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B5403: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000000670B5407: E9 F8 01 00 00     jmp         00000000670B5604
  00000000670B540C: 03 C2              add         eax,edx
  00000000670B540E: 89 41 18           mov         dword ptr [rcx+18h],eax
  00000000670B5411: 4C 8B 01           mov         r8,qword ptr [rcx]
  00000000670B5414: 48 63 59 10        movsxd      rbx,dword ptr [rcx+10h]
  00000000670B5418: 4D 63 48 08        movsxd      r9,dword ptr [r8+8]
  00000000670B541C: 4C 03 CB           add         r9,rbx
  00000000670B541F: 4B 8B 44 C8 18     mov         rax,qword ptr [r8+r9*8+18h]
  00000000670B5424: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000670B5427: 44 8B 57 0C        mov         r10d,dword ptr [rdi+0Ch]
  00000000670B542B: 41 8D 04 12        lea         eax,[r10+rdx]
  00000000670B542F: 3B C1              cmp         eax,ecx
  00000000670B5431: 7F 66              jg          00000000670B5499
  00000000670B5433: 41 8B 00           mov         eax,dword ptr [r8]
  00000000670B5436: 83 F8 01           cmp         eax,1
  00000000670B5439: 74 0E              je          00000000670B5449
  00000000670B543B: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B543E: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B5441: 48 8B CF           mov         rcx,rdi
  00000000670B5444: E8 17 C5 F7 FF     call        0000000067031960
  00000000670B5449: 48 8B 17           mov         rdx,qword ptr [rdi]
  00000000670B544C: 48 63 4A 08        movsxd      rcx,dword ptr [rdx+8]
  00000000670B5450: 48 03 CB           add         rcx,rbx
  00000000670B5453: 48 8D 5C CA 18     lea         rbx,[rdx+rcx*8+18h]
  00000000670B5458: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B545B: 8B 08              mov         ecx,dword ptr [rax]
  00000000670B545D: 83 F9 01           cmp         ecx,1
  00000000670B5460: 75 0D              jne         00000000670B546F
  00000000670B5462: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670B5465: 48 8D 41 18        lea         rax,[rcx+18h]
  00000000670B5469: 48 39 41 10        cmp         qword ptr [rcx+10h],rax
  00000000670B546D: 74 0E              je          00000000670B547D
  00000000670B546F: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B5472: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000670B5475: 48 8B CB           mov         rcx,rbx
  00000000670B5478: E8 93 82 F7 FF     call        000000006702D710
  00000000670B547D: 48 63 57 0C        movsxd      rdx,dword ptr [rdi+0Ch]
  00000000670B5481: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670B5484: 48 8B C2           mov         rax,rdx
  00000000670B5487: 48 03 41 10        add         rax,qword ptr [rcx+10h]
  00000000670B548B: 8B 4C 24 48        mov         ecx,dword ptr [rsp+48h]
  00000000670B548F: 03 D1              add         edx,ecx
  00000000670B5491: 89 57 0C           mov         dword ptr [rdi+0Ch],edx
  00000000670B5494: E9 6B 01 00 00     jmp         00000000670B5604
  00000000670B5499: 4B 8B 44 C8 18     mov         rax,qword ptr [r8+r9*8+18h]
  00000000670B549E: 8B 40 08           mov         eax,dword ptr [rax+8]
  00000000670B54A1: 99                 cdq
  00000000670B54A2: 2B C2              sub         eax,edx
  00000000670B54A4: D1 F8              sar         eax,1
  00000000670B54A6: 44 3B D0           cmp         r10d,eax
  00000000670B54A9: 7D 77              jge         00000000670B5522
  00000000670B54AB: 41 8B 00           mov         eax,dword ptr [r8]
  00000000670B54AE: 83 F8 01           cmp         eax,1
  00000000670B54B1: 74 0E              je          00000000670B54C1
  00000000670B54B3: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B54B6: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B54B9: 48 8B CF           mov         rcx,rdi
  00000000670B54BC: E8 9F C4 F7 FF     call        0000000067031960
  00000000670B54C1: 4C 8B 07           mov         r8,qword ptr [rdi]
  00000000670B54C4: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  00000000670B54C8: 03 57 0C           add         edx,dword ptr [rdi+0Ch]
  00000000670B54CB: 49 63 48 08        movsxd      rcx,dword ptr [r8+8]
  00000000670B54CF: 48 03 CB           add         rcx,rbx
  00000000670B54D2: 49 8D 4C C8 18     lea         rcx,[r8+rcx*8+18h]
  00000000670B54D7: E8 24 97 F7 FF     call        000000006702EC00
  00000000670B54DC: 48 63 77 10        movsxd      rsi,dword ptr [rdi+10h]
  00000000670B54E0: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B54E3: 8B 08              mov         ecx,dword ptr [rax]
  00000000670B54E5: 83 F9 01           cmp         ecx,1
  00000000670B54E8: 74 0E              je          00000000670B54F8
  00000000670B54EA: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B54ED: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B54F0: 48 8B CF           mov         rcx,rdi
  00000000670B54F3: E8 68 C4 F7 FF     call        0000000067031960
  00000000670B54F8: 48 8B 17           mov         rdx,qword ptr [rdi]
  00000000670B54FB: 48 63 5F 0C        movsxd      rbx,dword ptr [rdi+0Ch]
  00000000670B54FF: 48 63 4A 08        movsxd      rcx,dword ptr [rdx+8]
  00000000670B5503: 48 03 CE           add         rcx,rsi
  00000000670B5506: 48 8D 4C CA 18     lea         rcx,[rdx+rcx*8+18h]
  00000000670B550B: E8 00 ED F4 FF     call        0000000067004210
  00000000670B5510: 48 03 C3           add         rax,rbx
  00000000670B5513: 8B 4C 24 48        mov         ecx,dword ptr [rsp+48h]
  00000000670B5517: 8D 14 0B           lea         edx,[rbx+rcx]
  00000000670B551A: 89 57 0C           mov         dword ptr [rdi+0Ch],edx
  00000000670B551D: E9 E2 00 00 00     jmp         00000000670B5604
  00000000670B5522: 41 8B 00           mov         eax,dword ptr [r8]
  00000000670B5525: 83 F8 01           cmp         eax,1
  00000000670B5528: 74 0E              je          00000000670B5538
  00000000670B552A: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B552D: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B5530: 48 8B CF           mov         rcx,rdi
  00000000670B5533: E8 28 C4 F7 FF     call        0000000067031960
  00000000670B5538: 48 8B 17           mov         rdx,qword ptr [rdi]
  00000000670B553B: 48 63 4A 08        movsxd      rcx,dword ptr [rdx+8]
  00000000670B553F: 48 03 CB           add         rcx,rbx
  00000000670B5542: 48 8D 4C CA 18     lea         rcx,[rdx+rcx*8+18h]
  00000000670B5547: 8B 57 0C           mov         edx,dword ptr [rdi+0Ch]
  00000000670B554A: E8 B1 96 F7 FF     call        000000006702EC00
  00000000670B554F: 48 8D 1D AA B3 23  lea         rbx,[00000000672F0900h]
                    00
  00000000670B5556: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000670B555B: F0 83 05 9D B3 23  lock add    dword ptr [00000000672F0900h],1
                    00 01
  00000000670B5563: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670B5568: 48 8B CF           mov         rcx,rdi
  00000000670B556B: E8 10 C7 F7 FF     call        0000000067031C80
  00000000670B5570: 90                 nop
  00000000670B5571: F0 83 05 87 B3 23  lock add    dword ptr [00000000672F0900h],0FFFFFFFFh
                    00 FF
  00000000670B5579: 75 08              jne         00000000670B5583
  00000000670B557B: 48 8B CB           mov         rcx,rbx
  00000000670B557E: E8 2D AE F6 FF     call        00000000670203B0
  00000000670B5583: FF 47 10           inc         dword ptr [rdi+10h]
  00000000670B5586: 48 63 77 10        movsxd      rsi,dword ptr [rdi+10h]
  00000000670B558A: 48 8D 4F 14        lea         rcx,[rdi+14h]
  00000000670B558E: 48 8D 5C 24 48     lea         rbx,[rsp+48h]
  00000000670B5593: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  00000000670B5597: 39 01              cmp         dword ptr [rcx],eax
  00000000670B5599: 48 0F 4D D9        cmovge      rbx,rcx
  00000000670B559D: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B55A0: 8B 08              mov         ecx,dword ptr [rax]
  00000000670B55A2: 83 F9 01           cmp         ecx,1
  00000000670B55A5: 74 0E              je          00000000670B55B5
  00000000670B55A7: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B55AA: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B55AD: 48 8B CF           mov         rcx,rdi
  00000000670B55B0: E8 AB C3 F7 FF     call        0000000067031960
  00000000670B55B5: 4C 8B 07           mov         r8,qword ptr [rdi]
  00000000670B55B8: 49 63 48 08        movsxd      rcx,dword ptr [r8+8]
  00000000670B55BC: 48 03 CE           add         rcx,rsi
  00000000670B55BF: 49 8D 4C C8 18     lea         rcx,[r8+rcx*8+18h]
  00000000670B55C4: 8B 13              mov         edx,dword ptr [rbx]
  00000000670B55C6: E8 35 96 F7 FF     call        000000006702EC00
  00000000670B55CB: 44 8B 5C 24 48     mov         r11d,dword ptr [rsp+48h]
  00000000670B55D0: 44 89 5F 0C        mov         dword ptr [rdi+0Ch],r11d
  00000000670B55D4: 48 63 5F 10        movsxd      rbx,dword ptr [rdi+10h]
  00000000670B55D8: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B55DB: 8B 08              mov         ecx,dword ptr [rax]
  00000000670B55DD: 83 F9 01           cmp         ecx,1
  00000000670B55E0: 74 0E              je          00000000670B55F0
  00000000670B55E2: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B55E5: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B55E8: 48 8B CF           mov         rcx,rdi
  00000000670B55EB: E8 70 C3 F7 FF     call        0000000067031960
  00000000670B55F0: 48 8B 17           mov         rdx,qword ptr [rdi]
  00000000670B55F3: 48 63 4A 08        movsxd      rcx,dword ptr [rdx+8]
  00000000670B55F7: 48 03 CB           add         rcx,rbx
  00000000670B55FA: 48 8D 4C CA 18     lea         rcx,[rdx+rcx*8+18h]
  00000000670B55FF: E8 0C EC F4 FF     call        0000000067004210
  00000000670B5604: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000670B5609: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  00000000670B560E: 48 83 C4 30        add         rsp,30h
  00000000670B5612: 5F                 pop         rdi
  00000000670B5613: C3                 ret

  Summary

      1A7000 .text
