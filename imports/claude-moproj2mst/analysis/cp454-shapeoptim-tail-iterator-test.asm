Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DFC080: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DFC085: 48 83 EC 38        sub         rsp,38h
  0000000140DFC089: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140DFC08E: 8B 50 08           mov         edx,dword ptr [rax+8]
  0000000140DFC091: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140DFC096: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000140DFC099: 83 C1 01           add         ecx,1
  0000000140DFC09C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140DFC0A1: 89 48 08           mov         dword ptr [rax+8],ecx
  0000000140DFC0A4: 85 D2              test        edx,edx
  0000000140DFC0A6: 75 28              jne         0000000140DFC0D0
  0000000140DFC0A8: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140DFC0AD: 48 83 C2 18        add         rdx,18h
  0000000140DFC0B1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DFC0B6: 48 83 C1 10        add         rcx,10h
  0000000140DFC0BA: E8 B1 A3 32 FF     call        0000000140126470
  0000000140DFC0BF: 0F B6 C0           movzx       eax,al
  0000000140DFC0C2: 85 C0              test        eax,eax
  0000000140DFC0C4: 74 0A              je          0000000140DFC0D0
  0000000140DFC0C6: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140DFC0CE: EB 08              jmp         0000000140DFC0D8
  0000000140DFC0D0: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140DFC0D8: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  0000000140DFC0DC: 48 83 C4 38        add         rsp,38h
  0000000140DFC0E0: C3                 ret

  Summary

     1E02000 .text
