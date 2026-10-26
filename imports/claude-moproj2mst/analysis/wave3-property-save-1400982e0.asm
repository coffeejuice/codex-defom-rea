Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400982E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400982E5: 48 83 EC 68        sub         rsp,68h
  00000001400982E9: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001400982F2: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  00000001400982FB: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140098304: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140098309: E8 A2 D5 00 00     call        00000001400A58B0
  000000014009830E: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140098313: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140098318: 48 8B 00           mov         rax,qword ptr [rax]
  000000014009831B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140098320: FF 90 B8 01 00 00  call        qword ptr [rax+00000000000001B8h]
  0000000140098326: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000014009832A: C7 44 24 48 00 00  mov         dword ptr [rsp+48h],0
                    00 00
  0000000140098332: EB 0B              jmp         000000014009833F
  0000000140098334: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  0000000140098338: 83 C0 01           add         eax,1
  000000014009833B: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  000000014009833F: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140098343: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  0000000140098347: 7D 67              jge         00000001400983B0
  0000000140098349: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014009834E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140098351: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140098356: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014009835B: FF 90 C0 01 00 00  call        qword ptr [rax+00000000000001C0h]
  0000000140098361: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140098366: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014009836B: E8 A0 F5 FA FF     call        0000000140047910
  0000000140098370: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140098375: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014009837A: E8 11 DC FA FF     call        0000000140045F90
  000000014009837F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140098384: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140098389: E8 A2 8E FB FF     call        0000000140051230
  000000014009838E: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140098393: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  0000000140098399: 74 13              je          00000001400983AE
  000000014009839B: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400983A0: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400983A3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400983A8: FF 90 B8 00 00 00  call        qword ptr [rax+00000000000000B8h]
  00000001400983AE: EB 84              jmp         0000000140098334
  00000001400983B0: B0 01              mov         al,1
  00000001400983B2: 48 83 C4 68        add         rsp,68h
  00000001400983B6: C3                 ret

  Summary

     1E02000 .text
