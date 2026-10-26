Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067070E30: 48 8B C4           mov         rax,rsp
  0000000067070E33: 57                 push        rdi
  0000000067070E34: 48 83 EC 60        sub         rsp,60h
  0000000067070E38: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067070E41: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  0000000067070E45: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  0000000067070E49: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  0000000067070E4D: 0F 29 70 E8        movaps      xmmword ptr [rax-18h],xmm6
  0000000067070E51: 41 8B E9           mov         ebp,r9d
  0000000067070E54: 66 0F 28 F1        movapd      xmm6,xmm1
  0000000067070E58: 48 8B D9           mov         rbx,rcx
  0000000067070E5B: BE 01 00 00 00     mov         esi,1
  0000000067070E60: 41 8D 40 BF        lea         eax,[r8-41h]
  0000000067070E64: 33 FF              xor         edi,edi
  0000000067070E66: B9 40 00 00 00     mov         ecx,40h
  0000000067070E6B: 3C 19              cmp         al,19h
  0000000067070E6D: 0F 46 F9           cmovbe      edi,ecx
  0000000067070E70: 41 8D 40 BF        lea         eax,[r8-41h]
  0000000067070E74: 3C 19              cmp         al,19h
  0000000067070E76: 77 04              ja          0000000067070E7C
  0000000067070E78: 41 80 C0 20        add         r8b,20h
  0000000067070E7C: 41 0F BE C8        movsx       ecx,r8b
  0000000067070E80: 83 E9 65           sub         ecx,65h
  0000000067070E83: 74 0F              je          0000000067070E94
  0000000067070E85: 2B CE              sub         ecx,esi
  0000000067070E87: 74 0D              je          0000000067070E96
  0000000067070E89: 3B CE              cmp         ecx,esi
  0000000067070E8B: 75 09              jne         0000000067070E96
  0000000067070E8D: BE 02 00 00 00     mov         esi,2
  0000000067070E92: EB 02              jmp         0000000067070E96
  0000000067070E94: 33 F6              xor         esi,esi
  0000000067070E96: 45 33 C0           xor         r8d,r8d
  0000000067070E99: 41 8D 50 01        lea         edx,[r8+1]
  0000000067070E9D: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000067070EA2: E8 F9 C6 FD FF     call        000000006704D5A0
  0000000067070EA7: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000067070EAC: E8 8F 84 FD FF     call        0000000067049340
  0000000067070EB1: 48 8B C8           mov         rcx,rax
  0000000067070EB4: 89 7C 24 30        mov         dword ptr [rsp+30h],edi
  0000000067070EB8: 83 CF FF           or          edi,0FFFFFFFFh
  0000000067070EBB: 89 7C 24 28        mov         dword ptr [rsp+28h],edi
  0000000067070EBF: 89 74 24 20        mov         dword ptr [rsp+20h],esi
  0000000067070EC3: 44 8B CD           mov         r9d,ebp
  0000000067070EC6: 66 0F 28 D6        movapd      xmm2,xmm6
  0000000067070ECA: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000067070ECF: E8 9C DF FD FF     call        000000006704EE70
  0000000067070ED4: 48 8B F0           mov         rsi,rax
  0000000067070ED7: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000067070EDA: F0 83 01 01        lock add    dword ptr [rcx],1
  0000000067070EDE: 48 8B 13           mov         rdx,qword ptr [rbx]
  0000000067070EE1: F0 01 3A           lock add    dword ptr [rdx],edi
  0000000067070EE4: 75 08              jne         0000000067070EEE
  0000000067070EE6: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067070EE9: E8 E2 F1 FF FF     call        00000000670700D0
  0000000067070EEE: 48 8B 06           mov         rax,qword ptr [rsi]
  0000000067070EF1: 48 89 03           mov         qword ptr [rbx],rax
  0000000067070EF4: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000067070EF9: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067070EFD: 75 0F              jne         0000000067070F0E
  0000000067070EFF: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000067070F04: E8 C7 F1 FF FF     call        00000000670700D0
  0000000067070F09: 48 8B C3           mov         rax,rbx
  0000000067070F0C: EB 03              jmp         0000000067070F11
  0000000067070F0E: 48 8B C3           mov         rax,rbx
  0000000067070F11: 4C 8D 5C 24 60     lea         r11,[rsp+60h]
  0000000067070F16: 49 8B 5B 18        mov         rbx,qword ptr [r11+18h]
  0000000067070F1A: 49 8B 6B 20        mov         rbp,qword ptr [r11+20h]
  0000000067070F1E: 49 8B 73 28        mov         rsi,qword ptr [r11+28h]
  0000000067070F22: 0F 28 74 24 50     movaps      xmm6,xmmword ptr [rsp+50h]
  0000000067070F27: 49 8B E3           mov         rsp,r11
  0000000067070F2A: 5F                 pop         rdi
  0000000067070F2B: C3                 ret

  Summary

      1A7000 .text
