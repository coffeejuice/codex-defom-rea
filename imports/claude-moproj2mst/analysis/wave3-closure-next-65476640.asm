Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065476640: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000065476645: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  000000006547664A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  000000006547664F: 57                 push        rdi
  0000000065476650: 41 54              push        r12
  0000000065476652: 41 55              push        r13
  0000000065476654: 48 83 EC 20        sub         rsp,20h
  0000000065476658: 4C 8B 69 08        mov         r13,qword ptr [rcx+8]
  000000006547665C: 48 8B F9           mov         rdi,rcx
  000000006547665F: 8B CA              mov         ecx,edx
  0000000065476661: 49 8B 75 08        mov         rsi,qword ptr [r13+8]
  0000000065476665: 41 8B E9           mov         ebp,r9d
  0000000065476668: 41 8B D8           mov         ebx,r8d
  000000006547666B: 44 8B E2           mov         r12d,edx
  000000006547666E: E8 5D D4 FF FF     call        0000000065473AD0
  0000000065476673: 48 8B 8E 58 02 00  mov         rcx,qword ptr [rsi+0000000000000258h]
                    00
  000000006547667A: 8B D0              mov         edx,eax
  000000006547667C: E8 1F 6A F2 FF     call        000000006539D0A0
  0000000065476681: 48 85 C0           test        rax,rax
  0000000065476684: 74 07              je          000000006547668D
  0000000065476686: C6 86 91 02 00 00  mov         byte ptr [rsi+0000000000000291h],0
                    00
  000000006547668D: 49 8B 75 08        mov         rsi,qword ptr [r13+8]
  0000000065476691: 8B CB              mov         ecx,ebx
  0000000065476693: E8 38 D4 FF FF     call        0000000065473AD0
  0000000065476698: 48 8B 8E 58 02 00  mov         rcx,qword ptr [rsi+0000000000000258h]
                    00
  000000006547669F: 8B D0              mov         edx,eax
  00000000654766A1: E8 FA 69 F2 FF     call        000000006539D0A0
  00000000654766A6: 48 85 C0           test        rax,rax
  00000000654766A9: 74 07              je          00000000654766B2
  00000000654766AB: C6 86 91 02 00 00  mov         byte ptr [rsi+0000000000000291h],0
                    00
  00000000654766B2: 49 8B 75 08        mov         rsi,qword ptr [r13+8]
  00000000654766B6: 8B CD              mov         ecx,ebp
  00000000654766B8: E8 13 D4 FF FF     call        0000000065473AD0
  00000000654766BD: 48 8B 8E 58 02 00  mov         rcx,qword ptr [rsi+0000000000000258h]
                    00
  00000000654766C4: 8B D0              mov         edx,eax
  00000000654766C6: E8 D5 69 F2 FF     call        000000006539D0A0
  00000000654766CB: 48 85 C0           test        rax,rax
  00000000654766CE: 74 07              je          00000000654766D7
  00000000654766D0: C6 86 91 02 00 00  mov         byte ptr [rsi+0000000000000291h],0
                    00
  00000000654766D7: 4C 8B 47 08        mov         r8,qword ptr [rdi+8]
  00000000654766DB: 33 F6              xor         esi,esi
  00000000654766DD: 41 0F BA E4 08     bt          r12d,8
  00000000654766E2: 73 05              jae         00000000654766E9
  00000000654766E4: 41 8B CC           mov         ecx,r12d
  00000000654766E7: EB 13              jmp         00000000654766FC
  00000000654766E9: 0F BA E3 08        bt          ebx,8
  00000000654766ED: 73 04              jae         00000000654766F3
  00000000654766EF: 8B CB              mov         ecx,ebx
  00000000654766F1: EB 09              jmp         00000000654766FC
  00000000654766F3: 8B CE              mov         ecx,esi
  00000000654766F5: 0F BA E5 08        bt          ebp,8
  00000000654766F9: 0F 42 CD           cmovb       ecx,ebp
  00000000654766FC: 4D 8B 40 08        mov         r8,qword ptr [r8+8]
  0000000065476700: E8 CB D3 FF FF     call        0000000065473AD0
  0000000065476705: 49 8B 88 58 02 00  mov         rcx,qword ptr [r8+0000000000000258h]
                    00
  000000006547670C: 8B D0              mov         edx,eax
  000000006547670E: E8 2D 51 F2 FF     call        000000006539B840
  0000000065476713: 49 8B CD           mov         rcx,r13
  0000000065476716: 48 8B D0           mov         rdx,rax
  0000000065476719: E8 82 F9 FF FF     call        00000000654760A0
  000000006547671E: 41 0F BA E4 09     bt          r12d,9
  0000000065476723: 4C 8B 47 08        mov         r8,qword ptr [rdi+8]
  0000000065476727: 73 05              jae         000000006547672E
  0000000065476729: 41 8B F4           mov         esi,r12d
  000000006547672C: EB 11              jmp         000000006547673F
  000000006547672E: 0F BA E3 09        bt          ebx,9
  0000000065476732: 73 04              jae         0000000065476738
  0000000065476734: 8B F3              mov         esi,ebx
  0000000065476736: EB 07              jmp         000000006547673F
  0000000065476738: 0F BA E5 09        bt          ebp,9
  000000006547673C: 0F 42 F5           cmovb       esi,ebp
  000000006547673F: 4D 8B 40 08        mov         r8,qword ptr [r8+8]
  0000000065476743: 8B CE              mov         ecx,esi
  0000000065476745: E8 86 D3 FF FF     call        0000000065473AD0
  000000006547674A: 49 8B 88 58 02 00  mov         rcx,qword ptr [r8+0000000000000258h]
                    00
  0000000065476751: 8B D0              mov         edx,eax
  0000000065476753: E8 E8 50 F2 FF     call        000000006539B840
  0000000065476758: 49 8B CD           mov         rcx,r13
  000000006547675B: 48 8B D0           mov         rdx,rax
  000000006547675E: E8 6D F6 FF FF     call        0000000065475DD0
  0000000065476763: 45 85 E4           test        r12d,r12d
  0000000065476766: 74 12              je          000000006547677A
  0000000065476768: 41 F7 C4 00 FC FF  test        r12d,0FFFFFC00h
                    FF
  000000006547676F: 75 09              jne         000000006547677A
  0000000065476771: C6 87 90 02 00 00  mov         byte ptr [rdi+0000000000000290h],1
                    01
  0000000065476778: EB 1F              jmp         0000000065476799
  000000006547677A: 85 DB              test        ebx,ebx
  000000006547677C: 74 08              je          0000000065476786
  000000006547677E: F7 C3 00 FC FF FF  test        ebx,0FFFFFC00h
  0000000065476784: 74 EB              je          0000000065476771
  0000000065476786: 85 ED              test        ebp,ebp
  0000000065476788: 74 08              je          0000000065476792
  000000006547678A: F7 C5 00 FC FF FF  test        ebp,0FFFFFC00h
  0000000065476790: 74 DF              je          0000000065476771
  0000000065476792: C6 87 90 02 00 00  mov         byte ptr [rdi+0000000000000290h],0
                    00
  0000000065476799: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006547679E: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  00000000654767A3: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  00000000654767A8: 48 83 C4 20        add         rsp,20h
  00000000654767AC: 41 5D              pop         r13
  00000000654767AE: 41 5C              pop         r12
  00000000654767B0: 5F                 pop         rdi
  00000000654767B1: C3                 ret

  Summary

      695000 .text
