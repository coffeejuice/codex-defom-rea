Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014147FC20: 48 8B C4           mov         rax,rsp
  000000014147FC23: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000014147FC27: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  000000014147FC2B: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  000000014147FC2F: 89 48 08           mov         dword ptr [rax+8],ecx
  000000014147FC32: 57                 push        rdi
  000000014147FC33: 48 83 EC 20        sub         rsp,20h
  000000014147FC37: 48 8B CA           mov         rcx,rdx
  000000014147FC3A: 48 8B DA           mov         rbx,rdx
  000000014147FC3D: E8 06 9E FF FF     call        0000000141479A48
  000000014147FC42: 8B 4B 18           mov         ecx,dword ptr [rbx+18h]
  000000014147FC45: 48 63 F0           movsxd      rsi,eax
  000000014147FC48: F6 C1 82           test        cl,82h
  000000014147FC4B: 75 17              jne         000000014147FC64
  000000014147FC4D: E8 E2 44 FF FF     call        0000000141474134
  000000014147FC52: C7 00 09 00 00 00  mov         dword ptr [rax],9
  000000014147FC58: 83 4B 18 20        or          dword ptr [rbx+18h],20h
  000000014147FC5C: 83 C8 FF           or          eax,0FFFFFFFFh
  000000014147FC5F: E9 34 01 00 00     jmp         000000014147FD98
  000000014147FC64: F6 C1 40           test        cl,40h
  000000014147FC67: 74 0D              je          000000014147FC76
  000000014147FC69: E8 C6 44 FF FF     call        0000000141474134
  000000014147FC6E: C7 00 22 00 00 00  mov         dword ptr [rax],22h
  000000014147FC74: EB E2              jmp         000000014147FC58
  000000014147FC76: 33 FF              xor         edi,edi
  000000014147FC78: F6 C1 01           test        cl,1
  000000014147FC7B: 74 19              je          000000014147FC96
  000000014147FC7D: 89 7B 08           mov         dword ptr [rbx+8],edi
  000000014147FC80: F6 C1 10           test        cl,10h
  000000014147FC83: 0F 84 89 00 00 00  je          000000014147FD12
  000000014147FC89: 48 8B 43 10        mov         rax,qword ptr [rbx+10h]
  000000014147FC8D: 83 E1 FE           and         ecx,0FFFFFFFEh
  000000014147FC90: 48 89 03           mov         qword ptr [rbx],rax
  000000014147FC93: 89 4B 18           mov         dword ptr [rbx+18h],ecx
  000000014147FC96: 8B 43 18           mov         eax,dword ptr [rbx+18h]
  000000014147FC99: 89 7B 08           mov         dword ptr [rbx+8],edi
  000000014147FC9C: 83 E0 EF           and         eax,0FFFFFFEFh
  000000014147FC9F: 83 C8 02           or          eax,2
  000000014147FCA2: 89 43 18           mov         dword ptr [rbx+18h],eax
  000000014147FCA5: A9 0C 01 00 00     test        eax,10Ch
  000000014147FCAA: 75 2F              jne         000000014147FCDB
  000000014147FCAC: E8 D3 DE FE FF     call        000000014146DB84
  000000014147FCB1: 48 83 C0 30        add         rax,30h
  000000014147FCB5: 48 3B D8           cmp         rbx,rax
  000000014147FCB8: 74 0E              je          000000014147FCC8
  000000014147FCBA: E8 C5 DE FE FF     call        000000014146DB84
  000000014147FCBF: 48 83 C0 60        add         rax,60h
  000000014147FCC3: 48 3B D8           cmp         rbx,rax
  000000014147FCC6: 75 0B              jne         000000014147FCD3
  000000014147FCC8: 8B CE              mov         ecx,esi
  000000014147FCCA: E8 21 05 01 00     call        00000001414901F0
  000000014147FCCF: 3B C7              cmp         eax,edi
  000000014147FCD1: 75 08              jne         000000014147FCDB
  000000014147FCD3: 48 8B CB           mov         rcx,rbx
  000000014147FCD6: E8 E5 AA 00 00     call        000000014148A7C0
  000000014147FCDB: F7 43 18 08 01 00  test        dword ptr [rbx+18h],108h
                    00
  000000014147FCE2: 0F 84 8D 00 00 00  je          000000014147FD75
  000000014147FCE8: 8B 2B              mov         ebp,dword ptr [rbx]
  000000014147FCEA: 48 8B 53 10        mov         rdx,qword ptr [rbx+10h]
  000000014147FCEE: 2B 6B 10           sub         ebp,dword ptr [rbx+10h]
  000000014147FCF1: 48 8D 42 01        lea         rax,[rdx+1]
  000000014147FCF5: 48 89 03           mov         qword ptr [rbx],rax
  000000014147FCF8: 8B 43 24           mov         eax,dword ptr [rbx+24h]
  000000014147FCFB: FF C8              dec         eax
  000000014147FCFD: 3B EF              cmp         ebp,edi
  000000014147FCFF: 89 43 08           mov         dword ptr [rbx+8],eax
  000000014147FD02: 7E 19              jle         000000014147FD1D
  000000014147FD04: 44 8B C5           mov         r8d,ebp
  000000014147FD07: 8B CE              mov         ecx,esi
  000000014147FD09: E8 9A 57 00 00     call        00000001414854A8
  000000014147FD0E: 8B F8              mov         edi,eax
  000000014147FD10: EB 57              jmp         000000014147FD69
  000000014147FD12: 83 C9 20           or          ecx,20h
  000000014147FD15: 89 4B 18           mov         dword ptr [rbx+18h],ecx
  000000014147FD18: E9 3F FF FF FF     jmp         000000014147FC5C
  000000014147FD1D: 83 FE FF           cmp         esi,0FFFFFFFFh
  000000014147FD20: 74 23              je          000000014147FD45
  000000014147FD22: 83 FE FE           cmp         esi,0FFFFFFFEh
  000000014147FD25: 74 1E              je          000000014147FD45
  000000014147FD27: 48 8B CE           mov         rcx,rsi
  000000014147FD2A: 48 8B C6           mov         rax,rsi
  000000014147FD2D: 48 8D 15 CC 43 BD  lea         rdx,[0000000145054100h]
                    03
  000000014147FD34: 83 E1 1F           and         ecx,1Fh
  000000014147FD37: 48 C1 F8 05        sar         rax,5
  000000014147FD3B: 48 6B C9 58        imul        rcx,rcx,58h
  000000014147FD3F: 48 03 0C C2        add         rcx,qword ptr [rdx+rax*8]
  000000014147FD43: EB 07              jmp         000000014147FD4C
  000000014147FD45: 48 8D 0D A4 E9 6D  lea         rcx,[0000000143B5E6F0h]
                    02
  000000014147FD4C: F6 41 08 20        test        byte ptr [rcx+8],20h
  000000014147FD50: 74 17              je          000000014147FD69
  000000014147FD52: 33 D2              xor         edx,edx
  000000014147FD54: 8B CE              mov         ecx,esi
  000000014147FD56: 44 8D 42 02        lea         r8d,[rdx+2]
  000000014147FD5A: E8 99 05 01 00     call        00000001414902F8
  000000014147FD5F: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  000000014147FD63: 0F 84 EF FE FF FF  je          000000014147FC58
  000000014147FD69: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  000000014147FD6D: 8A 44 24 30        mov         al,byte ptr [rsp+30h]
  000000014147FD71: 88 01              mov         byte ptr [rcx],al
  000000014147FD73: EB 16              jmp         000000014147FD8B
  000000014147FD75: BD 01 00 00 00     mov         ebp,1
  000000014147FD7A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000014147FD7F: 8B CE              mov         ecx,esi
  000000014147FD81: 44 8B C5           mov         r8d,ebp
  000000014147FD84: E8 1F 57 00 00     call        00000001414854A8
  000000014147FD89: 8B F8              mov         edi,eax
  000000014147FD8B: 3B FD              cmp         edi,ebp
  000000014147FD8D: 0F 85 C5 FE FF FF  jne         000000014147FC58
  000000014147FD93: 0F B6 44 24 30     movzx       eax,byte ptr [rsp+30h]
  000000014147FD98: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  000000014147FD9D: 48 8B 6C 24 40     mov         rbp,qword ptr [rsp+40h]
  000000014147FDA2: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  000000014147FDA7: 48 83 C4 20        add         rsp,20h
  000000014147FDAB: 5F                 pop         rdi
  000000014147FDAC: C3                 ret

  Summary

     1E02000 .text
