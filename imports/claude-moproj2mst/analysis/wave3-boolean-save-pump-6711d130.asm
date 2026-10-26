Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711D130: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006711D135: 57                 push        rdi
  000000006711D136: 48 83 EC 20        sub         rsp,20h
  000000006711D13A: 48 8B D9           mov         rbx,rcx
  000000006711D13D: E8 5E DB F0 FF     call        000000006702ACA0
  000000006711D142: 48 83 78 20 00     cmp         qword ptr [rax+20h],0
  000000006711D147: 48 8B F8           mov         rdi,rax
  000000006711D14A: 74 2E              je          000000006711D17A
  000000006711D14C: F6 03 10           test        byte ptr [rbx],10h
  000000006711D14F: 74 14              je          000000006711D165
  000000006711D151: E8 4A DB F0 FF     call        000000006702ACA0
  000000006711D156: BA 34 00 00 00     mov         edx,34h
  000000006711D15B: 33 C9              xor         ecx,ecx
  000000006711D15D: 4C 8B C0           mov         r8,rax
  000000006711D160: E8 DB DE FF FF     call        000000006711B040
  000000006711D165: 8B 03              mov         eax,dword ptr [rbx]
  000000006711D167: 48 8B 4F 20        mov         rcx,qword ptr [rdi+20h]
  000000006711D16B: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006711D170: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  000000006711D174: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006711D177: FF 50 58           call        qword ptr [rax+58h]
  000000006711D17A: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006711D17F: 48 83 C4 20        add         rsp,20h
  000000006711D183: 5F                 pop         rdi
  000000006711D184: C3                 ret

  Summary

      1A7000 .text
