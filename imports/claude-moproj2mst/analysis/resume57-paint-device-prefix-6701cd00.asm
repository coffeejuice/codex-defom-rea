Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006701CD00: 40 53              push        rbx
  000000006701CD02: 56                 push        rsi
  000000006701CD03: 57                 push        rdi
  000000006701CD04: 48 81 EC 30 01 00  sub         rsp,130h
                    00
  000000006701CD0B: 48 8B 05 8E 6E 2D  mov         rax,qword ptr [00000000672F3BA0h]
                    00
  000000006701CD12: 48 33 C4           xor         rax,rsp
  000000006701CD15: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  000000006701CD1D: 8B D9              mov         ebx,ecx
  000000006701CD1F: 49 8B F0           mov         rsi,r8
  000000006701CD22: 48 8B FA           mov         rdi,rdx
  000000006701CD25: 48 8D 4C 24 21     lea         rcx,[rsp+21h]
  000000006701CD2A: 33 D2              xor         edx,edx
  000000006701CD2C: 41 B8 FF 00 00 00  mov         r8d,0FFh
  000000006701CD32: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000006701CD37: E8 36 BE 15 00     call        0000000067178B72
  000000006701CD3C: C6 84 24 1F 01 00  mov         byte ptr [rsp+000000000000011Fh],0
                    00 00
  000000006701CD44: 48 85 FF           test        rdi,rdi
  000000006701CD47: 74 15              je          000000006701CD5E
  000000006701CD49: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006701CD4E: 4C 8B CE           mov         r9,rsi
  000000006701CD51: 4C 8B C7           mov         r8,rdi
  000000006701CD54: BA FF 00 00 00     mov         edx,0FFh
  000000006701CD59: E8 02 F9 05 00     call        000000006707C660
  000000006701CD5E: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000006701CD63: 8B CB              mov         ecx,ebx
  000000006701CD65: E8 C6 FE FF FF     call        000000006701CC30
  000000006701CD6A: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  000000006701CD72: 48 33 CC           xor         rcx,rsp
  000000006701CD75: E8 96 BE 15 00     call        0000000067178C10
  000000006701CD7A: 48 81 C4 30 01 00  add         rsp,130h
                    00
  000000006701CD81: 5F                 pop         rdi
  000000006701CD82: 5E                 pop         rsi
  000000006701CD83: 5B                 pop         rbx
  000000006701CD84: C3                 ret

  Summary

      1A7000 .text
