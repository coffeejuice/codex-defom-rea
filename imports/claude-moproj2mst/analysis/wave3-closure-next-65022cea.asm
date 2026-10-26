Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065022CEA: 48 8B CF           mov         rcx,rdi
  0000000065022CED: FF 15 75 3D 67 00  call        qword ptr [0000000065696A68h]
  0000000065022CF3: 83 F8 0B           cmp         eax,0Bh
  0000000065022CF6: 74 0E              je          0000000065022D06
  0000000065022CF8: 48 8B CF           mov         rcx,rdi
  0000000065022CFB: FF 15 67 3D 67 00  call        qword ptr [0000000065696A68h]
  0000000065022D01: 83 F8 3E           cmp         eax,3Eh
  0000000065022D04: 75 21              jne         0000000065022D27
  0000000065022D06: 45 33 C0           xor         r8d,r8d
  0000000065022D09: 48 8B CE           mov         rcx,rsi
  0000000065022D0C: 41 8D 50 01        lea         edx,[r8+1]
  0000000065022D10: E8 CB 62 04 00     call        0000000065068FE0
  0000000065022D15: EB 10              jmp         0000000065022D27
  0000000065022D17: 41 B0 01           mov         r8b,1
  0000000065022D1A: BA 01 00 00 00     mov         edx,1
  0000000065022D1F: 48 8B CE           mov         rcx,rsi
  0000000065022D22: E8 B9 62 04 00     call        0000000065068FE0
  0000000065022D27: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  0000000065022D2B: 48 8B 88 A8 00 00  mov         rcx,qword ptr [rax+00000000000000A8h]
                    00
  0000000065022D32: 48 85 C9           test        rcx,rcx
  0000000065022D35: 74 08              je          0000000065022D3F
  0000000065022D37: 48 8B D7           mov         rdx,rdi
  0000000065022D3A: E8 81 3A 02 00     call        00000000650467C0
  0000000065022D3F: 4C 8B C7           mov         r8,rdi
  0000000065022D42: 48 8B D6           mov         rdx,rsi
  0000000065022D45: 48 8B CD           mov         rcx,rbp
  0000000065022D48: FF 15 5A 40 67 00  call        qword ptr [0000000065696DA8h]
  0000000065022D4E: 84 C0              test        al,al
  0000000065022D50: 0F 85 2C FF FF FF  jne         0000000065022C82
  0000000065022D56: 48 8B 06           mov         rax,qword ptr [rsi]
  0000000065022D59: 48 8B D7           mov         rdx,rdi
  0000000065022D5C: 48 8B CE           mov         rcx,rsi
  0000000065022D5F: FF 50 20           call        qword ptr [rax+20h]
  0000000065022D62: 48 8B 6C 24 38     mov         rbp,qword ptr [rsp+38h]
  0000000065022D67: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  0000000065022D6C: B9 FD FF 00 00     mov         ecx,0FFFDh
  0000000065022D71: 66 21 4F 12        and         word ptr [rdi+12h],cx
  0000000065022D75: 48 83 C4 20        add         rsp,20h
  0000000065022D79: 5F                 pop         rdi
  0000000065022D7A: C3                 ret

  Summary

      695000 .text
