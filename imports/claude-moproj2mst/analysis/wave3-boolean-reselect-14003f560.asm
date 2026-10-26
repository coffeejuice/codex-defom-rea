Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014003F560: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014003F565: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014003F569: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014003F56E: 48 83 EC 38        sub         rsp,38h
  000000014003F572: 83 7C 24 50 00     cmp         dword ptr [rsp+50h],0
  000000014003F577: 7E 07              jle         000000014003F580
  000000014003F579: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  000000014003F57E: EB 05              jmp         000000014003F585
  000000014003F580: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014003F585: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014003F58A: E8 31 57 FE FF     call        0000000140024CC0
  000000014003F58F: 0F B6 54 24 20     movzx       edx,byte ptr [rsp+20h]
  000000014003F594: 48 8B 88 D0 02 00  mov         rcx,qword ptr [rax+00000000000002D0h]
                    00
  000000014003F59B: FF 15 77 C9 DC 01  call        qword ptr [0000000141E0BF18h]
  000000014003F5A1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014003F5A6: E8 15 57 FE FF     call        0000000140024CC0
  000000014003F5AB: 0F B6 54 24 20     movzx       edx,byte ptr [rsp+20h]
  000000014003F5B0: 48 8B 88 D8 02 00  mov         rcx,qword ptr [rax+00000000000002D8h]
                    00
  000000014003F5B7: FF 15 5B C9 DC 01  call        qword ptr [0000000141E0BF18h]
  000000014003F5BD: 48 83 C4 38        add         rsp,38h
  000000014003F5C1: C3                 ret

  Summary

     1E02000 .text
