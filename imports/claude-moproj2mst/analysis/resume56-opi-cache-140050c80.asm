Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140050C80: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140050C85: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140050C8A: 48 83 EC 38        sub         rsp,38h
  0000000140050C8E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140050C93: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140050C9A: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140050C9F: FF 15 63 32 DB 01  call        qword ptr [0000000141E03F08h]
  0000000140050CA5: 0F B6 C0           movzx       eax,al
  0000000140050CA8: 85 C0              test        eax,eax
  0000000140050CAA: 74 02              je          0000000140050CAE
  0000000140050CAC: EB 43              jmp         0000000140050CF1
  0000000140050CAE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140050CB3: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140050CBA: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140050CBF: FF 15 AB 37 DB 01  call        qword ptr [0000000141E04470h]
  0000000140050CC5: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140050CCA: 0F B6 54 24 20     movzx       edx,byte ptr [rsp+20h]
  0000000140050CCF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140050CD4: E8 67 42 00 00     call        0000000140054F40
  0000000140050CD9: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140050CDE: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140050CE4: 74 0B              je          0000000140050CF1
  0000000140050CE6: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140050CEB: FF 15 0F A9 DB 01  call        qword ptr [0000000141E0B600h]
  0000000140050CF1: 48 83 C4 38        add         rsp,38h
  0000000140050CF5: C3                 ret

  Summary

     1E02000 .text
