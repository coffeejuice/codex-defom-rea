Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006530B770: 48 83 EC 28        sub         rsp,28h
  000000006530B774: 84 C9              test        cl,cl
  000000006530B776: 75 0B              jne         000000006530B783
  000000006530B778: 8A 05 2A 13 6C 00  mov         al,byte ptr [00000000659CCAA8h]
  000000006530B77E: 48 83 C4 28        add         rsp,28h
  000000006530B782: C3                 ret
  000000006530B783: E8 68 F2 FF FF     call        000000006530A9F0
  000000006530B788: 84 C0              test        al,al
  000000006530B78A: 74 2C              je          000000006530B7B8
  000000006530B78C: FF 15 46 12 6C 00  call        qword ptr [00000000659CC9D8h]
  000000006530B792: 84 C0              test        al,al
  000000006530B794: 74 22              je          000000006530B7B8
  000000006530B796: FF 15 34 12 6C 00  call        qword ptr [00000000659CC9D0h]
  000000006530B79C: 84 C0              test        al,al
  000000006530B79E: 75 0B              jne         000000006530B7AB
  000000006530B7A0: FF 15 7A B0 38 00  call        qword ptr [0000000065696820h]
  000000006530B7A6: 48 85 C0           test        rax,rax
  000000006530B7A9: 75 0D              jne         000000006530B7B8
  000000006530B7AB: B0 01              mov         al,1
  000000006530B7AD: 88 05 F5 12 6C 00  mov         byte ptr [00000000659CCAA8h],al
  000000006530B7B3: 48 83 C4 28        add         rsp,28h
  000000006530B7B7: C3                 ret
  000000006530B7B8: 32 C0              xor         al,al
  000000006530B7BA: 88 05 E8 12 6C 00  mov         byte ptr [00000000659CCAA8h],al
  000000006530B7C0: 48 83 C4 28        add         rsp,28h
  000000006530B7C4: C3                 ret

  Summary

      695000 .text
