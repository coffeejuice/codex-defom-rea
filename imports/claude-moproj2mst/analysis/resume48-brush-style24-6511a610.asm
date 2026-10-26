Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006511A610: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000006511A615: 57                 push        rdi
  000000006511A616: 48 83 EC 40        sub         rsp,40h
  000000006511A61A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006511A623: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  000000006511A628: 48 8B FA           mov         rdi,rdx
  000000006511A62B: 33 DB              xor         ebx,ebx
  000000006511A62D: 89 5C 24 50        mov         dword ptr [rsp+50h],ebx
  000000006511A631: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000006511A634: 83 79 04 18        cmp         dword ptr [rcx+4],18h
  000000006511A638: 75 07              jne         000000006511A641
  000000006511A63A: E8 61 FB FF FF     call        000000006511A1A0
  000000006511A63F: EB 14              jmp         000000006511A655
  000000006511A641: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006511A646: E8 65 E8 F8 FF     call        00000000650A8EB0
  000000006511A64B: 90                 nop
  000000006511A64C: BB 01 00 00 00     mov         ebx,1
  000000006511A651: 89 5C 24 50        mov         dword ptr [rsp+50h],ebx
  000000006511A655: 48 8B D0           mov         rdx,rax
  000000006511A658: 48 8B CF           mov         rcx,rdi
  000000006511A65B: E8 10 F4 F9 FF     call        00000000650B9A70
  000000006511A660: 83 CB 02           or          ebx,2
  000000006511A663: F6 C3 01           test        bl,1
  000000006511A666: 74 16              je          000000006511A67E
  000000006511A668: 83 E3 FE           and         ebx,0FFFFFFFEh
  000000006511A66B: 89 5C 24 50        mov         dword ptr [rsp+50h],ebx
  000000006511A66F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006511A674: E8 87 8C F9 FF     call        00000000650B3300
  000000006511A679: 48 8B C7           mov         rax,rdi
  000000006511A67C: EB 03              jmp         000000006511A681
  000000006511A67E: 48 8B C7           mov         rax,rdi
  000000006511A681: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  000000006511A686: 48 83 C4 40        add         rsp,40h
  000000006511A68A: 5F                 pop         rdi
  000000006511A68B: C3                 ret
  000000006511A68C: CC                 int         3

  Summary

      695000 .text
