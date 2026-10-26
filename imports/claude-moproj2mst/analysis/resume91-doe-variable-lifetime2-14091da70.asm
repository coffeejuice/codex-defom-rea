Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091DA70: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091DA75: 48 83 EC 48        sub         rsp,48h
  000000014091DA79: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091DA82: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  000000014091DA8A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014091DA8F: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091DA92: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000014091DA97: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091DA9C: FF 90 D8 01 00 00  call        qword ptr [rax+00000000000001D8h]
  000000014091DAA2: 90                 nop
  000000014091DAA3: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091DAAB: EB 0B              jmp         000000014091DAB8
  000000014091DAAD: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091DAB1: 83 C0 01           add         eax,1
  000000014091DAB4: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091DAB8: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091DABD: E8 1E 70 6E FF     call        0000000140004AE0
  000000014091DAC2: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014091DAC6: 7D 43              jge         000000014091DB0B
  000000014091DAC8: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091DACD: 48 83 C1 38        add         rcx,38h
  000000014091DAD1: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091DAD5: E8 36 31 84 FF     call        0000000140160C10
  000000014091DADA: 48 8B 08           mov         rcx,qword ptr [rax]
  000000014091DADD: E8 9E 2E 7E FF     call        0000000140100980
  000000014091DAE2: 3B 44 24 30        cmp         eax,dword ptr [rsp+30h]
  000000014091DAE6: 7C 21              jl          000000014091DB09
  000000014091DAE8: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091DAED: 48 83 C1 38        add         rcx,38h
  000000014091DAF1: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091DAF5: E8 16 31 84 FF     call        0000000140160C10
  000000014091DAFA: 48 8B 08           mov         rcx,qword ptr [rax]
  000000014091DAFD: E8 7E 2E 7E FF     call        0000000140100980
  000000014091DB02: 83 C0 01           add         eax,1
  000000014091DB05: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  000000014091DB09: EB A2              jmp         000000014091DAAD
  000000014091DB0B: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  000000014091DB0F: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  000000014091DB13: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091DB18: E8 83 A6 8E FF     call        00000001402081A0
  000000014091DB1D: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  000000014091DB21: 48 83 C4 48        add         rsp,48h
  000000014091DB25: C3                 ret

  Summary

     1E02000 .text
