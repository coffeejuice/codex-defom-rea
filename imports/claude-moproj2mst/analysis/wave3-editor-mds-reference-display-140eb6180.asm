Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EB6180: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140EB6185: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140EB618A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EB618F: 48 83 EC 38        sub         rsp,38h
  0000000140EB6193: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  0000000140EB6198: 48 8D 54 24 24     lea         rdx,[rsp+24h]
  0000000140EB619D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140EB61A2: E8 69 00 00 00     call        0000000140EB6210
  0000000140EB61A7: 83 7C 24 24 00     cmp         dword ptr [rsp+24h],0
  0000000140EB61AC: 74 07              je          0000000140EB61B5
  0000000140EB61AE: 83 7C 24 20 00     cmp         dword ptr [rsp+20h],0
  0000000140EB61B3: 75 44              jne         0000000140EB61F9
  0000000140EB61B5: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140EB61BD: EB 0B              jmp         0000000140EB61CA
  0000000140EB61BF: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140EB61C3: 83 C0 01           add         eax,1
  0000000140EB61C6: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140EB61CA: 83 7C 24 28 03     cmp         dword ptr [rsp+28h],3
  0000000140EB61CF: 7D 28              jge         0000000140EB61F9
  0000000140EB61D1: 48 63 4C 24 28     movsxd      rcx,dword ptr [rsp+28h]
  0000000140EB61D6: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140EB61DB: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140EB61DF: F2 0F 11 04 C8     movsd       mmword ptr [rax+rcx*8],xmm0
  0000000140EB61E4: 48 63 4C 24 28     movsxd      rcx,dword ptr [rsp+28h]
  0000000140EB61E9: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140EB61EE: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140EB61F2: F2 0F 11 04 C8     movsd       mmword ptr [rax+rcx*8],xmm0
  0000000140EB61F7: EB C6              jmp         0000000140EB61BF
  0000000140EB61F9: 48 83 C4 38        add         rsp,38h
  0000000140EB61FD: C3                 ret

  Summary

     1E02000 .text
