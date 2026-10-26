Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014053CD80: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  000000014053CD85: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014053CD8A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014053CD8E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014053CD93: 48 83 EC 38        sub         rsp,38h
  000000014053CD97: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  000000014053CD9C: 0F 85 AC 00 00 00  jne         000000014053CE4E
  000000014053CDA2: E8 D9 7D AC FF     call        0000000140004B80
  000000014053CDA7: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  000000014053CDAC: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  000000014053CDB1: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  000000014053CDB5: 89 44 24 2C        mov         dword ptr [rsp+2Ch],eax
  000000014053CDB9: 83 7C 24 2C 00     cmp         dword ptr [rsp+2Ch],0
  000000014053CDBE: 74 1E              je          000000014053CDDE
  000000014053CDC0: 83 7C 24 2C 01     cmp         dword ptr [rsp+2Ch],1
  000000014053CDC5: 74 3B              je          000000014053CE02
  000000014053CDC7: 83 7C 24 2C 02     cmp         dword ptr [rsp+2Ch],2
  000000014053CDCC: 74 40              je          000000014053CE0E
  000000014053CDCE: 83 7C 24 2C 03     cmp         dword ptr [rsp+2Ch],3
  000000014053CDD3: 74 45              je          000000014053CE1A
  000000014053CDD5: 83 7C 24 2C 04     cmp         dword ptr [rsp+2Ch],4
  000000014053CDDA: 74 4A              je          000000014053CE26
  000000014053CDDC: EB 70              jmp         000000014053CE4E
  000000014053CDDE: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014053CDE3: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  000000014053CDE7: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014053CDEC: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000014053CDF0: 44 8B 01           mov         r8d,dword ptr [rcx]
  000000014053CDF3: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014053CDF6: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014053CDFB: E8 B0 70 ED FF     call        0000000140413EB0
  000000014053CE00: EB 4C              jmp         000000014053CE4E
  000000014053CE02: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014053CE07: E8 F4 70 ED FF     call        0000000140413F00
  000000014053CE0C: EB 40              jmp         000000014053CE4E
  000000014053CE0E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014053CE13: E8 48 71 ED FF     call        0000000140413F60
  000000014053CE18: EB 34              jmp         000000014053CE4E
  000000014053CE1A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014053CE1F: E8 7C 71 ED FF     call        0000000140413FA0
  000000014053CE24: EB 28              jmp         000000014053CE4E
  000000014053CE26: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014053CE2B: E8 90 71 ED FF     call        0000000140413FC0
  000000014053CE30: 88 44 24 28        mov         byte ptr [rsp+28h],al
  000000014053CE34: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014053CE39: 48 83 38 00        cmp         qword ptr [rax],0
  000000014053CE3D: 74 0F              je          000000014053CE4E
  000000014053CE3F: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014053CE44: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000014053CE47: 0F B6 44 24 28     movzx       eax,byte ptr [rsp+28h]
  000000014053CE4C: 88 01              mov         byte ptr [rcx],al
  000000014053CE4E: 48 83 C4 38        add         rsp,38h
  000000014053CE52: C3                 ret

  Summary

     1E02000 .text
