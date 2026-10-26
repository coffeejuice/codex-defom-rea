Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D488B0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140D488B5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140D488BA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140D488BE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D488C3: 48 83 EC 38        sub         rsp,38h
  0000000140D488C7: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  0000000140D488CC: 75 65              jne         0000000140D48933
  0000000140D488CE: E8 AD C2 2B FF     call        0000000140004B80
  0000000140D488D3: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  0000000140D488D8: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  0000000140D488DD: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  0000000140D488E1: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140D488E5: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  0000000140D488EA: 74 09              je          0000000140D488F5
  0000000140D488EC: 83 7C 24 28 01     cmp         dword ptr [rsp+28h],1
  0000000140D488F1: 74 27              je          0000000140D4891A
  0000000140D488F3: EB 3E              jmp         0000000140D48933
  0000000140D488F5: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140D488FA: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140D488FE: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140D48903: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140D48907: 44 0F B6 00        movzx       r8d,byte ptr [rax]
  0000000140D4890B: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140D4890E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D48913: E8 A8 C6 FC FF     call        0000000140D14FC0
  0000000140D48918: EB 19              jmp         0000000140D48933
  0000000140D4891A: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140D4891F: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140D48923: 45 33 C0           xor         r8d,r8d
  0000000140D48926: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140D48929: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D4892E: E8 8D C6 FC FF     call        0000000140D14FC0
  0000000140D48933: 48 83 C4 38        add         rsp,38h
  0000000140D48937: C3                 ret

  Summary

     1E02000 .text
