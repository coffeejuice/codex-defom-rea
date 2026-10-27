Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091E2D0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014091E2D4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091E2D9: 48 83 EC 28        sub         rsp,28h
  000000014091E2DD: 83 7C 24 38 FF     cmp         dword ptr [rsp+38h],0FFFFFFFFh
  000000014091E2E2: 7E 28              jle         000000014091E30C
  000000014091E2E4: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014091E2E9: 48 83 C1 18        add         rcx,18h
  000000014091E2ED: E8 EE 67 6E FF     call        0000000140004AE0
  000000014091E2F2: 39 44 24 38        cmp         dword ptr [rsp+38h],eax
  000000014091E2F6: 7D 14              jge         000000014091E30C
  000000014091E2F8: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014091E2FD: 48 83 C1 18        add         rcx,18h
  000000014091E301: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  000000014091E305: E8 16 18 00 00     call        000000014091FB20
  000000014091E30A: EB 02              jmp         000000014091E30E
  000000014091E30C: 33 C0              xor         eax,eax
  000000014091E30E: 48 83 C4 28        add         rsp,28h
  000000014091E312: C3                 ret

  Summary

     1E02000 .text
