Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C15DB0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C15DB5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C15DBA: 48 83 EC 78        sub         rsp,78h
  0000000140C15DBE: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C15DC7: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C15DCF: EB 0B              jmp         0000000140C15DDC
  0000000140C15DD1: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140C15DD5: 83 C0 01           add         eax,1
  0000000140C15DD8: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140C15DDC: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C15DE4: 48 83 C1 10        add         rcx,10h
  0000000140C15DE8: E8 F3 EC 3E FF     call        0000000140004AE0
  0000000140C15DED: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  0000000140C15DF1: 0F 8D F9 00 00 00  jge         0000000140C15EF0
  0000000140C15DF7: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C15DFF: 48 83 C1 10        add         rcx,10h
  0000000140C15E03: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140C15E07: E8 44 2A 23 00     call        0000000140E48850
  0000000140C15E0C: 48 8B C8           mov         rcx,rax
  0000000140C15E0F: E8 DC 50 00 00     call        0000000140C1AEF0
  0000000140C15E14: 48 85 C0           test        rax,rax
  0000000140C15E17: 0F 84 CE 00 00 00  je          0000000140C15EEB
  0000000140C15E1D: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C15E25: 48 83 C1 10        add         rcx,10h
  0000000140C15E29: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140C15E2D: E8 1E 2A 23 00     call        0000000140E48850
  0000000140C15E32: 48 8B C8           mov         rcx,rax
  0000000140C15E35: E8 B6 50 00 00     call        0000000140C1AEF0
  0000000140C15E3A: 48 8B C8           mov         rcx,rax
  0000000140C15E3D: E8 1E 81 FF FF     call        0000000140C0DF60
  0000000140C15E42: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140C15E4A: 48 8B C8           mov         rcx,rax
  0000000140C15E4D: FF 15 B5 E0 1E 01  call        qword ptr [0000000141E03F08h]
  0000000140C15E53: 0F B6 C0           movzx       eax,al
  0000000140C15E56: 85 C0              test        eax,eax
  0000000140C15E58: 0F 84 8D 00 00 00  je          0000000140C15EEB
  0000000140C15E5E: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C15E66: 48 83 C1 10        add         rcx,10h
  0000000140C15E6A: 44 8B 44 24 20     mov         r8d,dword ptr [rsp+20h]
  0000000140C15E6F: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140C15E74: E8 37 12 00 00     call        0000000140C170B0
  0000000140C15E79: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C15E7E: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C15E83: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C15E88: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C15E8D: E8 5E 50 00 00     call        0000000140C1AEF0
  0000000140C15E92: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C15E97: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C15E9C: E8 5F 6A 0A 00     call        0000000140CBC900
  0000000140C15EA1: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C15EA6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C15EAB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C15EB0: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C15EB5: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140C15EBB: 74 1C              je          0000000140C15ED9
  0000000140C15EBD: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140C15EC2: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C15EC5: BA 01 00 00 00     mov         edx,1
  0000000140C15ECA: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140C15ECF: FF 50 18           call        qword ptr [rax+18h]
  0000000140C15ED2: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140C15ED7: EB 09              jmp         0000000140C15EE2
  0000000140C15ED9: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000140C15EE2: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140C15EEB: E9 E1 FE FF FF     jmp         0000000140C15DD1
  0000000140C15EF0: B0 01              mov         al,1
  0000000140C15EF2: 48 83 C4 78        add         rsp,78h
  0000000140C15EF6: C3                 ret
  0000000140C15EF7: CC                 int         3

  Summary

     1E02000 .text
