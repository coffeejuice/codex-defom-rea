Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014005D300: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014005D304: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014005D309: 48 83 EC 68        sub         rsp,68h
  000000014005D30D: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014005D316: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014005D31B: E8 80 24 00 00     call        000000014005F7A0
  000000014005D320: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014005D325: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014005D32B: 75 07              jne         000000014005D334
  000000014005D32D: 32 C0              xor         al,al
  000000014005D32F: E9 9C 00 00 00     jmp         000000014005D3D0
  000000014005D334: 44 8B 44 24 78     mov         r8d,dword ptr [rsp+78h]
  000000014005D339: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000014005D33E: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014005D343: E8 28 FC FF FF     call        000000014005CF70
  000000014005D348: 90                 nop
  000000014005D349: 8B 44 24 78        mov         eax,dword ptr [rsp+78h]
  000000014005D34D: 83 C0 01           add         eax,1
  000000014005D350: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  000000014005D354: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000014005D359: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014005D35E: FF 15 EC 6B DA 01  call        qword ptr [0000000141E03F50h]
  000000014005D364: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014005D369: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014005D36E: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014005D373: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014005D378: FF 15 DA 6B DA 01  call        qword ptr [0000000141E03F58h]
  000000014005D37E: 44 8B 44 24 34     mov         r8d,dword ptr [rsp+34h]
  000000014005D383: 48 8B D0           mov         rdx,rax
  000000014005D386: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014005D38B: E8 D0 8F 83 00     call        0000000140896360
  000000014005D390: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  000000014005D394: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000014005D399: FF 15 F1 6E DA 01  call        qword ptr [0000000141E04290h]
  000000014005D39F: 83 7C 24 30 00     cmp         dword ptr [rsp+30h],0
  000000014005D3A4: 74 16              je          000000014005D3BC
  000000014005D3A6: C6 44 24 40 00     mov         byte ptr [rsp+40h],0
  000000014005D3AB: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014005D3B0: FF 15 2A 86 DA 01  call        qword ptr [0000000141E059E0h]
  000000014005D3B6: 8A 44 24 40        mov         al,byte ptr [rsp+40h]
  000000014005D3BA: EB 14              jmp         000000014005D3D0
  000000014005D3BC: C6 44 24 41 01     mov         byte ptr [rsp+41h],1
  000000014005D3C1: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014005D3C6: FF 15 14 86 DA 01  call        qword ptr [0000000141E059E0h]
  000000014005D3CC: 8A 44 24 41        mov         al,byte ptr [rsp+41h]
  000000014005D3D0: 48 83 C4 68        add         rsp,68h
  000000014005D3D4: C3                 ret

  Summary

     1E02000 .text
