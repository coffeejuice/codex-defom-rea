Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065387CD0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000065387CD5: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  0000000065387CDA: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  0000000065387CDF: 57                 push        rdi
  0000000065387CE0: 41 54              push        r12
  0000000065387CE2: 41 55              push        r13
  0000000065387CE4: 48 83 EC 20        sub         rsp,20h
  0000000065387CE8: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  0000000065387CEC: 4C 8B E9           mov         r13,rcx
  0000000065387CEF: 49 8B 84 24 E0 01  mov         rax,qword ptr [r12+00000000000001E0h]
                    00 00
  0000000065387CF7: 49 8D 8C 24 50 02  lea         rcx,[r12+0000000000000250h]
                    00 00
  0000000065387CFF: 48 8B 28           mov         rbp,qword ptr [rax]
  0000000065387D02: FF 15 48 0B 31 00  call        qword ptr [0000000065698850h]
  0000000065387D08: 49 8D 8C 24 50 02  lea         rcx,[r12+0000000000000250h]
                    00 00
  0000000065387D10: 48 8B F8           mov         rdi,rax
  0000000065387D13: FF 15 37 0B 31 00  call        qword ptr [0000000065698850h]
  0000000065387D19: 49 8B 8C 24 E0 01  mov         rcx,qword ptr [r12+00000000000001E0h]
                    00 00
  0000000065387D21: 48 8B D0           mov         rdx,rax
  0000000065387D24: FF 55 68           call        qword ptr [rbp+68h]
  0000000065387D27: 49 8B 8C 24 E0 01  mov         rcx,qword ptr [r12+00000000000001E0h]
                    00 00
  0000000065387D2F: 4C 8B CF           mov         r9,rdi
  0000000065387D32: 44 8B C0           mov         r8d,eax
  0000000065387D35: 33 D2              xor         edx,edx
  0000000065387D37: FF 95 E0 00 00 00  call        qword ptr [rbp+00000000000000E0h]
  0000000065387D3D: 33 D2              xor         edx,edx
  0000000065387D3F: 41 B8 0C 80 00 00  mov         r8d,800Ch
  0000000065387D45: 49 8B CD           mov         rcx,r13
  0000000065387D48: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  0000000065387D4D: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  0000000065387D52: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  0000000065387D57: 48 83 C4 20        add         rsp,20h
  0000000065387D5B: 41 5D              pop         r13
  0000000065387D5D: 41 5C              pop         r12
  0000000065387D5F: 5F                 pop         rdi
  0000000065387D60: E9 FB AB 11 00     jmp         00000000654A2960

  Summary

      695000 .text
