Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704CCB0: 48 83 EC 28        sub         rsp,28h
  000000006704CCB4: 48 8B 05 C5 70 2A  mov         rax,qword ptr [00000000672F3D80h]
                    00
  000000006704CCBB: 48 85 C0           test        rax,rax
  000000006704CCBE: 75 60              jne         000000006704CD20
  000000006704CCC0: 38 05 C2 70 2A 00  cmp         byte ptr [00000000672F3D88h],al
  000000006704CCC6: 75 58              jne         000000006704CD20
  000000006704CCC8: B9 90 00 00 00     mov         ecx,90h
  000000006704CCCD: E8 B4 BD 12 00     call        0000000067178A86
  000000006704CCD2: 48 8B C8           mov         rcx,rax
  000000006704CCD5: 33 C0              xor         eax,eax
  000000006704CCD7: F0 48 0F B1 0D A0  lock cmpxchg qword ptr [00000000672F3D80h],rcx
                    70 2A 00
  000000006704CCE0: 74 11              je          000000006704CCF3
  000000006704CCE2: E8 6F BC 12 00     call        0000000067178956
  000000006704CCE7: 48 8B 05 92 70 2A  mov         rax,qword ptr [00000000672F3D80h]
                    00
  000000006704CCEE: 48 83 C4 28        add         rsp,28h
  000000006704CCF2: C3                 ret
  000000006704CCF3: 8B 05 7F 70 2A 00  mov         eax,dword ptr [00000000672F3D78h]
  000000006704CCF9: A8 01              test        al,1
  000000006704CCFB: 75 23              jne         000000006704CD20
  000000006704CCFD: 83 C8 01           or          eax,1
  000000006704CD00: 48 8D 0D D9 95 15  lea         rcx,[00000000671A62E0h]
                    00
  000000006704CD07: 89 05 6B 70 2A 00  mov         dword ptr [00000000672F3D78h],eax
  000000006704CD0D: 48 8D 05 6C 70 2A  lea         rax,[00000000672F3D80h]
                    00
  000000006704CD14: 48 89 05 55 70 2A  mov         qword ptr [00000000672F3D70h],rax
                    00
  000000006704CD1B: E8 34 BE 12 00     call        0000000067178B54
  000000006704CD20: 48 8B 05 59 70 2A  mov         rax,qword ptr [00000000672F3D80h]
                    00
  000000006704CD27: 48 83 C4 28        add         rsp,28h
  000000006704CD2B: C3                 ret

  Summary

      1A7000 .text
