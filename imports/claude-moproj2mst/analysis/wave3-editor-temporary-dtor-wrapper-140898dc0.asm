Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140898DC0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140898DC5: 48 83 EC 38        sub         rsp,38h
  0000000140898DC9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898DCE: C7 40 58 00 00 00  mov         dword ptr [rax+58h],0
                    00
  0000000140898DD5: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140898DDD: EB 0B              jmp         0000000140898DEA
  0000000140898DDF: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140898DE3: 83 C0 01           add         eax,1
  0000000140898DE6: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140898DEA: 83 7C 24 20 03     cmp         dword ptr [rsp+20h],3
  0000000140898DEF: 7D 26              jge         0000000140898E17
  0000000140898DF1: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  0000000140898DF6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898DFB: C7 44 88 10 00 00  mov         dword ptr [rax+rcx*4+10h],0
                    00 00
  0000000140898E03: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  0000000140898E08: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898E0D: C7 44 88 1C 00 00  mov         dword ptr [rax+rcx*4+1Ch],0
                    00 00
  0000000140898E15: EB C8              jmp         0000000140898DDF
  0000000140898E17: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898E1C: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  0000000140898E24: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898E29: 48 C7 40 38 00 00  mov         qword ptr [rax+38h],0
                    00 00
  0000000140898E31: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898E36: 48 C7 40 30 00 00  mov         qword ptr [rax+30h],0
                    00 00
  0000000140898E3E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898E43: 48 C7 40 28 00 00  mov         qword ptr [rax+28h],0
                    00 00
  0000000140898E4B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898E50: 48 C7 40 68 00 00  mov         qword ptr [rax+68h],0
                    00 00
  0000000140898E58: 33 D2              xor         edx,edx
  0000000140898E5A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140898E5F: E8 AC 01 00 00     call        0000000140899010
  0000000140898E64: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140898E69: 48 83 C4 38        add         rsp,38h
  0000000140898E6D: C3                 ret

  Summary

     1E02000 .text
