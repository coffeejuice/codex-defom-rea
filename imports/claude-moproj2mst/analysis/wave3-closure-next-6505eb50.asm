Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505EB50: 40 53              push        rbx
  000000006505EB52: 48 83 EC 20        sub         rsp,20h
  000000006505EB56: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006505EB5A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006505EB60: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006505EB64: 48 8B 88 D8 00 00  mov         rcx,qword ptr [rax+00000000000000D8h]
                    00
  000000006505EB6B: 48 85 C9           test        rcx,rcx
  000000006505EB6E: 74 30              je          000000006505EBA0
  000000006505EB70: E8 BB 29 49 00     call        00000000654F1530
  000000006505EB75: 48 8B 50 08        mov         rdx,qword ptr [rax+8]
  000000006505EB79: 80 BA 08 01 00 00  cmp         byte ptr [rdx+0000000000000108h],0
                    00
  000000006505EB80: 75 1E              jne         000000006505EBA0
  000000006505EB82: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006505EB86: 48 8B 88 D8 00 00  mov         rcx,qword ptr [rax+00000000000000D8h]
                    00
  000000006505EB8D: E8 9E 29 49 00     call        00000000654F1530
  000000006505EB92: BA 02 00 00 00     mov         edx,2
  000000006505EB97: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006505EB9B: E8 B0 B4 56 00     call        00000000655CA050
  000000006505EBA0: 48 8B CB           mov         rcx,rbx
  000000006505EBA3: FF 15 BF 7C 63 00  call        qword ptr [0000000065696868h]
  000000006505EBA9: 48 8B D8           mov         rbx,rax
  000000006505EBAC: 48 85 C0           test        rax,rax
  000000006505EBAF: 75 AF              jne         000000006505EB60
  000000006505EBB1: 48 83 C4 20        add         rsp,20h
  000000006505EBB5: 5B                 pop         rbx
  000000006505EBB6: C3                 ret

  Summary

      695000 .text
