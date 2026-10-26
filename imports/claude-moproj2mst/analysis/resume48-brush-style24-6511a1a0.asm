Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006511A1A0: 40 57              push        rdi
  000000006511A1A2: 48 83 EC 40        sub         rsp,40h
  000000006511A1A6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006511A1AF: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  000000006511A1B4: 48 8B F9           mov         rdi,rcx
  000000006511A1B7: 48 83 C1 78        add         rcx,78h
  000000006511A1BB: E8 D0 ED F8 FF     call        00000000650A8F90
  000000006511A1C0: 84 C0              test        al,al
  000000006511A1C2: 74 37              je          000000006511A1FB
  000000006511A1C4: 48 8B 4F 70        mov         rcx,qword ptr [rdi+70h]
  000000006511A1C8: 48 85 C9           test        rcx,rcx
  000000006511A1CB: 74 28              je          000000006511A1F5
  000000006511A1CD: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006511A1D2: E8 D9 4E FB FF     call        00000000650CF0B0
  000000006511A1D7: 90                 nop
  000000006511A1D8: 48 8B D0           mov         rdx,rax
  000000006511A1DB: 48 8D 4F 78        lea         rcx,[rdi+78h]
  000000006511A1DF: E8 3C ED F9 FF     call        00000000650B8F20
  000000006511A1E4: 90                 nop
  000000006511A1E5: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006511A1EA: E8 11 91 F9 FF     call        00000000650B3300
  000000006511A1EF: 48 8D 47 78        lea         rax,[rdi+78h]
  000000006511A1F3: EB 0A              jmp         000000006511A1FF
  000000006511A1F5: 48 8D 47 78        lea         rax,[rdi+78h]
  000000006511A1F9: EB 04              jmp         000000006511A1FF
  000000006511A1FB: 48 8D 47 78        lea         rax,[rdi+78h]
  000000006511A1FF: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  000000006511A204: 48 83 C4 40        add         rsp,40h
  000000006511A208: 5F                 pop         rdi
  000000006511A209: C3                 ret

  Summary

      695000 .text
