Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000655878C0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000655878C5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000655878CA: 57                 push        rdi
  00000000655878CB: 48 83 EC 20        sub         rsp,20h
  00000000655878CF: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000655878D3: 48 8B DA           mov         rbx,rdx
  00000000655878D6: 48 8B F1           mov         rsi,rcx
  00000000655878D9: F6 87 88 02 00 00  test        byte ptr [rdi+0000000000000288h],1
                    01
  00000000655878E0: 0F 84 86 00 00 00  je          000000006558796C
  00000000655878E6: 48 8B CA           mov         rcx,rdx
  00000000655878E9: FF 15 79 F1 10 00  call        qword ptr [0000000065696A68h]
  00000000655878EF: 83 F8 06           cmp         eax,6
  00000000655878F2: 74 2A              je          000000006558791E
  00000000655878F4: 83 F8 33           cmp         eax,33h
  00000000655878F7: 75 73              jne         000000006558796C
  00000000655878F9: 48 8B 8F F0 03 00  mov         rcx,qword ptr [rdi+00000000000003F0h]
                    00
  0000000065587900: 48 85 C9           test        rcx,rcx
  0000000065587903: 74 67              je          000000006558796C
  0000000065587905: 48 8B D3           mov         rdx,rbx
  0000000065587908: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006558790D: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000065587912: 48 83 C4 20        add         rsp,20h
  0000000065587916: 5F                 pop         rdi
  0000000065587917: 48 FF 25 F2 EE 10  jmp         qword ptr [0000000065696810h]
                    00
  000000006558791E: 48 8B 8F F0 03 00  mov         rcx,qword ptr [rdi+00000000000003F0h]
                    00
  0000000065587925: 48 85 C9           test        rcx,rcx
  0000000065587928: 74 42              je          000000006558796C
  000000006558792A: 8B 43 28           mov         eax,dword ptr [rbx+28h]
  000000006558792D: 3D 01 00 00 01     cmp         eax,1000001h
  0000000065587932: 74 07              je          000000006558793B
  0000000065587934: 3D 02 00 00 01     cmp         eax,1000002h
  0000000065587939: 75 31              jne         000000006558796C
  000000006558793B: 48 8B D3           mov         rdx,rbx
  000000006558793E: FF 15 CC EE 10 00  call        qword ptr [0000000065696810h]
  0000000065587944: 48 8B CB           mov         rcx,rbx
  0000000065587947: FF 15 43 F4 10 00  call        qword ptr [0000000065696D90h]
  000000006558794D: 84 C0              test        al,al
  000000006558794F: 74 12              je          0000000065587963
  0000000065587951: B0 01              mov         al,1
  0000000065587953: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065587958: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000006558795D: 48 83 C4 20        add         rsp,20h
  0000000065587961: 5F                 pop         rdi
  0000000065587962: C3                 ret
  0000000065587963: 48 8B CB           mov         rcx,rbx
  0000000065587966: FF 15 34 F4 10 00  call        qword ptr [0000000065696DA0h]
  000000006558796C: 48 8B D3           mov         rdx,rbx
  000000006558796F: 48 8B CE           mov         rcx,rsi
  0000000065587972: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065587977: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000006558797C: 48 83 C4 20        add         rsp,20h
  0000000065587980: 5F                 pop         rdi
  0000000065587981: E9 2A 0E EA FF     jmp         00000000654287B0

  Summary

      695000 .text
