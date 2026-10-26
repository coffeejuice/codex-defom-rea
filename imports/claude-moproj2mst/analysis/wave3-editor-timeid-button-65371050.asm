Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065371050: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065371055: 53                 push        rbx
  0000000065371056: 48 83 EC 30        sub         rsp,30h
  000000006537105A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065371063: 48 8B D9           mov         rbx,rcx
  0000000065371066: C7 44 24 58 00 00  mov         dword ptr [rsp+58h],0
                    00 00
  000000006537106E: 4C 8D 4C 24 58     lea         r9,[rsp+58h]
  0000000065371073: E8 88 FE CF FF     call        0000000065070F00
  0000000065371078: 90                 nop
  0000000065371079: 48 8D 05 C8 24 38  lea         rax,[00000000656F3548h]
                    00
  0000000065371080: 48 89 03           mov         qword ptr [rbx],rax
  0000000065371083: 48 8D 05 86 24 38  lea         rax,[00000000656F3510h]
                    00
  000000006537108A: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  000000006537108E: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  0000000065371092: E8 59 FD FF FF     call        0000000065370DF0
  0000000065371097: 90                 nop
  0000000065371098: 48 8B C3           mov         rax,rbx
  000000006537109B: 48 83 C4 30        add         rsp,30h
  000000006537109F: 5B                 pop         rbx
  00000000653710A0: C3                 ret

  Summary

      695000 .text
