Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067070410: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000067070415: 57                 push        rdi
  0000000067070416: 48 83 EC 20        sub         rsp,20h
  000000006707041A: 48 8B 02           mov         rax,qword ptr [rdx]
  000000006707041D: 48 8B FA           mov         rdi,rdx
  0000000067070420: 48 8D 15 09 07 28  lea         rdx,[00000000672F0B30h]
                    00
  0000000067070427: 48 8B D9           mov         rbx,rcx
  000000006707042A: 48 3B C2           cmp         rax,rdx
  000000006707042D: 0F 84 B0 00 00 00  je          00000000670704E3
  0000000067070433: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000067070436: 48 3B CA           cmp         rcx,rdx
  0000000067070439: 75 29              jne         0000000067070464
  000000006707043B: F0 83 00 01        lock add    dword ptr [rax],1
  000000006707043F: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067070442: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  0000000067070446: 75 08              jne         0000000067070450
  0000000067070448: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006707044B: E8 80 FC FF FF     call        00000000670700D0
  0000000067070450: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067070453: 48 89 03           mov         qword ptr [rbx],rax
  0000000067070456: 48 8B C3           mov         rax,rbx
  0000000067070459: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006707045E: 48 83 C4 20        add         rsp,20h
  0000000067070462: 5F                 pop         rdi
  0000000067070463: C3                 ret
  0000000067070464: 8B 01              mov         eax,dword ptr [rcx]
  0000000067070466: 83 F8 01           cmp         eax,1
  0000000067070469: 75 11              jne         000000006707047C
  000000006707046B: 48 8B 13           mov         rdx,qword ptr [rbx]
  000000006707046E: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067070471: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000067070474: 03 4A 08           add         ecx,dword ptr [rdx+8]
  0000000067070477: 3B 4A 04           cmp         ecx,dword ptr [rdx+4]
  000000006707047A: 7E 27              jle         00000000670704A3
  000000006707047C: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006707047F: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067070482: 8B 50 08           mov         edx,dword ptr [rax+8]
  0000000067070485: 03 51 08           add         edx,dword ptr [rcx+8]
  0000000067070488: 8D 0C 12           lea         ecx,[rdx+rdx]
  000000006707048B: BA 20 00 00 00     mov         edx,20h
  0000000067070490: E8 0B C6 FB FF     call        000000006702CAA0
  0000000067070495: 48 8B CB           mov         rcx,rbx
  0000000067070498: 48 63 D0           movsxd      rdx,eax
  000000006707049B: 48 D1 EA           shr         rdx,1
  000000006707049E: E8 FD FC FF FF     call        00000000670701A0
  00000000670704A3: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670704A6: 48 8B 17           mov         rdx,qword ptr [rdi]
  00000000670704A9: 48 63 48 08        movsxd      rcx,dword ptr [rax+8]
  00000000670704AD: 4C 63 42 08        movsxd      r8,dword ptr [rdx+8]
  00000000670704B1: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000000670704B5: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  00000000670704B9: 48 8D 0C 48        lea         rcx,[rax+rcx*2]
  00000000670704BD: 4D 03 C0           add         r8,r8
  00000000670704C0: E8 D9 85 10 00     call        0000000067178A9E
  00000000670704C5: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670704C8: 4C 8B 1B           mov         r11,qword ptr [rbx]
  00000000670704CB: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000670704CE: 41 01 4B 08        add         dword ptr [r11+8],ecx
  00000000670704D2: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670704D5: 48 63 50 08        movsxd      rdx,dword ptr [rax+8]
  00000000670704D9: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000000670704DD: 33 C0              xor         eax,eax
  00000000670704DF: 66 89 04 51        mov         word ptr [rcx+rdx*2],ax
  00000000670704E3: 48 8B C3           mov         rax,rbx
  00000000670704E6: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000670704EB: 48 83 C4 20        add         rsp,20h
  00000000670704EF: 5F                 pop         rdi
  00000000670704F0: C3                 ret

  Summary

      1A7000 .text
