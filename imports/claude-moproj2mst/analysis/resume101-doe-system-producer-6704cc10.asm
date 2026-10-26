Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704CC10: 48 83 EC 28        sub         rsp,28h
  000000006704CC14: 48 8B 05 45 71 2A  mov         rax,qword ptr [00000000672F3D60h]
                    00
  000000006704CC1B: 48 85 C0           test        rax,rax
  000000006704CC1E: 75 7B              jne         000000006704CC9B
  000000006704CC20: 38 05 42 71 2A 00  cmp         byte ptr [00000000672F3D68h],al
  000000006704CC26: 75 73              jne         000000006704CC9B
  000000006704CC28: 8D 48 08           lea         ecx,[rax+8]
  000000006704CC2B: E8 56 BE 12 00     call        0000000067178A86
  000000006704CC30: 48 8B C8           mov         rcx,rax
  000000006704CC33: 48 85 C0           test        rax,rax
  000000006704CC36: 74 0C              je          000000006704CC44
  000000006704CC38: 48 8D 05 21 C1 15  lea         rax,[00000000671A8D60h]
                    00
  000000006704CC3F: 48 89 01           mov         qword ptr [rcx],rax
  000000006704CC42: EB 02              jmp         000000006704CC46
  000000006704CC44: 33 C9              xor         ecx,ecx
  000000006704CC46: 33 C0              xor         eax,eax
  000000006704CC48: F0 48 0F B1 0D 0F  lock cmpxchg qword ptr [00000000672F3D60h],rcx
                    71 2A 00
  000000006704CC51: 74 1B              je          000000006704CC6E
  000000006704CC53: 48 85 C9           test        rcx,rcx
  000000006704CC56: 74 43              je          000000006704CC9B
  000000006704CC58: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006704CC5B: BA 01 00 00 00     mov         edx,1
  000000006704CC60: FF 10              call        qword ptr [rax]
  000000006704CC62: 48 8B 05 F7 70 2A  mov         rax,qword ptr [00000000672F3D60h]
                    00
  000000006704CC69: 48 83 C4 28        add         rsp,28h
  000000006704CC6D: C3                 ret
  000000006704CC6E: 8B 05 E4 70 2A 00  mov         eax,dword ptr [00000000672F3D58h]
  000000006704CC74: A8 01              test        al,1
  000000006704CC76: 75 23              jne         000000006704CC9B
  000000006704CC78: 83 C8 01           or          eax,1
  000000006704CC7B: 48 8D 0D 1E 96 15  lea         rcx,[00000000671A62A0h]
                    00
  000000006704CC82: 89 05 D0 70 2A 00  mov         dword ptr [00000000672F3D58h],eax
  000000006704CC88: 48 8D 05 D1 70 2A  lea         rax,[00000000672F3D60h]
                    00
  000000006704CC8F: 48 89 05 BA 70 2A  mov         qword ptr [00000000672F3D50h],rax
                    00
  000000006704CC96: E8 B9 BE 12 00     call        0000000067178B54
  000000006704CC9B: 48 8B 05 BE 70 2A  mov         rax,qword ptr [00000000672F3D60h]
                    00
  000000006704CCA2: 48 83 C4 28        add         rsp,28h
  000000006704CCA6: C3                 ret

  Summary

      1A7000 .text
