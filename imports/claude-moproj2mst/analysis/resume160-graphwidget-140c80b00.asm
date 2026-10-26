Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C80B00: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140C80B04: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C80B09: 48 83 EC 38        sub         rsp,38h
  0000000140C80B0D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C80B12: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  0000000140C80B17: 75 05              jne         0000000140C80B1E
  0000000140C80B19: E9 D0 00 00 00     jmp         0000000140C80BEE
  0000000140C80B1E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C80B23: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C80B27: C7 80 04 02 00 00  mov         dword ptr [rax+0000000000000204h],2
                    02 00 00 00
  0000000140C80B31: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  0000000140C80B35: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140C80B39: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80B3E: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C80B42: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140C80B46: 89 81 0C 02 00 00  mov         dword ptr [rcx+000000000000020Ch],eax
  0000000140C80B4C: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  0000000140C80B51: 75 10              jne         0000000140C80B63
  0000000140C80B53: B2 01              mov         dl,1
  0000000140C80B55: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80B5A: E8 41 A6 FF FF     call        0000000140C7B1A0
  0000000140C80B5F: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140C80B63: C7 44 24 24 10 01  mov         dword ptr [rsp+24h],110h
                    00 00
  0000000140C80B6B: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80B70: E8 6B B5 FF FF     call        0000000140C7C0E0
  0000000140C80B75: 0F B6 C0           movzx       eax,al
  0000000140C80B78: 85 C0              test        eax,eax
  0000000140C80B7A: 74 0D              je          0000000140C80B89
  0000000140C80B7C: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  0000000140C80B80: 0D 00 02 00 00     or          eax,200h
  0000000140C80B85: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  0000000140C80B89: 45 33 C9           xor         r9d,r9d
  0000000140C80B8C: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  0000000140C80B92: 8B 54 24 24        mov         edx,dword ptr [rsp+24h]
  0000000140C80B96: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80B9B: E8 50 05 00 00     call        0000000140C810F0
  0000000140C80BA0: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80BA5: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C80BA9: E8 72 BD 08 00     call        0000000140D0C920
  0000000140C80BAE: 0F B6 C0           movzx       eax,al
  0000000140C80BB1: 85 C0              test        eax,eax
  0000000140C80BB3: 75 12              jne         0000000140C80BC7
  0000000140C80BB5: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140C80BB9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80BBE: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140C80BC2: E8 09 E9 07 00     call        0000000140CFF4D0
  0000000140C80BC7: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C80BCC: E8 3F F6 00 00     call        0000000140C90210
  0000000140C80BD1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C80BD6: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C80BDB: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C80BDE: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140C80BE3: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140C80BE8: FF 90 40 01 00 00  call        qword ptr [rax+0000000000000140h]
  0000000140C80BEE: 48 83 C4 38        add         rsp,38h
  0000000140C80BF2: C3                 ret

  Summary

     1E02000 .text
