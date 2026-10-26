Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712D3F0: 48 8B C4           mov         rax,rsp
  000000006712D3F3: 56                 push        rsi
  000000006712D3F4: 57                 push        rdi
  000000006712D3F5: 41 54              push        r12
  000000006712D3F7: 41 55              push        r13
  000000006712D3F9: 41 56              push        r14
  000000006712D3FB: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  000000006712D402: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006712D40B: 48 89 58 18        mov         qword ptr [rax+18h],rbx
  000000006712D40F: 48 89 68 20        mov         qword ptr [rax+20h],rbp
  000000006712D413: 4C 8B E2           mov         r12,rdx
  000000006712D416: 48 8B D9           mov         rbx,rcx
  000000006712D419: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  000000006712D41D: 48 89 78 08        mov         qword ptr [rax+8],rdi
  000000006712D421: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  000000006712D425: 48 3B D0           cmp         rdx,rax
  000000006712D428: 0F 84 6E 02 00 00  je          000000006712D69C
  000000006712D42E: 33 F6              xor         esi,esi
  000000006712D430: 41 BE FD FF 00 00  mov         r14d,0FFFDh
  000000006712D436: 83 CD FF           or          ebp,0FFFFFFFFh
  000000006712D439: 48 85 C0           test        rax,rax
  000000006712D43C: 0F 84 D7 00 00 00  je          000000006712D519
  000000006712D442: 4C 8B 68 08        mov         r13,qword ptr [rax+8]
  000000006712D446: 45 8B 45 20        mov         r8d,dword ptr [r13+20h]
  000000006712D44A: 41 C1 E8 03        shr         r8d,3
  000000006712D44E: 41 83 E0 01        and         r8d,1
  000000006712D452: 74 10              je          000000006712D464
  000000006712D454: F6 41 20 08        test        byte ptr [rcx+20h],8
  000000006712D458: 74 0A              je          000000006712D464
  000000006712D45A: 49 39 7D 78        cmp         qword ptr [r13+78h],rdi
  000000006712D45E: 0F 84 B5 00 00 00  je          000000006712D519
  000000006712D464: 4D 8B 55 18        mov         r10,qword ptr [r13+18h]
  000000006712D468: 49 63 4A 0C        movsxd      rcx,dword ptr [r10+0Ch]
  000000006712D46C: 4D 63 4A 08        movsxd      r9,dword ptr [r10+8]
  000000006712D470: 8B C1              mov         eax,ecx
  000000006712D472: 41 2B C1           sub         eax,r9d
  000000006712D475: 85 C0              test        eax,eax
  000000006712D477: 7E 33              jle         000000006712D4AC
  000000006712D479: 49 8D 54 CA 18     lea         rdx,[r10+rcx*8+18h]
  000000006712D47E: 4B 8D 4C CA 18     lea         rcx,[r10+r9*8+18h]
  000000006712D483: 48 3B CA           cmp         rcx,rdx
  000000006712D486: 74 24              je          000000006712D4AC
  000000006712D488: 48 39 39           cmp         qword ptr [rcx],rdi
  000000006712D48B: 74 0B              je          000000006712D498
  000000006712D48D: 48 83 C1 08        add         rcx,8
  000000006712D491: 48 3B CA           cmp         rcx,rdx
  000000006712D494: 75 F2              jne         000000006712D488
  000000006712D496: EB 14              jmp         000000006712D4AC
  000000006712D498: 4A 8D 04 CD 18 00  lea         rax,[r9*8+0000000000000018h]
                    00 00
  000000006712D4A0: 48 2B C8           sub         rcx,rax
  000000006712D4A3: 49 2B CA           sub         rcx,r10
  000000006712D4A6: 48 C1 F9 03        sar         rcx,3
  000000006712D4AA: EB 02              jmp         000000006712D4AE
  000000006712D4AC: 8B CD              mov         ecx,ebp
  000000006712D4AE: 45 85 C0           test        r8d,r8d
  000000006712D4B1: 74 10              je          000000006712D4C3
  000000006712D4B3: 8B D1              mov         edx,ecx
  000000006712D4B5: 49 8D 4D 18        lea         rcx,[r13+18h]
  000000006712D4B9: E8 92 F1 FF FF     call        000000006712C650
  000000006712D4BE: 48 89 30           mov         qword ptr [rax],rsi
  000000006712D4C1: EB 56              jmp         000000006712D519
  000000006712D4C3: 8B D1              mov         edx,ecx
  000000006712D4C5: 49 8D 4D 18        lea         rcx,[r13+18h]
  000000006712D4C9: E8 D2 F1 FF FF     call        000000006712C6A0
  000000006712D4CE: F6 43 20 20        test        byte ptr [rbx+20h],20h
  000000006712D4D2: 74 45              je          000000006712D519
  000000006712D4D4: 41 F6 45 20 40     test        byte ptr [r13+20h],40h
  000000006712D4D9: 74 3E              je          000000006712D519
  000000006712D4DB: 4C 8B C7           mov         r8,rdi
  000000006712D4DE: BA 47 00 00 00     mov         edx,47h
  000000006712D4E3: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006712D4E8: E8 F3 00 FF FF     call        000000006711D5E0
  000000006712D4ED: 90                 nop
  000000006712D4EE: 66 44 21 74 24 4A  and         word ptr [rsp+4Ah],r14w
  000000006712D4F4: 48 8B 0D 25 6D 1C  mov         rcx,qword ptr [00000000672F4220h]
                    00
  000000006712D4FB: 48 85 C9           test        rcx,rcx
  000000006712D4FE: 74 0F              je          000000006712D50F
  000000006712D500: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  000000006712D505: 48 8B 53 10        mov         rdx,qword ptr [rbx+10h]
  000000006712D509: E8 62 C0 FE FF     call        0000000067119570
  000000006712D50E: 90                 nop
  000000006712D50F: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006712D514: E8 67 00 FF FF     call        000000006711D580
  000000006712D519: 4C 89 63 10        mov         qword ptr [rbx+10h],r12
  000000006712D51D: 4D 85 E4           test        r12,r12
  000000006712D520: 0F 84 5B 01 00 00  je          000000006712D681
  000000006712D526: 49 8B 4C 24 08     mov         rcx,qword ptr [r12+8]
  000000006712D52B: 48 8B 41 40        mov         rax,qword ptr [rcx+40h]
  000000006712D52F: 48 39 43 40        cmp         qword ptr [rbx+40h],rax
  000000006712D533: 74 15              je          000000006712D54A
  000000006712D535: 48 8D 0D E4 F3 10  lea         rcx,[000000006723C920h]
                    00
  000000006712D53C: E8 9F F9 EE FF     call        000000006701CEE0
  000000006712D541: 48 89 73 10        mov         qword ptr [rbx+10h],rsi
  000000006712D545: E9 52 01 00 00     jmp         000000006712D69C
  000000006712D54A: 48 83 C1 18        add         rcx,18h
  000000006712D54E: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  000000006712D556: E8 F5 69 FD FF     call        0000000067103F50
  000000006712D55B: 44 8B 5B 20        mov         r11d,dword ptr [rbx+20h]
  000000006712D55F: 41 F6 C3 20        test        r11b,20h
  000000006712D563: 0F 84 18 01 00 00  je          000000006712D681
  000000006712D569: 48 8B 43 10        mov         rax,qword ptr [rbx+10h]
  000000006712D56D: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006712D571: F6 41 20 40        test        byte ptr [rcx+20h],40h
  000000006712D575: 0F 84 06 01 00 00  je          000000006712D681
  000000006712D57B: 41 F6 C3 01        test        r11b,1
  000000006712D57F: 0F 85 FC 00 00 00  jne         000000006712D681
  000000006712D585: 4C 8B C7           mov         r8,rdi
  000000006712D588: BA 44 00 00 00     mov         edx,44h
  000000006712D58D: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006712D592: E8 49 00 FF FF     call        000000006711D5E0
  000000006712D597: 90                 nop
  000000006712D598: 66 44 21 74 24 6A  and         word ptr [rsp+6Ah],r14w
  000000006712D59E: 48 8B 0D 7B 6C 1C  mov         rcx,qword ptr [00000000672F4220h]
                    00
  000000006712D5A5: 48 85 C9           test        rcx,rcx
  000000006712D5A8: 74 0E              je          000000006712D5B8
  000000006712D5AA: 4C 8D 44 24 58     lea         r8,[rsp+58h]
  000000006712D5AF: 48 8B 53 10        mov         rdx,qword ptr [rbx+10h]
  000000006712D5B3: E8 B8 BF FE FF     call        0000000067119570
  000000006712D5B8: 80 3D E9 47 1C 00  cmp         byte ptr [00000000672F1DA8h],0
                    00
  000000006712D5BF: 0F 84 B2 00 00 00  je          000000006712D677
  000000006712D5C5: 48 8B 43 10        mov         rax,qword ptr [rbx+10h]
  000000006712D5C9: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006712D5CD: 48 8B 41 68        mov         rax,qword ptr [rcx+68h]
  000000006712D5D1: 83 78 08 00        cmp         dword ptr [rax+8],0
  000000006712D5D5: 75 3B              jne         000000006712D612
  000000006712D5D7: B9 18 00 00 00     mov         ecx,18h
  000000006712D5DC: E8 A5 B4 04 00     call        0000000067178A86
  000000006712D5E1: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  000000006712D5E9: 48 85 C0           test        rax,rax
  000000006712D5EC: 74 0F              je          000000006712D5FD
  000000006712D5EE: BA 43 00 00 00     mov         edx,43h
  000000006712D5F3: 48 8B C8           mov         rcx,rax
  000000006712D5F6: E8 55 FF FE FF     call        000000006711D550
  000000006712D5FB: EB 03              jmp         000000006712D600
  000000006712D5FD: 48 8B C6           mov         rax,rsi
  000000006712D600: 41 B8 01 00 00 00  mov         r8d,1
  000000006712D606: 48 8B D0           mov         rdx,rax
  000000006712D609: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  000000006712D60D: E8 7E D8 FE FF     call        000000006711AE90
  000000006712D612: 48 85 FF           test        rdi,rdi
  000000006712D615: 74 12              je          000000006712D629
  000000006712D617: 48 8B CF           mov         rcx,rdi
  000000006712D61A: E8 A1 B8 F3 FF     call        0000000067068EC0
  000000006712D61F: 48 8B F0           mov         rsi,rax
  000000006712D622: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000006712D627: EB 05              jmp         000000006712D62E
  000000006712D629: 48 89 74 24 28     mov         qword ptr [rsp+28h],rsi
  000000006712D62E: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  000000006712D633: 48 8B 43 10        mov         rax,qword ptr [rbx+10h]
  000000006712D637: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006712D63B: 48 83 C1 68        add         rcx,68h
  000000006712D63F: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006712D644: E8 E7 E7 FF FF     call        000000006712BE30
  000000006712D649: 90                 nop
  000000006712D64A: 48 85 F6           test        rsi,rsi
  000000006712D64D: 74 28              je          000000006712D677
  000000006712D64F: F0 83 46 08 FF     lock add    dword ptr [rsi+8],0FFFFFFFFh
  000000006712D654: 75 19              jne         000000006712D66F
  000000006712D656: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000006712D65B: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006712D65E: BA 01 00 00 00     mov         edx,1
  000000006712D663: FF 10              call        qword ptr [rax]
  000000006712D665: 48 8B BC 24 B0 00  mov         rdi,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000006712D66D: EB 08              jmp         000000006712D677
  000000006712D66F: 48 8B BC 24 B0 00  mov         rdi,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000006712D677: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006712D67C: E8 FF FE FE FF     call        000000006711D580
  000000006712D681: F6 43 20 08        test        byte ptr [rbx+20h],8
  000000006712D685: 75 15              jne         000000006712D69C
  000000006712D687: 48 8B 4B 78        mov         rcx,qword ptr [rbx+78h]
  000000006712D68B: 48 85 C9           test        rcx,rcx
  000000006712D68E: 74 0C              je          000000006712D69C
  000000006712D690: 4D 8B C4           mov         r8,r12
  000000006712D693: 48 8B D7           mov         rdx,rdi
  000000006712D696: FF 15 BC 6C 1C 00  call        qword ptr [00000000672F4358h]
  000000006712D69C: 4C 8D 9C 24 80 00  lea         r11,[rsp+0000000000000080h]
                    00 00
  000000006712D6A4: 49 8B 5B 40        mov         rbx,qword ptr [r11+40h]
  000000006712D6A8: 49 8B 6B 48        mov         rbp,qword ptr [r11+48h]
  000000006712D6AC: 49 8B E3           mov         rsp,r11
  000000006712D6AF: 41 5E              pop         r14
  000000006712D6B1: 41 5D              pop         r13
  000000006712D6B3: 41 5C              pop         r12
  000000006712D6B5: 5F                 pop         rdi
  000000006712D6B6: 5E                 pop         rsi
  000000006712D6B7: C3                 ret

  Summary

      1A7000 .text
