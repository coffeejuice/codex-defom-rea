Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067059CC0: 66 89 54 24 10     mov         word ptr [rsp+10h],dx
  0000000067059CC5: 53                 push        rbx
  0000000067059CC6: 48 83 EC 20        sub         rsp,20h
  0000000067059CCA: 44 0F B7 C2        movzx       r8d,dx
  0000000067059CCE: 48 8B D9           mov         rbx,rcx
  0000000067059CD1: 41 8B C0           mov         eax,r8d
  0000000067059CD4: 99                 cdq
  0000000067059CD5: 83 E2 3F           and         edx,3Fh
  0000000067059CD8: 03 C2              add         eax,edx
  0000000067059CDA: 83 E0 3F           and         eax,3Fh
  0000000067059CDD: 2B C2              sub         eax,edx
  0000000067059CDF: 48 63 D0           movsxd      rdx,eax
  0000000067059CE2: 48 8B 41 18        mov         rax,qword ptr [rcx+18h]
  0000000067059CE6: 81 7C 90 10 FF FF  cmp         dword ptr [rax+rdx*4+10h],7FFFFFFFh
                    FF 7F
  0000000067059CEE: 75 09              jne         0000000067059CF9
  0000000067059CF0: 8A 41 10           mov         al,byte ptr [rcx+10h]
  0000000067059CF3: 48 83 C4 20        add         rsp,20h
  0000000067059CF7: 5B                 pop         rbx
  0000000067059CF8: C3                 ret
  0000000067059CF9: 83 39 00           cmp         dword ptr [rcx],0
  0000000067059CFC: 74 27              je          0000000067059D25
  0000000067059CFE: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000067059D03: E8 A8 02 01 00     call        0000000067069FB0
  0000000067059D08: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000067059D0A: 0F A3 C1           bt          ecx,eax
  0000000067059D0D: 73 10              jae         0000000067059D1F
  0000000067059D0F: 45 33 C9           xor         r9d,r9d
  0000000067059D12: 44 38 4B 10        cmp         byte ptr [rbx+10h],r9b
  0000000067059D16: 0F 94 C0           sete        al
  0000000067059D19: 48 83 C4 20        add         rsp,20h
  0000000067059D1D: 5B                 pop         rbx
  0000000067059D1E: C3                 ret
  0000000067059D1F: 44 0F B7 44 24 38  movzx       r8d,word ptr [rsp+38h]
  0000000067059D25: 48 8B 53 08        mov         rdx,qword ptr [rbx+8]
  0000000067059D29: 45 33 C9           xor         r9d,r9d
  0000000067059D2C: 45 0F B7 D8        movzx       r11d,r8w
  0000000067059D30: 4C 63 52 08        movsxd      r10,dword ptr [rdx+8]
  0000000067059D34: 4D 8B C1           mov         r8,r9
  0000000067059D37: 4D 85 D2           test        r10,r10
  0000000067059D3A: 7E 20              jle         0000000067059D5C
  0000000067059D3C: 48 83 C2 12        add         rdx,12h
  0000000067059D40: 0F B7 42 FE        movzx       eax,word ptr [rdx-2]
  0000000067059D44: 41 8B CB           mov         ecx,r11d
  0000000067059D47: 2B C8              sub         ecx,eax
  0000000067059D49: 0F B7 02           movzx       eax,word ptr [rdx]
  0000000067059D4C: 3B C8              cmp         ecx,eax
  0000000067059D4E: 72 C2              jb          0000000067059D12
  0000000067059D50: 49 FF C0           inc         r8
  0000000067059D53: 48 83 C2 04        add         rdx,4
  0000000067059D57: 4D 3B C2           cmp         r8,r10
  0000000067059D5A: 7C E4              jl          0000000067059D40
  0000000067059D5C: 8A 43 10           mov         al,byte ptr [rbx+10h]
  0000000067059D5F: 48 83 C4 20        add         rsp,20h
  0000000067059D63: 5B                 pop         rbx
  0000000067059D64: C3                 ret

  Summary

      1A7000 .text
