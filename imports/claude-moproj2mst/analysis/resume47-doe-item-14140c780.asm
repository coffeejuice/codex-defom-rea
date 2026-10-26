Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014140C780: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014140C785: 48 83 EC 48        sub         rsp,48h
  000000014140C789: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014140C78E: 48 81 C1 90 00 00  add         rcx,90h
                    00
  000000014140C795: E8 46 83 BF FE     call        0000000140004AE0
  000000014140C79A: 85 C0              test        eax,eax
  000000014140C79C: 7E 66              jle         000000014140C804
  000000014140C79E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014140C7A3: 48 81 C1 90 00 00  add         rcx,90h
                    00
  000000014140C7AA: E8 61 AC EF FE     call        0000000140307410
  000000014140C7AF: 0F B6 C0           movzx       eax,al
  000000014140C7B2: 85 C0              test        eax,eax
  000000014140C7B4: 75 4E              jne         000000014140C804
  000000014140C7B6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014140C7BB: 48 81 C1 90 00 00  add         rcx,90h
                    00
  000000014140C7C2: E8 59 4F 84 FF     call        0000000140C51720
  000000014140C7C7: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014140C7CC: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014140C7D1: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014140C7D6: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014140C7DC: 74 1B              je          000000014140C7F9
  000000014140C7DE: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014140C7E3: 48 8B 00           mov         rax,qword ptr [rax]
  000000014140C7E6: BA 01 00 00 00     mov         edx,1
  000000014140C7EB: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014140C7F0: FF 10              call        qword ptr [rax]
  000000014140C7F2: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014140C7F7: EB 09              jmp         000000014140C802
  000000014140C7F9: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014140C802: EB 9A              jmp         000000014140C79E
  000000014140C804: 48 83 C4 48        add         rsp,48h
  000000014140C808: C3                 ret

  Summary

     1E02000 .text
