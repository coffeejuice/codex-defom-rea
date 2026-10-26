Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006506F080: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006506F085: 57                 push        rdi
  000000006506F086: 48 83 EC 20        sub         rsp,20h
  000000006506F08A: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006506F08E: 84 D2              test        dl,dl
  000000006506F090: 0F B6 FA           movzx       edi,dl
  000000006506F093: 41 0F 94 C0        sete        r8b
  000000006506F097: BA 20 00 00 00     mov         edx,20h
  000000006506F09C: E8 3F 9F FF FF     call        0000000065068FE0
  000000006506F0A1: 40 0F B6 D7        movzx       edx,dil
  000000006506F0A5: 48 8B CB           mov         rcx,rbx
  000000006506F0A8: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006506F0AD: 48 83 C4 20        add         rsp,20h
  000000006506F0B1: 5F                 pop         rdi
  000000006506F0B2: E9 49 AE FF FF     jmp         0000000065069F00

  Summary

      695000 .text
