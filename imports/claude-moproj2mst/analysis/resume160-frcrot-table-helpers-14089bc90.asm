Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014089BC90: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089BC95: 48 83 EC 18        sub         rsp,18h
  000000014089BC99: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014089BC9E: 8B 40 40           mov         eax,dword ptr [rax+40h]
  000000014089BCA1: 89 04 24           mov         dword ptr [rsp],eax
  000000014089BCA4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014089BCA9: 83 78 04 00        cmp         dword ptr [rax+4],0
  000000014089BCAD: 75 5C              jne         000000014089BD0B
  000000014089BCAF: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014089BCB4: 83 78 40 01        cmp         dword ptr [rax+40h],1
  000000014089BCB8: 7F 47              jg          000000014089BD01
  000000014089BCBA: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014089BCBF: 81 38 B5 00 00 00  cmp         dword ptr [rax],0B5h
  000000014089BCC5: 74 3A              je          000000014089BD01
  000000014089BCC7: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014089BCCC: 81 38 A7 00 00 00  cmp         dword ptr [rax],0A7h
  000000014089BCD2: 74 2D              je          000000014089BD01
  000000014089BCD4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014089BCD9: 81 38 A8 00 00 00  cmp         dword ptr [rax],0A8h
  000000014089BCDF: 74 20              je          000000014089BD01
  000000014089BCE1: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014089BCE6: 81 38 AA 00 00 00  cmp         dword ptr [rax],0AAh
  000000014089BCEC: 74 13              je          000000014089BD01
  000000014089BCEE: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014089BCF3: 81 38 AB 00 00 00  cmp         dword ptr [rax],0ABh
  000000014089BCF9: 74 06              je          000000014089BD01
  000000014089BCFB: 33 C0              xor         eax,eax
  000000014089BCFD: EB 56              jmp         000000014089BD55
  000000014089BCFF: EB 0A              jmp         000000014089BD0B
  000000014089BD01: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014089BD06: 8B 40 40           mov         eax,dword ptr [rax+40h]
  000000014089BD09: EB 4A              jmp         000000014089BD55
  000000014089BD0B: C7 44 24 04 00 00  mov         dword ptr [rsp+4],0
                    00 00
  000000014089BD13: EB 0B              jmp         000000014089BD20
  000000014089BD15: 8B 44 24 04        mov         eax,dword ptr [rsp+4]
  000000014089BD19: 83 C0 01           add         eax,1
  000000014089BD1C: 89 44 24 04        mov         dword ptr [rsp+4],eax
  000000014089BD20: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014089BD25: 8B 40 04           mov         eax,dword ptr [rax+4]
  000000014089BD28: 39 44 24 04        cmp         dword ptr [rsp+4],eax
  000000014089BD2C: 7D 17              jge         000000014089BD45
  000000014089BD2E: 48 63 54 24 04     movsxd      rdx,dword ptr [rsp+4]
  000000014089BD33: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014089BD38: 8B 04 24           mov         eax,dword ptr [rsp]
  000000014089BD3B: 0F AF 44 91 1C     imul        eax,dword ptr [rcx+rdx*4+1Ch]
  000000014089BD40: 89 04 24           mov         dword ptr [rsp],eax
  000000014089BD43: EB D0              jmp         000000014089BD15
  000000014089BD45: 83 3C 24 00        cmp         dword ptr [rsp],0
  000000014089BD49: 7D 07              jge         000000014089BD52
  000000014089BD4B: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  000000014089BD52: 8B 04 24           mov         eax,dword ptr [rsp]
  000000014089BD55: 48 83 C4 18        add         rsp,18h
  000000014089BD59: C3                 ret

  Summary

     1E02000 .text
