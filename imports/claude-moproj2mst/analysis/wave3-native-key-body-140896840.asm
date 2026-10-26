Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140896840: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140896845: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014089684A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089684F: 48 83 EC 58        sub         rsp,58h
  0000000140896853: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  000000014089685B: 48 8D 15 3A 65 F1  lea         rdx,[00000001427ACD9Ch]
                    01
  0000000140896862: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140896867: E8 6C 94 BD 00     call        000000014146FCD8
  000000014089686C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140896871: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140896877: 75 0A              jne         0000000140896883
  0000000140896879: B8 03 00 00 00     mov         eax,3
  000000014089687E: E9 B7 00 00 00     jmp         000000014089693A
  0000000140896883: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140896888: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  000000014089688F: 8B 54 24 70        mov         edx,dword ptr [rsp+70h]
  0000000140896893: E8 78 84 FB FF     call        000000014084ED10
  0000000140896898: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014089689D: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  00000001408968A5: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  00000001408968AA: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001408968AF: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408968B4: E8 A7 EC FF FF     call        0000000140895560
  00000001408968B9: 45 33 C9           xor         r9d,r9d
  00000001408968BC: 4C 8B 44 24 30     mov         r8,qword ptr [rsp+30h]
  00000001408968C1: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001408968C6: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408968CB: E8 60 71 0E 00     call        000000014097DA30
  00000001408968D0: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408968D5: 83 B8 60 23 00 00  cmp         dword ptr [rax+0000000000002360h],0
                    00
  00000001408968DC: 7E 36              jle         0000000140896914
  00000001408968DE: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408968E3: 8B 90 60 23 00 00  mov         edx,dword ptr [rax+0000000000002360h]
  00000001408968E9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408968EE: E8 7D 99 F9 FF     call        0000000140830270
  00000001408968F3: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001408968F8: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001408968FE: 74 14              je          0000000140896914
  0000000140896900: 4C 8B 44 24 38     mov         r8,qword ptr [rsp+38h]
  0000000140896905: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  000000014089690A: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014089690F: E8 5C 78 0E 00     call        000000014097E170
  0000000140896914: 4C 8B 44 24 30     mov         r8,qword ptr [rsp+30h]
  0000000140896919: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  000000014089691E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140896923: E8 78 79 15 00     call        00000001409EE2A0
  0000000140896928: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000014089692C: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140896931: E8 F2 91 BD 00     call        000000014146FB28
  0000000140896936: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  000000014089693A: 48 83 C4 58        add         rsp,58h
  000000014089693E: C3                 ret

  Summary

     1E02000 .text
