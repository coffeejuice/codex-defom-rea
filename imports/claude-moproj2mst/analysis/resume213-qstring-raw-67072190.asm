Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067072190: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000067072195: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  000000006707219A: 56                 push        rsi
  000000006707219B: 57                 push        rdi
  000000006707219C: 41 54              push        r12
  000000006707219E: 48 83 EC 20        sub         rsp,20h
  00000000670721A2: 45 33 E4           xor         r12d,r12d
  00000000670721A5: 48 8B E9           mov         rbp,rcx
  00000000670721A8: 41 8B F8           mov         edi,r8d
  00000000670721AB: 41 8D 4C 24 20     lea         ecx,[r12+20h]
  00000000670721B0: 48 8B F2           mov         rsi,rdx
  00000000670721B3: 44 89 64 24 50     mov         dword ptr [rsp+50h],r12d
  00000000670721B8: E8 E3 E1 FA FF     call        00000000670203A0
  00000000670721BD: 48 8B D8           mov         rbx,rax
  00000000670721C0: 48 85 C0           test        rax,rax
  00000000670721C3: 75 05              jne         00000000670721CA
  00000000670721C5: E8 E6 9D FA FF     call        000000006701BFB0
  00000000670721CA: 48 85 F6           test        rsi,rsi
  00000000670721CD: 74 06              je          00000000670721D5
  00000000670721CF: 48 89 73 10        mov         qword ptr [rbx+10h],rsi
  00000000670721D3: EB 0B              jmp         00000000670721E0
  00000000670721D5: 48 8D 43 1A        lea         rax,[rbx+1Ah]
  00000000670721D9: 41 8B FC           mov         edi,r12d
  00000000670721DC: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  00000000670721E0: B8 E0 FF 00 00     mov         eax,0FFE0h
  00000000670721E5: 48 89 5D 00        mov         qword ptr [rbp],rbx
  00000000670721E9: C7 03 01 00 00 00  mov         dword ptr [rbx],1
  00000000670721EF: 66 21 43 18        and         word ptr [rbx+18h],ax
  00000000670721F3: 89 7B 08           mov         dword ptr [rbx+8],edi
  00000000670721F6: 89 7B 04           mov         dword ptr [rbx+4],edi
  00000000670721F9: 48 8B C5           mov         rax,rbp
  00000000670721FC: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  0000000067072201: 66 44 89 63 1A     mov         word ptr [rbx+1Ah],r12w
  0000000067072206: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006707220B: 48 83 C4 20        add         rsp,20h
  000000006707220F: 41 5C              pop         r12
  0000000067072211: 5F                 pop         rdi
  0000000067072212: 5E                 pop         rsi
  0000000067072213: C3                 ret

  Summary

      1A7000 .text
