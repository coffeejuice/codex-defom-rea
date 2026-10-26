Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006701DAE3: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000006701DAE8: E8 99 AF 15 00     call        0000000067178A86
  000000006701DAED: 48 8B D8           mov         rbx,rax
  000000006701DAF0: 48 85 C0           test        rax,rax
  000000006701DAF3: 74 14              je          000000006701DB09
  000000006701DAF5: 48 8D 05 BC 30 2D  lea         rax,[00000000672F0BB8h]
                    00
  000000006701DAFC: 48 89 03           mov         qword ptr [rbx],rax
  000000006701DAFF: F0 83 05 B1 30 2D  lock add    dword ptr [00000000672F0BB8h],1
                    00 01
  000000006701DB07: EB 02              jmp         000000006701DB0B
  000000006701DB09: 33 DB              xor         ebx,ebx
  000000006701DB0B: 33 C0              xor         eax,eax
  000000006701DB0D: F0 48 0F B1 1D 52  lock cmpxchg qword ptr [00000000672F3C68h],rbx
                    61 2D 00
  000000006701DB16: 74 37              je          000000006701DB4F
  000000006701DB18: 48 85 DB           test        rbx,rbx
  000000006701DB1B: 74 5F              je          000000006701DB7C
  000000006701DB1D: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006701DB20: 48 85 C9           test        rcx,rcx
  000000006701DB23: 74 11              je          000000006701DB36
  000000006701DB25: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  000000006701DB29: 75 0B              jne         000000006701DB36
  000000006701DB2B: 48 8B 13           mov         rdx,qword ptr [rbx]
  000000006701DB2E: 48 8B CB           mov         rcx,rbx
  000000006701DB31: E8 EA FA FF FF     call        000000006701D620
  000000006701DB36: 48 8B CB           mov         rcx,rbx
  000000006701DB39: E8 18 AE 15 00     call        0000000067178956
  000000006701DB3E: 48 8B 05 23 61 2D  mov         rax,qword ptr [00000000672F3C68h]
                    00
  000000006701DB45: 48 8B 5C 24 20     mov         rbx,qword ptr [rsp+20h]
  000000006701DB4A: 48 83 C4 28        add         rsp,28h
  000000006701DB4E: C3                 ret
  000000006701DB4F: 8B 05 0B 61 2D 00  mov         eax,dword ptr [00000000672F3C60h]
  000000006701DB55: A8 01              test        al,1
  000000006701DB57: 75 23              jne         000000006701DB7C
  000000006701DB59: 83 C8 01           or          eax,1
  000000006701DB5C: 48 8D 0D 6D 84 18  lea         rcx,[00000000671A5FD0h]
                    00
  000000006701DB63: 89 05 F7 60 2D 00  mov         dword ptr [00000000672F3C60h],eax
  000000006701DB69: 48 8D 05 F8 60 2D  lea         rax,[00000000672F3C68h]
                    00
  000000006701DB70: 48 89 05 E1 60 2D  mov         qword ptr [00000000672F3C58h],rax
                    00
  000000006701DB77: E8 D8 AF 15 00     call        0000000067178B54
  000000006701DB7C: 48 8B 05 E5 60 2D  mov         rax,qword ptr [00000000672F3C68h]
                    00
  000000006701DB83: 48 8B 5C 24 20     mov         rbx,qword ptr [rsp+20h]
  000000006701DB88: 48 83 C4 28        add         rsp,28h
  000000006701DB8C: C3                 ret
  000000006701DB8D: 48
  000000006701DB8E: 8B
  000000006701DB8F: 05
  000000006701DB90: D4
  000000006701DB91: 60

  Summary

      1A7000 .text
