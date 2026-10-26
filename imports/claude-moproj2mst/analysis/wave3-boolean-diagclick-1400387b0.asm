Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400387B0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400387B5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400387BA: 48 83 EC 38        sub         rsp,38h
  00000001400387BE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400387C3: E8 D8 AB FD FF     call        00000001400133A0
  00000001400387C8: 48 8B C8           mov         rcx,rax
  00000001400387CB: E8 B0 CF 00 00     call        0000000140045780
  00000001400387D0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400387D5: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001400387DB: 74 11              je          00000001400387EE
  00000001400387DD: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400387E2: E8 89 6E 00 00     call        000000014003F670
  00000001400387E7: 0F B6 C0           movzx       eax,al
  00000001400387EA: 85 C0              test        eax,eax
  00000001400387EC: 74 02              je          00000001400387F0
  00000001400387EE: EB 4E              jmp         000000014003883E
  00000001400387F0: C6 44 24 2C 00     mov         byte ptr [rsp+2Ch],0
  00000001400387F5: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  00000001400387FD: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140038803: 74 14              je          0000000140038819
  0000000140038805: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014003880A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014003880D: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140038812: FF 50 28           call        qword ptr [rax+28h]
  0000000140038815: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140038819: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  000000014003881D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140038822: E8 29 01 00 00     call        0000000140038950
  0000000140038827: 88 44 24 2C        mov         byte ptr [rsp+2Ch],al
  000000014003882B: 0F B6 44 24 2C     movzx       eax,byte ptr [rsp+2Ch]
  0000000140038830: 85 C0              test        eax,eax
  0000000140038832: 75 0A              jne         000000014003883E
  0000000140038834: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140038839: E8 A2 22 FF FF     call        000000014002AAE0
  000000014003883E: 48 83 C4 38        add         rsp,38h
  0000000140038842: C3                 ret

  Summary

     1E02000 .text
