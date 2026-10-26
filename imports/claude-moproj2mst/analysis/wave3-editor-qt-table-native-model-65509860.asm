Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065509860: 4C 8B DC           mov         r11,rsp
  0000000065509863: 57                 push        rdi
  0000000065509864: 41 54              push        r12
  0000000065509866: 41 55              push        r13
  0000000065509868: 41 56              push        r14
  000000006550986A: 41 57              push        r15
  000000006550986C: 48 83 EC 50        sub         rsp,50h
  0000000065509870: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065509879: 49 89 5B 08        mov         qword ptr [r11+8],rbx
  000000006550987D: 49 89 6B 10        mov         qword ptr [r11+10h],rbp
  0000000065509881: 49 89 73 18        mov         qword ptr [r11+18h],rsi
  0000000065509885: 45 8B F8           mov         r15d,r8d
  0000000065509888: 4C 63 EA           movsxd      r13,edx
  000000006550988B: 4C 8B F1           mov         r14,rcx
  000000006550988E: 41 83 F8 01        cmp         r8d,1
  0000000065509892: 0F 8C 1D 01 00 00  jl          00000000655099B5
  0000000065509898: 85 D2              test        edx,edx
  000000006550989A: 0F 88 15 01 00 00  js          00000000655099B5
  00000000655098A0: 48 8B 41 28        mov         rax,qword ptr [rcx+28h]
  00000000655098A4: 44 3B 68 08        cmp         r13d,dword ptr [rax+8]
  00000000655098A8: 0F 8F 07 01 00 00  jg          00000000655099B5
  00000000655098AE: C7 44 24 30 FF FF  mov         dword ptr [rsp+30h],0FFFFFFFFh
                    FF FF
  00000000655098B6: C7 44 24 34 FF FF  mov         dword ptr [rsp+34h],0FFFFFFFFh
                    FF FF
  00000000655098BE: 33 ED              xor         ebp,ebp
  00000000655098C0: 49 89 6B C0        mov         qword ptr [r11-40h],rbp
  00000000655098C4: 49 89 6B C8        mov         qword ptr [r11-38h],rbp
  00000000655098C8: 47 8D 4C 05 FF     lea         r9d,[r13+r8-1]
  00000000655098CD: 45 8B C5           mov         r8d,r13d
  00000000655098D0: 49 8D 53 B8        lea         rdx,[r11-48h]
  00000000655098D4: FF 15 FE F1 18 00  call        qword ptr [0000000065698AD8h]
  00000000655098DA: 90                 nop
  00000000655098DB: 49 8B 46 20        mov         rax,qword ptr [r14+20h]
  00000000655098DF: 44 8B 60 08        mov         r12d,dword ptr [rax+8]
  00000000655098E3: 49 8B 46 28        mov         rax,qword ptr [r14+28h]
  00000000655098E7: 8B 58 08           mov         ebx,dword ptr [rax+8]
  00000000655098EA: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  00000000655098EF: 8D 55 01           lea         edx,[rbp+1]
  00000000655098F2: 48 8B C8           mov         rcx,rax
  00000000655098F5: FF 15 75 D1 18 00  call        qword ptr [0000000065696A70h]
  00000000655098FB: 84 C0              test        al,al
  00000000655098FD: 74 14              je          0000000065509913
  00000000655098FF: 49 8B 46 28        mov         rax,qword ptr [r14+28h]
  0000000065509903: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  0000000065509907: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006550990A: 49 8D 4E 28        lea         rcx,[r14+28h]
  000000006550990E: E8 AD 63 00 00     call        000000006550FCC0
  0000000065509913: 49 8B 46 28        mov         rax,qword ptr [r14+28h]
  0000000065509917: 4A 8D 54 E8 10     lea         rdx,[rax+r13*8+10h]
  000000006550991C: 4C 8D 4C 24 20     lea         r9,[rsp+20h]
  0000000065509921: 45 8B C7           mov         r8d,r15d
  0000000065509924: 49 8D 4E 28        lea         rcx,[r14+28h]
  0000000065509928: E8 B3 0C 02 00     call        000000006552A5E0
  000000006550992D: 85 DB              test        ebx,ebx
  000000006550992F: 75 12              jne         0000000065509943
  0000000065509931: 45 0F AF E7        imul        r12d,r15d
  0000000065509935: 49 8D 4E 18        lea         rcx,[r14+18h]
  0000000065509939: 41 8B D4           mov         edx,r12d
  000000006550993C: E8 3F 0C 02 00     call        000000006552A580
  0000000065509941: EB 65              jmp         00000000655099A8
  0000000065509943: 45 85 E4           test        r12d,r12d
  0000000065509946: 7E 60              jle         00000000655099A8
  0000000065509948: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  000000006550994D: 0F 1F 00           nop         dword ptr [rax]
  0000000065509950: 49 8B 46 28        mov         rax,qword ptr [r14+28h]
  0000000065509954: 8B 78 08           mov         edi,dword ptr [rax+8]
  0000000065509957: BA 01 00 00 00     mov         edx,1
  000000006550995C: 49 8B 4E 18        mov         rcx,qword ptr [r14+18h]
  0000000065509960: FF 15 0A D1 18 00  call        qword ptr [0000000065696A70h]
  0000000065509966: 84 C0              test        al,al
  0000000065509968: 74 14              je          000000006550997E
  000000006550996A: 49 8B 46 18        mov         rax,qword ptr [r14+18h]
  000000006550996E: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  0000000065509972: 8B 50 08           mov         edx,dword ptr [rax+8]
  0000000065509975: 49 8D 4E 18        lea         rcx,[r14+18h]
  0000000065509979: E8 42 63 00 00     call        000000006550FCC0
  000000006550997E: 0F AF FD           imul        edi,ebp
  0000000065509981: 41 03 FD           add         edi,r13d
  0000000065509984: 48 63 CF           movsxd      rcx,edi
  0000000065509987: 49 8B 46 18        mov         rax,qword ptr [r14+18h]
  000000006550998B: 48 8D 54 C8 10     lea         rdx,[rax+rcx*8+10h]
  0000000065509990: 4C 8D 4C 24 20     lea         r9,[rsp+20h]
  0000000065509995: 45 8B C7           mov         r8d,r15d
  0000000065509998: 49 8D 4E 18        lea         rcx,[r14+18h]
  000000006550999C: E8 3F 0C 02 00     call        000000006552A5E0
  00000000655099A1: FF C5              inc         ebp
  00000000655099A3: 41 3B EC           cmp         ebp,r12d
  00000000655099A6: 7C A8              jl          0000000065509950
  00000000655099A8: 49 8B CE           mov         rcx,r14
  00000000655099AB: FF 15 1F F1 18 00  call        qword ptr [0000000065698AD0h]
  00000000655099B1: B0 01              mov         al,1
  00000000655099B3: EB 02              jmp         00000000655099B7
  00000000655099B5: 32 C0              xor         al,al
  00000000655099B7: 4C 8D 5C 24 50     lea         r11,[rsp+50h]
  00000000655099BC: 49 8B 5B 30        mov         rbx,qword ptr [r11+30h]
  00000000655099C0: 49 8B 6B 38        mov         rbp,qword ptr [r11+38h]
  00000000655099C4: 49 8B 73 40        mov         rsi,qword ptr [r11+40h]
  00000000655099C8: 49 8B E3           mov         rsp,r11
  00000000655099CB: 41 5F              pop         r15
  00000000655099CD: 41 5E              pop         r14
  00000000655099CF: 41 5D              pop         r13
  00000000655099D1: 41 5C              pop         r12
  00000000655099D3: 5F                 pop         rdi
  00000000655099D4: C3                 ret

  Summary

      695000 .text
