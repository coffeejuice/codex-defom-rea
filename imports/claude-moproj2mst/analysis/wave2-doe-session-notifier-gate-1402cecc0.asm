Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CECC0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CECC5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001402CECCA: C6 40 78 00        mov         byte ptr [rax+78h],0
  00000001402CECCE: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001402CECD3: 48 C7 80 80 00 00  mov         qword ptr [rax+0000000000000080h],0
                    00 00 00 00 00
  00000001402CECDE: C3                 ret
  00000001402CECDF: CC                 int         3
  00000001402CECE0: CC                 int         3
  00000001402CECE1: CC                 int         3
  00000001402CECE2: CC                 int         3
  00000001402CECE3: CC                 int         3
  00000001402CECE4: CC                 int         3
  00000001402CECE5: CC                 int         3
  00000001402CECE6: CC                 int         3
  00000001402CECE7: CC                 int         3
  00000001402CECE8: CC                 int         3
  00000001402CECE9: CC                 int         3
  00000001402CECEA: CC                 int         3
  00000001402CECEB: CC                 int         3
  00000001402CECEC: CC                 int         3
  00000001402CECED: CC                 int         3
  00000001402CECEE: CC                 int         3
  00000001402CECEF: CC                 int         3
  00000001402CECF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CECF5: 48 83 EC 28        sub         rsp,28h
  00000001402CECF9: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402CECFE: 0F B6 40 78        movzx       eax,byte ptr [rax+78h]
  00000001402CED02: 85 C0              test        eax,eax
  00000001402CED04: 75 02              jne         00000001402CED08
  00000001402CED06: EB 26              jmp         00000001402CED2E
  00000001402CED08: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402CED0D: 0F B6 80 41 01 00  movzx       eax,byte ptr [rax+0000000000000141h]
                    00
  00000001402CED14: 85 C0              test        eax,eax
  00000001402CED16: 75 16              jne         00000001402CED2E
  00000001402CED18: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000001402CED1D: 48
  00000001402CED1E: 8B
  00000001402CED1F: 92                 xchg        eax,edx

  Summary

     1E02000 .text
