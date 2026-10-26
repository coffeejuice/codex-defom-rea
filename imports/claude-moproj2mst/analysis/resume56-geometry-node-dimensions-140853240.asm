Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140853240: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140853245: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014085324A: 48 83 B8 30 01 00  cmp         qword ptr [rax+0000000000000130h],0
                    00 00
  0000000140853252: 75 04              jne         0000000140853258
  0000000140853254: 33 C0              xor         eax,eax
  0000000140853256: EB 12              jmp         000000014085326A
  0000000140853258: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014085325D: 48 8B 80 30 01 00  mov         rax,qword ptr [rax+0000000000000130h]
                    00
  0000000140853264: 48 05 F8 15 00 00  add         rax,15F8h
  000000014085326A: F3

  Summary

     1E02000 .text
