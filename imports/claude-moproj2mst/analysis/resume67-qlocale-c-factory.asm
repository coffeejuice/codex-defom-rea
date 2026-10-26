Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067001130: 40 53              push        rbx
  0000000067001132: 48 83 EC 20        sub         rsp,20h
  0000000067001136: 45 33 C0           xor         r8d,r8d
  0000000067001139: 48 8B D9           mov         rbx,rcx
  000000006700113C: 41 8D 50 01        lea         edx,[r8+1]
  0000000067001140: E8 5B C4 04 00     call        000000006704D5A0
  0000000067001145: 48 8B C3           mov         rax,rbx
  0000000067001148: 48 83 C4 20        add         rsp,20h
  000000006700114C: 5B                 pop         rbx
  000000006700114D: C3                 ret
  000000006700114E: CC                 int         3
  000000006700114F: CC                 int         3
  0000000067001150: E9 3B 18 0C 00     jmp         00000000670C2990
  0000000067001155: CC                 int         3
  0000000067001156: CC                 int         3
  0000000067001157: CC                 int         3
  0000000067001158: CC                 int         3
  0000000067001159: CC                 int         3
  000000006700115A: CC                 int         3
  000000006700115B: CC                 int         3
  000000006700115C: CC                 int         3
  000000006700115D: CC                 int         3
  000000006700115E: CC                 int         3
  000000006700115F: CC                 int         3
  0000000067001160: E9 CB 18 0C 00     jmp         00000000670C2A30
  0000000067001165: CC                 int         3
  0000000067001166: CC                 int         3
  0000000067001167: CC                 int         3
  0000000067001168: CC                 int         3
  0000000067001169: CC                 int         3
  000000006700116A: CC                 int         3
  000000006700116B: CC                 int         3
  000000006700116C: CC                 int         3
  000000006700116D: CC                 int         3
  000000006700116E: CC                 int         3
  000000006700116F: CC                 int         3
  0000000067001170: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000067001175: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  000000006700117A: 57                 push        rdi
  000000006700117B: 48 83 EC 20        sub         rsp,20h
  000000006700117F: 8B FA              mov         edi,edx
  0000000067001181: 48 8B F1           mov         rsi,rcx
  0000000067001184: E8 07 18 0C 00     call        00000000670C2990
  0000000067001189: 48 8B CE           mov         rcx,rsi
  000000006700118C: 8B D8              mov         ebx,eax
  000000006700118E: E8 FD 17 0C 00     call        00000000670C2990
  0000000067001193: 48 8B CE           mov         rcx,rsi
  0000000067001196: 0B C7              or          eax,edi
  0000000067001198: 8B D0              mov         edx,eax
  000000006700119A: E8 91 18 0C 00     call        00000000670C2A30
  000000006700119F: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  00000000670011A4: 8B C3              mov         eax,ebx
  00000000670011A6: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000670011AB: 48 83 C4 20        add         rsp,20h
  00000000670011AF: 5F                 pop         rdi
  00000000670011B0: C3                 ret

  Summary

      1A7000 .text
