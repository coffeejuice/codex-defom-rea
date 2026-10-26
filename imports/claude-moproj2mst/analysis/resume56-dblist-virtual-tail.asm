Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140656156: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  000000014065615C: 74 4C              je          00000001406561AA
  000000014065615E: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  0000000140656166: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  000000014065616E: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000140656176: 48 8B 8C 24 68 01  mov         rcx,qword ptr [rsp+0000000000000168h]
                    00 00
  000000014065617E: FF 15 14 65 7B 01  call        qword ptr [0000000141E0C698h]
  0000000140656184: 48 89 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],rax
                    00 00
  000000014065618C: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140656191: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140656194: 41 B0 01           mov         r8b,1
  0000000140656197: 48 8B 94 24 68 02  mov         rdx,qword ptr [rsp+0000000000000268h]
                    00 00
  000000014065619F: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001406561A4: FF 90 18 02 00 00  call        qword ptr [rax+0000000000000218h]
  00000001406561AA: C6 84 24 70 01 00  mov         byte ptr [rsp+0000000000000170h],1
                    00 01
  00000001406561B2: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001406561B7: FF 15 23 F8 7A 01  call        qword ptr [0000000141E059E0h]
  00000001406561BD: 90                 nop
  00000001406561BE: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000001406561C6: FF 15 E4 64 7B 01  call        qword ptr [0000000141E0C6B0h]
  00000001406561CC: 90                 nop
  00000001406561CD: 48 8B 8C 24 98 02  mov         rcx,qword ptr [rsp+0000000000000298h]
                    00 00
  00000001406561D5: FF 15 CD 64 7B 01  call        qword ptr [0000000141E0C6A8h]
  00000001406561DB: 8A 84 24 70 01 00  mov         al,byte ptr [rsp+0000000000000170h]
                    00
  00000001406561E2: 48 81 C4 78 02 00  add         rsp,278h
                    00
  00000001406561E9: 5F                 pop         rdi
  00000001406561EA: 5E                 pop         rsi
  00000001406561EB: C3                 ret
  00000001406561EC: CC                 int         3

  Summary

     1E02000 .text
