Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065567990: 40 53              push        rbx
  0000000065567992: 48 83 EC 20        sub         rsp,20h
  0000000065567996: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  000000006556799A: 48 8B DA           mov         rbx,rdx
  000000006556799D: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000000655679A5: 48 89 0A           mov         qword ptr [rdx],rcx
  00000000655679A8: FF 15 12 EF 12 00  call        qword ptr [00000000656968C0h]
  00000000655679AE: 4C 8B 1B           mov         r11,qword ptr [rbx]
  00000000655679B1: 41 F6 43 10 01     test        byte ptr [r11+10h],1
  00000000655679B6: 75 0C              jne         00000000655679C4
  00000000655679B8: 41 8B 53 04        mov         edx,dword ptr [r11+4]
  00000000655679BC: 48 8B CB           mov         rcx,rbx
  00000000655679BF: E8 AC 7A FA FF     call        000000006550F470
  00000000655679C4: 48 8B C3           mov         rax,rbx
  00000000655679C7: 48 83 C4 20        add         rsp,20h
  00000000655679CB: 5B                 pop         rbx
  00000000655679CC: C3                 ret

  Summary

      695000 .text
