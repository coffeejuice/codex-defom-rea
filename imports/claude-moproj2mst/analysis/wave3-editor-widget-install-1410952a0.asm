Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001410952A0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001410952A4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001410952A9: 48 83 EC 38        sub         rsp,38h
  00000001410952AD: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  00000001410952B2: 7C 07              jl          00000001410952BB
  00000001410952B4: 83 7C 24 48 07     cmp         dword ptr [rsp+48h],7
  00000001410952B9: 7C 05              jl          00000001410952C0
  00000001410952BB: E9 A2 00 00 00     jmp         0000000141095362
  00000001410952C0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001410952C5: 48 8B 80 A0 11 00  mov         rax,qword ptr [rax+00000000000011A0h]
                    00
  00000001410952CC: 48 83 78 08 00     cmp         qword ptr [rax+8],0
  00000001410952D1: 75 05              jne         00000001410952D8
  00000001410952D3: E9 8A 00 00 00     jmp         0000000141095362
  00000001410952D8: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001410952E0: EB 0B              jmp         00000001410952ED
  00000001410952E2: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001410952E6: 83 C0 01           add         eax,1
  00000001410952E9: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001410952ED: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001410952F2: 48 8B 89 A0 11 00  mov         rcx,qword ptr [rcx+00000000000011A0h]
                    00
  00000001410952F9: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001410952FD: FF 15 7D 52 D7 00  call        qword ptr [0000000141E0A580h]
  0000000141095303: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  0000000141095307: 7D 3F              jge         0000000141095348
  0000000141095309: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  000000014109530D: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  0000000141095311: 74 33              je          0000000141095346
  0000000141095313: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141095318: 48 8B 89 A0 11 00  mov         rcx,qword ptr [rcx+00000000000011A0h]
                    00
  000000014109531F: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000141095323: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000141095327: FF 15 F3 64 D7 00  call        qword ptr [0000000141E0B820h]
  000000014109532D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141095332: 48 8B 89 A0 11 00  mov         rcx,qword ptr [rcx+00000000000011A0h]
                    00
  0000000141095339: 48 8B D0           mov         rdx,rax
  000000014109533C: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000141095340: FF 15 8A 52 D7 00  call        qword ptr [0000000141E0A5D0h]
  0000000141095346: EB 9A              jmp         00000001410952E2
  0000000141095348: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014109534D: 48 8B 89 A0 11 00  mov         rcx,qword ptr [rcx+00000000000011A0h]
                    00
  0000000141095354: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  0000000141095358: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014109535C: FF 15 06 68 D7 00  call        qword ptr [0000000141E0BB68h]
  0000000141095362: 48 83 C4 38        add         rsp,38h
  0000000141095366: C3                 ret

  Summary

     1E02000 .text
