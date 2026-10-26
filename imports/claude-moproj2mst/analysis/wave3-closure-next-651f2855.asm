Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000651F2855: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  00000000651F285A: 48 85 D2           test        rdx,rdx
  00000000651F285D: 74 09              je          00000000651F2868
  00000000651F285F: 48 8B CA           mov         rcx,rdx
  00000000651F2862: FF 15 58 40 4A 00  call        qword ptr [00000000656968C0h]
  00000000651F2868: 48 8B 33           mov         rsi,qword ptr [rbx]
  00000000651F286B: 48 89 3B           mov         qword ptr [rbx],rdi
  00000000651F286E: 48 85 F6           test        rsi,rsi
  00000000651F2871: 74 1D              je          00000000651F2890
  00000000651F2873: 48 8B CE           mov         rcx,rsi
  00000000651F2876: FF 15 FC 40 4A 00  call        qword ptr [0000000065696978h]
  00000000651F287C: 84 C0              test        al,al
  00000000651F287E: 75 10              jne         00000000651F2890
  00000000651F2880: 48 8B CE           mov         rcx,rsi
  00000000651F2883: E8 B8 F7 FF FF     call        00000000651F2040
  00000000651F2888: 48 8B CE           mov         rcx,rsi
  00000000651F288B: E8 1E E8 3E 00     call        00000000655E10AE
  00000000651F2890: 48 8B C3           mov         rax,rbx
  00000000651F2893: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  00000000651F2898: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000651F289D: 48 83 C4 20        add         rsp,20h
  00000000651F28A1: 5F                 pop         rdi
  00000000651F28A2: C3                 ret
  00000000651F28A3: 48 8B C1           mov         rax,rcx
  00000000651F28A6: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000651F28AB: 48 83 C4 20        add         rsp,20h
  00000000651F28AF: 5F                 pop         rdi

  Summary

      695000 .text
