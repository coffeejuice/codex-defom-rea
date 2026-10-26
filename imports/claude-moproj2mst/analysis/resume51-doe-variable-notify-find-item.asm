Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140086820: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140086824: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140086829: 48 83 EC 48        sub         rsp,48h
  000000014008682D: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140086836: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  000000014008683E: EB 0B              jmp         000000014008684B
  0000000140086840: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140086844: 83 C0 01           add         eax,1
  0000000140086847: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000014008684B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140086850: 48 83 C1 20        add         rcx,20h
  0000000140086854: E8 87 E2 F7 FF     call        0000000140004AE0
  0000000140086859: 39 44 24 28        cmp         dword ptr [rsp+28h],eax
  000000014008685D: 7D 4E              jge         00000001400868AD
  000000014008685F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140086864: 48 83 C1 20        add         rcx,20h
  0000000140086868: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  000000014008686C: E8 9F A3 0D 00     call        0000000140160C10
  0000000140086871: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140086874: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140086879: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014008687E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140086881: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140086886: FF 50 28           call        qword ptr [rax+28h]
  0000000140086889: 3B 44 24 58        cmp         eax,dword ptr [rsp+58h]
  000000014008688D: 75 1C              jne         00000001400868AB
  000000014008688F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140086894: 48 83 C1 20        add         rcx,20h
  0000000140086898: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  000000014008689C: E8 6F A3 0D 00     call        0000000140160C10
  00000001400868A1: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400868A4: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400868A9: EB 02              jmp         00000001400868AD
  00000001400868AB: EB 93              jmp         0000000140086840
  00000001400868AD: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400868B2: 48 83 C4 48        add         rsp,48h
  00000001400868B6: C3                 ret

  Summary

     1E02000 .text
