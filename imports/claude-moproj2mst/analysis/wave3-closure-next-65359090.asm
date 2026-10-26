Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065359090: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  0000000065359095: 48 89 6C 24 18     mov         qword ptr [rsp+18h],rbp
  000000006535909A: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  000000006535909F: 57                 push        rdi
  00000000653590A0: 48 83 EC 70        sub         rsp,70h
  00000000653590A4: 48 8B E9           mov         rbp,rcx
  00000000653590A7: 48 8B CA           mov         rcx,rdx
  00000000653590AA: 49 8B F9           mov         rdi,r9
  00000000653590AD: 41 8B F0           mov         esi,r8d
  00000000653590B0: 48 8B DA           mov         rbx,rdx
  00000000653590B3: FF 15 7F D7 33 00  call        qword ptr [0000000065696838h]
  00000000653590B9: 83 FE 0B           cmp         esi,0Bh
  00000000653590BC: 0F 84 7B 01 00 00  je          000000006535923D
  00000000653590C2: 83 FE 0E           cmp         esi,0Eh
  00000000653590C5: 0F 84 1A 01 00 00  je          00000000653591E5
  00000000653590CB: 83 FE 19           cmp         esi,19h
  00000000653590CE: 0F 84 69 01 00 00  je          000000006535923D
  00000000653590D4: 83 FE 28           cmp         esi,28h
  00000000653590D7: 74 25              je          00000000653590FE
  00000000653590D9: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000000653590E1: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000653590E6: 4C 8B CF           mov         r9,rdi
  00000000653590E9: 44 8B C6           mov         r8d,esi
  00000000653590EC: 48 8B CD           mov         rcx,rbp
  00000000653590EF: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000000653590F4: E8 67 3D F6 FF     call        00000000652BCE60
  00000000653590F9: E9 53 01 00 00     jmp         0000000065359251

  Summary

      695000 .text
