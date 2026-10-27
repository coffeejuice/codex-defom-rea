Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091EFF0: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000014091EFF5: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014091EFF9: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091EFFE: 48 83 EC 48        sub         rsp,48h
  000000014091F002: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091F00B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091F010: 48 83 C1 18        add         rcx,18h
  000000014091F014: E8 C7 5A 6E FF     call        0000000140004AE0
  000000014091F019: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  000000014091F01D: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014091F022: E8 89 34 72 FF     call        00000001400424B0
  000000014091F027: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091F02F: EB 0B              jmp         000000014091F03C
  000000014091F031: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091F035: 83 C0 01           add         eax,1
  000000014091F038: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091F03C: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  000000014091F040: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014091F044: 7D 43              jge         000000014091F089
  000000014091F046: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091F04B: 48 83 C1 18        add         rcx,18h
  000000014091F04F: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091F053: E8 C8 0A 00 00     call        000000014091FB20
  000000014091F058: 4C 8B D8           mov         r11,rax
  000000014091F05B: 8B 44 24 58        mov         eax,dword ptr [rsp+58h]
  000000014091F05F: 41 39 03           cmp         dword ptr [r11],eax
  000000014091F062: 75 23              jne         000000014091F087
  000000014091F064: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091F069: 48 83 C1 18        add         rcx,18h
  000000014091F06D: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091F071: E8 AA 0A 00 00     call        000000014091FB20
  000000014091F076: 48 8B D0           mov         rdx,rax
  000000014091F079: 48 83 C2 04        add         rdx,4
  000000014091F07D: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014091F082: E8 79 60 70 FF     call        0000000140025100
  000000014091F087: EB A8              jmp         000000014091F031
  000000014091F089: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014091F08E: E8 4D 5A 6E FF     call        0000000140004AE0
  000000014091F093: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000014091F097: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014091F09C: E8 FF 90 8E FF     call        00000001402081A0
  000000014091F0A1: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  000000014091F0A5: 48 83 C4 48        add         rsp,48h
  000000014091F0A9: C3                 ret

  Summary

     1E02000 .text
