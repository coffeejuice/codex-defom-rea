Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670729C0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670729C5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000670729CA: 57                 push        rdi
  00000000670729CB: 48 83 EC 20        sub         rsp,20h
  00000000670729CF: 33 F6              xor         esi,esi
  00000000670729D1: 8B DA              mov         ebx,edx
  00000000670729D3: 48 8B F9           mov         rdi,rcx
  00000000670729D6: 85 D2              test        edx,edx
  00000000670729D8: 79 04              jns         00000000670729DE
  00000000670729DA: 8B DE              mov         ebx,esi
  00000000670729DC: EB 02              jmp         00000000670729E0
  00000000670729DE: 75 3C              jne         0000000067072A1C
  00000000670729E0: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670729E3: F6 40 18 10        test        byte ptr [rax+18h],10h
  00000000670729E7: 75 33              jne         0000000067072A1C
  00000000670729E9: F0 83 05 5F E1 27  lock add    dword ptr [00000000672F0B50h],1
                    00 01
  00000000670729F1: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000670729F4: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670729F8: 75 08              jne         0000000067072A02
  00000000670729FA: 48 8B 0F           mov         rcx,qword ptr [rdi]
  00000000670729FD: E8 CE D6 FF FF     call        00000000670700D0
  0000000067072A02: 48 8D 05 47 E1 27  lea         rax,[00000000672F0B50h]
                    00
  0000000067072A09: 48 89 07           mov         qword ptr [rdi],rax
  0000000067072A0C: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000067072A11: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000067072A16: 48 83 C4 20        add         rsp,20h
  0000000067072A1A: 5F                 pop         rdi
  0000000067072A1B: C3                 ret
  0000000067072A1C: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067072A1F: 8B 08              mov         ecx,dword ptr [rax]
  0000000067072A21: 83 F9 01           cmp         ecx,1
  0000000067072A24: 75 1B              jne         0000000067072A41
  0000000067072A26: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000067072A29: 8B 41 04           mov         eax,dword ptr [rcx+4]
  0000000067072A2C: 3B D8              cmp         ebx,eax
  0000000067072A2E: 7F 11              jg          0000000067072A41
  0000000067072A30: F6 41 18 10        test        byte ptr [rcx+18h],10h
  0000000067072A34: 75 26              jne         0000000067072A5C
  0000000067072A36: 3B 59 08           cmp         ebx,dword ptr [rcx+8]
  0000000067072A39: 7D 21              jge         0000000067072A5C
  0000000067072A3B: D1 F8              sar         eax,1
  0000000067072A3D: 3B D8              cmp         ebx,eax
  0000000067072A3F: 7D 1B              jge         0000000067072A5C
  0000000067072A41: 8D 0C 1B           lea         ecx,[rbx+rbx]
  0000000067072A44: BA 20 00 00 00     mov         edx,20h
  0000000067072A49: E8 52 A0 FB FF     call        000000006702CAA0
  0000000067072A4E: 48 8B CF           mov         rcx,rdi
  0000000067072A51: 48 63 D0           movsxd      rdx,eax
  0000000067072A54: 48 D1 EA           shr         rdx,1
  0000000067072A57: E8 44 D7 FF FF     call        00000000670701A0
  0000000067072A5C: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067072A5F: 39 58 04           cmp         dword ptr [rax+4],ebx
  0000000067072A62: 7C 18              jl          0000000067072A7C
  0000000067072A64: 89 58 08           mov         dword ptr [rax+8],ebx
  0000000067072A67: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000067072A6A: 48 8D 41 1A        lea         rax,[rcx+1Ah]
  0000000067072A6E: 48 39 41 10        cmp         qword ptr [rcx+10h],rax
  0000000067072A72: 75 08              jne         0000000067072A7C
  0000000067072A74: 48 63 C3           movsxd      rax,ebx
  0000000067072A77: 66 89 74 41 1A     mov         word ptr [rcx+rax*2+1Ah],si
  0000000067072A7C: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000067072A81: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000067072A86: 48 83 C4 20        add         rsp,20h
  0000000067072A8A: 5F                 pop         rdi
  0000000067072A8B: C3                 ret

  Summary

      1A7000 .text
