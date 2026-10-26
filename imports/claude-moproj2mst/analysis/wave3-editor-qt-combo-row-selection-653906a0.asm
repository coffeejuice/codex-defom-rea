Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653906A0: 40 57              push        rdi
  00000000653906A2: 48 83 EC 30        sub         rsp,30h
  00000000653906A6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653906AF: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000653906B4: 48 8B D9           mov         rbx,rcx
  00000000653906B7: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000653906BB: BA 0F 00 00 00     mov         edx,0Fh
  00000000653906C0: 48 8B CF           mov         rcx,rdi
  00000000653906C3: E8 98 D6 CC FF     call        000000006505DD60
  00000000653906C8: C7 44 24 40 05 00  mov         dword ptr [rsp+40h],5
                    00 00
  00000000653906D0: BA 08 00 00 00     mov         edx,8
  00000000653906D5: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000653906DA: E8 B1 35 CB FF     call        0000000065043C90
  00000000653906DF: 8B 54 24 40        mov         edx,dword ptr [rsp+40h]
  00000000653906E3: 48 8B CF           mov         rcx,rdi
  00000000653906E6: E8 35 D4 CD FF     call        000000006506DB20
  00000000653906EB: 45 33 C0           xor         r8d,r8d
  00000000653906EE: 41 8D 50 2B        lea         edx,[r8+2Bh]
  00000000653906F2: 48 8B CB           mov         rcx,rbx
  00000000653906F5: E8 26 DD CC FF     call        000000006505E420
  00000000653906FA: B9 10 00 00 00     mov         ecx,10h
  00000000653906FF: E8 6E 0A 25 00     call        00000000655E1172
  0000000065390704: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000065390709: 48 85 C0           test        rax,rax
  000000006539070C: 74 13              je          0000000065390721
  000000006539070E: 4C 8B CF           mov         r9,rdi
  0000000065390711: 33 D2              xor         edx,edx
  0000000065390713: 44 8D 42 01        lea         r8d,[rdx+1]
  0000000065390717: 48 8B C8           mov         rcx,rax
  000000006539071A: E8 F1 D9 19 00     call        000000006552E110
  000000006539071F: EB 02              jmp         0000000065390723
  0000000065390721: 33 C0              xor         eax,eax
  0000000065390723: 48 8B D0           mov         rdx,rax
  0000000065390726: 48 8B CF           mov         rcx,rdi
  0000000065390729: E8 92 F4 FF FF     call        000000006538FBC0
  000000006539072E: 4C 8B 5F 08        mov         r11,qword ptr [rdi+8]
  0000000065390732: 49 83 BB E8 01 00  cmp         qword ptr [r11+00000000000001E8h],0
                    00 00
  000000006539073A: 75 11              jne         000000006539074D
  000000006539073C: 45 33 C0           xor         r8d,r8d
  000000006539073F: 41 8D 50 0E        lea         edx,[r8+0Eh]
  0000000065390743: 48 8B CF           mov         rcx,rdi
  0000000065390746: E8 95 88 CD FF     call        0000000065068FE0
  000000006539074B: EB 10              jmp         000000006539075D
  000000006539074D: 41 B0 01           mov         r8b,1
  0000000065390750: BA 0E 00 00 00     mov         edx,0Eh
  0000000065390755: 48 8B CF           mov         rcx,rdi
  0000000065390758: E8 83 88 CD FF     call        0000000065068FE0
  000000006539075D: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065390762: 48 83 C4 30        add         rsp,30h
  0000000065390766: 5F                 pop         rdi
  0000000065390767: C3                 ret

  Summary

      695000 .text
