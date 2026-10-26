Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140054C80: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140054C85: 48 83 EC 48        sub         rsp,48h
  0000000140054C89: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140054C8E: E8 FD 01 00 00     call        0000000140054E90
  0000000140054C93: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140054C98: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140054C9E: 75 02              jne         0000000140054CA2
  0000000140054CA0: EB 60              jmp         0000000140054D02
  0000000140054CA2: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140054CA7: E8 C4 AA 00 00     call        000000014005F770
  0000000140054CAC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140054CB1: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140054CB7: 75 02              jne         0000000140054CBB
  0000000140054CB9: EB 47              jmp         0000000140054D02
  0000000140054CBB: C6 44 24 28 01     mov         byte ptr [rsp+28h],1
  0000000140054CC0: 0F B6 54 24 28     movzx       edx,byte ptr [rsp+28h]
  0000000140054CC5: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140054CCA: E8 71 02 00 00     call        0000000140054F40
  0000000140054CCF: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140054CD4: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140054CD9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140054CDC: 4C 8B 44 24 30     mov         r8,qword ptr [rsp+30h]
  0000000140054CE1: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140054CE6: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140054CEB: FF 90 B0 00 00 00  call        qword ptr [rax+00000000000000B0h]
  0000000140054CF1: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140054CF6: 48 8D 0D 03 1A D2  lea         rcx,[0000000143D76700h]
                    03
  0000000140054CFD: E8 DE 9D 27 00     call        00000001402CEAE0
  0000000140054D02: 48 83 C4 48        add         rsp,48h
  0000000140054D06: C3                 ret

  Summary

     1E02000 .text
