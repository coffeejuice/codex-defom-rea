Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C91070: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C91075: 48 83 EC 48        sub         rsp,48h
  0000000140C91079: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C91082: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C91087: 48 83 B8 50 02 00  cmp         qword ptr [rax+0000000000000250h],0
                    00 00
  0000000140C9108F: 75 4C              jne         0000000140C910DD
  0000000140C91091: B9 08 00 00 00     mov         ecx,8
  0000000140C91096: E8 21 DA 7D 00     call        000000014146EABC
  0000000140C9109B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C910A0: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140C910A6: 74 11              je          0000000140C910B9
  0000000140C910A8: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140C910AD: E8 5E 36 0D 00     call        0000000140D64710
  0000000140C910B2: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C910B7: EB 09              jmp         0000000140C910C2
  0000000140C910B9: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140C910C2: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140C910C7: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C910CC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C910D1: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C910D6: 48 89 81 50 02 00  mov         qword ptr [rcx+0000000000000250h],rax
                    00
  0000000140C910DD: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C910E2: 48 8B 80 50 02 00  mov         rax,qword ptr [rax+0000000000000250h]
                    00
  0000000140C910E9: 48 83 C4 48        add         rsp,48h
  0000000140C910ED: C3                 ret

  Summary

     1E02000 .text
