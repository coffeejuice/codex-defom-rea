Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B58B0: 40 53              push        rbx
  00000000670B58B2: 48 83 EC 20        sub         rsp,20h
  00000000670B58B6: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670B58B9: 48 8B D9           mov         rbx,rcx
  00000000670B58BC: 8B 10              mov         edx,dword ptr [rax]
  00000000670B58BE: 83 FA 01           cmp         edx,1
  00000000670B58C1: 74 0B              je          00000000670B58CE
  00000000670B58C3: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670B58C6: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B58C9: E8 92 C0 F7 FF     call        0000000067031960
  00000000670B58CE: 48 8B 13           mov         rdx,qword ptr [rbx]
  00000000670B58D1: 48 63 42 0C        movsxd      rax,dword ptr [rdx+0Ch]
  00000000670B58D5: 48 8D 4C C2 18     lea         rcx,[rdx+rax*8+18h]
  00000000670B58DA: 8B 02              mov         eax,dword ptr [rdx]
  00000000670B58DC: 48 89 4C 24 30     mov         qword ptr [rsp+30h],rcx
  00000000670B58E1: 83 F8 01           cmp         eax,1
  00000000670B58E4: 74 0E              je          00000000670B58F4
  00000000670B58E6: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B58E9: 48 8B CB           mov         rcx,rbx
  00000000670B58EC: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B58EF: E8 6C C0 F7 FF     call        0000000067031960
  00000000670B58F4: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670B58F7: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  00000000670B58FC: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  00000000670B5901: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  00000000670B5905: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670B590A: 48 8D 4C C1 20     lea         rcx,[rcx+rax*8+20h]
  00000000670B590F: 48 89 4C 24 38     mov         qword ptr [rsp+38h],rcx
  00000000670B5914: 48 8B CB           mov         rcx,rbx
  00000000670B5917: E8 B4 FE FF FF     call        00000000670B57D0
  00000000670B591C: 4C 8B 1B           mov         r11,qword ptr [rbx]
  00000000670B591F: 41 8B 03           mov         eax,dword ptr [r11]
  00000000670B5922: 83 F8 01           cmp         eax,1
  00000000670B5925: 74 0E              je          00000000670B5935
  00000000670B5927: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B592A: 48 8B CB           mov         rcx,rbx
  00000000670B592D: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B5930: E8 2B C0 F7 FF     call        0000000067031960
  00000000670B5935: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670B5938: 33 D2              xor         edx,edx
  00000000670B593A: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  00000000670B593E: 48 8D 4C C1 18     lea         rcx,[rcx+rax*8+18h]
  00000000670B5943: E8 B8 92 F7 FF     call        000000006702EC00
  00000000670B5948: 4C 8B 1B           mov         r11,qword ptr [rbx]
  00000000670B594B: 41 8B 03           mov         eax,dword ptr [r11]
  00000000670B594E: 83 F8 01           cmp         eax,1
  00000000670B5951: 74 0E              je          00000000670B5961
  00000000670B5953: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B5956: 48 8B CB           mov         rcx,rbx
  00000000670B5959: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B595C: E8 FF BF F7 FF     call        0000000067031960
  00000000670B5961: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670B5964: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  00000000670B5968: 48 8D 4C C1 18     lea         rcx,[rcx+rax*8+18h]
  00000000670B596D: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670B5970: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000670B5973: 3B 50 04           cmp         edx,dword ptr [rax+4]
  00000000670B5976: 7D 05              jge         00000000670B597D
  00000000670B5978: E8 93 7D F7 FF     call        000000006702D710
  00000000670B597D: 33 C0              xor         eax,eax
  00000000670B597F: 89 43 0C           mov         dword ptr [rbx+0Ch],eax
  00000000670B5982: 89 43 08           mov         dword ptr [rbx+8],eax
  00000000670B5985: 89 43 10           mov         dword ptr [rbx+10h],eax
  00000000670B5988: 89 43 18           mov         dword ptr [rbx+18h],eax
  00000000670B598B: 48 83 C4 20        add         rsp,20h
  00000000670B598F: 5B                 pop         rbx
  00000000670B5990: C3                 ret

  Summary

      1A7000 .text
