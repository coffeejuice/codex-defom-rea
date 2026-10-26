Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065022C60: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  0000000065022C65: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  0000000065022C6A: 57                 push        rdi
  0000000065022C6B: 48 83 EC 20        sub         rsp,20h
  0000000065022C6F: 49 8B F8           mov         rdi,r8
  0000000065022C72: 48 8B F2           mov         rsi,rdx
  0000000065022C75: 48 8B E9           mov         rbp,rcx
  0000000065022C78: FF 15 32 41 67 00  call        qword ptr [0000000065696DB0h]
  0000000065022C7E: 84 C0              test        al,al
  0000000065022C80: 74 12              je          0000000065022C94
  0000000065022C82: B0 01              mov         al,1
  0000000065022C84: 48 8B 6C 24 38     mov         rbp,qword ptr [rsp+38h]
  0000000065022C89: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  0000000065022C8E: 48 83 C4 20        add         rsp,20h
  0000000065022C92: 5F                 pop         rdi
  0000000065022C93: C3                 ret
  0000000065022C94: 48 8B CE           mov         rcx,rsi
  0000000065022C97: FF 15 BB 3B 67 00  call        qword ptr [0000000065696858h]
  0000000065022C9D: 84 C0              test        al,al
  0000000065022C9F: 0F 84 9A 00 00 00  je          0000000065022D3F
  0000000065022CA5: 48 8B CF           mov         rcx,rdi
  0000000065022CA8: FF 15 BA 3D 67 00  call        qword ptr [0000000065696A68h]
  0000000065022CAE: 83 F8 0A           cmp         eax,0Ah
  0000000065022CB1: 74 0E              je          0000000065022CC1
  0000000065022CB3: 48 8B CF           mov         rcx,rdi
  0000000065022CB6: FF 15 AC 3D 67 00  call        qword ptr [0000000065696A68h]
  0000000065022CBC: 83 F8 3C           cmp         eax,3Ch
  0000000065022CBF: 75 29              jne         0000000065022CEA
  0000000065022CC1: E8 5A D9 FF FF     call        0000000065020620
  0000000065022CC6: 48 85 C0           test        rax,rax
  0000000065022CC9: 74 4C              je          0000000065022D17
  0000000065022CCB: 48 8B CE           mov         rcx,rsi

  Summary

      695000 .text
