Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006506A620: 40 53              push        rbx
  000000006506A622: 48 83 EC 20        sub         rsp,20h
  000000006506A626: 48 8B D9           mov         rbx,rcx
  000000006506A629: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006506A62D: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006506A631: 0F BA A0 C8 01 00  bt          dword ptr [rax+00000000000001C8h],19h
                    00 19
  000000006506A639: 72 3C              jb          000000006506A677
  000000006506A63B: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006506A63F: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  000000006506A643: 74 14              je          000000006506A659
  000000006506A645: E8 96 CB FA FF     call        00000000650171E0
  000000006506A64A: 48 8B CB           mov         rcx,rbx
  000000006506A64D: 8B D0              mov         edx,eax
  000000006506A64F: 48 83 C4 20        add         rsp,20h
  000000006506A653: 5B                 pop         rbx
  000000006506A654: E9 B7 FE FF FF     jmp         000000006506A510
  000000006506A659: FF 15 09 C2 62 00  call        qword ptr [0000000065696868h]
  000000006506A65F: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006506A663: 0F B6 81 CB 01 00  movzx       eax,byte ptr [rcx+00000000000001CBh]
                    00
  000000006506A66A: 48 8B CB           mov         rcx,rbx
  000000006506A66D: 83 E0 01           and         eax,1
  000000006506A670: 8B D0              mov         edx,eax
  000000006506A672: E8 99 FE FF FF     call        000000006506A510
  000000006506A677: 48 83 C4 20        add         rsp,20h
  000000006506A67B: 5B                 pop         rbx
  000000006506A67C: C3                 ret

  Summary

      695000 .text
