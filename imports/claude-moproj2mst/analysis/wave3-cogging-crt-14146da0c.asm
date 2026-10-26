Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146DA0C: 48 8B C4           mov         rax,rsp
  000000014146DA0F: 48 89 50 10        mov         qword ptr [rax+10h],rdx
  000000014146DA13: 48 89 48 08        mov         qword ptr [rax+8],rcx
  000000014146DA17: 4C 89 40 18        mov         qword ptr [rax+18h],r8
  000000014146DA1B: 4C 89 48 20        mov         qword ptr [rax+20h],r9
  000000014146DA1F: 53                 push        rbx
  000000014146DA20: 56                 push        rsi
  000000014146DA21: 57                 push        rdi
  000000014146DA22: 41 54              push        r12
  000000014146DA24: 48 83 EC 38        sub         rsp,38h
  000000014146DA28: 48 8B F9           mov         rdi,rcx
  000000014146DA2B: 33 DB              xor         ebx,ebx
  000000014146DA2D: 8B F3              mov         esi,ebx
  000000014146DA2F: 8B C3              mov         eax,ebx
  000000014146DA31: 48 3B CB           cmp         rcx,rbx
  000000014146DA34: 0F 95 C0           setne       al
  000000014146DA37: 3B C3              cmp         eax,ebx
  000000014146DA39: 75 27              jne         000000014146DA62
  000000014146DA3B: E8 F4 66 00 00     call        0000000141474134
  000000014146DA40: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146DA46: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000014146DA4B: 45 33 C9           xor         r9d,r9d
  000000014146DA4E: 45 33 C0           xor         r8d,r8d
  000000014146DA51: 33 D2              xor         edx,edx
  000000014146DA53: 33 C9              xor         ecx,ecx
  000000014146DA55: E8 0E 2B 00 00     call        0000000141470568
  000000014146DA5A: 83 C8 FF           or          eax,0FFFFFFFFh
  000000014146DA5D: E9 15 01 00 00     jmp         000000014146DB77
  000000014146DA62: 8B C3              mov         eax,ebx
  000000014146DA64: 48 3B D3           cmp         rdx,rbx
  000000014146DA67: 0F 95 C0           setne       al
  000000014146DA6A: 3B C3              cmp         eax,ebx
  000000014146DA6C: 75 27              jne         000000014146DA95
  000000014146DA6E: E8 C1 66 00 00     call        0000000141474134
  000000014146DA73: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146DA79: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000014146DA7E: 45 33 C9           xor         r9d,r9d
  000000014146DA81: 45 33 C0           xor         r8d,r8d
  000000014146DA84: 33 D2              xor         edx,edx
  000000014146DA86: 33 C9              xor         ecx,ecx
  000000014146DA88: E8 DB 2A 00 00     call        0000000141470568
  000000014146DA8D: 83 C8 FF           or          eax,0FFFFFFFFh
  000000014146DA90: E9 E2 00 00 00     jmp         000000014146DB77
  000000014146DA95: 4C 8D 64 24 70     lea         r12,[rsp+70h]
  000000014146DA9A: E8 F9 01 00 00     call        000000014146DC98
  000000014146DA9F: 90                 nop
  000000014146DAA0: F6 47 18 40        test        byte ptr [rdi+18h],40h
  000000014146DAA4: 0F 85 95 00 00 00  jne         000000014146DB3F
  000000014146DAAA: 48 8B CF           mov         rcx,rdi
  000000014146DAAD: E8 96 BF 00 00     call        0000000141479A48
  000000014146DAB2: 83 F8 FF           cmp         eax,0FFFFFFFFh
  000000014146DAB5: 74 2A              je          000000014146DAE1
  000000014146DAB7: 83 F8 FE           cmp         eax,0FFFFFFFEh
  000000014146DABA: 74 25              je          000000014146DAE1
  000000014146DABC: 48 63 D0           movsxd      rdx,eax
  000000014146DABF: 48 8B CA           mov         rcx,rdx
  000000014146DAC2: 48 C1 F9 05        sar         rcx,5
  000000014146DAC6: 4C 8D 05 33 66 BE  lea         r8,[0000000145054100h]
                    03
  000000014146DACD: 83 E2 1F           and         edx,1Fh
  000000014146DAD0: 48 6B D2 58        imul        rdx,rdx,58h
  000000014146DAD4: 49 03 14 C8        add         rdx,qword ptr [r8+rcx*8]
  000000014146DAD8: 48 8D 0D 11 0C 6F  lea         rcx,[0000000143B5E6F0h]
                    02
  000000014146DADF: EB 11              jmp         000000014146DAF2
  000000014146DAE1: 48 8D 0D 08 0C 6F  lea         rcx,[0000000143B5E6F0h]
                    02
  000000014146DAE8: 48 8B D1           mov         rdx,rcx
  000000014146DAEB: 4C 8D 05 0E 66 BE  lea         r8,[0000000145054100h]
                    03
  000000014146DAF2: F6 42 38 7F        test        byte ptr [rdx+38h],7Fh
  000000014146DAF6: 75 25              jne         000000014146DB1D
  000000014146DAF8: 83 F8 FF           cmp         eax,0FFFFFFFFh
  000000014146DAFB: 74 1A              je          000000014146DB17
  000000014146DAFD: 83 F8 FE           cmp         eax,0FFFFFFFEh
  000000014146DB00: 74 15              je          000000014146DB17
  000000014146DB02: 48 63 C8           movsxd      rcx,eax
  000000014146DB05: 48 8B C1           mov         rax,rcx
  000000014146DB08: 48 C1 F8 05        sar         rax,5
  000000014146DB0C: 83 E1 1F           and         ecx,1Fh
  000000014146DB0F: 48 6B C9 58        imul        rcx,rcx,58h
  000000014146DB13: 49 03 0C C0        add         rcx,qword ptr [r8+rax*8]
  000000014146DB17: F6 41 38 80        test        byte ptr [rcx+38h],80h
  000000014146DB1B: 74 22              je          000000014146DB3F
  000000014146DB1D: E8 12 66 00 00     call        0000000141474134
  000000014146DB22: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146DB28: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000014146DB2D: 45 33 C9           xor         r9d,r9d
  000000014146DB30: 45 33 C0           xor         r8d,r8d
  000000014146DB33: 33 D2              xor         edx,edx
  000000014146DB35: 33 C9              xor         ecx,ecx
  000000014146DB37: E8 2C 2A 00 00     call        0000000141470568
  000000014146DB3C: 83 CE FF           or          esi,0FFFFFFFFh
  000000014146DB3F: 3B F3              cmp         esi,ebx
  000000014146DB41: 75 2A              jne         000000014146DB6D
  000000014146DB43: 48 8B CF           mov         rcx,rdi
  000000014146DB46: E8 99 0E 01 00     call        000000014147E9E4
  000000014146DB4B: 8B D8              mov         ebx,eax
  000000014146DB4D: 4D 8B CC           mov         r9,r12
  000000014146DB50: 45 33 C0           xor         r8d,r8d
  000000014146DB53: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  000000014146DB58: 48 8B CF           mov         rcx,rdi
  000000014146DB5B: E8 E4 0F 01 00     call        000000014147EB44
  000000014146DB60: 8B F0              mov         esi,eax
  000000014146DB62: 48 8B D7           mov         rdx,rdi
  000000014146DB65: 8B CB              mov         ecx,ebx
  000000014146DB67: E8 4C 0F 01 00     call        000000014147EAB8
  000000014146DB6C: 90                 nop
  000000014146DB6D: 48 8B CF           mov         rcx,rdi
  000000014146DB70: E8 B3 01 00 00     call        000000014146DD28
  000000014146DB75: 8B C6              mov         eax,esi
  000000014146DB77: 48 83 C4 38        add         rsp,38h
  000000014146DB7B: 41 5C              pop         r12
  000000014146DB7D: 5F                 pop         rdi
  000000014146DB7E: 5E                 pop         rsi
  000000014146DB7F: 5B                 pop         rbx
  000000014146DB80: C3                 ret

  Summary

     1E02000 .text
