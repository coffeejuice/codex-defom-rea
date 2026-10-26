Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006706E4B0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006706E4B5: 57                 push        rdi
  000000006706E4B6: 48 83 EC 20        sub         rsp,20h
  000000006706E4BA: 48 8B DA           mov         rbx,rdx
  000000006706E4BD: 48 8B F9           mov         rdi,rcx
  000000006706E4C0: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  000000006706E4C8: E8 A3 FA 0D 00     call        000000006714DF70
  000000006706E4CD: 48 85 C0           test        rax,rax
  000000006706E4D0: 74 21              je          000000006706E4F3
  000000006706E4D2: E8 99 FA 0D 00     call        000000006714DF70
  000000006706E4D7: 4C 8B C7           mov         r8,rdi
  000000006706E4DA: 48 8B D3           mov         rdx,rbx
  000000006706E4DD: 48 8B C8           mov         rcx,rax
  000000006706E4E0: E8 4B C8 0D 00     call        000000006714AD30
  000000006706E4E5: 48 8B C3           mov         rax,rbx
  000000006706E4E8: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006706E4ED: 48 83 C4 20        add         rsp,20h
  000000006706E4F1: 5F                 pop         rdi
  000000006706E4F2: C3                 ret
  000000006706E4F3: 48 8B D3           mov         rdx,rbx
  000000006706E4F6: 48 8B CF           mov         rcx,rdi
  000000006706E4F9: E8 42 FF FF FF     call        000000006706E440
  000000006706E4FE: 48 8B C3           mov         rax,rbx
  000000006706E501: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006706E506: 48 83 C4 20        add         rsp,20h
  000000006706E50A: 5F                 pop         rdi
  000000006706E50B: C3                 ret
  000000006706E50C: CC                 int         3
  000000006706E50D: CC                 int         3
  000000006706E50E: CC                 int         3
  000000006706E50F: CC                 int         3
  000000006706E510: 40 53              push        rbx
  000000006706E512: 48 83 EC 20        sub         rsp,20h
  000000006706E516: 48 8B DA           mov         rbx,rdx
  000000006706E519: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006706E51C: 48 8D 05 0D 26 28  lea         rax,[00000000672F0B30h]
                    00
  000000006706E523: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  000000006706E52B: 48 3B D0           cmp         rdx,rax
  000000006706E52E: 75 1B              jne         000000006706E54B
  000000006706E530: 48 8D 05 C9 23 28  lea         rax,[00000000672F0900h]
                    00
  000000006706E537: 48 89 03           mov         qword ptr [rbx],rax
  000000006706E53A: F0 83 05 BE 23 28  lock add    dword ptr [00000000672F0900h],1
                    00 01
  000000006706E542: 48 8B C3           mov         rax,rbx
  000000006706E545: 48 83 C4 20        add         rsp,20h
  000000006706E549: 5B                 pop         rbx
  000000006706E54A: C3                 ret
  000000006706E54B: 44 8B 42 08        mov         r8d,dword ptr [rdx+8]
  000000006706E54F: 48

  Summary

      1A7000 .text
