Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400A20D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400A20D5: 48 83 EC 38        sub         rsp,38h
  00000001400A20D9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400A20DE: E8 DD F2 08 00     call        00000001401313C0
  00000001400A20E3: 88 44 24 28        mov         byte ptr [rsp+28h],al
  00000001400A20E7: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  00000001400A20F0: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400A20F5: E8 96 52 00 00     call        00000001400A7390
  00000001400A20FA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400A20FF: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001400A2105: 74 36              je          00000001400A213D
  00000001400A2107: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400A210C: E8 1F E2 F6 FF     call        0000000140010330
  00000001400A2111: 0F B6 C0           movzx       eax,al
  00000001400A2114: 85 C0              test        eax,eax
  00000001400A2116: 75 11              jne         00000001400A2129
  00000001400A2118: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400A211D: E8 3E E2 F6 FF     call        0000000140010360
  00000001400A2122: 0F B6 C0           movzx       eax,al
  00000001400A2125: 85 C0              test        eax,eax
  00000001400A2127: 74 14              je          00000001400A213D
  00000001400A2129: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400A212E: E8 1D 85 01 00     call        00000001400BA650
  00000001400A2133: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400A2138: E8 C3 86 01 00     call        00000001400BA800
  00000001400A213D: 8A 44 24 28        mov         al,byte ptr [rsp+28h]
  00000001400A2141: 48 83 C4 38        add         rsp,38h
  00000001400A2145: C3                 ret

  Summary

     1E02000 .text
