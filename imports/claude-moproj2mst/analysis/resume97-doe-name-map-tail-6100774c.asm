Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  000000006100774C: 48 89 5C 24 30     mov         qword ptr [rsp+30h],rbx
  0000000061007751: 48 89 74 24 38     mov         qword ptr [rsp+38h],rsi
  0000000061007756: FF 15 D4 BA 03 00  call        qword ptr [0000000061043230h]
  000000006100775C: 4C 8B 0F           mov         r9,qword ptr [rdi]
  000000006100775F: 41 8B 49 20        mov         ecx,dword ptr [r9+20h]
  0000000061007763: 8B F0              mov         esi,eax
  0000000061007765: 85 C9              test        ecx,ecx
  0000000061007767: 74 3A              je          00000000610077A3
  0000000061007769: 33 D2              xor         edx,edx
  000000006100776B: F7 F1              div         eax,ecx
  000000006100776D: 49 8B 49 08        mov         rcx,qword ptr [r9+8]
  0000000061007771: 48 8D 1C D1        lea         rbx,[rcx+rdx*8]
  0000000061007775: 48 8B 0C D1        mov         rcx,qword ptr [rcx+rdx*8]
  0000000061007779: 49 3B C9           cmp         rcx,r9
  000000006100777C: 74 28              je          00000000610077A6
  000000006100777E: 66 90              nop
  0000000061007780: 3B 71 08           cmp         esi,dword ptr [rcx+8]
  0000000061007783: 75 11              jne         0000000061007796
  0000000061007785: 48 8D 51 10        lea         rdx,[rcx+10h]
  0000000061007789: 48 8B CD           mov         rcx,rbp
  000000006100778C: FF 15 7E C0 03 00  call        qword ptr [0000000061043810h]
  0000000061007792: 84 C0              test        al,al
  0000000061007794: 75 10              jne         00000000610077A6
  0000000061007796: 48 8B 1B           mov         rbx,qword ptr [rbx]
  0000000061007799: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006100779C: 48 3B 0F           cmp         rcx,qword ptr [rdi]
  000000006100779F: 75 DF              jne         0000000061007780
  00000000610077A1: EB 03              jmp         00000000610077A6
  00000000610077A3: 48 8B DF           mov         rbx,rdi
  00000000610077A6: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000610077A9: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  00000000610077AE: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000610077B3: 48 3B 07           cmp         rax,qword ptr [rdi]
  00000000610077B6: 74 0F              je          00000000610077C7

  Summary

       42000 .text
