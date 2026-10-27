Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141471B7C: 48 85 C9           test        rcx,rcx
  0000000141471B7F: 74 37              je          0000000141471BB8
  0000000141471B81: 53                 push        rbx
  0000000141471B82: 48 83 EC 20        sub         rsp,20h
  0000000141471B86: 4C 8B C1           mov         r8,rcx
  0000000141471B89: 48 8B 0D 80 EC 28  mov         rcx,qword ptr [0000000144700810h]
                    03
  0000000141471B90: 33 D2              xor         edx,edx
  0000000141471B92: FF 15 60 17 99 00  call        qword ptr [0000000141E032F8h]
  0000000141471B98: 85 C0              test        eax,eax
  0000000141471B9A: 75 17              jne         0000000141471BB3
  0000000141471B9C: E8 93 25 00 00     call        0000000141474134
  0000000141471BA1: 48 8B D8           mov         rbx,rax
  0000000141471BA4: FF 15 3E 1A 99 00  call        qword ptr [0000000141E035E8h]
  0000000141471BAA: 8B C8              mov         ecx,eax
  0000000141471BAC: E8 3B 25 00 00     call        00000001414740EC
  0000000141471BB1: 89 03              mov         dword ptr [rbx],eax
  0000000141471BB3: 48 83 C4 20        add         rsp,20h
  0000000141471BB7: 5B                 pop         rbx
  0000000141471BB8: C3                 ret

  Summary

     1E02000 .text
