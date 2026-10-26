Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140896640: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140896645: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014089664A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089664F: 48 83 EC 58        sub         rsp,58h
  0000000140896653: 8B 44 24 70        mov         eax,dword ptr [rsp+70h]
  0000000140896657: 89 44 24 2C        mov         dword ptr [rsp+2Ch],eax
  000000014089665B: C6 44 24 28 01     mov         byte ptr [rsp+28h],1
  0000000140896660: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140896669: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140896671: 48 8D 15 1C 67 F1  lea         rdx,[00000001427ACD94h]
                    01
  0000000140896678: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  000000014089667D: E8 56 96 BD 00     call        000000014146FCD8
  0000000140896682: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140896687: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014089668D: 75 0A              jne         0000000140896699
  000000014089668F: B8 01 00 00 00     mov         eax,1
  0000000140896694: E9 9F 00 00 00     jmp         0000000140896738
  0000000140896699: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  00000001408966A1: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  00000001408966A6: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001408966AB: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408966B0: E8 AB EE FF FF     call        0000000140895560
  00000001408966B5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408966BA: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  00000001408966C1: 8B 54 24 70        mov         edx,dword ptr [rsp+70h]
  00000001408966C5: E8 46 86 FB FF     call        000000014084ED10
  00000001408966CA: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001408966CF: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001408966D4: E8 C7 E8 F9 FF     call        0000000140834FA0
  00000001408966D9: 0F B6 C0           movzx       eax,al
  00000001408966DC: 85 C0              test        eax,eax
  00000001408966DE: 74 11              je          00000001408966F1
  00000001408966E0: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001408966E5: E8 96 E5 F9 FF     call        0000000140834C80
  00000001408966EA: 0F B6 C0           movzx       eax,al
  00000001408966ED: 85 C0              test        eax,eax
  00000001408966EF: 75 07              jne         00000001408966F8
  00000001408966F1: B8 03 00 00 00     mov         eax,3
  00000001408966F6: EB 40              jmp         0000000140896738
  00000001408966F8: 44 8B 4C 24 2C     mov         r9d,dword ptr [rsp+2Ch]
  00000001408966FD: 4C 8B 44 24 38     mov         r8,qword ptr [rsp+38h]
  0000000140896702: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140896707: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014089670C: E8 4F 24 0E 00     call        0000000140978B60
  0000000140896711: 44 8B 4C 24 2C     mov         r9d,dword ptr [rsp+2Ch]
  0000000140896716: 4C 8B 44 24 38     mov         r8,qword ptr [rsp+38h]
  000000014089671B: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140896720: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140896725: E8 46 27 0E 00     call        0000000140978E70
  000000014089672A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014089672F: E8 F4 93 BD 00     call        000000014146FB28
  0000000140896734: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140896738: 48 83 C4 58        add         rsp,58h
  000000014089673C: C3                 ret

  Summary

     1E02000 .text
