Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140895F30: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140895F35: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140895F3A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140895F3F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140895F44: 48 83 EC 58        sub         rsp,58h
  0000000140895F48: C6 44 24 38 01     mov         byte ptr [rsp+38h],1
  0000000140895F4D: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140895F56: C7 44 24 3C 00 00  mov         dword ptr [rsp+3Ch],0
                    00 00
  0000000140895F5E: 83 BC 24 80 00 00  cmp         dword ptr [rsp+0000000000000080h],0
                    00 00
  0000000140895F66: 75 0B              jne         0000000140895F73
  0000000140895F68: 8B 44 24 70        mov         eax,dword ptr [rsp+70h]
  0000000140895F6C: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  0000000140895F73: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140895F79: 74 10              je          0000000140895F8B
  0000000140895F7B: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140895F80: E8 FB AB BD 00     call        0000000141470B80
  0000000140895F85: 48 83 F8 01        cmp         rax,1
  0000000140895F89: 73 0A              jae         0000000140895F95
  0000000140895F8B: B8 01 00 00 00     mov         eax,1
  0000000140895F90: E9 88 00 00 00     jmp         000000014089601D
  0000000140895F95: 48 8D 15 08 6D F1  lea         rdx,[00000001427ACCA4h]
                    01
  0000000140895F9C: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140895FA1: E8 32 9D BD 00     call        000000014146FCD8
  0000000140895FA6: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140895FAB: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140895FB1: 75 07              jne         0000000140895FBA
  0000000140895FB3: B8 01 00 00 00     mov         eax,1
  0000000140895FB8: EB 63              jmp         000000014089601D
  0000000140895FBA: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140895FBF: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140895FC4: 8B 00              mov         eax,dword ptr [rax]
  0000000140895FC6: 89 81 44 17 00 00  mov         dword ptr [rcx+0000000000001744h],eax
  0000000140895FCC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140895FD1: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  0000000140895FD8: 8B 54 24 70        mov         edx,dword ptr [rsp+70h]
  0000000140895FDC: E8 2F 8D FB FF     call        000000014084ED10
  0000000140895FE1: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140895FE6: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140895FEB: 88 44 24 20        mov         byte ptr [rsp+20h],al
  0000000140895FEF: 44 8B 8C 24 80 00  mov         r9d,dword ptr [rsp+0000000000000080h]
                    00 00
  0000000140895FF7: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  0000000140895FFC: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140896001: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140896006: E8 F5 DF 0D 00     call        0000000140974000
  000000014089600B: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  000000014089600F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140896014: E8 0F 9B BD 00     call        000000014146FB28
  0000000140896019: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  000000014089601D: 48 83 C4 58        add         rsp,58h
  0000000140896021: C3                 ret

  Summary

     1E02000 .text
