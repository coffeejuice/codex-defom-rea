Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006503D1E8: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006503D1EB: 44 39 78 08        cmp         dword ptr [rax+8],r15d
  000000006503D1EF: 7D 3E              jge         000000006503D22F
  000000006503D1F1: 48 8B C8           mov         rcx,rax
  000000006503D1F4: 48 85 C0           test        rax,rax
  000000006503D1F7: 74 18              je          000000006503D211
  000000006503D1F9: FF 15 79 97 65 00  call        qword ptr [0000000065696978h]
  000000006503D1FF: 84 C0              test        al,al
  000000006503D201: 75 0E              jne         000000006503D211
  000000006503D203: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006503D206: BA 08 00 00 00     mov         edx,8
  000000006503D20B: FF 15 E7 96 65 00  call        qword ptr [00000000656968F8h]
  000000006503D211: 48 8B CB           mov         rcx,rbx
  000000006503D214: E8 95 3E 5A 00     call        00000000655E10AE
  000000006503D219: 49 C7 86 D8 00 00  mov         qword ptr [r14+00000000000000D8h],0
                    00 00 00 00 00
  000000006503D224: 41 C7 86 08 01 00  mov         dword ptr [r14+0000000000000108h],0FFFFFFFFh
                    00 FF FF FF FF
  000000006503D22F: 41 89 B6 C4 00 00  mov         dword ptr [r14+00000000000000C4h],esi
                    00
  000000006503D236: 45 89 BE C0 00 00  mov         dword ptr [r14+00000000000000C0h],r15d
                    00
  000000006503D23D: 48 83 C4 38        add         rsp,38h
  000000006503D241: 41 5F              pop         r15
  000000006503D243: 41 5E              pop         r14
  000000006503D245: 5E                 pop         rsi
  000000006503D246: 5B                 pop         rbx
  000000006503D247: C3                 ret

  Summary

      695000 .text
