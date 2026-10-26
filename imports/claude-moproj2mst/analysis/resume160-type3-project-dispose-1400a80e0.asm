Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400A80E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400A80E5: 48 83 EC 58        sub         rsp,58h
  00000001400A80E9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400A80EE: E8 6D F2 FF FF     call        00000001400A7360
  00000001400A80F3: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001400A80F7: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000001400A8100: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000001400A8108: EB 0B              jmp         00000001400A8115
  00000001400A810A: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  00000001400A810E: 83 C0 01           add         eax,1
  00000001400A8111: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  00000001400A8115: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001400A8119: 39 44 24 30        cmp         dword ptr [rsp+30h],eax
  00000001400A811D: 7D 58              jge         00000001400A8177
  00000001400A811F: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400A8123: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400A8128: E8 C3 F3 FF FF     call        00000001400A74F0
  00000001400A812D: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400A8132: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001400A8138: 74 3B              je          00000001400A8175
  00000001400A813A: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001400A813F: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400A8144: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400A8149: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400A814E: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001400A8154: 74 16              je          00000001400A816C
  00000001400A8156: BA 01 00 00 00     mov         edx,1
  00000001400A815B: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001400A8160: E8 CB 41 00 00     call        00000001400AC330
  00000001400A8165: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400A816A: EB 09              jmp         00000001400A8175
  00000001400A816C: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  00000001400A8175: EB 93              jmp         00000001400A810A
  00000001400A8177: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400A817C: 48 83 C1 58        add         rcx,58h
  00000001400A8180: E8 0B 5A 16 00     call        000000014020DB90
  00000001400A8185: 48 83 C4 58        add         rsp,58h
  00000001400A8189: C3                 ret

  Summary

     1E02000 .text
