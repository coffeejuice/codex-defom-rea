Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C6DD0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670C6DD5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000670C6DDA: 57                 push        rdi
  00000000670C6DDB: 48 83 EC 20        sub         rsp,20h
  00000000670C6DDF: 8B FA              mov         edi,edx
  00000000670C6DE1: 48 8B F1           mov         rsi,rcx
  00000000670C6DE4: E8 D7 B4 06 00     call        00000000671322C0
  00000000670C6DE9: 8B D8              mov         ebx,eax
  00000000670C6DEB: 85 C0              test        eax,eax
  00000000670C6DED: 78 1E              js          00000000670C6E0D
  00000000670C6DEF: 85 FF              test        edi,edi
  00000000670C6DF1: 75 18              jne         00000000670C6E0B
  00000000670C6DF3: 83 F8 01           cmp         eax,1
  00000000670C6DF6: 7D 11              jge         00000000670C6E09
  00000000670C6DF8: 85 C0              test        eax,eax
  00000000670C6DFA: 75 0D              jne         00000000670C6E09
  00000000670C6DFC: 48 8B 4E 10        mov         rcx,qword ptr [rsi+10h]
  00000000670C6E00: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000670C6E04: E8 47 C1 FF FF     call        00000000670C2F50
  00000000670C6E09: FF CB              dec         ebx
  00000000670C6E0B: 8B C3              mov         eax,ebx
  00000000670C6E0D: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000670C6E12: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  00000000670C6E17: 48 83 C4 20        add         rsp,20h
  00000000670C6E1B: 5F                 pop         rdi
  00000000670C6E1C: C3                 ret

  Summary

      1A7000 .text
