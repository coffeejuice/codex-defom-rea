Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140004B90: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140004B95: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140004B9A: 48 83 EC 58        sub         rsp,58h
  0000000140004B9E: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140004BA7: BA 01 00 00 00     mov         edx,1
  0000000140004BAC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140004BB1: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140004BB4: FF 15 1E F7 DF 01  call        qword ptr [0000000141E042D8h]
  0000000140004BBA: 0F B6 C0           movzx       eax,al
  0000000140004BBD: 85 C0              test        eax,eax
  0000000140004BBF: 74 31              je          0000000140004BF2
  0000000140004BC1: 41 B8 01 00 00 00  mov         r8d,1
  0000000140004BC7: BA FF FF FF 7F     mov         edx,7FFFFFFFh
  0000000140004BCC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140004BD1: E8 FA 00 00 00     call        0000000140004CD0
  0000000140004BD6: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140004BDB: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140004BE0: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140004BE5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140004BEA: E8 F1 02 00 00     call        0000000140004EE0
  0000000140004BEF: 90                 nop
  0000000140004BF0: EB 69              jmp         0000000140004C5B
  0000000140004BF2: 33 C0              xor         eax,eax
  0000000140004BF4: 85 C0              test        eax,eax
  0000000140004BF6: 74 27              je          0000000140004C1F
  0000000140004BF8: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140004BFD: FF 15 FD F5 DF 01  call        qword ptr [0000000141E04200h]
  0000000140004C03: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140004C08: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140004C0D: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  0000000140004C12: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140004C17: E8 C4 02 00 00     call        0000000140004EE0
  0000000140004C1C: 90                 nop
  0000000140004C1D: EB 3C              jmp         0000000140004C5B
  0000000140004C1F: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140004C24: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000140004C29: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140004C2E: E8 AD 02 00 00     call        0000000140004EE0
  0000000140004C33: 90                 nop
  0000000140004C34: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140004C39: FF 15 C1 F5 DF 01  call        qword ptr [0000000141E04200h]
  0000000140004C3F: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140004C44: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140004C49: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140004C4E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140004C53: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140004C58: 48 89 01           mov         qword ptr [rcx],rax
  0000000140004C5B: 48 83 C4 58        add         rsp,58h
  0000000140004C5F: C3                 ret

  Summary

     1E02000 .text
