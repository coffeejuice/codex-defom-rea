Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140857ED0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140857ED5: 48 83 EC 38        sub         rsp,38h
  0000000140857ED9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140857EDE: 48 83 B8 F0 00 00  cmp         qword ptr [rax+00000000000000F0h],0
                    00 00
  0000000140857EE6: 74 1E              je          0000000140857F06
  0000000140857EE8: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140857EF0: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140857EF5: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140857EFA: 48 8B 89 F0 00 00  mov         rcx,qword ptr [rcx+00000000000000F0h]
                    00
  0000000140857F01: E8 EA 6C FD FF     call        000000014082EBF0
  0000000140857F06: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140857F0B: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000140857F0F: 48 83 C4 38        add         rsp,38h
  0000000140857F13: C3                 ret

  Summary

     1E02000 .text
