Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412EDAC0: 48 83 EC 38        sub         rsp,38h
  00000001412EDAC4: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412EDACD: B9 30 00 00 00     mov         ecx,30h
  00000001412EDAD2: E8 E5 0F 18 00     call        000000014146EABC
  00000001412EDAD7: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001412EDADC: 48 85 C0           test        rax,rax
  00000001412EDADF: 74 07              je          00000001412EDAE8
  00000001412EDAE1: 33 C9              xor         ecx,ecx
  00000001412EDAE3: 48 89 08           mov         qword ptr [rax],rcx
  00000001412EDAE6: EB 02              jmp         00000001412EDAEA
  00000001412EDAE8: 33 C9              xor         ecx,ecx
  00000001412EDAEA: 48 8D 50 08        lea         rdx,[rax+8]
  00000001412EDAEE: 48 85 D2           test        rdx,rdx
  00000001412EDAF1: 74 03              je          00000001412EDAF6
  00000001412EDAF3: 48 89 0A           mov         qword ptr [rdx],rcx
  00000001412EDAF6: 48 8D 50 10        lea         rdx,[rax+10h]
  00000001412EDAFA: 48 85 D2           test        rdx,rdx
  00000001412EDAFD: 74 03              je          00000001412EDB02
  00000001412EDAFF: 48 89 0A           mov         qword ptr [rdx],rcx
  00000001412EDB02: C6 40 28 01        mov         byte ptr [rax+28h],1
  00000001412EDB06: C6 40 29 00        mov         byte ptr [rax+29h],0
  00000001412EDB0A: 48 83 C4 38        add         rsp,38h
  00000001412EDB0E: C3                 ret

  Summary

     1E02000 .text
