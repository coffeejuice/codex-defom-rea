Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006508B370: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006508B375: 53                 push        rbx
  000000006508B376: 48 83 EC 30        sub         rsp,30h
  000000006508B37A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006508B383: 48 8B D9           mov         rbx,rcx
  000000006508B386: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006508B38A: 48 85 C9           test        rcx,rcx
  000000006508B38D: 74 0A              je          000000006508B399
  000000006508B38F: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006508B392: BA 01 00 00 00     mov         edx,1
  000000006508B397: FF 10              call        qword ptr [rax]
  000000006508B399: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  000000006508B39D: 48 85 C9           test        rcx,rcx
  000000006508B3A0: 74 0A              je          000000006508B3AC
  000000006508B3A2: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006508B3A5: BA 01 00 00 00     mov         edx,1
  000000006508B3AA: FF 10              call        qword ptr [rax]
  000000006508B3AC: 48 8B 4B 38        mov         rcx,qword ptr [rbx+38h]
  000000006508B3B0: 48 85 C9           test        rcx,rcx
  000000006508B3B3: 74 07              je          000000006508B3BC
  000000006508B3B5: FF 15 CD E5 60 00  call        qword ptr [0000000065699988h]
  000000006508B3BB: 90                 nop
  000000006508B3BC: 48 8D 4B 18        lea         rcx,[rbx+18h]
  000000006508B3C0: 48 83 C4 30        add         rsp,30h
  000000006508B3C4: 5B                 pop         rbx
  000000006508B3C5: E9 56 35 04 00     jmp         00000000650CE920

  Summary

      695000 .text
