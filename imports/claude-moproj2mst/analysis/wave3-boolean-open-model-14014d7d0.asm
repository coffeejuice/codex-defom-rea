Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014014D7D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014014D7D5: 48 83 EC 48        sub         rsp,48h
  000000014014D7D9: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  000000014014D7E2: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  000000014014D7EA: EB 0B              jmp         000000014014D7F7
  000000014014D7EC: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  000000014014D7F0: 83 C0 01           add         eax,1
  000000014014D7F3: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000014014D7F7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014014D7FC: 48 83 C1 10        add         rcx,10h
  000000014014D800: E8 DB 72 EB FF     call        0000000140004AE0
  000000014014D805: 39 44 24 28        cmp         dword ptr [rsp+28h],eax
  000000014014D809: 7D 46              jge         000000014014D851
  000000014014D80B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014014D810: 48 83 C1 10        add         rcx,10h
  000000014014D814: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  000000014014D818: E8 F3 33 01 00     call        0000000140160C10
  000000014014D81D: 48 8B 00           mov         rax,qword ptr [rax]
  000000014014D820: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014014D825: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014014D82A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014014D82D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014014D832: FF 50 28           call        qword ptr [rax+28h]
  000000014014D835: 44 8B D8           mov         r11d,eax
  000000014014D838: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014014D83D: 44 3B 58 2C        cmp         r11d,dword ptr [rax+2Ch]
  000000014014D841: 75 0C              jne         000000014014D84F
  000000014014D843: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014014D848: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014014D84D: EB 02              jmp         000000014014D851
  000000014014D84F: EB 9B              jmp         000000014014D7EC
  000000014014D851: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014014D856: 48 83 C4 48        add         rsp,48h
  000000014014D85A: C3                 ret

  Summary

     1E02000 .text
