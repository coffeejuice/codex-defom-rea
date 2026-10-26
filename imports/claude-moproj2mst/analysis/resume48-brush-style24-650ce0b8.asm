Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650CE0B8: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  00000000650CE0BD: 48 85 C9           test        rcx,rcx
  00000000650CE0C0: 74 0A              je          00000000650CE0CC
  00000000650CE0C2: 48 83 C1 18        add         rcx,18h
  00000000650CE0C6: FF 15 F4 87 5C 00  call        qword ptr [00000000656968C0h]
  00000000650CE0CC: 48 8B 3B           mov         rdi,qword ptr [rbx]
  00000000650CE0CF: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000650CE0D2: 48 89 03           mov         qword ptr [rbx],rax
  00000000650CE0D5: 48 85 FF           test        rdi,rdi
  00000000650CE0D8: 74 1B              je          00000000650CE0F5
  00000000650CE0DA: 48 8D 4F 18        lea         rcx,[rdi+18h]
  00000000650CE0DE: FF 15 94 88 5C 00  call        qword ptr [0000000065696978h]
  00000000650CE0E4: 84 C0              test        al,al
  00000000650CE0E6: 75 0D              jne         00000000650CE0F5
  00000000650CE0E8: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000650CE0EB: BA 01 00 00 00     mov         edx,1
  00000000650CE0F0: 48 8B CF           mov         rcx,rdi
  00000000650CE0F3: FF 10              call        qword ptr [rax]
  00000000650CE0F5: 48 8B C3           mov         rax,rbx
  00000000650CE0F8: 48 8B 7C 24 30     mov         rdi,qword ptr [rsp+30h]
  00000000650CE0FD: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000650CE102: 48 83 C4 20        add         rsp,20h
  00000000650CE106: 5E                 pop         rsi
  00000000650CE107: C3                 ret
  00000000650CE108: 48 8B C3           mov         rax,rbx
  00000000650CE10B: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000650CE110: 48 83 C4 20        add         rsp,20h
  00000000650CE114: 5E                 pop         rsi
  00000000650CE115: C3                 ret

  Summary

      695000 .text
