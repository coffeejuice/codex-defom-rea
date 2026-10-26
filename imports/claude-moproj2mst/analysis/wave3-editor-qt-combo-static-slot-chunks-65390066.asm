Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065390066: 48 89 5C 24 30     mov         qword ptr [rsp+30h],rbx
  000000006539006B: 8B 94 81 C0 02 39  mov         edx,dword ptr [rcx+rax*4+00000000003902C0h]
                    00
  0000000065390072: 48 03 D1           add         rdx,rcx
  0000000065390075: FF E2              jmp         rdx
  0000000065390077: 49 8B 41 08        mov         rax,qword ptr [r9+8]
  000000006539007B: 48 8D 15 16 7B 36  lea         rdx,[00000000656F7B98h]
                    00
  0000000065390082: 4C 8D 4C 24 20     lea         r9,[rsp+20h]
  0000000065390087: 45 33 C0           xor         r8d,r8d
  000000006539008A: 49 8B CA           mov         rcx,r10
  000000006539008D: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000065390096: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000006539009B: FF 15 0F 68 30 00  call        qword ptr [00000000656968B0h]
  00000000653900A1: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000653900A6: 48 83 C4 38        add         rsp,38h
  00000000653900AA: C3                 ret

  Summary

      695000 .text
