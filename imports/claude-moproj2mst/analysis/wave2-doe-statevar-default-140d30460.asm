Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D30460: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D30465: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  0000000140D3046A: C6 80 7D 04 00 00  mov         byte ptr [rax+000000000000047Dh],0
                    00
  0000000140D30471: C3                 ret
  0000000140D30472: CC                 int         3
  0000000140D30473: CC                 int         3
  0000000140D30474: CC                 int         3
  0000000140D30475: CC                 int         3
  0000000140D30476: CC                 int         3
  0000000140D30477: CC                 int         3
  0000000140D30478: CC                 int         3
  0000000140D30479: CC                 int         3
  0000000140D3047A: CC                 int         3
  0000000140D3047B: CC                 int         3
  0000000140D3047C: CC                 int         3
  0000000140D3047D: CC                 int         3
  0000000140D3047E: CC                 int         3
  0000000140D3047F: CC                 int         3
  0000000140D30480: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D30485: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D3048A: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  0000000140D30491: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000140D3049A: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  0000000140D304A3: 48 C7 44 24 58 00  mov         qword ptr [rsp+58h],0
                    00 00 00
  0000000140D304AC: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000140D304B4: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D304BC: E8 8F 39 00 00     call        0000000140D33E50
  0000000140D304C1: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140D304C6: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  0000000140D304CC: 75 05              jne         0000000140D304D3
  0000000140D304CE: E9 93 01 00 00     jmp         0000000140D30666
  0000000140D304D3: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000140D304DB: EB 0B              jmp         0000000140D304E8
  0000000140D304DD: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D304E1: 83 C0 01           add         eax,1
  0000000140D304E4: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140D304E8: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D304ED: E8 AE BC F4 FF     call        0000000140C7C1A0
  0000000140D304F2: 39 44 24 40        cmp         dword ptr [rsp+40h],eax
  0000000140D304F6: 0F 8D 6A 01 00 00  jge         0000000140D30666
  0000000140D304FC: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D30500: 83 C0 01           add         eax,1
  0000000140D30503: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  0000000140D30507: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  0000000140D3050C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D30511: E8 3A 28 F5 FF     call        0000000140C82D50
  0000000140D30516: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140D3051B: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D30520: E8 EB 14 4C 00     call        00000001411F1A10
  0000000140D30525: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D3052A: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140D30530: 75 02              jne         0000000140D30534
  0000000140D30532: EB A9              jmp         0000000140D304DD
  0000000140D30534: C7 44 24 6C 01 00  mov         dword ptr [rsp+6Ch],1
                    00 00
  0000000140D3053C: 48 8D 54 24 6C     lea         rdx,[rsp+6Ch]
  0000000140D30541: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D30546: E8 F5 E4 4B 00     call        00000001411EEA40
  0000000140D3054B: 0F B6 C0           movzx       eax,al
  0000000140D3054E: 85 C0              test        eax,eax
  0000000140D30550: 75 02              jne         0000000140D30554
  0000000140D30552: EB 89              jmp         0000000140D304DD
  0000000140D30554: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D30559: E8 92 14 4C 00     call        00000001411F19F0
  0000000140D3055E: 48
  0000000140D3055F: 89
  0000000140D30560: 44

  Summary

     1E02000 .text
