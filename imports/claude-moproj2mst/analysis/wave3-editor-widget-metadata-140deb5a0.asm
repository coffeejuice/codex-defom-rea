Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DEB5A0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140DEB5A5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140DEB5AA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140DEB5AE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DEB5B3: 48 83 EC 38        sub         rsp,38h
  0000000140DEB5B7: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  0000000140DEB5BC: 0F 85 B4 00 00 00  jne         0000000140DEB676
  0000000140DEB5C2: E8 B9 95 21 FF     call        0000000140004B80
  0000000140DEB5C7: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  0000000140DEB5CC: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  0000000140DEB5D1: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  0000000140DEB5D5: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140DEB5D9: 83 7C 24 28 06     cmp         dword ptr [rsp+28h],6
  0000000140DEB5DE: 0F 87 92 00 00 00  ja          0000000140DEB676
  0000000140DEB5E4: 48 63 44 24 28     movsxd      rax,dword ptr [rsp+28h]
  0000000140DEB5E9: 48 8D 0D 10 4A 21  lea         rcx,[0000000140000000h]
                    FF
  0000000140DEB5F0: 8B 84 81 7C B6 DE  mov         eax,dword ptr [rcx+rax*4+0000000000DEB67Ch]
                    00
  0000000140DEB5F7: 48 03 C1           add         rax,rcx
  0000000140DEB5FA: FF E0              jmp         rax
  0000000140DEB5FC: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140DEB601: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140DEB605: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140DEB60A: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140DEB60E: 44 0F B6 00        movzx       r8d,byte ptr [rax]
  0000000140DEB612: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140DEB615: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DEB61A: E8 11 2C FC FF     call        0000000140DAE230
  0000000140DEB61F: EB 55              jmp         0000000140DEB676
  0000000140DEB621: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140DEB626: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140DEB62A: 45 33 C0           xor         r8d,r8d
  0000000140DEB62D: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140DEB630: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DEB635: E8 F6 2B FC FF     call        0000000140DAE230
  0000000140DEB63A: EB 3A              jmp         0000000140DEB676
  0000000140DEB63C: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DEB641: E8 7A 2C FC FF     call        0000000140DAE2C0
  0000000140DEB646: EB 2E              jmp         0000000140DEB676
  0000000140DEB648: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DEB64D: E8 EE 2C FC FF     call        0000000140DAE340
  0000000140DEB652: EB 22              jmp         0000000140DEB676
  0000000140DEB654: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DEB659: E8 62 2D FC FF     call        0000000140DAE3C0
  0000000140DEB65E: EB 16              jmp         0000000140DEB676
  0000000140DEB660: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DEB665: E8 E6 2D FC FF     call        0000000140DAE450
  0000000140DEB66A: EB 0A              jmp         0000000140DEB676
  0000000140DEB66C: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DEB671: E8 DA 31 FC FF     call        0000000140DAE850
  0000000140DEB676: 48 83 C4 38        add         rsp,38h
  0000000140DEB67A: C3                 ret
  0000000140DEB67B: 90                 nop
  0000000140DEB67C: FC                 cld
  0000000140DEB67D: B5 DE              mov         ch,0DEh
  0000000140DEB67F: 00 21              add         byte ptr [rcx],ah
  0000000140DEB681: B6 DE              mov         dh,0DEh
  0000000140DEB683: 00 3C B6           add         byte ptr [rsi+rsi*4],bh
  0000000140DEB686: DE 00              fiadd       word ptr [rax]
  0000000140DEB688: 48 B6 DE           mov         sil,0DEh
  0000000140DEB68B: 00 54 B6 DE        add         byte ptr [rsi+rsi*4-22h],dl
  0000000140DEB68F: 00 60 B6           add         byte ptr [rax-4Ah],ah
  0000000140DEB692: DE 00              fiadd       word ptr [rax]
  0000000140DEB694: 6C                 ins         byte ptr [rdi],dx
  0000000140DEB695: B6 DE              mov         dh,0DEh
  0000000140DEB697: 00

  Summary

     1E02000 .text
