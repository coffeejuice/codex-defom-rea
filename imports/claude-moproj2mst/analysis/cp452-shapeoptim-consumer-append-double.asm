Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140140810: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140140815: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014014081A: 48 83 EC 58        sub         rsp,58h
  000000014014081E: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140140827: BA 01 00 00 00     mov         edx,1
  000000014014082C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140140831: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140140834: FF 15 9E 3A CC 01  call        qword ptr [0000000141E042D8h]
  000000014014083A: 0F B6 C0           movzx       eax,al
  000000014014083D: 85 C0              test        eax,eax
  000000014014083F: 74 31              je          0000000140140872
  0000000140140841: 41 B8 01 00 00 00  mov         r8d,1
  0000000140140847: BA FF FF FF 7F     mov         edx,7FFFFFFFh
  000000014014084C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140140851: E8 2A 02 00 00     call        0000000140140A80
  0000000140140856: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014014085B: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140140860: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140140865: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014014086A: E8 61 03 00 00     call        0000000140140BD0
  000000014014086F: 90                 nop
  0000000140140870: EB 69              jmp         00000001401408DB
  0000000140140872: 33 C0              xor         eax,eax
  0000000140140874: 85 C0              test        eax,eax
  0000000140140876: 74 27              je          000000014014089F
  0000000140140878: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014014087D: FF 15 7D 39 CC 01  call        qword ptr [0000000141E04200h]
  0000000140140883: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140140888: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  000000014014088D: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  0000000140140892: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140140897: E8 34 03 00 00     call        0000000140140BD0
  000000014014089C: 90                 nop
  000000014014089D: EB 3C              jmp         00000001401408DB
  000000014014089F: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  00000001401408A4: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001401408A9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001401408AE: E8 1D 03 00 00     call        0000000140140BD0
  00000001401408B3: 90                 nop
  00000001401408B4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001401408B9: FF 15 41 39 CC 01  call        qword ptr [0000000141E04200h]
  00000001401408BF: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001401408C4: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001401408C9: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001401408CE: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001401408D3: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001401408D8: 48 89 01           mov         qword ptr [rcx],rax
  00000001401408DB: 48 83 C4 58        add         rsp,58h
  00000001401408DF: C3                 ret

  Summary

     1E02000 .text
