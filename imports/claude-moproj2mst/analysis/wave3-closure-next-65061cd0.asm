Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065061CD0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  0000000065061CD5: 57                 push        rdi
  0000000065061CD6: 48 83 EC 30        sub         rsp,30h
  0000000065061CDA: 48 8B D9           mov         rbx,rcx
  0000000065061CDD: 48 8B F9           mov         rdi,rcx
  0000000065061CE0: FF 15 82 4B 63 00  call        qword ptr [0000000065696868h]
  0000000065061CE6: 48 8B 53 20        mov         rdx,qword ptr [rbx+20h]
  0000000065061CEA: 48 8B C8           mov         rcx,rax
  0000000065061CED: F6 42 0C 01        test        byte ptr [rdx+0Ch],1
  0000000065061CF1: 75 1B              jne         0000000065061D0E
  0000000065061CF3: 48 85 C9           test        rcx,rcx
  0000000065061CF6: 74 16              je          0000000065061D0E
  0000000065061CF8: 48 8B F9           mov         rdi,rcx
  0000000065061CFB: FF 15 67 4B 63 00  call        qword ptr [0000000065696868h]
  0000000065061D01: 48 8B C8           mov         rcx,rax
  0000000065061D04: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065061D08: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  0000000065061D0C: 74 E5              je          0000000065061CF3
  0000000065061D0E: E8 8D 53 FB FF     call        00000000650170A0
  0000000065061D13: 48 3B F8           cmp         rdi,rax
  0000000065061D16: 0F 84 B8 01 00 00  je          0000000065061ED4
  0000000065061D1C: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  0000000065061D20: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065061D23: C1 E9 0F           shr         ecx,0Fh
  0000000065061D26: 80 E1 01           and         cl,1
  0000000065061D29: 74 0E              je          0000000065061D39
  0000000065061D2B: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065061D2F: 80 78 0C 09        cmp         byte ptr [rax+0Ch],9
  0000000065061D33: 0F 84 9B 01 00 00  je          0000000065061ED4
  0000000065061D39: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065061D3D: 48 8B 90 88 00 00  mov         rdx,qword ptr [rax+0000000000000088h]
                    00
  0000000065061D44: 48 85 D2           test        rdx,rdx
  0000000065061D47: 74 1C              je          0000000065061D65
  0000000065061D49: 84 C9              test        cl,cl
  0000000065061D4B: 74 18              je          0000000065061D65
  0000000065061D4D: 48 8B 4A 10        mov         rcx,qword ptr [rdx+10h]
  0000000065061D51: 48 85 C9           test        rcx,rcx
  0000000065061D54: 74 0F              je          0000000065061D65
  0000000065061D56: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065061D5B: 48 83 C4 30        add         rsp,30h
  0000000065061D5F: 5F                 pop         rdi
  0000000065061D60: E9 4B E3 52 00     jmp         00000000655900B0
  0000000065061D65: 48 8B 43 08        mov         rax,qword ptr [rbx+8]

  Summary

      695000 .text
