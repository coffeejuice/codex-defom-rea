Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006525BD90: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006525BD95: 57                 push        rdi
  000000006525BD96: 48 83 EC 30        sub         rsp,30h
  000000006525BD9A: 48 8B D9           mov         rbx,rcx
  000000006525BD9D: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000006525BDA0: 4C 8D 05 09 35 E4  lea         r8,[000000006509F2B0h]
                    FF
  000000006525BDA7: 48 8D 15 72 C0 2C  lea         rdx,[0000000065527E20h]
                    00
  000000006525BDAE: 41 B9 18 00 00 00  mov         r9d,18h
  000000006525BDB4: C7 44 24 20 08 00  mov         dword ptr [rsp+20h],8
                    00 00
  000000006525BDBC: FF 15 0E AE 43 00  call        qword ptr [0000000065696BD0h]
  000000006525BDC2: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006525BDC5: 48 83 C1 10        add         rcx,10h
  000000006525BDC9: 48 8B F8           mov         rdi,rax
  000000006525BDCC: FF 15 A6 AB 43 00  call        qword ptr [0000000065696978h]
  000000006525BDD2: 84 C0              test        al,al
  000000006525BDD4: 75 10              jne         000000006525BDE6
  000000006525BDD6: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006525BDD9: 48 8D 15 D0 34 E4  lea         rdx,[000000006509F2B0h]
                    FF
  000000006525BDE0: FF 15 CA AD 43 00  call        qword ptr [0000000065696BB0h]
  000000006525BDE6: 48 89 3B           mov         qword ptr [rbx],rdi
  000000006525BDE9: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006525BDEE: 48 83 C4 30        add         rsp,30h
  000000006525BDF2: 5F                 pop         rdi
  000000006525BDF3: C3                 ret

  Summary

      695000 .text
