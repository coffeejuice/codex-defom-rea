Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D302C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D302C5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140D302CA: 48 C7 80 70 04 00  mov         qword ptr [rax+0000000000000470h],0
                    00 00 00 00 00
  0000000140D302D5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140D302DA: C7 80 78 04 00 00  mov         dword ptr [rax+0000000000000478h],0FFFFFFFFh
                    FF FF FF FF
  0000000140D302E4: C3                 ret
  0000000140D302E5: CC                 int         3
  0000000140D302E6: CC                 int         3
  0000000140D302E7: CC                 int         3
  0000000140D302E8: CC                 int         3
  0000000140D302E9: CC                 int         3
  0000000140D302EA: CC                 int         3
  0000000140D302EB: CC                 int         3
  0000000140D302EC: CC                 int         3
  0000000140D302ED: CC                 int         3
  0000000140D302EE: CC                 int         3
  0000000140D302EF: CC                 int         3
  0000000140D302F0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140D302F5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D302FA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D302FF: 48 83 EC 28        sub         rsp,28h
  0000000140D30303: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D30308: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D3030C: 39 81 78 04 00 00  cmp         dword ptr [rcx+0000000000000478h],eax
  0000000140D30312: 74 0A              je          0000000140D3031E
  0000000140D30314: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D30319: E8 A2 FF FF FF     call        0000000140D302C0
  0000000140D3031E: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140D30324: 74 20              je          0000000140D30346
  0000000140D30326: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D3032B: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D30330: 48 89 81 70 04 00  mov         qword ptr [rcx+0000000000000470h],rax
                    00
  0000000140D30337: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D3033C: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D30340: 89 81 78 04 00 00  mov         dword ptr [rcx+0000000000000478h],eax
  0000000140D30346: 48 83 C4 28        add         rsp,28h
  0000000140D3034A: C3                 ret
  0000000140D3034B: CC                 int         3
  0000000140D3034C: CC                 int         3
  0000000140D3034D: CC                 int         3
  0000000140D3034E: CC                 int         3
  0000000140D3034F: CC                 int         3
  0000000140D30350: CC                 int         3
  0000000140D30351: CC                 int         3
  0000000140D30352: CC                 int         3
  0000000140D30353: CC                 int         3
  0000000140D30354: CC                 int         3
  0000000140D30355: CC                 int         3
  0000000140D30356: CC                 int         3
  0000000140D30357: CC                 int         3
  0000000140D30358: CC                 int         3
  0000000140D30359: CC                 int         3
  0000000140D3035A: CC                 int         3
  0000000140D3035B: CC                 int         3
  0000000140D3035C: CC                 int         3
  0000000140D3035D: CC                 int         3
  0000000140D3035E: CC                 int         3
  0000000140D3035F: CC                 int         3
  0000000140D30360: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140D30364: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D30369: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  0000000140D3036E: 8B 44 24 10        mov         eax,dword ptr [rsp+10h]
  0000000140D30372: 39 81 78 04 00 00  cmp         dword ptr [rcx+0000000000000478h],eax
  0000000140D30378: 75 0E              jne         0000000140D30388
  0000000140D3037A: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140D3037F: 48 8B 80 70 04 00  mov         rax,qword ptr [rax+0000000000000470h]
                    00
  0000000140D30386: EB 02              jmp         0000000140D3038A
  0000000140D30388: 33 C0              xor         eax,eax
  0000000140D3038A: F3 C3              rep ret
  0000000140D3038C: CC                 int         3
  0000000140D3038D: CC                 int         3
  0000000140D3038E: CC                 int         3
  0000000140D3038F: CC                 int         3
  0000000140D30390: CC                 int         3
  0000000140D30391: CC                 int         3
  0000000140D30392: CC                 int         3
  0000000140D30393: CC                 int         3
  0000000140D30394: CC                 int         3
  0000000140D30395: CC                 int         3
  0000000140D30396: CC                 int         3
  0000000140D30397: CC                 int         3
  0000000140D30398: CC                 int         3
  0000000140D30399: CC                 int         3
  0000000140D3039A: CC                 int         3
  0000000140D3039B: CC                 int         3
  0000000140D3039C: CC                 int         3
  0000000140D3039D: CC                 int         3
  0000000140D3039E: CC                 int         3
  0000000140D3039F: CC                 int         3
  0000000140D303A0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D303A5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D303AA: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  0000000140D303AF: 48 8B 44 24 10     mov         rax,qword ptr [rsp+10h]
  0000000140D303B4: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000140D303B7: 88 81 7C 04 00 00  mov         byte ptr [rcx+000000000000047Ch],al
  0000000140D303BD: C3                 ret
  0000000140D303BE: CC                 int         3
  0000000140D303BF: CC                 int         3
  0000000140D303C0: CC                 int         3

  Summary

     1E02000 .text
