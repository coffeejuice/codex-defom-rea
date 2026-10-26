Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C90F10: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C90F15: 48 83 EC 48        sub         rsp,48h
  0000000140C90F19: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C90F22: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C90F27: 48 83 B8 48 02 00  cmp         qword ptr [rax+0000000000000248h],0
                    00 00
  0000000140C90F2F: 75 51              jne         0000000140C90F82
  0000000140C90F31: B9 28 44 00 00     mov         ecx,4428h
  0000000140C90F36: E8 81 DB 7D 00     call        000000014146EABC
  0000000140C90F3B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C90F40: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140C90F46: 74 16              je          0000000140C90F5E
  0000000140C90F48: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140C90F4D: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140C90F52: E8 19 1C 57 00     call        0000000141202B70
  0000000140C90F57: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C90F5C: EB 09              jmp         0000000140C90F67
  0000000140C90F5E: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140C90F67: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140C90F6C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C90F71: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C90F76: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C90F7B: 48 89 81 48 02 00  mov         qword ptr [rcx+0000000000000248h],rax
                    00
  0000000140C90F82: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C90F87: 48 8B 80 48 02 00  mov         rax,qword ptr [rax+0000000000000248h]
                    00
  0000000140C90F8E: 48 83 C4 48        add         rsp,48h
  0000000140C90F92: C3                 ret

  Summary

     1E02000 .text
