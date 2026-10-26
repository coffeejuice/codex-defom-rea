Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006705349A: 48 8B 74 24 28     mov         rsi,qword ptr [rsp+28h]
  000000006705349F: 4D 85 FF           test        r15,r15
  00000000670534A2: 74 17              je          00000000670534BB
  00000000670534A4: 41 C6 07 00        mov         byte ptr [r15],0
  00000000670534A8: EB 11              jmp         00000000670534BB
  00000000670534AA: 8B 8C 24 90 00 00  mov         ecx,dword ptr [rsp+0000000000000090h]
                    00
  00000000670534B1: 85 C9              test        ecx,ecx
  00000000670534B3: 74 06              je          00000000670534BB
  00000000670534B5: 48 F7 D6           not         rsi
  00000000670534B8: 48 FF C6           inc         rsi
  00000000670534BB: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670534C3: 4C 8B 74 24 38     mov         r14,qword ptr [rsp+38h]
  00000000670534C8: 4C 8B 6C 24 40     mov         r13,qword ptr [rsp+40h]
  00000000670534CD: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670534D2: 48 85 C0           test        rax,rax
  00000000670534D5: 74 14              je          00000000670534EB

  Summary

      1A7000 .text
