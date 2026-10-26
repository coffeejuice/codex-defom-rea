Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141496F50: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000141496F55: 57                 push        rdi
  0000000141496F56: 48 83 EC 20        sub         rsp,20h
  0000000141496F5A: 48 8B F9           mov         rdi,rcx
  0000000141496F5D: E8 8E 34 00 00     call        000000014149A3F0
  0000000141496F62: 48 8B D8           mov         rbx,rax
  0000000141496F65: 48 85 C0           test        rax,rax
  0000000141496F68: 75 0B              jne         0000000141496F75
  0000000141496F6A: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000141496F6F: 48 83 C4 20        add         rsp,20h
  0000000141496F73: 5F                 pop         rdi
  0000000141496F74: C3                 ret
  0000000141496F75: 48 8B 0D AC 2E 97  mov         rcx,qword ptr [0000000141E09E28h]
                    00
  0000000141496F7C: 48 8B D0           mov         rdx,rax
  0000000141496F7F: FF 15 BB D1 96 00  call        qword ptr [0000000141E04140h]
  0000000141496F85: 48 85 C0           test        rax,rax
  0000000141496F88: 0F 85 89 00 00 00  jne         0000000141497017
  0000000141496F8E: 48 8B 0D 13 F3 96  mov         rcx,qword ptr [0000000141E062A8h]
                    00
  0000000141496F95: 48 8B D3           mov         rdx,rbx
  0000000141496F98: FF 15 A2 D1 96 00  call        qword ptr [0000000141E04140h]
  0000000141496F9E: 48 85 C0           test        rax,rax
  0000000141496FA1: 75 74              jne         0000000141497017
  0000000141496FA3: 48 8B 0D 2E 21 97  mov         rcx,qword ptr [0000000141E090D8h]
                    00
  0000000141496FAA: 48 8B D3           mov         rdx,rbx
  0000000141496FAD: FF 15 8D D1 96 00  call        qword ptr [0000000141E04140h]
  0000000141496FB3: 48 85 C0           test        rax,rax
  0000000141496FB6: 75 5F              jne         0000000141497017
  0000000141496FB8: 48 8B 0D 91 20 97  mov         rcx,qword ptr [0000000141E09050h]
                    00
  0000000141496FBF: 48 8B D3           mov         rdx,rbx
  0000000141496FC2: FF 15 78 D1 96 00  call        qword ptr [0000000141E04140h]
  0000000141496FC8: 48 85 C0           test        rax,rax
  0000000141496FCB: 75 4A              jne         0000000141497017
  0000000141496FCD: 48 8B 0D 74 44 97  mov         rcx,qword ptr [0000000141E0B448h]
                    00
  0000000141496FD4: 48 8B D3           mov         rdx,rbx
  0000000141496FD7: FF 15 63 D1 96 00  call        qword ptr [0000000141E04140h]
  0000000141496FDD: 48 8B D3           mov         rdx,rbx
  0000000141496FE0: 48 85 C0           test        rax,rax
  0000000141496FE3: 74 20              je          0000000141497005
  0000000141496FE5: 48 8B 0D 5C 4C 97  mov         rcx,qword ptr [0000000141E0BC48h]
                    00
  0000000141496FEC: FF 15 4E D1 96 00  call        qword ptr [0000000141E04140h]
  0000000141496FF2: 48 85 C0           test        rax,rax
  0000000141496FF5: 74 20              je          0000000141497017
  0000000141496FF7: 48 8B C3           mov         rax,rbx
  0000000141496FFA: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000141496FFF: 48 83 C4 20        add         rsp,20h
  0000000141497003: 5F                 pop         rdi
  0000000141497004: C3                 ret
  0000000141497005: 48 8B 0D 34 4C 97  mov         rcx,qword ptr [0000000141E0BC40h]
                    00
  000000014149700C: FF 15 2E D1 96 00  call        qword ptr [0000000141E04140h]
  0000000141497012: 48 85 C0           test        rax,rax
  0000000141497015: 74 19              je          0000000141497030
  0000000141497017: 80 7F 48 00        cmp         byte ptr [rdi+48h],0
  000000014149701B: 74 13              je          0000000141497030
  000000014149701D: 80 7F 49 00        cmp         byte ptr [rdi+49h],0
  0000000141497021: 74 0D              je          0000000141497030
  0000000141497023: 48 8B 57 58        mov         rdx,qword ptr [rdi+58h]
  0000000141497027: 48 8B CB           mov         rcx,rbx
  000000014149702A: FF 15 D8 E5 96 00  call        qword ptr [0000000141E05608h]
  0000000141497030: 48 8B C3           mov         rax,rbx
  0000000141497033: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000141497038: 48 83 C4 20        add         rsp,20h
  000000014149703C: 5F                 pop         rdi
  000000014149703D: C3                 ret

  Summary

     1E02000 .text
