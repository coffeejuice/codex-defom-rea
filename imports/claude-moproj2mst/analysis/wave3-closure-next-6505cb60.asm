Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505CB60: 40 53              push        rbx
  000000006505CB62: 48 83 EC 20        sub         rsp,20h
  000000006505CB66: 48 8B D9           mov         rbx,rcx
  000000006505CB69: FF 15 F9 9C 63 00  call        qword ptr [0000000065696868h]
  000000006505CB6F: 4C 8B D8           mov         r11,rax
  000000006505CB72: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006505CB76: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  000000006505CB7A: 75 22              jne         000000006505CB9E
  000000006505CB7C: 0F 1F 40 00        nop         dword ptr [rax]
  000000006505CB80: 4D 85 DB           test        r11,r11
  000000006505CB83: 74 19              je          000000006505CB9E
  000000006505CB85: 49 8B CB           mov         rcx,r11
  000000006505CB88: 49 8B DB           mov         rbx,r11
  000000006505CB8B: FF 15 D7 9C 63 00  call        qword ptr [0000000065696868h]
  000000006505CB91: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  000000006505CB95: F6 41 0C 01        test        byte ptr [rcx+0Ch],1
  000000006505CB99: 4C 8B D8           mov         r11,rax
  000000006505CB9C: 74 E2              je          000000006505CB80
  000000006505CB9E: 48 8B C3           mov         rax,rbx
  000000006505CBA1: 48 83 C4 20        add         rsp,20h
  000000006505CBA5: 5B                 pop         rbx
  000000006505CBA6: C3                 ret

  Summary

      695000 .text
