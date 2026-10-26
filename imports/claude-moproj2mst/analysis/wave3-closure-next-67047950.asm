Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067047950: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067047953: 33 D2              xor         edx,edx
  0000000067047955: 44 8B 40 08        mov         r8d,dword ptr [rax+8]
  0000000067047959: 4C 8B 48 10        mov         r9,qword ptr [rax+10h]
  000000006704795D: 45 85 C0           test        r8d,r8d
  0000000067047960: 74 25              je          0000000067047987
  0000000067047962: 41 0F B6 01        movzx       eax,byte ptr [r9]
  0000000067047966: C1 E2 04           shl         edx,4
  0000000067047969: 49 FF C1           inc         r9
  000000006704796C: 03 D0              add         edx,eax
  000000006704796E: 8B CA              mov         ecx,edx
  0000000067047970: C1 EA 17           shr         edx,17h
  0000000067047973: 81 E2 E0 01 00 00  and         edx,1E0h
  0000000067047979: 33 D1              xor         edx,ecx
  000000006704797B: 81 E2 FF FF FF 0F  and         edx,0FFFFFFFh
  0000000067047981: 41 83 E8 01        sub         r8d,1
  0000000067047985: 75 DB              jne         0000000067047962
  0000000067047987: 8B C2              mov         eax,edx
  0000000067047989: C3                 ret
  000000006704798A: CC                 int         3
  000000006704798B: CC                 int         3
  000000006704798C: CC                 int         3
  000000006704798D: CC                 int         3
  000000006704798E: CC                 int         3
  000000006704798F: CC                 int         3
  0000000067047990: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067047993: 33 D2              xor         edx,edx
  0000000067047995: 44 8B 40 08        mov         r8d,dword ptr [rax+8]
  0000000067047999: 4C 8B 48 10        mov         r9,qword ptr [rax+10h]
  000000006704799D: 45 85 C0           test        r8d,r8d
  00000000670479A0: 74 34              je          00000000670479D6
  00000000670479A2: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    0F 1F 84 00 00 00
                    00 00

  Summary

      1A7000 .text
