Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140140BD0: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140140BD5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140140BDA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140140BDF: 48 83 EC 48        sub         rsp,48h
  0000000140140BE3: 33 C0              xor         eax,eax
  0000000140140BE5: 85 C0              test        eax,eax
  0000000140140BE7: 74 4D              je          0000000140140C36
  0000000140140BE9: B9 08 00 00 00     mov         ecx,8
  0000000140140BEE: E8 C9 DE 32 01     call        000000014146EABC
  0000000140140BF3: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140140BF8: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140140BFE: 74 1E              je          0000000140140C1E
  0000000140140C00: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140140C05: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140140C0A: F2 0F 10 00        movsd       xmm0,mmword ptr [rax]
  0000000140140C0E: F2 0F 11 01        movsd       mmword ptr [rcx],xmm0
  0000000140140C12: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140140C17: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140140C1C: EB 09              jmp         0000000140140C27
  0000000140140C1E: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140140C27: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140140C2C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140140C31: 48 89 01           mov         qword ptr [rcx],rax
  0000000140140C34: EB 60              jmp         0000000140140C96
  0000000140140C36: 33 C0              xor         eax,eax
  0000000140140C38: 85 C0              test        eax,eax
  0000000140140C3A: 74 45              je          0000000140140C81
  0000000140140C3C: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140140C41: B9 08 00 00 00     mov         ecx,8
  0000000140140C46: E8 35 26 EC FF     call        0000000140003280
  0000000140140C4B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140140C50: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140140C56: 74 1E              je          0000000140140C76
  0000000140140C58: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140140C5D: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140140C62: F2 0F 10 00        movsd       xmm0,mmword ptr [rax]
  0000000140140C66: F2 0F 11 01        movsd       mmword ptr [rcx],xmm0
  0000000140140C6A: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140140C6F: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140140C74: EB 09              jmp         0000000140140C7F
  0000000140140C76: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140140C7F: EB 15              jmp         0000000140140C96
  0000000140140C81: 41 B8 08 00 00 00  mov         r8d,8
  0000000140140C87: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140140C8C: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140140C91: E8 6A E4 32 01     call        000000014146F100
  0000000140140C96: 48 83 C4 48        add         rsp,48h
  0000000140140C9A: C3                 ret

  Summary

     1E02000 .text
