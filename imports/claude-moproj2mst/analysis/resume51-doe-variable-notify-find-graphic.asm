Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140171190: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140171195: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014017119A: 48 83 EC 28        sub         rsp,28h
  000000014017119E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001401711A3: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001401711A7: 48 83 C1 20        add         rcx,20h
  00000001401711AB: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001401711B0: E8 3B C6 C2 00     call        0000000140D9D7F0
  00000001401711B5: 0F B6 C0           movzx       eax,al
  00000001401711B8: 85 C0              test        eax,eax
  00000001401711BA: 74 1C              je          00000001401711D8
  00000001401711BC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001401711C1: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001401711C5: 48 83 C1 20        add         rcx,20h
  00000001401711C9: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001401711CE: E8 FD 75 25 01     call        00000001413C87D0
  00000001401711D3: 48 8B 00           mov         rax,qword ptr [rax]
  00000001401711D6: EB 02              jmp         00000001401711DA
  00000001401711D8: 33 C0              xor         eax,eax
  00000001401711DA: 48 83 C4 28        add         rsp,28h
  00000001401711DE: C3                 ret

  Summary

     1E02000 .text
