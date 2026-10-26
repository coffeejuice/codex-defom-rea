Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670EF1F0: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  00000000670EF1F5: E8 D6 FB FF FF     call        00000000670EEDD0
  00000000670EF1FA: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670EF1FD: 41 83 CB FF        or          r11d,0FFFFFFFFh
  00000000670EF201: 66 44 89 5B 10     mov         word ptr [rbx+10h],r11w
  00000000670EF206: 8B 78 08           mov         edi,dword ptr [rax+8]
  00000000670EF209: 83 EF 01           sub         edi,1
  00000000670EF20C: 78 46              js          00000000670EF254
  00000000670EF20E: 66 90              nop
  00000000670EF210: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670EF213: 8B 08              mov         ecx,dword ptr [rax]
  00000000670EF215: 83 F9 01           cmp         ecx,1
  00000000670EF218: 75 0D              jne         00000000670EF227
  00000000670EF21A: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670EF21D: 48 8D 41 1A        lea         rax,[rcx+1Ah]
  00000000670EF221: 48 39 41 10        cmp         qword ptr [rcx+10h],rax
  00000000670EF225: 74 08              je          00000000670EF22F
  00000000670EF227: 48 8B CB           mov         rcx,rbx
  00000000670EF22A: E8 91 11 F8 FF     call        00000000670703C0
  00000000670EF22F: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670EF232: 48 63 D7           movsxd      rdx,edi
  00000000670EF235: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000000670EF239: 66 83 3C 51 2F     cmp         word ptr [rcx+rdx*2],2Fh
  00000000670EF23E: 74 10              je          00000000670EF250
  00000000670EF240: 83 EF 01           sub         edi,1
  00000000670EF243: 79 CB              jns         00000000670EF210
  00000000670EF245: 48 8B 7C 24 30     mov         rdi,qword ptr [rsp+30h]
  00000000670EF24A: 48 83 C4 20        add         rsp,20h
  00000000670EF24E: 5B                 pop         rbx
  00000000670EF24F: C3                 ret

  Summary

      1A7000 .text
