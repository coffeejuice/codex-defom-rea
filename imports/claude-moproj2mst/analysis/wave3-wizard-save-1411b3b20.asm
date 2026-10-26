Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411B3B20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411B3B25: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  00000001411B3B2C: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001411B3B35: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001411B3B3A: FF 15 80 09 C5 00  call        qword ptr [0000000141E044C0h]
  00000001411B3B40: 90                 nop
  00000001411B3B41: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411B3B49: 48 83 78 30 00     cmp         qword ptr [rax+30h],0
  00000001411B3B4E: 74 16              je          00000001411B3B66
  00000001411B3B50: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411B3B58: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001411B3B5C: E8 2F 52 44 FF     call        00000001405F8D90
  00000001411B3B61: 48 85 C0           test        rax,rax
  00000001411B3B64: 75 1C              jne         00000001411B3B82
  00000001411B3B66: C7 44 24 30 FF FF  mov         dword ptr [rsp+30h],0FFFFFFFFh
                    FF FF
  00000001411B3B6E: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001411B3B73: FF 15 67 1E C5 00  call        qword ptr [0000000141E059E0h]
  00000001411B3B79: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  00000001411B3B7D: E9 A7 00 00 00     jmp         00000001411B3C29
  00000001411B3B82: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411B3B8A: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001411B3B8E: E8 FD 51 44 FF     call        00000001405F8D90
  00000001411B3B93: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001411B3B98: 48 8B C8           mov         rcx,rax
  00000001411B3B9B: E8 50 D2 E9 FE     call        0000000140050DF0
  00000001411B3BA0: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001411B3BA5: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001411B3BAA: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001411B3BAF: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  00000001411B3BB4: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001411B3BB9: FF 15 B1 08 C5 00  call        qword ptr [0000000141E04470h]
  00000001411B3BBF: 90                 nop
  00000001411B3BC0: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001411B3BC5: FF 15 15 1E C5 00  call        qword ptr [0000000141E059E0h]
  00000001411B3BCB: BA 01 00 00 00     mov         edx,1
  00000001411B3BD0: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411B3BD8: E8 B3 72 00 00     call        00000001411BAE90
  00000001411B3BDD: 48 8D 44 24 40     lea         rax,[rsp+40h]
  00000001411B3BE2: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001411B3BE7: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001411B3BEC: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411B3BF1: FF 15 E1 1D C5 00  call        qword ptr [0000000141E059D8h]
  00000001411B3BF7: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001411B3BFC: 48 8B 54 24 70     mov         rdx,qword ptr [rsp+70h]
  00000001411B3C01: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411B3C09: E8 32 00 00 00     call        00000001411B3C40
  00000001411B3C0E: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001411B3C12: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001411B3C16: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  00000001411B3C1A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001411B3C1F: FF 15 BB 1D C5 00  call        qword ptr [0000000141E059E0h]
  00000001411B3C25: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001411B3C29: 48 81 C4 88 00 00  add         rsp,88h
                    00
  00000001411B3C30: C3                 ret

  Summary

     1E02000 .text
