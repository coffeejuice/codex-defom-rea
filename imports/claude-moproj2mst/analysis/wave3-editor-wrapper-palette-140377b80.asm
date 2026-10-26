Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140377B80: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140377B84: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140377B89: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  0000000140377B8E: 8B 44 24 10        mov         eax,dword ptr [rsp+10h]
  0000000140377B92: 89 41 30           mov         dword ptr [rcx+30h],eax
  0000000140377B95: C3                 ret
  0000000140377B96: CC                 int         3
  0000000140377B97: CC                 int         3
  0000000140377B98: CC                 int         3
  0000000140377B99: CC                 int         3
  0000000140377B9A: CC                 int         3
  0000000140377B9B: CC                 int         3
  0000000140377B9C: CC                 int         3
  0000000140377B9D: CC                 int         3
  0000000140377B9E: CC                 int         3
  0000000140377B9F: CC                 int         3
  0000000140377BA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140377BA5: 48 83 EC 28        sub         rsp,28h
  0000000140377BA9: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140377BAE: 83 78 30 00        cmp         dword ptr [rax+30h],0
  0000000140377BB2: 7E 0A              jle         0000000140377BBE
  0000000140377BB4: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140377BB9: 8B 40 30           mov         eax,dword ptr [rax+30h]
  0000000140377BBC: EB 2F              jmp         0000000140377BED
  0000000140377BBE: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140377BC3: 48 83 C1 28        add         rcx,28h
  0000000140377BC7: E8 24 33 8A 00     call        0000000140C1AEF0
  0000000140377BCC: 48 85 C0           test        rax,rax
  0000000140377BCF: 75 04              jne         0000000140377BD5
  0000000140377BD1: 33 C0              xor         eax,eax
  0000000140377BD3: EB 18              jmp         0000000140377BED
  0000000140377BD5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140377BDA: 48 83 C1 28        add         rcx,28h
  0000000140377BDE: E8
  0000000140377BDF: 0D

  Summary

     1E02000 .text
