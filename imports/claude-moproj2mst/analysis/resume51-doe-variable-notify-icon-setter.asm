Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014016DBE0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014016DBE5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014016DBEA: 48 83 EC 48        sub         rsp,48h
  000000014016DBEE: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014016DBF3: 48 8B 40 68        mov         rax,qword ptr [rax+68h]
  000000014016DBF7: 48 83 B8 18 01 00  cmp         qword ptr [rax+0000000000000118h],0
                    00 00
  000000014016DBFF: 74 46              je          000000014016DC47
  000000014016DC01: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014016DC06: 48 8B 40 68        mov         rax,qword ptr [rax+68h]
  000000014016DC0A: 48 8B 80 18 01 00  mov         rax,qword ptr [rax+0000000000000118h]
                    00
  000000014016DC11: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014016DC16: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014016DC1B: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014016DC20: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014016DC26: 74 16              je          000000014016DC3E
  000000014016DC28: BA 01 00 00 00     mov         edx,1
  000000014016DC2D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014016DC32: E8 99 0C 00 00     call        000000014016E8D0
  000000014016DC37: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014016DC3C: EB 09              jmp         000000014016DC47
  000000014016DC3E: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014016DC47: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014016DC4C: 48 8B 49 68        mov         rcx,qword ptr [rcx+68h]
  000000014016DC50: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014016DC55: 48 89 81 18 01 00  mov         qword ptr [rcx+0000000000000118h],rax
                    00
  000000014016DC5C: 48 83 C4 48        add         rsp,48h
  000000014016DC60: C3                 ret

  Summary

     1E02000 .text
