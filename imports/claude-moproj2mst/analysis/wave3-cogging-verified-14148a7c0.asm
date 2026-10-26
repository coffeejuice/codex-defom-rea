Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014148A7C0: 40 53              push        rbx
  000000014148A7C2: 48 83 EC 20        sub         rsp,20h
  000000014148A7C6: FF 05 F4 52 27 03  inc         dword ptr [00000001446FFAC0h]
  000000014148A7CC: 48 8B D9           mov         rbx,rcx
  000000014148A7CF: B9 00 10 00 00     mov         ecx,1000h
  000000014148A7D4: E8 FB 3B FF FF     call        000000014147E3D4
  000000014148A7D9: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  000000014148A7DD: 48 85 C0           test        rax,rax
  000000014148A7E0: 74 0D              je          000000014148A7EF
  000000014148A7E2: 83 4B 18 08        or          dword ptr [rbx+18h],8
  000000014148A7E6: C7 43 24 00 10 00  mov         dword ptr [rbx+24h],1000h
                    00
  000000014148A7ED: EB 13              jmp         000000014148A802
  000000014148A7EF: 83 4B 18 04        or          dword ptr [rbx+18h],4
  000000014148A7F3: 48 8D 43 20        lea         rax,[rbx+20h]
  000000014148A7F7: C7 43 24 02 00 00  mov         dword ptr [rbx+24h],2
                    00
  000000014148A7FE: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  000000014148A802: 48 8B 43 10        mov         rax,qword ptr [rbx+10h]
  000000014148A806: 83 63 08 00        and         dword ptr [rbx+8],0
  000000014148A80A: 48 89 03           mov         qword ptr [rbx],rax
  000000014148A80D: 48 83 C4 20        add         rsp,20h
  000000014148A811: 5B                 pop         rbx
  000000014148A812: C3                 ret

  Summary

     1E02000 .text
