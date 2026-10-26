Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006552A60A: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006552A60F: 48 89 74 24 48     mov         qword ptr [rsp+48h],rsi
  000000006552A614: 49 8B 31           mov         rsi,qword ptr [r9]
  000000006552A617: BA 01 00 00 00     mov         edx,1
  000000006552A61C: 48 89 7C 24 50     mov         qword ptr [rsp+50h],rdi
  000000006552A621: FF 15 49 C4 16 00  call        qword ptr [0000000065696A70h]
  000000006552A627: 84 C0              test        al,al
  000000006552A629: 75 11              jne         000000006552A63C
  000000006552A62B: 48 8B 55 00        mov         rdx,qword ptr [rbp]
  000000006552A62F: 8B 42 08           mov         eax,dword ptr [rdx+8]
  000000006552A632: 41 8D 4C 05 00     lea         ecx,[r13+rax]
  000000006552A637: 3B 4A 04           cmp         ecx,dword ptr [rdx+4]
  000000006552A63A: 7E 2F              jle         000000006552A66B
  000000006552A63C: 48 8B 45 00        mov         rax,qword ptr [rbp]
  000000006552A640: 45 33 C9           xor         r9d,r9d
  000000006552A643: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006552A646: 45 8D 41 08        lea         r8d,[r9+8]
  000000006552A64A: 41 8D 54 0D 00     lea         edx,[r13+rcx]
  000000006552A64F: 41 8D 49 18        lea         ecx,[r9+18h]
  000000006552A653: FF 15 8F CA 16 00  call        qword ptr [00000000656970E8h]
  000000006552A659: 48 8B CD           mov         rcx,rbp
  000000006552A65C: 44 8B C0           mov         r8d,eax
  000000006552A65F: 48 8B 45 00        mov         rax,qword ptr [rbp]
  000000006552A663: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006552A666: E8 55 56 FE FF     call        000000006550FCC0
  000000006552A66B: 48 8B 4D 00        mov         rcx,qword ptr [rbp]
  000000006552A66F: 49 63 C4           movsxd      rax,r12d
  000000006552A672: 48 8D 7C C1 10     lea         rdi,[rcx+rax*8+10h]
  000000006552A677: 8B 41 08           mov         eax,dword ptr [rcx+8]
  000000006552A67A: 41 2B C4           sub         eax,r12d
  000000006552A67D: 4A 8D 1C EF        lea         rbx,[rdi+r13*8]
  000000006552A681: 48 8B D7           mov         rdx,rdi
  000000006552A684: 4C 63 C0           movsxd      r8,eax
  000000006552A687: 48 8B CB           mov         rcx,rbx
  000000006552A68A: 49 C1 E0 03        shl         r8,3
  000000006552A68E: FF 15 CC BF 16 00  call        qword ptr [0000000065696660h]
  000000006552A694: 48 3B DF           cmp         rbx,rdi
  000000006552A697: 74 15              je          000000006552A6AE
  000000006552A699: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006552A6A0: 48 83 EB 08        sub         rbx,8
  000000006552A6A4: 74 03              je          000000006552A6A9
  000000006552A6A6: 48 89 33           mov         qword ptr [rbx],rsi
  000000006552A6A9: 48 3B DF           cmp         rbx,rdi
  000000006552A6AC: 75 F2              jne         000000006552A6A0
  000000006552A6AE: 48 8B 45 00        mov         rax,qword ptr [rbp]
  000000006552A6B2: 48 8B 7C 24 50     mov         rdi,qword ptr [rsp+50h]
  000000006552A6B7: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  000000006552A6BC: 44 01 68 08        add         dword ptr [rax+8],r13d
  000000006552A6C0: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]

  Summary

      695000 .text
