Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014175FAD0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014175FAD5: 48 83 EC 38        sub         rsp,38h
  000000014175FAD9: 48 8D 0D D8 6B 61  lea         rcx,[0000000143D766B8h]
                    02
  000000014175FAE0: E8 AB 85 B6 FE     call        00000001402C8090
  000000014175FAE5: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014175FAEA: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014175FAF0: 75 04              jne         000000014175FAF6
  000000014175FAF2: 33 C0              xor         eax,eax
  000000014175FAF4: EB 13              jmp         000000014175FB09
  000000014175FAF6: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014175FAFB: 48 8B 00           mov         rax,qword ptr [rax]
  000000014175FAFE: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014175FB03: FF 90 78 02 00 00  call        qword ptr [rax+0000000000000278h]
  000000014175FB09: 48 83 C4 38        add         rsp,38h
  000000014175FB0D: C3                 ret

  Summary

     1E02000 .text
