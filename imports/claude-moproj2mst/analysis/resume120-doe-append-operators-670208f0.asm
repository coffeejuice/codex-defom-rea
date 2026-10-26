Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670208F0: 66 89 54 24 10     mov         word ptr [rsp+10h],dx
  00000000670208F5: 53                 push        rbx
  00000000670208F6: 48 83 EC 20        sub         rsp,20h
  00000000670208FA: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670208FD: 48 8B D9           mov         rbx,rcx
  0000000067020900: 8B 10              mov         edx,dword ptr [rax]
  0000000067020902: 83 FA 01           cmp         edx,1
  0000000067020905: 75 0D              jne         0000000067020914
  0000000067020907: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006702090A: 8B 42 08           mov         eax,dword ptr [rdx+8]
  000000006702090D: FF C0              inc         eax
  000000006702090F: 3B 42 04           cmp         eax,dword ptr [rdx+4]
  0000000067020912: 7E 17              jle         000000006702092B
  0000000067020914: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067020917: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006702091A: FF C1              inc         ecx
  000000006702091C: E8 DF A9 04 00     call        000000006706B300
  0000000067020921: 48 8B CB           mov         rcx,rbx
  0000000067020924: 8B D0              mov         edx,eax
  0000000067020926: E8 75 F8 04 00     call        00000000670701A0
  000000006702092B: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006702092E: 48 63 50 08        movsxd      rdx,dword ptr [rax+8]
  0000000067020932: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  0000000067020936: 0F B7 44 24 38     movzx       eax,word ptr [rsp+38h]
  000000006702093B: 66 89 04 51        mov         word ptr [rcx+rdx*2],ax
  000000006702093F: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000067020942: FF 40 08           inc         dword ptr [rax+8]
  0000000067020945: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000067020948: 48 63 50 08        movsxd      rdx,dword ptr [rax+8]
  000000006702094C: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  0000000067020950: 33 C0              xor         eax,eax
  0000000067020952: 66 89 04 51        mov         word ptr [rcx+rdx*2],ax
  0000000067020956: 48 8B C3           mov         rax,rbx
  0000000067020959: 48 83 C4 20        add         rsp,20h
  000000006702095D: 5B                 pop         rbx
  000000006702095E: C3                 ret

  Summary

      1A7000 .text
