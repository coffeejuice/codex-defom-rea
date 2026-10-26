Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006550CFF0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006550CFF5: 57                 push        rdi
  000000006550CFF6: 48 83 EC 20        sub         rsp,20h
  000000006550CFFA: 8B FA              mov         edi,edx
  000000006550CFFC: 48 8B 51 08        mov         rdx,qword ptr [rcx+8]
  000000006550D000: 49 8B D8           mov         rbx,r8
  000000006550D003: 4D 85 C0           test        r8,r8
  000000006550D006: 74 2F              je          000000006550D037
  000000006550D008: 49 89 48 18        mov         qword ptr [r8+18h],rcx
  000000006550D00C: 48 8B 92 80 02 00  mov         rdx,qword ptr [rdx+0000000000000280h]
                    00
  000000006550D013: 48 8D 0D AE C3 4B  lea         rcx,[00000000659C93C8h]
                    00
  000000006550D01A: FF 15 E8 99 18 00  call        qword ptr [0000000065696A08h]
  000000006550D020: 4C 8B C3           mov         r8,rbx
  000000006550D023: 8B D7              mov         edx,edi
  000000006550D025: 48 8B C8           mov         rcx,rax
  000000006550D028: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006550D02D: 48 83 C4 20        add         rsp,20h
  000000006550D031: 5F                 pop         rdi
  000000006550D032: E9 E9 CF FF FF     jmp         000000006550A020
  000000006550D037: 8B D7              mov         edx,edi
  000000006550D039: E8 F2 DE FF FF     call        000000006550AF30
  000000006550D03E: 48 85 C0           test        rax,rax
  000000006550D041: 74 0E              je          000000006550D051
  000000006550D043: 4C 8B 00           mov         r8,qword ptr [rax]
  000000006550D046: BA 01 00 00 00     mov         edx,1
  000000006550D04B: 48 8B C8           mov         rcx,rax
  000000006550D04E: 41 FF 10           call        qword ptr [r8]
  000000006550D051: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006550D056: 48 83 C4 20        add         rsp,20h
  000000006550D05A: 5F                 pop         rdi
  000000006550D05B: C3                 ret

  Summary

      695000 .text
