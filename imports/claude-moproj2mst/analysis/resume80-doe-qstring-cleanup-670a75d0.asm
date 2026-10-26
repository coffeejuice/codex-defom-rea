Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670A75D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670A75D5: 57                 push        rdi
  00000000670A75D6: 48 83 EC 30        sub         rsp,30h
  00000000670A75DA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670A75E3: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670A75E8: 48 8B D9           mov         rbx,rcx
  00000000670A75EB: 48 8B 51 08        mov         rdx,qword ptr [rcx+8]
  00000000670A75EF: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  00000000670A75F3: 75 0A              jne         00000000670A75FF
  00000000670A75F5: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000670A75F9: E8 D2 8A FC FF     call        00000000670700D0
  00000000670A75FE: 90                 nop
  00000000670A75FF: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670A7602: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670A7606: 75 08              jne         00000000670A7610
  00000000670A7608: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670A760B: E8 C0 8A FC FF     call        00000000670700D0
  00000000670A7610: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670A7615: 48 83 C4 30        add         rsp,30h
  00000000670A7619: 5F                 pop         rdi
  00000000670A761A: C3                 ret

  Summary

      1A7000 .text
