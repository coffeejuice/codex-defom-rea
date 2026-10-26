Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146EABC: 40 53              push        rbx
  000000014146EABE: 48 83 EC 40        sub         rsp,40h
  000000014146EAC2: 48 8B D9           mov         rbx,rcx
  000000014146EAC5: EB 0F              jmp         000000014146EAD6
  000000014146EAC7: 48 8B CB           mov         rcx,rbx
  000000014146EACA: E8 19 32 01 00     call        0000000141481CE8
  000000014146EACF: 85 C0              test        eax,eax
  000000014146EAD1: 74 13              je          000000014146EAE6
  000000014146EAD3: 48 8B CB           mov         rcx,rbx
  000000014146EAD6: E8 C1 5D 00 00     call        000000014147489C
  000000014146EADB: 48 85 C0           test        rax,rax
  000000014146EADE: 74 E7              je          000000014146EAC7
  000000014146EAE0: 48 83 C4 40        add         rsp,40h
  000000014146EAE4: 5B                 pop         rbx
  000000014146EAE5: C3                 ret
  000000014146EAE6: 8B 05 4C 10 29 03  mov         eax,dword ptr [00000001446FFB38h]
  000000014146EAEC: 41 B8 01 00 00 00  mov         r8d,1
  000000014146EAF2: 48 8D 1D 57 3F CD  lea         rbx,[0000000142142A50h]
                    00
  000000014146EAF9: 41 84 C0           test        r8b,al
  000000014146EAFC: 75 2F              jne         000000014146EB2D
  000000014146EAFE: 41 0B C0           or          eax,r8d
  000000014146EB01: 48 8D 15 70 F5 6E  lea         rdx,[0000000143B5E078h]
                    02
  000000014146EB08: 48 8D 0D 11 10 29  lea         rcx,[00000001446FFB20h]
                    03
  000000014146EB0F: 89 05 23 10 29 03  mov         dword ptr [00000001446FFB38h],eax
  000000014146EB15: E8 1A 1C 00 00     call        0000000141470734
  000000014146EB1A: 48 8D 0D 9F 19 99  lea         rcx,[0000000141E004C0h]
                    00
  000000014146EB21: 48 89 1D F8 0F 29  mov         qword ptr [00000001446FFB20h],rbx
                    03
  000000014146EB28: E8 0B 02 00 00     call        000000014146ED38
  000000014146EB2D: 48 8D 15 EC 0F 29  lea         rdx,[00000001446FFB20h]
                    03
  000000014146EB34: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014146EB39: E8 12 1C 00 00     call        0000000141470750
  000000014146EB3E: 48 8D 15 5B CA 09  lea         rdx,[000000014350B5A0h]
                    02
  000000014146EB45: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014146EB4A: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000014146EB4F: E8 04 00 00 00     call        000000014146EB58
  000000014146EB54: CC                 int         3

  Summary

     1E02000 .text
