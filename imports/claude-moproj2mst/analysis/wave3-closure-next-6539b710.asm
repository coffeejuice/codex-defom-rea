Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006539B710: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006539B715: 57                 push        rdi
  000000006539B716: 48 83 EC 30        sub         rsp,30h
  000000006539B71A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006539B723: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006539B728: 48 8B FA           mov         rdi,rdx
  000000006539B72B: 48 8B D9           mov         rbx,rcx
  000000006539B72E: B9 48 02 00 00     mov         ecx,248h
  000000006539B733: E8 3A 5A 24 00     call        00000000655E1172
  000000006539B738: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000006539B73D: 48 85 C0           test        rax,rax
  000000006539B740: 74 0F              je          000000006539B751
  000000006539B742: BA 01 00 00 00     mov         edx,1
  000000006539B747: 48 8B C8           mov         rcx,rax
  000000006539B74A: E8 91 FC FF FF     call        000000006539B3E0
  000000006539B74F: EB 02              jmp         000000006539B753
  000000006539B751: 33 C0              xor         eax,eax
  000000006539B753: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  000000006539B75B: 4C 8D 4C 24 50     lea         r9,[rsp+50h]
  000000006539B760: 4C 8B C7           mov         r8,rdi
  000000006539B763: 48 8B D0           mov         rdx,rax
  000000006539B766: 48 8B CB           mov         rcx,rbx
  000000006539B769: E8 92 57 CD FF     call        0000000065070F00
  000000006539B76E: 90                 nop
  000000006539B76F: 48 8D 05 52 14 36  lea         rax,[00000000656FCBC8h]
                    00
  000000006539B776: 48 89 03           mov         qword ptr [rbx],rax
  000000006539B779: 48 8D 05 10 14 36  lea         rax,[00000000656FCB90h]
                    00
  000000006539B780: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  000000006539B784: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006539B788: E8 73 FD FF FF     call        000000006539B500
  000000006539B78D: 90                 nop
  000000006539B78E: 48 8B C3           mov         rax,rbx
  000000006539B791: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006539B796: 48 83 C4 30        add         rsp,30h
  000000006539B79A: 5F                 pop         rdi
  000000006539B79B: C3                 ret

  Summary

      695000 .text
