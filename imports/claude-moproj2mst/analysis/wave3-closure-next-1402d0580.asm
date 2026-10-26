Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402D0580: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402D0585: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402D058A: 48 83 EC 58        sub         rsp,58h
  00000001402D058E: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001402D0597: BA 01 00 00 00     mov         edx,1
  00000001402D059C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001402D05A1: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001402D05A4: FF 15 2E 3D B3 01  call        qword ptr [0000000141E042D8h]
  00000001402D05AA: 0F B6 C0           movzx       eax,al
  00000001402D05AD: 85 C0              test        eax,eax
  00000001402D05AF: 74 31              je          00000001402D05E2
  00000001402D05B1: 41 B8 01 00 00 00  mov         r8d,1
  00000001402D05B7: BA FF FF FF 7F     mov         edx,7FFFFFFFh
  00000001402D05BC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001402D05C1: E8 2A 04 00 00     call        00000001402D09F0
  00000001402D05C6: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001402D05CB: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  00000001402D05D0: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001402D05D5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001402D05DA: E8 61 95 B1 00     call        0000000140DE9B40
  00000001402D05DF: 90                 nop
  00000001402D05E0: EB 69              jmp         00000001402D064B
  00000001402D05E2: 33 C0              xor         eax,eax
  00000001402D05E4: 85 C0              test        eax,eax
  00000001402D05E6: 74 27              je          00000001402D060F
  00000001402D05E8: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001402D05ED: FF 15 0D 3C B3 01  call        qword ptr [0000000141E04200h]
  00000001402D05F3: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001402D05F8: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  00000001402D05FD: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001402D0602: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001402D0607: E8 34 95 B1 00     call        0000000140DE9B40
  00000001402D060C: 90                 nop
  00000001402D060D: EB 3C              jmp         00000001402D064B
  00000001402D060F: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  00000001402D0614: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001402D0619: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001402D061E: E8 1D 95 B1 00     call        0000000140DE9B40
  00000001402D0623: 90                 nop
  00000001402D0624: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001402D0629: FF 15 D1 3B B3 01  call        qword ptr [0000000141E04200h]
  00000001402D062F: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001402D0634: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001402D0639: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001402D063E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001402D0643: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001402D0648: 48 89 01           mov         qword ptr [rcx],rax
  00000001402D064B: 48 83 C4 58        add         rsp,58h
  00000001402D064F: C3                 ret

  Summary

     1E02000 .text
