Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146FBA0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  000000014146FBA5: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  000000014146FBAA: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  000000014146FBAF: 41 54              push        r12
  000000014146FBB1: 48 83 EC 40        sub         rsp,40h
  000000014146FBB5: 48 89 64 24 30     mov         qword ptr [rsp+30h],rsp
  000000014146FBBA: 45 8B E0           mov         r12d,r8d
  000000014146FBBD: 48 8B FA           mov         rdi,rdx
  000000014146FBC0: 48 8B F1           mov         rsi,rcx
  000000014146FBC3: 33 C0              xor         eax,eax
  000000014146FBC5: 48 85 C9           test        rcx,rcx
  000000014146FBC8: 0F 95 C0           setne       al
  000000014146FBCB: 85 C0              test        eax,eax
  000000014146FBCD: 75 27              jne         000000014146FBF6
  000000014146FBCF: E8 60 45 00 00     call        0000000141474134
  000000014146FBD4: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146FBDA: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014146FBE0: 45 33 C9           xor         r9d,r9d
  000000014146FBE3: 45 33 C0           xor         r8d,r8d
  000000014146FBE6: 33 D2              xor         edx,edx
  000000014146FBE8: 33 C9              xor         ecx,ecx
  000000014146FBEA: E8 79 09 00 00     call        0000000141470568
  000000014146FBEF: 33 C0              xor         eax,eax
  000000014146FBF1: E9 C9 00 00 00     jmp         000000014146FCBF
  000000014146FBF6: 33 C0              xor         eax,eax
  000000014146FBF8: 48 85 D2           test        rdx,rdx
  000000014146FBFB: 0F 95 C0           setne       al
  000000014146FBFE: 85 C0              test        eax,eax
  000000014146FC00: 75 27              jne         000000014146FC29
  000000014146FC02: E8 2D 45 00 00     call        0000000141474134
  000000014146FC07: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146FC0D: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014146FC13: 45 33 C9           xor         r9d,r9d
  000000014146FC16: 45 33 C0           xor         r8d,r8d
  000000014146FC19: 33 D2              xor         edx,edx
  000000014146FC1B: 33 C9              xor         ecx,ecx
  000000014146FC1D: E8 46 09 00 00     call        0000000141470568
  000000014146FC22: 33 C0              xor         eax,eax
  000000014146FC24: E9 96 00 00 00     jmp         000000014146FCBF
  000000014146FC29: 33 C0              xor         eax,eax
  000000014146FC2B: 38 02              cmp         byte ptr [rdx],al
  000000014146FC2D: 0F 95 C0           setne       al
  000000014146FC30: 85 C0              test        eax,eax
  000000014146FC32: 75 24              jne         000000014146FC58
  000000014146FC34: E8 FB 44 00 00     call        0000000141474134
  000000014146FC39: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146FC3F: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014146FC45: 45 33 C9           xor         r9d,r9d
  000000014146FC48: 45 33 C0           xor         r8d,r8d
  000000014146FC4B: 33 D2              xor         edx,edx
  000000014146FC4D: 33 C9              xor         ecx,ecx
  000000014146FC4F: E8 14 09 00 00     call        0000000141470568
  000000014146FC54: 33 C0              xor         eax,eax
  000000014146FC56: EB 67              jmp         000000014146FCBF
  000000014146FC58: E8 17 27 01 00     call        0000000141482374
  000000014146FC5D: 48 8B D8           mov         rbx,rax
  000000014146FC60: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014146FC65: 48 85 C0           test        rax,rax
  000000014146FC68: 75 0F              jne         000000014146FC79
  000000014146FC6A: E8 C5 44 00 00     call        0000000141474134
  000000014146FC6F: C7 00 18 00 00 00  mov         dword ptr [rax],18h
  000000014146FC75: 33 C0              xor         eax,eax
  000000014146FC77: EB 46              jmp         000000014146FCBF
  000000014146FC79: 80 3E 00           cmp         byte ptr [rsi],0
  000000014146FC7C: 75 22              jne         000000014146FCA0
  000000014146FC7E: E8 B1 44 00 00     call        0000000141474134
  000000014146FC83: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146FC89: 48 8D 15 0C 00 00  lea         rdx,[000000014146FC9Ch]
                    00
  000000014146FC90: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014146FC95: E8 56 28 01 00     call        00000001414824F0
  000000014146FC9A: 90                 nop
  000000014146FC9B: 90                 nop
  000000014146FC9C: 33 C0              xor         eax,eax
  000000014146FC9E: EB 1F              jmp         000000014146FCBF
  000000014146FCA0: 4C 8B C8           mov         r9,rax
  000000014146FCA3: 45 8B C4           mov         r8d,r12d
  000000014146FCA6: 48 8B D7           mov         rdx,rdi
  000000014146FCA9: 48 8B CE           mov         rcx,rsi
  000000014146FCAC: E8 FB 23 01 00     call        00000001414820AC
  000000014146FCB1: 48 8B F8           mov         rdi,rax
  000000014146FCB4: 48 8B CB           mov         rcx,rbx
  000000014146FCB7: E8 6C E0 FF FF     call        000000014146DD28
  000000014146FCBC: 48 8B C7           mov         rax,rdi
  000000014146FCBF: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  000000014146FCC4: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  000000014146FCC9: 48 8B 7C 24 68     mov         rdi,qword ptr [rsp+68h]
  000000014146FCCE: 48 83 C4 40        add         rsp,40h
  000000014146FCD2: 41 5C              pop         r12
  000000014146FCD4: C3                 ret

  Summary

     1E02000 .text
