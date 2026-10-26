Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

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

  Summary

      1A7000 .text
