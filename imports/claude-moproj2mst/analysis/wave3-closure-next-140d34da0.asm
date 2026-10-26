Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D34DA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D34DA5: 48 83 EC 68        sub         rsp,68h
  0000000140D34DA9: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D34DB2: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140D34DB7: E8 64 FB FF FF     call        0000000140D34920
  0000000140D34DBC: B9 20 00 00 00     mov         ecx,20h
  0000000140D34DC1: E8 F6 9C 73 00     call        000000014146EABC
  0000000140D34DC6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D34DCB: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140D34DD1: 74 36              je          0000000140D34E09
  0000000140D34DD3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D34DD8: FF 15 B2 33 0D 01  call        qword ptr [0000000141E08190h]
  0000000140D34DDE: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  0000000140D34DE3: 48 8D 05 CE E2 43  lea         rax,[00000001421730B8h]
                    01
  0000000140D34DEA: 49 89 03           mov         qword ptr [r11],rax
  0000000140D34DED: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D34DF2: 48 8D 05 3F E2 43  lea         rax,[0000000142173038h]
                    01
  0000000140D34DF9: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140D34DFD: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D34E02: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140D34E07: EB 09              jmp         0000000140D34E12
  0000000140D34E09: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  0000000140D34E12: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140D34E17: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D34E1C: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D34E21: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D34E26: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140D34E2E: 45 33 C9           xor         r9d,r9d
  0000000140D34E31: 45 33 C0           xor         r8d,r8d
  0000000140D34E34: 33 D2              xor         edx,edx
  0000000140D34E36: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D34E3B: FF 15 67 65 0D 01  call        qword ptr [0000000141E0B3A8h]
  0000000140D34E41: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140D34E46: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140D34E4B: FF 15 F7 6A 0D 01  call        qword ptr [0000000141E0B948h]
  0000000140D34E51: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140D34E56: 48 83 C4 68        add         rsp,68h
  0000000140D34E5A: C3                 ret

  Summary

     1E02000 .text
