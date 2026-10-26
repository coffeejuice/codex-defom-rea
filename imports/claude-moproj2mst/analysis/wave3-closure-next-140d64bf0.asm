Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D64BF0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D64BF5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D64BFA: 48 83 EC 38        sub         rsp,38h
  0000000140D64BFE: 45 33 C0           xor         r8d,r8d
  0000000140D64C01: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140D64C06: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D64C0B: E8 D0 3F 32 FF     call        0000000140088BE0
  0000000140D64C10: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140D64C14: 83 7C 24 20 FF     cmp         dword ptr [rsp+20h],0FFFFFFFFh
  0000000140D64C19: 74 12              je          0000000140D64C2D
  0000000140D64C1B: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140D64C1F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D64C24: E8 C7 3C AB FF     call        00000001408188F0
  0000000140D64C29: B0 01              mov         al,1
  0000000140D64C2B: EB 02              jmp         0000000140D64C2F
  0000000140D64C2D: 32 C0              xor         al,al
  0000000140D64C2F: 48 83 C4 38        add         rsp,38h
  0000000140D64C33: C3                 ret

  Summary

     1E02000 .text
