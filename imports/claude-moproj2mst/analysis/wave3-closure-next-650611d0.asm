Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650611D0: 40 57              push        rdi
  00000000650611D2: 48 83 EC 50        sub         rsp,50h
  00000000650611D6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650611DF: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  00000000650611E4: 48 8B F9           mov         rdi,rcx
  00000000650611E7: 44 8B 81 28 01 00  mov         r8d,dword ptr [rcx+0000000000000128h]
                    00
  00000000650611EE: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000000650611F3: E8 58 E1 FF FF     call        000000006505F350
  00000000650611F8: 90                 nop
  00000000650611F9: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  00000000650611FE: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065061203: 48 8D 8F 80 01 00  lea         rcx,[rdi+0000000000000180h]
                    00
  000000006506120A: E8 91 2A 19 00     call        00000000651F3CA0
  000000006506120F: 90                 nop
  0000000065061210: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065061215: 48 8D 8F 80 01 00  lea         rcx,[rdi+0000000000000180h]
                    00
  000000006506121C: E8 1F 13 19 00     call        00000000651F2540
  0000000065061221: 84 C0              test        al,al
  0000000065061223: 74 0C              je          0000000065061231
  0000000065061225: 8B 87 88 01 00 00  mov         eax,dword ptr [rdi+0000000000000188h]
  000000006506122B: 3B 44 24 30        cmp         eax,dword ptr [rsp+30h]
  000000006506122F: 74 0E              je          000000006506123F
  0000000065061231: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065061236: 48 8B CF           mov         rcx,rdi
  0000000065061239: E8 D2 E2 FF FF     call        000000006505F510
  000000006506123E: 90                 nop
  000000006506123F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065061244: E8 D7 58 1A 00     call        0000000065206B20
  0000000065061249: 90                 nop
  000000006506124A: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006506124F: E8 CC 58 1A 00     call        0000000065206B20
  0000000065061254: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  0000000065061259: 48 83 C4 50        add         rsp,50h
  000000006506125D: 5F                 pop         rdi
  000000006506125E: C3                 ret

  Summary

      695000 .text
