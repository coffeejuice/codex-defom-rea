Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C36910: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140C36915: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140C3691A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140C3691E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C36923: 48 83 EC 58        sub         rsp,58h
  0000000140C36927: 83 7C 24 68 00     cmp         dword ptr [rsp+68h],0
  0000000140C3692C: 0F 85 84 00 00 00  jne         0000000140C369B6
  0000000140C36932: E8 49 E2 3C FF     call        0000000140004B80
  0000000140C36937: 4C 8B 5C 24 60     mov         r11,qword ptr [rsp+60h]
  0000000140C3693C: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  0000000140C36941: 8B 44 24 70        mov         eax,dword ptr [rsp+70h]
  0000000140C36945: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140C36949: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140C3694E: 74 10              je          0000000140C36960
  0000000140C36950: 83 7C 24 38 01     cmp         dword ptr [rsp+38h],1
  0000000140C36955: 74 15              je          0000000140C3696C
  0000000140C36957: 83 7C 24 38 02     cmp         dword ptr [rsp+38h],2
  0000000140C3695C: 74 42              je          0000000140C369A0
  0000000140C3695E: EB 56              jmp         0000000140C369B6
  0000000140C36960: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C36965: E8 76 66 FD FF     call        0000000140C0CFE0
  0000000140C3696A: EB 4A              jmp         0000000140C369B6
  0000000140C3696C: 48 8D 44 24 28     lea         rax,[rsp+28h]
  0000000140C36971: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C36976: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140C3697B: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140C3697F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C36984: FF 15 4E F0 1C 01  call        qword ptr [0000000141E059D8h]
  0000000140C3698A: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C3698F: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140C36994: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C36999: E8 12 69 FD FF     call        0000000140C0D2B0
  0000000140C3699E: EB 16              jmp         0000000140C369B6
  0000000140C369A0: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140C369A5: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140C369A9: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140C369AC: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C369B1: E8 3A 6C FD FF     call        0000000140C0D5F0
  0000000140C369B6: 48 83 C4 58        add         rsp,58h
  0000000140C369BA: C3                 ret

  Summary

     1E02000 .text
