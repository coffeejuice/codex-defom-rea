Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  0000000061008350: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  0000000061008355: 57                 push        rdi
  0000000061008356: 48 83 EC 20        sub         rsp,20h
  000000006100835A: 48 8B 49 68        mov         rcx,qword ptr [rcx+68h]
  000000006100835E: 48 8B DA           mov         rbx,rdx
  0000000061008361: 49 8B D0           mov         rdx,r8
  0000000061008364: 48 83 C1 08        add         rcx,8
  0000000061008368: 49 8B F9           mov         rdi,r9
  000000006100836B: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000061008373: E8 B8 F3 FF FF     call        0000000061007730
  0000000061008378: 48 85 C0           test        rax,rax
  000000006100837B: 75 1A              jne         0000000061008397
  000000006100837D: 48 8B D7           mov         rdx,rdi
  0000000061008380: 48 8B CB           mov         rcx,rbx
  0000000061008383: FF 15 EF B4 03 00  call        qword ptr [0000000061043878h]
  0000000061008389: 48 8B C3           mov         rax,rbx
  000000006100838C: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  0000000061008391: 48 83 C4 20        add         rsp,20h
  0000000061008395: 5F                 pop         rdi
  0000000061008396: C3                 ret
  0000000061008397: 48 8B D3           mov         rdx,rbx
  000000006100839A: 48 8B C8           mov         rcx,rax
  000000006100839D: E8 EE 95 FF FF     call        0000000061001990
  00000000610083A2: 48 8B C3           mov         rax,rbx
  00000000610083A5: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000610083AA: 48 83 C4 20        add         rsp,20h
  00000000610083AE: 5F                 pop         rdi
  00000000610083AF: C3                 ret

  Summary

       42000 .text
