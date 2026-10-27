Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014013AF20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014013AF25: 48 83 EC 18        sub         rsp,18h
  000000014013AF29: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014013AF2E: 48 83 C0 28        add         rax,28h
  000000014013AF32: 48 85 C0           test        rax,rax
  000000014013AF35: 75 0A              jne         000000014013AF41
  000000014013AF37: 48 C7 04 24 00 00  mov         qword ptr [rsp],0
                    00 00
  000000014013AF3F: EB 1B              jmp         000000014013AF5C
  000000014013AF41: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014013AF46: 48 8B 40 30        mov         rax,qword ptr [rax+30h]
  000000014013AF4A: 48 63 48 04        movsxd      rcx,dword ptr [rax+4]
  000000014013AF4E: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014013AF53: 48 8D 44 08 30     lea         rax,[rax+rcx+30h]
  000000014013AF58: 48 89 04 24        mov         qword ptr [rsp],rax
  000000014013AF5C: 48 8B 04 24        mov         rax,qword ptr [rsp]
  000000014013AF60: 48 83 C4 18        add         rsp,18h
  000000014013AF64: C3                 ret

  Summary

     1E02000 .text
