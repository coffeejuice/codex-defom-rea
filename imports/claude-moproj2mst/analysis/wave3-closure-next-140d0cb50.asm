Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D0CB50: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140D0CB54: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D0CB59: 48 83 EC 48        sub         rsp,48h
  0000000140D0CB5D: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D0CB66: 0F B6 44 24 58     movzx       eax,byte ptr [rsp+58h]
  0000000140D0CB6B: 85 C0              test        eax,eax
  0000000140D0CB6D: 74 6C              je          0000000140D0CBDB
  0000000140D0CB6F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D0CB74: 48 81 C1 40 08 00  add         rcx,840h
                    00
  0000000140D0CB7B: E8 70 E3 F0 FF     call        0000000140C1AEF0
  0000000140D0CB80: 48 85 C0           test        rax,rax
  0000000140D0CB83: 75 56              jne         0000000140D0CBDB
  0000000140D0CB85: B9 30 01 00 00     mov         ecx,130h
  0000000140D0CB8A: E8 2D 1F 76 00     call        000000014146EABC
  0000000140D0CB8F: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D0CB94: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140D0CB9A: 74 16              je          0000000140D0CBB2
  0000000140D0CB9C: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140D0CBA1: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140D0CBA6: E8 25 50 03 00     call        0000000140D41BD0
  0000000140D0CBAB: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D0CBB0: EB 09              jmp         0000000140D0CBBB
  0000000140D0CBB2: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140D0CBBB: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D0CBC0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D0CBC5: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D0CBCA: 48 81 C1 40 08 00  add         rcx,840h
                    00
  0000000140D0CBD1: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140D0CBD6: E8 45 FD FA FF     call        0000000140CBC920
  0000000140D0CBDB: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D0CBE0: 48 81 C1 40 08 00  add         rcx,840h
                    00
  0000000140D0CBE7: E8 04 E3 F0 FF     call        0000000140C1AEF0
  0000000140D0CBEC: 48 83 C4 48        add         rsp,48h
  0000000140D0CBF0: C3                 ret

  Summary

     1E02000 .text
