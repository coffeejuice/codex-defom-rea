Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C2210: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670C2215: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  00000000670C221A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  00000000670C221F: 57                 push        rdi
  00000000670C2220: 48 83 EC 20        sub         rsp,20h
  00000000670C2224: 33 ED              xor         ebp,ebp
  00000000670C2226: C7 81 B0 00 00 00  mov         dword ptr [rcx+00000000000000B0h],6
                    06 00 00 00
  00000000670C2230: C7 81 C0 00 00 00  mov         dword ptr [rcx+00000000000000C0h],1
                    01 00 00 00
  00000000670C223A: 89 A9 B4 00 00 00  mov         dword ptr [rcx+00000000000000B4h],ebp
  00000000670C2240: 89 A9 B8 00 00 00  mov         dword ptr [rcx+00000000000000B8h],ebp
  00000000670C2246: 89 A9 C4 00 00 00  mov         dword ptr [rcx+00000000000000C4h],ebp
  00000000670C224C: B8 20 00 00 00     mov         eax,20h
  00000000670C2251: 48 8B F9           mov         rdi,rcx
  00000000670C2254: 66 89 81 BC 00 00  mov         word ptr [rcx+00000000000000BCh],ax
                    00
  00000000670C225B: 89 A9 C8 00 00 00  mov         dword ptr [rcx+00000000000000C8h],ebp
  00000000670C2261: 48 89 29           mov         qword ptr [rcx],rbp
  00000000670C2264: 40 88 69 20        mov         byte ptr [rcx+20h],bpl
  00000000670C2268: 48 89 69 28        mov         qword ptr [rcx+28h],rbp
  00000000670C226C: 89 69 30           mov         dword ptr [rcx+30h],ebp
  00000000670C226F: 89 69 34           mov         dword ptr [rcx+34h],ebp
  00000000670C2272: 89 A9 A0 00 00 00  mov         dword ptr [rcx+00000000000000A0h],ebp
  00000000670C2278: 48 89 A9 A8 00 00  mov         qword ptr [rcx+00000000000000A8h],rbp
                    00
  00000000670C227F: 89 A9 8C 00 00 00  mov         dword ptr [rcx+000000000000008Ch],ebp
  00000000670C2285: E8 E6 BC 08 00     call        000000006714DF70
  00000000670C228A: 48 8D 5F 40        lea         rbx,[rdi+40h]
  00000000670C228E: 48 8B CB           mov         rcx,rbx
  00000000670C2291: 48 89 47 38        mov         qword ptr [rdi+38h],rax
  00000000670C2295: E8 66 92 08 00     call        000000006714B500
  00000000670C229A: 48 85 DB           test        rbx,rbx
  00000000670C229D: 74 15              je          00000000670C22B4
  00000000670C229F: 89 2B              mov         dword ptr [rbx],ebp
  00000000670C22A1: 89 6B 04           mov         dword ptr [rbx+4],ebp
  00000000670C22A4: 89 6B 08           mov         dword ptr [rbx+8],ebp
  00000000670C22A7: 48 89 6B 18        mov         qword ptr [rbx+18h],rbp
  00000000670C22AB: 89 6B 14           mov         dword ptr [rbx+14h],ebp
  00000000670C22AE: 89 6B 10           mov         dword ptr [rbx+10h],ebp
  00000000670C22B1: 89 6B 0C           mov         dword ptr [rbx+0Ch],ebp
  00000000670C22B4: 48 8D 5F 60        lea         rbx,[rdi+60h]
  00000000670C22B8: 48 8B CB           mov         rcx,rbx
  00000000670C22BB: E8 40 92 08 00     call        000000006714B500
  00000000670C22C0: 48 85 DB           test        rbx,rbx
  00000000670C22C3: 74 15              je          00000000670C22DA
  00000000670C22C5: 89 2B              mov         dword ptr [rbx],ebp
  00000000670C22C7: 89 6B 04           mov         dword ptr [rbx+4],ebp
  00000000670C22CA: 89 6B 08           mov         dword ptr [rbx+8],ebp
  00000000670C22CD: 48 89 6B 18        mov         qword ptr [rbx+18h],rbp
  00000000670C22D1: 89 6B 14           mov         dword ptr [rbx+14h],ebp
  00000000670C22D4: 89 6B 10           mov         dword ptr [rbx+10h],ebp
  00000000670C22D7: 89 6B 0C           mov         dword ptr [rbx+0Ch],ebp
  00000000670C22DA: 48 8B B7 80 00 00  mov         rsi,qword ptr [rdi+0000000000000080h]
                    00
  00000000670C22E1: 48 85 F6           test        rsi,rsi
  00000000670C22E4: 74 10              je          00000000670C22F6
  00000000670C22E6: 48 8B CE           mov         rcx,rsi
  00000000670C22E9: E8 12 92 08 00     call        000000006714B500
  00000000670C22EE: 48 8B CE           mov         rcx,rsi
  00000000670C22F1: E8 60 66 0B 00     call        0000000067178956
  00000000670C22F6: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  00000000670C22FB: 48 89 AF 80 00 00  mov         qword ptr [rdi+0000000000000080h],rbp
                    00
  00000000670C2302: 83 0B 01           or          dword ptr [rbx],1
  00000000670C2305: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000670C230A: 48 8B 6C 24 38     mov         rbp,qword ptr [rsp+38h]
  00000000670C230F: C6 87 88 00 00 00  mov         byte ptr [rdi+0000000000000088h],1
                    01
  00000000670C2316: 48 83 C4 20        add         rsp,20h
  00000000670C231A: 5F                 pop         rdi
  00000000670C231B: C3                 ret
  00000000670C231C: CC                 int         3
  00000000670C231D: CC                 int         3
  00000000670C231E: CC                 int         3
  00000000670C231F: CC                 int         3
  00000000670C2320: 40 53              push        rbx
  00000000670C2322: 48 83 EC 30        sub         rsp,30h
  00000000670C2326: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C232F: 48 8B D9           mov         rbx,rcx
  00000000670C2332: 48 81 C1 98 00 00  add         rcx,98h
                    00
  00000000670C2339: 48 8D 05 F0 E7 22  lea         rax,[00000000672F0B30h]
                    00
  00000000670C2340: 48 39 01           cmp         qword ptr [rcx],rax
  00000000670C2343: 74 2D              je          00000000670C2372
  00000000670C2345: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670C234A: F0 83 05 DE E7 22  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670C2352: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670C2357: E8 74 E0 FA FF     call        00000000670703D0
  00000000670C235C: 90                 nop
  00000000670C235D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670C2362: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670C2366: 75 0A              jne         00000000670C2372
  00000000670C2368: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670C236D: E8 5E DD FA FF     call        00000000670700D0
  00000000670C2372: 33 C0              xor         eax,eax
  00000000670C2374: 89 83 A0 00 00 00  mov         dword ptr [rbx+00000000000000A0h],eax
  00000000670C237A: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670C237D: 48 85 C9           test        rcx,rcx
  00000000670C2380: 74 0F              je          00000000670C2391
  00000000670C2382: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670C2385: FF 50 70           call        qword ptr [rax+70h]
  00000000670C2388: 48 89 83 A8 00 00  mov         qword ptr [rbx+00000000000000A8h],rax
                    00
  00000000670C238F: EB 07              jmp         00000000670C2398
  00000000670C2391: 48 89 83 A8 00 00  mov         qword ptr [rbx+00000000000000A8h],rax
                    00
  00000000670C2398: 48 83 C4 30        add         rsp,30h
  00000000670C239C: 5B                 pop         rbx
  00000000670C239D: C3                 ret
  00000000670C239E: CC                 int         3
  00000000670C239F: CC                 int         3
  00000000670C23A0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  00000000670C23A5: 57                 push        rdi
  00000000670C23A6: 48 83 EC 20        sub         rsp,20h
  00000000670C23AA: 48 83 79 58 00     cmp         qword ptr [rcx+58h],0
  00000000670C23AF: 48 8B FA           mov         rdi,rdx
  00000000670C23B2: 48 8B D9           mov         rbx,rcx
  00000000670C23B5: 75 74              jne         00000000670C242B
  00000000670C23B7: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  00000000670C23BC: 33 F6              xor         esi,esi
  00000000670C23BE: 48 39 B1 80 00 00  cmp         qword ptr [rcx+0000000000000080h],rsi
                    00
  00000000670C23C5: 75 2E              jne         00000000670C23F5
  00000000670C23C7: 8D 4E 20           lea         ecx,[rsi+20h]
  00000000670C23CA: E8 B7 66 0B 00     call        0000000067178A86
  00000000670C23CF: 48 85 C0           test        rax,rax
  00000000670C23D2: 74 17              je          00000000670C23EB
  00000000670C23D4: 89 30              mov         dword ptr [rax],esi
  00000000670C23D6: 89 70 04           mov         dword ptr [rax+4],esi
  00000000670C23D9: 89 70 08           mov         dword ptr [rax+8],esi
  00000000670C23DC: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  00000000670C23E0: 89 70 14           mov         dword ptr [rax+14h],esi
  00000000670C23E3: 89 70 10           mov         dword ptr [rax+10h],esi
  00000000670C23E6: 89 70 0C           mov         dword ptr [rax+0Ch],esi
  00000000670C23E9: EB 03              jmp         00000000670C23EE
  00000000670C23EB: 48 8B C6           mov         rax,rsi
  00000000670C23EE: 48 89 83 80 00 00  mov         qword ptr [rbx+0000000000000080h],rax
                    00
  00000000670C23F5: 8B 43 40           mov         eax,dword ptr [rbx+40h]
  00000000670C23F8: 48 8B 8B 80 00 00  mov         rcx,qword ptr [rbx+0000000000000080h]
                    00
  00000000670C23FF: 89 01              mov         dword ptr [rcx],eax
  00000000670C2401: 8B 43 48           mov         eax,dword ptr [rbx+48h]
  00000000670C2404: 89 41 08           mov         dword ptr [rcx+8],eax
  00000000670C2407: 8B 43 4C           mov         eax,dword ptr [rbx+4Ch]
  00000000670C240A: 89 41 0C           mov         dword ptr [rcx+0Ch],eax
  00000000670C240D: 8B 43 50           mov         eax,dword ptr [rbx+50h]
  00000000670C2410: 89 41 10           mov         dword ptr [rcx+10h],eax
  00000000670C2413: 8B 43 54           mov         eax,dword ptr [rbx+54h]
  00000000670C2416: 89 41 14           mov         dword ptr [rcx+14h],eax
  00000000670C2419: 89 B3 A4 00 00 00  mov         dword ptr [rbx+00000000000000A4h],esi
  00000000670C241F: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  00000000670C2424: 48 89 BB A8 00 00  mov         qword ptr [rbx+00000000000000A8h],rdi
                    00
  00000000670C242B: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670C2430: 48 83 C4 20        add         rsp,20h
  00000000670C2434: 5F                 pop         rdi
  00000000670C2435: C3                 ret
  00000000670C2436: CC                 int         3
  00000000670C2437: CC                 int         3
  00000000670C2438: CC                 int         3
  00000000670C2439: CC                 int         3
  00000000670C243A: CC                 int         3
  00000000670C243B: CC                 int         3
  00000000670C243C: CC                 int         3
  00000000670C243D: CC                 int         3
  00000000670C243E: CC                 int         3
  00000000670C243F: CC                 int         3
  00000000670C2440: 48

  Summary

      1A7000 .text
