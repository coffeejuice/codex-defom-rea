Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065505FB0: 85 D2              test        edx,edx
  0000000065505FB2: 0F 88 AA 00 00 00  js          0000000065506062
  0000000065505FB8: 48 83 EC 48        sub         rsp,48h
  0000000065505FBC: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065505FC5: 4C 8B D1           mov         r10,rcx
  0000000065505FC8: 48 8B 41 28        mov         rax,qword ptr [rcx+28h]
  0000000065505FCC: 44 8B 40 08        mov         r8d,dword ptr [rax+8]
  0000000065505FD0: 44 89 44 24 50     mov         dword ptr [rsp+50h],r8d
  0000000065505FD5: 44 3B C2           cmp         r8d,edx
  0000000065505FD8: 0F 84 80 00 00 00  je          000000006550605E
  0000000065505FDE: 44 3B C2           cmp         r8d,edx
  0000000065505FE1: 7D 4D              jge         0000000065506030
  0000000065505FE3: 33 C0              xor         eax,eax
  0000000065505FE5: 89 44 24 58        mov         dword ptr [rsp+58h],eax
  0000000065505FE9: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  0000000065505FF1: C7 44 24 2C FF FF  mov         dword ptr [rsp+2Ch],0FFFFFFFFh
                    FF FF
  0000000065505FF9: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000065505FFE: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000065506003: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000065506008: 48 8D 44 24 50     lea         rax,[rsp+50h]
  000000006550600D: 45 85 C0           test        r8d,r8d
  0000000065506010: 48 0F 49 C8        cmovns      rcx,rax
  0000000065506014: 49 8B 02           mov         rax,qword ptr [r10]
  0000000065506017: 41 2B D0           sub         edx,r8d
  000000006550601A: 4C 8D 4C 24 28     lea         r9,[rsp+28h]
  000000006550601F: 44 8B C2           mov         r8d,edx
  0000000065506022: 8B 11              mov         edx,dword ptr [rcx]
  0000000065506024: 49 8B CA           mov         rcx,r10
  0000000065506027: FF 90 D8 00 00 00  call        qword ptr [rax+00000000000000D8h]
  000000006550602D: 90                 nop
  000000006550602E: EB 2E              jmp         000000006550605E
  0000000065506030: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  0000000065506038: C7 44 24 2C FF FF  mov         dword ptr [rsp+2Ch],0FFFFFFFFh
                    FF FF
  0000000065506040: 33 C0              xor         eax,eax
  0000000065506042: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000065506047: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000006550604C: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006550604F: 44 2B C2           sub         r8d,edx
  0000000065506052: 4C 8D 4C 24 28     lea         r9,[rsp+28h]
  0000000065506057: FF 90 E8 00 00 00  call        qword ptr [rax+00000000000000E8h]
  000000006550605D: 90                 nop
  000000006550605E: 48 83 C4 48        add         rsp,48h
  0000000065506062: F3 C3              rep ret

  Summary

      695000 .text
